#include "User.h"
#include "Post.h"
#include "Group.h"
#include "Facebook.h"
#include "Profile.h"
#include <algorithm>
#include <iostream>

using std::chrono::system_clock;
using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::find;

// dtor
User::~User() {
	// removes connections to avoid dangaling pointers
	for (User* _friend : userConnections) {
		try { 
			_friend->removeUserConnection(this, false);
		} 
		catch (const std::invalid_argument&){
			// empty try - catch to avoid stopping the destructor if user isn't found in friend's friend list
		}
	}
	for (Group* group : groups) {
		try {
			group->removeUserConnection(this, false);
		}
		catch (const std::invalid_argument&) {
			// empty try - catch to avoid stopping the destructor if user isn't found in friend's friend list
		}
	}
}

/*-------
* Getters
-------*/
const Group* User::accessGroupByIndex(size_t index) const {
	return groups.at(index);
}

size_t User::getGroupAmount() const noexcept {
	if (groups.empty())
		return 0;
	return groups.size();
}


/*----------------------------------------
adding functions
----------------------------------------*/

// adds friend to users' friend list. if friend already exists in the list - returns false. else, adds the friend and returns true. 
void User::addUserConnection(User* user, bool addToOther) {
	if (user == nullptr)
		throw std::invalid_argument("Cannot add a null user as a friend");
	// if user is already a friend throw exception
	if (isUserInConnections(user)) 
		throw std::invalid_argument("User \"" + user->name + "\" is already friends with " + this->name);

	userConnections.push_back(user);
	if(addToOther)
		user->addUserConnection(this, false);
}

void User::addGroup(Group* group, bool addToOther) {
	if (group == nullptr)
		throw std::invalid_argument("Cannot add a null group");
	if(isInGroup(group))
		throw std::invalid_argument("Current user " + this->name + " is already in group \"" + group->getName()+"\"");

	groups.push_back(group);
	if(addToOther)
		group->addUserConnection(this,false);
}


/*----------------------------------------
removing functions
----------------------------------------*/

/* removes friend, by default removes user from friend's friend list too (can be changed).*/
const User* User::removeUserConnection(User* user, bool removeFromOther) {
	// searches for friend in user's friend list. if not found throw exception
	if (user == nullptr)
		throw std::invalid_argument("Can't remove nullptr");
	if(!isUserInConnections(user))
		throw std::invalid_argument("Current user " + this->name + " isn't friends with \"" +user->name + "\"");

	std::erase(userConnections,user); // Works in c++20 and after apperantly
	if (removeFromOther)
		user->removeUserConnection(this, false);
	return user;
}

/* removes friend via index.
* size_t is unsigned - so if function gets a negative index underflow will make it really big anyways and the if statment would catch it. */
const User* User::removeUserConnection(size_t index, bool removeFromOther) {
	if (index >= getUserConnectionsAmount())
		throw std::out_of_range("Chosen user is out of bounds");
	User* friendToErase = userConnections.at(index);
	userConnections.erase(userConnections.begin() + index);
	
	if (removeFromOther)
		friendToErase->removeUserConnection(this, false);
	return friendToErase;
}


/* removes group, by default removes user from groups's user list too (can be changed). */
void User::removeGroup(Group* group, bool removeFromOther) {
	// searches for group in user's group list. if not found throw exception
	if(!isInGroup(group))
		throw std::invalid_argument("Current user " + this->name + " isn't in group \"" + group->getName() + "\"");

	std::erase(groups, group);
	if (removeFromOther)
		group->removeUserConnection(this, false);
}

void User::removeGroup(size_t index, bool removeFromOther) {
	if (index >= groups.size())
		throw std::out_of_range("Chosen group is out of bounds");
	Group* groupToErase = groups.at(index);
	groups.erase(groups.begin() + index);
	if (removeFromOther)
		groupToErase->removeUserConnection(this, false);
}

/*------------------------
* boolean "is" functions
-------------------------*/
// Checks if the user is in a given group
bool User::isInGroup(Group* group) const{
	auto _group = find(groups.begin(), groups.end(), group);
	if (_group == groups.end())
		return false;
	return true;
}

/*------------------
print functions
--------------------*/
void User::print(std::ostream& os) const{
	os << "UserName: \"" << name << "\"   -   BirthDate: " << std::format("{:%Y-%m-%d}", birthDate);
}

/*-------
* Files
-------*/
void User::saveToFile(std::ofstream& outFile) const {
	outFile << birthDate.time_since_epoch().count() << Facebook::UNIT_DELIMITER;
	Profile::saveToFile(outFile);
}