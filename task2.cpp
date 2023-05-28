// STD
#include <iostream>
#include <string>
#include <vector>

// OPENCV
#include <opencv2/opencv.hpp>

// MIE
#include "functions/utils.hpp"
#include "functions/calibrationUtils.hpp"

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
    std::vector<cv::Mat> images;     // images = vector where images read are put
    std::vector<cv::Point2f> corners;
    std::vector<std::vector<cv::Point2f>> imagePoints;

    cv::Size patternSize;
    if (directory.find("checkboard") != std::string::npos)
    {
        patternSize = cv::Size(6, 5); // interior number of corners
    }
    else
    {
        patternSize = cv::Size(9, 6); // interior number of corners
    }

    readImages(directory, images);

    // Reading all images and putting them into a vector

    for (int i = 0; i < images.size(); i++)
    {
        computeChessboard(images[i], patternSize, corners, imagePoints);
        std::string fileName = "../Results/ChessboardResult/result" + std::to_string(i);
        cv::imwrite(fileName + ".jpg", images[i]);
    }

    cout << "width: " << patternSize.width << endl;
    cout << "height: " << patternSize.height << endl;

    // Computing Object Points
    std::vector<std::vector<cv::Point3f>> objectPoints;
    computeObjectPts(objectPoints, imagePoints, patternSize, corners);

    // Print Object and Image points
    printPoints("Image", imagePoints);
    printPoints("Object", objectPoints);

    // Calibration of the Camera
    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rotationVec, translationVec;
    double rms = cv::calibrateCamera(objectPoints, imagePoints, images[0].size(), cameraMatrix,
                                     distCoeffs, rotationVec, translationVec);

    cout << cameraMatrix << endl;
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
        std::size_t pos = path.find_last_of("/\\");
        std::string fileToPass = path.substr(pos + 1);
        std::cout << fileToPass << std::endl;

        cv::Mat distortedImg = cv::imread(path, cv::IMREAD_COLOR);
        undistortImages1(fileToPass, distortedImg, cameraMatrix, distCoeffs);
        undistortImages2(fileToPass, distortedImg, cameraMatrix, distCoeffs);
    }
    else
    {
        std::cerr << "ERROR. Path does not exist.";
        return -1;
    }
}