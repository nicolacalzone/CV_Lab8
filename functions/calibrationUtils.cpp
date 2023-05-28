#include "calibrationUtils.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

void computeObjectPts(std::vector<std::vector<cv::Point3f>> &objectPoints, // input & output
                      std::vector<std::vector<cv::Point2f>> &imagePoints,  // input & output
                      cv::Size patternSize, std::vector<cv::Point2f> corners)
{
    std::vector<cv::Point3f> obj;
    for (int i = 0; i < patternSize.height; i++)
    {
        for (int j = 0; j < patternSize.width; j++)
        {
            double resJ = (double)j * CHECKER_SIZE;
            double resI = (double)i * CHECKER_SIZE;
            obj.push_back(cv::Point3f(resJ, resI, 0));
        }
    }

    imagePoints.push_back(corners);
    objectPoints.push_back(obj);
    objectPoints.resize(imagePoints.size(), objectPoints[0]);
}

void computeChessboard(cv::Mat &image, cv::Size patternSize, std::vector<cv::Point2f> &corners, std::vector<std::vector<cv::Point2f>> &imagePoints)
{
    cv::Mat gray;
    bool patternFound;

    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY); // Convert the image to grayscale
    gray.convertTo(gray, CV_8U);

    patternFound = cv::findChessboardCorners(gray, patternSize, corners,
                                             cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

    if (patternFound)
    {
        cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
                     cv::TermCriteria(cv::TermCriteria::Type::EPS + cv::TermCriteria::Type::MAX_ITER, 30, 0.1));
    }

    cv::drawChessboardCorners(image, patternSize, cv::Mat(corners), patternFound);

    // cv::imshow(std::to_string(i), images[i]);
    // cv::waitKey();
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