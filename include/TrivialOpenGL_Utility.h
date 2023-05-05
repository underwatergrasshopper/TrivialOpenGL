/**
* @file TrivialOpenGL_Utility.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_UTILITY_H_
#define TRIVIALOPENGL_UTILITY_H_

#include "TrivialOpenGL_Headers.h"

//==============================================================================
// Declarations
//==============================================================================

#define togl_print_i32(variable) { printf(#variable"=%d\n", int(variable)); fflush(stdout); } (void)0

// Disables copy ability of class.
#define TOGL_NO_COPY(Class) \
    Class(const Class&) = delete; \
    Class& operator=(const Class&) = delete

#define TOGL_CASE_STR(name) case name: return #name

//------------------------------------------------------------------------------
// TOGL_GL_Version
//------------------------------------------------------------------------------

struct TOGL_GL_Version {
    int major;
    int minor;
};

//------------------------------------------------------------------------------
// TOGL_Point
//------------------------------------------------------------------------------

template <typename Type>
struct TOGL_Point {
    Type x;
    Type y;

    TOGL_Point() : x(Type()), y(Type()) {}

    template<typename S_Type>
    explicit TOGL_Point(const S_Type& s) : x(Type(s)), y(Type(s)) {}

    template<typename X_Type, typename Y_Type>
    TOGL_Point(const X_Type& x, const Y_Type& y) : x(Type(x)), y(Type(y)) {}

    template <typename SizeType>
    explicit TOGL_Point(const TOGL_Point<SizeType>& p) : x(Type(p.x)), y(Type(p.y)) {}

    virtual ~TOGL_Point() {}
};

using TOGL_PointI16  = TOGL_Point<int16_t>;
using TOGL_PointI    = TOGL_Point<int32_t>;
using TOGL_PointI64  = TOGL_Point<int64_t>;

using TOGL_PointU16  = TOGL_Point<uint16_t>;
using TOGL_PointU    = TOGL_Point<uint32_t>;
using TOGL_PointU64  = TOGL_Point<uint64_t>;

using TOGL_PointF    = TOGL_Point<float>;
using TOGL_PointD    = TOGL_Point<float>;

template <typename Type>
inline bool operator==(const TOGL_Point<Type>& l, const TOGL_Point<Type>& r) {
    return l.x == r.x && l.y == r.y;
}

template <typename Type>
inline bool operator!=(const TOGL_Point<Type>& l, const TOGL_Point<Type>& r) {
    return l.x != r.x || l.y != r.y;
}

template <typename Type>
inline bool operator>(const TOGL_Point<Type>& l, const TOGL_Point<Type>& r) {
    return l.x > r.x && l.y > r.y;
}

template <typename Type>
inline bool operator<(const TOGL_Point<Type>& l, const TOGL_Point<Type>& r) {
    return l.x < r.x && l.y < r.y;
}

template <typename Type>
inline bool operator>=(const TOGL_Point<Type>& l, const TOGL_Point<Type>& r) {
    return l.x >= r.x && l.y >= r.y;
}

template <typename Type>
inline bool operator<=(const TOGL_Point<Type>& l, const TOGL_Point<Type>& r) {
    return l.x <= r.x && l.y <= r.y;
}


template <typename Type>
inline TOGL_Point<Type> operator+(const TOGL_Point<Type>& l, const TOGL_Point<Type>& r) {
    return {l.x + r.x, l.y + r.y};
}

template <typename Type>
inline TOGL_Point<Type> operator-(const TOGL_Point<Type>& l, const TOGL_Point<Type>& r) {
    return {l.x - r.x, l.y - r.y};
}

template <typename Type>
inline TOGL_Point<Type> operator*(const TOGL_Point<Type>& l, const Type& r) {
    return {l.x * r, l.y * r};
}

template <typename Type>
inline TOGL_Point<Type> operator/(const TOGL_Point<Type>& l, const Type& r) {
    return {l.x / r, l.y / r};
}


template <typename Type>
inline TOGL_Point<Type>& operator+=(TOGL_Point<Type>& l, const TOGL_Point<Type>& r) {
    l = l + r;
    return l;
}
    
template <typename Type>
inline TOGL_Point<Type>& operator-=(TOGL_Point<Type>& l, const TOGL_Point<Type>& r) {
    l = l - r;
    return l;
}
    
template <typename Type>
inline TOGL_Point<Type>& operator*=(TOGL_Point<Type>& l, const Type& r) {
    l = l * r;
    return l;
}
    
template <typename Type>
inline TOGL_Point<Type>& operator/=(TOGL_Point<Type>& l, const Type& r) {
    l = l / r;
    return l;
}

//------------------------------------------------------------------------------
// TOGL_Size
//------------------------------------------------------------------------------

template <typename Type>
struct TOGL_Size {
    Type width;
    Type height;

    TOGL_Size() : width(Type()), height(Type()) {}

    template <typename S_Type>
    explicit TOGL_Size(const S_Type& s) : width(Type(s)), height(Type(s)) {}

    template <typename WidthType, typename HeightType>
    TOGL_Size(const WidthType& width, const HeightType& height) : width(Type(width)), height(Type(height)) {}

    template <typename SizeType>
    explicit TOGL_Size(const TOGL_Size<SizeType>& s) : width(Type(s.width)), height(Type(s.height)) {}

    virtual ~TOGL_Size() {}
};

using TOGL_SizeI16  = TOGL_Size<int16_t>;
using TOGL_SizeI    = TOGL_Size<int32_t>;
using TOGL_SizeI64  = TOGL_Size<int64_t>;

using TOGL_SizeU16  = TOGL_Size<uint16_t>;
using TOGL_SizeU    = TOGL_Size<uint32_t>;
using TOGL_SizeU64  = TOGL_Size<uint64_t>;

using TOGL_SizeF    = TOGL_Size<float>;
using TOGL_SizeD    = TOGL_Size<float>;

template <typename Type>
inline bool operator==(const TOGL_Size<Type>& l, const TOGL_Size<Type>& r) {
    return l.width == r.width && l.height == r.height;
}

template <typename Type>
inline bool operator!=(const TOGL_Size<Type>& l, const TOGL_Size<Type>& r) {
    return l.width != r.width || l.height != r.height;
}

template <typename Type>
inline bool operator>(const TOGL_Size<Type>& l, const TOGL_Size<Type>& r) {
    return l.width > r.width && l.height > r.height;
}

template <typename Type>
inline bool operator<(const TOGL_Size<Type>& l, const TOGL_Size<Type>& r) {
    return l.width < r.width && l.height < r.height;
}

template <typename Type>
inline bool operator>=(const TOGL_Size<Type>& l, const TOGL_Size<Type>& r) {
    return l.width >= r.width && l.height >= r.height;
}

template <typename Type>
inline bool operator<=(const TOGL_Size<Type>& l, const TOGL_Size<Type>& r) {
    return l.width <= r.width && l.height <= r.height;
}


template <typename Type>
inline TOGL_Size<Type> operator+(const TOGL_Size<Type>& l, const TOGL_Size<Type>& r) {
    return {l.width + r.width, l.height + r.height};
}

template <typename Type>
inline TOGL_Size<Type> operator-(const TOGL_Size<Type>& l, const TOGL_Size<Type>& r) {
    return {l.width - r.width, l.height - r.height};
}

template <typename Type>
inline TOGL_Size<Type> operator*(const TOGL_Size<Type>& l, const Type& r) {
    return {l.width * r, l.height * r};
}

template <typename Type>
inline TOGL_Size<Type> operator/(const TOGL_Size<Type>& l, const Type& r) {
    return {l.width / r, l.height / r};
}


template <typename Type>
inline TOGL_Size<Type>& operator+=(TOGL_Size<Type>& l, const TOGL_Size<Type>& r) {
    l = l + r;
    return l;
}

template <typename Type>
inline TOGL_Size<Type>& operator-=(TOGL_Size<Type>& l, const TOGL_Size<Type>& r) {
    l = l - r;
    return l;
}

template <typename Type>
inline TOGL_Size<Type>& operator*=(TOGL_Size<Type>& l, const Type& r) {
    l = l * r;
    return l;
}

template <typename Type>
inline TOGL_Size<Type>& operator/=(TOGL_Size<Type>& l, const Type& r) {
    l = l / r;
    return l;
}

//------------------------------------------------------------------------------
// TOGL_Area
//------------------------------------------------------------------------------

template <typename PointType, typename SizeType = PointType>
struct TOGL_Area {
    PointType  x;
    PointType  y;
    SizeType   width;
    SizeType   height;

    TOGL_Area() : x(PointType()), y(PointType()), width(SizeType()), height(SizeType()) {}

    TOGL_Area(const PointType& x, const PointType& y, const SizeType& width, const SizeType& height) : x(x), y(y), width(width), height(height) {}

    template <typename PointType2, typename SizeType2 = PointType2>
    explicit TOGL_Area(const TOGL_Area<PointType2, SizeType2>& area) : x(PointType(area.x)), y(PointType(area.y)), width(SizeType(area.width)), height(SizeType(area.height)) {}

    TOGL_Area(const TOGL_Point<PointType>& pos, const TOGL_Size<SizeType>& size) : x(pos.x), y(pos.y), width(size.width), height(size.height) {}

    explicit TOGL_Area(const TOGL_Point<PointType>& pos) : x(pos.x), y(pos.y), width(SizeType()), height(SizeType()) {}
    explicit TOGL_Area(const TOGL_Size<SizeType>& size) : x(PointType()), y(PointType()), width(size.width), height(size.height) {}

    void SetPos(const TOGL_Point<PointType>& pos) {  
        x = pos.x;
        y = pos.y;
    }

    void SetSize(const TOGL_Size<SizeType>& size) {  
        width   = size.width;
        height  = size.height;
    }

    TOGL_Point<PointType> GetPos() const { return TOGL_Point<PointType>(x, y); }
    TOGL_Size<SizeType> GetSize() const { return TOGL_Size<SizeType>(width, height); }

    bool IsIn(const TOGL_Point<PointType>& pos) const {
        return TOGL_Point<PointType>(x, y) <= pos && pos < TOGL_Point<PointType>(x + width, y + height);
    }

    virtual ~TOGL_Area() {}
};

using TOGL_AreaI    = TOGL_Area<int32_t>;
using TOGL_AreaI64  = TOGL_Area<int64_t>;

using TOGL_AreaU    = TOGL_Area<uint32_t>;
using TOGL_AreaU64  = TOGL_Area<uint64_t>;

using TOGL_AreaF    = TOGL_Area<float>;
using TOGL_AreaD    = TOGL_Area<float>;

using TOGL_AreaIU   = TOGL_Area<int32_t, uint32_t>;
using TOGL_AreaIU16 = TOGL_Area<int32_t, uint16_t>;

template <typename PointType, typename SizeType = PointType>
inline bool operator==(const TOGL_Area<PointType, SizeType>& l, const TOGL_Area<PointType, SizeType>& r) {
    return l.x == r.x && l.y == r.y && l.width == r.width && l.height == r.height;
}

template <typename PointType, typename SizeType = PointType>
inline bool operator!=(const TOGL_Area<PointType, SizeType>& l, const TOGL_Area<PointType, SizeType>& r) {
    return l.x != r.x || l.y != r.y || l.width != r.width || l.height != r.height;
}

template <typename PointType, typename SizeType = PointType>
inline RECT MakeRECT(const TOGL_Area<PointType, SizeType>& area) {
    return {LONG(area.x), LONG(area.y), LONG(area.x + area.width), LONG(area.y + area.height)};
}

TOGL_AreaI TOGL_MakeArea(const RECT& r);
TOGL_AreaIU TOGL_MakeAreaIU(const RECT& r);
TOGL_AreaIU16 TOGL_MakeAreaIU16(const RECT& r);

//------------------------------------------------------------------------------
// TOGL_Color
//------------------------------------------------------------------------------

template <typename Type>
struct TOGL_Color4 {
    Type r;
    Type g;
    Type b;
    Type a;

    TOGL_Color4() : r(0), g(0), b(0), a(0) {}

    template <typename TypeR, typename TypeG, typename TypeB, typename TypeA>
    TOGL_Color4(const TypeR& r, const TypeG& g, const TypeB& b, const TypeA& a) : r(Type(r)), g(Type(g)), b(Type(b)), a(Type(a)) {}

    Type* ToData() {
        return (Type*)this;
    }

    const Type* ToData() const {
        return (const Type*)this;
    }
};

using TOGL_Color4U8 = TOGL_Color4<uint8_t>;

//------------------------------------------------------------------------------
// TOGL_Global
//------------------------------------------------------------------------------

// Makes global object of specified type in header-only library.
template <typename Type>
class TOGL_Global {
public:
    static Type& To() { return sm_object; }
private:
    static Type sm_object;
};

template <typename Type>
Type TOGL_Global<Type>::sm_object;

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

// Converts ascii string to utf-16 string.
std::wstring TOGL_ASCII_ToUTF16(const std::string& text_ascii);

// Converts utf-8 string to utf-16 string.
std::wstring TOGL_ToUTF16(const std::string& text_utf8);

// Converts utf-16 string to utf-8 string.
std::string TOGL_ToUTF8(const std::wstring& text_utf16);

//------------------------------------------------------------------------------
// Log
//------------------------------------------------------------------------------

enum TOGL_LogMessageTypeId {
    TOGL_LOG_MESSAGE_TYPE_ID_FATAL_ERROR,
    TOGL_LOG_MESSAGE_TYPE_ID_ERROR,
    TOGL_LOG_MESSAGE_TYPE_ID_WARNING,
    TOGL_LOG_MESSAGE_TYPE_ID_INFO,
    TOGL_LOG_MESSAGE_TYPE_ID_DEBUG,
};
using TOGL_CustomLogFnP_T = void (*)(TOGL_LogMessageTypeId message_type, const char* message);

using TOGL_LogLevel = uint32_t;
enum : uint32_t {
    TOGL_LOG_LEVEL_ERROR         = 0,    // log errors
    TOGL_LOG_LEVEL_WARNING       = 1,    // log errors, warnings
    TOGL_LOG_LEVEL_INFO          = 2,    // log errors, warnings, infos
    TOGL_LOG_LEVEL_DEBUG         = 3,    // log errors, warnings, infos, debugs
};                                 

// Sets current log level.
void TOGL_SetLogLevel(TOGL_LogLevel log_level);
TOGL_LogLevel TOGL_GetLogLevel();

// Returns true if log_level is less or equal to current log level.
bool TOGL_IsLogLevelAtLeast(TOGL_LogLevel log_level);

// Logs message to standard output (by default) or redirect to custom function (provided by SetHandleLogFunction).
// message      - Message in ascii encoding.

void TOGL_LogDebug(const std::string& message);
void TOGL_LogInfo(const std::string& message);
void TOGL_LogWarning(const std::string& message);
void TOGL_LogError(const std::string& message);
void TOGL_LogFatalError(const std::string& message);

// Sets callback function, which will be called each time when any TOGL_Log{...} function is called, 
// and log massage will be redirected to callback function instead being processed by called TOGL_Log{...} function.
void TOGL_SetCustomLogFunction(TOGL_CustomLogFnP_T custom_log);

// ---

// Gives access to logger singleton.
class TOGL_Logger;
inline TOGL_Logger& TOGL_ToLogger();

// Singleton.
class TOGL_Logger {
public:
    friend TOGL_Global<TOGL_Logger>;

    virtual ~TOGL_Logger();

    // ---

    // message      - Message in ascii encoding.

    void LogDebug(const std::string& message);
    void LogInfo(const std::string& message);
    void LogWarning(const std::string& message);
    void LogError(const std::string& message);

    // This function exits executable with error code EXIT_FAILURE.
    void LogFatalError(const std::string& message);

    // ---

    // Sets current log level.
    void SetLogLevel(TOGL_LogLevel log_level);
    TOGL_LogLevel GetLogLevel() const;

    // Returns true if log_level is less or equal to current log level.
    bool IsLogLevelAtLeast(TOGL_LogLevel log_level);

    // ---

    // Sets callback function, which will be called each time when any Log{...} method is called, 
    // and log massage will be redirected to callback function instead being processed by called Log{...} method.
    void SetCustomLogFunction(TOGL_CustomLogFnP_T custom_log);

private:
    TOGL_Logger();

    static std::string GetMessagePrefix(TOGL_LogMessageTypeId log_message_type);
    void Log(TOGL_LogMessageTypeId message_type, const std::string& message);
    void LogTextToStdOut(const std::string& message);
        
    TOGL_LogLevel        m_log_level;
    TOGL_CustomLogFnP_T  m_custom_log;
};

//------------------------------------------------------------------------------
// BMP
//------------------------------------------------------------------------------

// Saves image pixel data to file as BMP image.
// file_name        - Full name with extension of BMP file. Variable encoding format: UTF8.
// pixel_data       - Contains array of pixels. Each pixels contains for channel (in order: red, green, blue, alpha). 
//                    Each channel occupies one byte (value range: 0 - 255).
// width            - Length of single image row in pixels. And also, width of saved image. At least 1. 
// height           - Number of rows. And also, height of saved image. At least 1. 
// Returns true if image was saved to file.
bool TOGL_SaveAsBMP(const std::string& file_name, const uint8_t* pixel_data, uint32_t width, uint32_t height, bool is_reverse_rows = true);

// Saves content of OpenGL texture to file as BMP image.
// file_name        - Full name with extension of BMP file. Variable encoding format: UTF8.
// tex_obj          - OpneGL Texture Object (Name).
// Returns true if image was saved to file.
bool TOGL_SaveTextureAsBMP(const std::string& file_name, GLuint tex_obj);

//------------------------------------------------------------------------------
// Common
//------------------------------------------------------------------------------

TOGL_AreaIU16 TOGL_GetDesktopAreaNoTaskBar();
TOGL_SizeU16 TOGL_GetDesktopAreaSizeNoTaskBar();
TOGL_SizeU16 TOGL_GetScreenSize();
TOGL_PointI TOGL_GetCursorPosInScreen();

//------------------------------------------------------------------------------

std::string TOGL_HexToStr(uint16_t value);
std::vector<std::string> TOGL_Split(const std::string& text, char c);

//==============================================================================
// Definitions
//==============================================================================

//------------------------------------------------------------------------------
// TOGL_Area
//------------------------------------------------------------------------------

inline TOGL_AreaI TOGL_MakeArea(const RECT& r) {
    return {
        r.left,
        r.top,
        r.right - r.left,
        r.bottom - r.top
    };
}

inline TOGL_AreaIU TOGL_MakeAreaIU(const RECT& r) {
    return TOGL_AreaIU(
        r.left,
        r.top,
        r.right - r.left,
        r.bottom - r.top
    );
}

inline TOGL_AreaIU16 TOGL_MakeAreaIU16(const RECT& r) {
    return TOGL_AreaIU16(
        r.left,
        r.top,
        uint16_t(r.right - r.left),
        uint16_t(r.bottom - r.top)
    );
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

enum { _TOGL_CONVERSION_STACK_BUFFER_LENGTH = 4096 };

inline std::wstring TOGL_ASCII_ToUTF16(const std::string& text_ascii) {
    std::wstring text_utf16;
    for (size_t index = 0; index < text_ascii.length(); ++index) text_utf16 += wchar_t(text_ascii[index]);
    return text_utf16;
}

inline std::wstring TOGL_ToUTF16(const std::string& text_utf8) {
    std::wstring text_utf16;

    wchar_t stack_buffer[_TOGL_CONVERSION_STACK_BUFFER_LENGTH] = {};

    if (!text_utf8.empty()) {
        // number of characters with '\0'
        int number = MultiByteToWideChar(CP_UTF8, 0, text_utf8.c_str(), -1, NULL, 0);
        if (number == 0) {
            TOGL_LogFatalError("Can not convert a text from utf-16 to utf-8.");
        }

        wchar_t* buffer = (number > _TOGL_CONVERSION_STACK_BUFFER_LENGTH) ? (new wchar_t[number]) : stack_buffer;

        number = MultiByteToWideChar(CP_UTF8, 0, text_utf8.c_str(), -1, buffer, number);
        if (number == 0) {
            TOGL_LogFatalError("Can not convert a text from utf-16 to utf-8.");
        }

        if (number > 1) text_utf16 = std::wstring(buffer, number - 1);

        if (buffer != stack_buffer) delete[] buffer;
    }

    return text_utf16;
}

inline std::string TOGL_ToUTF8(const std::wstring& text_utf16) {
    std::string text_utf8;

    char stack_buffer[_TOGL_CONVERSION_STACK_BUFFER_LENGTH] = {};

    if (!text_utf16.empty()) {
        // number of characters with '\0'
        int number = WideCharToMultiByte(CP_UTF8, 0, text_utf16.c_str(), -1, NULL, 0, NULL, NULL);
        if (number == 0) {
            TOGL_LogFatalError("Can not convert a text from utf-8 to utf-16.");
        }

        char* buffer = (number > _TOGL_CONVERSION_STACK_BUFFER_LENGTH) ? (new char[number]) : stack_buffer;

        number = WideCharToMultiByte(CP_UTF8, 0, text_utf16.c_str(), -1, buffer, number, NULL, NULL);
        if (number == 0) {
            TOGL_LogFatalError("Can not convert a text from utf-8 to utf-16.");
        }

        if (number > 1) text_utf8 = std::string(buffer, number - 1);

        if (buffer != stack_buffer) delete[] buffer;
    }

    return text_utf8;
}

//------------------------------------------------------------------------------
// Log
//------------------------------------------------------------------------------

inline void TOGL_SetLogLevel(TOGL_LogLevel log_level) {
    TOGL_ToLogger().SetLogLevel(log_level);
}

inline TOGL_LogLevel TOGL_GetLogLevel() {
    return TOGL_ToLogger().GetLogLevel();
}

inline bool TOGL_IsLogLevelAtLeast(TOGL_LogLevel log_level) {
    return TOGL_ToLogger().IsLogLevelAtLeast(log_level);
}

inline void TOGL_LogDebug(const std::string& message) {
    TOGL_ToLogger().LogDebug(message);
}

inline void TOGL_LogInfo(const std::string& message) {
    TOGL_ToLogger().LogInfo(message);
}

inline void TOGL_LogWarning(const std::string& message) {
    TOGL_ToLogger().LogWarning(message);
}

inline void TOGL_LogError(const std::string& message) {
    TOGL_ToLogger().LogError(message);
}

inline void TOGL_LogFatalError(const std::string& message) {
    TOGL_ToLogger().LogFatalError(message);
}

inline void TOGL_SetCustomLogFunction(TOGL_CustomLogFnP_T custom_log) {
    TOGL_ToLogger().SetCustomLogFunction(custom_log);
}

inline TOGL_Logger& TOGL_ToLogger() {
    return TOGL_Global<TOGL_Logger>::To();
}

// ---

inline TOGL_Logger::~TOGL_Logger() {

}

inline void TOGL_Logger::LogDebug(const std::string& message) {
    if (IsLogLevelAtLeast(TOGL_LOG_LEVEL_DEBUG)) Log(TOGL_LOG_MESSAGE_TYPE_ID_DEBUG, message);
}

inline void TOGL_Logger::LogInfo(const std::string& message) {
    if (IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) Log(TOGL_LOG_MESSAGE_TYPE_ID_INFO, message);
}

inline void TOGL_Logger::LogWarning(const std::string& message) {
    if (IsLogLevelAtLeast(TOGL_LOG_LEVEL_WARNING)) Log(TOGL_LOG_MESSAGE_TYPE_ID_WARNING, message);
}

inline void TOGL_Logger::LogError(const std::string& message) {
    Log(TOGL_LOG_MESSAGE_TYPE_ID_ERROR, message);
}

inline void TOGL_Logger::LogFatalError(const std::string& message) {
    Log(TOGL_LOG_MESSAGE_TYPE_ID_FATAL_ERROR, message);
}

inline void TOGL_Logger::SetLogLevel(TOGL_LogLevel log_level) {
    m_log_level = log_level;
}

inline TOGL_LogLevel TOGL_Logger::GetLogLevel() const {
    return m_log_level;
}

inline bool TOGL_Logger::IsLogLevelAtLeast(TOGL_LogLevel log_level) {
    return log_level <= m_log_level;
}

inline void TOGL_Logger::SetCustomLogFunction(TOGL_CustomLogFnP_T custom_log) {
    m_custom_log = custom_log;
}

// ---

inline TOGL_Logger::TOGL_Logger() {
    m_log_level     = TOGL_LOG_LEVEL_INFO;
    m_custom_log    = nullptr;
}

inline std::string TOGL_Logger::GetMessagePrefix(TOGL_LogMessageTypeId log_message_type) {
    switch (log_message_type) {
    case TOGL_LOG_MESSAGE_TYPE_ID_FATAL_ERROR:  return "(TOGL) Fatal Error: ";
    case TOGL_LOG_MESSAGE_TYPE_ID_ERROR:        return "(TOGL) Error: ";
    case TOGL_LOG_MESSAGE_TYPE_ID_WARNING:      return "(TOGL) Warning: ";
    case TOGL_LOG_MESSAGE_TYPE_ID_INFO:         return "(TOGL) Info: ";
    case TOGL_LOG_MESSAGE_TYPE_ID_DEBUG:        return "(TOGL) Debug: ";
    }
    return "";
}

inline void TOGL_Logger::Log(TOGL_LogMessageTypeId message_type, const std::string& message) {
    if (m_custom_log) {
        m_custom_log(message_type, message.c_str());
    } else {
        LogTextToStdOut(GetMessagePrefix(message_type) + message);
    }
    if (message_type == TOGL_LOG_MESSAGE_TYPE_ID_FATAL_ERROR) exit(EXIT_FAILURE);
}

inline void TOGL_Logger::LogTextToStdOut(const std::string& message) {
    if (fwide(stdout, 0) > 0) {
        wprintf(L"%ls\n", TOGL_ASCII_ToUTF16(message).c_str());
    } else {
        printf("%s\n", message.c_str());
    }
    fflush(stdout);
}

//------------------------------------------------------------------------------
// BMP
//------------------------------------------------------------------------------

inline bool TOGL_SaveAsBMP(const std::string& file_name, const uint8_t* pixel_data, uint32_t width, uint32_t height, bool is_reverse_rows) {
    bool is_success = false; 

    if (width > 0 && height > 0) {
        enum {
            PIXEL_SIZE = 4, // in bytes
        };

        const uint32_t pixel_data_size      = width * height * PIXEL_SIZE;
        const uint32_t header_size          = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPV5HEADER);
        const uint32_t file_size            = header_size + pixel_data_size;

        uint8_t* file_data = new uint8_t[file_size];
        memset(file_data, 0, file_size);

        BITMAPFILEHEADER* fh = (BITMAPFILEHEADER*) file_data;
        fh->bfType          = 0x4D42; // bmp file signature = {'B', 'M'}
        fh->bfSize          = file_size;
        fh->bfOffBits       = header_size;

        BITMAPV5HEADER* ih = (BITMAPV5HEADER*)(file_data + sizeof(BITMAPFILEHEADER));
        ih->bV5Size         = sizeof(BITMAPV5HEADER);
        ih->bV5Width        = width;
        ih->bV5Height       = height;
        ih->bV5Planes       = 1;
        ih->bV5BitCount     = 32;
        ih->bV5Compression  = BI_BITFIELDS;
        ih->bV5SizeImage    = pixel_data_size;
        ih->bV5RedMask      = 0xFF000000;
        ih->bV5GreenMask    = 0x00FF0000;
        ih->bV5BlueMask     = 0x0000FF00;
        ih->bV5AlphaMask    = 0x000000FF;
        ih->bV5CSType       = LCS_sRGB;
        ih->bV5Intent       = LCS_GM_GRAPHICS;

        uint8_t* file_pixel_data = (uint8_t*)(file_data + header_size);
        uint32_t ix = 0;

        const uint32_t row_size = width * PIXEL_SIZE; // in bytes

        for (uint32_t column_ix = 0; column_ix < height; ++column_ix) {
            const uint8_t* column =  pixel_data + row_size * (is_reverse_rows ? (height - 1 - column_ix) : column_ix);

            for (uint32_t pixel_ix = 0; pixel_ix < width; ++pixel_ix) {
                const uint8_t* pixel = column + pixel_ix * PIXEL_SIZE;

                // reverse order of channels
                file_pixel_data[ix++] = pixel[3];
                file_pixel_data[ix++] = pixel[2];
                file_pixel_data[ix++] = pixel[1];
                file_pixel_data[ix++] = pixel[0];
            }
        }

        FILE* file = NULL;
        is_success = _wfopen_s(&file, TOGL_ToUTF16(file_name).c_str(), L"wb") == 0 && file;

        if (is_success) {
            is_success = fwrite(file_data, 1, file_size, file) == file_size;
            fclose(file);
        }

        delete[] file_data;
    }

    return is_success;
}

inline bool TOGL_SaveTextureAsBMP(const std::string& file_name, GLuint tex_obj) {
    bool is_success = false;

    glPushAttrib(GL_TEXTURE_BIT);
    glBindTexture(GL_TEXTURE_2D, tex_obj);

    GLint width     = 0;
    GLint height    = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    if (width > 0 && height > 0) {
        enum { PIXEL_SIZE = 4 }; // in bytes

        const uint32_t  data_size   = width * height * PIXEL_SIZE;
        uint8_t*        data        = new uint8_t[data_size];
                   
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        is_success = TOGL_SaveAsBMP(file_name, data, width, height, false);
                    
        delete[] data;
    }

    glPopAttrib();

    return is_success;
}

//------------------------------------------------------------------------------
// Common
//------------------------------------------------------------------------------

inline TOGL_AreaIU16 TOGL_GetDesktopAreaNoTaskBar() {
    RECT rc;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
    return TOGL_MakeAreaIU16(rc);
}

inline TOGL_SizeU16 TOGL_GetDesktopAreaSizeNoTaskBar() {
    return TOGL_GetDesktopAreaNoTaskBar().GetSize();
}

inline TOGL_SizeU16 TOGL_GetScreenSize() {
    return TOGL_SizeU16(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

inline TOGL_PointI TOGL_GetCursorPosInScreen() {
    POINT pos;
    if (GetCursorPos(&pos)) {
        return {pos.x, pos.y};
    }
    return {};
}

//------------------------------------------------------------------------------

inline std::vector<std::string> TOGL_Split(const std::string& text, char c) {
    std::string temp = text;
    std::vector<std::string> list;

    while (true) {
        size_t pos = temp.find(c, 0);

        if (pos == std::string::npos) {
            list.push_back(temp);
            break;
        } else {
            list.push_back(temp.substr(0, pos));
            temp = temp.substr(pos + 1);
        }
    }
    return list;
}

inline std::string TOGL_HexToStr(uint16_t value) {
    std::stringstream stream;
    stream << std::hex << std::setfill('0') << std::setw(4) << std::right << std::uppercase << value;
    return stream.str();
};

#endif // TRIVIALOPENGL_UTILITY_H_
