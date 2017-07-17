#include <iostream>
#include <limits>
#include <opencv2/opencv.hpp>
#include <cmath>

#include "Spline.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    Spline<double> s;
    s.addNode(0.0, exp(0.0));
    s.addNode(1.0, exp(1.0));
    s.addNode(2.0, exp(2.0));
    s.addNode(3.0, exp(3.0));
    s.calculate();
    
    cout << "Spline Coefficients:" << endl << endl;

    cout.precision(80);
    
    for(int i = 0; i < s.a().size(); ++i)
    {
        cout << "a[" << i << "] = " << s.a()[i] << endl;
        cout << "b[" << i << "] = " << s.b()[i] << endl;
        cout << "c[" << i << "] = " << s.c()[i] << endl;
        cout << "d[" << i << "] = " << s.d()[i] << endl;
        cout << endl;
    }

    return 0;
}

