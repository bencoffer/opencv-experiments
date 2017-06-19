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
    
    for(SplineNode<double> const &node : s.nodes())
    {
        cout << node.x << "\t" << node.y << endl;
    }

    // auto n = s.nodes();                             // note: using auto n causes n to be a copy, not a const reference.
    // const auto &n = s.nodes();                      // But you can do this
    const vector<SplineNode<double> > &n = s.nodes();  // Or this

    cout << endl;
    cout << n[2].x << endl;
    cout << n[2].y << endl;
    cout << endl;

    // n[2].x = 21.2;  // these throw an error as they should
    // n[2].y = 31.3;
    
    s.deleteNode(3);

    for(SplineNode<double> const &node : s.nodes())
    {
        cout << node.x << "\t" << node.y << endl;
    }

    return 0;
}

