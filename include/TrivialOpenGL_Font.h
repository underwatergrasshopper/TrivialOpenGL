/**
* @file TrivialOpenGL_Font.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_FONT_H_
#define TRIVIALOPENGL_FONT_H_

#include "TrivialOpenGL_Window.h"

namespace TrivialOpenGL {

    //==========================================================================
    // Declarations
    //==========================================================================

    enum FontSizeUnit {
        FONT_SIZE_UNIT_PIXELS,
        FONT_SIZE_UNIT_POINTS,
    };

    //enum FontStyle {
    //    FONT_STYLE_NORMAL,
    //    FONT_STYLE_BOLD,
    //};
    // 
    //// Ranges are from unicode space.
    //enum FontCharSet {
    //    FONT_CHAR_SET_ENGLISH,
    //
    //    // Note: Font might not have all glyphs from this range.
    //    FONT_CHAR_SET_RANGE_0000_FFFF,
    //};

    enum {
        // "WHITE SQUARE" 
        // Represents missing glyph in unicode space.
        CODE_WHITE_SQUARE               = 0x25A1,

        // "REPLACEMENT CHARACTER"
        // Represent out of range character code from unicode space.
        CODE_REPLACEMENT_CHARACTER      = 0xFFFD,
    };

    struct FontInfo {
        std::string     name;       // encoding format: UTF8
        uint32_t        size;
        FontSizeUnit    size_unit;
        FontStyle       style;
        FontCharSet     char_set;

        FontInfo() {
            name        = "";
            size        = 0;
            size_unit   = FONT_SIZE_UNIT_PIXELS;
            style       = FONT_STYLE_NORMAL;
            char_set    = FONT_CHAR_SET_ENGLISH;
        }
        
        FontInfo(const std::string& name, uint32_t size, FontSizeUnit size_unit, FontStyle style, FontCharSet char_set) {
            this->name        = name;
            this->size        = size;    
            this->size_unit   = size_unit;
            this->style       = style;    
            this->char_set    = char_set;
        }
    };

    struct GlyphData {
        uint16_t    width;      // in pixels
        GLuint      tex_obj;    // opengl texture object (name)

        // texture coordinates
        double      x1;
        double      y1;
        double      x2;
        double      y2;

        GlyphData() {
            width       = 0;
            tex_obj     = 0;
                
            x1          = 0;
            y1          = 0;
            x2          = 0;
            y2          = 0;
        }
    };

    struct FontData {
        FontInfo info;

        uint32_t glyph_height;               // in pixels
        uint32_t glyph_ascent;               // in pixels
        uint32_t glyph_descent;              // in pixels
        uint32_t glyph_internal_leading;     // in pixels

        std::map<uint32_t, GlyphData>   glyphs;     // indexed by character code from unicode space
        std::vector<GLuint>             tex_objs;   // opengl texture objects (names)

        FontData() {
            info                    = {};

            glyph_height            = 0;           
            glyph_ascent            = 0;           
            glyph_descent           = 0;          
            glyph_internal_leading  = 0; 
        }
    };

    class FontDataGenerator {
    public:
        FontDataGenerator() {
            m_device_context_handle = NULL;
        }
        virtual ~FontDataGenerator() {

        }

        FontData Generate(const FontInfo& font_info) {
            m_data.info     = font_info;
            m_err_msg       = "";

            WindowInnerAccessor& window_inner_accessor = ToWindow();
            HWND window_handle = window_inner_accessor.ToHWND();

            m_device_context_handle = GetDC(window_handle);

            m_data.glyph_height = ToPixels(font_info.size, font_info.size_unit);

            HFONT font_handle = CreateFontW(
                    m_data.glyph_height,                    
                    0, 0, 0,                            
                    (font_info.style == FONT_STYLE_BOLD) ? FW_BOLD : FW_NORMAL,
                    FALSE, FALSE, FALSE,
                    SolveCreateFontCharSet(font_info.char_set),
                    OUT_TT_PRECIS,
                    CLIP_DEFAULT_PRECIS,
                    ANTIALIASED_QUALITY,
                    FF_DONTCARE | DEFAULT_PITCH,
                    ToUTF16(font_info.name).c_str());  

            if (font_handle == NULL) {
                AddErrMsg("Can not create font source.");
            } else {
                HFONT old_font_handle = (HFONT)SelectObject(m_device_context_handle, font_handle); 

                // --- Gets Font Metrics --- //
                TEXTMETRICW metric;
                GetTextMetricsW(m_device_context_handle, &metric);
                const uint32_t glyph_height    = metric.tmHeight;
                m_data.glyph_ascent            = metric.tmAscent;
                m_data.glyph_descent           = metric.tmDescent;
                m_data.glyph_internal_leading  = metric.tmInternalLeading;

                if (glyph_height != m_data.glyph_height) {
                    AddErrMsg("Mismatch between font height requested and created.");
                } else {
                    // --- Gets Code Ranges --- //

                    DWORD buffer_size = GetFontUnicodeRanges(m_device_context_handle, NULL);
                    BYTE* buffer = new BYTE[buffer_size];

                    GLYPHSET* glyphset = (GLYPHSET*)buffer;
                    GetFontUnicodeRanges(m_device_context_handle, glyphset);
                    // debug
                    //togl_print_i32(glyphset->cbThis);
                    //togl_print_i32(glyphset->flAccel);
                    //togl_print_i32(glyphset->cGlyphsSupported);
                    //togl_print_i32(glyphset->cRanges);

                    // --- Solves Code Ranges --- //
                    switch (font_info.char_set) {
                    case FONT_CHAR_SET_ENGLISH:
                        m_code_ranges.push_back({0x0020, 0x007E});  
                        m_code_ranges.push_back({CODE_WHITE_SQUARE}); 
                        m_code_ranges.push_back({CODE_REPLACEMENT_CHARACTER}); 
                        break;
                    case FONT_CHAR_SET_RANGE_0000_FFFF:
                        m_code_ranges.push_back({0x0000, 0xFFFF});
                        break;
                    default:
                        for (uint32_t ix = 0; ix < glyphset->cRanges; ++ix) {
                            const uint32_t from = glyphset->ranges[ix].wcLow;
                            const uint32_t to   = from + glyphset->ranges[ix].cGlyphs - 1;

                            m_code_ranges.push_back({from, to});
                        }
                        break;
                    }

                    delete[] buffer;

                    //for (const auto& code_range : m_code_ranges) printf("[%04X..%04X]\n", code_range.from, code_range.to); // debug

                    // --- Generates Display Lists and Intermediary Font Bitmaps --- //
                    auto HexToStr = [](uint16_t value) -> std::string {
                        std::stringstream stream;
                        stream << std::hex << std::setfill('0') << std::setw(4) << std::right << value;
                        return stream.str();
                    };

                    for (auto& code_range : m_code_ranges) {
                        code_range.list_first = code_range.from;
                        code_range.list_range = code_range.to - code_range.from + 1;
                        code_range.list_base = glGenLists(code_range.list_range);

                        if (code_range.list_base == 0) {
                            AddErrMsg(std::string() + "Error: Can not generate display list for unicode range [" + HexToStr(code_range.from) + ".." + HexToStr(code_range.to) + "].");
                            break;
                        }

                        bool is_success = wglUseFontBitmapsW(m_device_context_handle, code_range.list_first, code_range.list_range, code_range.list_base);

                        // Workaround for strange behavior. For POPUP window first call of wglUseFontBitmapsA fail with GetError() = 0.
                        // Second call, right after first, seams to succeed.
                        if (!is_success) is_success = wglUseFontBitmapsW(m_device_context_handle, code_range.list_first, code_range.list_range, code_range.list_base);

                        if (!is_success) {
                            AddErrMsg("Error: Can not create intermediary font bitmap for unicode range [" + HexToStr(code_range.from) + ".." + HexToStr(code_range.to) + "].");
                            break;
                        }

                        togl_print_i32(code_range.list_first);
                        togl_print_i32(code_range.list_range);
                        togl_print_i32(code_range.list_base);
                    }

                    // --- Generate Font Textures --- //

                    if (IsOk()) {
                        GenerateFontTextures(1024, 1024);
                    }

                    // --- Destroys Display Lists and Clears Ranges --- //

                    for (auto& code_range : m_code_ranges) {
                        glDeleteLists(code_range.list_base, code_range.list_range);
                    }
                    m_code_ranges.clear();

                }

                SelectObject(m_device_context_handle, old_font_handle);
                DeleteObject(font_handle);
            }

            ReleaseDC(window_handle, m_device_context_handle);
            m_device_context_handle = NULL;

            FontData data = m_data;

            m_data = {};

            return IsOk() ? data : FontData();
        }

        bool IsOk() const {
            return m_err_msg.empty();
        }

        std::string GetErrMsg() const {
            return m_err_msg;
        }

    private:
        struct CodeRange {
            uint32_t    from;
            uint32_t    to;

            GLuint      list_first;
            GLsizei     list_range;
            GLuint      list_base;

            CodeRange() {
                from            = 0;
                to              = 0;

                list_first      = 0;
                list_range      = 0;
                list_base       = 0; 
            }

            CodeRange(uint32_t code) {
                from            = code;
                to              = code;

                list_first      = 0;
                list_range      = 0;
                list_base       = 0; 
            }

            CodeRange(uint32_t from, uint32_t to) {
                this->from      = from;
                this->to        = to;

                list_first      = 0;
                list_range      = 0;
                list_base       = 0; 
            }
        };

        class FrameBuffer {
        public:
            FrameBuffer(uint16_t width, uint16_t height) {
                Load(m_glGenFramebuffersEXT, "glGenFramebuffersEXT");
                Load(m_glDeleteFramebuffersEXT, "glDeleteFramebuffersEXT");
                Load(m_glBindFramebufferEXT, "glBindFramebufferEXT");
                Load(m_glFramebufferTexture2DEXT, "glFramebufferTexture2DEXT");
                Load(m_glCheckFramebufferStatusEXT, "glCheckFramebufferStatusEXT");
                    
                m_width     = width;
                m_height    = height;

                if (IsOk()) {
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
                }

                m_fbo       = 0;
                m_prev_fbo  = 0;

                if (IsOk()) {
                    m_glGenFramebuffersEXT(1, &m_fbo);

                    glGetIntegerv(TOGL_GL_FRAMEBUFFER_BINDING, (GLint*)&m_prev_fbo);
                    m_glBindFramebufferEXT(TOGL_GL_FRAMEBUFFER_EXT, m_fbo);
                }

                glPushAttrib(GL_ENABLE_BIT);
                glPushAttrib(GL_TEXTURE_BIT);
            }

            virtual ~FrameBuffer() {
                if (IsOk()) m_glFramebufferTexture2DEXT(TOGL_GL_FRAMEBUFFER_EXT, TOGL_GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 0, 0);

                glPopAttrib();
                glPopAttrib();

                if (IsOk()) {
                    m_glBindFramebufferEXT(TOGL_GL_FRAMEBUFFER_EXT, m_prev_fbo);
                    m_glDeleteFramebuffersEXT(1, &m_fbo);
                }
            }

            GLuint GenAndBindTex() {
                GLuint tex_obj = 0;

                if (IsOk()) {
                    glGenTextures(1, &tex_obj);
                    glBindTexture(GL_TEXTURE_2D, tex_obj);
                    glDisable(GL_TEXTURE_2D);

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

                    m_glFramebufferTexture2DEXT(TOGL_GL_FRAMEBUFFER_EXT, TOGL_GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex_obj, 0);
                    
                    if (m_glCheckFramebufferStatusEXT(TOGL_GL_FRAMEBUFFER_EXT) != TOGL_GL_FRAMEBUFFER_COMPLETE_EXT) {
                        AddErrMsg("Frame buffer is not complete.");

                        m_glFramebufferTexture2DEXT(TOGL_GL_FRAMEBUFFER_EXT, TOGL_GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 0, 0);
                        glDeleteTextures(1, &tex_obj);
                        tex_obj = 0;
                    } 
                }

                return tex_obj;
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

            void AddErrMsg(const std::string& err_msg) {
                if (!m_err_msg.empty()) m_err_msg += "\n";
                m_err_msg += "Font FrameBuffer Error: ";
                m_err_msg += err_msg;
            }

            template <typename Type>
            void Load(Type& function, const std::string& function_name) {
                function = (Type)wglGetProcAddress(function_name.c_str());
                if (!function) {
                    AddErrMsg(std::string() + "Can not load function: \"" + function_name + "\".");
                }
            }

            void (APIENTRY *m_glGenFramebuffersEXT)(GLsizei n, GLuint *framebuffers);
            void (APIENTRY *m_glDeleteFramebuffersEXT)(GLsizei n, const GLuint *framebuffers);
            void (APIENTRY *m_glBindFramebufferEXT)(GLenum target, GLuint framebuffer);
            void (APIENTRY *m_glFramebufferTexture2DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
            GLenum (APIENTRY* m_glCheckFramebufferStatusEXT)(GLenum target);

            uint16_t    m_width;
            uint16_t    m_height;

            GLuint      m_fbo;
            GLuint      m_prev_fbo;

            std::string m_err_msg;
        };

        void Initialize() {
            m_device_context_handle = NULL;

            m_data = {};
        }

        void GenerateFontTextures(uint16_t width, uint16_t height) {
            FrameBuffer frame_buffer(width, height);
            GLuint tex_obj = frame_buffer.GenAndBindTex();

            if (frame_buffer.IsOk()) {
                m_data.tex_objs.push_back(tex_obj);

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

                int y = int(height) - int(m_data.glyph_height);
                PointI pos = {0, y};
 
                for (const CodeRange& code_range : m_code_ranges) {
                    for (uint32_t code = code_range.from; code <= code_range.to; ++code) {

                        const SizeU16 size = GetCharSize((wchar_t)code);

                        if ((pos.x + size.width) >= width) {
                            pos.x = 0;

                            if ((pos.y - m_data.glyph_height) <= 0) {
                                // run out of space, generate next texture
                                tex_obj = frame_buffer.GenAndBindTex();

                                if (!frame_buffer.IsOk()) {
                                    MergErrMsg(frame_buffer.GetErrMsg());
                                    break;
                                }
                                glClear(GL_COLOR_BUFFER_BIT);

                                m_data.tex_objs.push_back(tex_obj);

                                //printf("T%d [%04X]\n", (int)m_data.tex_objs.size() - 1, code); // debug

                                pos = {0, y};
                            } else {
                                pos.y -= m_data.glyph_height;
                            }
                        }

                        auto ToTexSpace = [](uint16_t pos, uint16_t size) -> double {
                            return pos / double(size);
                        };

                        GlyphData glyph_data = {};

                        glyph_data.width = size.width;
                        glyph_data.tex_obj = tex_obj;

                        glyph_data.x1 = ToTexSpace(pos.x, width);
                        glyph_data.y1 = ToTexSpace(pos.y - m_data.glyph_descent, height);
                        glyph_data.x2 = ToTexSpace(pos.x + size.width, width);
                        glyph_data.y2 = ToTexSpace(pos.y - m_data.glyph_descent + m_data.glyph_height, height);

                        m_data.glyphs[code] = glyph_data;
              
                        RenderGlyphToTexture(
                            code_range.list_base, 
                            pos.x, pos.y, 
                            (wchar_t)(code - code_range.list_first) // corrects character code to match glyph index in display list
                        );
                    
                        pos.x += size.width;
                    }
                }

                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glPopMatrix();
                    
                glPopAttrib();
                glPopAttrib();
                glPopAttrib();
            }
        }

        void RenderGlyphToTexture(GLint list_base, int x, int y, wchar_t c) {
            glPushAttrib(GL_ENABLE_BIT);
            glPushAttrib(GL_COLOR_BUFFER_BIT);
            glPushAttrib(GL_LIST_BIT);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
                
            glColor4ub(255, 255, 255, 255);
            glRasterPos2i(x, y);

            glListBase(list_base);
            glCallLists(1, GL_UNSIGNED_SHORT, &c);

            glPopAttrib();
            glPopAttrib();
            glPopAttrib();
        }

        SizeU16 GetCharSize(wchar_t c) const {
            SIZE size;
            if (GetTextExtentPoint32W(m_device_context_handle, &c, 1, &size)) {
                return SizeU16(size.cx, size.cy);
            }
            return {};
        }

        void AddErrMsg(const std::string& err_msg) {
            if (!m_err_msg.empty()) m_err_msg += "\n";
            m_err_msg += "FontGenerator Error: ";
            m_err_msg += err_msg;
        }

        void MergErrMsg(const std::string& err_msg) {
            if (!m_err_msg.empty()) m_err_msg += "\n";
            m_err_msg += err_msg;
        }

        static uint32_t PointsToPixels(uint32_t points) {
            return points * 4 / 3;
        }

        static uint32_t ToPixels(uint32_t size, FontSizeUnit size_unit) {
            switch (size_unit) {
            case FONT_SIZE_UNIT_PIXELS: return size;
            case FONT_SIZE_UNIT_POINTS: return PointsToPixels(size);
            }
            return 0;
        }

        static DWORD SolveCreateFontCharSet(FontCharSet char_set) {
            switch (char_set) {
            case FONT_CHAR_SET_ENGLISH:            return ANSI_CHARSET;
            case FONT_CHAR_SET_RANGE_0000_FFFF:    return ANSI_CHARSET;
            }
            return 0;
        }

        template <typename Type>
        void Load(Type& function, const std::string& function_name) {
            function = (Type)wglGetProcAddress(function_name.c_str());
            if (!function) {
                AddErrMsg(std::string() + "Can not load function: \"" + function_name + "\".");
            }
        }

        FontData                m_data;
        std::string             m_err_msg;

        HDC                     m_device_context_handle;
        std::vector<CodeRange>  m_code_ranges;
    };

    class Font {
    public:
        Font() {
            Initialize();
        }
        virtual ~Font() {
            Unload();
        }

        void Load(const std::string& name, uint32_t size, FontSizeUnit size_unit, FontStyle style, FontCharSet char_set) {
            Unload();

            FontDataGenerator font_data_generator;
            m_data = font_data_generator.Generate({name, size, size_unit, style, char_set});

            if (font_data_generator.IsOk()) {
                m_is_loaded = true;
            } else {
                m_data = {};
                MergeErrMsg(font_data_generator.GetErrMsg());
            }
        } 

        void Unload() {
            for (auto& tex_obj : m_data.tex_objs) {
                glDeleteTextures(1, &tex_obj);
            }
            Initialize();
        }
        bool IsLoaded() const {
            return m_is_loaded;
        }

        void RenderBegin() {
            glPushAttrib(GL_TEXTURE_BIT);
            glPushAttrib(GL_ENABLE_BIT);
            glPushAttrib(GL_COLOR_BUFFER_BIT);
            glPushAttrib(GL_LIST_BIT);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
        }

        void RenderEnd() {
            glPopAttrib();
            glPopAttrib();
            glPopAttrib();
            glPopAttrib();
        }

        void RenderGlyph(uint32_t code) {
            if (m_is_loaded) {
                const GlyphData& glyph_data = FindOrAddGlyphData(code);

                if (glyph_data.tex_obj != 0) {
                    glBindTexture(GL_TEXTURE_2D, glyph_data.tex_obj);
                    glEnable(GL_TEXTURE_2D);

                    glBegin(GL_TRIANGLE_FAN);
                    glTexCoord2d(glyph_data.x1, glyph_data.y1);
                    glVertex2i(0, 0);

                    glTexCoord2d(glyph_data.x2, glyph_data.y1);
                    glVertex2i(glyph_data.width, 0);

                    glTexCoord2d(glyph_data.x2, glyph_data.y2);
                    glVertex2i(glyph_data.width, m_data.glyph_height);
                
                    glTexCoord2d(glyph_data.x1, glyph_data.y2);
                    glVertex2i(0, m_data.glyph_height);
                    glEnd();
                } else {
                    // Renders replacement for missing glyph.
                    glDisable(GL_TEXTURE_2D);

                    glBegin(GL_TRIANGLE_FAN);
                    glVertex2i(0, 0);
                    glVertex2i(glyph_data.width, 0);
                    glVertex2i(glyph_data.width, m_data.glyph_height);
                    glVertex2i(0, m_data.glyph_height);
                    glEnd();
                }
            }
        } 

        // Renders array of glyphs. 
        // Special characters (like '\n', '\t', ... and so on) are interpreted as "unknown characters".
        // text         - Encoding format: UTF8.
        void RenderGlyphs(const std::string& text) {
            if (m_is_loaded) {
                const std::wstring text_utf16 = ToUTF16(text);

                RenderBegin();
                int x = 0;
                for (const uint32_t& code : text_utf16) {
                    glPushMatrix();
                    glTranslatef(float(x), 0, 0);
                    RenderGlyph(code);
                    x += GetGlyphSize(code).width;
                    glPopMatrix();
                }
                RenderEnd();
            }
        }

        SizeU GetGlyphSize(uint32_t code) {
            if (m_is_loaded) {
                const GlyphData& glyph_data = FindOrAddGlyphData(code);
                return {glyph_data.width, m_data.glyph_height};
            }
            return {0, 0};
        }

        bool IsOk() const {
            return m_err_msg.empty();
        }

        std::string GetErrMsg() const {
            return m_err_msg;
        }

        bool SaveAsBMP(const std::string& path) const {
            bool is_success = false;

            if (m_is_loaded) {
                std::string file_name_prefix = path;

                if (file_name_prefix.length() > 0 && file_name_prefix[file_name_prefix.length() - 1] != '/' && file_name_prefix[file_name_prefix.length() - 1] != '\\') {
                    file_name_prefix += "/";
                }

                file_name_prefix += m_data.info.name;

                for (size_t ix = 0; ix < m_data.tex_objs.size(); ++ix) {
                    std::string file_name = file_name_prefix;
                    if (ix > 0) file_name += " (" + std::to_string(ix + 1) + ")";
                    file_name += ".bmp";

                    is_success = SaveTextureAsBMP(file_name, m_data.tex_objs[ix]);
                    if (!is_success) break;
                }
            }

            return is_success;
        }

    private:
        TOGL_NO_COPY(Font);

        void Initialize() {
            m_data          = {};
            m_is_loaded     = false;
            m_err_msg       = "";
        }

        void AddErrMsg(const std::string& err_msg) {
            if (!m_err_msg.empty()) m_err_msg += "\n";
            m_err_msg += "Font Error: ";
            m_err_msg += err_msg;
        }

        void MergeErrMsg(const std::string& err_msg) {
            if (!m_err_msg.empty()) m_err_msg += "\n";
            m_err_msg += err_msg;
        }

        GlyphData* FindGlyphData(uint32_t code) {
            auto it = m_data.glyphs.find(code);

            if (it == m_data.glyphs.end()) it = m_data.glyphs.find(CODE_WHITE_SQUARE);
            if (it == m_data.glyphs.end()) it = m_data.glyphs.find(CODE_REPLACEMENT_CHARACTER);

            if (it != m_data.glyphs.end()) {
                return &(it->second);
            } 
            return nullptr;
        }

        GlyphData& FindOrAddGlyphData(uint32_t code) {
            GlyphData* glyph_data = FindGlyphData(code);
            if (glyph_data) {
                return *glyph_data;
            } else {
                GlyphData& new_glyph_data = m_data.glyphs[code];
                new_glyph_data.width = m_data.glyph_height;
                return new_glyph_data;
            }
        }

        FontData                m_data;
        bool                    m_is_loaded;
        std::string             m_err_msg;
    };

    //==========================================================================
    // Definitions
    //==========================================================================

}; // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_FONT_H_
