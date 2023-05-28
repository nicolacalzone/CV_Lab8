#ifndef CALIBRATION_UTILS_H
#define CALIBRATION_UTILS_H

#include <vector>
#include <opencv2/core.hpp>

#define CHECKER_SIZE 21.5 // about 2,15 cm per square

void computeObjectPts(std::vector<std::vector<cv::Point3f>> &objectPoints, // input & output
                      std::vector<std::vector<cv::Point2f>> &imagePoints,  // input & output
                      cv::Size patternSize, std::vector<cv::Point2f> corners);

double computeReprojectionError(cv::Mat cameraMatrix, cv::Mat distCoeffs,
                                std::vector<cv::Mat> rotationVec, std::vector<cv::Mat> translationVec,
                                std::vector<std::vector<cv::Point2f>> imagePoints,
                                std::vector<std::vector<cv::Point3f>> objectPoints);

void computeChessboard(cv::Mat &image, cv::Size patternSize,
                       std::vector<cv::Point2f> &corners,
                       std::vector<std::vector<cv::Point2f>> &imagePoints);

#endif