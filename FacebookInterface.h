#pragma once
#include <string>
#include "Post.h"
#include "User.h"


class Facebook;
class Profile;

class FacebookInterface
{
	// interface variable
	Facebook& system;
	// dynamic menu variables (changes according to active user / active group when chosen in menu)
	Profile* activeProfile;


	/*-----------------------------
	* Menu option amount constants
	-------------------------------*/
	constexpr static size_t MIN_USER_NAME_LENGTH = 3;
	constexpr static size_t MIN_GROUP_NAME_LENGTH = 4;

	/* Menu user choice enums */
	enum class MAIN_MENU_OPTIONS { EXIT_0 , CREATE_USER , CREATE_GROUP, USERS_MENU, GROUPS_MENU, VIEW_ALL_PROFILES, EXIT_MAX };
	enum class USER_MAIN_MENU_OPTIONS { EXIT_0, CREATE_POST, VIEW_ALL_POSTS, VIEW_ALL_FRIENDS, VIEW_LATESTS_FRIENDS_POSTS, ADD_FRIENDS, REMOVE_FRIENDS, JOIN_GROUP, LEAVE_GROUP, DELETE_USER , EXIT_MAX };
	enum class GROUP_MAIN_MENU_OPTIONS { EXIT_0, CREATE_POST, VIEW_ALL_POSTS, VIEW_ALL_MEMBERS, REMOVE_MEMBER, DELETE_GROUP ,EXIT_MAX };

	static constexpr int MAIN_MENU_OPTIONS_SIZE = static_cast<int>(MAIN_MENU_OPTIONS::EXIT_MAX);
	static constexpr int USER_MAIN_MENU_OPTIONS_SIZE = static_cast<int>(USER_MAIN_MENU_OPTIONS::EXIT_MAX);
	static constexpr int GROUP_MAIN_MENU_OPTIONS_SIZE = static_cast<int>(GROUP_MAIN_MENU_OPTIONS::EXIT_MAX);

	/*------------
	* Menu text
	-------------*/
	const static inline std::string MAIN_MENU = "\nEnter number to navigate the system. Enter "+ std::to_string(static_cast<int>(MAIN_MENU_OPTIONS::EXIT_0)) +" or "+ std::to_string(static_cast<int>(MAIN_MENU_OPTIONS::EXIT_MAX)) + " to exit\n"
		+std::to_string(static_cast<int>(MAIN_MENU_OPTIONS::CREATE_USER)) + ". Create new user\n"
		+ std::to_string(static_cast<int>(MAIN_MENU_OPTIONS::CREATE_GROUP)) + ". Create new group\n"
		+ std::to_string(static_cast<int>(MAIN_MENU_OPTIONS::USERS_MENU)) + ". Enter user profile\n"
		+ std::to_string(static_cast<int>(MAIN_MENU_OPTIONS::GROUPS_MENU))+". Enter group profile\n"
		+ std::to_string(static_cast<int>(MAIN_MENU_OPTIONS::VIEW_ALL_PROFILES)) +". View all current groups and users\n"
		+ std::to_string(static_cast<int>(MAIN_MENU_OPTIONS::EXIT_MAX))+". exit and save\n";

	const static inline std::string USER_MAIN_MENU = "\nPlease choose a valid option: \n"
		+ std::to_string(static_cast<int>(USER_MAIN_MENU_OPTIONS::CREATE_POST)) + ". Add a new post\n"
		+ std::to_string(static_cast<int>(USER_MAIN_MENU_OPTIONS::VIEW_ALL_POSTS)) + ". Show all the posts published by the user\n"
		+ std::to_string(static_cast<int>(USER_MAIN_MENU_OPTIONS::VIEW_ALL_FRIENDS)) + ". Show all of the users' friends\n"
		+ std::to_string(static_cast<int>(USER_MAIN_MENU_OPTIONS::VIEW_LATESTS_FRIENDS_POSTS)) + ". Show the lastest " + std::to_string(User::DEFAULT_LATEST_POSTS_AMOUNT) + " posts by the user's friends\n"
		+ std::to_string(static_cast<int>(USER_MAIN_MENU_OPTIONS::ADD_FRIENDS)) + ". Add a new friend\n"
		+ std::to_string(static_cast<int>(USER_MAIN_MENU_OPTIONS::REMOVE_FRIENDS)) + ". Remove a friend\n"
		+ std::to_string(static_cast<int>(USER_MAIN_MENU_OPTIONS::JOIN_GROUP)) + ". Join a new group\n"
		+ std::to_string(static_cast<int>(USER_MAIN_MENU_OPTIONS::LEAVE_GROUP)) + ". Leave a group\n"
		+ std::to_string(static_cast<int>(USER_MAIN_MENU_OPTIONS::DELETE_USER)) + ". * DELETE USER *\n"
		+ std::to_string(static_cast<int>(USER_MAIN_MENU_OPTIONS::EXIT_MAX)) + ". exit\n";

