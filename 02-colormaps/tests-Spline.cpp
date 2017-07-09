
#include "catch.hpp"

#include "Spline.h"


SCENARIO("Spline nodes can be added and removed")
{
    GIVEN("a Spline with some nodes")
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

        THEN("the nodes will be strictly increasing in x")
        {
            auto nodes = s.nodes();
            bool nodesAreStrictlyIncreasing = true;

            for(size_t i = 1; i < nodes.size(); ++i)
            {
                if(nodes[i-1].x >= nodes[i].x)
                {
                    nodesAreStrictlyIncreasing = false;
                    break;
                }
            }

            REQUIRE(nodesAreStrictlyIncreasing == true);
        }
    }
}

