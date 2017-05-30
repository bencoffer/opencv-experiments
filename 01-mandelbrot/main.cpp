#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
    const int max_iterations = 1000;

    const double window_startx = -2.0;
    const double window_width = 3.0;
    const double window_starty = 1.2;
    const double window_height = 1.2;

    const int image_width = 2000;
    const int image_height = round(image_width * window_height / window_width);
    
    Mat image(image_height, image_width, CV_8UC3);

    int row, col, n;
    double x, y, cx, cy, temp;
    double shade;
    int B, G, R;

    // Loop through each pixel of the output image
    for(row = 0; row < image_height; ++row)
    {
        for(col = 0; col < image_width; ++col)
        {

            // Get the point (cx, cy) corresponding to the current pixel
            cx = window_startx + col*window_width/image_width;
            cy = window_starty - row*window_height/image_height;


            // Calculate the escape time n for point (cx, cy)
            x = cx;
            y = cy;
            for(n = 0; n < max_iterations; ++n)
            {
                temp = x;
                x = x*x - y*y + cx;
                y = 2*temp*y + cy;

                if (x*x + y*y > 256)
                    break;
            }


            // Calculate a smooth shade
            if(n == max_iterations)
                shade = n;
            else
                shade = n + 1 - log(log(sqrt(x*x + y*y)))/log(2);

            shade = sqrt(shade / max_iterations);


            // Calculate a pretty color based on the shade
            const double breakpoint = 0.30;
            if (shade < breakpoint)
            {
                shade = shade / breakpoint;
                B = shade*240;
                G = shade*180;
                R = shade*190;
            }
            else
            {
                shade = (shade - breakpoint) / (1 - breakpoint);
                B = (1-shade)*240 + shade*255;
                G = (1-shade)*180 + shade*255;
                R = (1-shade)*190 + shade*255;
            }

            // Set the pixel color
            image.at<Vec3b>(row, col) = Vec3b(B, G, R);
        }
    }

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image);

    waitKey(0);

    return 0;
}

