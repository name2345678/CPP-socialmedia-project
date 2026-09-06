#pragma once

#include <string>
#include "Post.h"
class TextPost : public Post
{
public: 
	TextPost(const std::string& bodyText, std::chrono::system_clock::time_point postDate) : Post(bodyText, postDate) {};
	TextPost(const std::string& bodyText): TextPost(bodyText, std::chrono::system_clock::now()) {};
	TextPost(std::string&& bodyText, std::chrono::system_clock::time_point postDate) : Post(std::move(bodyText), postDate) {};
	TextPost(std::string&& bodyText) : TextPost(std::move(bodyText), std::chrono::system_clock::now()) {};

	// prints post body text + post date.
	void print(std::ostream& os) const override;
	void saveToFile(std::ofstream& outFile) const;
};

