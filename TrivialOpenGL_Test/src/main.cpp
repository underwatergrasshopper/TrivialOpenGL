#include <stdio.h>
#include <string>
#include <set>

#include <TrivialTestKit.h>
#include <ToStr.h>

#include "TrivialOpenGL.h"

//--------------------------------------------------------------------------

template <unsigned N>
std::string CodeToTextUTF8(const uint8_t (&codes)[N]) {
    return std::string((const char*)codes);
}

template <unsigned N>
std::wstring CodeToTextUTF16(const uint16_t (&codes)[N]) {
    return std::wstring((const wchar_t*)codes);
}

//--------------------------------------------------------------------------

void TestTOGL_Point() {
    // default constructor
    {
        TOGL_Point<int> p;
        TTK_ASSERT_M(p.x == 0, ToStr("%d", p.x));
        TTK_ASSERT_M(p.y == 0, ToStr("%d", p.y));
    }
    {
        TOGL_Point<float> p;
        TTK_ASSERT_M(p.x == 0.0f, ToStr("%f", p.x));
        TTK_ASSERT_M(p.y == 0.0f, ToStr("%f", p.y));
    }
    // parametrized constructor
    {
        TOGL_Point<int> p(3);
        TTK_ASSERT_M(p.x == 3, ToStr("%d", p.x));
        TTK_ASSERT_M(p.y == 3, ToStr("%d", p.y));
    }
    {
        TOGL_Point<int> p(3, 7);
        TTK_ASSERT_M(p.x == 3, ToStr("%d", p.x));
        TTK_ASSERT_M(p.y == 7, ToStr("%d", p.y));
    }
    // constructor width type casting
    {
        TOGL_Point<int>      pi  = TOGL_Point<int>(3, 7);
        TOGL_Point<float>    p   = TOGL_Point<float>(pi);

        TTK_ASSERT_M(p.x == 3.0f, ToStr("%f", p.x));
        TTK_ASSERT_M(p.y == 7.0f, ToStr("%f", p.y));
    }
    {
        TOGL_Point<int>      pi  = TOGL_Point<int>(3, 7);
        TOGL_Point<int>      p   = pi;

        TTK_ASSERT_M(p.x == 3.0f, ToStr("%f", p.x));
        TTK_ASSERT_M(p.y == 7.0f, ToStr("%f", p.y));
    }
    // assignment operator
    {
        TOGL_Point<int>      p   = TOGL_Point<int>(0);
        TOGL_Point<int>      p2  = TOGL_Point<int>(3, 7);

        p = p2;

        TTK_ASSERT_M(p.x == 3, ToStr("%d", p.x));
        TTK_ASSERT_M(p.y == 7, ToStr("%d", p.y));
    }

    // operators: ==, !=, >, <
    {
        TTK_ASSERT((TOGL_Point<int>(10, 20)  ==  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(9, 20)   ==  TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(11, 20)  ==  TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(10, 19)  ==  TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(10, 21)  ==  TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(11, 21)  ==  TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(9, 18)   ==  TOGL_Point<int>(10, 20)    == false));


        TTK_ASSERT((TOGL_Point<int>(10, 20)  !=  TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(9, 20)   !=  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(11, 20)  !=  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(10, 19)  !=  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(10, 21)  !=  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(11, 21)  !=  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(9, 19)   !=  TOGL_Point<int>(10, 20)    == true));

        TTK_ASSERT((TOGL_Point<int>(10, 20)  >   TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(9, 20)   >   TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(11, 20)  >   TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(10, 19)  >   TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(10, 21)  >   TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(11, 21)  >   TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(9, 19)   >   TOGL_Point<int>(10, 20)    == false));

        TTK_ASSERT((TOGL_Point<int>(10, 20)  <   TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(9, 20)   <   TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(11, 20)  <   TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(10, 19)  <   TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(10, 21)  <   TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(11, 21)  <   TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(9, 19)   <   TOGL_Point<int>(10, 20)    == true));

        TTK_ASSERT((TOGL_Point<int>(10, 20)  >=  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(9, 20)   >=  TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(11, 20)  >=  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(10, 19)  >=  TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(10, 21)  >=  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(11, 21)  >=  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(9, 19)   >=  TOGL_Point<int>(10, 20)    == false));
                                         
        TTK_ASSERT((TOGL_Point<int>(10, 20)  <=  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(9, 20)   <=  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(11, 20)  <=  TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(10, 19)  <=  TOGL_Point<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Point<int>(10, 21)  <=  TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(11, 21)  <=  TOGL_Point<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Point<int>(9, 19)   <=  TOGL_Point<int>(10, 20)    == true));
    }
    // operators: +, -, *, /
    {
        TTK_ASSERT(TOGL_Point<int>(1, 2) + TOGL_Point<int>(10, 20) == TOGL_Point<int>(11, 22));
        TTK_ASSERT(TOGL_Point<int>(10, 20) - TOGL_Point<int>(1, 2) == TOGL_Point<int>(9, 18));
        TTK_ASSERT(TOGL_Point<int>(10, 20) * 3 == TOGL_Point<int>(30, 60));
        TTK_ASSERT(TOGL_Point<int>(10, 20) / 5 == TOGL_Point<int>(2, 4));

        int i = 3;
        i += i;
    }
    // operators: +=, -=, *=, /=
    {
        TOGL_Point<int> p(1, 10);
        p += p;
        TTK_ASSERT(p == TOGL_Point<int>(2, 20));
    }
    {
        TOGL_Point<int> p(10, 20);
        p += TOGL_Point<int>(1, 2);
        TTK_ASSERT(p == TOGL_Point<int>(11, 22));
    }
    {
        TOGL_Point<int> p(1, 10);
        p -= p;
        TTK_ASSERT(p == TOGL_Point<int>(0, 0));
    }
    {
        TOGL_Point<int> p(10, 20);
        p -= TOGL_Point<int>(1, 2);
        TTK_ASSERT(p == TOGL_Point<int>(9, 18));
    }
    {
        TOGL_Point<int> p(10, 20);
        p *= 3;
        TTK_ASSERT(p == TOGL_Point<int>(30, 60));
    }
    {
        TOGL_Point<int> p(10, 20);
        p /= 5;
        TTK_ASSERT(p == TOGL_Point<int>(2, 4));
    }
}

