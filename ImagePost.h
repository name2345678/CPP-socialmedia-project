#pragma once
#include <string>

#include "Post.h"
class ImagePost : public Post
{
	std::string imagePath;
public:
	ImagePost(const std::string& bodyText, const std::string& _imagePath, std::chrono::system_clock::time_point _postDate) :
		Post(bodyText, _postDate), imagePath(_imagePath) {
	};
	ImagePost(const std::string& bodyText, const std::string& _imagePath) :
		Post(bodyText), imagePath(_imagePath) {
	};
	ImagePost(const std::string& _imagePath) :
		Post(), imagePath(_imagePath) {
	};

	void print(std::ostream& os) const override;
	void saveToFile(std::ofstream& outFile) const;
};