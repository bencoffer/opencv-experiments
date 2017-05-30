#ifndef SPLINE_H
#define SPLINE_H

#include <vector>

template <typename T>
class SplineNode
{
public:
    double x;
    T y;
    SplineNode(double x, T y) : x(x), y(y) {}
};

template <typename T>
class Spline
{
public:
    std::vector<SplineNode<T> > nodes;
    
    void addNode(double x, T y)
    {
        if(nodes.size() == 0 || x > nodes.back().x)
        {
            nodes.push_back(SplineNode<T>(x, y));
        }
        else
        {
            auto end = nodes.size() - 1;
            decltype(end) start = -1;
            decltype(end) mid = end / 2;

            while(end - start > 1)
            {
                if(x > nodes[mid].x)
                    start = mid;
                else
                    end = mid;
                mid = (start + end) / 2;
            }

            if(x == nodes[end].x)
            {
                nodes[end].y = y;
            }
            else
            {
                // item is inserted BEFORE nodes.begin()+end
                nodes.insert(nodes.begin()+end, SplineNode<T>(x, y));
            }
        }
    }
};


/*
Spline<Vec3d> spline;

spline.addnode(double x, Vec3d y
*/


#endif
