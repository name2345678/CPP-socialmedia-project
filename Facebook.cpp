#include "Facebook.h"
#include "User.h"
#include "Group.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <filesystem>
#include <string>
#include <typeinfo>

using std::cout;
using std::cin;
using std::chrono::system_clock;
using std::string;
using std::vector;

Facebook::Facebook() { 
	loadUsersFromFile(USERS_DATA_FILE_NAME + DATA_FILES_EXTENTION_SUFFIX);
	loadGroupsFromFile(GROUPS_DATA_FILE_NAME + DATA_FILES_EXTENTION_SUFFIX);
	loadUserConnectionsFromFile(USER_CONNECTIONS_DATA_FILE_NAME + DATA_FILES_EXTENTION_SUFFIX);
	loadGroupConnectionsFromFile(GROUP_CONNECTIONS_DATA_FILE_NAME + DATA_FILES_EXTENTION_SUFFIX);
}

Facebook::~Facebook() {
	saveSystemData();
	for (User* user : users)
		delete user;
	for (Group* group : groups)
		delete group;
}

size_t Facebook::getUserAmount() const noexcept {
	if (users.empty())
		return 0;
	return users.size();
}

size_t Facebook::getGroupAmount() const noexcept {
	if (groups.empty())
		return 0;
	return groups.size();
}

// searches for user by name. returns pointer if found and nullptr else.
User* Facebook::findUserByName(const string& name) const noexcept{
	for (User* user : users) {
		if (user->getName() == name)
			return user;
	}
	return nullptr;
}

// searches for group by name. returns pointer if found and nullptr else.
Group* Facebook::findGroupByName(const string& name) const noexcept{
	for (Group* group : groups) {
		if (group->getName() == name)
			return group;
	}
	return nullptr;
}

// searches for a user (via index) and sends the poiner back. if out of bounds - sends exception
User* Facebook::accessUserByIndex(const size_t index) const{
	// if negative number is sent, it would be large when unsigned anyways (left most bit would be on), so if statement to throw exception would work.
	if (index >= users.size())
		throw std::out_of_range("Chosen user index is out of bounds");
	return users[index];
}

// searches for a group (via index) and sends the poiner back.  if out of bounds - sends exception
Group* Facebook::accessGroupByIndex(const size_t index) const{
	// if negative number is sent, it would be large when unsigned anyways (left most bit would be on), so if statement to throw exception would work.
	if (index >= groups.size())
		throw std::out_of_range("Chosen group index is out of bounds");
	return groups[index];
}


/*--------------------------------------------------
Object creation functions
--------------------------------------------------*/
// Creates a new user in heap. if name already exists returns throws exception. returns a pointer to the created user
User* Facebook::createUser(const string& name, const system_clock::time_point birthDate) {
	// if user already exists throw exception
	if (findUserByName(name) != nullptr)
		throw std::invalid_argument("User \"" + name + "\" already exists in system");

	User* newUser = new User(name, birthDate);
	users.push_back(newUser);
	return newUser;
}

// Creates a new group in heap. if (name) alrady exists returns false, otherwise returns true 
Group* Facebook::createGroup(const string& name) {
	// if group already exists throw exception
	if (findGroupByName(name) != nullptr)
		throw std::invalid_argument("Group \"" + name + "\" already exists in system");

	Group* newGroup = new Group(name);
	groups.push_back(newGroup);
	return newGroup;
}

std::vector<User*> Facebook::getPotentialUserConnectionsFor(const Profile* profile) {
	vector<User*>potential_connections;
	size_t connections = profile->getUserConnectionsAmount();
	if (users.size() > connections + 1) { // to prevent underflow
		potential_connections.reserve(users.size() - 1 - connections);
	} // reserve general size minus the user itself and his friends
	for (auto potential_user : users) {
		if (potential_user != profile && !profile->isUserInConnections(potential_user))
			potential_connections.push_back(potential_user);
	}
	return potential_connections;
}

std::vector<Group*> Facebook::getPotentialGroupsFor(const User* user) {
	vector<Group*>potential_groups;
	size_t userGroupsAmount = user->getGroupAmount();
	if (groups.size() > userGroupsAmount) { // to prevent underflow
		potential_groups.reserve(groups.size() - userGroupsAmount);
	} // reserve general size minus the user's groups
	for (auto group : groups) {
		if (!user->isInGroup(group))
			potential_groups.push_back(group);
	}
	return potential_groups;
}

/*----------------
* Object deletion
-----------------*/
void Facebook::deleteUser(User* user) {	
	if (user == nullptr)
		throw std::invalid_argument("ERROR: deleteUser() received nullptr");
	std::erase(users, user);
	delete user;
}

