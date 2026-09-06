#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <chrono>
#include "Profile.h"
#include "User.h"
#include "Post.h"
#include "TextPost.h"
#include "ImagePost.h"
#include "VideoPost.h"
#include "Facebook.h"


using std::string;
using std::vector;
using std::chrono::system_clock;

Profile::~Profile() {
	for (auto post : posts) 
		delete post;
}

size_t Profile::getPostAmount() const noexcept {
	if (posts.empty())
		return 0;
	return posts.size();
}

// returns const post pointer at certain index. (If out of bounds .at() returns exception)
const Post* Profile::getPostByIndex(size_t index) const {
	return posts.at(index);
}

/* (printUserConnectionsLatestPosts helper function) returns vecor of the latest profiles' user connections posts */
vector<const Post*> Profile::getUserConnectionsLatestPosts(const size_t latest_posts_amount) const {
	vector<const Post*> latestPosts;
	latestPosts.reserve(latest_posts_amount); // known max size
	size_t friendCount = getUserConnectionsAmount();
	vector<int>indices(friendCount); // initializes vector with known size

	// find the indices of the latest posts by all of the user's friends
	for (int i = 0; i < friendCount; i++) {
		indices[i] = static_cast<int>(userConnections[i]->posts.size()) - 1;
	}

	// for loop to fill latest posts array
	for (int j = 0; j < latest_posts_amount; j++) {
		Post* tempLatestPost = nullptr;
		int* biggestIndex = nullptr; // saves pointer to the latest date's post's index
		// for loop to compare all of the friend's latest posts.
		for (int i = 0; i < friendCount; i++) {
			if (indices[i] >= 0 && (tempLatestPost == nullptr || userConnections[i]->posts[indices[i]]->getDate() >= tempLatestPost->getDate())) {
				tempLatestPost = userConnections[i]->posts[indices[i]];
				biggestIndex = &indices[i];
			}
		}
		// if tempLatestPost wasn't updated this means there are no posts left to compare
		if (tempLatestPost == nullptr)
			break;
		latestPosts.push_back(tempLatestPost);
		// move to the next post if the latest date was found
		if (biggestIndex != nullptr)
			(*biggestIndex)--;
	}
	return latestPosts;
}

// creates and adds post to heap. By deafult creates a text post and if not sent anything creates an empty body text post
void Profile::createAndAddPost(const string& bodyText, Post::PostType postType, const string& mediaPath, system_clock::time_point postDate) {
	if (postType == Post::PostType::Text && mediaPath != "") 
		throw std::invalid_argument("ERROR: Beware that a text post has no file path. please recreate without a path");
	Post* newPost = nullptr;
	switch (postType) {
		case Post::PostType::Text:
			newPost = new TextPost(bodyText, postDate);
			break;
		case Post::PostType::Image:
			newPost = new ImagePost(bodyText, mediaPath, postDate);
			break;
		case Post::PostType::Video:
			newPost = new VideoPost(bodyText, mediaPath, postDate);
			break;
	}
	if (newPost != nullptr)
		posts.push_back(newPost);
}

/*----------
* Adders
-----------*/
Profile& Profile::operator+=(User & userToAdd) {
	this->addUserConnection(&userToAdd);
	return *this;
}

/*----------
* Removers
-----------*/
Profile& Profile::operator-=(User& userToRemove) {
	this->removeUserConnection(&userToRemove);
	return *this;
}

/*--------
* Print 
--------*/
std::ostream& operator<<(std::ostream& os, const Profile& profile) {
	profile.print(os);
	return os;
}



/*-------------
* Booleans 
------------*/
bool Profile::isUserInConnections(const Profile* other) const noexcept {
	if (other == this)
		return false;
	auto userToFind = std::find(userConnections.begin(),userConnections.end(), other);
	if (userToFind == userConnections.end())
		return false;
	return true;
}

bool Profile::operator>(const Profile& other) const noexcept {
	return this->getUserConnectionsAmount() > other.getUserConnectionsAmount();
}

bool  Profile::operator==(const Profile& other) const noexcept {
	return this->getUserConnectionsAmount() == other.getUserConnectionsAmount();
}

bool  Profile::operator!=(const Profile& other) const noexcept {
	return !(*this == other);
}

bool  Profile::operator<(const Profile& other) const noexcept {
	return other > *this;
}

bool  Profile::operator<=(const Profile& other) const noexcept {
	return *this < other || *this == other;
}

bool  Profile::operator>=(const Profile& other) const noexcept {
	return *this > other || *this == other;
}

/*------
* Files
________*/
void Profile::saveUserConnectionsToFile(std::ofstream& outFile) const{
	if (!outFile.is_open())
		throw std::runtime_error("Failed to open the file for writing");

	outFile << name << Facebook::UNIT_DELIMITER;
	outFile << static_cast<int>(getUserConnectionsAmount()) << Facebook::UNIT_DELIMITER << Facebook::RECORD_DELIMITER;
	for (User* user : userConnections)
		outFile << user->getName() << Facebook::UNIT_DELIMITER;

	outFile << Facebook::RECORD_DELIMITER;
}

void Profile::saveToFile(std::ofstream & outFile) const {
	if (!outFile.is_open())
		throw std::runtime_error("Failed to open the file for writing");
	
	outFile << name << Facebook::UNIT_DELIMITER;
	outFile << static_cast<int>(getPostAmount()) << Facebook::UNIT_DELIMITER << Facebook::RECORD_DELIMITER;
	for (Post* post : posts) {
		post->saveToFile(outFile);
	}
}

void Profile::loadPostsFromFile(std::ifstream& inFile, int postAmount) {
	posts.reserve(postAmount);
		for (int i = 0; i < postAmount; i++) {
			posts.push_back(Post::loadFromFile(inFile));
	}
}