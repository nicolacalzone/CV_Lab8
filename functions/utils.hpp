#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>

/*
    USEFUL FUNCTIONS
*/
void readImages(std::string directory, std::vector<cv::Mat> &images);
double computeReprojectionError(cv::Mat cameraMatrix, cv::Mat distCoeffs,
                                std::vector<cv::Mat> rotationVec, std::vector<cv::Mat> translationVec,
                                std::vector<std::vector<cv::Point2f>> imagePoints,
                                std::vector<std::vector<cv::Point3f>> objectPoints);
void undistortImages1(cv::Mat distortedImg, cv::Mat cameraMatrix, cv::Mat distCoeffs);
void undistortImages2(cv::Mat distortedImg, cv::Mat cameraMatrix, cv::Mat distCoeffs);

#endif // UTILS_H