void Facebook::deleteGroup(Group* group) {
	if (group == nullptr)
		throw std::invalid_argument("ERROR: deleteGroup() received nullptr");
	std::erase(groups, group);
	delete group;
}

/*-----------
* Files
-----------*/
void Facebook::saveSystemData() const {
	saveUsersToFile(USERS_DATA_FILE_NAME + DATA_FILES_EXTENTION_SUFFIX);
	saveGroupsToFile(GROUPS_DATA_FILE_NAME + DATA_FILES_EXTENTION_SUFFIX);
	saveUserConnectionsToFile(USER_CONNECTIONS_DATA_FILE_NAME + DATA_FILES_EXTENTION_SUFFIX);
	saveGroupConnectionsToFile(GROUP_CONNECTIONS_DATA_FILE_NAME + DATA_FILES_EXTENTION_SUFFIX);
}

void Facebook::loadSystemData() {
	loadUsersFromFile(USERS_DATA_FILE_NAME + DATA_FILES_EXTENTION_SUFFIX);
	loadGroupsFromFile(GROUPS_DATA_FILE_NAME + DATA_FILES_EXTENTION_SUFFIX);
	loadUserConnectionsFromFile(USER_CONNECTIONS_DATA_FILE_NAME + DATA_FILES_EXTENTION_SUFFIX);
	loadGroupConnectionsFromFile(GROUP_CONNECTIONS_DATA_FILE_NAME + DATA_FILES_EXTENTION_SUFFIX);
}

// Consumes text in a file until a delimiter. by default consumes until the record delimiter ('\x1E') to signal the end of a record
void Facebook::consumeRecordFromFile(std::ifstream& inFile, char delim) {
	inFile.ignore(std::numeric_limits<std::streamsize>::max(), delim);
}

std::string Facebook::readStringFromFile(std::ifstream& inFile, char delim) {
	std::string value;
	std::getline(inFile, value, delim);
	return value;
}

void Facebook::saveUsersToFile(const std::string& fileName) const {
	std::ofstream outFile(fileName);
	if (!outFile.is_open())
		throw std::runtime_error("Failed to open the file \""+fileName+"\" for writing");
	 
	outFile << static_cast<int>(getUserAmount()) << UNIT_DELIMITER << RECORD_DELIMITER;
	for (User* user : users) {
		user->saveToFile(outFile);
	}
	outFile.close(); 
}

void Facebook::saveGroupsToFile(const std::string& fileName) const {
	std::ofstream outFile(fileName);
	if (!outFile.is_open())
		throw std::runtime_error("Failed to open the file \"" + fileName + "\" for writing");
	outFile << static_cast<int>(getGroupAmount()) << UNIT_DELIMITER << RECORD_DELIMITER;
	for (Group* group : groups) {
		group->saveToFile(outFile);
	}
	outFile.close();
}

void Facebook::saveUserConnectionsToFile(const std::string& fileName) const {
	std::ofstream outFile(fileName);
	if (!outFile.is_open()) 
		throw std::runtime_error("Failed to open the file \"" + fileName + "\" for writing");
	for (User* user : users) {
		user->saveUserConnectionsToFile(outFile);
	}
	outFile.close();
}

void Facebook::saveGroupConnectionsToFile(const std::string& fileName) const {
	std::ofstream outFile(fileName);
	if (!outFile.is_open())
		throw std::runtime_error("Failed to open the file \"" + fileName + "\" for writing");
	for (Group* group : groups) {
		group->saveUserConnectionsToFile(outFile);
	}
	outFile.close();
}


void Facebook::loadUsersFromFile(const std::string& fileName) {
	if (!std::filesystem::exists(fileName)) // if filename doesn't exist this is a fresh run
		return;
	std::ifstream inFile(fileName);
	if (!inFile.is_open())
		throw std::runtime_error("Failed to open the file \"" + fileName + "\" for reading");
	if (!users.empty()) 
		throw std::runtime_error("Loading file \"" + fileName + "\"failed - system already has users");
	
	auto userAmount = readNumFromFile(inFile, UNIT_DELIMITER);
	consumeRecordFromFile(inFile); // finished reading the user list data
	users.reserve(userAmount);

	for (size_t i = 0; i < userAmount; i++) {
		long long birthDateTicks = readNumFromFile<long long>(inFile);
		string name = readStringFromFile(inFile);
		system_clock::time_point birthDate{system_clock::duration(birthDateTicks)}; // recreate date from ticks
		User* newUser = createUser(name, birthDate);
		int postAmount = readNumFromFile(inFile);
		consumeRecordFromFile(inFile); // finished reading the user data
		newUser->loadPostsFromFile(inFile, postAmount);
	}
	inFile.close();
}

