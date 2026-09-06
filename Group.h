#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Profile.h"

class User;
class Post;

class Group : public Profile
{
public:
	/*-------------
	* ctor / dtor
	---------------*/
	Group(const std::string & _name) :
		Profile(_name){};

	~Group();

	// no need to copy Groups since each group is unique
	Group(const Group& other) = delete;
	Group& operator=(const Group& other) = delete;
	Group(Group&& other) = delete;
	Group& operator=(Group&& other) = delete;

	/*------
	* Print
	-------*/
	void print(std::ostream& os) const override;

	/*------
	* Add
	-------*/
	void addUserConnection(User* user, bool addToOther = true) override;

	/*--------
	* remove
	---------*/
	const User* removeUserConnection(User* user,  bool removeFromOther = true) override;
	const User* removeUserConnection(size_t index, bool removeFromOther = true) override;

	// files
	void saveToFile(std::ofstream& outFile) const;
};

