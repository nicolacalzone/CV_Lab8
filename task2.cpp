// STD
#include <iostream>
#include <string>
#include <vector>

// OPENCV
#include <opencv2/opencv.hpp>

// MIE
#include "functions/utils.hpp"

#define CHECKER_SIZE 21.5 // about 2,15 cm per square

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
        }

        cv::drawChessboardCorners(images[i], patternSize, cv::Mat(corners), patternFound);

        // cv::imshow(std::to_string(i), images[i]);
        // cv::waitKey();
        std::string fileName = "../Results/result" + std::to_string(i);
        cv::imwrite(fileName + ".jpg", images[i]);
    }

    cout << "width: " << patternSize.width << endl;
    cout << "height: " << patternSize.height << endl;

    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<cv::Point3f> obj;
    for (int i = 0; i < patternSize.height; i++)
    {
        for (int j = 0; j < patternSize.width; j++)
        {
            double resJ = j * CHECKER_SIZE;
            double resI = i * CHECKER_SIZE;
            obj.push_back(cv::Point3f(resJ, resI, 0));
        }
    }

    imagePoints.push_back(corners);
    objectPoints.push_back(obj);
    objectPoints.resize(imagePoints.size(), objectPoints[0]);

    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rotationVec, translationVec;
    double rms = cv::calibrateCamera(objectPoints, imagePoints, images[0].size(), cameraMatrix,
                                     distCoeffs, rotationVec, translationVec);
    cout << "RMS: " << rms << endl;

    // Computing mean reprojection error
    double meanError = computeReprojectionError(cameraMatrix, distCoeffs,
                                                rotationVec, translationVec,
                                                imagePoints, objectPoints);
    cout << "Mean Reprojection Error: " << meanError << endl;

    // Undistorting input image
    std::cout << " ## UNDISTORT IMAGES ##" << endl
              << "Please enter path to an image: ";
    std::string path;
    std::getline(std::cin, path);

    if (!path.empty())
    {
        cv::Mat distortedImg = cv::imread(path, cv::IMREAD_COLOR);
        undistortImages1(distortedImg, cameraMatrix, distCoeffs);
        undistortImages2(distortedImg, cameraMatrix, distCoeffs);
    }
    else
    {
        std::cerr << "ERROR. Path does not exist.";
        return -1;
    }
}