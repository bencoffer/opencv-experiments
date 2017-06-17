#ifndef SPLINE_H
#define SPLINE_H

#include <vector>
//#include "TridiagonalMatrix.h"

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
    
    void addNode(double x, T y)
    {
        if(nodes_.size() == 0 || x > nodes_.back().x)
        {
            // Add node to the end of the vector
            nodes_.push_back(SplineNode<T>(x, y));
        }
        else
        {
            // We want the nodes vector to remain sorted by the x values.
            // So, we do a binary search to find the insertion point.
            auto end = nodes_.size() - 1;
            decltype(end) start = -1;
            decltype(end) mid = end / 2;
            while(end - start > 1)
            {
                if(x > nodes_[mid].x)
                    start = mid;
                else
                    end = mid;
                mid = (start + end) / 2;
            }

            if(nodes_[end].x == x)
            {
                // If the new node has the same x value as an existing node,
                // then update the existing node with the new y value.
                nodes_[end].y = y;
            }
            else
            {
                // This will insert the new node BEFORE nodes.begin()+end
                nodes_.insert(nodes_.begin()+end, SplineNode<T>(x, y));
            }
        }
    }

    // Make nodes_ publicly accessible as a read-only property
    const std::vector<SplineNode<T> > &nodes() const
    {
        return nodes_;
    }

private:

    std::vector<SplineNode<T> > nodes_;

};


#endif
