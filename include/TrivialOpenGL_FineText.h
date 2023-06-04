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

//------------------------------------------------------------------------------
// TOGL_FineTextElement
//------------------------------------------------------------------------------

enum TOGL_FineTextElementTypeId {
    TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT,
    TOGL_FINE_TEXT_ELEMENT_TYPE_ID_COLOR,
    TOGL_FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER
};

class TOGL_FineTextElement {
public:
    virtual ~TOGL_FineTextElement() {}
    
    virtual TOGL_FineTextElementTypeId GetTypeId() const = 0;

    // Might return nullptr.
    virtual TOGL_FineTextElement* CopyNew() const = 0;
};

class TOGL_Text : public TOGL_FineTextElement {
public:
    TOGL_Text() {}
    // text - Encoding format: ascii or utf8
    TOGL_Text(const std::string& text) : m_text(TOGL_ToUTF16(text)) {}
    // text - Encoding format: utf16
    TOGL_Text(const std::wstring& text) : m_text(text) {}
    // text - Encoding format: utf16
    TOGL_Text(std::wstring&& text) : m_text(std::move(text)) {}


    // text - Encoding format: utf16
    void SetText(const std::wstring& text) {
        m_text = text;
    }

    std::wstring GetText() const {
        return m_text;
    }

    TOGL_FineTextElementTypeId GetTypeId() const override final {
        return TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT;
    }

    TOGL_Text* CopyNew() const override final {
        return new TOGL_Text(*this);
    }
private:
    std::wstring m_text;
};

class TOGL_TextColor : public TOGL_FineTextElement {
public:
    TOGL_TextColor() {}
    TOGL_TextColor(const TOGL_Color4U8& color) : m_color(color) {}

    void SetColor(const TOGL_Color4U8& color) {
        m_color = color;
    }

    TOGL_Color4U8 GetColor() const {
        return m_color;
    }

    TOGL_FineTextElementTypeId GetTypeId() const override final {
        return TOGL_FINE_TEXT_ELEMENT_TYPE_ID_COLOR;
    }

    TOGL_TextColor* CopyNew() const override final {
        return new TOGL_TextColor(*this);
    }
private:
    TOGL_Color4U8 m_color;
};

class TOGL_TextHorizontalSpacer : public TOGL_FineTextElement {
public:
    TOGL_TextHorizontalSpacer() : m_width(0) {}
    // width - in pixels
    TOGL_TextHorizontalSpacer(uint32_t width) : m_width(width) {}

    void SetWidth(uint32_t width) {
        m_width = width;
    }

    // Returns width in pixels.
    uint32_t GetWidth() const {
        return m_width;
    }

    TOGL_FineTextElementTypeId GetTypeId() const override final {
        return TOGL_FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER;
    }

    TOGL_TextHorizontalSpacer* CopyNew() const override final {
        return new TOGL_TextHorizontalSpacer(*this);
    }
private:
    uint32_t m_width; // in pixels
};

//------------------------------------------------------------------------------
// TOGL_FineTextElementContainer
//------------------------------------------------------------------------------

class TOGL_FineTextElementContainer {
public:
    TOGL_FineTextElementContainer()                                 : m_element(nullptr) {}
    // text - Encoding format: ascii or utf8
    TOGL_FineTextElementContainer(const std::string& text)          : m_element(new TOGL_Text(text)) {}
    // text - Encoding format: utf16
    TOGL_FineTextElementContainer(const std::wstring& text)         : m_element(new TOGL_Text(text)) {}
    // text - Encoding format: utf16
    TOGL_FineTextElementContainer(std::wstring&& text)              : m_element(new TOGL_Text(std::move(text))) {}

    TOGL_FineTextElementContainer(const TOGL_Color4U8& color)       : m_element(new TOGL_TextColor(color)) {}
    // horizontal_space_width - in pixels
    TOGL_FineTextElementContainer(uint32_t horizontal_space_width)  : m_element(new TOGL_TextHorizontalSpacer(horizontal_space_width)) {}

    TOGL_FineTextElementContainer(const TOGL_FineTextElementContainer& other);
    TOGL_FineTextElementContainer(TOGL_FineTextElementContainer&& other) noexcept;

