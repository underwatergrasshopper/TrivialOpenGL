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

            SetNumberOfSpacesInTab(4);
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
            m_tab_as_spaces = std::string(number, ' ');
        }

        void SetText(const std::string& text) {
            m_text = SolveText(text);
        }

        // ---
        
        // text             - Encoding format: UTF8.
        void RenderText(Font& font, const std::string& text) {
            RenderSolvedText(font, SolveText(text));
        }

        // text             - Encoding format: UTF8.
        SizeU GetTextSize(Font& font, const std::string& text) const {
            return GetSolvedTextSize(font, SolveText(text));
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
        static void ReplaceAll(std::string& text, const std::string& from, const std::string& to) {
            if (!from.empty()) {
                size_t pos = 0;
                while((pos = text.find(from, pos)) != std::string::npos) {
                    text.replace(pos, from.length(), to);
                    pos += to.length();
                }
            }
        }

        std::wstring SolveText(const std::string& text) const {
            std::string text_utf8 = text;

            ReplaceAll(text_utf8, "\t", m_tab_as_spaces);

            return ToUTF16(text_utf8);
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

        std::string             m_tab_as_spaces;
    };


} // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_TEXTDRAWER_H_
