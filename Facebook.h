#pragma once

#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include "Post.h"

class User;
class Group;
class Profile;

class Facebook
{	
	// file names
	static const inline std::string DATA_FILES_EXTENTION_SUFFIX = ".txt";
	static const inline std::string USERS_DATA_FILE_NAME = "facebook_users_data";
	static const inline std::string GROUPS_DATA_FILE_NAME = "facebook_groups_data";
	static const inline std::string USER_CONNECTIONS_DATA_FILE_NAME = "facebook_user_connections_data";
	static const inline std::string GROUP_CONNECTIONS_DATA_FILE_NAME = "facebook_group_connections_data";


	// vectors
	std::vector<User*> users;
	std::vector<Group*> groups;

	/*-----
	* Files
	------*/

	/* save */
	void saveUsersToFile(const std::string& fileName) const;
	void saveGroupsToFile(const std::string& fileName) const;
	void saveUserConnectionsToFile(const std::string& fileName) const;
	void saveGroupConnectionsToFile(const std::string& fileName) const;
	/* load */
	void loadUsersFromFile(const std::string& fileName);
	void loadGroupsFromFile(const std::string& fileName);
	void loadUserConnectionsFromFile(const std::string& fileName);
	void loadGroupConnectionsFromFile(const std::string& fileName);

public:
	// file delimiters
	static const inline char UNIT_DELIMITER = '\x1F';
	static const inline char RECORD_DELIMITER = '\x1E';

	/*--------------
	* ctor / dtor
	---------------*/
	explicit Facebook();
	~Facebook();

	// Facebook is unique so no need for copying
	Facebook(const Facebook& other) = delete;
	Facebook(Facebook&& other) = delete;
	Facebook& operator=(const Facebook& other) = delete;
	Facebook& operator=(Facebook&& other) = delete;

	/*----------------
	* Object creation
	----------------*/
	User* createUser(const std::string& name, const std::chrono::system_clock::time_point birthDate);
	Group* createGroup(const std::string& name);

	/*----------------
	* Object deletion
	-----------------*/
	void deleteUser(User* user);
	void deleteGroup(Group* group);

	/*---------
	* Getters
	----------*/
	size_t getUserAmount() const noexcept;
	size_t getGroupAmount() const noexcept;
	User* accessUserByIndex(const size_t index) const;
	Group* accessGroupByIndex(const size_t index) const;
	User* findUserByName(const std::string& name) const noexcept;
	Group* findGroupByName(const std::string& name) const noexcept;

	/*---------
	* Filters
	----------*/
	std::vector<User*> getPotentialUserConnectionsFor(const Profile* profile);
	std::vector<Group*> getPotentialGroupsFor(const User* user);

	/*----------
	* Files
	----------*/
	void saveSystemData() const;
	void loadSystemData();

	/* static helpers */
	// reads a num from file until given delimiter. clears buffer after from given delimiter. by default reads an int
	template<typename NumType = int>
	static NumType readNumFromFile(std::ifstream& inFile, char delim = UNIT_DELIMITER) {
		if (!inFile.is_open())
			throw std::runtime_error("ERROR: Failed to read int - file isn't open");
		NumType value;
		if (!(inFile >> value))
			throw std::invalid_argument("ERROR: failed to read / type read isn't int");
		inFile.ignore(std::numeric_limits<std::streamsize>::max(), delim);
		return value;
	}
	static std::string readStringFromFile(std::ifstream& inFile, char delim = UNIT_DELIMITER);
	// Consumes text in a file until a delimiter. by default consumes until the record delimiter ('\x1E') to signal the end of a record
	static void consumeRecordFromFile(std::ifstream& inFile, char delim = RECORD_DELIMITER);
	
};

