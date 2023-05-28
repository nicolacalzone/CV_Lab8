#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>

/*
    USEFUL FUNCTIONS
*/
void readImages(std::string directory, std::vector<cv::Mat> &images);

#endif // UTILS_H