void TestTOGL_Size() {
    // default constructor
    {
        TOGL_Size<int> s;
        TTK_ASSERT_M(s.width == 0, ToStr("%d", s.width));
        TTK_ASSERT_M(s.height == 0, ToStr("%d", s.height));
    }
    {
        TOGL_Size<float> s;
        TTK_ASSERT_M(s.width == 0.0f, ToStr("%f", s.width));
        TTK_ASSERT_M(s.height == 0.0f, ToStr("%f", s.height));
    }
    // parametrized constructor
    {
        TOGL_Size<int> s(3);
        TTK_ASSERT_M(s.width == 3, ToStr("%d", s.width));
        TTK_ASSERT_M(s.height == 3, ToStr("%d", s.height));
    }
    {
        TOGL_Size<int> s(3, 7);
        TTK_ASSERT_M(s.width == 3, ToStr("%d", s.width));
        TTK_ASSERT_M(s.height == 7, ToStr("%d", s.height));
    }
    // constructor width type casting
    {
        TOGL_Size<int>      si   = TOGL_Size<int>(3, 7);
        TOGL_Size<float>    s    = TOGL_Size<float>(si);

        TTK_ASSERT_M(s.width == 3.0f, ToStr("%f", s.width));
        TTK_ASSERT_M(s.height == 7.0f, ToStr("%f", s.height));
    }
    {
        TOGL_Size<int>      si   = TOGL_Size<int>(3, 7);
        TOGL_Size<int>      s    = si;

        TTK_ASSERT_M(s.width == 3.0f, ToStr("%f", s.width));
        TTK_ASSERT_M(s.height == 7.0f, ToStr("%f", s.height));
    }
    // assignment operator
    {
        TOGL_Size<int>      s   = TOGL_Size<int>(0);
        TOGL_Size<int>      s2  = TOGL_Size<int>(3, 7);

        s = s2;

        TTK_ASSERT_M(s.width == 3, ToStr("%d", s.width));
        TTK_ASSERT_M(s.height == 7, ToStr("%d", s.height));
    }

    // operators: ==, !=, >, <
    {
        TTK_ASSERT((TOGL_Size<int>(10, 20)  ==  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(9, 20)   ==  TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(11, 20)  ==  TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(10, 19)  ==  TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(10, 21)  ==  TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(11, 21)  ==  TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(9, 18)   ==  TOGL_Size<int>(10, 20)    == false));


        TTK_ASSERT((TOGL_Size<int>(10, 20)  !=  TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(9, 20)   !=  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(11, 20)  !=  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(10, 19)  !=  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(10, 21)  !=  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(11, 21)  !=  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(9, 19)   !=  TOGL_Size<int>(10, 20)    == true));

        TTK_ASSERT((TOGL_Size<int>(10, 20)  >   TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(9, 20)   >   TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(11, 20)  >   TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(10, 19)  >   TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(10, 21)  >   TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(11, 21)  >   TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(9, 19)   >   TOGL_Size<int>(10, 20)    == false));

        TTK_ASSERT((TOGL_Size<int>(10, 20)  <   TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(9, 20)   <   TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(11, 20)  <   TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(10, 19)  <   TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(10, 21)  <   TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(11, 21)  <   TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(9, 19)   <   TOGL_Size<int>(10, 20)    == true));

        TTK_ASSERT((TOGL_Size<int>(10, 20)  >=  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(9, 20)   >=  TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(11, 20)  >=  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(10, 19)  >=  TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(10, 21)  >=  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(11, 21)  >=  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(9, 19)   >=  TOGL_Size<int>(10, 20)    == false));

        TTK_ASSERT((TOGL_Size<int>(10, 20)  <=  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(9, 20)   <=  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(11, 20)  <=  TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(10, 19)  <=  TOGL_Size<int>(10, 20)    == true));
        TTK_ASSERT((TOGL_Size<int>(10, 21)  <=  TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(11, 21)  <=  TOGL_Size<int>(10, 20)    == false));
        TTK_ASSERT((TOGL_Size<int>(9, 19)   <=  TOGL_Size<int>(10, 20)    == true));
    }
    // operators: +, -, *, /
    {
        TTK_ASSERT(TOGL_Size<int>(1, 2) + TOGL_Size<int>(10, 20) == TOGL_Size<int>(11, 22));
        TTK_ASSERT(TOGL_Size<int>(10, 20) - TOGL_Size<int>(1, 2) == TOGL_Size<int>(9, 18));
        TTK_ASSERT(TOGL_Size<int>(10, 20) * 3 == TOGL_Size<int>(30, 60));
        TTK_ASSERT(TOGL_Size<int>(10, 20) / 5 == TOGL_Size<int>(2, 4));
    }
    // operators: +=, -=, *=, /=
    {
        TOGL_Size<int> s(1, 10);
        s += s;
        TTK_ASSERT(s == TOGL_Size<int>(2, 20));
    }
    {
        TOGL_Size<int> s(10, 20);
        s += TOGL_Size<int>(1, 2);
        TTK_ASSERT(s == TOGL_Size<int>(11, 22));
    }
    {
        TOGL_Size<int> s(1, 10);
        s -= s;
        TTK_ASSERT(s == TOGL_Size<int>(0, 0));
    }
    {
        TOGL_Size<int> s(10, 20);
        s -= TOGL_Size<int>(1, 2);
        TTK_ASSERT(s == TOGL_Size<int>(9, 18));
    }
    {
        TOGL_Size<int> s(10, 20);
        s *= 3;
        TTK_ASSERT(s == TOGL_Size<int>(30, 60));
    }
    {
        TOGL_Size<int> s(10, 20);
        s /= 5;
        TTK_ASSERT(s == TOGL_Size<int>(2, 4));
    }
}