    TOGL_FineTextElementContainer& operator=(const TOGL_FineTextElementContainer& other);
    TOGL_FineTextElementContainer& operator=(TOGL_FineTextElementContainer&& other) noexcept;

    virtual ~TOGL_FineTextElementContainer();
    
    // If contained element is not TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT, then is changed to it.
    void SetText(const std::wstring& text);

    // If contained element is not TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT, then empty text is returned.
    std::wstring GetText() const;

    // If contained element is not TOGL_FINE_TEXT_ELEMENT_TYPE_ID_COLOR, then is changed to it.
    void SetTextColor(const TOGL_Color4U8& text_color);

    // If contained element is not TOGL_FINE_TEXT_ELEMENT_TYPE_ID_COLOR, then color (0, 0, 0, 0) is returned.
    TOGL_Color4U8 GetTextColor() const;

    // If contained element is not TOGL_FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER, then is changed to it.
    void SetTextHorizontalSpaceWidth(uint32_t width);

    // If contained element is not TOGL_FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER, then 0 is returned.
    uint32_t GetTextHorizontalSpaceWidth() const;

    bool IsTypeId(TOGL_FineTextElementTypeId type_id) const;
    TOGL_FineTextElementTypeId GetTypeId() const;

    // Might return nullptr.
    TOGL_FineTextElement* ToElement();

    // Might return nullptr.
    const TOGL_FineTextElement* ToElement() const;

private:
    class DefRawElement {
    public:
        friend class TOGL_Global<DefRawElement>;

        virtual ~DefRawElement() {}

        const std::wstring& ToText() const { return m_text; }
        const TOGL_Color4U8& ToTextColor() const { return m_text_color; }
        const uint32_t& ToTextHorizontalSpaceWidth() const { return m_text_horizontal_space_width; }

    private:
        DefRawElement() : m_text_horizontal_space_width(0) {}

        std::wstring    m_text;
        TOGL_Color4U8   m_text_color;
        uint32_t        m_text_horizontal_space_width;
    };

    static DefRawElement& ToDefRawElement();
    static TOGL_FineTextElement* MakeNew(TOGL_FineTextElementTypeId type_id);
    void MakeNewIfMismatch(TOGL_FineTextElementTypeId type_id);

    TOGL_FineTextElement* m_element;
};

//------------------------------------------------------------------------------
// TOGL_FineText
//------------------------------------------------------------------------------

class TOGL_FineText {
public:
    using ElementContainer = TOGL_FineTextElementContainer;

    TOGL_FineText();

    // text - Encoding format: UTF8.
    TOGL_FineText(const std::string& text);

    TOGL_FineText(const std::wstring& text);

    template <typename Type, typename... Types>
    explicit TOGL_FineText(const Type& raw_element, const Types&... raw_elements);

    explicit TOGL_FineText(const std::vector<ElementContainer>& element_containers);

    virtual ~TOGL_FineText();

    // raw_element1, raw_element2, raw_elements   - Any of those types: 
    //                                              std::string                   - interpreted as text, in either ascii or utf8 encoding;
    //                                              std::wstring                  - interpreted as text, in utf16 encoding;
    //                                              TOGL_ Color3FU                - interpreted as text color;
    //                                              uint32_t                      - interpreted as horizontal space width in pixels;
    //                                              ElementContainer.
    template <typename Type, typename... Types>
    void Set(const Type& raw_element, const Types&... raw_elements);

    // Adds new text.
    void Append(const std::string& text);

    // Adds new text.
    void Append(const std::wstring& text);

    // Sets text color. Will be used for text newly added after this element.
    void Append(const TOGL_Color4U8& color);

    // Adds horizontal spacer to text.
    // horizontal_space_width - in pixels
    void Append(uint32_t horizontal_space_width);

    void Append(const ElementContainer& element_container);

    void Append(const std::vector<ElementContainer>& element_containers);

