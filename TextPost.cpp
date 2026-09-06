#include <iostream>
#include <fstream>

#include "Facebook.h"
#include "TextPost.h"
#include "Post.h"

using std::cout;

// prints post + post date with new line after.
void TextPost::print(std::ostream& os) const {
	Post::printBaseData(os);
}

void TextPost::saveToFile(std::ofstream& outFile) const {
	outFile << static_cast<int>(Post::PostType::Text) << Facebook::UNIT_DELIMITER;
	Post::saveBaseData(outFile);
	outFile << Facebook::RECORD_DELIMITER;
}