#include <dirent.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "utils.hpp"

void readImages(std::string directory, std::vector<cv::Mat> &images)
{
    for (int i = 1; i <= 13; i++)
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