/**
* @file TrivialOpenGL_FineText.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_FINETEXT_H_
#define TRIVIALOPENGL_FINETEXT_H_

#include "TrivialOpenGL_Utility.h"

//==============================================================================
// Declarations
//==============================================================================

enum TOGL_FineTextElementTypeId {
    TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT,
    TOGL_FINE_TEXT_ELEMENT_TYPE_ID_COLOR,
    TOGL_FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER
};

//------------------------------------------------------------------------------
// TOGL_TextHorizontalSpacer
//------------------------------------------------------------------------------

class TOGL_TextHorizontalSpacer {
public:
    TOGL_TextHorizontalSpacer() : m_width(0) {}

    // width - in pixels
    TOGL_TextHorizontalSpacer(uint32_t width) : m_width(width) {}
    virtual ~TOGL_TextHorizontalSpacer() {}

    // Returns width in pixels.
    uint32_t GetWidth() const {
        return m_width;
    }

private:
    uint32_t m_width; // in pixels
};

//------------------------------------------------------------------------------
// TOGL_FineTextElement
//------------------------------------------------------------------------------

class TOGL_FineTextElement {
public:
    TOGL_FineTextElement()                                                   : m_container(nullptr) {}
    TOGL_FineTextElement(const std::string& text_utf8)                       : m_container(new TextContainer(text_utf8)) {}
    TOGL_FineTextElement(const std::wstring& text)                           : m_container(new TextContainer(text)) {}
    TOGL_FineTextElement(const TOGL_Color4U8& color)                         : m_container(new TextColorContainer(color)) {}
    TOGL_FineTextElement(const TOGL_TextHorizontalSpacer& horizontal_spacer) : m_container(new TextHorizontalSpacerContainer(horizontal_spacer)) {}

    TOGL_FineTextElement(const TOGL_FineTextElement& text_element)           : m_container(text_element.m_container->CopyNew()) {}
    TOGL_FineTextElement& operator=(const TOGL_FineTextElement& text_element) {
        if (this != &text_element) {
            delete m_container;
            m_container = text_element.m_container->CopyNew();
        }
        return *this;
    }

    TOGL_FineTextElement(TOGL_FineTextElement&& text_element) noexcept {
        m_container = text_element.m_container;
        text_element.m_container = nullptr;
    }

    TOGL_FineTextElement& operator=(TOGL_FineTextElement&& text_element) noexcept {
        delete m_container;
        m_container = text_element.m_container;
        text_element.m_container = nullptr;
        return *this;
    }

    virtual ~TOGL_FineTextElement() {
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

    void SetColor(const TOGL_Color4U8& color) {
        if (!m_container) m_container = new TextContainer(L"");
        m_container->SetColor(color);
    }

    TOGL_Color4U8 GetColor() const {
        if (!m_container) return {};
        return m_container->GetColor();
    }

    void SetTextHorizontalSpacer(TOGL_TextHorizontalSpacer& spacer) {
        if (!m_container) m_container = new TextContainer(L"");
        m_container->SetTextHorizontalSpacer(spacer);
    }

    TOGL_TextHorizontalSpacer GetTextHorizontalSpacer() const {
        if (!m_container) return {};
        return m_container->GetTextHorizontalSpacer();
    }

    TOGL_FineTextElementTypeId GetTypeId() const {
        if (!m_container) return TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT;
        return m_container->GetTypeId();
    }

private:
    struct Container {
        virtual ~Container() {}

        virtual void SetText(const std::wstring& text) {}
        virtual std::wstring GetText() const { return L""; }

        virtual void SetColor(const TOGL_Color4U8& color) {}
        virtual TOGL_Color4U8 GetColor() const { return {}; }

        virtual void SetTextHorizontalSpacer(const TOGL_TextHorizontalSpacer& spacer) {}
        virtual TOGL_TextHorizontalSpacer GetTextHorizontalSpacer() const { return {}; }

        virtual Container* CopyNew() const = 0;
        virtual TOGL_FineTextElementTypeId GetTypeId() const = 0;
    };

    struct TextContainer : Container {
        std::wstring text;

        TextContainer(const std::string& text_utf8) : text(TOGL_ToUTF16(text_utf8)) {}
        TextContainer(const std::wstring& text) : text(text) {}

        void SetText(const std::wstring& text) override final { this->text = text; }
        std::wstring GetText() const override final { return text; }

        TextContainer* CopyNew() const override final {
            return new TextContainer(text);
        }

        TOGL_FineTextElementTypeId GetTypeId() const override final {
            return TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT;
        }
    };

    struct TextColorContainer : Container {
        TOGL_Color4U8 color;

        TextColorContainer(const TOGL_Color4U8& color) : color(color) {}

        void SetColor(const TOGL_Color4U8& color) override final { this->color = color; }
        TOGL_Color4U8 GetColor() const override final { return color; }

        TextColorContainer* CopyNew() const override final {
            return new TextColorContainer(color);
        }

        TOGL_FineTextElementTypeId GetTypeId() const override final {
            return TOGL_FINE_TEXT_ELEMENT_TYPE_ID_COLOR;
        }
    };

    struct TextHorizontalSpacerContainer : Container {
        TOGL_TextHorizontalSpacer horizontal_spacer;

        TextHorizontalSpacerContainer(const TOGL_TextHorizontalSpacer& horizontal_spacer) : horizontal_spacer(horizontal_spacer) {}

        void SetTextHorizontalSpacer(const TOGL_TextHorizontalSpacer& spacer) override final { horizontal_spacer = spacer; }
        TOGL_TextHorizontalSpacer GetTextHorizontalSpacer() const override final { return horizontal_spacer; }

        TextHorizontalSpacerContainer* CopyNew() const override final {
            return new TextHorizontalSpacerContainer(horizontal_spacer);
        }

        TOGL_FineTextElementTypeId GetTypeId() const override final {
            return TOGL_FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER;
        }
    };

    Container* m_container;
};

//------------------------------------------------------------------------------
// TOGL_FineTextElement
//------------------------------------------------------------------------------

class TOGL_FineText {
public:
    TOGL_FineText() {}

    // text - Encoding format: UTF8.
    TOGL_FineText(const std::string& text) {
        Append(TOGL_FineTextElement(text));
    }

    TOGL_FineText(const std::wstring& text) {
        Append(TOGL_FineTextElement(text));
    }

    template <typename... Types>
    explicit TOGL_FineText(const TOGL_FineTextElement& element, const Types&... arguments) {
        Append(element, TOGL_FineTextElement(arguments)...);
    }

    explicit TOGL_FineText(const std::vector<TOGL_FineTextElement>& elements) : m_elements(elements) {}

    virtual ~TOGL_FineText() {}


    void Append(const TOGL_FineTextElement& element) {
        m_elements.push_back(element);
    }

    template <typename... Types>
    void Append(const TOGL_FineTextElement& element1, const TOGL_FineTextElement& element2, const Types&... arguments) {
        Append(element1);
        Append(element2, TOGL_FineTextElement(arguments)...);
    }

    void Append(const std::vector<TOGL_FineTextElement>& elements) {
        m_elements.insert(m_elements.end(), elements.begin(), elements.end());
    }

    void Clean() {
        m_elements.clear();
    }

    std::vector<TOGL_FineTextElement> GetElements() const {
        return m_elements;
    }

    std::vector<TOGL_FineTextElement>& ToElements() {
        return m_elements;
    }

    const std::vector<TOGL_FineTextElement>& ToElements() const {
        return m_elements;
    }

private:
    std::vector<TOGL_FineTextElement> m_elements;
};

inline TOGL_FineText& operator+=(TOGL_FineText& l, const TOGL_FineText& r) {
    l.Append(r.ToElements());
    return l;
}

#endif // TRIVIALOPENGL_FINETEXT_H_