void TestTOGL_Area() {
    // default constructor
    {
        TOGL_Area<int> a;
        TTK_ASSERT_M(a.x        == 0, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 0, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 0, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 0, ToStr("%d", a.height));
    }
    // parametrized constructor
    {
        TOGL_Area<int> a(1, 2, 3, 4);
        TTK_ASSERT_M(a.x        == 1, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 2, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 3, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 4, ToStr("%d", a.height));
    }
    {
        TOGL_Area<float> a2(1.0f, 2.0f, 3.0f, 4.0f);
        TOGL_Area<int>   a = TOGL_Area<int>(a2);
        TTK_ASSERT_M(a.x        == 1, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 2, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 3, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 4, ToStr("%d", a.height));
    }
    {
        TOGL_Area<int> a(TOGL_Point<int>(1, 2), TOGL_Size<int>(3, 4));
        TTK_ASSERT_M(a.x        == 1, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 2, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 3, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 4, ToStr("%d", a.height));
    }
    {
        TOGL_Area<int, unsigned> a(TOGL_Point<int>(1, 2), TOGL_Size<unsigned>(3, 4));
        TTK_ASSERT_M(a.x        == 1, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 2, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 3, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 4, ToStr("%d", a.height));
    }
    {
        TOGL_Area<int> a(TOGL_Point<int>(1, 2));
        TTK_ASSERT_M(a.x        == 1, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 2, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 0, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 0, ToStr("%d", a.height));
    }
    {
        TOGL_Area<int> a(TOGL_Size<int>(3, 4));
        TTK_ASSERT_M(a.x        == 0, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 0, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 3, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 4, ToStr("%d", a.height));
    }
    // operator ==, !=
    {
        TTK_ASSERT((TOGL_Area<int>(1, 2, 3, 4)       == TOGL_Area<int>(1, 2, 3, 4)) == true);
        TTK_ASSERT((TOGL_Area<int>(10, 2, 3, 4)      == TOGL_Area<int>(1, 2, 3, 4)) == false);
        TTK_ASSERT((TOGL_Area<int>(1, 20, 3, 4)      == TOGL_Area<int>(1, 2, 3, 4)) == false);
        TTK_ASSERT((TOGL_Area<int>(1, 2, 30, 4)      == TOGL_Area<int>(1, 2, 3, 4)) == false);
        TTK_ASSERT((TOGL_Area<int>(1, 2, 3, 40)      == TOGL_Area<int>(1, 2, 3, 4)) == false);
        TTK_ASSERT((TOGL_Area<int>(10, 20, 30, 40)   == TOGL_Area<int>(1, 2, 3, 4)) == false);

        TTK_ASSERT((TOGL_Area<int>(1, 2, 3, 4)       != TOGL_Area<int>(1, 2, 3, 4)) == false);
        TTK_ASSERT((TOGL_Area<int>(10, 2, 3, 4)      != TOGL_Area<int>(1, 2, 3, 4)) == true);
        TTK_ASSERT((TOGL_Area<int>(1, 20, 3, 4)      != TOGL_Area<int>(1, 2, 3, 4)) == true);
        TTK_ASSERT((TOGL_Area<int>(1, 2, 30, 4)      != TOGL_Area<int>(1, 2, 3, 4)) == true);
        TTK_ASSERT((TOGL_Area<int>(1, 2, 3, 40)      != TOGL_Area<int>(1, 2, 3, 4)) == true);
        TTK_ASSERT((TOGL_Area<int>(10, 20, 30, 40)   != TOGL_Area<int>(1, 2, 3, 4)) == true);
    }
    // Get/Set
    {
        TOGL_Area<int> a(1, 2, 3, 4);

        a.SetPos({10, 20});
        TTK_ASSERT(a == TOGL_Area<int>(10, 20, 3, 4));
        TTK_ASSERT(a.GetPos() == TOGL_Point<int>(10, 20));


        a.SetSize({30, 40});
        TTK_ASSERT(a == TOGL_Area<int>(10, 20, 30, 40));
        TTK_ASSERT(a.GetSize() == TOGL_Size<int>(30, 40));
    }
    // IsIn
    {
        TOGL_Area<int> a(1, 2, 30, 40);

        TTK_ASSERT(a.IsIn(TOGL_Point<int>()) == false);

        TTK_ASSERT(a.IsIn(TOGL_Point<int>(1, 2)) == true);
        TTK_ASSERT(a.IsIn(TOGL_Point<int>(0, 2)) == false);
        TTK_ASSERT(a.IsIn(TOGL_Point<int>(1, 1)) == false);
        TTK_ASSERT(a.IsIn(TOGL_Point<int>(0, 1)) == false);

        TTK_ASSERT(a.IsIn(TOGL_Point<int>(30, 41)) == true);
        TTK_ASSERT(a.IsIn(TOGL_Point<int>(31, 41)) == false);
        TTK_ASSERT(a.IsIn(TOGL_Point<int>(30, 42)) == false);
        TTK_ASSERT(a.IsIn(TOGL_Point<int>(32, 42)) == false);
    }
    // MakeRECT
    {
        TOGL_Area<float> a(1, 2, 30, 40);
        RECT r = MakeRECT(a);

        TTK_ASSERT(r.left   == 1);
        TTK_ASSERT(r.top    == 2);
        TTK_ASSERT(r.right  == 31);
        TTK_ASSERT(r.bottom == 42);
    }
}

