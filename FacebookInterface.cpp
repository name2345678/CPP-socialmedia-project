#include "FacebookInterface.h"
#include "Facebook.h"
#include "User.h"
#include "Group.h"
#include "Post.h"
#include "TextPost.h"
#include "VideoPost.h"
#include "Profile.h"
#include <iostream>
#include <stdexcept>
#include <typeinfo>

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::chrono::system_clock;

// AI USAGE: A helper string trimming function that trims the spaces at the start and at the end of a string
static string trim(const string& str) {
	size_t first = str.find_first_not_of(" \t\n\r");
	if (first == string::npos) return "";
	size_t last = str.find_last_not_of(" \t\n\r");
	return str.substr(first, (last - first + 1));
}

/*-------------------------------------------
* Console user interface helpers (AI USAGE)
--------------------------------------------*/
// clears console screen
void FacebookInterface::clearScreen(){
	cout << "\x1B[2J\x1B[H"; // special string that clears the screen
}

// 'pauses' console screen until user input (so user could see end messeges)
void FacebookInterface::pause() {
	cout << "\nPress Enter to continue...";
	cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cin.get(); 
}

/*--------
* Inputs
---------*/
// safe int input in certain bounds (by default starts at 0)
int FacebookInterface::getIntInput(int rightBound, int leftBound) {
	int choice;
	// makes sure input is int and in bounds
	while (!(std::cin >> choice) || choice <leftBound || choice > rightBound)
	{
		std::cout << "Invalid option. Please re-enter: \n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return choice;
}

// returns string for body text with a user instruction messege
std::string FacebookInterface::getStringInput(const std::string& messege) {
	cout << messege;
	string txt;
	std::getline(cin, txt);
	return txt;
}

std::chrono::system_clock::time_point FacebookInterface::getDateInput(std::string dateName, bool futureDates) {
	system_clock::time_point date;
	cout << "Please enter a "<< dateName <<" using the format YYYY-MM-DD:";
	while (!(cin >> parse("%Y-%m-%d", date)) || ( !futureDates &&  date > system_clock::now()))
	{
		cin.clear();
		// ^ ^ ^ V V V - AI recommended this (to ignore all sequences of the wrong input instead of just one character)
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "\nWrong format / Invalid date. Please re-enter date (YYYY-MM-DD): ";
	}
	return date;
}


/*-------------------
* System wide prints
--------------------*/
void FacebookInterface::printAllUsers() const {
	if (system.getUserAmount() == 0) {
		cout << "System has no users.\n";
		return;
	}
	for (int i = 0; i < system.getUserAmount(); i++) 
		cout << i + 1 << ". " << *(system.accessUserByIndex(i)) << "\n";
}

void FacebookInterface::printAllGroups() const{
	if (system.getGroupAmount() == 0) {
		cout << "System has no groups.\n";
		return;
	}
	for (int i = 0; i < system.getGroupAmount(); i++) {
		cout << i + 1 << ". " << *(system.accessGroupByIndex(i)) << "\n";
	}
}

/*----------------------
* Active profile prints
-----------------------*/
void FacebookInterface::printProfilePosts() const {
	for (int i = static_cast<int>(activeProfile->getPostAmount() - 1); i >= 0 ; i--) 
		cout << i + 1 << ". " << *(activeProfile->getPostByIndex(i)) << "\n\n";
}

void FacebookInterface::printProfileUserConnections() const {
	if (activeProfile->getUserConnectionsAmount() == 0) {
		cout << "\nUser has no friends.\n";
		return;
	}
	for (int i = 0; i < activeProfile->getUserConnectionsAmount(); i++) 
		cout << i + 1 << ". " << *(activeProfile->getUserConnectionByIndex(i)) << "\n";
}

void FacebookInterface::printUserConnectionsLatestPosts() const {
	vector<const Post*> latest = activeProfile->getUserConnectionsLatestPosts();
	int i = 1;
	for (auto it : latest) {
		cout << i << ". " << *it << "\n\n";
		++i;
	}
}

/*---------------------
* Active User prints
----------------------*/
void FacebookInterface::printUserGroups() const {
	User* currentUser = dynamic_cast<User*>(activeProfile);
	if (currentUser == nullptr)
		throw std::invalid_argument("ERROR: Active user isn't of type 'User'");

	if (currentUser->getGroupAmount() == 0) {
		cout << "\nUser is in no groups.\n";
		return;
	}
	for (int i = 0; i < currentUser->getGroupAmount(); i++) 
		cout << i + 1 << ". " << *(currentUser->accessGroupByIndex(i)) << "\n";
}

/*------------------
* Main menus
-------------------*/
// Starts the main facebook interface menu loop

void FacebookInterface::run() {
	if (system.getUserAmount() == 0) 
		cout << "\n[!] BEWARE: the system has no users. create one to get started\n\n";
	
	if (system.getGroupAmount() == 0) 
		cout << "\n[!] BEWARE: the system has no groups. create one to get started\n\n";

	bool menu_loop = true;
	while (menu_loop) {
		cout << "           Welcome to The Facebook!\n          -------------------------\n";
		cout << MAIN_MENU;

		int choice = getIntInput(static_cast<int>(MAIN_MENU_OPTIONS_SIZE));
		switch (static_cast<MAIN_MENU_OPTIONS>(choice)) {
		case MAIN_MENU_OPTIONS::CREATE_USER:
			FacebookUserCreationMenu();
			break;

		case MAIN_MENU_OPTIONS::CREATE_GROUP:
			FacebookGroupCreationMenu();
			break;

		case MAIN_MENU_OPTIONS::USERS_MENU:
			FacebookUserChoiceMenu();
			break;
		case MAIN_MENU_OPTIONS::GROUPS_MENU:
			FacebookGroupChoiceMenu();
			break;
		case MAIN_MENU_OPTIONS::VIEW_ALL_PROFILES:
			clearScreen();
			cout << "\nUsers:\n";
			printAllUsers();
			cout << "\nGroups:\n";
			printAllGroups();
			pause();
			break;
		case MAIN_MENU_OPTIONS::EXIT_MAX:
		case MAIN_MENU_OPTIONS::EXIT_0:
			menu_loop = false;
			break;
		}
		clearScreen();
	}
	cout << "\n\texiting... thanks for using The Facebook!\n";
}

void FacebookInterface::FacebookUserCreationMenu() {
	string newName;
	system_clock::time_point newBirthDate;
	while (true) {
		clearScreen();
		cout << "\n			* User creation screen *\n";
		cout << "Please enter a name:";
		cin >> std::ws;
		std::getline(cin, newName);
		newName = trim(newName);
		if (newName.length() < MIN_USER_NAME_LENGTH) {
			cout << "\nUsername must be at least " << MIN_USER_NAME_LENGTH << " characters long. Please try again:\n";
			pause();
			continue;
		}
		// if user exists - print the exception message
		if (system.findUserByName(newName) != nullptr) {
			cout << "\n Username \"" + newName + "\" already exists in system -- Please try again\n";
			pause();
			continue;
		}
		newBirthDate = getDateInput("birthDate");
		system.createUser(newName, newBirthDate);
		break;
	}
	cout << "\n\tUser creation successfull";
	pause();
}

void FacebookInterface::FacebookGroupCreationMenu() {
	string newName;
	while (true) {
		clearScreen();
		cout << "\n            * Group creation screen *\n";
		cout << "Please enter a name:";
		cin >> std::ws; // gets rid of leftover spaces
		std::getline(cin, newName);
		newName = trim(newName);
		if (newName.length() < MIN_GROUP_NAME_LENGTH) {
			cout << "\nGroup name must be at least " << MIN_GROUP_NAME_LENGTH << " characters long. Please try again:\n";
			pause();
			continue;
		}
		try {
			system.createGroup(newName);
		}
		// if createGroup thorws exception this means the group name already exists
		catch (const std::exception& e) {
			cout << "\n" << e.what() <<" -- Please try again\n";
			pause();
			continue;
		}
		break;
	}
	cout << "\n\tGroup creation successfull";
	pause();
}

void FacebookInterface::FacebookUserChoiceMenu() {
	clearScreen();
	cout << "Choose a user: (To exit enter 0)\n";
	if (system.getUserAmount() == 0)
	{
		cout << "\tSystem has no users.\n";
		pause();
		return;
	}
	printAllUsers();
	int choice = getIntInput(static_cast<int>(system.getUserAmount()));

	if (choice == 0) {
		cout << "    - exiting user choice menu... - \n";
		return;
	}
	activeProfile = system.accessUserByIndex(choice-1);
	UserMenu();
}

void FacebookInterface::FacebookGroupChoiceMenu() {
	clearScreen();
	cout << "Choose a group: \nTo exit enter 0\n";
	if (system.getGroupAmount() == 0) {
		cout << "\tSystem has no groups\n";
		pause();
		return;
	}
	printAllGroups();
	int choice = getIntInput(static_cast<int>(system.getGroupAmount()));
	if (choice == 0) {
		cout << "\nexiting user choice menu...";
		return;
	}
	activeProfile = system.accessGroupByIndex(choice-1);
	GroupMenu();
}

/*-----------------------
* Active user sub-menus
------------------------*/
void FacebookInterface::UserMenu() {
	while (true) {
		clearScreen();
		cout << "\n               * Menu for the user: \"" << activeProfile->getName() << "\" *\n(To exit enter " + std::to_string(static_cast<int>(USER_MAIN_MENU_OPTIONS::EXIT_0)) + " or " + std::to_string(static_cast<int>(USER_MAIN_MENU_OPTIONS::EXIT_MAX)) + ")\n";
		cout << USER_MAIN_MENU;
		int choice = getIntInput(static_cast<int>(USER_MAIN_MENU_OPTIONS_SIZE));
		switch (static_cast<USER_MAIN_MENU_OPTIONS>(choice)) {
		case USER_MAIN_MENU_OPTIONS::CREATE_POST:
			postCreationMenu();
			break;
		case USER_MAIN_MENU_OPTIONS::VIEW_ALL_POSTS:
			clearScreen();
			cout << "\nCurrent Posts:\n";
			if (activeProfile->getPostAmount() == 0) {
				cout << "User has no posts\n";
				pause();
				break;
			}
			printProfilePosts();
			pause();
			break;
		case USER_MAIN_MENU_OPTIONS::VIEW_ALL_FRIENDS:
			clearScreen();
			cout << "\nCurrent friends:\n";
			if (activeProfile->getUserConnectionsAmount() == 0) {
				cout << "User has no friends\n";
				pause();
				break;
			}
			printProfileUserConnections();
			pause();
			break;
		case USER_MAIN_MENU_OPTIONS::VIEW_LATESTS_FRIENDS_POSTS:
			clearScreen();
			cout << "\nThe lastest " << User::DEFAULT_LATEST_POSTS_AMOUNT << " posts by the user's friends:\n";
			printUserConnectionsLatestPosts();
			pause();
			break;
		case USER_MAIN_MENU_OPTIONS::ADD_FRIENDS:
			UserAddNewFriendMenu();
			break;
		case USER_MAIN_MENU_OPTIONS::REMOVE_FRIENDS:
			UserRemoveFriendMenu();
			break;
		case USER_MAIN_MENU_OPTIONS::JOIN_GROUP:
			UserAddNewGroupMenu();
			break;
		case USER_MAIN_MENU_OPTIONS::LEAVE_GROUP:
			UserRemoveGroupMenu();
			break;
		case USER_MAIN_MENU_OPTIONS::DELETE_USER:
			if (UserDeleteMenu()) 
				return;
			break;
		case USER_MAIN_MENU_OPTIONS::EXIT_MAX:
		case USER_MAIN_MENU_OPTIONS::EXIT_0:
			cout << "exiting " << activeProfile->getName() << "'s menu...\n";
			activeProfile = nullptr;
			pause();
			return;
		}
	}
}

bool FacebookInterface::UserDeleteMenu() {
	User* currentUser = dynamic_cast<User*>(activeProfile);
	if (currentUser == nullptr)
		throw std::invalid_argument("ERROR: Active profile isn't a user!");
	clearScreen();
	cout << "\n Are you sure you want to delete user? all data would be lost.\n";
	cout << "\nPress 9 and enter to delete.\n Press and enter any other button to cancel\n";
	int choice;
	if (!(cin >> choice) || choice != 9) {
		cin.clear(); // If entered invalid input that's not int - clear cin so it doesn't crash
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return false;
	}
	system.deleteUser(currentUser);
	activeProfile = nullptr;
	cout << "\nUser deletion successful";
	pause();
	return true;
}

void FacebookInterface::UserAddNewFriendMenu() {
	clearScreen();
	cout << "\n				* Choose which to add as friends for user: \"" << activeProfile->getName() << "\". Enter 0 to exit * \n";
	vector<User*> potential_friends = system.getPotentialUserConnectionsFor(activeProfile);
	if (potential_friends.empty()) {
		cout << "User is already friends with all users! exiting friend finding menu... \n";
		pause();
		return;
	}
	int i = 1;
	for (auto it : potential_friends) {
		cout << i << ". " << *it << "\n";
		i++;
	}
	int choice = getIntInput(i-1);
	if (choice == 0) {
		cout << "exiting friend finding menu...\n";
		pause();
		return;
	}
	activeProfile->addUserConnection(potential_friends.at(choice - 1));
	cout << "\nAdding friend \"" + potential_friends.at(choice - 1)->getName() + "\" successful";
	pause();
}

void FacebookInterface::UserAddNewGroupMenu() {
	User* activeUser = dynamic_cast<User*>(activeProfile);
	if (activeUser == nullptr) 
		throw std::invalid_argument("ERROR: Active profile isn't a user!");
	clearScreen();
	cout << "\n				* Choose which group to join for user: \"" << activeProfile->getName() << "\". Enter 0 to exit * \n";
	vector<Group*> potential_groups = system.getPotentialGroupsFor(activeUser);
	if (potential_groups.empty()) {
		cout << "User is already in all the existing groups! exiting friend finding menu... \n";
		pause();
		return;
	}
	int i = 1;
	for (auto it : potential_groups) {
		cout << i << ". " << *it << "\n";
		i++;
	}
	int choice = getIntInput(i - 1);
	if (choice == 0) {
		cout << "exiting group finding menu...\n";
		pause();
		return;
	}
	activeUser->addGroup(potential_groups.at(choice - 1));
	cout << "\nJoining group \""+ potential_groups.at(choice - 1)->getName()+"\" successful";
	pause();
}

void FacebookInterface::UserRemoveFriendMenu() {
	clearScreen();
	cout << "\n				* Choose which friend to remove for user: \"" << activeProfile->getName() << "\". Enter 0 to exit * \n";
	while (activeProfile->getUserConnectionsAmount() > 0) {
		cout << "\nPlease choose a friend to remove:\n";
		printProfileUserConnections();
		int choice = getIntInput(static_cast<int>(activeProfile->getUserConnectionsAmount()));
		if (choice == 0) {
			cout << "exiting user removal menu...\n";
			pause();
			return;
		}
		const User* friendToRemove = activeProfile->getUserConnectionByIndex(choice - 1);
		cout << "Removing: \"" << friendToRemove->getName() << "\"\n";
		activeProfile->removeUserConnection(choice-1);
	}
	cout << "User has no friends. exiting...\n";
	pause();
}

void FacebookInterface::UserRemoveGroupMenu() {
	User* activeUser = dynamic_cast<User*>(activeProfile);
	if (activeUser == nullptr)
		throw std::invalid_argument("ERROR: Active profile isn't a user!");
	while (activeUser->getGroupAmount() > 0) {
		clearScreen();
		cout << "\n			* Choose which group for user:  \"" << activeUser->getName() << "\" to leave* \n";
		cout << "Please choose a group to leave from:  \n Enter 0 to exit \n";
		printUserGroups();
		int choice = getIntInput(static_cast<int>(activeUser->getGroupAmount()));
		if (choice == 0) {
			cout << "exiting group removal menu...\n";
			pause();
			return;
		}
		const Group* groupToRemove = activeUser->accessGroupByIndex(choice-1);
		cout << "leaving: \"" << groupToRemove->getName() << "\"\n";
		activeUser->removeGroup(choice-1);
		pause();
	}
	cout << "User has no groups. exiting...\n";
	pause();
}


/*----------------------
* Active group sub-menus
-----------------------*/
void FacebookInterface::GroupMenu() {
	while (true) {
		clearScreen();
		cout << "\n               * Menu for the group: \"" << activeProfile->getName() << "\" *\n  (To exit enter " + std::to_string(static_cast<int>(GROUP_MAIN_MENU_OPTIONS::EXIT_0)) + " or " << std::to_string(static_cast<int>(GROUP_MAIN_MENU_OPTIONS::EXIT_MAX)) << ")\n";
		size_t postCount = activeProfile->getPostAmount();
		size_t userCount = activeProfile->getUserConnectionsAmount();
		cout << GROUP_MAIN_MENU;
		int choice = getIntInput(GROUP_MAIN_MENU_OPTIONS_SIZE);
		switch (static_cast<GROUP_MAIN_MENU_OPTIONS>(choice)) {
		case GROUP_MAIN_MENU_OPTIONS::CREATE_POST:
			postCreationMenu();
			break;
		case GROUP_MAIN_MENU_OPTIONS::VIEW_ALL_POSTS:
			clearScreen();
			cout << "\nCurrent Posts:\n";
			if (postCount == 0) {
				cout << "Group has no posts\n";
				pause();
				break;
			}
			printProfilePosts();
			pause();
			break;
		case GROUP_MAIN_MENU_OPTIONS::VIEW_ALL_MEMBERS:
			clearScreen();
			cout << "\nCurrent members:\n";
			if (userCount == 0) {
				cout << "Group has no users\n";
				pause();
				break;
			}
			printProfileUserConnections();
			pause();
			break;
		case GROUP_MAIN_MENU_OPTIONS::REMOVE_MEMBER:
			GroupRemoveUserMenu();
			break;
		case GROUP_MAIN_MENU_OPTIONS::DELETE_GROUP:
			if (GroupDeleteMenu())
				return;
			break;
		case GROUP_MAIN_MENU_OPTIONS::EXIT_MAX:
		case GROUP_MAIN_MENU_OPTIONS::EXIT_0:
			cout << "exiting group: \"" << activeProfile->getName() << "\"'s menu...\n";
			activeProfile = nullptr;
			pause();
			return;
		}
	}
}

void FacebookInterface::GroupRemoveUserMenu() {
	while (activeProfile->getUserConnectionsAmount() > 0) {
		clearScreen();
		cout << "			* Remove user from \"" << activeProfile->getName() << "\" group menu * \n";
		cout << "Please choose a user to remove from the group:) \n Enter 0 to exit \n";
		printProfileUserConnections();
		int choice = getIntInput(static_cast<int>(activeProfile->getUserConnectionsAmount()));

		if (choice == 0) {
			cout << "exiting user removal menu...\n";
			pause();
			return;
		}

		cout << "Removing: \"" << activeProfile->removeUserConnection(choice-1)->getName() << "\"...\n";
		pause();
	}
	// if user amount is 0 - group has no users and while loop is skipped
	cout << "Group has no users. exiting...\n";
	pause();
}

bool FacebookInterface::GroupDeleteMenu() {
	Group* activeGroup = dynamic_cast<Group*>(activeProfile);
	if(activeGroup == nullptr)
		throw std::invalid_argument("ERROR: Active profile isn't a Group!");

	clearScreen();
	cout << "\n Are you sure you want to delete group? all data would be lost.\n";
	cout << "\nPress 9 and enter to delete.\n Press and enter any other button to cancel\n";
	int choice;
	if (!(cin >> choice) || choice != 9) {
		cin.clear(); // If entered invalid input that's not int - clear cin so it doesn't crash
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return false;
	}
	system.deleteGroup(activeGroup);
	activeProfile = nullptr;
	cout << "\nUser deletion successful";
	pause();
	return true;
}

/*---------------------
* Post creation menu
---------------------*/
void FacebookInterface::postCreationMenu() {
	clearScreen();
	Profile* activeProfile = getActiveProfile();
	if (activeProfile == nullptr)
		throw std::domain_error("ERROR: No active profile");

	string bodyText =  getStringInput("\n * Creating post * Please enter the body text: ");
	cout << POST_CREATION_MENU;
	int option = getIntInput(static_cast<int>(Post::PostType::END)-1);
	string mediaPath;
	switch (static_cast<Post::PostType>(option)) {
	case Post::PostType::Text:
		activeProfile->createAndAddPost(bodyText);
		break;
	case Post::PostType::Image:
		mediaPath = getStringInput("\nPlease enter a path to the image file: ");
		activeProfile->createAndAddPost(bodyText, Post::PostType::Image ,mediaPath);
		break;

	case Post::PostType::Video: 
		mediaPath = getStringInput("\nPlease enter a path to the video file: ");
		activeProfile->createAndAddPost(bodyText ,Post::PostType::Video, mediaPath);
		break;
	}
	cout << "\nPost creation successful";
	pause();
}