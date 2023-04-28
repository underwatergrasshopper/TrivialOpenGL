/**
* @file TrivialOpenGL_TextDrawer.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_TEXTDRAWER_H_
#define TRIVIALOPENGL_TEXTDRAWER_H_

#include "TrivialOpenGL_Font.h"

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

            m_text = L"";

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

        void SetText(const std::string& text) {
            m_text = PrepareText(text);
        }

        // width        - in pixels
        void SetLineWrapWidth(uint32_t width) {
            m_text_preparer.SetLineWrapWidth(width);
        }

        // ---
        
        // text             - Encoding format: UTF8.
        void RenderText(Font& font, const std::string& text) {
            RenderSolvedText(font, m_text_preparer.PrepareText(font, text));
        }

        // text             - Encoding format: UTF8.
        SizeU GetTextSize(Font& font, const std::string& text) const {
            return GetSolvedTextSize(font, m_text_preparer.PrepareText(font, text));
        }

        // text             - Encoding format: UTF8.
        void RenderText(Font& font) {
            RenderSolvedText(font, m_text);
        }

        // text             - Encoding format: UTF8.
        SizeU GetTextSize(Font& font) const {
            return GetSolvedTextSize(font, m_text);
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
                m_tab_as_spaces = std::string(number, ' ');
            }


            std::wstring PrepareText(const Font& font, const std::string& text) const {
                std::wstring prepared_text;

                const std::vector<std::wstring> words = SplitToWords(ToUTF16(text));

                size_t      line_length = 0;
                uint32_t    line_width = 0; // in pixels

                for (const auto& word : words) {

                    if (word == L"\n") {
                        prepared_text   += word;
                        line_length     = 0;

                    } else if (word == L"\t") {
                        uint32_t num_of_spaces = line_length % m_num_of_spaces_in_tab;
                        if (num_of_spaces == 0) num_of_spaces = m_num_of_spaces_in_tab;

                        if (m_wrap_line_width != 0 && (line_length + num_of_spaces) > m_wrap_line_width) {
                            prepared_text   += L"\n";
                            line_length     = 0;

                            prepared_text   += std::wstring(m_num_of_spaces_in_tab, L' ');
                            line_length     += m_num_of_spaces_in_tab;
                        } else {
                            prepared_text   += std::wstring(num_of_spaces, L' ');
                            line_length     += num_of_spaces;
                        }
                    } else {
                        if (m_wrap_line_width != 0 && word.length() > m_wrap_line_width) {
                            const size_t line_length_left = m_wrap_line_width - line_length;

                            
                            prepared_text   += word.substr(0, line_length_left);
                            line_length     += line_length_left;

                            prepared_text   += L"\n";
                            line_length     = 0;

                            prepared_text   += word.substr(line_length_left);
                            line_length     += word.substr(line_length_left).length();

                        } else if (m_wrap_line_width != 0 && (line_length + word.length()) > m_wrap_line_width) {
                            puts("0");
                            // Spaces are ignored if they are going after wrap line width.
                            if (word != L" ") {
                                prepared_text   += L"\n";
                                line_length     = 0;

                                prepared_text   += word;
                                line_length     += word.length();
                            }
  
                        } else {
                            prepared_text   += word;
                            line_length     += word.length();
                        }

                    }

                    // debug
                    //if (word == L"\n") {
                    //    wprintf(L"><\n");
                    //} else {
                    //    wprintf(L">%ls<\n", word.c_str());
                    //}
                    
                }
                return prepared_text;
            }
        private:
            uint32_t            m_num_of_spaces_in_tab;
            std::string         m_tab_as_spaces;

            uint32_t            m_wrap_line_width;          // in pixels

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

        std::wstring PrepareText(const std::string& text) const {
            return L"";


            //std::string text_utf8 = text;
            //
            //ReplaceAll(text_utf8, "\t", m_tab_as_spaces);
            //
            //return ToUTF16(text_utf8);
        }

        void RenderSolvedText(Font& font, const std::wstring& text_utf16) {
            glPushAttrib(GL_CURRENT_BIT);
            glColor4ubv(m_color.ToData());

            font.RenderBegin();
            for (const uint32_t code : text_utf16) {
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

            glPopAttrib();
        }

        // text             - Encoding format: UTF8.
        SizeU GetSolvedTextSize(Font& font, const std::wstring& text_utf16) const {
            SizeU size = {0, font.GetGlyphHeight()};

            uint32_t width = 0;
            for (const uint32_t code : text_utf16) {
                if (code == '\n') {
                    size.height += font.GetGlyphHeight();

                    if (size.width < width) size.width = width;
                    width = 0;
                } else {
                    width += font.GetGlyphSize(code).width;
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
        std::wstring            m_text;
        
        TextPreparer            m_text_preparer;
    };


} // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_TEXTDRAWER_H_
