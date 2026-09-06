#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include "Post.h"
#include "Profile.h"

class Group;

class User : public Profile
{
	// user info
	std::chrono::system_clock::time_point birthDate;

	std::vector<Group*> groups;

public:

	/*---------------
	ctor / dtor 
	-----------------*/
	User(const std::string& _name,const std::chrono::system_clock::time_point _birthDate) :
		Profile(_name),
		birthDate(_birthDate){};

	//dtor
	~User();

	// copy ctor - each user is unique so no need for copying
	User(const User& other) = delete;
	User& operator=(const User& other) = delete;
	User(User&& other) = delete;
	User& operator=(User&& other) = delete;

	/*----------
	*Booleans
	-----------*/
	bool isInGroup(Group* group) const;

	/*----------------
	get functions
	------------------*/
	constexpr std::chrono::system_clock::time_point getBirthDate() const { return birthDate; };

	size_t getGroupAmount() const noexcept;

	const Group* accessGroupByIndex(size_t index) const;


	/*-----------------------
	Console interface
	------------------------*/
	void print(std::ostream& os) const override;
	 
	/*------------------
	adding functions
	--------------------*/
	void addUserConnection(User* user, bool addToOther = true) override;
	void addGroup(Group* group, bool addToOther = true);


	/*------------------------------------------------
	removing functions. by default removes both ways
	--------------------------------------------------*/
	const User* removeUserConnection(User* user, bool removeFromOther = true) override;
	const User* removeUserConnection(size_t index, bool removeFromOther = true) override;

	void removeGroup(Group* group, bool removeFromOther = true);
	void removeGroup(size_t index, bool removeFromOther = true);

	/*-------
	* Files
	--------*/
	void saveToFile(std::ofstream& outFile) const override;
};

