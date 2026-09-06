#include <iostream>
#include <fstream>

#include "ImagePost.h"
#include "Post.h"
#include "Facebook.h"


void ImagePost::print(std::ostream& os) const {
	Post::printBaseData(os);
	os << "\n[Attached Image path: " << imagePath << "]";
}

void ImagePost::saveToFile(std::ofstream& outFile) const {
	outFile << static_cast<int>(Post::PostType::Image) << Facebook::UNIT_DELIMITER;
	Post::saveBaseData(outFile);
	outFile << imagePath << Facebook::UNIT_DELIMITER << Facebook::RECORD_DELIMITER;
}