void TestTOGL_ASCII_ToUTF16() {
    TTK_ASSERT(TOGL_ASCII_ToUTF16("") == L"");
    TTK_ASSERT(TOGL_ASCII_ToUTF16("Some Text.") == L"Some Text.");
}

void TestTOGL_ToUTF16() {
    // empty text
    TTK_ASSERT(TOGL_ToUTF16(u8"") == L"");

    // unicode characters
    TTK_ASSERT(TOGL_ToUTF16(u8"Some text\u0444\U0002F820.") == L"Some text\u0444\U0002F820.");

    // long text
    {
        const std::string sequence_utf8 =  
            u8"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. " 
            u8"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. " 
            u8"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. " 
            u8"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. ";

        const std::wstring sequence_utf16 =  
            L"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. " 
            L"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. " 
            L"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. " 
            L"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. ";

        std::string long_text_utf8;
        std::wstring long_text_utf16;

        const size_t number = _TOGL_CONVERSION_STACK_BUFFER_LENGTH * 2 / sequence_utf16.length();
        for (size_t ix = 0; ix < number; ++ix) {
            long_text_utf8  += sequence_utf8;
            long_text_utf16 += sequence_utf16;
        }

        TTK_ASSERT(TOGL_ToUTF16(long_text_utf8) == long_text_utf16);
    }

    // wrong encoding
    {
        TTK_ASSERT(TOGL_ToUTF16(CodeToTextUTF8({'t', 'e', 'x', 't', '\0'})) == CodeToTextUTF16({'t', 'e', 'x', 't', '\0'})); // correct, control one
        // U+FFFD - Replacement Character
        TTK_ASSERT(TOGL_ToUTF16(CodeToTextUTF8({0xC2, 'e', 'x', 't', '\0'})) == CodeToTextUTF16({0xFFFD, 'e', 'x', 't', '\0'}));
        TTK_ASSERT(TOGL_ToUTF16(CodeToTextUTF8({0xFF, 'e', 'x', 't', '\0'})) == CodeToTextUTF16({0xFFFD, 'e', 'x', 't', '\0'}));
    }

}