	const static inline std::string GROUP_MAIN_MENU = "\nPlease choose a valid option: \n"
		+ std::to_string(static_cast<int>(GROUP_MAIN_MENU_OPTIONS::CREATE_POST))+". Add a new post\n"
		+ std::to_string(static_cast<int>(GROUP_MAIN_MENU_OPTIONS::VIEW_ALL_POSTS)) + ". Show all the posts currently in the group\n"
		+ std::to_string(static_cast<int>(GROUP_MAIN_MENU_OPTIONS::VIEW_ALL_MEMBERS)) + ". Show all of the group's members\n"
		+ std::to_string(static_cast<int>(GROUP_MAIN_MENU_OPTIONS::REMOVE_MEMBER)) + ". Remove a member from the group\n"
		+ std::to_string(static_cast<int>(GROUP_MAIN_MENU_OPTIONS::DELETE_GROUP)) + ". * DELETE GROUP *\n"
		+ std::to_string(static_cast<int>(GROUP_MAIN_MENU_OPTIONS::EXIT_MAX)) + ". exit\n";

	const static inline std::string POST_CREATION_MENU = "\nPlease choose a valid option: \n"
		+ std::to_string(static_cast<int>(Post::PostType::Text)) + ". Plain Text Post\n"
		+ std::to_string(static_cast<int>(Post::PostType::Image)) + ". Image post\n"
		+ std::to_string(static_cast<int>(Post::PostType::Video)) + ". Video post\n";

	/*-------
	* Inputs
	--------*/
	static int getIntInput(int rightBound, int leftBound = 0); 	// static function to get safe int input in a range
	std::string getStringInput(const std::string& messege);
	std::chrono::system_clock::time_point getDateInput(std::string dateName, bool futureDates = false);

	/*--------
	*Getters
	---------*/
	Profile* getActiveProfile() { return activeProfile; };

	/*------- 
	* Menus
	--------*/
	// Main menus
	void FacebookUserCreationMenu();
	void FacebookGroupCreationMenu();
	void FacebookUserChoiceMenu();
	void FacebookGroupChoiceMenu();

	// Active user sub-menus
	void UserMenu();
	void UserAddNewFriendMenu();
	void UserAddNewGroupMenu();
	void UserRemoveFriendMenu();
	void UserRemoveGroupMenu();
	bool UserDeleteMenu();

	// Active group sub-menus
	void GroupMenu();
	void GroupRemoveUserMenu();
	bool GroupDeleteMenu();
	
	// Post creation menu
	void postCreationMenu();

	/*-------
	* Prints
	--------*/
	// System wide prints
	void printAllUsers() const;
	void printAllGroups() const;

	// Active profile prints
	void printProfilePosts() const;
	void printProfileUserConnections() const;
	void printUserConnectionsLatestPosts() const;

	// Active user prints
	void printUserGroups() const;

	/*----------------------------
	* Console interface helpers
	-----------------------------*/
	static void clearScreen();
	static void pause();


public:
	FacebookInterface(Facebook& _system) :
		system(_system),
		activeProfile(nullptr) { };

	// FacebookInterface is unique so no need for copying
	FacebookInterface(const FacebookInterface& other) = delete;
	FacebookInterface(Facebook&& other) = delete;
	FacebookInterface& operator=(const FacebookInterface& other) = delete;
	FacebookInterface& operator=(FacebookInterface&& other) = delete;

	// The main program loop
	void run();
};

