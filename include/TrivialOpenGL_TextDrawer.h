/**
* @file TrivialOpenGL_TextDrawer.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_TEXTDRAWER_H_
#define TRIVIALOPENGL_TEXTDRAWER_H_

#include "TrivialOpenGL_Font.h"
#include "TrivialOpenGL_FineText.h"

//==========================================================================
// Declarations
//==========================================================================

//------------------------------------------------------------------------------
// Global TextAdjuster
//------------------------------------------------------------------------------

class TOGL_TextAdjuster;
TOGL_TextAdjuster& TOGL_ToGlobalTextAdjuster();

void TOGL_ResetTextAdjuster();

// Sets line width in pixels after which words are wrapped.
// width        - In pixels.
void TOGL_SetLineWrapWidth(uint32_t width);

void TOGL_SetNumberOfSpacesInTab(uint32_t number);

// Adjust text by using word wrapping and replaces tabs with equivalent in length (in pixels) in spaces.
TOGL_FineText TOGL_AdjustText(const TOGL_FineText& text);

//------------------------------------------------------------------------------
// TOGL_TextAdjuster
//------------------------------------------------------------------------------

class TOGL_TextAdjuster {
public:
    TOGL_TextAdjuster();
    virtual ~TOGL_TextAdjuster();

    void Reset();

    // Sets line width in pixels after which words are wrapped.
    // width        - In pixels.
    void SetLineWrapWidth(uint32_t width);
    void SetNumberOfSpacesInTab(uint32_t number);

    // Adjust text by using word wrapping and replaces tabs with equivalent in length (in pixels) in spaces.
    TOGL_FineText AdjustText(const TOGL_Font& font, const  TOGL_FineText& text) const;

private:
    uint32_t            m_num_of_spaces_in_tab;

    uint32_t            m_wrap_line_width;          // in pixels

    uint32_t            m_width_of_full_tab;        // in pixels
    uint32_t            m_line_width;               // in pixels

    // <sentence_part>
    //     '\t'         # tab
    //     '\n'         # new line
    //     ' '          # space
    //     '[^\t\n ]'   # word (any array of characters which doesn't contain tab, new line or space
    static size_t GetSentencePartPos(const std::wstring& sentence, size_t current_pos);

    TOGL_SizeU GetSentenceSize(const TOGL_Font& font, const std::wstring& sentence) const;
    uint32_t GetSentenceWidth(const TOGL_Font& font, const std::wstring& sentence) const;

    static std::vector<std::wstring> SplitSentenceToParts(const std::wstring& sentence);

    TOGL_FineText PrepareTextElementHorizontalSpacer(const TOGL_Font & font, uint32_t text_horizontal_space_width, uint32_t & line_width) const;
    TOGL_FineText PrepareTextElementText(const TOGL_Font& font, const std::wstring& text, uint32_t& line_width) const;
};

//------------------------------------------------------------------------------
// Global TextDrawer
//------------------------------------------------------------------------------

class TOGL_TextDrawer;
TOGL_TextDrawer& TOGL_ToGlobalTextDrawer();

void TOGL_ResetTextDrawer();

// text - Encoding format: UTF8.

void TOGL_RenderText(const TOGL_PointI& pos, const TOGL_Color4U8& color, const std::string& text);
void TOGL_RenderText(const TOGL_PointI& pos, const TOGL_Color4U8& color, const TOGL_FineText& fine_text);

void TOGL_AdjustAndRenderText(const TOGL_PointI& pos, const TOGL_Color4U8& color, const std::string& text);
void TOGL_AdjustAndRenderText(const TOGL_PointI& pos, const TOGL_Color4U8& color, const TOGL_FineText& fine_text);

TOGL_SizeU TOGL_GetTextSize(const std::string& text);
TOGL_SizeU TOGL_GetTextSize(const TOGL_FineText& fine_text);

// Adjusts with global text adjuster, accessed by TOGL_ToGlobalTextAdjuster().
TOGL_SizeU TOGL_AdjustAndGetTextSize(const std::string& text);
TOGL_SizeU TOGL_AdjustAndGetTextSize(const TOGL_FineText& fine_text);

// Sets coordinates system origin for drawing text.
void TOGL_SetOrigin(TOGL_OriginId origin_id);

TOGL_OriginId TOGL_GetOrigin();

//------------------------------------------------------------------------------
// TOGL_TextDrawer
//------------------------------------------------------------------------------

class TOGL_TextDrawer {
public:
    TOGL_TextDrawer();
    virtual ~TOGL_TextDrawer();

    // ---

    void Reset();

    // Sets start position of text.
    void SetPos(int x, int y);
    void SetPos(const TOGL_PointI& pos);
    
    // Sets color of text.
    void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void SetColor(const TOGL_Color4U8& color);

    // ---

    // Renders text by using provided font.
    void RenderText(TOGL_Font& font, const std::string& text);
    void RenderText(TOGL_Font& font, const TOGL_FineText& fine_text);

    // text             - Encoding format: UTF8.
    TOGL_SizeU GetTextSize(TOGL_Font& font, const std::string& text) const;
    TOGL_SizeU GetTextSize(TOGL_Font& font, const  TOGL_FineText& fine_text) const;

    // ---

    // Sets origin of coordinate system to be in specific place in window area rectangle.
    void SetOrigin(TOGL_OriginId orientation);
    TOGL_OriginId GetOrigin() const;

private:
    static void ReplaceAll(std::string& text, const std::string& from, const std::string& to);

    TOGL_OriginId                   m_origin_id;
    uint32_t                        m_orientation_factor_y;

    TOGL_PointI                     m_pos;
    TOGL_PointI                     m_base;

    TOGL_Color4U8                   m_color;

    TOGL_FineText                   m_text;
};

//==========================================================================
// Definitions
//==========================================================================

//------------------------------------------------------------------------------
// Global TextAdjuster
//------------------------------------------------------------------------------

inline TOGL_TextAdjuster& TOGL_ToGlobalTextAdjuster() {
    return TOGL_Global<TOGL_TextAdjuster>::ToObject();
}

inline void TOGL_ResetTextAdjuster() {
    TOGL_ToGlobalTextAdjuster().Reset();
}

// width        - In pixels.
inline void TOGL_SetLineWrapWidth(uint32_t width) {
    TOGL_ToGlobalTextAdjuster().SetLineWrapWidth(width);
}

inline void TOGL_SetNumberOfSpacesInTab(uint32_t number) {
    TOGL_ToGlobalTextAdjuster().SetNumberOfSpacesInTab(number);
}

inline TOGL_FineText TOGL_AdjustText(const TOGL_FineText& text) {
    return TOGL_ToGlobalTextAdjuster().AdjustText(TOGL_ToGlobalFont(), text);
}

//------------------------------------------------------------------------------
// TOGL_TextAdjuster
//------------------------------------------------------------------------------

inline TOGL_TextAdjuster::TOGL_TextAdjuster() {            
    Reset();

    m_width_of_full_tab = 0;        
    m_line_width        = 0;   
}

inline TOGL_TextAdjuster::~TOGL_TextAdjuster() {

}

inline void TOGL_TextAdjuster::Reset() {
    m_wrap_line_width = 0;
    SetNumberOfSpacesInTab(4);
}

inline void TOGL_TextAdjuster::SetLineWrapWidth(uint32_t width) {
    m_wrap_line_width = width;
}

inline void TOGL_TextAdjuster::SetNumberOfSpacesInTab(uint32_t number) {
    m_num_of_spaces_in_tab = number;
}

inline TOGL_FineText TOGL_TextAdjuster::AdjustText(const TOGL_Font& font, const TOGL_FineText& fine_text) const {
    TOGL_FineText adjusted_fine_text;

    if (font.IsLoaded()) {
        uint32_t line_width = 0; // in pixels

        for (const TOGL_FineTextElementContainer& element_container : fine_text.ToElementContainers()) {
            switch (element_container.GetTypeId()) {
            case TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT:
                adjusted_fine_text += PrepareTextElementText(font, element_container.GetText(), line_width);
                break;
            case TOGL_FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER:
                adjusted_fine_text += PrepareTextElementHorizontalSpacer(font, element_container.GetTextHorizontalSpaceWidth(), line_width);
                break;
            case TOGL_FINE_TEXT_ELEMENT_TYPE_ID_COLOR:
                adjusted_fine_text.Append(element_container);
                break;
            } // switch
            
        }
    } else {
        adjusted_fine_text = fine_text;
    }

    return adjusted_fine_text;
}

//------------------------------------------------------------------------------

inline TOGL_SizeU TOGL_TextAdjuster::GetSentenceSize(const TOGL_Font& font, const std::wstring& sentence) const {
    return {GetSentenceWidth(font, sentence), font.GetHeight()};
}

inline uint32_t TOGL_TextAdjuster::GetSentenceWidth(const TOGL_Font& font, const std::wstring& sentence) const {
    uint32_t width = 0;

    bool is_glyph_before = false;

    for (const wchar_t c : sentence) {
        if (c == L'\t') {
            if (is_glyph_before) width += font.GetDistanceBetweenGlyphs();

            width += font.GetGlyphSize(L' ').width * m_num_of_spaces_in_tab;
            if (m_num_of_spaces_in_tab > 1) width += (font.GetDistanceBetweenGlyphs() * (m_num_of_spaces_in_tab - 1));

            is_glyph_before = true;
        } else if (c == L'\n') {
            is_glyph_before = false;
        } else {
            if (is_glyph_before) width += font.GetDistanceBetweenGlyphs();

            width += font.GetGlyphSize(c).width;
            is_glyph_before = true;
        }
    }

    return width;
}

inline size_t TOGL_TextAdjuster::GetSentencePartPos(const std::wstring& sentence, size_t current_pos) {
    auto IsWhiteSpace = [](wchar_t c) -> bool {
        return c == L' ' || c == L'\t' || c == L'\n';
    };

    for (size_t next_sentence_part_pos = current_pos; next_sentence_part_pos < sentence.size(); ++next_sentence_part_pos) {
        const wchar_t c = sentence[next_sentence_part_pos];

        if (IsWhiteSpace(sentence[next_sentence_part_pos])) return next_sentence_part_pos + 1;
        if (next_sentence_part_pos + 1 < sentence.size() && IsWhiteSpace(sentence[next_sentence_part_pos + 1])) return next_sentence_part_pos + 1;
    }
    return std::wstring::npos;
};

inline std::vector<std::wstring> TOGL_TextAdjuster::SplitSentenceToParts(const std::wstring& sentence) {
    std::vector<std::wstring> parts;

    size_t pos = 0;
    while (pos < sentence.size()) {
        size_t next_part_pos = GetSentencePartPos(sentence, pos);

        const std::wstring part = sentence.substr(pos, next_part_pos != std::wstring::npos ? (next_part_pos - pos) : std::wstring::npos);

        parts.push_back(part);
        pos = next_part_pos;
    }

    return parts;
};

inline TOGL_FineText TOGL_TextAdjuster::PrepareTextElementHorizontalSpacer(const TOGL_Font & font, uint32_t text_horizontal_space_width, uint32_t & line_width) const {
    TOGL_FineText prepared_fine_text;

    if (text_horizontal_space_width + line_width > m_wrap_line_width) {
        prepared_fine_text.Append(TOGL_FineTextElementContainer(L"\n"));
        line_width = 0;
    }
    prepared_fine_text.Append(text_horizontal_space_width);
    line_width += text_horizontal_space_width;

    return prepared_fine_text;
}

inline TOGL_FineText TOGL_TextAdjuster::PrepareTextElementText(const TOGL_Font& font, const std::wstring& text, uint32_t& line_width) const {
    TOGL_FineText prepared_fine_text;
    std::wstring prepared_text;

    const std::vector<std::wstring> sentence_parts = SplitSentenceToParts(text);

    bool is_glyph_before = false;

    for (const auto& part : sentence_parts) {
        // Any first glyph in line don't have spacing. Only following ones.
        const uint32_t part_width = GetSentenceWidth(font, part) + (is_glyph_before ? font.GetDistanceBetweenGlyphs() : 0);

        if (part == L"\n") {
            // New line.
            prepared_text   += part;
            line_width      = 0;

            is_glyph_before = false;
        } else if (part == L"\t") {
            // Tab.
            const uint32_t width_of_full_tab = part_width;

            uint32_t width_of_tab = width_of_full_tab - (line_width % width_of_full_tab);
            if (width_of_tab == 0) width_of_tab = width_of_full_tab;
                        
            if (m_wrap_line_width != 0 && (line_width + width_of_full_tab) > m_wrap_line_width) {
                prepared_text   += L"\n";
                line_width      = 0;

                width_of_tab = width_of_full_tab;
            } 

            // To preserve same length of tabs, horizontal spacer is used for custom pixel perfect lengths.
            prepared_fine_text.Append(prepared_text, width_of_tab);
            prepared_text = L"";

            line_width += width_of_tab;

            is_glyph_before = true;
        } else {
            // Word or Space.
            if (m_wrap_line_width != 0 && (line_width + part_width) > m_wrap_line_width) {
                // Word or Space is crossing wrap line width. Needs to be moved or split.

                if (part == L" ") {
                    // Spaces are ignored if they are behind wrap line width.
                    // Current value of is_glyph_before is preserved. 
                } else {
                    // Word.
                    if (part_width > m_wrap_line_width) {
                        // Word is longer than line. Must be split between two or multiple lines.
                        std::wstring    long_part       = part;
                        uint32_t        long_part_width = part_width;

                        while (long_part_width > m_wrap_line_width) {
                            const uint32_t line_width_left = m_wrap_line_width - line_width;

                            const size_t glyph_count = font.GetGlyphCountInWidth(long_part, line_width_left);

                            prepared_text   += long_part.substr(0, glyph_count);
                            prepared_text   += L"\n";
                            line_width      = 0;

                            long_part       = long_part.substr(glyph_count);
                            long_part_width = GetSentenceWidth(font, long_part);
                        }

                        prepared_text   += long_part;
                        line_width      += long_part_width;
                    } else {
                        // Word is shorter than line. Whole word is moved to next line.
                        prepared_text   += L"\n";
                        line_width      = 0;

                        prepared_text   += part;
                        line_width      += part_width;
                    }
                    is_glyph_before = true;
                }
            } else {
                // Entire Word or Space fits in line.
                prepared_text   += part;
                line_width      += part_width;

                is_glyph_before = true;
            }
        }
    }
    prepared_fine_text.Append(prepared_text);
    return prepared_fine_text;
}

//------------------------------------------------------------------------------
// Global TextDrawer
//------------------------------------------------------------------------------

inline TOGL_TextDrawer& TOGL_ToGlobalTextDrawer() {
    return TOGL_Global<TOGL_TextDrawer>::ToObject();
}

inline void TOGL_ResetTextDrawer() {
    return TOGL_ToGlobalTextDrawer().Reset();
}

inline void TOGL_RenderText(const TOGL_PointI& pos, const TOGL_Color4U8& color, const std::string& text) {
    TOGL_RenderText(pos, color, TOGL_FineText(text));
}

inline void TOGL_RenderText(const TOGL_PointI& pos, const TOGL_Color4U8& color, const TOGL_FineText& fine_text) {
    TOGL_TextDrawer& drawer = TOGL_ToGlobalTextDrawer();
    drawer.SetPos(pos);
    drawer.SetColor(color);
    drawer.RenderText(TOGL_ToGlobalFont(), fine_text);
}

inline void TOGL_AdjustAndRenderText(const TOGL_PointI& pos, const TOGL_Color4U8& color, const std::string& text) {
    TOGL_AdjustAndRenderText(pos, color, TOGL_FineText(text));
}

inline void TOGL_AdjustAndRenderText(const TOGL_PointI& pos, const TOGL_Color4U8& color, const TOGL_FineText& fine_text) {
    TOGL_Font&          font            = TOGL_ToGlobalFont();
    TOGL_TextAdjuster&  text_adjuster   = TOGL_ToGlobalTextAdjuster();
    TOGL_TextDrawer&    drawer          = TOGL_ToGlobalTextDrawer();

    drawer.SetPos(pos);
    drawer.SetColor(color);
    drawer.RenderText(font, text_adjuster.AdjustText(font, fine_text));
}

inline TOGL_SizeU TOGL_GetTextSize(const std::string& text) {
    return TOGL_GetTextSize(TOGL_FineText(text));
}

inline TOGL_SizeU TOGL_GetTextSize(const TOGL_FineText& fine_text) {
    TOGL_Font&          font            = TOGL_ToGlobalFont();
    TOGL_TextDrawer&    drawer          = TOGL_ToGlobalTextDrawer();

    return drawer.GetTextSize(font, fine_text);
}

inline TOGL_SizeU TOGL_AdjustAndGetTextSize(const std::string& text) {
    return TOGL_AdjustAndGetTextSize(TOGL_FineText(text));
}

inline TOGL_SizeU TOGL_AdjustAndGetTextSize(const TOGL_FineText& fine_text) {
    TOGL_Font&          font            = TOGL_ToGlobalFont();
    TOGL_TextAdjuster&  text_adjuster   = TOGL_ToGlobalTextAdjuster();
    TOGL_TextDrawer&    drawer          = TOGL_ToGlobalTextDrawer();

    return drawer.GetTextSize(font, text_adjuster.AdjustText(font, fine_text));
}

inline void TOGL_SetOrigin(TOGL_OriginId origin_id) {
    TOGL_ToGlobalTextDrawer().SetOrigin(origin_id);
}

inline TOGL_OriginId TOGL_GetOrigin() {
    return TOGL_ToGlobalTextDrawer().GetOrigin();
}

//------------------------------------------------------------------------------
// TOGL_TextDrawer
//------------------------------------------------------------------------------

inline TOGL_TextDrawer::TOGL_TextDrawer() {
    Reset();
}

inline TOGL_TextDrawer::~TOGL_TextDrawer() {

}

inline void TOGL_TextDrawer::Reset() {
    SetOrigin(TOGL_ORIGIN_ID_LEFT_BOTTOM);
    SetPos(0, 0);

    m_color = {255, 255, 255, 255};
    m_text = {};
}

inline void TOGL_TextDrawer::SetPos(int x, int y) {
    SetPos({x, y});
}

inline void TOGL_TextDrawer::SetPos(const TOGL_PointI& pos) {
    m_pos   = pos;
    m_base  = pos;
}

inline void TOGL_TextDrawer::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SetColor({r, g, b, a});
}

inline void TOGL_TextDrawer::SetColor(const TOGL_Color4U8& color) {
    m_color = color;
}

inline void TOGL_TextDrawer::RenderText(TOGL_Font& font, const std::string& text) {
    RenderText(font, TOGL_FineText(text));
}

inline void TOGL_TextDrawer::RenderText(TOGL_Font& font, const TOGL_FineText& fine_text) {
    if (font.IsLoaded()) {
        const TOGL_OriginId old_origin_id = font.GetOrigin();
        font.SetOrigin(m_origin_id);

        glPushAttrib(GL_CURRENT_BIT);
        glColor4ubv(m_color.ToData());

        for (const TOGL_FineTextElementContainer& element_container : fine_text.ToElementContainers()) {

            switch (element_container.GetTypeId()) {

            case TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT: {
                font.RenderBegin();
                for (const uint32_t code : element_container.GetText()) {
                    if (code == '\n') {
                        m_pos.x = m_base.x;
                        m_pos.y += (font.GetHeight() + font.GetDistanceBetweenLines()) * m_orientation_factor_y;
                    } else {
                        glPushMatrix();
                        glTranslatef(float(m_pos.x), float(m_pos.y), 0);

                        font.RenderGlyph(code);
                        m_pos.x += font.GetGlyphSize(code).width + font.GetDistanceBetweenGlyphs();

                        glPopMatrix();
                    }
                }
                font.RenderEnd();
                break;
            }

            case TOGL_FINE_TEXT_ELEMENT_TYPE_ID_COLOR: {
                glColor4ubv(element_container.GetTextColor().ToData());
                break;
            }

            case TOGL_FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER:
                m_pos.x += element_container.GetTextHorizontalSpaceWidth();
                break;
            } // switch
        }

        glPopAttrib();

        font.SetOrigin(old_origin_id);
    }
}

inline TOGL_SizeU TOGL_TextDrawer::GetTextSize(TOGL_Font& font, const std::string& text) const {
    return GetTextSize(font, TOGL_FineText(text));
}

inline TOGL_SizeU TOGL_TextDrawer::GetTextSize(TOGL_Font& font, const  TOGL_FineText& fine_text) const {
    TOGL_SizeU size = {0, font.GetHeight()};
    uint32_t width = 0;

    bool is_glyph_before = false;
    
    if (font.IsLoaded()) {
        for (const TOGL_FineTextElementContainer& element_container : fine_text.ToElementContainers()) {
            switch (element_container.GetTypeId()) {

            case TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT: {
                for (const uint32_t code : element_container.GetText()) {
                    if (code == '\n') {
                        size.height += font.GetHeight() + font.GetDistanceBetweenLines();

                        if (size.width < width) size.width = width;
                        width = 0;

                        is_glyph_before = false;
                    } else {
                        if (is_glyph_before) width += font.GetDistanceBetweenGlyphs();

                        width += font.GetGlyphSize(code).width;

                        is_glyph_before = true;
                    }
                }
                break;
            }

            case TOGL_FINE_TEXT_ELEMENT_TYPE_ID_COLOR:
                break;

            case TOGL_FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER: {
                width += element_container.GetTextHorizontalSpaceWidth();

                is_glyph_before = false;
                break;
            }
            }
        }
        if (size.width < width) size.width = width;
    }

    return size;
}

inline void TOGL_TextDrawer::SetOrigin(TOGL_OriginId origin_id) {
    m_origin_id = origin_id;

    switch (m_origin_id) {
    case TOGL_ORIGIN_ID_LEFT_BOTTOM:
        m_orientation_factor_y = -1;
        break;
    case TOGL_ORIGIN_ID_LEFT_TOP:
        m_orientation_factor_y = 1;
        break;
    }
}

inline TOGL_OriginId TOGL_TextDrawer::GetOrigin() const {
    return m_origin_id;
}

//------------------------------------------------------------------------------

inline void TOGL_TextDrawer::ReplaceAll(std::string& text, const std::string& from, const std::string& to) {
    if (!from.empty()) {
        size_t pos = 0;
        while((pos = text.find(from, pos)) != std::string::npos) {
            text.replace(pos, from.length(), to);
            pos += to.length();
        }
    }
}

#endif // TRIVIALOPENGL_TEXTDRAWER_H_