    // raw_element1, raw_element2, raw_elements   - Any of those types: 
    //                                              std::string                   - interpreted as text, in either ascii or utf8 encoding;
    //                                              std::wstring                  - interpreted as text, in utf16 encoding;
    //                                              TOGL_ Color3FU                - interpreted as text color;
    //                                              uint32_t                      - interpreted as horizontal space width in pixels;
    //                                              ElementContainer.
    template <typename Type1, typename Type2, typename... Types>
    void Append(const Type1& raw_element1, const Type2& raw_element2, const Types&... raw_elements);

    void Clear();

    std::vector<ElementContainer> GetElementContainers() const;
    std::vector<ElementContainer>& ToElementContainers();
    const std::vector<ElementContainer>& ToElementContainers() const;

private:
    std::vector<ElementContainer> m_element_containers;
};

//------------------------------------------------------------------------------

TOGL_FineText& operator+=(TOGL_FineText& l, const TOGL_FineText& r);

//==============================================================================
// Definition
//==============================================================================
// 
//------------------------------------------------------------------------------
// TOGL_FineTextElementContainer
//------------------------------------------------------------------------------

inline TOGL_FineTextElementContainer::TOGL_FineTextElementContainer(const TOGL_FineTextElementContainer& other) {
    if (other.m_element) {
        m_element = other.m_element->CopyNew();
    } else {
        m_element = nullptr;
    }
}

inline TOGL_FineTextElementContainer::TOGL_FineTextElementContainer(TOGL_FineTextElementContainer&& other) noexcept {
    m_element       = other.m_element;
    other.m_element = nullptr;
}


inline TOGL_FineTextElementContainer& TOGL_FineTextElementContainer::operator=(const TOGL_FineTextElementContainer& other) {
    if (this != &other) {
        if (other.m_element) {
            m_element = other.m_element->CopyNew();
        } else {
            m_element = nullptr;
        }
    }
    return *this;
}

inline TOGL_FineTextElementContainer& TOGL_FineTextElementContainer::operator=(TOGL_FineTextElementContainer&& other) noexcept {
    if (m_element) delete m_element;

    m_element       = other.m_element;
    other.m_element = nullptr;

    return *this;
}

inline TOGL_FineTextElementContainer::~TOGL_FineTextElementContainer() {
    if (m_element) delete m_element;
}
    
inline void TOGL_FineTextElementContainer::SetText(const std::wstring& text) {
    MakeNewIfMismatch(TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT);
    static_cast<TOGL_Text*>(m_element)->SetText(text);
}

inline std::wstring TOGL_FineTextElementContainer::GetText() const {
    if (!IsTypeId(TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT)) {
        return ToDefRawElement().ToText();
    }
    return static_cast<const TOGL_Text*>(m_element)->GetText();
}


inline void TOGL_FineTextElementContainer::SetTextColor(const TOGL_Color4U8& text_color) {
    MakeNewIfMismatch(TOGL_FINE_TEXT_ELEMENT_TYPE_ID_COLOR);
    static_cast<TOGL_TextColor*>(m_element)->SetColor(text_color);
}

inline TOGL_Color4U8 TOGL_FineTextElementContainer::GetTextColor() const {
    if (!IsTypeId(TOGL_FINE_TEXT_ELEMENT_TYPE_ID_COLOR)) {
        return ToDefRawElement().ToTextColor();
    }
    return static_cast<const TOGL_TextColor*>(m_element)->GetColor();
}


inline void TOGL_FineTextElementContainer::SetTextHorizontalSpaceWidth(uint32_t width) {
    MakeNewIfMismatch(TOGL_FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER);
    static_cast<TOGL_TextHorizontalSpacer*>(m_element)->SetWidth(width);
}

inline uint32_t TOGL_FineTextElementContainer::GetTextHorizontalSpaceWidth() const {
    if (!IsTypeId(TOGL_FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER)) {
        return ToDefRawElement().ToTextHorizontalSpaceWidth();
    }
    return static_cast<const TOGL_TextHorizontalSpacer*>(m_element)->GetWidth();
}

inline bool TOGL_FineTextElementContainer::IsTypeId(TOGL_FineTextElementTypeId type_id) const {
    return m_element && m_element->GetTypeId() == type_id;
}

inline TOGL_FineTextElementTypeId TOGL_FineTextElementContainer::GetTypeId() const {
    if (m_element) return m_element->GetTypeId();
    return TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT;
}


