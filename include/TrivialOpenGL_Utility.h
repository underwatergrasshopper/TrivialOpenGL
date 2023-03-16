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

        template <typename SizeType>
        explicit Point(const Point<SizeType>& p) : x(Type(p.x)), y(Type(p.y)) {}

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

        template <typename SizeType>
        explicit Size(const Size<SizeType>& s) : width(Type(s.width)), height(Type(s.height)) {}

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

    template <typename PointType, typename SizeType = PointType>
    struct Area {
        PointType  x;
        PointType  y;
        SizeType   width;
        SizeType   height;

        Area() : x(PointType()), y(PointType()), width(SizeType()), height(SizeType()) {}

        Area(const PointType& x, const PointType& y, const SizeType& width, const SizeType& height) : x(x), y(y), width(width), height(height) {}

        template <typename PointType2, typename SizeType2 = PointType2>
        explicit Area(const Area<PointType2, SizeType2>& area) : x(PointType(area.x)), y(PointType(area.y)), width(SizeType(area.width)), height(SizeType(area.height)) {}

        Area(const Point<PointType>& pos, const Size<SizeType>& size) : x(pos.x), y(pos.y), width(size.width), height(size.height) {}

        explicit Area(const Point<PointType>& pos) : x(pos.x), y(pos.y), width(SizeType()), height(SizeType()) {}
        explicit Area(const Size<SizeType>& size) : x(PointType()), y(PointType()), width(size.width), height(size.height) {}

        void SetPos(const Point<PointType>& pos) {  
            x = pos.x;
            y = pos.y;
        }

        void SetSize(const Size<SizeType>& size) {  
            width   = size.width;
            height  = size.height;
        }

        Point<PointType> GetPos() const { return Point<PointType>(x, y); }
        Size<SizeType> GetSize() const { return Size<SizeType>(width, height); }

        bool IsIn(const Point<PointType>& pos) const {
            return Point<PointType>(x, y) <= pos && pos < Point<PointType>(x + width, y + height);
        }

        virtual ~Area() {}
    };

    using AreaI    = Area<int32_t>;
    using AreaI64  = Area<int64_t>;

    using AreaU    = Area<uint32_t>;
    using AreaU64  = Area<uint64_t>;

    using AreaF    = Area<float>;
    using AreaD    = Area<float>;

    using AreaIU   = Area<int32_t, uint32_t>;

    template <typename Type>
    inline bool operator==(const Area<Type>& l, const Area<Type>& r) {
        return l.x == r.x && l.y == r.y && l.width == r.width && l.height == r.height;
    }

    template <typename Type>
    inline bool operator!=(const Area<Type>& l, const Area<Type>& r) {
        return l.x != r.x || l.y != r.y || l.width != r.width || l.height != r.height;
    }

    template <typename PointType, typename SizeType = PointType>
    inline RECT MakeRECT(const Area<PointType, SizeType>& area) {
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

    inline AreaIU MakeAreaIU(const RECT& r) {
        return AreaIU(
            r.left,
            r.top,
            r.right - r.left,
            r.bottom - r.top
        );
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
    // CountStack
    //--------------------------------------------------------------------------

    class CountStack {
    public:
        CountStack() {
            count = 0;
        }

        virtual ~CountStack() {}

        bool Push() {
            if (count == -1) return false;

            count += 1;
            return true;
        }

        bool Pop() {
            if (count == 0) return false;

            count -= 1;
            return true;
        }

        bool Is() const {
            return count > 0;
        }
    private:
        uint32_t count;
    };

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
        DEBUG,
        INFO,
        FATAL_ERROR
    };

    using CustomLogFnP_T = void (*)(LogMessageType message_type, const char* message);

    // Logs message to standard output (by default) redirect to custom function which was set by calling SetHandleLogFunction.
    // message          A message in ASCII encoding.
    void Log(LogMessageType message_type, const char* message);

    // message      Message in ascii encoding.
    void Log(LogMessageType message_type, const std::string& message);
    void LogDebug(const std::string& message);
    void LogInfo(const std::string& message);
    void LogFatalError(const std::string& message);

    void SetCustomLogFunction(CustomLogFnP_T custom_log);

    //--------------------------------------------------------------------------
    // WindowAreaCorrector
    //--------------------------------------------------------------------------
    
    // Corrects window position and size to remove invisible window frame in Windows 10. 
    class WindowAreaCorrector {
    public:
        WindowAreaCorrector() {
            m_dwmapi_lib_handle = LoadLibraryA("Dwmapi.dll");
            if (m_dwmapi_lib_handle) {

                m_dwm_get_window_attribute          = (decltype(m_dwm_get_window_attribute)) GetProcAddress(m_dwmapi_lib_handle, "DwmGetWindowAttribute");
                m_dwm_extend_frame_into_client_area = (decltype(m_dwm_extend_frame_into_client_area)) GetProcAddress(m_dwmapi_lib_handle, "DwmExtendFrameIntoClientArea");
                
            } else {
                m_dwm_get_window_attribute          = nullptr;
                m_dwm_extend_frame_into_client_area = nullptr;
            }
        }

        virtual ~WindowAreaCorrector() {
            FreeLibrary(m_dwmapi_lib_handle);
        }

        void DisableComposition() {
            enum {
                TOGL_DWM_EC_DISABLECOMPOSITION  = 0,
                TOGL_DWM_EC_ENABLECOMPOSITION   = 1,
            };
            HRESULT (*DwmEnableComposition)(UINT uCompositionAction) = (decltype(DwmEnableComposition)) GetProcAddress(m_dwmapi_lib_handle, "DwmEnableComposition");

            DwmEnableComposition(TOGL_DWM_EC_DISABLECOMPOSITION);
        }

        AreaI Get(HWND window_handle) const {
            AreaI area = {};

            if (m_dwm_get_window_attribute) {
                RECT window_rect;
                GetWindowRect(window_handle, &window_rect);

                // Added TOGL_ prefix.
                enum { TOGL_DWMWA_EXTENDED_FRAME_BOUNDS = 9 };
                RECT actual_window_rect;

                // Note: To return correct values, must be called after ShowWindow(window_handle, SW_SHOW).
                m_dwm_get_window_attribute(window_handle, TOGL_DWMWA_EXTENDED_FRAME_BOUNDS, &actual_window_rect, sizeof(RECT));

                RECT frame = {};
                frame.left      = actual_window_rect.left   - window_rect.left;
                frame.top       = actual_window_rect.top    - window_rect.top;
                frame.right     = window_rect.right         - actual_window_rect.right;
                frame.bottom    = window_rect.bottom        - actual_window_rect.bottom;

                area = {-frame.left, -frame.top, frame.left + frame.right, frame.top + frame.bottom};
            }

            return area;
        }

        // area         - Window area without invisible frame.
        AreaI AddInvisibleFrameTo(const AreaI& area, HWND window_hangle) const {
            const AreaI correction = Get(window_hangle);
            return {
                area.x      + correction.x,
                area.y      + correction.y,
                area.width  + correction.width,
                area.height + correction.height
            };
        }

        // size         - Window size without invisible frame.
        SizeI AddInvisibleFrameTo(const SizeI& size, HWND window_hangle) const {
            const AreaI correction = Get(window_hangle);
            return {
                size.width  + correction.width,
                size.height + correction.height
            };
        }

        // pos          - Window position without invisible frame.
        PointI AddInvisibleFrameTo(const PointI& pos, HWND window_hangle) const {
            const AreaI correction = Get(window_hangle);
            return {
                pos.x + correction.x,
                pos.y + correction.y,
            };
        }

        // area         - Window area with invisible frame.
        AreaI RemoveInvisibleFrameFrom(const AreaI& area, HWND window_hangle) const {
            const AreaI correction = Get(window_hangle);
            return {
                area.x      - correction.x,
                area.y      - correction.y,
                area.width  - correction.width,
                area.height - correction.height
            };
        }

        // size         - Window size with invisible frame.
        SizeI RemoveInvisibleFrameFrom(const SizeI& size, HWND window_hangle) const {
            const AreaI correction = Get(window_hangle);
            return {
                size.width  - correction.width,
                size.height - correction.height
            };
        }

        // pos          - Window position with invisible frame.
        PointI RemoveInvisibleFrameFrom(const PointI& pos, HWND window_hangle) const {
            const AreaI correction = Get(window_hangle);
            return {
                pos.x - correction.x,
                pos.y - correction.y,
            };
        }
    private:
        HMODULE  m_dwmapi_lib_handle;

        struct MARGINS {
            int cxLeftWidth;
            int cxRightWidth;
            int cyTopHeight;
            int cyBottomHeight;
        };
        
        HRESULT (*m_dwm_get_window_attribute)(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute);
        HRESULT (*m_dwm_extend_frame_into_client_area)(HWND hWnd, const MARGINS *pMarInset);
    };

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
                wprintf(L"%ls", ASCII_ToUTF16(message).c_str());
            } else {
                printf("%s", message);
            }
            fflush(stdout);
        }
        if (message_type == LogMessageType::FATAL_ERROR) exit(EXIT_FAILURE);
    }

    inline void Log(LogMessageType message_type, const std::string& message) {
        Log(message_type, message.c_str());
    }

    inline void LogDebug(const std::string& message) {
        const std::string info_message = std::string("(TOGL) Debug: ") + message + "\n";

        Log(LogMessageType::DEBUG, info_message.c_str());
    }

    inline void LogInfo(const std::string& message) {
        const std::string info_message = std::string("(TOGL) Info: ") + message + "\n";

        Log(LogMessageType::INFO, info_message.c_str());
    }

    inline void LogFatalError(const std::string& message) {
        const std::string fatal_error_message = std::string("(TOGL) Fatal Error: ") + message + "\n";

        Log(LogMessageType::FATAL_ERROR, fatal_error_message.c_str());
    }

    inline void SetCustomLogFunction(CustomLogFnP_T custom_log) {
        Static<CustomLogFnP_T>::To() = custom_log;
    }
} // namespace TrivialOpenGL 

#endif // TRIVIALOPENGL_UTILITY_H_
