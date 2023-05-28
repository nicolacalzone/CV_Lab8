// STD
#include <iostream>
#include <string>
#include <vector>

// OPENCV
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/xfeatures2d.hpp>

// MIE
#include "functions/utils.hpp"

#define SQUARE_SIZE 21.5
#define INTERSECTIONS 54

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " /Path/to/Images/" << endl;
        return -1;
    }

    std::string directory = argv[1]; // disrectory name

    std::vector<cv::Mat> images;   // images = vector where images read are put
    readImages(directory, images); // reads all the images and puts them into a vector of images

    std::vector<cv::Point2f> corners;
    std::vector<std::vector<cv::Point2f>> imagePoints;
    cv::Size patternSize(9, 6); // interior number of corners
    cv::Mat gray;
    bool patternFound;

    for (int i = 0; i < images.size(); i++)
    {
        cv::cvtColor(images[i], gray, cv::COLOR_BGR2GRAY); // Convert the image to grayscale
        gray.convertTo(gray, CV_8U);

        patternFound = cv::findChessboardCorners(gray, patternSize, corners,
                                                 cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

        if (patternFound)
        {
            cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
                         cv::TermCriteria(cv::TermCriteria::Type::EPS + cv::TermCriteria::Type::MAX_ITER, 30, 0.1));
            imagePoints.push_back(corners);
        }

        cv::drawChessboardCorners(images[i], patternSize, cv::Mat(corners), patternFound);

        // cv::imshow(std::to_string(i), images[i]);
        // cv::waitKey();
    }

    cout << "width: " << patternSize.width;
    cout << "height: " << patternSize.height;

    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<cv::Point3f> obj;
    for (int i = 0; i < patternSize.height; i++)
    {
        for (int j = 0; j < patternSize.width; j++)
        {
            double resJ = j * SQUARE_SIZE;
            double resI = i * SQUARE_SIZE;
            obj.push_back(cv::Point3f(resJ, resI, 0));
        }
    }

    objectPoints.push_back(obj);

    objectPoints.resize(imagePoints.size(), objectPoints[0]);
    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;
    double rms = cv::calibrateCamera(objectPoints, imagePoints, images[0].size(), cameraMatrix,
                                     distCoeffs, rvecs, tvecs);

    // Compute the mean reprojection error
    double totalError = 0;
    int totalPoints = 0;
    for (size_t i = 0; i < objectPoints.size(); i++)
    {
        std::vector<cv::Point2f> imagePoints2;
        cv::projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix,
                          distCoeffs, imagePoints2);
        double error = cv::norm(imagePoints[i], imagePoints2, cv::NORM_L2);
        totalError += error * error;
        totalPoints += objectPoints[i].size();
    }
    double meanError = std::sqrt(totalError / totalPoints);

    cout << "Reprojection error: " << meanError << endl;
}