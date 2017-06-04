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
    
    void addNode(double x, T y)
    {
        if(nodes.size() == 0 || x > nodes.back().x)
        {
            // Add node to the end of the vector
            nodes.push_back(SplineNode<T>(x, y));
        }
        else
        {
            // We want the nodes vector to remain sorted by the x values.
            // So, we do a binary search to find the insertion point.
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

            if(nodes[end].x == x)
            {
                // If the new node has the same x value as an existing node,
                // then update the existing node with the new y value.
                nodes[end].y = y;
            }
            else
            {
                // This will insert the new node BEFORE nodes.begin()+end
                nodes.insert(nodes.begin()+end, SplineNode<T>(x, y));
            }
        }
    }

private:

    std::vector<SplineNode<T> > nodes;

};


#endif
