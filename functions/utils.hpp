#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>

/*
    USEFUL FUNCTIONS
*/
void readImages(std::string directory, std::vector<cv::Mat> &images);

template <typename T>
void printPoints(const std::string &name, const std::vector<std::vector<T>> &vec)
{
    std::cout << name << " points:" << std::endl;

    for (const auto &row : vec)
        for (const auto &point : row)
            std::cout << point << std::endl;

    std::cout << "#############" << std::endl;
}

/*
    DISTORTION FUNCTIONS
*/
void undistortImages1(std::string filePassed, cv::Mat distortedImg, cv::Mat cameraMatrix, cv::Mat distCoeffs);
void undistortImages2(std::string filePassed, cv::Mat distortedImg, cv::Mat cameraMatrix, cv::Mat distCoeffs);

#endif // UTILS_H