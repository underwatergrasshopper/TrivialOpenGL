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
    // constructor width type casting
    {
        Point<int>      pi  = Point<int>(3, 7);
        Point<float>    p   = Point<float>(pi);

        TTK_ASSERT_M(p.x == 3.0f, ToStr("%f", p.x));
        TTK_ASSERT_M(p.y == 7.0f, ToStr("%f", p.y));
    }
    {
        Point<int>      pi  = Point<int>(3, 7);
        Point<int>      p   = pi;

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

void TestTOGL_Size() {
    using TOGL::Size;

    // default constructor
    {
        Size<int> s;
        TTK_ASSERT_M(s.width == 0, ToStr("%d", s.width));
        TTK_ASSERT_M(s.height == 0, ToStr("%d", s.height));
    }
    {
        Size<float> s;
        TTK_ASSERT_M(s.width == 0.0f, ToStr("%f", s.width));
        TTK_ASSERT_M(s.height == 0.0f, ToStr("%f", s.height));
    }
    // parametrized constructor
    {
        Size<int> s(3);
        TTK_ASSERT_M(s.width == 3, ToStr("%d", s.width));
        TTK_ASSERT_M(s.height == 3, ToStr("%d", s.height));
    }
    {
        Size<int> s(3, 7);
        TTK_ASSERT_M(s.width == 3, ToStr("%d", s.width));
        TTK_ASSERT_M(s.height == 7, ToStr("%d", s.height));
    }
    // constructor width type casting
    {
        Size<int>      si   = Size<int>(3, 7);
        Size<float>    s    = Size<float>(si);

        TTK_ASSERT_M(s.width == 3.0f, ToStr("%f", s.width));
        TTK_ASSERT_M(s.height == 7.0f, ToStr("%f", s.height));
    }
    {
        Size<int>      si   = Size<int>(3, 7);
        Size<int>      s    = si;

        TTK_ASSERT_M(s.width == 3.0f, ToStr("%f", s.width));
        TTK_ASSERT_M(s.height == 7.0f, ToStr("%f", s.height));
    }
    // assignment operator
    {
        Size<int>      s   = Size<int>(0);
        Size<int>      s2  = Size<int>(3, 7);

        s = s2;

        TTK_ASSERT_M(s.width == 3, ToStr("%d", s.width));
        TTK_ASSERT_M(s.height == 7, ToStr("%d", s.height));
    }

    // operators: ==, !=, >, <
    {
        TTK_ASSERT((Size<int>(10, 20)  ==  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(9, 20)   ==  Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(11, 20)  ==  Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(10, 19)  ==  Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(10, 21)  ==  Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(11, 21)  ==  Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(9, 18)   ==  Size<int>(10, 20)    == false));


        TTK_ASSERT((Size<int>(10, 20)  !=  Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(9, 20)   !=  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(11, 20)  !=  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(10, 19)  !=  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(10, 21)  !=  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(11, 21)  !=  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(9, 19)   !=  Size<int>(10, 20)    == true));

        TTK_ASSERT((Size<int>(10, 20)  >   Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(9, 20)   >   Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(11, 20)  >   Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(10, 19)  >   Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(10, 21)  >   Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(11, 21)  >   Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(9, 19)   >   Size<int>(10, 20)    == false));

        TTK_ASSERT((Size<int>(10, 20)  <   Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(9, 20)   <   Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(11, 20)  <   Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(10, 19)  <   Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(10, 21)  <   Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(11, 21)  <   Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(9, 19)   <   Size<int>(10, 20)    == true));

        TTK_ASSERT((Size<int>(10, 20)  >=  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(9, 20)   >=  Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(11, 20)  >=  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(10, 19)  >=  Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(10, 21)  >=  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(11, 21)  >=  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(9, 19)   >=  Size<int>(10, 20)    == false));

        TTK_ASSERT((Size<int>(10, 20)  <=  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(9, 20)   <=  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(11, 20)  <=  Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(10, 19)  <=  Size<int>(10, 20)    == true));
        TTK_ASSERT((Size<int>(10, 21)  <=  Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(11, 21)  <=  Size<int>(10, 20)    == false));
        TTK_ASSERT((Size<int>(9, 19)   <=  Size<int>(10, 20)    == true));
    }
    // operators: +, -, *, /
    {
        TTK_ASSERT(Size<int>(1, 2) + Size<int>(10, 20) == Size<int>(11, 22));
        TTK_ASSERT(Size<int>(10, 20) - Size<int>(1, 2) == Size<int>(9, 18));
        TTK_ASSERT(Size<int>(10, 20) * 3 == Size<int>(30, 60));
        TTK_ASSERT(Size<int>(10, 20) / 5 == Size<int>(2, 4));
    }
    // operators: +=, -=, *=, /=
    {
        Size<int> s(1, 10);
        s += s;
        TTK_ASSERT(s == Size<int>(2, 20));
    }
    {
        Size<int> s(10, 20);
        s += Size<int>(1, 2);
        TTK_ASSERT(s == Size<int>(11, 22));
    }
    {
        Size<int> s(1, 10);
        s -= s;
        TTK_ASSERT(s == Size<int>(0, 0));
    }
    {
        Size<int> s(10, 20);
        s -= Size<int>(1, 2);
        TTK_ASSERT(s == Size<int>(9, 18));
    }
    {
        Size<int> s(10, 20);
        s *= 3;
        TTK_ASSERT(s == Size<int>(30, 60));
    }
    {
        Size<int> s(10, 20);
        s /= 5;
        TTK_ASSERT(s == Size<int>(2, 4));
    }
}

void TestTOGL_Area() {
    using TOGL::Point;
    using TOGL::Size;
    using TOGL::Area;

    // default constructor
    {
        Area<int> a;
        TTK_ASSERT_M(a.x        == 0, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 0, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 0, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 0, ToStr("%d", a.height));
    }
    // parametrized constructor
    {
        Area<int> a(1, 2, 3, 4);
        TTK_ASSERT_M(a.x        == 1, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 2, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 3, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 4, ToStr("%d", a.height));
    }
    {
        Area<float> a2(1.0f, 2.0f, 3.0f, 4.0f);
        Area<int>   a = Area<int>(a2);
        TTK_ASSERT_M(a.x        == 1, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 2, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 3, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 4, ToStr("%d", a.height));
    }
    {
        Area<int> a(Point<int>(1, 2), Size<int>(3, 4));
        TTK_ASSERT_M(a.x        == 1, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 2, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 3, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 4, ToStr("%d", a.height));
    }
    // operator ==, !=
    {
        TTK_ASSERT((Area<int>(1, 2, 3, 4)       == Area<int>(1, 2, 3, 4)) == true);
        TTK_ASSERT((Area<int>(10, 2, 3, 4)      == Area<int>(1, 2, 3, 4)) == false);
        TTK_ASSERT((Area<int>(1, 20, 3, 4)      == Area<int>(1, 2, 3, 4)) == false);
        TTK_ASSERT((Area<int>(1, 2, 30, 4)      == Area<int>(1, 2, 3, 4)) == false);
        TTK_ASSERT((Area<int>(1, 2, 3, 40)      == Area<int>(1, 2, 3, 4)) == false);
        TTK_ASSERT((Area<int>(10, 20, 30, 40)   == Area<int>(1, 2, 3, 4)) == false);

        TTK_ASSERT((Area<int>(1, 2, 3, 4)       != Area<int>(1, 2, 3, 4)) == false);
        TTK_ASSERT((Area<int>(10, 2, 3, 4)      != Area<int>(1, 2, 3, 4)) == true);
        TTK_ASSERT((Area<int>(1, 20, 3, 4)      != Area<int>(1, 2, 3, 4)) == true);
        TTK_ASSERT((Area<int>(1, 2, 30, 4)      != Area<int>(1, 2, 3, 4)) == true);
        TTK_ASSERT((Area<int>(1, 2, 3, 40)      != Area<int>(1, 2, 3, 4)) == true);
        TTK_ASSERT((Area<int>(10, 20, 30, 40)   != Area<int>(1, 2, 3, 4)) == true);
    }
    // Get/Set
    {
        Area<int> a(1, 2, 3, 4);

        a.SetPoint({10, 20});
        TTK_ASSERT(a == Area<int>(10, 20, 3, 4));
        TTK_ASSERT(a.GetPoint() == Point<int>(10, 20));


        a.SetSize({30, 40});
        TTK_ASSERT(a == Area<int>(10, 20, 30, 40));
        TTK_ASSERT(a.GetSize() == Size<int>(30, 40));
    }
    // IsIn
    {
        Area<int> a(1, 2, 30, 40);

        TTK_ASSERT(a.IsIn(Point<int>()) == false);

        TTK_ASSERT(a.IsIn(Point<int>(1, 2)) == true);
        TTK_ASSERT(a.IsIn(Point<int>(0, 2)) == false);
        TTK_ASSERT(a.IsIn(Point<int>(1, 1)) == false);
        TTK_ASSERT(a.IsIn(Point<int>(0, 1)) == false);

        TTK_ASSERT(a.IsIn(Point<int>(30, 41)) == true);
        TTK_ASSERT(a.IsIn(Point<int>(31, 41)) == false);
        TTK_ASSERT(a.IsIn(Point<int>(30, 42)) == false);
        TTK_ASSERT(a.IsIn(Point<int>(32, 42)) == false);
    }
    // MakeRECT
    {
        Area<float> a(1, 2, 30, 40);
        RECT r = MakeRECT(a);

        TTK_ASSERT(r.left   == 1);
        TTK_ASSERT(r.top    == 2);
        TTK_ASSERT(r.right  == 31);
        TTK_ASSERT(r.bottom == 42);
    }
}

int main() {
    TTK_ADD_TEST(TestTOGL_Point, 0);
    TTK_ADD_TEST(TestTOGL_Size, 0);
    TTK_ADD_TEST(TestTOGL_Area, 0);
    return !TTK_Run();
}