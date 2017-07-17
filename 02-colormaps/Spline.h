#ifndef SPLINE_H_
#define SPLINE_H_

#include <vector>
#include <map>
#include <iostream>


template <typename T>
class Spline
{
public:
    
    void addNode(double x, T y)
    {
        nodes_[x] = y;
        isCalculated_ = false;
    }

    void deleteNode(double x)
    {
        nodes_.erase(x);
        isCalculated_ = false;
    }

    void calculate()
    {
        // Calculate the spline coefficients a_, b_, c_, d_
        // from the spline nodes_.
        
        // This function uses Algorithm 3.4 ("Natural Cubic Spline")
        // from the book "Numerical Analysis, 10th Ed" by Burden and Faires.
        // The algorithm has been generalized slightly by allowing the "f(x)" values
        // to be of any type T that implements some basic arithmetic operators.
        // Specificly, we must be able to add or subtract values of type T and we
        // must be able to multiply or divide values of type T by a double.

        // Note: the code below is optimized more for readability
        //       than for computational efficiency.
        

        // Copy the node data into vectors x and a_

        std::vector<double>  x;
        a_.clear();
        for(auto it = nodes_.begin(); it != nodes_.end(); ++it)
        {
            x.push_back(it->first);
            a_.push_back(it->second);
        }


        // Calculate the remaining spline coefficients: b_, c_, d_
        // Note: there are  n  segments (0 thru n-1)
        //             and n+1  nodes   (0 thru  n )

        std::size_t i;
        std::size_t n = x.size() - 1;

        b_.resize(n);
        c_.resize(n+1);
        d_.resize(n);

        T zero = a_[0] * 0.0;

        std::vector<double>      h(n);
        std::vector<T>       alpha(n);
        std::vector<double>      l(n+1);
        std::vector<double>     mu(n);
        std::vector<T>           z(n+1);
        
        for(i = 0; i < n; ++i)
            h[i] = x[i+1] - x[i];

        for(i = 1; i < n; ++i)
            alpha[i] = 3*(  (a_[i+1] - a_[i])/h[i] - (a_[i] - a_[i-1])/h[i-1]  );

        l[0] = 1.0;
        mu[0] = 0.0;
        z[0] = zero;
        for(i = 1; i < n; ++i)
        {
            l[i] = 2*(x[i+1] - x[i-1]) - h[i-1]*mu[i-1];
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
            b_[i] = (a_[i+1] - a_[i])/h[i] - h[i]*(c_[i+1] + 2*c_[i])/3;
            d_[i] = (c_[i+1] - c_[i]) / (3*h[i]);
        }

        // Remove the extra values that are at the end of a_ and c_
        a_.pop_back();
        c_.pop_back();

        isCalculated_ = true;
    }


    T evaluate(double x)
    {
        if(!isCalculated_)
            calculate();

        // Find the node that is at the beginning of the spline segment containing x
        auto it = nodes_.lower_bound(x);
        
        if(it == nodes_.end())
        {
            // go back two
            --(--it);
        }
        else if(it == --nodes_.end())
        {
            // go back one
            --it;
        }

        auto i = std::distance(nodes_.begin(), it);

        // Get x relative to the node
        x = x - it->first;

        // Finally, evaluate the cubic polynomial
        return   a_[i]
               + b_[i] * x
               + c_[i] * std::pow(x,2)
               + d_[i] * std::pow(x,3) ;
    }


    // Make the private data publicly accessible as read-only properties

    const std::map<double, T> &nodes() const {return nodes_;}
    const std::vector<T> &a() const {return a_;}
    const std::vector<T> &b() const {return b_;}
    const std::vector<T> &c() const {return c_;}
    const std::vector<T> &d() const {return d_;}
    const bool &isCalculated() const {return isCalculated_;}

private:

    // Spline Nodes
    std::map<double, T> nodes_;

    // Spline Coefficients
    std::vector<T> a_;
    std::vector<T> b_;
    std::vector<T> c_;
    std::vector<T> d_;

    bool isCalculated_ = false;

};


#endif  // SPLINE_H_

