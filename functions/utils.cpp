#include "utils.hpp"
#include <dirent.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

void readImages(std::string directory, std::vector<cv::Mat> &images)
{
    for (int i = 1; i <= 27; i++)
    {
        cv::Mat image;

        std::string fileName = "scacchiera (" + std::to_string(i) + ")"; // orders the images
        std::string filePath = directory + "/" + fileName + ".jpg";

        if (FILE *file = fopen(filePath.c_str(), "r"))
        {
            fclose(file);
            image = cv::imread(filePath, cv::IMREAD_COLOR);
        }
        else
        {
            std::cout << "ERROR - UTILS.CPP";
            return;
        }
        images.push_back(image);
    }
}

double computeReprojectionError(cv::Mat cameraMatrix, cv::Mat distCoeffs,
                                std::vector<cv::Mat> rotationVec, std::vector<cv::Mat> translationVec,
                                std::vector<std::vector<cv::Point2f>> imagePoints,
                                std::vector<std::vector<cv::Point3f>> objectPoints)
{
    double totalError = 0;
    int totalPoints = 0;
    for (size_t i = 0; i < objectPoints.size(); i++)
    {
        std::vector<cv::Point2f> imagePoints2;
        cv::projectPoints(objectPoints[i], rotationVec[i], translationVec[i], cameraMatrix,
                          distCoeffs, imagePoints2);
        double error = cv::norm(imagePoints[i], imagePoints2, cv::NORM_L2);
        totalError += error * error;
        totalPoints += objectPoints[i].size();
    }
    double meanError = std::sqrt(totalError / totalPoints);
}

void undistortImages1(cv::Mat distortedImg, cv::Mat cameraMatrix, cv::Mat distCoeffs)
{
    /*
     * InitUndistortRectifyMap
     */
    cv::Mat map1, map2;
    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
                                cameraMatrix, distortedImg.size(), CV_16SC2,
                                map1, map2);
    cv::Mat undistortedImg;
    cv::remap(distortedImg, undistortedImg, map1, map2, cv::INTER_LINEAR);

    cv::Mat combinedImg;
    cv::hconcat(distortedImg, undistortedImg, combinedImg);
    cv::namedWindow("Distortion Method 1", cv::WINDOW_AUTOSIZE);
    cv::imshow("Distortion Method 1", combinedImg);
    cv::waitKey(0);
    cv::imwrite("Distortion Method 1.jpg", combinedImg);
}

void undistortImages2(cv::Mat distortedImg, cv::Mat cameraMatrix, cv::Mat distCoeffs)
{
    /*
     * Undistort
     */
    cv::Mat out;
    cv::undistort(distortedImg, out, cameraMatrix, distCoeffs);
    cv::namedWindow("Distortion Method 2", cv::WINDOW_AUTOSIZE);
    cv::imshow("Distortion Method 2", out);
    cv::waitKey(0);
    cv::imwrite("Distortion Method 2.jpg", out);
}