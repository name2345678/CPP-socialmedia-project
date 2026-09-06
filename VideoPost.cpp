#include <iostream>
#include <fstream>

#include "VideoPost.h"
#include "Post.h"
#include "Facebook.h"

void VideoPost::print(std::ostream& os) const {
	Post::printBaseData(os);
	os << "\n[Attached Video path: " << videoPath << "]";
}

void VideoPost::saveToFile(std::ofstream& outFile) const {
	outFile << static_cast<int>(Post::PostType::Video) << Facebook::UNIT_DELIMITER;
	Post::saveBaseData(outFile);
	outFile << videoPath << Facebook::UNIT_DELIMITER << Facebook::RECORD_DELIMITER;
}