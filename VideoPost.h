#pragma once

#include <string>

#include "Post.h"

class VideoPost : public Post
{
	std::string videoPath;

public:
	VideoPost(const std::string& bodyText, const std::string& _videoPath, std::chrono::system_clock::time_point _postDate):
		Post(bodyText, _postDate), videoPath(_videoPath) {};
	VideoPost(const std::string& bodyText, const std::string& _videoPath) :
		Post(bodyText), videoPath(_videoPath) {};
	VideoPost(const std::string& _videoPath) :
		Post(), videoPath(_videoPath) {};

	void print(std::ostream& os) const override;
	void saveToFile(std::ofstream& outFile) const;

};

