#pragma once

#include <chrono>
#include <string>

class Post
{
	// post details
protected:
	std::chrono::system_clock::time_point postDate;
	std::string text;

	void saveBaseData(std::ofstream& outFile) const;

public:
	enum class PostType {
		Text,
		Image,
		Video,
		END
	};

	// ctor
	Post(const std::string& _text, std::chrono::system_clock::time_point _postDate):
		text(_text), postDate(_postDate){};
	Post(std::string&& _text, std::chrono::system_clock::time_point _postDate) :
		text(std::move(_text)), postDate(_postDate){};
	// ctors to add date on object creation
	Post(const std::string& _text):
		Post(_text, std::chrono::system_clock::time_point::clock::now()){};
	Post(std::string&& _text):
		Post(std::move(_text), std::chrono::system_clock::time_point::clock::now()){};

	Post(): Post("- Empty post body -"){}; 

	virtual ~Post() = default;

	friend std::ostream& operator<<(std::ostream& os, const Post& post);
	// virtual print method
	virtual void print(std::ostream& os) const = 0;
	// virtual save to file method
	virtual void saveToFile(std::ofstream& outFile) const = 0;
	// print basic info (body text and date)
	void printBaseData(std::ostream& os) const;

	// get
	std::chrono::system_clock::time_point getDate() const { return postDate; };

	//boleans
	bool operator==(const Post& post) const noexcept;
	bool operator!=(const Post& post) const noexcept;

	/*-------
	* Files
	--------*/
	// Post factory - reads from file and creates a heap allocated post.
	static Post* loadFromFile(std::ifstream& inFile);
};
