/**
* @file TrivialOpenGL_Utility.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_UTILITY_H_
#define TRIVIALOPENGL_UTILITY_H_

#include <stdint.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN 

#include <string>

//==============================================================================
// Declarations
//==============================================================================

#define togl_print_i32(variable) { printf(#variable"=%d\n", int(variable)); fflush(stdout); } (void)0

namespace TrivialOpenGL {

    //--------------------------------------------------------------------------
    // Version
    //--------------------------------------------------------------------------

    struct Version {
        int major;
        int minor;
    };

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
        explicit Area(const Area<Type2>& area) : x(Type(area.x)), y(Type(area.y)), width(Type(area.width)), height(Type(area.height)) {}

        Area(const Point<Type>& pos, const Size<Type>& size) : x(pos.x), y(pos.y), width(size.width), height(size.height) {}

        void SetPos(const Point<Type>& pos) {  
            x = pos.x;
            y = pos.y;
        }

        void SetSize(const Size<Type>& size) {  
            width   = size.width;
            height  = size.height;
        }

        Point<Type> GetPos() const { return Point<Type>(x, y); }
        Size<Type> GetSize() const { return Size<Type>(width, height); }

        bool IsIn(const Point<Type>& pos) const {
            return Point<Type>(x, y) <= pos && pos < Point<Type>(x + width, y + height);
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

    inline AreaI MakeArea(const RECT& r) {
        return {
            r.left,
            r.top,
            r.right - r.left,
            r.bottom - r.top
        };
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
    // Conversion
    //--------------------------------------------------------------------------

    enum { CONV_STACK_DEF_BUFFER_SIZE = 4096 };

    // Converts ascii string to utf-16 string.
    std::wstring ASCII_ToUTF16(const std::string& text_ascii);

    // Converts utf-8 string to utf-16 string.
    std::wstring ToUTF16(const std::string& text_utf8);

    // Converts utf-16 string to utf-8 string.
    std::string ToUTF8(const std::wstring& text_utf16);

    //--------------------------------------------------------------------------
    // Log
    //--------------------------------------------------------------------------

    enum class LogMessageType {
        UNKNOWN,
        INFO,
        FATAL_ERROR
    };

    using CustomLogFnP_T = void (*)(LogMessageType message_type, const char* message);

    // Logs message to standard output (by default) redirect to custom function which was set by calling SetHandleLogFunction.
    // message          A message in ASCII encoding.
    void Log(LogMessageType message_type, const char* message);

    // message      Message in ascii encoding.
    void Log(LogMessageType message_type, const std::string& message);
    void LogInfo(const std::string& message);
    void LogFatalError(const std::string& message);

    void SetCustomLogFunction(CustomLogFnP_T custom_log);

    //--------------------------------------------------------------------------

    inline AreaI GetDesktopAreaNoTaskBar() {
        RECT rc;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
        return MakeArea(rc);
    }

    inline SizeI GetDesktopAreaSizeNoTaskBar() {
        return GetDesktopAreaNoTaskBar().GetSize();
    }

    inline SizeI GetScreenSize() {
        return { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
    }

} // namespace TrivialOpenGL

namespace TOGL = TrivialOpenGL;

//==============================================================================
// Definitions
//==============================================================================

namespace TrivialOpenGL {

    //--------------------------------------------------------------------------
    // Conversion
    //--------------------------------------------------------------------------

    inline std::wstring ASCII_ToUTF16(const std::string& text_ascii) {
        std::wstring text_utf16;
        for (size_t index = 0; index < text_ascii.length(); ++index) text_utf16 += wchar_t(text_ascii[index]);
        return text_utf16;
    }

    inline std::wstring ToUTF16(const std::string& text_utf8) {
        std::wstring text_utf16;

        wchar_t stack_buffer[CONV_STACK_DEF_BUFFER_SIZE] = {};

        if (!text_utf8.empty()) {
            int size = MultiByteToWideChar(CP_UTF8, 0, text_utf8.c_str(), -1, NULL, 0);
            if (size == 0) {
                LogFatalError("Error TOGLW::Window::ToUTF16: Can not convert a text from utf-16 to utf-8.");
            }

            wchar_t* buffer = (size > CONV_STACK_DEF_BUFFER_SIZE) ? (new wchar_t[size]) : stack_buffer;

            size = MultiByteToWideChar(CP_UTF8, 0, text_utf8.c_str(), -1, buffer, size);
            if (size == 0) {
                LogFatalError("Error TOGLW::Window::ToUTF16: Can not convert a text from utf-16 to utf-8.");
            }

            if (size > 1) text_utf16 = std::wstring(buffer, size - 1);

            if (buffer != stack_buffer) delete[] buffer;
        }

        return text_utf16;
    }

    inline std::string ToUTF8(const std::wstring& text_utf16) {
        std::string text_utf8;

        char stack_buffer[CONV_STACK_DEF_BUFFER_SIZE] = {};

        if (!text_utf16.empty()) {
            int size = WideCharToMultiByte(CP_UTF8, 0, text_utf16.c_str(), -1, NULL, 0, NULL, NULL);
            if (size == 0) {
                LogFatalError("Error TOGLW::Window::ToUTF8: Can not convert a text from utf-8 to utf-16.");
            }

            char* buffer = (size > CONV_STACK_DEF_BUFFER_SIZE) ? (new char[size]) : stack_buffer;

            size = WideCharToMultiByte(CP_UTF8, 0, text_utf16.c_str(), -1, buffer, size, NULL, NULL);
            if (size == 0) {
                LogFatalError("Error TOGLW::Window::ToUTF8: Can not convert a text from utf-8 to utf-16.");
            }

            if (size > 1) text_utf8 = std::string(buffer, size - 1);

            if (buffer != stack_buffer) delete[] buffer;
        }

        return text_utf8;
    }

    //--------------------------------------------------------------------------
    // Log
    //--------------------------------------------------------------------------

    inline void Log(LogMessageType message_type, const char* message) {
        auto& custom_log = Static<CustomLogFnP_T>::To();

        if (custom_log) {
            custom_log(message_type, message);
        } else {
            if (fwide(stdout, 0) > 0) {
                wprintf(L"%ls\n", ASCII_ToUTF16(message).c_str());
            } else {
                printf("%s\n", message);
            }
            fflush(stdout);
        }
        if (message_type == LogMessageType::FATAL_ERROR) exit(EXIT_FAILURE);
    }

    inline void Log(LogMessageType message_type, const std::string& message) {
        Log(message_type, message.c_str());
    }

    inline void LogInfo(const std::string& message) {
        Log(LogMessageType::INFO, message.c_str());
    }

    inline void LogFatalError(const std::string& message) {
        Log(LogMessageType::FATAL_ERROR, message.c_str());
    }

    inline void SetCustomLogFunction(CustomLogFnP_T custom_log) {
        Static<CustomLogFnP_T>::To() = custom_log;
    }
} // namespace TrivialOpenGL 

#endif // TRIVIALOPENGL_UTILITY_H_
