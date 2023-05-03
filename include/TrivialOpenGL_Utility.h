/**
* @file TrivialOpenGL_Utility.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_UTILITY_H_
#define TRIVIALOPENGL_UTILITY_H_

#include "TrivialOpenGL_Headers.h"
#include "TrivialOpenGL_Debug.h"

//==============================================================================
// Declarations
//==============================================================================

// Disables copy ability of class.
#define TOGL_NO_COPY(Class) \
    Class(const Class&) = delete; \
    Class& operator=(const Class&) = delete

#define TOGL_CASE_STR(name) case name: return #name

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

        template <typename Type2>
        explicit Size(const Type2& s) : width(s), height(s) {}

        template <typename WidthType, typename HeightType>
        Size(const WidthType& width, const HeightType& height) : width(Type(width)), height(Type(height)) {}

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
    // Color
    //--------------------------------------------------------------------------

    template <typename Type>
    struct Color4 {
        Type r;
        Type g;
        Type b;
        Type a;

        Color4() : r(0), g(0), b(0), a(0) {}

        template <typename TypeR, typename TypeG, typename TypeB, typename TypeA>
        Color4(const TypeR& r, const TypeG& g, const TypeB& b, const TypeA& a) : r(Type(r)), g(Type(g)), b(Type(b)), a(Type(a)) {}

        Type* ToData() {
            return (Type*)this;
        }

        const Type* ToData() const {
            return (const Type*)this;
        }
    };

    using Color4U8 = Color4<uint8_t>;

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

    enum LogMessageType {
        LOG_MESSAGE_TYPE_FATAL_ERROR,
        LOG_MESSAGE_TYPE_INFO,
        LOG_MESSAGE_TYPE_DEBUG,
    };

    using CustomLogFnP_T = void (*)(LogMessageType message_type, const char* message);

    // Logs message to standard output (by default) or redirect to custom function (provided by SetHandleLogFunction).
    // message      - Message in ascii encoding.

    void LogDebug(const std::string& message);
    void LogInfo(const std::string& message);
    void LogFatalError(const std::string& message);

    void SetCustomLogFunction(CustomLogFnP_T custom_log);

    class Logger {
    public:
        friend Global<Logger>;

        virtual ~Logger() {}

        // message      - Message in ascii encoding.

        void LogDebug(const std::string& message) {
            Logger::Log(LOG_MESSAGE_TYPE_DEBUG, message);
        }

        void LogInfo(const std::string& message) {
            Logger::Log(LOG_MESSAGE_TYPE_INFO, message);
        }

        // This function exits executable with error code EXIT_FAILURE.
        void LogFatalError(const std::string& message) {
            Logger::Log(LOG_MESSAGE_TYPE_FATAL_ERROR, message);
        }

        void SetCustomLogFunction(CustomLogFnP_T custom_log) {
            m_custom_log = custom_log;
        }

    private:
        Logger() {
            m_custom_log = nullptr;
        }

        static std::string GetMessagePrefix(LogMessageType log_message_type) {
            switch (log_message_type) {
            case LOG_MESSAGE_TYPE_FATAL_ERROR:  return "(TOGL) Fatal Error: ";
            case LOG_MESSAGE_TYPE_INFO:         return "(TOGL) Info: ";
            case LOG_MESSAGE_TYPE_DEBUG:        return "(TOGL) Debug: ";
            }
            return "";
        }

        void Log(LogMessageType message_type, const std::string& message) {
            if (m_custom_log) {
                m_custom_log(message_type, message.c_str());
            } else {
                LogTextToStdOut(GetMessagePrefix(message_type) + message);
            }
            if (message_type == LOG_MESSAGE_TYPE_FATAL_ERROR) exit(EXIT_FAILURE);
        }

        void LogTextToStdOut(const std::string& message) {
            if (fwide(stdout, 0) > 0) {
                wprintf(L"%ls\n", ASCII_ToUTF16(message).c_str());
            } else {
                printf("%s\n", message.c_str());
            }
            fflush(stdout);
        }

        CustomLogFnP_T m_custom_log;
    };

    inline Logger& ToLogger() {
        return Global<Logger>::To();
    }

    //--------------------------------------------------------------------------
    // BMP
    //--------------------------------------------------------------------------

    // Saves image pixel data to file as BMP image.
    // file_name        - Full name with extension of BMP file. Variable encoding format: UTF8.
    // pixel_data       - Contains array of pixels. Each pixels contains for channel (in order: red, green, blue, alpha). 
    //                    Each channel occupies one byte (value range: 0 - 255).
    // width            - Length of single image row in pixels. And also, width of saved image. At least 1. 
    // height           - Number of rows. And also, height of saved image. At least 1. 
    // Returns true if image was saved to file.
    bool SaveAsBMP(const std::string& file_name, const uint8_t* pixel_data, uint32_t width, uint32_t height, bool is_reverse_rows = true);

    // Saves content of OpenGL texture to file as BMP image.
    // file_name        - Full name with extension of BMP file. Variable encoding format: UTF8.
    // tex_obj          - OpneGL Texture Object (Name).
    // Returns true if image was saved to file.
    bool SaveTextureAsBMP(const std::string& file_name, GLuint tex_obj);

    //--------------------------------------------------------------------------
    // Common
    //--------------------------------------------------------------------------

    AreaIU16 GetDesktopAreaNoTaskBar();
    SizeU16 GetDesktopAreaSizeNoTaskBar();
    SizeU16 GetScreenSize();
    PointI GetCursorPosInScreen();

    //--------------------------------------------------------------------------

    std::string HexToStr(uint16_t value);
    std::vector<std::string> Split(const std::string& text, char c);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // InnerUtility
    //--------------------------------------------------------------------------

    // Content this class is for this library inner purpose only.
    class InnerUtility {
    public:
        friend class Window;

    private:
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
        
            HRESULT (WINAPI *m_dwm_get_window_attribute)(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute);
        };
    };

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

    inline void LogDebug(const std::string& message) {
        ToLogger().LogDebug(message);
    }

    inline void LogInfo(const std::string& message) {
        ToLogger().LogInfo(message);
    }

    inline void LogFatalError(const std::string& message) {
        ToLogger().LogFatalError(message);
    }

    inline void SetCustomLogFunction(CustomLogFnP_T custom_log) {
        ToLogger().SetCustomLogFunction(custom_log);
    }

    //--------------------------------------------------------------------------
    // Common
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

    inline std::string HexToStr(uint16_t value) {
        std::stringstream stream;
        stream << std::hex << std::setfill('0') << std::setw(4) << std::right << std::uppercase << value;
        return stream.str();
    };

    //--------------------------------------------------------------------------
    // BMP
    //--------------------------------------------------------------------------

    inline bool SaveAsBMP(const std::string& file_name, const uint8_t* pixel_data, uint32_t width, uint32_t height, bool is_reverse_rows) {
        bool is_success = false; 

        if (width > 0 && height > 0) {
            enum {
                PIXEL_SIZE = 4, // in bytes
            };

            const uint32_t pixel_data_size      = width * height * PIXEL_SIZE;
            const uint32_t header_size          = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPV5HEADER);
            const uint32_t file_size            = header_size + pixel_data_size;

            uint8_t* file_data = new uint8_t[file_size];
            memset(file_data, 0, file_size);

            BITMAPFILEHEADER* fh = (BITMAPFILEHEADER*) file_data;
            fh->bfType          = 0x4D42; // bmp file signature = {'B', 'M'}
            fh->bfSize          = file_size;
            fh->bfOffBits       = header_size;

            BITMAPV5HEADER* ih = (BITMAPV5HEADER*)(file_data + sizeof(BITMAPFILEHEADER));
            ih->bV5Size         = sizeof(BITMAPV5HEADER);
            ih->bV5Width        = width;
            ih->bV5Height       = height;
            ih->bV5Planes       = 1;
            ih->bV5BitCount     = 32;
            ih->bV5Compression  = BI_BITFIELDS;
            ih->bV5SizeImage    = pixel_data_size;
            ih->bV5RedMask      = 0xFF000000;
            ih->bV5GreenMask    = 0x00FF0000;
            ih->bV5BlueMask     = 0x0000FF00;
            ih->bV5AlphaMask    = 0x000000FF;
            ih->bV5CSType       = LCS_sRGB;
            ih->bV5Intent       = LCS_GM_GRAPHICS;

            uint8_t* file_pixel_data = (uint8_t*)(file_data + header_size);
            uint32_t ix = 0;

            const uint32_t row_size = width * PIXEL_SIZE; // in bytes

            for (uint32_t column_ix = 0; column_ix < height; ++column_ix) {
                const uint8_t* column =  pixel_data + row_size * (is_reverse_rows ? (height - 1 - column_ix) : column_ix);

                for (uint32_t pixel_ix = 0; pixel_ix < width; ++pixel_ix) {
                    const uint8_t* pixel = column + pixel_ix * PIXEL_SIZE;

                    // reverse order of channels
                    file_pixel_data[ix++] = pixel[3];
                    file_pixel_data[ix++] = pixel[2];
                    file_pixel_data[ix++] = pixel[1];
                    file_pixel_data[ix++] = pixel[0];
                }
            }

            FILE* file = NULL;
            is_success = _wfopen_s(&file, ToUTF16(file_name).c_str(), L"wb") == 0 && file;

            if (is_success) {
                is_success = fwrite(file_data, 1, file_size, file) == file_size;
                fclose(file);
            }

            delete[] file_data;
        }

        return is_success;
    }

    inline bool SaveTextureAsBMP(const std::string& file_name, GLuint tex_obj) {
        bool is_success = false;

        glPushAttrib(GL_TEXTURE_BIT);
        glBindTexture(GL_TEXTURE_2D, tex_obj);

        GLint width     = 0;
        GLint height    = 0;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

        if (width > 0 && height > 0) {
            enum { PIXEL_SIZE = 4 }; // in bytes

            const uint32_t  data_size   = width * height * PIXEL_SIZE;
            uint8_t*        data        = new uint8_t[data_size];
                   
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            is_success = SaveAsBMP(file_name, data, width, height, false);
                    
            delete[] data;
        }

        glPopAttrib();

        return is_success;
    }


} // namespace TrivialOpenGL 

#endif // TRIVIALOPENGL_UTILITY_H_