void Facebook::loadGroupsFromFile(const std::string& fileName) {
	if (!std::filesystem::exists(fileName)) // if filename doesn't exist this is a fresh run
		return;
	std::ifstream inFile(fileName);
	if (!inFile.is_open())
		throw std::runtime_error("Failed to open the file \"" + fileName + "\" for reading");
	if (!groups.empty()) 
		throw std::runtime_error("Loading file \"" + fileName + "\"failed - system already has groups");
	
	auto groupAmount = readNumFromFile(inFile);
	consumeRecordFromFile(inFile);
	groups.reserve(groupAmount);

	for (size_t i = 0; i < groupAmount; i++) {
		string name = readStringFromFile(inFile);
		Group* newGroup = createGroup(name);
		int postAmount = readNumFromFile(inFile);
		consumeRecordFromFile(inFile); // finished reading group data
		newGroup->loadPostsFromFile(inFile, postAmount);
	}
	inFile.close();
}

void Facebook::loadUserConnectionsFromFile(const std::string& fileName) {
	if (!std::filesystem::exists(fileName)) // if filename doesn't exist this is a fresh run
		return;
	std::ifstream inFile(fileName);
	if (!inFile.is_open())
		throw std::runtime_error("Failed to open the file \"" + fileName + "\" for reading");


	if (users.empty()){
		// (AI USAGE for .peek) if file isn't empty but vector is return error. else if vector is just empty return
		if (inFile.peek() != std::char_traits<char>::eof())  
			throw std::logic_error("ERROR: Tried to load connections, but users array is empty! Check load order or data corruption.");
		return;
	}

	size_t userAmount = getUserAmount();
	for (size_t j = 0; j < userAmount; j++) {
		string curName = readStringFromFile(inFile);
		User* curUser = findUserByName(curName);

		if (curUser == nullptr) 
			throw std::runtime_error("ERROR: Failed to find user \"" + curName + "\" While loading friend connections in file \"" + fileName + "\"");

		int curFriendAmount = readNumFromFile(inFile);
		consumeRecordFromFile(inFile); // finished reading friends list data
		if (curFriendAmount  >= userAmount) 
			throw std::runtime_error("ERROR: Failed to read friend amount value when loading user \"" + curName + "\" from file \"" + fileName + "\"");
		
		for (size_t i = 0; i < curFriendAmount; i++) {
			string curFriendName = readStringFromFile(inFile);
			User* newFriend = findUserByName(curFriendName);
			if (newFriend == nullptr) 
				throw std::runtime_error("ERROR: Failed to find friend by the username of \"" + curFriendName + "\" While loading friend connections in file \"" + fileName + "\" for user \""+curName+"\"");
		
			if(!newFriend->isUserInConnections(curUser))
				curUser->addUserConnection(newFriend);
			
		}
		consumeRecordFromFile(inFile);
	}
	inFile.close();
}

void Facebook::loadGroupConnectionsFromFile(const std::string& fileName) {
	if (!std::filesystem::exists(fileName)) // if filename doesn't exist this is a fresh run
		return;
	std::ifstream inFile(fileName);
	if (!inFile.is_open())
		throw std::runtime_error("Failed to open the file \"" + fileName + "\" for reading");
	if (users.empty()) {
		if (inFile.peek() != std::char_traits<char>::eof()) 
			throw std::logic_error("ERROR: Tried to load connections, but groups array is empty! Check load order or data corruption.");
		return;
	}
	size_t userAmount = getUserAmount();
	size_t groupAmount = getGroupAmount();
	for (size_t j = 0; j < groupAmount; j++) {
		string curName = readStringFromFile(inFile);
		Group* curGroup = findGroupByName(curName);
		if (curGroup == nullptr) 
			throw std::runtime_error("ERROR: Failed to find group \"" + curName + "\" While loading friend connections in file \"" + fileName + "\"");

		int curFriendAmount = readNumFromFile(inFile);
		consumeRecordFromFile(inFile);
		for (int i = 0; i < curFriendAmount; ++i) {
			string curFriendName = readStringFromFile(inFile);
			User* newFriend = findUserByName(curFriendName);
			if (newFriend == nullptr) 
				throw std::runtime_error("ERROR: Failed to find friend by the username of \"" + curFriendName + "\" While loading friend connections in file \"" + fileName + "\" for group \"" + curName + "\"");
			
			curGroup->addUserConnection(newFriend);
		}
		consumeRecordFromFile(inFile);
	}
	inFile.close();
}

