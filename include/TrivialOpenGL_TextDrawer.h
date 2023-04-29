/**
* @file TrivialOpenGL_TextDrawer.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_TEXTDRAWER_H_
#define TRIVIALOPENGL_TEXTDRAWER_H_

#include "TrivialOpenGL_Font.h"
#include "TrivialOpenGL_FineText.h"

namespace TrivialOpenGL {

    enum TextDrawerOrientation {
        TEXT_DRAWER_ORIENTATION_LEFT_BOTTOM,
        TEXT_DRAWER_ORIENTATION_LEFT_TOP,
    };

    class TextDrawer {
    public:
        TextDrawer() {
            Reset();
        }

        virtual ~TextDrawer() {}

        // ---

        void Reset() {
            SetOrientation(TEXT_DRAWER_ORIENTATION_LEFT_BOTTOM);
            SetPos(0, 0);

            m_color = {255, 255, 255, 255};

            m_text_preparer.Reset();
        }

        void SetPos(int x, int y) {
            SetPos({x, y});
        }

        void SetPos(const PointI& pos) {
            m_pos   = pos;
            m_base  = pos;
        }

        void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
            SetColor({r, g, b, a});
        }

        void SetColor(const Color4U8& color) {
            m_color = color;
        }

        void SetNumberOfSpacesInTab(uint32_t number) {
            m_text_preparer.SetNumberOfSpacesInTab(number);
        }

        // width        - in pixels
        void SetLineWrapWidth(uint32_t width) {
            m_text_preparer.SetLineWrapWidth(width);
        }

        // ---
        
        // text             - Encoding format: UTF8.
        void RenderText(Font& font, const std::string& text) {
            RenderText(font, FineText(text));
        }
        void RenderText(Font& font, const FineText& text) {
            RenderSolvedText(font, m_text_preparer.PrepareText(font, text));
        }

        // text             - Encoding format: UTF8.
       SizeU GetTextSize(Font& font, const std::string& text) const {
            return GetTextSize(font, FineText(text));
        }

        SizeU GetTextSize(Font& font, const FineText& text) const {
            return GetSolvedTextSize(font, m_text_preparer.PrepareText(font, text));
        }

        // ---

        void SetOrientation(TextDrawerOrientation orientation) {
            m_orientation = orientation;

            switch (m_orientation) {
            case TEXT_DRAWER_ORIENTATION_LEFT_BOTTOM:
                m_orientation_factor_y = -1;
                break;
            case TEXT_DRAWER_ORIENTATION_LEFT_TOP:
                m_orientation_factor_y = 1;
                break;
            }
        }

        TextDrawerOrientation GetOrientation() const {
            return m_orientation;
        }

    private:

        class TextPreparer {
        public:
            TextPreparer() {            
                Reset();

                m_width_of_full_tab = 0;        
                m_line_width        = 0;   
            }
            virtual ~TextPreparer() {}

            void Reset() {
                m_wrap_line_width = 0;
                SetNumberOfSpacesInTab(4);
            }

            // width        - in pixels
            void SetLineWrapWidth(uint32_t width) {
                m_wrap_line_width = width;
            }

            void SetNumberOfSpacesInTab(uint32_t number) {
                m_num_of_spaces_in_tab = number;
                m_tab_as_spaces = std::wstring(number, L' ');
            }

            static SizeU GetWordSize(const Font& font, const std::wstring& word) {
                SizeU size;
                size.height = font.GetGlyphHeight();
                for (const wchar_t c : word) size.width += font.GetGlyphSize(c).width;
                return size;
            }

            static uint32_t GetWordWidth(const Font& font, const std::wstring& word) {
                uint32_t width = 0;
                for (const wchar_t c : word) width += font.GetGlyphSize(c).width;
                return width;
            }

            // width - in pixels
            static size_t GetGlyphCountInWidth(const Font& font, const std::wstring& word, uint32_t max_width) {
                size_t      count = 0;
                uint32_t    width = 0;
                for (const wchar_t c : word) {
                    width += font.GetGlyphSize(c).width;
                    if (max_width < width) break;
                    count += 1;
                }
                return count;
            }

            FineText PrepareText(const Font& font, const FineText& text) const {
                FineText prepared_text;

                uint32_t line_width = 0; // in pixels

                for (const TextElement& element : text.ToElements()) {
                    switch (element.type_id) {
                    case TEXT_ELEMENT_TYPE_ID_TEXT:
                        prepared_text += PrepareTextElementText(font, element, line_width);
                        break;
                    case TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER:
                        prepared_text += PrepareTextElementHorizontalSpacer(font, element, line_width);
                        break;
                    case TEXT_ELEMENT_TYPE_ID_COLOR:
                        prepared_text.Append(element);
                        break;

                    } // switch
                }
 
                return prepared_text;
            }

        private:
            uint32_t            m_num_of_spaces_in_tab;
            std::wstring        m_tab_as_spaces;

            uint32_t            m_wrap_line_width;          // in pixels

            uint32_t            m_width_of_full_tab;        // in pixels
            uint32_t            m_line_width;               // in pixels

            // <word>
            //     '\t'         # tab
            //     '\n'         # new line
            //     ' '          # space
            //     '[^\t\n ]'   # any array of characters which doesn't contain tab, new line or space
            static size_t GetWordPos(const std::wstring& text, size_t current_pos) {
                auto IsWhiteSpace = [](wchar_t c) -> bool {
                    return c == L' ' || c == L'\t' || c == L'\n';
                };

                for (size_t next_word_pos = current_pos; next_word_pos < text.size(); ++next_word_pos) {
                    const wchar_t c = text[next_word_pos];

                    if (IsWhiteSpace(text[next_word_pos])) return next_word_pos + 1;
                    if (next_word_pos + 1 < text.size() && IsWhiteSpace(text[next_word_pos + 1])) return next_word_pos + 1;
                }
                return std::wstring::npos;
            };

            static std::vector<std::wstring> SplitToWords(const std::wstring& text) {
                std::vector<std::wstring> words;

                size_t pos = 0;
                while (pos < text.size()) {
                    size_t next_word_pos = GetWordPos(text, pos);

                    const std::wstring word = text.substr(pos, next_word_pos - pos);

                    words.push_back(word);
                    pos = next_word_pos;
                }

                return words;
            };

            FineText PrepareTextElementHorizontalSpacer(const Font & font, const TextElement & element_spacer, uint32_t & line_width) const {
                FineText prepared_text;

                const uint32_t spacer_width = element_spacer.horizontal_spacer.GetWidth();

                if (spacer_width + line_width > m_wrap_line_width) {
                    prepared_text.Append(TextElement(L"\n"));
                    line_width = 0;
                }
                prepared_text.Append(element_spacer);
                line_width += spacer_width;

                return prepared_text;
            }

            FineText PrepareTextElementText(const Font& font, const TextElement& element_text, uint32_t& line_width) const {
                FineText prepared_text;
                TextElement element = TextElement(L"");

                const uint32_t width_of_full_tab = GetWordWidth(font, m_tab_as_spaces); // in pixels

                const std::vector<std::wstring> words = SplitToWords(element_text.text);

                for (const auto& word : words) {
                    const uint32_t word_width = GetWordWidth(font, word);

                    if (word == L"\n") {
                        element.text    += word;
                        line_width      = 0;

                    } else if (word == L"\t") {
                        uint32_t width_of_tab = width_of_full_tab - (line_width % width_of_full_tab);
                        if (width_of_tab == 0) width_of_tab = width_of_full_tab;
                        
                        if (m_wrap_line_width != 0 && (line_width + width_of_full_tab) > m_wrap_line_width) {
                            element.text    += L"\n";
                            line_width      = 0;

                            width_of_tab = width_of_full_tab;
                        } 

                        prepared_text.Append(element, TextHorizontalSpacer(width_of_tab));
                        element = TextElement(L"");

                        line_width += width_of_tab;
                    } else {
                        if (m_wrap_line_width != 0 && (line_width + word_width) > m_wrap_line_width) {
                            // Word is crossing wrap line width. Needs to be moved or split.

                            // Spaces are ignored if they are behind wrap line width.
                            if (word != L" ") {

                                if (word_width > m_wrap_line_width) {
                                    // Word is longer than line. Must be split between two or multiple lines.
                                    std::wstring    long_word       = word;
                                    uint32_t        long_word_width = word_width;

                                    while (long_word_width > m_wrap_line_width) {
                                        const uint32_t line_width_left = m_wrap_line_width - line_width;

                                        const size_t glyph_count = GetGlyphCountInWidth(font, long_word, line_width_left);

                                        element.text    += long_word.substr(0, glyph_count);
                                        element.text    += L"\n";
                                        line_width      = 0;

                                        long_word       = long_word.substr(glyph_count);
                                        long_word_width = GetWordWidth(font, long_word);
                                    }

                                    element.text    += long_word;
                                    line_width      += long_word_width;

                                } else {
                                    // Word is shorter than line. Whole word is moved to next line
                                    element.text    += L"\n";
                                    line_width      = 0;

                                    element.text    += word;
                                    line_width      += word_width;
                                }
                            }
  
                        } else {
                            element.text    += word;
                            line_width      += word_width;
                        }

                    }
                }
                prepared_text.Append(element);
                return prepared_text;
            }

        };

        static void ReplaceAll(std::string& text, const std::string& from, const std::string& to) {
            if (!from.empty()) {
                size_t pos = 0;
                while((pos = text.find(from, pos)) != std::string::npos) {
                    text.replace(pos, from.length(), to);
                    pos += to.length();
                }
            }
        }

        void RenderSolvedText(Font& font, const FineText& text) {
            glPushAttrib(GL_CURRENT_BIT);
            glColor4ubv(m_color.ToData());

            for (const TextElement& element : text.ToElements()) {
                switch (element.type_id) {

                case TEXT_ELEMENT_TYPE_ID_TEXT:
                    font.RenderBegin();
                    for (const uint32_t code : element.text) {
                        if (code == '\n') {
                            m_pos.x = m_base.x;
                            m_pos.y += font.GetGlyphHeight() * m_orientation_factor_y;
                        } else {
                            glPushMatrix();
                            glTranslatef(float(m_pos.x), float(m_pos.y), 0);

                            font.RenderGlyph(code);
                            m_pos.x += font.GetGlyphSize(code).width;

                            glPopMatrix();
                        }
                    }
                    font.RenderEnd();
                    break;

                case TEXT_ELEMENT_TYPE_ID_COLOR:
                    glColor4ubv(element.color.ToData());
                    break;

                case TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER:
                    m_pos.x += element.horizontal_spacer.GetWidth();
                    break;
                } // switch
            }

            glPopAttrib();
        }

        // text             - Encoding format: UTF8.
        SizeU GetSolvedTextSize(Font& font, const FineText& text) const {
            SizeU size = {0, font.GetGlyphHeight()};
            uint32_t width = 0;


            for (const TextElement& element : text.ToElements()) {
                switch (element.type_id) {

                case TEXT_ELEMENT_TYPE_ID_TEXT:
                    for (const uint32_t code : element.text) {
                        if (code == '\n') {
                            size.height += font.GetGlyphHeight();

                            if (size.width < width) size.width = width;
                            width = 0;
                        } else {
                            width += font.GetGlyphSize(code).width;
                        }
                    }
                    break;
                case TEXT_ELEMENT_TYPE_ID_COLOR:
                    break;

                case TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER:
                    width += element.horizontal_spacer.GetWidth();
                    break;
                }
            }

            if (size.width < width) size.width = width;


            return size;
        }

        TextDrawerOrientation   m_orientation;
        uint32_t                m_orientation_factor_y;

        PointI                  m_pos;
        PointI                  m_base;

        Color4U8                m_color;
        
        TextPreparer            m_text_preparer;
    };


} // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_TEXTDRAWER_H_