void TestTOGL_ToUTF8() {
    // empty text
    TTK_ASSERT(TOGL_ToUTF8(L"") == u8"");

    // unicode characters
    TTK_ASSERT(TOGL_ToUTF8(L"Some text\u0444\U0002F820.") == u8"Some text\u0444\U0002F820.");

    // long text
    {
        const std::string sequence_utf8 =  
            u8"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. " 
            u8"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. " 
            u8"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. " 
            u8"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. ";

        const std::wstring sequence_utf16 =  
            L"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. " 
            L"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. " 
            L"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. " 
            L"Some text\u0444. Some text\u0444. Some text\u0444. Some text\u0444. ";

        std::string long_text_utf8;
        std::wstring long_text_utf16;

        const size_t number = _TOGL_CONVERSION_STACK_BUFFER_LENGTH * 2 / sequence_utf8.length();
        for (size_t ix = 0; ix < number; ++ix) {
            long_text_utf8  += sequence_utf8;
            long_text_utf16 += sequence_utf16;
        }

        TTK_ASSERT(TOGL_ToUTF8(long_text_utf16) == long_text_utf8);
    }

    // wrong encoding
    {
        TTK_ASSERT(TOGL_ToUTF8(CodeToTextUTF16({'t', 'e', 'x', 't', '\0'})) == CodeToTextUTF8({'t', 'e', 'x', 't', '\0'})); // correct, control one
        // U+FFFD - Replacement Character (EF BF BD in utf8)
        TTK_ASSERT(TOGL_ToUTF8(CodeToTextUTF16({0xDC00, 'e', 'x', 't', '\0'})) == CodeToTextUTF8({0xEF, 0xBF, 0xBD, 'e', 'x', 't', '\0'})); 
        TTK_ASSERT(TOGL_ToUTF8(CodeToTextUTF16({0xD800, 'e', 'x', 't', '\0'})) == CodeToTextUTF8({0xEF, 0xBF, 0xBD, 'e', 'x', 't', '\0'}));
    }
}

