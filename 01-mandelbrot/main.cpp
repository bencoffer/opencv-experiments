#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
    const int max_iterations = 1000;

    const int image_height = 1000;
    const int image_width = 1000;

    Mat image(image_height, image_width, CV_8UC3);

    const double window_startx = -2.0;
    const double window_width = 3.0;
    const double window_starty = 1.0;
    const double window_height = 2.0;

    int row, col, n, escape_time;
    double x, y, cx, cy, temp;

    for(row = 0; row < image_height; ++row)
    {
        for(col = 0; col < image_width; ++col)
        {
            cx = window_startx + col*window_width/image_width;
            cy = window_starty - row*window_height/image_height;

            x = cx;
            y = cy;

            escape_time = max_iterations;

            for(n = 0; n < max_iterations; ++n)
            {
                temp = x;
                x = x*x - y*y + cx;
                y = 2*temp*y + cy;

                if (x*x + y*y > 4)
                {
                    escape_time = n;
                    break;
                }
            }

            if(escape_time == max_iterations)
            {
                image.at<uchar>(row, col, 0) = 0;
            }
            else
            {
                image.at<Vec3b>(row, col)[0] = 255;
                image.at<Vec3b>(row, col)[1] = 255;
                image.at<Vec3b>(row, col)[2] = 255;
            }
        }
    }

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image);

    waitKey(0);

    return 0;
}

