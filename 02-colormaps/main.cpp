
#include <iostream>
#include <limits>
#include <opencv2/opencv.hpp>
#include <cmath>

#include "Spline.h"


using std::size_t;


int main(int argc, char *argv[])
{

    std::cout.precision(4);
    std::cout << std::fixed;
    std::cout << "This program will test the Spline class with images." << std::endl;

    std::vector<cv::Mat> images;

    // Load images and make them all the same size
    std::cout << "Loading and resizing images..." << std::endl;
    size_t outputCols = 1280;
    size_t outputRows = 720;
    float outputAspectRatio = static_cast<float>(outputCols) / outputRows;

    for(size_t i = 1; i < argc; ++i)
    {
        // Load image and convert to Lab color space
        cv::Mat image = cv::imread(argv[i], CV_LOAD_IMAGE_COLOR);
        cv::cvtColor(image, image, cv::COLOR_BGR2Lab);

        // Convert to float
        image.convertTo(image, CV_32FC3);
        
        // Get aspect ratio of image
        double imageAspectRatio = static_cast<double>(image.cols) / image.rows;

        // Crop image to the output aspect ratio
        cv::Rect cropRect;
        if(imageAspectRatio > outputAspectRatio)
        {
            size_t croppedCols = std::round(image.rows * outputAspectRatio);
            cropRect.x = std::round((image.cols - croppedCols)/2.0);
            cropRect.y = 0;
            cropRect.width = croppedCols;
            cropRect.height = image.rows;
        }
        else
        {
            size_t croppedRows = std::round(image.cols / outputAspectRatio);
            cropRect.x = 0;
            cropRect.y = std::round((image.rows - croppedRows)/2.0);
            cropRect.width = image.cols;
            cropRect.height = croppedRows;
        }
        image = image(cropRect);

        // Resize the image
        cv::resize(image, image, cv::Size(outputCols, outputRows), 0, 0, CV_INTER_AREA);

        // Add image to the images vector
        images.push_back(image);
    }


    /*
    // View the images
    cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE);
    for(cv::Mat &image : images)
    {
        cv::Mat temp;
        cv::Mat temp2;
        image.convertTo(temp, CV_8UC3);
        cv::cvtColor(temp, temp2, cv::COLOR_Lab2BGR);
        std::cout << image.cols << "x" << image.rows << std::endl;
        cv::imshow("Display Window", temp2);
        cv::waitKey(0);
    }
    return 0;
    */
    
    
    // Create a spline of OpenCV images
    Spline<cv::Mat> imageSpline;
    double time = 0.0;
    std::cout << "Adding images to spline..." << std::endl;
    for(const cv::Mat &image : images)
    {
        imageSpline.addNode(time, image); time += 1.0;
        imageSpline.addNode(time, image); time += 1.0;
    }
    double finalTime = time;
    std::cout << "Calculating spline coefficients..." << std::endl;
    imageSpline.calculate();


    // Create an animation by stepping through the image spline
    cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE);
    cv::Mat image;
    for(time = 0.0; time <= finalTime; time += 1.0/60)
    {
        imageSpline.evaluate(time).convertTo(image, CV_8UC3);
        cv::cvtColor(image, image, cv::COLOR_Lab2BGR);
        cv::imshow("Display Window", image);
        cv::waitKey(1);
    }
    cv::waitKey(0);


    return 0;
}

