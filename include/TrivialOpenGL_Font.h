/**
* @file TrivialOpenGL_Font.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_FONT_H_
#define TRIVIALOPENGL_FONT_H_

#include "TrivialOpenGL_Window.h"

//==========================================================================
// Declarations
//==========================================================================

enum TOGL_FontSizeUnitId {
    TOGL_FONT_SIZE_UNIT_ID_PIXELS,
    TOGL_FONT_SIZE_UNIT_ID_POINTS,
};

enum TOGL_FontStyleId {
    TOGL_FONT_STYLE_ID_NORMAL,
    TOGL_FONT_STYLE_ID_BOLD,
};
    
// Ranges are from unicode space.
// Font might not have all glyphs from this ranges.
enum TOGL_FontCharSetId {
    TOGL_FONT_CHAR_SET_ID_CUSTOM,
    TOGL_FONT_CHAR_SET_ID_RANGE_0000_FFFF,

    TOGL_FONT_CHAR_SET_ID_ENGLISH,
};

enum {
    // "WHITE SQUARE" 
    // Represents missing glyph in unicode space.
    TOGL_UNICODE_WHITE_SQUARE               = 0x25A1,

    // "REPLACEMENT CHARACTER"
    // Represent out of range character code from unicode space.
    TOGL_UNICODE_REPLACEMENT_CHARACTER      = 0xFFFD,
};


//-----------------------------------------------------------------------------
// TOGL_UnicodeRange
//-----------------------------------------------------------------------------

struct TOGL_UnicodeRange {
    uint32_t from;
    uint32_t to;
        
    TOGL_UnicodeRange() : from(0), to(0) {}
    TOGL_UnicodeRange(uint32_t code) : from(code), to(code) {}
    TOGL_UnicodeRange(uint32_t from, uint32_t to) : from(from), to(to) {}
};

std::vector<TOGL_UnicodeRange> TOGL_GetUnicodeRanges(TOGL_FontCharSetId char_set);

//-----------------------------------------------------------------------------
// TOGL_UnicodeRangeGroup
//-----------------------------------------------------------------------------

class TOGL_UnicodeRangeGroup {
public:
    TOGL_UnicodeRangeGroup()                                                    : m_char_set(TOGL_FONT_CHAR_SET_ID_CUSTOM) {}
    TOGL_UnicodeRangeGroup(TOGL_FontCharSetId char_set)                         : m_char_set(char_set),  m_ranges(TOGL_GetUnicodeRanges(char_set)) {}
    template <uint32_t NUMBER>
    explicit TOGL_UnicodeRangeGroup(const TOGL_UnicodeRange (&ranges)[NUMBER])  : m_char_set(TOGL_FONT_CHAR_SET_ID_CUSTOM), m_ranges(ranges, ranges + NUMBER) {}

    virtual ~TOGL_UnicodeRangeGroup() {}

    TOGL_FontCharSetId GetCharSet() const;
    std::vector<TOGL_UnicodeRange> GetRanges() const;
    const std::vector<TOGL_UnicodeRange>& ToRanges() const;

private:
    TOGL_FontCharSetId              m_char_set;
    std::vector<TOGL_UnicodeRange>  m_ranges;

};

//-----------------------------------------------------------------------------
// TOGL_FontInfo
//-----------------------------------------------------------------------------

struct TOGL_FontInfo {
    std::string             name;                   // encoding format: UTF8
    uint32_t                size;
    TOGL_FontSizeUnitId     size_unit;
    TOGL_FontStyleId        style;
    TOGL_UnicodeRangeGroup  unicode_range_group;

    TOGL_FontInfo();
    TOGL_FontInfo(const std::string& name, uint32_t size, TOGL_FontSizeUnitId size_unit, TOGL_FontStyleId style, const TOGL_UnicodeRangeGroup& unicode_range_group);
};

//-----------------------------------------------------------------------------
// TOGL_GlyphData
//-----------------------------------------------------------------------------

struct TOGL_GlyphData {
    uint16_t    width;      // in pixels
    GLuint      tex_obj;    // opengl texture object (name)

    // texture coordinates
    double      x1;
    double      y1;
    double      x2;
    double      y2;

    TOGL_GlyphData();
};

//-----------------------------------------------------------------------------
// TOGL_FontData
//-----------------------------------------------------------------------------

struct TOGL_FontData {
    TOGL_FontInfo info;

    uint32_t glyph_height;               // in pixels
    uint32_t glyph_ascent;               // in pixels
    uint32_t glyph_descent;              // in pixels
    uint32_t glyph_internal_leading;     // in pixels

    std::map<uint32_t, TOGL_GlyphData> glyphs;     // indexed by character code from unicode space

    // Array of OpenGL Texture Object Identifiers (Texture Names).
    // Pixel Format: RGBA (8 bits per channel).
    // Orientation: First pixel refers to left-bottom corner of image.
    std::vector<GLuint> tex_objs;

    TOGL_FontData();
};

//-----------------------------------------------------------------------------
// TOGL_FontDataGenerator
//-----------------------------------------------------------------------------

class TOGL_FontDataGenerator {
public:
    TOGL_FontDataGenerator();
    virtual ~TOGL_FontDataGenerator();

    TOGL_FontData Generate(const TOGL_FontInfo& font_info);

    bool IsOk() const;
    std::string GetErrMsg() const;

private:
    struct DisplayListSet {
        TOGL_UnicodeRange    unicode_range;

        GLuint          first;
        GLsizei         range;
        GLuint          base;

        DisplayListSet();
        DisplayListSet(uint32_t code);
        DisplayListSet(uint32_t from, uint32_t to);
    };

    class FrameBuffer {
    public:
        FrameBuffer(uint16_t width, uint16_t height);
        virtual ~FrameBuffer();

        GLuint GenAndBindTex();

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

        void AddErrMsg(const std::string& err_msg);

        template <typename Type>
        void Load(Type& function, const std::string& function_name);

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

    void Initialize();

    void GenerateFontTextures(uint16_t width, uint16_t height);
    void RenderGlyphToTexture(GLuint list_base, int x, int y, wchar_t c);

    TOGL_SizeU16 GetCharSize(wchar_t c) const;

    void AddErrMsg(const std::string& err_msg);
    void MergErrMsg(const std::string& err_msg);

    static uint32_t PointsToPixels(uint32_t points) {
        return points * 4 / 3;
    }

    static uint32_t ToPixels(uint32_t size, TOGL_FontSizeUnitId size_unit);

    template <typename Type>
    void Load(Type& function, const std::string& function_name);

    TOGL_FontData               m_data;
    std::string                 m_err_msg;

    HDC                         m_device_context_handle;
    std::vector<DisplayListSet> m_display_list_sets;
};

//-----------------------------------------------------------------------------
// Global Font
//-----------------------------------------------------------------------------

class TOGL_Font;
TOGL_Font& TOGL_ToGlobalFont();

// If global font has been loaded successfully, then TOGL_IsFontOk() should return true. Otherwise, global font failed to load, and error message can be retrieved by TOGL_GetFontErrMsg().
void TOGL_LoadFont(const TOGL_FontInfo& font_info);
void TOGL_LoadFont(const std::string& name, uint32_t size, TOGL_FontSizeUnitId size_unit, TOGL_FontStyleId style, const TOGL_UnicodeRangeGroup& unicode_range_group);

void TOGL_UnloadFont();

bool TOGL_IsFontOk();
std::string TOGL_GetFontErrMsg();

//-----------------------------------------------------------------------------
// TOGL_Font
//-----------------------------------------------------------------------------
class TOGL_Font {
public:
    TOGL_Font();
    virtual ~TOGL_Font();

    // If font has been loaded successfully, then IsOk() should return true. Otherwise, font failed to load, and error message can be retrieved by GetErrMsg().
    void Load(const TOGL_FontInfo& font_info);
    void Load(const std::string& name, uint32_t size, TOGL_FontSizeUnitId size_unit, TOGL_FontStyleId style, const TOGL_UnicodeRangeGroup& unicode_range_group);

    void Unload();
    bool IsLoaded() const;

    // Warning!!! Each text render section of code which starts with RenderBegin() MUST end with RenderEnd().
    void RenderBegin();
    void RenderEnd();

    // Renders single glyph.
    // Can be used only in between RenderBegin() and RenderEnd().
    void RenderGlyph(uint32_t code);

    // Renders array of glyphs. 
    // Special characters (like '\n', '\t', ... and so on) are interpreted as "unknown characters".
    // Must be used in between RenderBegin() and RenderEnd().
    // text         - Encoding format: UTF8.
    void RenderGlyphs(const std::string& text);

    TOGL_SizeU GetGlyphSize(uint32_t code) const;

    // Returns glyph height in pixels.
    uint32_t GetGlyphHeight() const;

    // width        - In pixels.
    // Returns number of glyphs from text which will fit in width.
    uint32_t GetGlyphCountInWidth(const std::wstring& text, uint32_t width) const;

    bool IsOk() const;
    std::string GetErrMsg() const;

    bool SaveAsBMP(const std::string& path = "") const;

private:
    TOGL_NO_COPY(TOGL_Font);

    void Initialize();

    void AddErrMsg(const std::string& err_msg);
    void MergeErrMsg(const std::string& err_msg);

    const TOGL_GlyphData* FindGlyphData(uint32_t code) const;

    TOGL_FontData           m_data;
    bool                    m_is_loaded;
    std::string             m_err_msg;
};

inline TOGL_Font& TOGL_ToGlobalFont() {
    return TOGL_Global<TOGL_Font>::To();
}

inline void TOGL_LoadFont(const TOGL_FontInfo& font_info) {
    TOGL_ToGlobalFont().Load(font_info);
}

inline void TOGL_LoadFont(const std::string& name, uint32_t size, TOGL_FontSizeUnitId size_unit, TOGL_FontStyleId style, const TOGL_UnicodeRangeGroup& unicode_range_group) {
    TOGL_ToGlobalFont().Load(name, size, size_unit, style, unicode_range_group);
}

inline void TOGL_UnloadFont() {
    TOGL_ToGlobalFont().Unload();
}

inline bool TOGL_IsFontOk() {
    TOGL_ToGlobalFont().IsOk();
}

inline std::string TOGL_GetFontErrMsg() {
    TOGL_ToGlobalFont().GetErrMsg();
}

//==========================================================================
// Definitions
//==========================================================================

//-----------------------------------------------------------------------------
// TOGL_UnicodeRange
//-----------------------------------------------------------------------------

inline std::vector<TOGL_UnicodeRange> TOGL_GetUnicodeRanges(TOGL_FontCharSetId char_set) {
    switch (char_set) {
    case TOGL_FONT_CHAR_SET_ID_CUSTOM:
        return {};

    case TOGL_FONT_CHAR_SET_ID_RANGE_0000_FFFF:
        return {
            {0x0000, 0xFFFF}
        };

    case TOGL_FONT_CHAR_SET_ID_ENGLISH:
        return {
            {0x0020, 0x007E},
            {TOGL_UNICODE_WHITE_SQUARE},
            {TOGL_UNICODE_REPLACEMENT_CHARACTER}
        };

    } // switch

    return {};
}

//-----------------------------------------------------------------------------
// TOGL_UnicodeRangeGroup
//-----------------------------------------------------------------------------

inline TOGL_FontCharSetId TOGL_UnicodeRangeGroup::GetCharSet() const {
    return m_char_set;
}

inline std::vector<TOGL_UnicodeRange> TOGL_UnicodeRangeGroup::GetRanges() const {
    return m_ranges;
}

inline const std::vector<TOGL_UnicodeRange>& TOGL_UnicodeRangeGroup::ToRanges() const {
    return m_ranges;
}

//-----------------------------------------------------------------------------
// TOGL_FontInfo
//-----------------------------------------------------------------------------

inline TOGL_FontInfo::TOGL_FontInfo() {
    name                = "";
    size                = 0;
    size_unit           = TOGL_FONT_SIZE_UNIT_ID_PIXELS;
    style               = TOGL_FONT_STYLE_ID_NORMAL;
    unicode_range_group = TOGL_FONT_CHAR_SET_ID_ENGLISH;
}
        
inline TOGL_FontInfo::TOGL_FontInfo(const std::string& name, uint32_t size, TOGL_FontSizeUnitId size_unit, TOGL_FontStyleId style, const TOGL_UnicodeRangeGroup& unicode_range_group) {
    this->name                  = name;
    this->size                  = size;    
    this->size_unit             = size_unit;
    this->style                 = style;    
    this->unicode_range_group   = unicode_range_group;
}

//-----------------------------------------------------------------------------
// TOGL_GlyphData
//-----------------------------------------------------------------------------

inline TOGL_GlyphData::TOGL_GlyphData() {
    width       = 0;
    tex_obj     = 0;
                
    x1          = 0;
    y1          = 0;
    x2          = 0;
    y2          = 0;
}

//-----------------------------------------------------------------------------
// TOGL_FontData
//-----------------------------------------------------------------------------

inline TOGL_FontData::TOGL_FontData() {
    info                    = {};

    glyph_height            = 0;           
    glyph_ascent            = 0;           
    glyph_descent           = 0;          
    glyph_internal_leading  = 0; 
}

//-----------------------------------------------------------------------------
// TOGL_FontDataGenerator
//-----------------------------------------------------------------------------

inline TOGL_FontDataGenerator::TOGL_FontDataGenerator() {
    m_device_context_handle = NULL;
}

inline TOGL_FontDataGenerator::~TOGL_FontDataGenerator() {

}

inline TOGL_FontData TOGL_FontDataGenerator::Generate(const TOGL_FontInfo& font_info) {
    m_data.info     = font_info;
    m_err_msg       = "";

    _TOGL_WindowInnerAccessor& window_inner_accessor = TOGL_ToWindow();
    HWND window_handle = window_inner_accessor.ToHWND();

    m_device_context_handle = GetDC(window_handle);

    m_data.glyph_height = ToPixels(font_info.size, font_info.size_unit);

    HFONT font_handle = CreateFontW(
            m_data.glyph_height,                    
            0, 0, 0,                            
            (font_info.style == TOGL_FONT_STYLE_ID_BOLD) ? FW_BOLD : FW_NORMAL,
            FALSE, FALSE, FALSE,
            ANSI_CHARSET, // For W version of this function, should create a font bitmap with all having glyphs for unicode range from 0000 to FFFF.
            OUT_TT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            ANTIALIASED_QUALITY,
            FF_DONTCARE | DEFAULT_PITCH,
            TOGL_ToUTF16(font_info.name).c_str());  

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

        if (false && glyph_height != m_data.glyph_height) {
            AddErrMsg(std::string() + "Mismatch between font height requested (" + std::to_string(glyph_height) + "px) and created (" + std::to_string(glyph_height) + "px).");
        } else {
            auto GetUnicodeRangesOfCurrentWinFont = [](HDC device_context_handle) {
                std::vector<TOGL_UnicodeRange> unicode_ranges;

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
                    const uint32_t to   = from + glyphset->ranges[ix].cGlyphs - 1;

                    unicode_ranges.push_back({from, to});
                }

                delete[] buffer;

                return unicode_ranges;
            };

            // debug
            //for (const auto& range : GetUnicodeRangesOfCurrentWinFont(m_device_context_handle)) {
            //    LogDebug(std::string() + "[" + HexToStr(range.from) + ".." + HexToStr(range.to) + "]");
            //}

            // --- Generates Display Lists and Intermediary Font Bitmaps --- //

            std::vector<TOGL_UnicodeRange> ranges;
            for (TOGL_UnicodeRange range : m_data.info.unicode_range_group.ToRanges()) {
                if (range.from > 0xFFFF || range.to > 0xFFFF) {
                        AddErrMsg("Unicode code point is out of supported range. Supported unicode code point range is from 0000 to (including) FFFF. ");
                        break;
                }

                auto PushRangeWithoutCodePoint = [](uint32_t code_point, const TOGL_UnicodeRange& range, std::vector<TOGL_UnicodeRange>& ranges) {
                    auto IsIn = [](uint32_t code, const TOGL_UnicodeRange& range) -> bool {
                        return range.from <= code && code <= range.to;
                    };

                    if (IsIn(code_point, range)) {
                        if (range.from != code_point) {
                            ranges.push_back({range.from, code_point - 1});
                        }
                        if (range.to != code_point) {
                            ranges.push_back({code_point + 1, range.to});
                        }
                    } else {
                        ranges.push_back(range);
                    }
                };

                // Function wglUseFontBitmapsW have problem with generating font bitmap when FFFF code point is present in range.
                // Workaround: Remove this code point from range, since it's a non-character code point. 
                PushRangeWithoutCodePoint(0xFFFF, range, ranges);
            }

            if (IsOk()) {
                for (const auto& range : ranges) {
                    //LogDebug(std::string() + "[" + HexToStr(range.from) + ".." + HexToStr(range.to) + "]"); // debug

                    DisplayListSet display_list_set(range.from, range.to);

                    display_list_set.first  = display_list_set.unicode_range.from;
                    display_list_set.range  = GLsizei(display_list_set.unicode_range.to - display_list_set.unicode_range.from + 1);
                    display_list_set.base   = glGenLists(display_list_set.range);

                    if (display_list_set.base == 0) {
                        AddErrMsg(std::string() + "Can not generate display list for unicode range [" + TOGL_HexToStr(display_list_set.unicode_range.from) + ".." + TOGL_HexToStr(display_list_set.unicode_range.to) + "].");
                        break;
                    }

                    bool is_success = wglUseFontBitmapsW(m_device_context_handle, display_list_set.first, display_list_set.range, display_list_set.base);

                    // Workaround for strange behavior. For POPUP window first call of wglUseFontBitmapsA fail with GetError() = 0.
                    // Second call, right after first, seams to succeed.
                    if (!is_success) is_success = wglUseFontBitmapsW(m_device_context_handle, display_list_set.first, display_list_set.range, display_list_set.base);

                    if (!is_success) {
                        AddErrMsg("Error: Can not create intermediary font bitmap for unicode range [" + TOGL_HexToStr(display_list_set.unicode_range.from) + ".." + TOGL_HexToStr(display_list_set.unicode_range.to) + "].");
                        break;
                    }

                    m_display_list_sets.push_back(display_list_set);
                }

                // --- Generate Font Textures --- //

                if (IsOk()) {
                    GenerateFontTextures(1024, 1024);
                }

                // --- Destroys Display Lists and Clears Ranges --- //

                for (auto& display_list_set : m_display_list_sets) {
                    glDeleteLists(display_list_set.base, display_list_set.range);
                }
                m_display_list_sets.clear();
            }
        }
        SelectObject(m_device_context_handle, old_font_handle);
        DeleteObject(font_handle);
    }

    ReleaseDC(window_handle, m_device_context_handle);
    m_device_context_handle = NULL;

    TOGL_FontData data = m_data;

    m_data = {};

    return IsOk() ? data : TOGL_FontData();
}

inline bool TOGL_FontDataGenerator::IsOk() const {
    return m_err_msg.empty();
}

inline std::string TOGL_FontDataGenerator::GetErrMsg() const {
    return m_err_msg;
}

//-----------------------------------------------------------------------------

inline TOGL_FontDataGenerator::DisplayListSet::DisplayListSet() {
    first      = 0;
    range      = 0;
    base       = 0; 
}

inline TOGL_FontDataGenerator::DisplayListSet::DisplayListSet(uint32_t code) {
    unicode_range = TOGL_UnicodeRange(code);

    first      = 0;
    range      = 0;
    base       = 0; 
}

inline TOGL_FontDataGenerator::DisplayListSet::DisplayListSet(uint32_t from, uint32_t to) {
    unicode_range = TOGL_UnicodeRange(from, to);

    first      = 0;
    range      = 0;
    base       = 0; 
}

//-----------------------------------------------------------------------------

inline TOGL_FontDataGenerator::FrameBuffer::FrameBuffer(uint16_t width, uint16_t height) {
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

inline TOGL_FontDataGenerator::FrameBuffer::~FrameBuffer() {
    if (IsOk()) m_glFramebufferTexture2DEXT(TOGL_GL_FRAMEBUFFER_EXT, TOGL_GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 0, 0);

    glPopAttrib();
    glPopAttrib();

    if (IsOk()) {
        m_glBindFramebufferEXT(TOGL_GL_FRAMEBUFFER_EXT, m_prev_fbo);
        m_glDeleteFramebuffersEXT(1, &m_fbo);
    }
}

inline GLuint TOGL_FontDataGenerator::FrameBuffer::GenAndBindTex() {
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

inline void TOGL_FontDataGenerator::FrameBuffer::AddErrMsg(const std::string& err_msg) {
    if (!m_err_msg.empty()) m_err_msg += "\n";
    m_err_msg += err_msg;
}

template <typename Type>
inline void TOGL_FontDataGenerator::FrameBuffer::Load(Type& function, const std::string& function_name) {
    function = (Type)wglGetProcAddress(function_name.c_str());
    if (!function) {
        AddErrMsg(std::string() + "Can not load function: \"" + function_name + "\".");
    }
}

//-----------------------------------------------------------------------------

inline void TOGL_FontDataGenerator::Initialize() {
    m_device_context_handle = NULL;

    m_data = {};
}

inline void TOGL_FontDataGenerator::GenerateFontTextures(uint16_t width, uint16_t height) {
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
        TOGL_PointI pos = {0, y};
 
        for (const DisplayListSet& display_list_set : m_display_list_sets) {
            for (uint32_t code = display_list_set.unicode_range.from; code <= display_list_set.unicode_range.to; ++code) {

                const TOGL_SizeU16 size = GetCharSize((wchar_t)code);

                if ((pos.x + size.width) >= width) {
                    pos.x = 0;

                    if ((pos.y - m_data.glyph_height) <= 0) {
                        // run out of space in texture, generate next texture
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

                TOGL_GlyphData glyph_data = {};

                glyph_data.width = size.width;
                glyph_data.tex_obj = tex_obj;

                glyph_data.x1 = ToTexSpace(pos.x, width);
                glyph_data.y1 = ToTexSpace(pos.y - m_data.glyph_descent, height);
                glyph_data.x2 = ToTexSpace(pos.x + size.width, width);
                glyph_data.y2 = ToTexSpace(pos.y - m_data.glyph_descent + m_data.glyph_height, height);

                m_data.glyphs[code] = glyph_data;
              
                RenderGlyphToTexture(
                    display_list_set.base, 
                    pos.x, pos.y, 
                    (wchar_t)(code - display_list_set.first) // corrects character code to match glyph index in display list
                );
                    
                // Used as workaround for overlapping glyphs when they are drawn.
                enum { ADDITIONAL_SAFE_SPACE = 1 };

                pos.x += size.width + ADDITIONAL_SAFE_SPACE;
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

inline void TOGL_FontDataGenerator::RenderGlyphToTexture(GLuint list_base, int x, int y, wchar_t c) {
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

inline TOGL_SizeU16 TOGL_FontDataGenerator::GetCharSize(wchar_t c) const {
    SIZE size;
    if (GetTextExtentPoint32W(m_device_context_handle, &c, 1, &size)) {
        return TOGL_SizeU16(size.cx, size.cy);
    }
    return {};
}

inline void TOGL_FontDataGenerator::AddErrMsg(const std::string& err_msg) {
    if (!m_err_msg.empty()) m_err_msg += "\n";
    m_err_msg += err_msg;
}

inline void TOGL_FontDataGenerator::MergErrMsg(const std::string& err_msg) {
    if (!m_err_msg.empty()) m_err_msg += "\n";
    m_err_msg += err_msg;
}

inline uint32_t TOGL_FontDataGenerator::ToPixels(uint32_t size, TOGL_FontSizeUnitId size_unit) {
    switch (size_unit) {
    case TOGL_FONT_SIZE_UNIT_ID_PIXELS: return size;
    case TOGL_FONT_SIZE_UNIT_ID_POINTS: return PointsToPixels(size);
    }
    return 0;
}

template <typename Type>
inline void TOGL_FontDataGenerator::Load(Type& function, const std::string& function_name) {
    function = (Type)wglGetProcAddress(function_name.c_str());
    if (!function) {
        AddErrMsg(std::string() + "Can not load function: \"" + function_name + "\".");
    }
}

//-----------------------------------------------------------------------------
// TOGL_Font
//-----------------------------------------------------------------------------

inline TOGL_Font::TOGL_Font() {
    Initialize();
}
inline TOGL_Font::~TOGL_Font() {
    Unload();
}

inline void TOGL_Font::Load(const TOGL_FontInfo& font_info) {
    Unload();

    if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_DEBUG)) {
        TOGL_LogDebug("Font Unicode Ranges:");
        for (const auto& range : font_info.unicode_range_group.ToRanges()) {
            if (range.from == range.to) { 
                TOGL_LogDebug(std::string() + "[" + TOGL_HexToStr(range.from) + "]");
            } else {
                TOGL_LogDebug(std::string() + "[" + TOGL_HexToStr(range.from) + ".." + TOGL_HexToStr(range.to) + "]");
            }
        }
    }

    TOGL_FontDataGenerator font_data_generator;

    if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_DEBUG)) {
        TOGL_LogDebug("Generating font textures...");
    }

    m_data = font_data_generator.Generate(font_info);
    if (font_data_generator.IsOk()) {

        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_DEBUG)) {
            TOGL_LogDebug("Font textures has been generated.");
        }

        m_is_loaded = true;
    } else {
        m_data = {};
        MergeErrMsg(font_data_generator.GetErrMsg());
    }
} 

inline void TOGL_Font::Load(const std::string& name, uint32_t size, TOGL_FontSizeUnitId size_unit, TOGL_FontStyleId style, const TOGL_UnicodeRangeGroup& unicode_range_group) {
    Load(TOGL_FontInfo(name, size, size_unit, style, unicode_range_group));
} 

inline void TOGL_Font::Unload() {
    for (auto& tex_obj : m_data.tex_objs) {
        glDeleteTextures(1, &tex_obj);
    }
    Initialize();
}

inline bool TOGL_Font::IsLoaded() const {
    return m_is_loaded;
}

inline void TOGL_Font::RenderBegin() {
    glPushAttrib(GL_TEXTURE_BIT);
    glPushAttrib(GL_ENABLE_BIT);
    glPushAttrib(GL_COLOR_BUFFER_BIT);
    glPushAttrib(GL_LIST_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
}

inline void TOGL_Font::RenderEnd() {
    glPopAttrib();
    glPopAttrib();
    glPopAttrib();
    glPopAttrib();
}

inline void TOGL_Font::RenderGlyph(uint32_t code) {
    if (m_is_loaded) {
        const TOGL_GlyphData* glyph_data = FindGlyphData(code);

        if (glyph_data->tex_obj != 0) {
            glBindTexture(GL_TEXTURE_2D, glyph_data->tex_obj);
            glEnable(GL_TEXTURE_2D);

            glBegin(GL_TRIANGLE_FAN);
            glTexCoord2d(glyph_data->x1, glyph_data->y1);
            glVertex2i(0, 0);

            glTexCoord2d(glyph_data->x2, glyph_data->y1);
            glVertex2i(glyph_data->width, 0);

            glTexCoord2d(glyph_data->x2, glyph_data->y2);
            glVertex2i(glyph_data->width, m_data.glyph_height);
                
            glTexCoord2d(glyph_data->x1, glyph_data->y2);
            glVertex2i(0, m_data.glyph_height);
            glEnd();
        } else {
            // Renders replacement for missing glyph.
            glDisable(GL_TEXTURE_2D);

            glBegin(GL_TRIANGLE_FAN);
            glVertex2i(0, 0);
            glVertex2i(m_data.glyph_height, 0);
            glVertex2i(m_data.glyph_height, m_data.glyph_height);
            glVertex2i(0, m_data.glyph_height);
            glEnd();
        }
    }
} 

inline void TOGL_Font::RenderGlyphs(const std::string& text) {
    if (m_is_loaded) {
        const std::wstring text_utf16 = TOGL_ToUTF16(text);

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

inline TOGL_SizeU TOGL_Font::GetGlyphSize(uint32_t code) const {
    TOGL_SizeU size = {0, 0};
    if (m_is_loaded) {
        const TOGL_GlyphData* glyph_data = FindGlyphData(code);
        if (glyph_data) {
            size = {glyph_data->width, m_data.glyph_height};
        } else {
            size = {m_data.glyph_height, m_data.glyph_height};
        }
    }
    return size;
}

inline uint32_t TOGL_Font::GetGlyphHeight() const {
    return m_data.glyph_height;
}

inline uint32_t TOGL_Font::GetGlyphCountInWidth(const std::wstring& text, uint32_t width) const {
    uint32_t    count           = 0;
    uint32_t    current_width   = 0;
    for (const wchar_t c : text) {
        current_width += GetGlyphSize(c).width;
        if (current_width > width) break;
        count += 1;
    }
    return count;
}

inline bool TOGL_Font::IsOk() const {
    return m_err_msg.empty();
}

inline std::string TOGL_Font::GetErrMsg() const {
    return m_err_msg;
}

inline bool TOGL_Font::SaveAsBMP(const std::string& path) const {
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

            is_success = TOGL_SaveTextureAsBMP(file_name, m_data.tex_objs[ix]);
            if (!is_success) break;
        }
    }

    return is_success;
}

inline void TOGL_Font::Initialize() {
    m_data          = {};
    m_is_loaded     = false;
    m_err_msg       = "";
}

inline void TOGL_Font::AddErrMsg(const std::string& err_msg) {
    if (!m_err_msg.empty()) m_err_msg += "\n";
    m_err_msg += "Font Error: ";
    m_err_msg += err_msg;
}

inline void TOGL_Font::MergeErrMsg(const std::string& err_msg) {
    if (!m_err_msg.empty()) m_err_msg += "\n";
    m_err_msg += err_msg;
}

inline const TOGL_GlyphData* TOGL_Font::FindGlyphData(uint32_t code) const {
    auto it = m_data.glyphs.find(code);

    if (it == m_data.glyphs.end()) it = m_data.glyphs.find(TOGL_UNICODE_WHITE_SQUARE);
    if (it == m_data.glyphs.end()) it = m_data.glyphs.find(TOGL_UNICODE_REPLACEMENT_CHARACTER);

    if (it != m_data.glyphs.end()) {
        return &(it->second);
    } 
    return nullptr;
}

#endif // TRIVIALOPENGL_FONT_H_
