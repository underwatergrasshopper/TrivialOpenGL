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
// TOGL_FineTextElementContainer
//------------------------------------------------------------------------------

class TOGL_FineTextElementContainer {
public:
    TOGL_FineTextElementContainer()                                                   : m_container(nullptr) {}
    TOGL_FineTextElementContainer(const std::string& text_utf8)                       : m_container(new TextContainer(text_utf8)) {}
    TOGL_FineTextElementContainer(const std::wstring& text)                           : m_container(new TextContainer(text)) {}
    TOGL_FineTextElementContainer(const TOGL_Color4U8& color)                         : m_container(new TextColorContainer(color)) {}
    TOGL_FineTextElementContainer(const TOGL_TextHorizontalSpacer& horizontal_spacer) : m_container(new TextHorizontalSpacerContainer(horizontal_spacer)) {}

    TOGL_FineTextElementContainer(const TOGL_FineTextElementContainer& text_element)  : m_container(text_element.m_container->CopyNew()) {}

    TOGL_FineTextElementContainer& operator=(const TOGL_FineTextElementContainer& text_element) {
        if (this != &text_element) {
            delete m_container;
            m_container = text_element.m_container->CopyNew();
        }
        return *this;
    }

    TOGL_FineTextElementContainer(TOGL_FineTextElementContainer&& text_element) noexcept {
        m_container = text_element.m_container;
        text_element.m_container = nullptr;
    }

    TOGL_FineTextElementContainer& operator=(TOGL_FineTextElementContainer&& text_element) noexcept {
        delete m_container;
        m_container = text_element.m_container;
        text_element.m_container = nullptr;
        return *this;
    }

    virtual ~TOGL_FineTextElementContainer() {
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
// TOGL_FineText
//------------------------------------------------------------------------------

class TOGL_FineText {
public:
    using ElementContainer = TOGL_FineTextElementContainer;

    TOGL_FineText() {}

    // text - Encoding format: UTF8.
    TOGL_FineText(const std::string& text) {
        Append(text);
    }

    TOGL_FineText(const std::wstring& text) {
        Append(text);
    }

    template <typename Type, typename... Types>
    explicit TOGL_FineText(const Type& element, const Types&... elements) {
        Append(element, elements...);
    }

    explicit TOGL_FineText(const std::vector<ElementContainer>& element_containers) : m_element_containers(element_containers) {}

    virtual ~TOGL_FineText() {}

    // element, elements    - Any of those types: std::string, std::wstring,TOGL_ Color3FU, 
    //                        TOGL_TextHorizontalSpacer, ElementContainer, std::vector<TOGL_FineTextElementContainer>.
    template <typename Type, typename... Types>
    void Set(const Type& element, const Types&... elements) {
        Clear();
        Append(element, elements...);
    }

    // Adds new text.
    void Append(const std::string& text) {
        Append(TOGL_ToUTF16(text));
    }

    // Adds new text.
    void Append(const std::wstring& text) {
        auto _TOGL_Min = [](const auto& a, const auto& b) -> decltype(a + b) {
            return a < b ? a : b;
        };

        // std::vector<Type>::size_type can be uint32_t or uint64_t or something else. 
        // To avoid problems with converting to type with smaller range, max_size will be at maximum half size of UINT32.
        const std::vector<ElementContainer>::size_type max_size = _TOGL_Min(UINT_MAX / 2, m_element_containers.max_size());

        if (text.size() > max_size) {
            m_element_containers.push_back(ElementContainer(text.substr(0, max_size)));
            Append(text.substr(max_size));
        } else {
            m_element_containers.push_back(ElementContainer(text));
        }
    }

    // Sets text color. Will be used for text newly added after this element.
    void Append(const TOGL_Color4U8& color) {
        m_element_containers.push_back(ElementContainer(color));
    }

    // Adds horizontal spacer to text.
    void Append(const TOGL_TextHorizontalSpacer& horizontal_spacer) {
        m_element_containers.push_back(ElementContainer(horizontal_spacer));
    }

    void Append(const ElementContainer& element) {
        m_element_containers.push_back(element);
    }

    void Append(const std::vector<ElementContainer>& element_containers) {
        m_element_containers.insert(m_element_containers.end(), element_containers.begin(), element_containers.end());
    }

    // element1, element2, elements   - Any of those types: std::string, std::wstring,TOGL_ Color3FU, 
    //                                  TOGL_TextHorizontalSpacer, ElementContainer, std::vector<TOGL_FineTextElementContainer>.
    template <typename Type1, typename Type2, typename... Types>
    void Append(const Type1& element1, const Type2& element2, const Types&... elements) {
        Append(element1);
        Append(element2, elements...);
    }

    void Clear() {
        m_element_containers.clear();
    }

    std::vector<ElementContainer> GetElementContainers() const {
        return m_element_containers;
    }

    std::vector<ElementContainer>& ToElementContainers() {
        return m_element_containers;
    }

    const std::vector<ElementContainer>& ToElementContainers() const {
        return m_element_containers;
    }

private:
    std::vector<ElementContainer> m_element_containers;
};

inline TOGL_FineText& operator+=(TOGL_FineText& l, const TOGL_FineText& r) {
    l.Append(r.ToElementContainers());
    return l;
}

#endif // TRIVIALOPENGL_FINETEXT_H_
