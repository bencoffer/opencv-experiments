#ifndef SPLINE_H_
#define SPLINE_H_

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

        isCalculated_ = false;
    }

    void deleteNode(size_t index)
    {
        nodes_.erase(nodes_.begin() + index);
        isCalculated_ = false;
    }

    void calculate()
    {
        // Calculate the spline coefficients a_, b_, c_, d_
        // from the spline nodes_.
        
        // This function uses Algorithm 3.4 ("Natural Cubic Spline")
        // from the book "Numerical Analysis, 10th Ed" by Burden and Faires.
        // The algorithm has been generalized slightly by allowing the "y" values
        // to be of any type T that implements some basic arithmetic operators.
        // Specificly, we must be able to add or subtract values of type T and we
        // must be able to multiply or divide values of type T by a double.

        // Note: the code below is optimized more for readability
        //       than for computational efficiency.
        
        size_t i;
        size_t n = nodes_.size() - 1;

        // Note: there are  n  segments (0 thru n-1)
        //             and n+1  nodes   (0 thru  n )

        std::vector<double>      h(n);
        std::vector<T>       alpha(n);
        std::vector<double>      l(n+1);
        std::vector<double>     mu(n);
        std::vector<T>           z(n+1);

        // Calclate Spline Coefficients
        a_.resize(n+1);
        b_.resize(n);
        c_.resize(n+1);
        d_.resize(n);

        T zero = nodes_[0].y * 0.0;

        for(i = 0; i <= n; ++i)
            a_[i] = nodes_[i].y;

        for(i = 0; i < n; ++i)
            h[i] = nodes_[i+1].x - nodes_[i].x;

        for(i = 1; i < n; ++i)
            alpha[i] = 3*(  (a_[i+1] - a_[i])/h[i] + (a_[i] - a_[i-1])/h[i-1]  );

        l[0] = 1.0;
        mu[0] = 0.0;
        z[0] = zero;
        for(i = 1; i < n; ++i)
        {
            l[i] = 2*(nodes_[i+1].x - nodes_[i-1].x) - h[i-1]*mu[i-1];
            mu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i-1]*z[i-1]) / l[i];
        }

        l[n] = 1;
        z[n] = zero;
        c_[n] = zero;
        i = n;
        while(i-- > 0)
        {
            c_[i] = z[i] - mu[i]*c_[i+1];
            b_[i] = (a_[i+1] - a_[i])/h[i] - h[i]*(c_[i+1] - 2*c_[i])/3;
            d_[i] = (c_[i+1] - c_[i]) / (3*h[i]);
        }

        isCalculated_ = true;

    }

    T evaluate(double x)
    {
        if(!isCalculated_)
            calculate();

        // todo: evaluate the spline at position x
    }

    // Make nodes_ publicly accessible as a read-only property
    const std::vector<SplineNode<T> > &nodes() const
    {
        return nodes_;
    }

private:

    // Spline Nodes
    std::vector<SplineNode<T> > nodes_;

    // Spline Coefficients
    std::vector<T> a_;
    std::vector<T> b_;
    std::vector<T> c_;
    std::vector<T> d_;

    bool isCalculated_ = false;

};


#endif  // SPLINE_H_

