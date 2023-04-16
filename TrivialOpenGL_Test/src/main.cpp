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
    {
        Area<int, unsigned> a(Point<int>(1, 2), Size<unsigned>(3, 4));
        TTK_ASSERT_M(a.x        == 1, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 2, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 3, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 4, ToStr("%d", a.height));
    }
    {
        Area<int> a(Point<int>(1, 2));
        TTK_ASSERT_M(a.x        == 1, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 2, ToStr("%d", a.y));
        TTK_ASSERT_M(a.width    == 0, ToStr("%d", a.width));
        TTK_ASSERT_M(a.height   == 0, ToStr("%d", a.height));
    }
    {
        Area<int> a(Size<int>(3, 4));
        TTK_ASSERT_M(a.x        == 0, ToStr("%d", a.x));
        TTK_ASSERT_M(a.y        == 0, ToStr("%d", a.y));
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

        a.SetPos({10, 20});
        TTK_ASSERT(a == Area<int>(10, 20, 3, 4));
        TTK_ASSERT(a.GetPos() == Point<int>(10, 20));


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

void TestTOGL_ASCII_ToUTF16() {
    using TOGL::ASCII_ToUTF16;

    TTK_ASSERT(ASCII_ToUTF16("") == L"");
    TTK_ASSERT(ASCII_ToUTF16("Some Text.") == L"Some Text.");
}

void TestTOGL_ToUTF16() {
    using TOGL::ToUTF16;

    // empty text
    TTK_ASSERT(ToUTF16(u8"") == L"");

    // unicode characters
    TTK_ASSERT(ToUTF16(u8"Some text\u0444\U0002F820.") == L"Some text\u0444\U0002F820.");

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

        const size_t number = TOGL::CONV_STACK_DEF_BUFFER_SIZE * 2 / sequence_utf16.length();
        for (size_t ix = 0; ix < number; ++ix) {
            long_text_utf8  += sequence_utf8;
            long_text_utf16 += sequence_utf16;
        }

        TTK_ASSERT(ToUTF16(long_text_utf8) == long_text_utf16);
    }

    // wrong encoding
    {
        TTK_ASSERT(ToUTF16(CodeToTextUTF8({'t', 'e', 'x', 't', '\0'})) == CodeToTextUTF16({'t', 'e', 'x', 't', '\0'})); // correct, control one
        // U+FFFD - Replacement Character
        TTK_ASSERT(ToUTF16(CodeToTextUTF8({0xC2, 'e', 'x', 't', '\0'})) == CodeToTextUTF16({0xFFFD, 'e', 'x', 't', '\0'}));
        TTK_ASSERT(ToUTF16(CodeToTextUTF8({0xFF, 'e', 'x', 't', '\0'})) == CodeToTextUTF16({0xFFFD, 'e', 'x', 't', '\0'}));
    }

}

void TestTOGL_ToUTF8() {
    using TOGL::ToUTF8;

    // empty text
    TTK_ASSERT(ToUTF8(L"") == u8"");

    // unicode characters
    TTK_ASSERT(ToUTF8(L"Some text\u0444\U0002F820.") == u8"Some text\u0444\U0002F820.");

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

        const size_t number = TOGL::CONV_STACK_DEF_BUFFER_SIZE * 2 / sequence_utf8.length();
        for (size_t ix = 0; ix < number; ++ix) {
            long_text_utf8  += sequence_utf8;
            long_text_utf16 += sequence_utf16;
        }

        TTK_ASSERT(ToUTF8(long_text_utf16) == long_text_utf8);
    }

    // wrong encoding
    {
        TTK_ASSERT(ToUTF8(CodeToTextUTF16({'t', 'e', 'x', 't', '\0'})) == CodeToTextUTF8({'t', 'e', 'x', 't', '\0'})); // correct, control one
        // U+FFFD - Replacement Character (EF BF BD in utf8)
        TTK_ASSERT(ToUTF8(CodeToTextUTF16({0xDC00, 'e', 'x', 't', '\0'})) == CodeToTextUTF8({0xEF, 0xBF, 0xBD, 'e', 'x', 't', '\0'})); 
        TTK_ASSERT(ToUTF8(CodeToTextUTF16({0xD800, 'e', 'x', 't', '\0'})) == CodeToTextUTF8({0xEF, 0xBF, 0xBD, 'e', 'x', 't', '\0'}));
    }
}

