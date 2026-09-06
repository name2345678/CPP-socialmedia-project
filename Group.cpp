#include "Group.h"
#include "User.h"
#include "Post.h"
#include "Facebook.h"
#include "Profile.h"
#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::cin;
using std::vector;
using std::find;

// dtor
Group::~Group() {
	for (User* user : userConnections)
		try {
			user->removeGroup(this, false);
		}
	catch (const std::invalid_argument&) {
		// empty try - catch to avoid stopping the destructor if group isn't found in user's group list
	}
}

/*--------------------
* print functions
----------------------*/
void Group::print(std::ostream& os) const {
	os << "Group: \"" << name << "\"";
}

/*--------------
* add functions
----------------*/

void Group::addUserConnection(User* user, bool addToOther) {
	if (isUserInConnections(user))
		throw std::invalid_argument("User \""+user->getName()+"\" is already in group "+name);

	userConnections.push_back(user);
	if(addToOther)
		user->addGroup(this,false);
}

/*-----------------
* remove functions
-------------------*/

/* removes user, by default removes group from user's group list too (can be changed).*/
const User* Group::removeUserConnection(User* user, bool removeFromOther) {
	// searches for friend in user's friend list. if not found throw exception
	if (!isUserInConnections(user))
		throw std::invalid_argument("Current group " + this->name + " doesn't include the user \"" + user->getName() + "\"");

	std::erase(userConnections, user); // Works in c++20 and after apperantly
	if (removeFromOther)
		user->removeGroup(this, false);
	return user;
}

const User* Group::removeUserConnection(size_t index, bool removeFromOther) {
	if (index >= getUserConnectionsAmount())
		throw std::out_of_range("Chosen user is out of bounds");
	User* userToErase = userConnections.at(index);
	userConnections.erase(userConnections.begin() + index);

	if (removeFromOther)
		userToErase->removeGroup(this, false);
	return userToErase;
}

/*---------
* Files
---------*/

void Group::saveToFile(std::ofstream& outFile) const {
	Profile::saveToFile(outFile);
}