void TestTOGL_Log() {
    TTK_ASSERT(CreateDirectoryA(".\\log", 0) || GetLastError() == ERROR_ALREADY_EXISTS);
    TTK_ASSERT(CreateDirectoryA(".\\log\\test", 0) || GetLastError() == ERROR_ALREADY_EXISTS);

    {
        system("TrivialOpenGL_Test.exe LOG > log\\test\\log.txt");
        const std::string expectd_content = 
            "(TOGL) Debug: Some message.\n"
            "(TOGL) Info: Some message 2.\n"
            "(TOGL) Warning: Some message 2.5.\n"
            "(TOGL) Error: Some message 2.999.\n"
            "(TOGL) Fatal Error: Some message 3.\n";
        TTK_ASSERT(LoadTextFromFile("log\\test\\log.txt") == expectd_content);
    }
    {
        system("TrivialOpenGL_Test.exe LOG_NO_DEBUG > log\\test\\log_no_debug.txt");
        const std::string expectd_content = 
            "(TOGL) Info: Some message 2.\n"
            "(TOGL) Warning: Some message 2.5.\n"
            "(TOGL) Error: Some message 2.999.\n"
            "(TOGL) Fatal Error: Some message 3.\n";
        TTK_ASSERT(LoadTextFromFile("log\\test\\log_no_debug.txt") == expectd_content);
    }
    {
        system("TrivialOpenGL_Test.exe LOG_TRY_WIDE > log\\test\\log_try_wide.txt");
        const std::string expectd_content = 
            "(TOGL) Debug: Some message.\n"
            "(TOGL) Info: Some message 2.\n"
            "(TOGL) Warning: Some message 2.5.\n"
            "(TOGL) Error: Some message 2.999.\n"
            "(TOGL) Fatal Error: Some message 3.\n";
        TTK_ASSERT(LoadTextFromFile("log\\test\\log_try_wide.txt") == expectd_content);
    }

    {
        system("TrivialOpenGL_Test.exe LOG_CUSTOM > log\\test\\log_custom.txt");
        const std::string expectd_content = 
            "Custom Debug: Some message.\n"
            "Custom Info: Some message 2.\n"
            "Custom Warning: Some message 2.5.\n"
            "Custom Error: Some message 2.999.\n"
            "Custom Fatal Error: Some message 3.\n";
        TTK_ASSERT(LoadTextFromFile("log\\test\\log_custom.txt") == expectd_content);
    }


}

void TestTOGL_Split() {
    {
        auto list = TOGL_Split("", '\n');
        TTK_ASSERT(list.size() == 1);
        TTK_ASSERT(list[0] == "");
    }
    {
        auto list = TOGL_Split("\n", '\n');
        TTK_ASSERT(list.size() == 2);
        TTK_ASSERT(list[0] == "");
        TTK_ASSERT(list[1] == "");
    }
    {
        auto list = TOGL_Split("\n\n", '\n');
        TTK_ASSERT(list.size() == 3);
        TTK_ASSERT(list[0] == "");
        TTK_ASSERT(list[1] == "");
        TTK_ASSERT(list[2] == "");
    }
    {
        auto list = TOGL_Split("x", '\n');
        TTK_ASSERT(list.size() == 1);
        TTK_ASSERT(list[0] == "x");
    }

    {
        auto list = TOGL_Split("\nx", '\n');
        TTK_ASSERT(list.size() == 2);
        TTK_ASSERT(list[0] == "");
        TTK_ASSERT(list[1] == "x");
    }

    {
        auto list = TOGL_Split("x\n", '\n');
        TTK_ASSERT(list.size() == 2);
        TTK_ASSERT(list[0] == "x");
        TTK_ASSERT(list[1] == "");
    }

    {
        auto list = TOGL_Split("x\nabc\n123", '\n');
        TTK_ASSERT(list.size() == 3);
        TTK_ASSERT(list[0] == "x");
        TTK_ASSERT(list[1] == "abc");
        TTK_ASSERT(list[2] == "123");
    }

    {
        auto list = TOGL_Split("\nx\nabc\n\n123\n", '\n');
        TTK_ASSERT(list.size() == 6);
        TTK_ASSERT(list[0] == "");
        TTK_ASSERT(list[1] == "x");
        TTK_ASSERT(list[2] == "abc");
        TTK_ASSERT(list[3] == "");
        TTK_ASSERT(list[4] == "123");
        TTK_ASSERT(list[5] == "");
    }
}

