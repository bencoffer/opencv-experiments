
#include <limits>
#include "catch.hpp"
#include "Spline.h"

SCENARIO("Spline nodes can be added and deleted")
{
    GIVEN("a Spline with some nodes added")
    {
        Spline<double> s;
        
        s.addNode(1.00, 3.100);
        s.addNode(5.00, 3.500);
        s.addNode(3.00, 3.300);
        s.addNode(2.00, 3.200);
        s.addNode(4.00, 3.400);
        s.addNode(5.00, 3.550);
        s.addNode(1.00, 3.110);
        s.addNode(2.00, 3.220);
        s.addNode(1.01, 3.101);
        s.addNode(0.99, 3.099);

        THEN("the nodes have the correct values and ordering")
        {
            REQUIRE(s.nodes().size() == 7);

            auto it = s.nodes().begin();
            REQUIRE(it->first == 0.99); REQUIRE(it->second == 3.099); it++;
            REQUIRE(it->first == 1.00); REQUIRE(it->second == 3.110); it++;
            REQUIRE(it->first == 1.01); REQUIRE(it->second == 3.101); it++;
            REQUIRE(it->first == 2.00); REQUIRE(it->second == 3.220); it++;
            REQUIRE(it->first == 3.00); REQUIRE(it->second == 3.300); it++;
            REQUIRE(it->first == 4.00); REQUIRE(it->second == 3.400); it++;
            REQUIRE(it->first == 5.00); REQUIRE(it->second == 3.550); it++;
        }

        WHEN("several nodes are deleted")
        {
            s.deleteNode(1.01);
            s.deleteNode(2.00);
            s.deleteNode(4.00);


            THEN("the nodes have the correct values and ordering")
            {
                REQUIRE(s.nodes().size() == 4);

                auto it = s.nodes().begin();
                REQUIRE(it->first == 0.99); REQUIRE(it->second == 3.099); it++;
                REQUIRE(it->first == 1.00); REQUIRE(it->second == 3.110); it++;
                REQUIRE(it->first == 3.00); REQUIRE(it->second == 3.300); it++;
                REQUIRE(it->first == 5.00); REQUIRE(it->second == 3.550); it++;
            }
        }
    }
}
