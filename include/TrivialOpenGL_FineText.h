/**
* @file TrivialOpenGL_FineText.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_FINETEXT_H_
#define TRIVIALOPENGL_FINETEXT_H_

#include "TrivialOpenGL_Utility.h"

namespace TrivialOpenGL {

    enum FineTextElementTypeId {
        FINE_TEXT_ELEMENT_TYPE_ID_TEXT,
        FINE_TEXT_ELEMENT_TYPE_ID_COLOR,
        FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER
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

    class FineTextElement {
    public:
        FineTextElement()                                               : m_container(nullptr) {}
        FineTextElement(const std::string& text_utf8)                   : m_container(new TextContainer(text_utf8)) {}
        FineTextElement(const std::wstring& text)                       : m_container(new TextContainer(text)) {}
        FineTextElement(const Color4U8& color)                          : m_container(new TextColorContainer(color)) {}
        FineTextElement(const TextHorizontalSpacer& horizontal_spacer)  : m_container(new TextHorizontalSpacerContainer(horizontal_spacer)) {}

        FineTextElement(const FineTextElement& text_element)                : m_container(text_element.m_container->CopyNew()) {}
        FineTextElement& operator=(const FineTextElement& text_element) {
            if (this != &text_element) {
                delete m_container;
                m_container = text_element.m_container->CopyNew();
            }
            return *this;
        }

        FineTextElement(FineTextElement&& text_element) noexcept {
            m_container = text_element.m_container;
            text_element.m_container = nullptr;
        }

        FineTextElement& operator=(FineTextElement&& text_element) noexcept {
            delete m_container;
            m_container = text_element.m_container;
            text_element.m_container = nullptr;
            return *this;
        }

        virtual ~FineTextElement() {
            delete m_container;
        }

        void SetText(const std::wstring& text) {
            if (!m_container) m_container = new TextContainer(L"");
            m_container->SetText(text);
        }

        std::wstring GetText() const {
            if (!m_container) return {};
            return m_container->GetText();
        }

        void SetColor(const Color4U8& color) {
            if (!m_container) m_container = new TextContainer(L"");
            m_container->SetColor(color);
        }

        Color4U8 GetColor() const {
            if (!m_container) return {};
            return m_container->GetColor();
        }

        void SetTextHorizontalSpacer(TextHorizontalSpacer& spacer) {
            if (!m_container) m_container = new TextContainer(L"");
            m_container->SetTextHorizontalSpacer(spacer);
        }

        TextHorizontalSpacer GetTextHorizontalSpacer() const {
            if (!m_container) return {};
            return m_container->GetTextHorizontalSpacer();
        }

        FineTextElementTypeId GetTypeId() const {
            if (!m_container) return FINE_TEXT_ELEMENT_TYPE_ID_TEXT;
            return m_container->GetTypeId();
        }

    private:
        struct Container {
            virtual ~Container() {}

            virtual void SetText(const std::wstring& text) {}
            virtual std::wstring GetText() const { return L""; }

            virtual void SetColor(const Color4U8& color) {}
            virtual Color4U8 GetColor() const { return {}; }

            virtual void SetTextHorizontalSpacer(const TextHorizontalSpacer& spacer) {}
            virtual TextHorizontalSpacer GetTextHorizontalSpacer() const { return {}; }

            virtual Container* CopyNew() const = 0;
            virtual FineTextElementTypeId GetTypeId() const = 0;
        };

        struct TextContainer : Container {
            std::wstring text;

            TextContainer(const std::string& text_utf8) : text(ToUTF16(text_utf8)) {}
            TextContainer(const std::wstring& text) : text(text) {}

            void SetText(const std::wstring& text) override final { this->text = text; }
            std::wstring GetText() const override final { return text; }

            TextContainer* CopyNew() const override final {
                return new TextContainer(text);
            }

            FineTextElementTypeId GetTypeId() const override final {
                return FINE_TEXT_ELEMENT_TYPE_ID_TEXT;
            }
        };

        struct TextColorContainer : Container {
            Color4U8 color;

            TextColorContainer(const Color4U8& color) : color(color) {}

            void SetColor(const Color4U8& color) override final { this->color = color; }
            Color4U8 GetColor() const override final { return color; }

            TextColorContainer* CopyNew() const override final {
                return new TextColorContainer(color);
            }

            FineTextElementTypeId GetTypeId() const override final {
                return FINE_TEXT_ELEMENT_TYPE_ID_COLOR;
            }
        };

        struct TextHorizontalSpacerContainer : Container {
            TextHorizontalSpacer horizontal_spacer;

            TextHorizontalSpacerContainer(const TextHorizontalSpacer& horizontal_spacer) : horizontal_spacer(horizontal_spacer) {}

            void SetTextHorizontalSpacer(const TextHorizontalSpacer& spacer) override final { horizontal_spacer = spacer; }
            TextHorizontalSpacer GetTextHorizontalSpacer() const override final { return horizontal_spacer; }

            TextHorizontalSpacerContainer* CopyNew() const override final {
                return new TextHorizontalSpacerContainer(horizontal_spacer);
            }

            FineTextElementTypeId GetTypeId() const override final {
                return FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER;
            }
        };

        Container* m_container;
    };

    class FineText {
    public:
        FineText() {}

        // text - Encoding format: UTF8.
        FineText(const std::string& text) {
            Append(FineTextElement(text));
        }

        FineText(const std::wstring& text) {
            Append(FineTextElement(text));
        }

        template <typename... Types>
        explicit FineText(const FineTextElement& element, const Types&... arguments) {
            Append(element, FineTextElement(arguments)...);
        }

        explicit FineText(const std::vector<FineTextElement>& elements) : m_elements(elements) {}

        virtual ~FineText() {}


        void Append(const FineTextElement& element) {
            m_elements.push_back(element);
        }

        template <typename... Types>
        void Append(const FineTextElement& element1, const FineTextElement& element2, const Types&... arguments) {
            Append(element1);
            Append(element2, FineTextElement(arguments)...);
        }

        void Append(const std::vector<FineTextElement>& elements) {
            m_elements.insert(m_elements.end(), elements.begin(), elements.end());
        }

        void Clean() {
            m_elements.clear();
        }

        std::vector<FineTextElement> GetElements() const {
            return m_elements;
        }

        std::vector<FineTextElement>& ToElements() {
            return m_elements;
        }

        const std::vector<FineTextElement>& ToElements() const {
            return m_elements;
        }

    private:
        std::vector<FineTextElement> m_elements;
    };

    inline FineText& operator+=(FineText& l, const FineText& r) {
        l.Append(r.ToElements());
        return l;
    }

} // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_FINETEXT_H_
