/**
* @file TrivialOpenGL_FineText.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_FINETEXT_H_
#define TRIVIALOPENGL_FINETEXT_H_

#include "TrivialOpenGL_Utility.h"

namespace TrivialOpenGL {

    enum TextElementTypeId {
        TEXT_ELEMENT_TYPE_ID_TEXT,
        TEXT_ELEMENT_TYPE_ID_COLOR,
        TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER
    };

   class TextHorizontalSpacer {
    public:
        TextHorizontalSpacer() : m_width(0) {}

        // width - in pixels
        TextHorizontalSpacer(uint32_t width) : m_width(width) {}
        virtual ~TextHorizontalSpacer() {}

        // Returns width in pixels.
        uint32_t GetWidth() const {
            return m_width;
        }

    private:
        uint32_t m_width; // in pixels
    };

    struct TextElement {
        std::wstring            text;
        Color4U8                color;
        TextHorizontalSpacer    horizontal_spacer;
        TextElementTypeId       type_id;

        TextElement() : text(), color(),  horizontal_spacer(),  type_id(TEXT_ELEMENT_TYPE_ID_TEXT) {}
        TextElement(const std::string& text_utf8) : text(ToUTF16(text_utf8)), color(),  horizontal_spacer(),  type_id(TEXT_ELEMENT_TYPE_ID_TEXT) {}
        TextElement(const std::wstring& text) : text(text), color(),  horizontal_spacer(),  type_id(TEXT_ELEMENT_TYPE_ID_TEXT) {}
        TextElement(const Color4U8& color) : text(), color(color),  horizontal_spacer(),  type_id(TEXT_ELEMENT_TYPE_ID_COLOR) {}
        TextElement(const TextHorizontalSpacer& horizontal_spacer) : text(), color(),  horizontal_spacer(horizontal_spacer),  type_id(TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER) {}
    };

    class FineText {
    public:
        FineText() {}

        // text - Encoding format: UTF8.
        FineText(const std::string& text) {
            Append(TextElement(text));
        }

        FineText(const std::wstring& text) {
            Append(TextElement(text));
        }

        template <typename... Types>
        explicit FineText(const TextElement& element, const Types&... arguments) {
            Append(element, TextElement(arguments)...);
        }

        explicit FineText(const std::vector<TextElement>& elements) : m_elements(elements) {}

        virtual ~FineText() {}


        void Append(const TextElement& element) {
            m_elements.push_back(element);
        }

        template <typename... Types>
        void Append(const TextElement& element1, const TextElement& element2, const Types&... arguments) {
            Append(element1);
            Append(element2, TextElement(arguments)...);
        }

        void Append(const std::vector<TextElement>& elements) {
            m_elements.insert(m_elements.end(), elements.begin(), elements.end());
        }

        void Clean() {
            m_elements.clear();
        }

        std::vector<TextElement> GetElements() const {
            return m_elements;
        }

        std::vector<TextElement>& ToElements() {
            return m_elements;
        }

        const std::vector<TextElement>& ToElements() const {
            return m_elements;
        }

    private:
        std::vector<TextElement> m_elements;
    };

    inline FineText& operator+=(FineText& l, const FineText& r) {
        l.Append(r.ToElements());
        return l;
    }

} // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_FINETEXT_H_