void TestTOGL_Color() {
    {
        TOGL_Color4U8 color;
        TTK_ASSERT(color.r == 0);
        TTK_ASSERT(color.g == 0);
        TTK_ASSERT(color.b == 0);
        TTK_ASSERT(color.a == 0);
    }

    {
        TOGL_Color4U8 color(1, 2, 3, 4);
        TTK_ASSERT(color.r == 1);
        TTK_ASSERT(color.g == 2);
        TTK_ASSERT(color.b == 3);
        TTK_ASSERT(color.a == 4);
    }
}

int main(int argc, char *argv[]) {
    std::set<std::string> flags;

    for (size_t index = 0; index < size_t(argc); ++index) {
        flags.insert(argv[index]);
    }

    auto IsFlag = [&flags](const std::string& flag) { 
        return flags.find(flag) != flags.end(); 
    };

    if (IsFlag("LOG")) {
        printf(""); // try force narrow stream
        TOGL_SetLogLevel(TOGL_LOG_LEVEL_DEBUG);
        TOGL_LogDebug("Some message.");
        TOGL_LogInfo("Some message 2.");
        TOGL_LogWarning("Some message 2.5.");
        TOGL_LogError("Some message 2.999.");
        TOGL_LogFatalError("Some message 3.");
        TOGL_LogInfo("Some message 4.");
        return 0;

    } else if (IsFlag("LOG_NO_DEBUG")) {
        printf(""); // try force narrow stream
        TOGL_SetLogLevel(TOGL_LOG_LEVEL_INFO);
        TOGL_LogDebug("Some message.");
        TOGL_LogInfo("Some message 2.");
        TOGL_LogWarning("Some message 2.5.");
        TOGL_LogError("Some message 2.999.");
        TOGL_LogFatalError("Some message 3.");
        TOGL_LogInfo("Some message 4.");
        return 0;

    } else if (IsFlag("LOG_TRY_WIDE")) {
        TOGL_SetLogLevel(TOGL_LOG_LEVEL_DEBUG);
        wprintf(L""); // try force wide stream
        TOGL_LogDebug("Some message.");
        TOGL_LogInfo("Some message 2.");
        TOGL_LogWarning("Some message 2.5.");
        TOGL_LogError("Some message 2.999.");
        TOGL_LogFatalError("Some message 3.");
        TOGL_LogInfo("Some message 4.");
        return 0;

    } else if (IsFlag("LOG_CUSTOM")) {
        TOGL_SetLogLevel(TOGL_LOG_LEVEL_DEBUG);
        TOGL_SetCustomLogFunction([](TOGL_LogMessageTypeId message_type, const char* message) {
            if (message_type == TOGL_LOG_MESSAGE_TYPE_ID_FATAL_ERROR) {
                printf("Custom Fatal Error: %s\n", message);
            } else if (message_type == TOGL_LOG_MESSAGE_TYPE_ID_DEBUG) {
                printf("Custom Debug: %s\n", message);
            } else if (message_type == TOGL_LOG_MESSAGE_TYPE_ID_WARNING) {
                printf("Custom Warning: %s\n", message);
            } else if (message_type == TOGL_LOG_MESSAGE_TYPE_ID_ERROR) {
                printf("Custom Error: %s\n", message);
            } else {
                printf("Custom Info: %s\n", message);
            }
            fflush(stdout);
        });

        TOGL_LogDebug("Some message.");
        TOGL_LogInfo("Some message 2.");
        TOGL_LogWarning("Some message 2.5.");
        TOGL_LogError("Some message 2.999.");
        TOGL_LogFatalError("Some message 3.");
        TOGL_LogInfo("Some message 4.");
        return 0;

    } else {
        TTK_ADD_TEST(TestTOGL_Point, 0);
        TTK_ADD_TEST(TestTOGL_Size, 0);
        TTK_ADD_TEST(TestTOGL_Area, 0);
        TTK_ADD_TEST(TestTOGL_ASCII_ToUTF16, 0);
        TTK_ADD_TEST(TestTOGL_ToUTF16, 0);
        TTK_ADD_TEST(TestTOGL_ToUTF8, 0);
        TTK_ADD_TEST(TestTOGL_Log, 0);
        TTK_ADD_TEST(TestTOGL_Split, 0);
        TTK_ADD_TEST(TestTOGL_Color, 0);
        
        return !TTK_Run();
    }
}