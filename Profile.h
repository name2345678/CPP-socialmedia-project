#pragma once
#include <string>
#include <vector>
#include "Post.h"

class User;

class Profile
{
protected:
	std::string name;
	std::vector<Post*>posts;
	std::vector<User*>userConnections;

public:
	// latest posts default amount
	static constexpr size_t DEFAULT_LATEST_POSTS_AMOUNT = 10;

	// Profile type enum
	enum class ProfileType {USER, GROUP, END};
	
	// ctor / dtor
	Profile(std::string _name): name(_name){};

	virtual ~Profile();

	// Each profile is unique so no need for copying
	Profile(const Profile& other) = delete;
	Profile(Profile&& other) = delete;
	Profile& operator=(const Profile& other) = delete;
	Profile& operator=(Profile&& other) = delete;

	// print
	virtual void print(std::ostream& os) const = 0;
	friend std::ostream& operator<<(std::ostream& os, const Profile& profile);
	
	// booleans
	bool isUserInConnections(const Profile* other) const noexcept;
	bool operator>(const Profile& other) const noexcept;
	bool operator<(const Profile& other) const noexcept;
	bool operator<=(const Profile& other) const noexcept;
	bool operator>=(const Profile& other) const noexcept;
	bool operator==(const Profile& other) const noexcept;
	bool operator!=(const Profile& other) const noexcept;

	// getters
	const std::string& getName() const noexcept { return name; };
	size_t getPostAmount() const noexcept;
	const Post* getPostByIndex(size_t index) const;
	size_t getUserConnectionsAmount() const noexcept { return userConnections.size(); };
	const User* getUserConnectionByIndex(size_t index) const { return userConnections.at(index); };
	std::vector<const Post*> getUserConnectionsLatestPosts(const size_t latest_posts_amount = DEFAULT_LATEST_POSTS_AMOUNT) const;

	// adders
	void createAndAddPost(const std::string& bodyText, Post::PostType postType = Post::PostType::Text, const std::string& mediaPath = "", std::chrono::system_clock::time_point postDate = std::chrono::system_clock::now());
	virtual void addUserConnection(User* user, bool addToOther = true) = 0;
	Profile& operator+=(User& userToAdd);

	// removers
	virtual const User* removeUserConnection(User* user, bool removeFromOther = true) = 0;
	virtual const User* removeUserConnection(size_t index, bool removeFromOther = true) = 0;
	Profile& operator-=(User& userToRemove);

	/*------
	* Files
	________*/
	virtual void saveToFile(std::ofstream& outFile) const = 0;
	void saveUserConnectionsToFile(std::ofstream& outFile) const;
	void loadPostsFromFile(std::ifstream& inFile, int postAmount);

};

