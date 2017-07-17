
#include <limits>
#include <cmath>
#include "catch.hpp"
#include "Spline.h"

SCENARIO( "spline nodes can be added and deleted" )
{
    GIVEN( "a Spline with some nodes added" )
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

        THEN( "the nodes have the correct values and ordering" )
        {
            REQUIRE( s.nodes().size() == 7 );

            auto it = s.nodes().begin();
            REQUIRE( it->first == 0.99 ); REQUIRE( it->second == 3.099 ); it++;
            REQUIRE( it->first == 1.00 ); REQUIRE( it->second == 3.110 ); it++;
            REQUIRE( it->first == 1.01 ); REQUIRE( it->second == 3.101 ); it++;
            REQUIRE( it->first == 2.00 ); REQUIRE( it->second == 3.220 ); it++;
            REQUIRE( it->first == 3.00 ); REQUIRE( it->second == 3.300 ); it++;
            REQUIRE( it->first == 4.00 ); REQUIRE( it->second == 3.400 ); it++;
            REQUIRE( it->first == 5.00 ); REQUIRE( it->second == 3.550 ); it++;
        }

        WHEN( "several nodes are deleted" )
        {
            s.deleteNode(1.01);
            s.deleteNode(2.00);
            s.deleteNode(4.00);


            THEN( "the nodes have the correct values and ordering" )
            {
                REQUIRE( s.nodes().size() == 4 );

                auto it = s.nodes().begin();
                REQUIRE( it->first == 0.99 ); REQUIRE( it->second == 3.099 ); it++;
                REQUIRE( it->first == 1.00 ); REQUIRE( it->second == 3.110 ); it++;
                REQUIRE( it->first == 3.00 ); REQUIRE( it->second == 3.300 ); it++;
                REQUIRE( it->first == 5.00 ); REQUIRE( it->second == 3.550 ); it++;
            }
        }
    }
}

SCENARIO( "test some spline calculations" )
{
    GIVEN( "the spline from Burden, Faires - Numerical Analysis 10th Ed, Ch 3, Example 1" )
    {
        Spline<double> s;
        s.addNode(1.0, 2.0);
        s.addNode(2.0, 3.0);
        s.addNode(3.0, 5.0);
        s.calculate();

        THEN( "the nodes have the correct values and ordering" )
        {
            REQUIRE( s.nodes().size() == 3 );

            auto it = s.nodes().begin();
            REQUIRE( it->first == 1.0 ); REQUIRE( it->second == 2.0 ); it++;
            REQUIRE( it->first == 2.0 ); REQUIRE( it->second == 3.0 ); it++;
            REQUIRE( it->first == 3.0 ); REQUIRE( it->second == 5.0 ); it++;
        }

        THEN( "the spline has the correct coefficients" )
        {
            const std::vector<double> &a = s.a();
            const std::vector<double> &b = s.b();
            const std::vector<double> &c = s.c();
            const std::vector<double> &d = s.d();

            REQUIRE( a.size() == 2 );
            REQUIRE( b.size() == 2 );
            REQUIRE( c.size() == 2 );
            REQUIRE( d.size() == 2 );

            double tolerance = std::numeric_limits<double>::epsilon()*1000;

            CHECK( a[0] == Approx( 2.00 ).epsilon(tolerance) );
            CHECK( b[0] == Approx( 0.75 ).epsilon(tolerance) );
            CHECK( c[0] == Approx( 0.00 ).epsilon(tolerance) );
            CHECK( d[0] == Approx( 0.25 ).epsilon(tolerance) );

            CHECK( a[1] == Approx( 3.00 ).epsilon(tolerance) );
            CHECK( b[1] == Approx( 1.50 ).epsilon(tolerance) );
            CHECK( c[1] == Approx( 0.75 ).epsilon(tolerance) );
            CHECK( d[1] == Approx(-0.25 ).epsilon(tolerance) );
        }

        THEN( "the spline passes perfectly through its nodes" )
        {
            for(auto it = s.nodes().cbegin(); it != s.nodes().cend(); ++it)
            {
                REQUIRE( s.evaluate(it->first) == it->second );
            }
        }
    }


    GIVEN( "the spline from Burden, Faires - Numerical Analysis 10th Ed, Ch 3, Example 2" )
    {
        Spline<double> s;
        s.addNode(0.0, std::exp(0.0));
        s.addNode(1.0, std::exp(1.0));
        s.addNode(2.0, std::exp(2.0));
        s.addNode(3.0, std::exp(3.0));
        s.calculate();

        THEN( "the nodes have the correct values and ordering" )
        {
            REQUIRE( s.nodes().size() == 4 );

            auto it = s.nodes().begin();
            REQUIRE( it->first == 0.0 ); REQUIRE( it->second == std::exp(0.0) ); it++;
            REQUIRE( it->first == 1.0 ); REQUIRE( it->second == std::exp(1.0) ); it++;
            REQUIRE( it->first == 2.0 ); REQUIRE( it->second == std::exp(2.0) ); it++;
            REQUIRE( it->first == 3.0 ); REQUIRE( it->second == std::exp(3.0) ); it++;
        }

        THEN( "the spline has the correct coefficients" )
        {
            const std::vector<double> &a = s.a();
            const std::vector<double> &b = s.b();
            const std::vector<double> &c = s.c();
            const std::vector<double> &d = s.d();

            REQUIRE( a.size() == 3 );
            REQUIRE( b.size() == 3 );
            REQUIRE( c.size() == 3 );
            REQUIRE( d.size() == 3 );

            double tolerance = std::numeric_limits<double>::epsilon()*1000;

            CHECK( a[0] == Approx( 1.0000000000000000 ).epsilon(tolerance) );
            CHECK( b[0] == Approx( 1.4659976141747231 ).epsilon(tolerance) );
            CHECK( c[0] == Approx( 0.0000000000000000 ).epsilon(tolerance) );
            CHECK( d[0] == Approx( 0.2522842142843220 ).epsilon(tolerance) );

            CHECK( a[1] == Approx( 2.7182818284590451 ).epsilon(tolerance) );
            CHECK( b[1] == Approx( 2.2228502570276891 ).epsilon(tolerance) );
            CHECK( c[1] == Approx( 0.7568526428529661 ).epsilon(tolerance) );
            CHECK( d[1] == Approx( 1.6910713705909506 ).epsilon(tolerance) );

            CHECK( a[2] == Approx( 7.3890560989306504 ).epsilon(tolerance) );
            CHECK( b[2] == Approx( 8.8097696545064732 ).epsilon(tolerance) );
            CHECK( c[2] == Approx( 5.8300667546258174 ).epsilon(tolerance) );
            CHECK( d[2] == Approx(-1.9433555848752724 ).epsilon(tolerance) );
        }

        THEN( "the spline passes perfectly through its nodes" )
        {
            for(auto it = s.nodes().cbegin(); it != s.nodes().cend(); ++it)
            {
                REQUIRE( s.evaluate(it->first) == it->second );
            }
        }

        THEN( "this particular spline approximates the exponential function" )
        {
            CHECK( std::abs(std::exp(-0.10) - s.evaluate(-0.10)) < 0.2 );
            CHECK( std::abs(std::exp( 0.50) - s.evaluate( 0.50)) < 0.2 );
            CHECK( std::abs(std::exp( 1.70) - s.evaluate( 1.70)) < 0.2 );
            CHECK( std::abs(std::exp( 2.10) - s.evaluate( 2.10)) < 0.2 );
            CHECK( std::abs(std::exp( 3.01) - s.evaluate( 3.01)) < 0.2 );
        }
    }
}

