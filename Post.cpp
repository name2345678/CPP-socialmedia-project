#include "Post.h"
#include "VideoPost.h"
#include "ImagePost.h"
#include "TextPost.h"
#include "Facebook.h"
#include <iostream>
#include <format>
#include <stdexcept>
#include <string>


using std::cout;
using std::string;
using std::chrono::system_clock;

bool Post::operator==(const Post& post) const noexcept{
	return post.text == text;
}

bool Post::operator!=(const Post& post) const noexcept{
	return !(*this == post);
} 

void Post::printBaseData(std::ostream& os) const {
	std::chrono::zoned_time localTime{ std::chrono::current_zone(), postDate }; // Makes the time accurate to the time zone
	std::cout << "Text : \"" << text << "\"\nPublishing date: " << std::format("{:%Y-%m-%d %H:%M:%S}", localTime);
}

std::ostream& operator<<(std::ostream& os, const Post& post) {
	post.print(os);
	return os;
}

// Protected function - Saves the base data of a post (postDate and bodyText) WITHOUT finishing with a '\x1E'
void Post::saveBaseData(std::ofstream& outFile) const {
	if (!outFile.is_open()) {
		throw std::runtime_error("ERROR: data file isn't open");
	}
	outFile << postDate.time_since_epoch().count() << Facebook::UNIT_DELIMITER;
	outFile << text << Facebook::UNIT_DELIMITER;
}


/* Reads data from file and returns a !*memory allocated*! post.
if failed to read file or make post throws exception */
Post* Post::loadFromFile(std::ifstream& inFile) {
	int postType = Facebook::readNumFromFile(inFile);
	long long postDateTicks = Facebook::readNumFromFile<long long>(inFile);
	system_clock::time_point postDate{ system_clock::duration(postDateTicks) }; // converts numeric ticks to date
	string bodyText = Facebook::readStringFromFile(inFile);
	string path;

	Post* newPost = nullptr;
	switch (static_cast<PostType>(postType)) {
	case Post::PostType::Text:
		newPost = new TextPost(bodyText, postDate);
		break;
	case PostType::Image:
		path = Facebook::readStringFromFile(inFile);
		newPost =  new ImagePost(bodyText, path, postDate);
		break;
	case PostType::Video:
		path = Facebook::readStringFromFile(inFile);
		newPost =  new VideoPost(bodyText, path, postDate);
		break;
	default:
		throw std::runtime_error("ERROR: Got an invalid post type value when reading a file");
	}
	Facebook::consumeRecordFromFile(inFile);
	return newPost;
}