inline TOGL_FineTextElement* TOGL_FineTextElementContainer::ToElement() {
    return m_element;
}

inline const TOGL_FineTextElement* TOGL_FineTextElementContainer::ToElement() const {
    return m_element;
}

//------------------------------------------------------------------------------

inline TOGL_FineTextElementContainer::DefRawElement& TOGL_FineTextElementContainer::ToDefRawElement() {
    return TOGL_Global<DefRawElement>::ToObject();
}

inline TOGL_FineTextElement* TOGL_FineTextElementContainer::MakeNew(TOGL_FineTextElementTypeId type_id) {
    switch (type_id) {
    case TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT:               return new TOGL_Text();
    case TOGL_FINE_TEXT_ELEMENT_TYPE_ID_HORIZONTAL_SPACER:  return new TOGL_TextHorizontalSpacer();
    case TOGL_FINE_TEXT_ELEMENT_TYPE_ID_COLOR:              return new TOGL_TextColor();
    }
    return nullptr;
}
    
inline void TOGL_FineTextElementContainer::MakeNewIfMismatch(TOGL_FineTextElementTypeId type_id) {
    if (m_element) {
        if (m_element->GetTypeId() != type_id) {
            delete m_element;
            m_element = MakeNew(type_id);
        }
    } else {
        m_element = MakeNew(type_id);
    }
}

//------------------------------------------------------------------------------
// TOGL_FineText
//------------------------------------------------------------------------------

inline TOGL_FineText::TOGL_FineText() {

}

inline TOGL_FineText::TOGL_FineText(const std::string& text) {
    Append(text);
}

inline TOGL_FineText::TOGL_FineText(const std::wstring& text) {
    Append(text);
}

template <typename Type, typename... Types>
inline TOGL_FineText::TOGL_FineText(const Type& raw_element, const Types&... raw_elements) {
    Append(raw_element, raw_elements...);
}

inline TOGL_FineText::TOGL_FineText(const std::vector<ElementContainer>& element_containers) : m_element_containers(element_containers) {

}

inline TOGL_FineText::~TOGL_FineText() {

}

template <typename Type, typename... Types>
inline void TOGL_FineText::Set(const Type& raw_element, const Types&... raw_elements) {
    Clear();
    Append(raw_element, raw_elements...);
}

inline void TOGL_FineText::Append(const std::string& text) {
    Append(TOGL_ToUTF16(text));
}

inline void TOGL_FineText::Append(const std::wstring& text) {
    auto _TOGL_Min = [](const auto& a, const auto& b) -> decltype(a < b ? a : b) {
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

inline void TOGL_FineText::Append(const TOGL_Color4U8& color) {
    m_element_containers.push_back(ElementContainer(color));
}

inline void TOGL_FineText::Append(uint32_t horizontal_space_width) {
    m_element_containers.push_back(ElementContainer(horizontal_space_width));
}

inline void TOGL_FineText::Append(const ElementContainer& element) {
    m_element_containers.push_back(element);
}

inline void TOGL_FineText::Append(const std::vector<ElementContainer>& element_containers) {
    m_element_containers.insert(m_element_containers.end(), element_containers.begin(), element_containers.end());
}

template <typename Type1, typename Type2, typename... Types>
inline void TOGL_FineText::Append(const Type1& raw_element1, const Type2& raw_element2, const Types&... raw_elements) {
    Append(raw_element1);
    Append(raw_element2, raw_elements...);
}

inline void TOGL_FineText::Clear() {
    m_element_containers.clear();
}

inline std::vector<TOGL_FineTextElementContainer> TOGL_FineText::GetElementContainers() const {
    return m_element_containers;
}

inline std::vector<TOGL_FineTextElementContainer>& TOGL_FineText::ToElementContainers() {
    return m_element_containers;
}

inline const std::vector<TOGL_FineTextElementContainer>& TOGL_FineText::ToElementContainers() const {
    return m_element_containers;
}

//------------------------------------------------------------------------------

inline TOGL_FineText& operator+=(TOGL_FineText& l, const TOGL_FineText& r) {
    l.Append(r.ToElementContainers());
    return l;
}

#endif // TRIVIALOPENGL_FINETEXT_H_
