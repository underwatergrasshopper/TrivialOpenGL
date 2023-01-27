#include <stdio.h>
#include <TrivialTestKit.h>
#include <ToStr.h>

#include "TrivialOpenGL.h"

void TestTOGL_Point() {
    using TOGL::Point;

    // default constructor
    {
        Point<int> p;
        TTK_ASSERT_M(p.x == 0, ToStr("%d", p.x));
        TTK_ASSERT_M(p.y == 0, ToStr("%d", p.y));
    }
    {
        Point<float> p;
        TTK_ASSERT_M(p.x == 0.0f, ToStr("%f", p.x));
        TTK_ASSERT_M(p.y == 0.0f, ToStr("%f", p.y));
    }
    // parametrized constructor
    {
        Point<int> p(3);
        TTK_ASSERT_M(p.x == 3, ToStr("%d", p.x));
        TTK_ASSERT_M(p.y == 3, ToStr("%d", p.y));
    }
    {
        Point<int> p(3, 7);
        TTK_ASSERT_M(p.x == 3, ToStr("%d", p.x));
        TTK_ASSERT_M(p.y == 7, ToStr("%d", p.y));
    }
    // constructor with type casting
    {
        Point<int>      pi  = Point<int>(3, 7);
        Point<float>    p   = Point<float>(pi);

        TTK_ASSERT_M(p.x == 3.0f, ToStr("%f", p.x));
        TTK_ASSERT_M(p.y == 7.0f, ToStr("%f", p.y));
    }
    // assignment operator
    {
        Point<int>      p   = Point<int>(0);
        Point<int>      p2  = Point<int>(3, 7);

        p = p2;

        TTK_ASSERT_M(p.x == 3, ToStr("%d", p.x));
        TTK_ASSERT_M(p.y == 7, ToStr("%d", p.y));
    }

    // operators: ==, !=, >, <
    {
        TTK_ASSERT((Point<int>(10, 20)  ==  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(9, 20)   ==  Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(11, 20)  ==  Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(10, 19)  ==  Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(10, 21)  ==  Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(11, 21)  ==  Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(9, 18)   ==  Point<int>(10, 20)    == false));


        TTK_ASSERT((Point<int>(10, 20)  !=  Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(9, 20)   !=  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(11, 20)  !=  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(10, 19)  !=  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(10, 21)  !=  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(11, 21)  !=  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(9, 19)   !=  Point<int>(10, 20)    == true));

        TTK_ASSERT((Point<int>(10, 20)  >   Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(9, 20)   >   Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(11, 20)  >   Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(10, 19)  >   Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(10, 21)  >   Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(11, 21)  >   Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(9, 19)   >   Point<int>(10, 20)    == false));

        TTK_ASSERT((Point<int>(10, 20)  <   Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(9, 20)   <   Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(11, 20)  <   Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(10, 19)  <   Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(10, 21)  <   Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(11, 21)  <   Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(9, 19)   <   Point<int>(10, 20)    == true));

        TTK_ASSERT((Point<int>(10, 20)  >=  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(9, 20)   >=  Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(11, 20)  >=  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(10, 19)  >=  Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(10, 21)  >=  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(11, 21)  >=  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(9, 19)   >=  Point<int>(10, 20)    == false));
                                         
        TTK_ASSERT((Point<int>(10, 20)  <=  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(9, 20)   <=  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(11, 20)  <=  Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(10, 19)  <=  Point<int>(10, 20)    == true));
        TTK_ASSERT((Point<int>(10, 21)  <=  Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(11, 21)  <=  Point<int>(10, 20)    == false));
        TTK_ASSERT((Point<int>(9, 19)   <=  Point<int>(10, 20)    == true));
    }
    // operators: +, -, *, /
    {
        TTK_ASSERT(Point<int>(1, 2) + Point<int>(10, 20) == Point<int>(11, 22));
        TTK_ASSERT(Point<int>(10, 20) - Point<int>(1, 2) == Point<int>(9, 18));
        TTK_ASSERT(Point<int>(10, 20) * 3 == Point<int>(30, 60));
        TTK_ASSERT(Point<int>(10, 20) / 5 == Point<int>(2, 4));

        int i = 3;
        i += i;
    }
    // operators: +=, -=, *=, /=
    {
        Point<int> p(1, 10);
        p += p;
        TTK_ASSERT(p == Point<int>(2, 20));
    }
    {
        Point<int> p(10, 20);
        p += Point<int>(1, 2);
        TTK_ASSERT(p == Point<int>(11, 22));
    }
    {
        Point<int> p(1, 10);
        p -= p;
        TTK_ASSERT(p == Point<int>(0, 0));
    }
    {
        Point<int> p(10, 20);
        p -= Point<int>(1, 2);
        TTK_ASSERT(p == Point<int>(9, 18));
    }
    {
        Point<int> p(10, 20);
        p *= 3;
        TTK_ASSERT(p == Point<int>(30, 60));
    }
    {
        Point<int> p(10, 20);
        p /= 5;
        TTK_ASSERT(p == Point<int>(2, 4));
    }
}

int main() {
    TTK_ADD_TEST(TestTOGL_Point, 0);
    return !TTK_Run();
}