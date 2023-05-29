#include "utils.hpp"
#include <dirent.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

void readImages(std::string directory, std::vector<cv::Mat> &images)
{
    for (int i = 1; i <= 27; i++) // change to 27 if you want to try with my camera pictures
    {
        cv::Mat image;

        std::string fileName = "scacchiera (" + std::to_string(i) + ")"; // orders the images
        std::string filePath = directory + "/" + fileName + ".jpg";      // change to .JPG if you want to try my camera settings

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

void undistortImages1(std::string filePassed, cv::Mat distortedImg, cv::Mat cameraMatrix, cv::Mat distCoeffs)
{
    /*
     * InitUndistortRectifyMap + Remap
     */
    cv::Mat map1, map2;

    std::cout << "old camera matrix" << std::endl
              << cameraMatrix << std::endl;

    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
                                cameraMatrix, cv::Size(distortedImg.cols, distortedImg.rows), CV_32FC1,
                                map1, map2);

    std::cout << "new camera matrix" << std::endl
              << cameraMatrix << std::endl;

    cv::Mat undistortedImg;
    cv::remap(distortedImg, undistortedImg, map1, map2, cv::INTER_LINEAR);

    cv::Mat combinedImg;
    cv::hconcat(distortedImg, undistortedImg, combinedImg);

    cv::namedWindow("Distortion Method 1", cv::WINDOW_AUTOSIZE);
    cv::imshow("Distortion Method 1", combinedImg);
    cv::waitKey(0);
    cv::imwrite("../Results/TestResult/" + filePassed, combinedImg);
}

void undistortImages2(std::string filePassed, cv::Mat distortedImg, cv::Mat cameraMatrix, cv::Mat distCoeffs)
{
    /*
     * Undistort
     */
    cv::Mat undistortedImg;
    cv::undistort(distortedImg, undistortedImg, cameraMatrix, distCoeffs);

    cv::Mat combinedImg;
    cv::hconcat(distortedImg, undistortedImg, combinedImg);

    cv::namedWindow("Distortion Method 2", cv::WINDOW_AUTOSIZE);
    cv::imshow("Distortion Method 2", combinedImg);
    cv::waitKey(0);
    cv::imwrite("../Results/TestResult/" + filePassed, combinedImg);
}