void TestTOGL_Log() {
    TTK_ASSERT(CreateDirectoryA(".\\log", 0) || GetLastError() == ERROR_ALREADY_EXISTS);
    TTK_ASSERT(CreateDirectoryA(".\\log\\test", 0) || GetLastError() == ERROR_ALREADY_EXISTS);

    system("TrivialOpenGL_Test.exe LOG > log\\test\\log.txt");
    TTK_ASSERT(LoadTextFromFile("log\\test\\log.txt") == "Some message.\nSome message 2.\n");

    system("TrivialOpenGL_Test.exe LOG_DEDICATED > log\\test\\log_dedicated.txt");
    const std::string expectd_content = 
        "(TOGL) Debug: Some message.\n"
        "(TOGL) Info: Some message 2.\n"
        "(TOGL) Fatal Error: Some message 3.\n";
    TTK_ASSERT(LoadTextFromFile("log\\test\\log_dedicated.txt") == expectd_content);


    system("TrivialOpenGL_Test.exe LOG_TRY_WIDE > log\\test\\log_try_wide.txt");
    TTK_ASSERT(LoadTextFromFile("log\\test\\log_try_wide.txt") == "Some message.\nSome message 2.\n");

    system("TrivialOpenGL_Test.exe LOG_CUSTOM > log\\test\\log_custom.txt");
    TTK_ASSERT(LoadTextFromFile("log\\test\\log_custom.txt") == "Info: Some message.\nFatal Error: Some message 2.\n");
}

void TestTOGL_Split() {
    {
        auto list = TOGL::Split("", '\n');
        TTK_ASSERT(list.size() == 1);
        TTK_ASSERT(list[0] == "");
    }
    {
        auto list = TOGL::Split("\n", '\n');
        TTK_ASSERT(list.size() == 2);
        TTK_ASSERT(list[0] == "");
        TTK_ASSERT(list[1] == "");
    }
    {
        auto list = TOGL::Split("\n\n", '\n');
        TTK_ASSERT(list.size() == 3);
        TTK_ASSERT(list[0] == "");
        TTK_ASSERT(list[1] == "");
        TTK_ASSERT(list[2] == "");
    }
    {
        auto list = TOGL::Split("x", '\n');
        TTK_ASSERT(list.size() == 1);
        TTK_ASSERT(list[0] == "x");
    }

    {
        auto list = TOGL::Split("\nx", '\n');
        TTK_ASSERT(list.size() == 2);
        TTK_ASSERT(list[0] == "");
        TTK_ASSERT(list[1] == "x");
    }

    {
        auto list = TOGL::Split("x\n", '\n');
        TTK_ASSERT(list.size() == 2);
        TTK_ASSERT(list[0] == "x");
        TTK_ASSERT(list[1] == "");
    }

    {
        auto list = TOGL::Split("x\nabc\n123", '\n');
        TTK_ASSERT(list.size() == 3);
        TTK_ASSERT(list[0] == "x");
        TTK_ASSERT(list[1] == "abc");
        TTK_ASSERT(list[2] == "123");
    }

    {
        auto list = TOGL::Split("\nx\nabc\n\n123\n", '\n');
        TTK_ASSERT(list.size() == 6);
        TTK_ASSERT(list[0] == "");
        TTK_ASSERT(list[1] == "x");
        TTK_ASSERT(list[2] == "abc");
        TTK_ASSERT(list[3] == "");
        TTK_ASSERT(list[4] == "123");
        TTK_ASSERT(list[5] == "");
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
        TOGL::Log(TOGL::LogMessageType::INFO, "Some message.\n");
        TOGL::Log(TOGL::LogMessageType::FATAL_ERROR, "Some message 2.\n");
        TOGL::Log(TOGL::LogMessageType::INFO, "Some message 3.\n");
        return 0;

    } else if (IsFlag("LOG_DEDICATED")) {
        printf(""); // try force wide stream
        TOGL::LogDebug("Some message.");
        TOGL::LogInfo("Some message 2.");
        TOGL::LogFatalError("Some message 3.");
        TOGL::LogInfo("Some message 4.");
        return 0;

    } else if (IsFlag("LOG_TRY_WIDE")) {
        wprintf(L""); // try force wide stream
        TOGL::Log(TOGL::LogMessageType::INFO, "Some message.\n");
        TOGL::Log(TOGL::LogMessageType::FATAL_ERROR, "Some message 2.\n");
        TOGL::Log(TOGL::LogMessageType::INFO, "Some message 3.\n");
        return 0;

    } else if (IsFlag("LOG_CUSTOM")) {
        TOGL::SetCustomLogFunction([](TOGL::LogMessageType message_type, const char* message) {
            if (message_type == TOGL::LogMessageType::FATAL_ERROR) {
                printf("Fatal Error: %s\n", message);
            } else {
                printf("Info: %s\n", message);
            }
            fflush(stdout);
        });

        TOGL::Log(TOGL::LogMessageType::INFO, "Some message.");
        TOGL::Log(TOGL::LogMessageType::FATAL_ERROR, "Some message 2.");
        TOGL::Log(TOGL::LogMessageType::INFO, "Some message 3.");
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
        
        return !TTK_Run();
    }
}