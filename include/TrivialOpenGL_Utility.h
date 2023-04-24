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
    // Font
    //--------------------------------------------------------------------------
    enum FontStyle {
        FONT_STYLE_NORMAL,
        FONT_STYLE_BOLD,
    };
    
    // Ranges are from unicode space.
    enum FontCharSet {
        FONT_CHAR_SET_ENGLISH,
    
        // Note: Font might not have all glyphs from this range.
        FONT_CHAR_SET_RANGE_0000_FFFF,
    };
    
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
        explicit Size(const Size<SizeType>& s) : width(s.width), height(s.height) {}

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

    std::vector<std::string> Split(const std::string& text, char c);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // InnerUtility
    //--------------------------------------------------------------------------

    // Content this class is for this library inner purpose only.
    class InnerUtility {
    public:
        friend class Window;
        friend class Font;

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

        //--------------------------------------------------------------------------
        // PixelDataBuffer
        //--------------------------------------------------------------------------

        class PixelDataBuffer{
        public:
            PixelDataBuffer() {
                m_fbo           = 0;
                m_prev_fbo      = 0;
                m_tex_obj       = 0;
                m_prev_tex_obj  = 0;

                m_glGenFramebuffersEXT          = nullptr;
                m_glDeleteFramebuffersEXT       = nullptr;
                m_glBindFramebufferEXT          = nullptr;
                m_glFramebufferTexture2DEXT     = nullptr;
                m_glCheckFramebufferStatusEXT   = nullptr;
            }
            virtual ~PixelDataBuffer() {

            }

            void SetUp(uint16_t width, uint16_t height) {
                Load(m_glGenFramebuffersEXT, "glGenFramebuffersEXT");
                Load(m_glDeleteFramebuffersEXT, "glDeleteRenderbuffersEXT");
                Load(m_glBindFramebufferEXT, "glBindFramebufferEXT");
                Load(m_glFramebufferTexture2DEXT, "glFramebufferTexture2DEXT");
                Load(m_glCheckFramebufferStatusEXT, "glCheckFramebufferStatusEXT");

                GLint max_viewport_size[2] = {};
                glGetIntegerv(GL_MAX_VIEWPORT_DIMS, max_viewport_size);
                GLint max_texture_size = 0;
                glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

                if (width > max_viewport_size[0] || width > max_texture_size || height > max_viewport_size[1] || height > max_texture_size) {
                    AddErrMsg(std::string() + "Value of width or/and height are to big. ("
                        "max_viewport_width=" + std::to_string(max_viewport_size[0]) + 
                        ", max_viewport_height=" + std::to_string(max_viewport_size[1]) + 
                        ", max_texture_width=" + std::to_string(max_texture_size) + 
                        ", max_texture_height=" + std::to_string(max_texture_size) + 
                        ")"
                    );
                }

                if (IsOk()) {
                    m_glGenFramebuffersEXT(1, &m_fbo);
                    glGetIntegerv(TOGL_GL_FRAMEBUFFER_BINDING, (GLint*)&m_prev_fbo);
                    m_glBindFramebufferEXT(TOGL_GL_FRAMEBUFFER_EXT, m_fbo);

                    glGenTextures(1, &m_tex_obj);
                    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&m_prev_tex_obj);
                    glBindTexture(GL_TEXTURE_2D, m_tex_obj);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

                    m_glFramebufferTexture2DEXT(TOGL_GL_FRAMEBUFFER_EXT, TOGL_GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_tex_obj, 0);
                    
                    if (m_glCheckFramebufferStatusEXT(TOGL_GL_FRAMEBUFFER_EXT) != TOGL_GL_FRAMEBUFFER_COMPLETE_EXT) {
                        AddErrMsg("Framebuffer is not complete.");
                    }
                }
            }

            // Returned texture object (opengl texture name) needs to be deleted (by glDeleteTextures function).
            GLint TakeOverTexObj() {
                m_glFramebufferTexture2DEXT(TOGL_GL_FRAMEBUFFER_EXT, TOGL_GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 0, 0);
                glBindTexture(GL_TEXTURE_2D, m_prev_tex_obj);

                GLint tex_obj = m_tex_obj;
                m_tex_obj = 0;
                return tex_obj;
            }

            void CleanUp() {
                m_glFramebufferTexture2DEXT(TOGL_GL_FRAMEBUFFER_EXT, TOGL_GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 0, 0);
                glBindTexture(GL_TEXTURE_2D, m_prev_tex_obj);
                glDeleteTextures(1, &m_tex_obj);

                m_glBindFramebufferEXT(TOGL_GL_FRAMEBUFFER_EXT, m_prev_fbo);
                m_glDeleteFramebuffersEXT(1, &m_fbo);

                m_prev_tex_obj  = 0;
                m_tex_obj       = 0;
                m_prev_fbo      = 0;
                m_fbo           = 0;
            }

            bool IsOk() const {
                return m_err_msg.empty();
            }

            std::string GetErrMsg() const {
                return m_err_msg;
            }

        private:
            enum {
                TOGL_GL_FRAMEBUFFER_EXT             = 0x8D40,
                TOGL_GL_FRAMEBUFFER_BINDING         = 0x8CA6,
                TOGL_GL_COLOR_ATTACHMENT0_EXT       = 0x8CE0,
                TOGL_GL_FRAMEBUFFER_COMPLETE_EXT    = 0x8CD5,
            };

            template <typename Type>
            void Load(Type& function, const std::string& function_name) {
                function = (Type)wglGetProcAddress(function_name.c_str());
                if (!function) {
                    AddErrMsg(std::string() + "Can not load function: \"" + function_name + "\".");
                }
            }

            void AddErrMsg(const std::string& err_msg) {
                if (!m_err_msg.empty()) m_err_msg += "\n";
                m_err_msg += std::string() + "PixelDataBuffer Error: " + err_msg;
            }

            GLuint m_fbo;
            GLuint m_prev_fbo;
            GLuint m_tex_obj;
            GLuint m_prev_tex_obj;

            std::string m_err_msg;

            void (APIENTRY *m_glGenFramebuffersEXT)(GLsizei n, GLuint *framebuffers);
            void (APIENTRY *m_glDeleteFramebuffersEXT)(GLsizei n, const GLuint *framebuffers);
            void (APIENTRY *m_glBindFramebufferEXT)(GLenum target, GLuint framebuffer);
            void (APIENTRY *m_glFramebufferTexture2DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
            GLenum (APIENTRY* m_glCheckFramebufferStatusEXT)(GLenum target);
        };

        //--------------------------------------------------------------------------
        // TextDrawer
        //--------------------------------------------------------------------------

        class TextDrawer {
        public:
            TextDrawer() {
                m_font_height = 0;

                m_device_context_handle = NULL;
                m_font_handle           = NULL;
                m_list_base             = 0;
                m_list_range            = 0;

                m_height                = 0;
                m_ascent                = 0;
                m_descent               = 0;
                m_internal_leading      = 0;
            }
            virtual ~TextDrawer() {

            }

            // device_context_handle    - Must still exist for each call of RenderText.
            // name                     - Font name. Encoding Format: ASCII.
            // size                     - Height of characters in pixels.
            bool LoadFont(HDC device_context_handle, const std::string& name, uint16_t height, FontStyle style, FontCharSet char_set) {
                UnloadFont();

                m_device_context_handle = device_context_handle;
                m_font_height = height;

                m_font_handle = CreateFontW(
                    height,                    
                    0, 0, 0,                            
                    (style == FONT_STYLE_BOLD) ? FW_BOLD : FW_NORMAL,
                    FALSE, FALSE, FALSE,
                    ANSI_CHARSET,
                    OUT_TT_PRECIS,
                    CLIP_DEFAULT_PRECIS,
                    ANTIALIASED_QUALITY,
                    FF_DONTCARE | DEFAULT_PITCH,
                    ToUTF16(name).c_str());  
                

                if (m_font_handle) {
                    HFONT old_font_handle = (HFONT)SelectObject(m_device_context_handle, m_font_handle); 

                    // --- Gets Font Metrics --- //
                    TEXTMETRICW metric;
                    GetTextMetricsW(m_device_context_handle, &metric);
                    m_height            = metric.tmHeight;
                    m_ascent            = metric.tmAscent;
                    m_descent           = metric.tmDescent;
                    m_internal_leading  = metric.tmInternalLeading;

                    // --- Gets Code Ranges --- //

                    m_code_ranges.clear();

                    DWORD buffer_size = GetFontUnicodeRanges(device_context_handle, NULL);
                    BYTE* buffer = new BYTE[buffer_size];

                    GLYPHSET* glyphset = (GLYPHSET*)buffer;
                    GetFontUnicodeRanges(device_context_handle, glyphset);
                    // debug
                    //togl_print_i32(glyphset->cbThis);
                    //togl_print_i32(glyphset->flAccel);
                    //togl_print_i32(glyphset->cGlyphsSupported);
                    //togl_print_i32(glyphset->cRanges);

                    for (uint32_t ix = 0; ix < glyphset->cRanges; ++ix) {
                        const uint32_t from = glyphset->ranges[ix].wcLow;
                        const uint32_t to = from + glyphset->ranges[ix].cGlyphs - 1;
                        m_code_ranges.push_back({from, to});

                        //printf("[%04X..%04X]\n", from, to); // debug
                    }

                    delete[] buffer;

                    // --- Generates Inner Font Bitmap  --- //

                    m_list_range = GetListRange(char_set);
                    m_list_base = glGenLists(m_list_range);
                    if (m_list_base) {
                        bool is_success = wglUseFontBitmapsW(m_device_context_handle, 0, m_list_range, m_list_base);

                        // Workaround for strange behavior. For POPUP window first call of wglUseFontBitmapsA fail with GetError() = 0.
                        // Second call, right after first, seams to succeed.
                        if (!is_success) is_success = wglUseFontBitmapsA(m_device_context_handle, 0, m_list_range, m_list_base);

                        if (!is_success) {
                            m_err_msg = "Error: Can not font bitmap.";
                        }
                    } else {
                        m_err_msg = "Error: Can not generate display list.";
                    }

                    // ---

                    SelectObject(m_device_context_handle, old_font_handle);
                } else {
                    m_err_msg = "Error: Can not create font.";
                }

                if (!IsOk()) {
                    UnloadFont();
                }

                return IsOk();
            }

            void SaveFont(const std::string& file_name, uint16_t width, uint16_t height) {
                if (m_list_base && m_font_height > 0) {
                    m_pixel_data_buffer.SetUp(width, height);

                    if (!m_pixel_data_buffer.IsOk()) {
                        LogFatalError(m_pixel_data_buffer.GetErrMsg());
                    }

                    glPushAttrib(GL_VIEWPORT_BIT);
                    glViewport(0, 0, width, height);

                    glPushAttrib(GL_MATRIX_MODE);
                    glMatrixMode(GL_PROJECTION);
                    glPushMatrix();
                    glLoadIdentity();
                    glOrtho(0, width, 0, height, 1, -1);
                    glMatrixMode(GL_MODELVIEW);
                    glPushMatrix();
                    glLoadIdentity();

                    glPushAttrib(GL_COLOR_BUFFER_BIT);
                    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                    glClear(GL_COLOR_BUFFER_BIT);

                    PointI pos = {0, height - m_font_height};

                    const bool is_full_range = false;
                    if (is_full_range) {
                        for (uint32_t code = 0; code <= uint32_t(m_list_range); ++code) {

                            const std::wstring c(1, (wchar_t)code);

                            const SizeU16 size = GetTextSize(c);
                       
                            RenderTextUTF16(pos.x, pos.y, 255, 255, 255, 255, c);
                    
                            pos.x += size.width;
                            if (pos.x >= width) {
                                pos.x = 0;
                                pos.y -= m_font_height;
                            }
                    
                            if (pos.y >= height) {
                                break;
                            }
                        }
                    } else {
                        for (const CodeRange& code_range : m_code_ranges) {
                            for (uint32_t code = code_range.from; code <= code_range.to; ++code) {

                                const std::wstring c(1, (wchar_t)code);

                                const SizeU16 size = GetTextSize(c);
                       
                                RenderTextUTF16(pos.x, pos.y, 255, 255, 255, 255, c);
                    
                                pos.x += size.width;
                                if (pos.x >= width) {
                                    pos.x = 0;
                                    pos.y -= m_font_height;
                                }
                    
                                if (pos.y >= height) {
                                    break;
                                }
                            }
                        }
                    }

 
                    GLuint font_tex_obj = m_pixel_data_buffer.TakeOverTexObj();

                    glMatrixMode(GL_PROJECTION);
                    glPopMatrix();
                    glMatrixMode(GL_MODELVIEW);
                    glPopMatrix();
                    
                    glPopAttrib();
                    glPopAttrib();
                    glPopAttrib();

                    m_pixel_data_buffer.CleanUp();

                    // ---
          
                    SaveTextureAsBMP(file_name, font_tex_obj);

                    glDeleteTextures(1, &font_tex_obj);
                }
            }

            void UnloadFont() {
                m_err_msg = "";

                if (m_list_base) {
                    glDeleteLists(m_list_base, m_list_range);
                    m_list_base = 0;
                }
                if (m_font_handle) {
                    DeleteObject(m_font_handle);
                    m_font_handle = NULL;
                }

                m_device_context_handle = NULL;
            }

            // text - Encoding Format: ASCII.
            void RenderTextASCII(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const std::string& text) {
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

            void RenderTextUTF16(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const std::wstring& text) {
                if (m_list_base) {
                    glPushAttrib(GL_ENABLE_BIT);
                    glPushAttrib(GL_COLOR_BUFFER_BIT);
                    glPushAttrib(GL_LIST_BIT);

                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
                
                    glColor4ub(r, g, b, a);
                    glRasterPos2i(x, y);

                    glListBase(m_list_base);
                    glCallLists((GLsizei)text.length(), GL_UNSIGNED_SHORT, text.c_str());

                    glPopAttrib();
                    glPopAttrib();
                    glPopAttrib();
                }
            }

            // text - Encoding Format: UTF8.
            void RenderText(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const std::string& text) {
                RenderTextUTF16(x, y, r, g, b, a, ToUTF16(text));
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

            SizeU16 GetTextSize(const std::wstring& text) const {
                if (m_font_handle) {
                    HFONT old_font_handle = (HFONT)SelectObject(m_device_context_handle, m_font_handle); 

                    SIZE size = {};
                    BOOL is_success = GetTextExtentPoint32W(m_device_context_handle, text.c_str(), (int)text.length(), &size);

                    SelectObject(m_device_context_handle, old_font_handle);

                    if (is_success) return SizeU16((uint16_t)size.cx, (uint16_t)size.cy);
                }
                return {};
            }

            SizeU16 GetCharSize(wchar_t c) const {
                if (m_font_handle) {
                    HFONT old_font_handle = (HFONT)SelectObject(m_device_context_handle, m_font_handle); 

                    SIZE size = {};
                    BOOL is_success = GetTextExtentPoint32W(m_device_context_handle, &c, 1, &size);

                    SelectObject(m_device_context_handle, old_font_handle);

                    if (is_success) return SizeU16((uint16_t)size.cx, (uint16_t)size.cy);
                }
                return {};
            }

            uint32_t GetFontDescent() const {
                return m_descent;
            }

            bool IsOk() const {
                return m_err_msg.empty();
            }

            std::string GetErrMsg() const {
                return m_err_msg;
            }

        private:
            enum { 
                ASCII_LIST_RANGE = 128,      // Full ASCII table.
                UNICODE_0000_FFFF_LIST
            };

            struct CodeRange {
                uint32_t from;
                uint32_t to;
            };

            TOGL_NO_COPY(TextDrawer);

            static GLsizei GetListRange(FontCharSet font_char_set) {
                switch (font_char_set) {
                case FONT_CHAR_SET_ENGLISH:             return 128;     // 0x80
                case FONT_CHAR_SET_RANGE_0000_FFFF:   return 65535;   // 0xFFFF
                }
                return 0;
            }

            uint16_t    m_font_height;
            HDC         m_device_context_handle;    // not own
            HFONT       m_font_handle;
            GLint       m_list_base;
            GLsizei     m_list_range;

            std::vector<CodeRange> m_code_ranges;

            uint32_t    m_height;
            uint32_t    m_ascent;
            uint32_t    m_descent;
            uint32_t    m_internal_leading;

            PixelDataBuffer m_pixel_data_buffer;

            std::string m_err_msg;
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
