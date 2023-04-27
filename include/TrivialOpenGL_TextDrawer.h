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

    template <typename Type>
    struct Color4 {
        Type r;
        Type g;
        Type b;
        Type a;

        Color4() : r(0), g(0), b(0), a(0) {}

        template <typename TypeRGBA>
        Color4(const TypeRGBA& r, const TypeRGBA& g, const TypeRGBA& b, const TypeRGBA& a) : r(r), g(g), b(b), a(a) {}

        Type* ToData() {
            return (Type*)this;
        }

        const Type* ToData() const {
            return (const Type*)this;
        }
    };

    using Color4U8 = Color4<uint8_t>;

    class TextDrawer {
    public:
        TextDrawer() {
            SetOrientation(TEXT_DRAWER_ORIENTATION_LEFT_BOTTOM);
            SetPos(0, 0);

            m_color = {255, 255, 255, 255};
        }

        virtual ~TextDrawer() {}

        // ---

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

        // text             - Encoding format: UTF8.
        void RenderText(Font& font, const std::string& text) {
            glPushAttrib(GL_CURRENT_BIT);

            glColor4ubv(m_color.ToData());

            const std::wstring text_utf16 = ToUTF16(text);

            for (const wchar_t c : text_utf16) {
                // TODO: for char len = 2?
                if (c == L'\t') {
                    InnerRenderText(font, L"    ", 4);
                } else {
                    InnerRenderText(font, &c, 1);
                }
            }

            glPopAttrib();
        }

        void InnerRenderText(Font& font, const wchar_t* text_utf16, size_t length) {
            font.RenderBegin();

            for (size_t index = 0; index < length; ++index) {
                const uint32_t code = text_utf16[index];

                if (code == '\n') {
                    m_pos.x = m_base.x;
                    m_pos.y += font.GetFontHeight() * m_orientation_factor_y;
                } else {
                    glPushMatrix();
                    glTranslatef(float(m_pos.x), 0, 0);

                    font.RenderGlyph(code);
                    m_pos.x += font.GetGlyphSize(code).width;
                    glPopMatrix();
                }
            }

            font.RenderEnd();
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
        TextDrawerOrientation   m_orientation;
        uint32_t                m_orientation_factor_y;

        PointI                  m_pos;
        PointI                  m_base;
        Color4U8                m_color;
    };


} // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_TEXTDRAWER_H_
