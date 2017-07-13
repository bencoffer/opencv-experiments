#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

#include "Spline.h"

using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
    Spline<double> s;

    s.addNode(1.0, 3.1);
    s.addNode(5.0, 3.5);
    s.addNode(3.0, 3.3);
    s.addNode(2.0, 3.2);
    s.addNode(4.0, 3.4);
    s.addNode(5.0, 3.55);
    s.addNode(1.0, 3.11);
    s.addNode(2.0, 3.22);
    s.addNode(1.01, 3.101);
    s.addNode(0.99, 3.099);

    
    cout << "Spline Nodes:" << endl;
    
    for(pair<double, double> const &node : s.nodes())
    {
        cout << node.first << "\t" << node.second << endl;
    }

    s.calculate();

    return 0;
}

