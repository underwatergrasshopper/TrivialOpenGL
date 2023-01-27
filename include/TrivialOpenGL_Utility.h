/**
* @file TrivialOpenGL_Utility.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_UTILITY_H_
#define TRIVIALOPENGL_UTILITY_H_

#include <stdint.h>
#include <stdio.h>
#include <windows.h>

namespace TrivialOpenGL {

    //--------------------------------------------------------------------------
    // Point
    //--------------------------------------------------------------------------

    template <typename Type>
    struct Point {
        Type x;
        Type y;

        Point() : x(Type()), y(Type()) {}

        explicit Point(const Type& s) : x(s), y(s) {}

        Point(const Type& x, const Type& y) : x(x), y(y) {}

        template <typename Type2>
        explicit Point(const Point<Type2>& p) : x(Type(p.x)), y(Type(p.y)) {}

        virtual ~Point() {}
    };

    using PointI    = Point<int32_t>;
    using PointI64  = Point<int64_t>;

    using PointU    = Point<uint32_t>;
    using PointU64  = Point<uint64_t>;

    using PointF    = Point<float>;
    using PointD    = Point<float>;

    template <typename Type>
    inline bool operator==(const Point<Type>& l, const Point<Type>& r) {
        return l.x == r.x && l.y == r.y;
    }

    template <typename Type>
    inline bool operator!=(const Point<Type>& l, const Point<Type>& r) {
        return l.x != r.x || l.y != r.y;
    }

    template <typename Type>
    inline bool operator>(const Point<Type>& l, const Point<Type>& r) {
        return l.x > r.x && l.y > r.y;
    }

    template <typename Type>
    inline bool operator<(const Point<Type>& l, const Point<Type>& r) {
        return l.x < r.x && l.y < r.y;
    }

    template <typename Type>
    inline bool operator>=(const Point<Type>& l, const Point<Type>& r) {
        return l.x >= r.x && l.y >= r.y;
    }

    template <typename Type>
    inline bool operator<=(const Point<Type>& l, const Point<Type>& r) {
        return l.x <= r.x && l.y <= r.y;
    }


    template <typename Type>
    inline Point<Type> operator+(const Point<Type>& l, const Point<Type>& r) {
        return {l.x + r.x, l.y + r.y};
    }

    template <typename Type>
    inline Point<Type> operator-(const Point<Type>& l, const Point<Type>& r) {
        return {l.x - r.x, l.y - r.y};
    }

    template <typename Type>
    inline Point<Type> operator*(const Point<Type>& l, const Type& r) {
        return {l.x * r, l.y * r};
    }

    template <typename Type>
    inline Point<Type> operator/(const Point<Type>& l, const Type& r) {
        return {l.x / r, l.y / r};
    }


    template <typename Type>
    inline Point<Type>& operator+=(Point<Type>& l, const Point<Type>& r) {
        l = l + r;
        return l;
    }
    
    template <typename Type>
    inline Point<Type>& operator-=(Point<Type>& l, const Point<Type>& r) {
        l = l - r;
        return l;
    }
    
    template <typename Type>
    inline Point<Type>& operator*=(Point<Type>& l, const Type& r) {
        l = l * r;
        return l;
    }
    
    template <typename Type>
    inline Point<Type>& operator/=(Point<Type>& l, const Type& r) {
        l = l / r;
        return l;
    }

    //--------------------------------------------------------------------------
    // Size
    //--------------------------------------------------------------------------

    template <typename Type>
    struct Size {
        Type width;
        Type height;

        Size() : width(Type()), height(Type()) {}

        explicit Size(const Type& s) : width(s), height(s) {}

        Size(const Type& width, const Type& height) : width(width), height(height) {}

        template <typename Type2>
        explicit Size(const Size<Type2>& s) : width(Type(s.width)), height(Type(s.height)) {}

        virtual ~Size() {}
    };

    using SizeI    = Size<int32_t>;
    using SizeI64  = Size<int64_t>;

    using SizeU    = Size<uint32_t>;
    using SizeU64  = Size<uint64_t>;

    using SizeF    = Size<float>;
    using SizeD    = Size<float>;

    template <typename Type>
    inline bool operator==(const Size<Type>& l, const Size<Type>& r) {
        return l.width == r.width && l.height == r.height;
    }

    template <typename Type>
    inline bool operator!=(const Size<Type>& l, const Size<Type>& r) {
        return l.width != r.width || l.height != r.height;
    }

    template <typename Type>
    inline bool operator>(const Size<Type>& l, const Size<Type>& r) {
        return l.width > r.width && l.height > r.height;
    }

    template <typename Type>
    inline bool operator<(const Size<Type>& l, const Size<Type>& r) {
        return l.width < r.width && l.height < r.height;
    }

    template <typename Type>
    inline bool operator>=(const Size<Type>& l, const Size<Type>& r) {
        return l.width >= r.width && l.height >= r.height;
    }

    template <typename Type>
    inline bool operator<=(const Size<Type>& l, const Size<Type>& r) {
        return l.width <= r.width && l.height <= r.height;
    }


    template <typename Type>
    inline Size<Type> operator+(const Size<Type>& l, const Size<Type>& r) {
        return {l.width + r.width, l.height + r.height};
    }

    template <typename Type>
    inline Size<Type> operator-(const Size<Type>& l, const Size<Type>& r) {
        return {l.width - r.width, l.height - r.height};
    }

    template <typename Type>
    inline Size<Type> operator*(const Size<Type>& l, const Type& r) {
        return {l.width * r, l.height * r};
    }

    template <typename Type>
    inline Size<Type> operator/(const Size<Type>& l, const Type& r) {
        return {l.width / r, l.height / r};
    }


    template <typename Type>
    inline Size<Type>& operator+=(Size<Type>& l, const Size<Type>& r) {
        l = l + r;
        return l;
    }

    template <typename Type>
    inline Size<Type>& operator-=(Size<Type>& l, const Size<Type>& r) {
        l = l - r;
        return l;
    }

    template <typename Type>
    inline Size<Type>& operator*=(Size<Type>& l, const Type& r) {
        l = l * r;
        return l;
    }

    template <typename Type>
    inline Size<Type>& operator/=(Size<Type>& l, const Type& r) {
        l = l / r;
        return l;
    }

    //--------------------------------------------------------------------------
    // Area
    //--------------------------------------------------------------------------

    template <typename Type>
    struct Area {
        Type x;
        Type y;
        Type width;
        Type height;

        Area() : x(Type()), y(Type()), width(Type()), height(Type()) {}

        Area(const Type& x, const Type& y, const Type& width, const Type& height) : x(x), y(y), width(width), height(height) {}

        template <typename Type2>
        explicit Area(const Area<Type2>& a) : x(Type(a.x)), y(Type(a.y)), width(Type(a.width)), height(Type(a.height)) {}

        explicit Area(const Point<Type>& p, const Size<Type>& s) : x(p.x), y(p.y), width(s.width), height(s.height) {}

        void SetPoint(const Point<Type>& p) {  
            x = p.x;
            y = p.y;
        }

        void SetSize(const Size<Type>& s) {  
            width   = s.width;
            height  = s.height;
        }

        Point<Type> GetPoint() const { return Point<Type>(x, y); }
        Size<Type> GetSize() const { return Size<Type>(width, height); }

        bool IsIn(const Point<Type>& p) const {
            return Point<Type>(x, y) <= p && p < Point<Type>(x + width, y + height);
        }

        virtual ~Area() {}
    };

    using AreaI    = Area<int32_t>;
    using AreaI64  = Area<int64_t>;

    using AreaU    = Area<uint32_t>;
    using AreaU64  = Area<uint64_t>;

    using AreaF    = Area<float>;
    using AreaD    = Area<float>;

    template <typename Type>
    inline bool operator==(const Area<Type>& l, const Area<Type>& r) {
        return l.x == r.x && l.y == r.y && l.width == r.width && l.height == r.height;
    }

    template <typename Type>
    inline bool operator!=(const Area<Type>& l, const Area<Type>& r) {
        return l.x != r.x || l.y != r.y || l.width != r.width || l.height != r.height;
    }

    template <typename Type>
    inline RECT MakeRECT(const Area<Type>& area) {
        return {LONG(area.x), LONG(area.y), LONG(area.x + area.width), LONG(area.y + area.height)};
    }

    //--------------------------------------------------------------------------
    // Static
    //--------------------------------------------------------------------------

    template <typename Type>
    class Static {
    public:
        static Type& To() { return sm_object; }
    private:
        static Type sm_object;
    };

    template <typename Type>
    Type Static<Type>::sm_object;

    //--------------------------------------------------------------------------
    // Log
    //--------------------------------------------------------------------------

    enum class LogMessageType {
        UNKNOWN,
        INFO,
        FATAL_ERROR
    };

    using HandleLogFnP_T = void (*)(LogMessageType message_type, const char* message);

    void Log(LogMessageType message_type, const char* message) {
        auto& handle_log = Static<HandleLogFnP_T>::To();

        if (handle_log) {
            handle_log(message_type, message);
        } else {
            if (fwide(stdout, 0) > 0) {

            } else {
                printf("%s", message);
                fflush(stdout);
                if (message_type == LogMessageType::FATAL_ERROR) exit(EXIT_FAILURE);
            }
        }
    }

    void SetHandleLogFunction(HandleLogFnP_T handle_log) {
        Static<HandleLogFnP_T>::To() = handle_log;
    }

} // namespace TrivialOpenGL

namespace TOGL = TrivialOpenGL;

#endif // TRIVIALOPENGL_UTILITY_H_
