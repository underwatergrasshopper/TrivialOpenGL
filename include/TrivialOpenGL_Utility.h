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
#include <windowsx.h>
#undef WIN32_LEAN_AND_MEAN 

#include <gl\gl.h>

#include <string>
#include <map>
#include <stack>
#include <vector>

//==============================================================================
// Declarations
//==============================================================================

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

    using PointI16  = Point<int16_t>;
    using PointI    = Point<int32_t>;
    using PointI64  = Point<int64_t>;

    using PointU16  = Point<uint16_t>;
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

    using SizeI16  = Size<int16_t>;
    using SizeI    = Size<int32_t>;
    using SizeI64  = Size<int64_t>;

    using SizeU16  = Size<uint16_t>;
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
    using AreaIU16 = Area<int32_t, uint16_t>;

    template <typename PointType, typename SizeType = PointType>
    inline bool operator==(const Area<PointType, SizeType>& l, const Area<PointType, SizeType>& r) {
        return l.x == r.x && l.y == r.y && l.width == r.width && l.height == r.height;
    }

    template <typename PointType, typename SizeType = PointType>
    inline bool operator!=(const Area<PointType, SizeType>& l, const Area<PointType, SizeType>& r) {
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

    inline AreaIU16 MakeAreaIU16(const RECT& r) {
        return AreaIU16(
            r.left,
            r.top,
            uint16_t(r.right - r.left),
            uint16_t(r.bottom - r.top)
        );
    }

    //--------------------------------------------------------------------------
    // Global
    //--------------------------------------------------------------------------

    template <typename Type>
    class Global {
    public:
        static Type& To() { return sm_object; }
    private:
        static Type sm_object;
    };

    template <typename Type>
    Type Global<Type>::sm_object;

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
            } else {
                m_dwm_get_window_attribute          = nullptr;
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

        AreaIU16 Get(HWND window_handle) const {
            AreaIU16 area = {};

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

                area = AreaIU16(- frame.left, -frame.top, uint16_t(frame.left + frame.right), uint16_t(frame.top + frame.bottom));
            }

            return area;
        }

        // area         - Window area without invisible frame.
        AreaIU16 AddInvisibleFrameTo(const AreaIU16& area, HWND window_hangle) const {
            const AreaIU16 correction = Get(window_hangle);
            return AreaIU16(
                area.x      + correction.x,
                area.y      + correction.y,
                area.width  + correction.width,
                area.height + correction.height
            );
        }

        // size         - Window size without invisible frame.
        SizeU16 AddInvisibleFrameTo(const SizeU16& size, HWND window_hangle) const {
            const AreaIU16 correction = Get(window_hangle);
            return SizeU16(
                size.width  + correction.width,
                size.height + correction.height
            );
        }

        // pos          - Window position without invisible frame.
        PointI AddInvisibleFrameTo(const PointI& pos, HWND window_hangle) const {
            const AreaIU16 correction = Get(window_hangle);
            return {
                pos.x + correction.x,
                pos.y + correction.y,
            };
        }

        // area         - Window area with invisible frame.
        AreaIU16 RemoveInvisibleFrameFrom(const AreaIU16& area, HWND window_hangle) const {
            const AreaIU16 correction = Get(window_hangle);
            return AreaIU16(
                area.x      - correction.x,
                area.y      - correction.y,
                area.width  - correction.width,
                area.height - correction.height
            );
        }

        // size         - Window size with invisible frame.
        SizeU16 RemoveInvisibleFrameFrom(const SizeU16& size, HWND window_hangle) const {
            const AreaIU16 correction = Get(window_hangle);
            return SizeU16(
                size.width  - correction.width,
                size.height - correction.height
            );
        }

        // pos          - Window position with invisible frame.
        PointI RemoveInvisibleFrameFrom(const PointI& pos, HWND window_hangle) const {
            const AreaIU16 correction = Get(window_hangle);
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
    };

    //--------------------------------------------------------------------------
    // WindowAreaCorrector
    //--------------------------------------------------------------------------

    class TextDrawer {
    public:
        TextDrawer() {
            m_device_context_handle = NULL;
            m_font_handle           = NULL;
            m_list_base             = 0;
        }
        virtual ~TextDrawer() {}

        // device_context_handle    - Must still exist for each call of RenderText.
        // name                     - Font name.
        // size                     - Height of characters in pixels.
        bool LoadFont(HDC device_context_handle, const std::string& name, uint16_t size, bool is_bold = false) {
            UnloadFont();

            m_device_context_handle = device_context_handle;

            m_font_handle = CreateFontA(
                size,                    
                0, 0, 0,                            
                is_bold ? FW_BOLD : FW_NORMAL,
                FALSE, FALSE, FALSE,
                ANSI_CHARSET,
                OUT_TT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                ANTIALIASED_QUALITY,
                FF_DONTCARE | DEFAULT_PITCH,
                name.c_str());  

            if (m_font_handle) {
                HFONT old_font_handle = (HFONT)SelectObject(m_device_context_handle, m_font_handle); 

                m_list_base = glGenLists(PRINT_LIST_LEN);
                if (m_list_base) {
                    bool is_success = wglUseFontBitmapsA(m_device_context_handle, 0, PRINT_LIST_LEN, m_list_base);
                    // Workaround for strange behavior. For POPUP window first call of wglUseFontBitmapsA fail with GetError() = 0.
                    // Second call, right after first, seams to succeed.
                    if (!is_success) is_success = wglUseFontBitmapsA(m_device_context_handle, 0, PRINT_LIST_LEN, m_list_base);

                    if (!is_success) {
                        m_err_msg = "Error: Can not font bitmap.";
                    }
                } else {
                    m_err_msg = "Error: Can not generate display list.";
                }

                SelectObject(m_device_context_handle, old_font_handle);
            } else {
                m_err_msg = "Error: Can not create font.";
            }

            if (!IsOk()) {
                UnloadFont();
            }

            return IsOk();
        }

        void UnloadFont() {
            m_err_msg = "";

            if (m_list_base) {
                glDeleteLists(m_list_base, PRINT_LIST_LEN);
                m_list_base = 0;
            }
            if (m_font_handle) {
                DeleteObject(m_font_handle);
                m_font_handle = NULL;
            }

            m_device_context_handle = NULL;
        }

        void RenderText(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const std::string& text) {
            if (m_list_base) {
                glPushAttrib(GL_ENABLE_BIT);
                glPushAttrib(GL_COLOR_BUFFER_BIT);
                glPushAttrib(GL_LIST_BIT);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
                
                glColor4ub(r, g, b, a);
                glRasterPos2i(x, y);

                glListBase(m_list_base);
                glCallLists((GLsizei)text.length(), GL_UNSIGNED_BYTE, text.c_str());

                glPopAttrib();
                glPopAttrib();
                glPopAttrib();
            }
        }

        SizeU16 GetTextSize(const std::string& text) const {
            if (m_font_handle) {
                HFONT old_font_handle = (HFONT)SelectObject(m_device_context_handle, m_font_handle); 

                SIZE size = {};
                BOOL is_success = GetTextExtentPoint32A(m_device_context_handle, text.c_str(), (int)text.length(), &size);

                SelectObject(m_device_context_handle, old_font_handle);

                if (is_success) return SizeU16((uint16_t)size.cx, (uint16_t)size.cy);
            }
            return {};
        }

        bool IsOk() const {
            return m_err_msg.empty();
        }

        std::string GetErrMsg() const {
            return m_err_msg;
        }

    private:
        enum { 
            PRINT_LIST_LEN      = 256,      // Size for full ascii table.
        };

        // No Copy
        TextDrawer(const TextDrawer&) = delete;
        TextDrawer& operator=(const TextDrawer&) = delete;

        HDC         m_device_context_handle;    // not own
        HFONT       m_font_handle;
        GLint       m_list_base;

        std::string m_err_msg;
    };

    inline int PointsToPixels(HDC hdc, int size) {
        return MulDiv(size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    }

    //--------------------------------------------------------------------------

    inline AreaIU16 GetDesktopAreaNoTaskBar() {
        RECT rc;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
        return MakeAreaIU16(rc);
    }

    inline SizeU16 GetDesktopAreaSizeNoTaskBar() {
        return GetDesktopAreaNoTaskBar().GetSize();
    }

    inline SizeU16 GetScreenSize() {
        return SizeU16(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    }

    inline PointI GetCursorPosInScreen() {
        POINT pos;
        if (GetCursorPos(&pos)) {
            return {pos.x, pos.y};
        }
        return {};
    }

    //--------------------------------------------------------------------------

    inline std::vector<std::string> Split(const std::string& text, char c) {
        std::string temp = text;
        std::vector<std::string> list;

        while (true) {
            size_t pos = temp.find(c, 0);

            if (pos == std::string::npos) {
                list.push_back(temp);
                break;
            } else {
                list.push_back(temp.substr(0, pos));
                temp = temp.substr(pos + 1);
            }
        }
        return list;
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
        auto& custom_log = Global<CustomLogFnP_T>::To();

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
        Global<CustomLogFnP_T>::To() = custom_log;
    }
} // namespace TrivialOpenGL 

#endif // TRIVIALOPENGL_UTILITY_H_
