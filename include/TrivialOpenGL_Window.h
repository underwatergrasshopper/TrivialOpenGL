/**
* @file TrivialOpenGL_Window.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_WINDOW_H_
#define TRIVIALOPENGL_WINDOW_H_

#include "TrivialOpenGL_Headers.h"
#include "TrivialOpenGL_Utility.h"
#include "TrivialOpenGL_Key.h"

//==============================================================================
// Declarations
//==============================================================================

using TOGL_WindowStyleBitfield = uint32_t;
enum : uint32_t {
    TOGL_WINDOW_STYLE_BIT_NO_RESIZE                     = 0x0001,
    TOGL_WINDOW_STYLE_BIT_NO_MAXIMIZE                   = 0x0002,
    TOGL_WINDOW_STYLE_BIT_CENTERED                      = 0x0004,
    TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE                = 0x0008,
    TOGL_WINDOW_STYLE_BIT_DRAW_AREA_ONLY                = 0x0010,
    TOGL_WINDOW_STYLE_BIT_REDRAW_ON_CHANGE_OR_REQUEST   = 0x0020,
};

enum TOGL_WindowStateId {
    TOGL_WINDOW_STATE_ID_NORMAL,
    TOGL_WINDOW_STATE_ID_MAXIMIZED,
    TOGL_WINDOW_STATE_ID_MINIMIZED,
    TOGL_WINDOW_STATE_ID_WINDOWED_FULL_SCREENED,
};

struct TOGL_SpecialDebug {
    bool is_notify_any_message          = false;
    bool is_notify_draw_call            = false;
    bool is_notify_mouse_move           = false;
    bool is_notify_key_message          = false;
    bool is_notify_character_message    = false;
};

enum TOGL_WindowOptionId {
    TOGL_WINDOW_OPTION_ID_AUTO_SLEEP_MODE,
};

// Data used to run window.
struct TOGL_Data {
    // Encoding: ASCII or UTF8.
    std::string                 window_name             = "Window";

    // Window area.
    // If area is equal to {0, 0, 0, 0} then created window will be centered in screen working area (desktop area excluding task bar) 
    // with width equal to half of screen working area width and height equal to half of screen working area height. 
    // Actual window width or height might be higher, depends on operating system minimal window size.
    TOGL_AreaIU16               area                    = {0, 0, 0, 0};

    // NO_RESIZE                      - Window wont have resize frame.
    // NO_MAXIMIZE                    - Window wont have maximize button.
    // CENTERED                       - Window will be centered at middle of working area (desktop area excluding task bar area).
    // DRAW_AREA_SIZE                 - Width and height from 'area' variable will set size of window draw area instead whole window area.
    // DRAW_AREA_ONLY                 - Window will contain only draw area (without borders and title bar).
    // REDRAW_ON_CHANGE_OR_REQUEST    - Window will only be redrawing on change or call of Window::RequestDraw method.
    TOGL_WindowStyleBitfield    style                   = 0;

    // Tries create OpenGL Rendering Context which support to at least this version, with compatibility to all previous versions.
    // If opengl_version.major and opengl_version.minor is DEF then creates for any available OpenGL version. Can be checked by GetOpenGL_Version().
    TOGL_GL_Version             opengl_verion           = {0, 0};

    // File name of icon image file (.ico). 
    // Loaded icon will be presented on window title bar and on task bar.

    // If not empty string, then will load icon image (.ico). 
    // Will be visible on window title bar and task bar.
    // Encoding: ASCII or UTF8.
    std::string                 icon_file_name          = "";

    // (Optional) Resource id of icon file. 
    // Id must be different than 0 and icon_file_name must be an empty string.
    // Loaded icon will be presented on window title bar and at application button on task bar. 

    // Resource identifier.
    // If not 0, then will use icon image from resources identified by this variable as application icon, title bar icon and task bar icon.
    uint16_t                    icon_resource_id        = 0;       


    // If not zero then this is a time interval for which do_on_time(elapsed_time) callback function will be called. 
    uint32_t                    timer_time_interval     = 0; // in milliseconds

    // true - disables auto entering to screen save mode and to any power save mode.
    bool                        is_auto_sleep_blocked   = false;

    // LOG_LEVEL_ERROR         - error messages only
    // LOG_LEVEL_WARNING        - warning and error messages
    // LOG_LEVEL_INFO          - info, warning and error messages
    // LOG_LEVEL_DEBUG         - debug, info, warning and error messages
    TOGL_LogLevel               log_level               = TOGL_LOG_LEVEL_INFO;

    // To enable special debugging messages.
    TOGL_SpecialDebug           special_debug           = {}; // Warning!!! Can slowdown application significantly.   

    // Called right after window is created.
    void (*do_on_create)()                                              = nullptr;

    // Called right before window is closed.
    void (*do_on_destroy)()                                             = nullptr;

    // Called each time when window content needs to be redrawn.
    void (*draw)()                                                      = nullptr;

    // Called when window receive keyboard key or mouse button message.
    // is_down          - true when key is down, false when key is down.
    // extra            - Contains additional informations, like: 
    //                    cursor position in draw area (extra.x, extra.y), 
    //                    indicator if pressed key is left right or doesn't matter (extra.keyboard_side).
    void (*do_on_key)(TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra)   = nullptr;

    // code     - code unit of utf8 character
    void (*do_on_char)(char code)                                       = nullptr;

    // code     - code unit of utf16 character
    void (*do_on_char_utf16)(wchar_t code)                              = nullptr;

    // code     - code unit of utf32 character
    void (*do_on_char_utf32)(int code)                                  = nullptr;

    // Called each time window resize.
    void (*do_on_resize)(uint16_t width, uint16_t height)               = nullptr;

    // step_count       - Number of wheel rotation steps away from user.
    // x, y             - Cursor position in draw area.
    void (*do_on_mouse_wheel_roll)(int step_count, int x, int y)        = nullptr;

    // Called when cursor change position in draw area.
    // x, y             - Cursor position in draw area.
    void (*do_on_mouse_move)(int x, int y)                              = nullptr;

    // Called each time when window state change between:
    // NORMAL       - when restored from any other state by using Window::SetArea, Window::SetSize, Window::Center,
    // MAXIMIZED,
    // MINIMIZED,
    // WINDOWED_FULL_SCREENED.
    void (*do_on_state_change)(TOGL_WindowStateId window_state)         = nullptr;

    void (*do_on_show)()                                                = nullptr;
    void (*do_on_hide)()                                                = nullptr;

    void (*do_on_foreground)(bool is_gain)                              = nullptr;   

    // time_interval - in milliseconds
    void (*do_on_time)(uint32_t time_interval)                          = nullptr;
};

//------------------------------------------------------------------------------

// Creates and runs window.
int TOGL_Run(const TOGL_Data& data);

// If called from inside of do_on_{...} function, then close window after exiting from current do_on_{...} function.
void TOGL_RequestClose();

// If called from inside of do_on_{...} function, then redraws window after exiting from current do_on_{...} function.
void TOGL_RequestRedraw();

// ---
        
// WINDOW_OPTION_AUTO_SLEEP_MODE     - true - if window at foreground then can automatically enter screen saver mode or any sleep mode,
//                                     false - if window at foreground then can not automatically enter screen saver mode or any sleep mode.
void TOGL_SetOption(TOGL_WindowOptionId window_option, bool is_enabled);
bool TOGL_IsEnabled(TOGL_WindowOptionId window_option);

// ---

// Moves window to position in screen coordinates.
void TOGL_MoveTo(int x, int y, bool is_draw_area = false);
void TOGL_MoveTo(const TOGL_PointI& pos, bool is_draw_area = false);

// Moves from current position by offset (x, y).
void TOGL_MoveBy(int x, int y);
void TOGL_MoveBy(const TOGL_PointI& offset);

// Resizes window and keeps current window position.
void TOGL_Resize(uint16_t width, uint16_t height, bool is_draw_area = false);
void TOGL_Resize(const TOGL_SizeU16& size, bool is_draw_area = false);

// Moves and resizes window area.
void TOGL_SetArea(int x, int y, uint16_t width, uint16_t height, bool is_draw_area = false);
void TOGL_SetArea(const TOGL_AreaIU16& area, bool is_draw_area = false);

// ---
// 
// Puts window in center of working area. 
// Where working area is desktop area excluding task bar area.
// If STYLE_BIT_DRAW_AREA_SIZE then width and height correspond to draw area.
void TOGL_Center(uint16_t width, uint16_t height);
void TOGL_Center(const TOGL_SizeU16& size);

void TOGL_Center(uint16_t width, uint16_t height, bool is_draw_area_size);
void TOGL_Center(const TOGL_SizeU16& size, bool is_draw_area_size);

// ---
        
// Returns window left-top corner position in screen coordinates.
TOGL_PointI TOGL_GetPos();
        
// Returns window size.
TOGL_SizeU16 TOGL_GetSize();
        
// Returns window area in screen coordinates.
TOGL_AreaIU16 TOGL_GetArea();
        
// Returns draw area top-left corner position in screen coordinates.
TOGL_PointI TOGL_GetDrawAreaPos();
        
// Returns draw area size.
TOGL_SizeU16 TOGL_GetDrawAreaSize();
        
// Returns draw area in screen coordinates.
TOGL_AreaIU16 TOGL_GetDrawArea();

// ---

void TOGL_Hide();
void TOGL_Show();
bool TOGL_IsVisible();

// ---

inline void TOGL_Minimize();
inline void TOGL_Maximize();
inline void TOGL_GoWindowedFullScreen();

inline TOGL_WindowStateId TOGL_GetState();

inline bool TOGL_IsNormal();
inline bool TOGL_IsMinimized();
inline bool TOGL_IsMaximized();
inline bool TOGL_IsFullScreen();

// ---

// Moves window to foreground.
inline void TOGL_GoForeground();
inline bool TOGL_IsForeground();

// ---
// 
// Returns window style.
inline TOGL_WindowStyleBitfield TOGL_GetStyle();

inline TOGL_PointI TOGL_GetCursorPosInDrawArea();

inline TOGL_GL_Version TOGL_GetOpenGL_Version();

// ---

// Gives access to window singleton.
class TOGL_Window;
TOGL_Window& TOGL_ToWindow();

//------------------------------------------------------------------------------
// TOGL_Window
//------------------------------------------------------------------------------

// For inner purpose only.
class _TOGL_WindowInnerAccessor {
public:
    friend class TOGL_FontDataGenerator;
protected:
    virtual ~_TOGL_WindowInnerAccessor() {}
private:
    virtual HWND ToHWND() = 0;
};


// Singleton.
class TOGL_Window : public _TOGL_WindowInnerAccessor {
public:
    friend TOGL_Global<TOGL_Window>;

    virtual ~TOGL_Window();

    // ---

    // Creates and runs window.
    int Run(const TOGL_Data& data);

    // If called from inside of do_on_{...} function, then close window after exiting from current do_on_{...} function.
    void RequestClose();

    // If called from inside of do_on_{...} function, then redraws window after exiting from current do_on_{...} function.
    void RequestRedraw();

    // ---
        
    // WINDOW_OPTION_AUTO_SLEEP_MODE     - true - if window at foreground then can automatically enter screen saver mode or any sleep mode,
    //                                     false - if window at foreground then can not automatically enter screen saver mode or any sleep mode.
    void SetOption(TOGL_WindowOptionId window_option, bool is_enabled);
    bool IsEnabled(TOGL_WindowOptionId window_option) const;

    // ---

    // Moves window to position in screen coordinates.
    void MoveTo(int x, int y, bool is_draw_area = false);
    void MoveTo(const TOGL_PointI& pos, bool is_draw_area = false);

    // Moves from current position by offset (x, y).
    void MoveBy(int x, int y);
    void MoveBy(const TOGL_PointI& offset);

    // Resizes window and keeps current window position.
    void Resize(uint16_t width, uint16_t height, bool is_draw_area = false);
    void Resize(const TOGL_SizeU16& size, bool is_draw_area = false);

    // Moves and resizes window area.
    void SetArea(int x, int y, uint16_t width, uint16_t height, bool is_draw_area = false);
    void SetArea(const TOGL_AreaIU16& area, bool is_draw_area = false);

    // Puts window in center of desktop area excluding task bar area.

    // If STYLE_BIT_DRAW_AREA_SIZE then width and height correspond to draw area.
    void Center(uint16_t width, uint16_t height);
    void Center(const TOGL_SizeU16& size);

    void Center(uint16_t width, uint16_t height, bool is_draw_area_size);
    void Center(const TOGL_SizeU16& size, bool is_draw_area_size);

    // ---
        
    // Returns window left-top corner position in screen coordinates.
    TOGL_PointI GetPos() const;
        
    // Returns window size.
    TOGL_SizeU16 GetSize() const;
        
    // Returns window area in screen coordinates.
    TOGL_AreaIU16 GetArea() const;
        
    // Returns draw area top-left corner position in screen coordinates.
    TOGL_PointI GetDrawAreaPos() const;
        
    // Returns draw area size.
    TOGL_SizeU16 GetDrawAreaSize() const;
        
    // Returns draw area in screen coordinates.
    TOGL_AreaIU16 GetDrawArea() const;

    // ---

    void Hide();
    void Show();
    bool IsVisible() const;

    // ---

    void Minimize();
    void Maximize();
    void GoWindowedFullScreen();

    TOGL_WindowStateId GetState() const;

    bool IsNormal() const;
    bool IsWindowMinimized() const;
    bool IsWindowMaximized() const;
    bool IsWindowedFullScreen() const;

    // Moves window to foreground.
    void GoForeground();
    bool IsForeground() const;

    // ---

    // Returns window style.
    TOGL_WindowStyleBitfield GetStyle() const;

    TOGL_PointI GetCursorPosInDrawArea() const;

    TOGL_GL_Version GetOpenGL_Version() const;

private:
    enum AreaPartId {
        TOGL_AREA_PART_ID_POSITION,
        TOGL_AREA_PART_ID_POSITION_OFFSET,
        TOGL_AREA_PART_ID_SIZE,
        TOGL_AREA_PART_ID_ALL
    };

    // Window client area width extension to force functional windowed full screen window (reduced flashing and be able alt+tab in Windows 7).
    enum {
        TOGL_WIDTH_CORRECTION_TO_FAKE        = 1,
        TOGL_DEFAULT_TIMER_ID                = 1,
        TOGL_MAX_NUM_OF_UTF16_CODE_UNITS     = 2,
    };

    // Corrects window position and size to remove invisible window frame in Windows 10. 
    class WindowAreaCorrector {
    public:
        WindowAreaCorrector();

        virtual ~WindowAreaCorrector();

        TOGL_AreaIU16 Get(HWND window_handle) const;

        // area         - Window area without invisible frame.
        TOGL_AreaIU16 AddInvisibleFrameTo(const TOGL_AreaIU16& area, HWND window_hangle) const;

        // size         - Window size without invisible frame.
        TOGL_SizeU16 AddInvisibleFrameTo(const TOGL_SizeU16& size, HWND window_hangle) const;

        // pos          - Window position without invisible frame.
        TOGL_PointI AddInvisibleFrameTo(const TOGL_PointI& pos, HWND window_hangle) const;

        // area         - Window area with invisible frame.
        TOGL_AreaIU16 RemoveInvisibleFrameFrom(const TOGL_AreaIU16& area, HWND window_hangle) const;

        // size         - Window size with invisible frame.
        TOGL_SizeU16 RemoveInvisibleFrameFrom(const TOGL_SizeU16& size, HWND window_hangle) const;

        // pos          - Window position with invisible frame.
        TOGL_PointI RemoveInvisibleFrameFrom(const TOGL_PointI& pos, HWND window_hangle) const;
    private:
        HMODULE  m_dwmapi_lib_handle;
        HRESULT (WINAPI *m_dwm_get_window_attribute)(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute);
    };

    TOGL_Window();
    TOGL_NO_COPY(TOGL_Window);

    HWND ToHWND() override final;

    void SetState(TOGL_WindowStateId state);

    void Push(bool& value, bool new_value_after_push);
    void Pop(bool& value);

    // Changes area by applying style from data parameter which was provided to Run function.
    void ChangeArea(const TOGL_AreaIU16& area);

    HICON TryLoadIcon();
    int ExecuteMainLoop();

    void Draw();

    TOGL_AreaIU16 GenerateWindowArea(const TOGL_AreaIU16& area);
    void Restore();
    void SetArea(const TOGL_AreaIU16& area, AreaPartId area_part_id, bool is_client_area);

    void Create(HWND window_handle);
    void Destroy();

    static DWORD GetWindowStyle_DrawAreaOnly();
    static DWORD GetWindowExtendedStyle_DrawAreaOnly();

    static TOGL_AreaIU16 GetWindowArea(HWND window_handle);
    static TOGL_AreaIU16 GetWindowAreaFromDrawArea(const TOGL_AreaIU16& draw_area, DWORD window_style);

    void HandleDoOnKeyboardKey(WPARAM w_param, LPARAM l_param);
    void HandleDoOnMouseKey(UINT message, WPARAM w_param, LPARAM l_param);

    template <unsigned NUNMBER_OF_CODE_UNITS>
    void HandleUTF16_ToUTF8(const wchar_t (&char_utf16)[NUNMBER_OF_CODE_UNITS]);

    LRESULT InnerWindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);
    static LRESULT CALLBACK WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);
    static std::string WM_ToStr(UINT message);

    TOGL_Data               m_data;

    HINSTANCE               m_instance_handle;
    HWND                    m_window_handle;
    HDC                     m_device_context_handle;
    HGLRC                   m_rendering_context_handle;

    DWORD                   m_window_style;
    DWORD                   m_window_extended_style;

    bool                    m_is_active;
    bool                    m_is_visible;
    bool                    m_is_frame;

    TOGL_WindowStateId      m_state;
    TOGL_WindowStateId      m_prev_state;

    bool                    m_is_win7;

    bool                    m_is_apply_fake_width;
    bool                    m_is_enable_do_on_resize;
    bool                    m_is_enable_change_state_at_resize;

    uint64_t                m_dbg_message_id;

    WindowAreaCorrector     m_window_area_corrector;

    std::map<bool*, std::stack<bool>>   m_on_off_stack_map;

    wchar_t                             m_char_utf16[TOGL_MAX_NUM_OF_UTF16_CODE_UNITS];
};

//==============================================================================
// Definitions
//==============================================================================

inline int TOGL_Run(const TOGL_Data& data) {
    return TOGL_ToWindow().Run(data);
}

inline void TOGL_RequestClose() {
    TOGL_ToWindow().RequestClose();
}

inline void TOGL_RequestRedraw() {
    TOGL_ToWindow().RequestRedraw();
}

// ---

inline void TOGL_SetOption(TOGL_WindowOptionId window_option, bool is_enabled) {
    TOGL_ToWindow().SetOption(window_option, is_enabled);
}
inline bool TOGL_IsEnabled(TOGL_WindowOptionId window_option) {
    return TOGL_ToWindow().IsEnabled(window_option);
}

// ---

inline void TOGL_MoveTo(int x, int y, bool is_draw_area) {
    TOGL_ToWindow().MoveTo(x, y, is_draw_area);
}
inline void TOGL_MoveTo(const TOGL_PointI& pos, bool is_draw_area) {
    TOGL_ToWindow().MoveTo(pos, is_draw_area);
}

inline void TOGL_MoveBy(int x, int y) {
    TOGL_ToWindow().MoveBy(x, y);
}
inline void TOGL_MoveBy(const TOGL_PointI& offset) {
    TOGL_ToWindow().MoveBy(offset);
}

inline void TOGL_Resize(uint16_t width, uint16_t height, bool is_draw_area) {
    TOGL_ToWindow().Resize(width, height, is_draw_area);
}
inline void TOGL_Resize(const TOGL_SizeU16& size, bool is_draw_area) {
    TOGL_ToWindow().Resize(size, is_draw_area);
}

inline void TOGL_SetArea(int x, int y, uint16_t width, uint16_t height, bool is_draw_area) {
    TOGL_ToWindow().SetArea(x, y, width, height, is_draw_area);
}
inline void TOGL_SetArea(const TOGL_AreaIU16& area, bool is_draw_area) {
    TOGL_ToWindow().SetArea(area, is_draw_area);
}

inline void TOGL_Center(uint16_t width, uint16_t height) {
    TOGL_ToWindow().Center(width, height);
}
inline void TOGL_Center(const TOGL_SizeU16& size) {
    TOGL_ToWindow().Center(size);
}

inline void TOGL_Center(uint16_t width, uint16_t height, bool is_draw_area_size) {
    TOGL_ToWindow().Center(width, height, is_draw_area_size);
}
inline void TOGL_Center(const TOGL_SizeU16& size, bool is_draw_area_size) {
    TOGL_ToWindow().Center(size, is_draw_area_size);
}

// ---

inline TOGL_PointI TOGL_GetPos() {
    return TOGL_ToWindow().GetPos();
}
        
inline TOGL_SizeU16 TOGL_GetSize() {
    return TOGL_ToWindow().GetSize();
}
        
inline TOGL_AreaIU16 TOGL_GetArea() {
    return TOGL_ToWindow().GetArea();
}

inline TOGL_PointI TOGL_GetDrawAreaPos() {
    return TOGL_ToWindow().GetDrawAreaPos();
}

inline TOGL_SizeU16 TOGL_GetDrawAreaSize() {
    return TOGL_ToWindow().GetDrawAreaSize();
}

inline TOGL_AreaIU16 TOGL_GetDrawArea() {
    return TOGL_ToWindow().GetDrawArea();
}

// ---

inline void TOGL_Hide() {
    TOGL_ToWindow().Hide();
}

inline void TOGL_Show() {
    TOGL_ToWindow().Show();
}

inline bool TOGL_IsVisible() {
    return TOGL_ToWindow().IsVisible();
}

// ---

inline void TOGL_Minimize() {
    TOGL_ToWindow().Minimize();
}

inline void TOGL_Maximize() {
    TOGL_ToWindow().Maximize();
}

inline void TOGL_GoWindowedFullScreen() {
    TOGL_ToWindow().GoWindowedFullScreen();
}

inline TOGL_WindowStateId TOGL_GetState()  {
    return TOGL_ToWindow().GetState();
}

inline bool TOGL_IsNormal() {
    return TOGL_ToWindow().IsNormal();
}

inline bool TOGL_IsMinimized() {
    return TOGL_ToWindow().IsWindowMinimized();
}

inline bool TOGL_IsMaximized() {
    return TOGL_ToWindow().IsWindowMaximized();
}

inline bool TOGL_IsFullScreen() {
    return TOGL_ToWindow().IsWindowedFullScreen();
}

// ---

inline void TOGL_GoForeground() {
    TOGL_ToWindow().GoForeground();
}
inline bool TOGL_IsForeground() {
    return TOGL_ToWindow().IsForeground();
}

// ---

inline TOGL_WindowStyleBitfield TOGL_GetStyle() {
    return TOGL_ToWindow().GetStyle();
}

inline TOGL_PointI TOGL_GetCursorPosInDrawArea() {
    return TOGL_ToWindow().GetCursorPosInDrawArea();
}

inline TOGL_GL_Version TOGL_GetOpenGL_Version() {
    return TOGL_ToWindow().GetOpenGL_Version();
}

// ---

inline TOGL_Window& TOGL_ToWindow() {
    return TOGL_Global<TOGL_Window>::To();
}

//--------------------------------------------------------------------------
// TOGL_Window::WindowAreaCorrector
//--------------------------------------------------------------------------

inline TOGL_Window::WindowAreaCorrector::WindowAreaCorrector() {
    m_dwmapi_lib_handle = LoadLibraryA("Dwmapi.dll");
    if (m_dwmapi_lib_handle) {
        m_dwm_get_window_attribute          = (decltype(m_dwm_get_window_attribute)) GetProcAddress(m_dwmapi_lib_handle, "DwmGetWindowAttribute");
    } else {
        m_dwm_get_window_attribute          = nullptr;
    }
}

inline TOGL_Window::WindowAreaCorrector::~WindowAreaCorrector() {
    FreeLibrary(m_dwmapi_lib_handle);
}

inline TOGL_AreaIU16 TOGL_Window::WindowAreaCorrector::Get(HWND window_handle) const {
    TOGL_AreaIU16 area = {};

    if (m_dwm_get_window_attribute) {
        RECT window_rect;
        GetWindowRect(window_handle, &window_rect);

        // Added TOGL_ prefix.
        enum { TOGL_DWMWA_EXTENDED_FRAME_BOUNDS = 9 };
        RECT actual_window_rect;

        // Note: To return correct values, must be called after ShowWindow(window_handle, SW_SHOW).
        m_dwm_get_window_attribute(window_handle, TOGL_DWMWA_EXTENDED_FRAME_BOUNDS, &actual_window_rect, sizeof(RECT));

        RECT frame = {};
        frame.left      = actual_window_rect.left   - window_rect.left;
        frame.top       = actual_window_rect.top    - window_rect.top;
        frame.right     = window_rect.right         - actual_window_rect.right;
        frame.bottom    = window_rect.bottom        - actual_window_rect.bottom;

        area = TOGL_AreaIU16(- frame.left, -frame.top, uint16_t(frame.left + frame.right), uint16_t(frame.top + frame.bottom));
    }

    return area;
}

inline TOGL_AreaIU16 TOGL_Window::WindowAreaCorrector::AddInvisibleFrameTo(const TOGL_AreaIU16& area, HWND window_hangle) const {
    const TOGL_AreaIU16 correction = Get(window_hangle);
    return TOGL_AreaIU16(
        area.x      + correction.x,
        area.y      + correction.y,
        area.width  + correction.width,
        area.height + correction.height
    );
}

inline TOGL_SizeU16 TOGL_Window::WindowAreaCorrector::AddInvisibleFrameTo(const TOGL_SizeU16& size, HWND window_hangle) const {
    const TOGL_AreaIU16 correction = Get(window_hangle);
    return TOGL_SizeU16(
        size.width  + correction.width,
        size.height + correction.height
    );
}

inline TOGL_PointI TOGL_Window::WindowAreaCorrector::AddInvisibleFrameTo(const TOGL_PointI& pos, HWND window_hangle) const {
    const TOGL_AreaIU16 correction = Get(window_hangle);
    return {
        pos.x + correction.x,
        pos.y + correction.y,
    };
}

inline TOGL_AreaIU16 TOGL_Window::WindowAreaCorrector::RemoveInvisibleFrameFrom(const TOGL_AreaIU16& area, HWND window_hangle) const {
    const TOGL_AreaIU16 correction = Get(window_hangle);
    return TOGL_AreaIU16(
        area.x      - correction.x,
        area.y      - correction.y,
        area.width  - correction.width,
        area.height - correction.height
    );
}

inline TOGL_SizeU16 TOGL_Window::WindowAreaCorrector::RemoveInvisibleFrameFrom(const TOGL_SizeU16& size, HWND window_hangle) const {
    const TOGL_AreaIU16 correction = Get(window_hangle);
    return TOGL_SizeU16(
        size.width  - correction.width,
        size.height - correction.height
    );
}

inline TOGL_PointI TOGL_Window::WindowAreaCorrector::RemoveInvisibleFrameFrom(const TOGL_PointI& pos, HWND window_hangle) const {
    const TOGL_AreaIU16 correction = Get(window_hangle);
    return {
        pos.x - correction.x,
        pos.y - correction.y,
    };
}

//--------------------------------------------------------------------------
// TOGL_Window
//--------------------------------------------------------------------------

inline TOGL_Window::~TOGL_Window() {

}

//--------------------------------------------------------------------------
// Run, Close, Redraw
//--------------------------------------------------------------------------

inline int TOGL_Window::Run(const TOGL_Data& data) {
    m_data = data;
    TOGL_SetLogLevel(m_data.log_level);

    m_instance_handle = GetModuleHandleW(NULL);

    constexpr wchar_t WINDOW_CLASS_NAME[] = L"TrivialOpenGL_WindowClass";

    WNDCLASSEXW wc = {};
    wc.cbSize           = sizeof(WNDCLASSEXW);
    wc.style            = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = WindowProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = m_instance_handle;
    wc.hIcon            = TryLoadIcon();
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = NULL;
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = WINDOW_CLASS_NAME;
    wc.hIconSm          = NULL;

    if (!RegisterClassExW(&wc)) {
        TOGL_LogFatalError("Error TOGL_Window::Run: Cannot create window class.");
    }
    m_window_style = WS_OVERLAPPEDWINDOW;
    if (m_data.style & TOGL_WINDOW_STYLE_BIT_NO_RESIZE)     m_window_style &= ~WS_THICKFRAME;
    if (m_data.style & TOGL_WINDOW_STYLE_BIT_NO_MAXIMIZE)   m_window_style &= ~WS_MAXIMIZEBOX;
    if (m_data.style & TOGL_WINDOW_STYLE_BIT_DRAW_AREA_ONLY) {
        m_window_style          = GetWindowStyle_DrawAreaOnly();
        m_window_extended_style = GetWindowExtendedStyle_DrawAreaOnly();
    }

    m_window_handle = CreateWindowExW(
        m_window_extended_style,
        WINDOW_CLASS_NAME,
        TOGL_ToUTF16(m_data.window_name).c_str(),
        m_window_style,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        m_instance_handle,
        NULL
    );
                
    if (!m_window_handle) {
        TOGL_LogFatalError("Cannot create window.");
    }

    if (m_data.timer_time_interval > 0) {
        const UINT_PTR result = SetTimer(m_window_handle, TOGL_DEFAULT_TIMER_ID, m_data.timer_time_interval, NULL);

        if (!result) TOGL_LogFatalError(std::string() + "Can not set timer. (windows error code:" + std::to_string(GetLastError()) + ")");
    }

    ShowWindow(m_window_handle, SW_SHOW);
    SetForegroundWindow(m_window_handle);
    SetFocus(m_window_handle);
        
    // For tests only.
    //m_window_area_corrector.DisableComposition();

    // Here because, actual window area can be fetched by DwmGetWindowAttribute only after SW_SHOW.
    ChangeArea(m_data.area);

    if (m_data.do_on_create) m_data.do_on_create();

    UpdateWindow(m_window_handle);

    int result = ExecuteMainLoop();

    UnregisterClassW(WINDOW_CLASS_NAME, m_instance_handle);

    return result;
}

inline void TOGL_Window::RequestClose() {
    DestroyWindow(m_window_handle);
}

inline void TOGL_Window::RequestRedraw() {
    InvalidateRect(m_window_handle, NULL, FALSE);
}

//--------------------------------------------------------------------------
// Option
//--------------------------------------------------------------------------

inline void TOGL_Window::SetOption(TOGL_WindowOptionId window_option, bool is_enabled) {
    switch (window_option) {
    case TOGL_WINDOW_OPTION_ID_AUTO_SLEEP_MODE:
        m_data.is_auto_sleep_blocked = !is_enabled;
        break;
    }
}

inline bool TOGL_Window::IsEnabled(TOGL_WindowOptionId window_option) const {
    switch (window_option) {
    case TOGL_WINDOW_OPTION_ID_AUTO_SLEEP_MODE: return !m_data.is_auto_sleep_blocked;
    }
}

//--------------------------------------------------------------------------
// Position, Size, Area
//--------------------------------------------------------------------------

inline void TOGL_Window::MoveTo(int x, int y, bool is_draw_area) {
    SetArea({x, y, 0, 0}, TOGL_AREA_PART_ID_POSITION, is_draw_area);
}

inline void TOGL_Window::MoveTo(const TOGL_PointI& pos, bool is_draw_area) {
    MoveTo(pos.x, pos.y, is_draw_area);
}

inline void TOGL_Window::MoveBy(int x, int y) {
    SetArea({x, y, 0, 0}, TOGL_AREA_PART_ID_POSITION_OFFSET, false);
}

inline void TOGL_Window::MoveBy(const TOGL_PointI& offset) {
    MoveBy(offset.x, offset.y);
}


inline void TOGL_Window::Resize(uint16_t width, uint16_t height, bool is_draw_area) {
    SetArea({0, 0, width, height}, TOGL_AREA_PART_ID_SIZE, is_draw_area);
}

inline void TOGL_Window::Resize(const TOGL_SizeU16& size, bool is_draw_area) {
    Resize(size.width, size.height, is_draw_area);
}


inline void TOGL_Window::SetArea(int x, int y, uint16_t width, uint16_t height, bool is_draw_area) {
    SetArea({x, y, width, height}, TOGL_AREA_PART_ID_ALL, is_draw_area);
}

inline void TOGL_Window::SetArea(const TOGL_AreaIU16& area, bool is_draw_area) {
    SetArea(area.x, area.y, area.width, area.height, is_draw_area);
}


inline void TOGL_Window::Center(const TOGL_SizeU16& size) {
    Center(size, m_data.style & TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE);
}

inline void TOGL_Window::Center(uint16_t width, uint16_t height) {
    Center(width, height, m_data.style & TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE);
}

inline void TOGL_Window::Center(const TOGL_SizeU16& size, bool is_draw_area_size) {
    const TOGL_AreaIU16 desktop_area = TOGL_GetDesktopAreaNoTaskBar();

    TOGL_AreaIU16 window_area = TOGL_AreaIU16({}, size);

    Restore();

    if (is_draw_area_size) {
        window_area = GetWindowAreaFromDrawArea(window_area, m_window_style);
        window_area = m_window_area_corrector.RemoveInvisibleFrameFrom(window_area, m_window_handle);
    }

    window_area.x = (desktop_area.width - window_area.width) / 2;
    window_area.y = (desktop_area.height - window_area.height) / 2;
        
    SetArea(window_area);
}

inline void TOGL_Window::Center(uint16_t width, uint16_t height, bool is_draw_area_size) {
    Center(TOGL_SizeU16(width, height), is_draw_area_size);
}

//--------------------------------------------------------------------------

inline TOGL_PointI TOGL_Window::GetPos() const {
    return GetArea().GetPos();
}
    
inline TOGL_SizeU16 TOGL_Window::GetSize() const {
    return GetArea().GetSize();
}
    
inline TOGL_AreaIU16 TOGL_Window::GetArea() const {
    if (m_state == TOGL_WINDOW_STATE_ID_MINIMIZED) {
        return TOGL_AreaIU16(0, 0, 0, 0);
    }

    TOGL_AreaIU16 area = GetWindowArea(m_window_handle);
    
    // Workaround.
    if (IsWindowedFullScreen()) area.width -= TOGL_WIDTH_CORRECTION_TO_FAKE;
    
    return m_window_area_corrector.RemoveInvisibleFrameFrom(area, m_window_handle);
}
    
inline TOGL_PointI TOGL_Window::GetDrawAreaPos() const {
    return GetDrawArea().GetPos();
}
    
inline TOGL_SizeU16 TOGL_Window::GetDrawAreaSize() const {
    return GetDrawArea().GetSize();
}
    
inline TOGL_AreaIU16 TOGL_Window::GetDrawArea() const {
    RECT r;
    if (m_state == TOGL_WINDOW_STATE_ID_MINIMIZED) {
        return TOGL_AreaIU16(0, 0, 0, 0);
    }

    if (GetClientRect(m_window_handle, &r) && ClientToScreen(m_window_handle, (POINT*)&r) && ClientToScreen(m_window_handle, (POINT*)&r.right)) {
        TOGL_AreaIU16 area = TOGL_MakeAreaIU16(r);
    
        // Workaround.
        if (IsWindowedFullScreen()) area.width -= TOGL_WIDTH_CORRECTION_TO_FAKE;
    
        return area;
    }
    return TOGL_AreaIU16(0, 0, 0, 0);
}

//--------------------------------------------------------------------------
// Show, Hide
//--------------------------------------------------------------------------

inline void TOGL_Window::Hide() {
    ShowWindow(m_window_handle, SW_HIDE);
}

inline void TOGL_Window::Show() {
    ShowWindow(m_window_handle, SW_SHOW);
}

inline bool TOGL_Window::IsVisible() const {
    return m_is_visible;
}


//--------------------------------------------------------------------------
// State
//--------------------------------------------------------------------------

inline void TOGL_Window::Minimize() {
    if (m_state == TOGL_WINDOW_STATE_ID_WINDOWED_FULL_SCREENED) {
        Push(m_is_enable_do_on_resize, false);
        Push(m_is_enable_change_state_at_resize, false);

        SetWindowLongPtrW(m_window_handle, GWL_STYLE,   m_window_style);
        SetWindowLongPtrW(m_window_handle, GWL_EXSTYLE, m_window_extended_style);
        ShowWindow(m_window_handle, SW_NORMAL);

        Pop(m_is_enable_change_state_at_resize);
        Pop(m_is_enable_do_on_resize);
    } else if (m_state == TOGL_WINDOW_STATE_ID_MAXIMIZED) {
        Push(m_is_enable_do_on_resize, false);
        Push(m_is_enable_change_state_at_resize, false);

        ShowWindow(m_window_handle, SW_RESTORE);

        Pop(m_is_enable_change_state_at_resize);
        Pop(m_is_enable_do_on_resize);
    }

    ShowWindow(m_window_handle, SW_MINIMIZE);
}

inline void TOGL_Window::Maximize() {
    if (!m_is_visible) {
        ShowWindow(m_window_handle, SW_SHOW);
    }

    if (m_state == TOGL_WINDOW_STATE_ID_WINDOWED_FULL_SCREENED) {
        Push(m_is_enable_do_on_resize, false);
        Push(m_is_enable_change_state_at_resize, false);

        SetWindowLongPtrW(m_window_handle, GWL_STYLE,   m_window_style);
        SetWindowLongPtrW(m_window_handle, GWL_EXSTYLE, m_window_extended_style);

        Pop(m_is_enable_change_state_at_resize);
        Pop(m_is_enable_do_on_resize);
    } else if (m_state == TOGL_WINDOW_STATE_ID_MINIMIZED) {
        ShowWindow(m_window_handle, SW_RESTORE);
    }

    if (m_data.style & TOGL_WINDOW_STYLE_BIT_DRAW_AREA_ONLY) {
        const TOGL_SizeU16 work_area_size = TOGL_GetDesktopAreaSizeNoTaskBar();

        Push(m_is_enable_change_state_at_resize, false);

        SetWindowPos(m_window_handle, HWND_TOP, 0, 0, work_area_size.width, work_area_size.height, SWP_SHOWWINDOW);

        Pop(m_is_enable_change_state_at_resize);

        SetState(TOGL_WINDOW_STATE_ID_MAXIMIZED);
    } else {
        ShowWindow(m_window_handle, SW_MAXIMIZE);
    }
}

inline void TOGL_Window::GoWindowedFullScreen() {
    if (!m_is_visible) {
        ShowWindow(m_window_handle, SW_SHOW);
    }

    Push(m_is_enable_do_on_resize, false);
    Push(m_is_enable_change_state_at_resize, false);

    SetWindowLongPtrW(m_window_handle, GWL_STYLE, GetWindowStyle_DrawAreaOnly());
    SetWindowLongPtrW(m_window_handle, GWL_EXSTYLE, GetWindowExtendedStyle_DrawAreaOnly());

    Pop(m_is_enable_change_state_at_resize);
    Pop(m_is_enable_do_on_resize);

    // ---
    // Workaround.
    // In Windows 7, if window is borderless and covers exactly whole screen then alt+tab is not working. 
    // To omit that, size of window is extended beyond borders of screen, internally.
    // Library provides size of window without this internal adjustment.
    RECT screen_rectangle = MakeRECT(TOGL_AreaIU16({}, TOGL_GetScreenSize()));
    AdjustWindowRectEx(&screen_rectangle, GetWindowStyle_DrawAreaOnly(), FALSE, GetWindowExtendedStyle_DrawAreaOnly());

    const TOGL_AreaIU16 screen_area = TOGL_MakeAreaIU16(screen_rectangle);

    Push(m_is_enable_change_state_at_resize, false);
    m_is_apply_fake_width = true;

    SetWindowPos(m_window_handle, HWND_TOP, screen_area.x, screen_area.y, screen_area.width + TOGL_WIDTH_CORRECTION_TO_FAKE, screen_area.height, SWP_SHOWWINDOW);

    m_is_apply_fake_width = false;
    Pop(m_is_enable_change_state_at_resize);

    SetState(TOGL_WINDOW_STATE_ID_WINDOWED_FULL_SCREENED);
}

inline TOGL_WindowStateId TOGL_Window::GetState() const {
    return m_state;
}

inline bool TOGL_Window::IsNormal() const {
    return GetState() == TOGL_WINDOW_STATE_ID_NORMAL;
}

inline bool TOGL_Window::IsWindowMinimized() const {
    return GetState() == TOGL_WINDOW_STATE_ID_MINIMIZED;
}

inline bool TOGL_Window::IsWindowMaximized() const {
    return GetState() == TOGL_WINDOW_STATE_ID_MAXIMIZED;
}

inline bool TOGL_Window::IsWindowedFullScreen() const {
    return GetState() == TOGL_WINDOW_STATE_ID_WINDOWED_FULL_SCREENED;
}

inline void TOGL_Window::GoForeground() {
    SetForegroundWindow(m_window_handle);
}

inline bool TOGL_Window::IsForeground() const {
    return GetForegroundWindow() == m_window_handle;
}

//--------------------------------------------------------------------------

inline TOGL_WindowStyleBitfield TOGL_Window::GetStyle() const {
    return m_data.style;
}

inline TOGL_PointI TOGL_Window::GetCursorPosInDrawArea() const {
    POINT pos;
    if (GetCursorPos(&pos) && ScreenToClient(m_window_handle, &pos)) {
        return { pos.x, pos.y };
    }
    return {};
}

inline TOGL_GL_Version TOGL_Window::GetOpenGL_Version() const {
    return m_data.opengl_verion;
}

//--------------------------------------------------------------------------
// TOGL_Window (private)
//--------------------------------------------------------------------------

inline TOGL_Window::TOGL_Window() {
    m_data                      = {};

    m_instance_handle           = NULL;
    m_window_handle             = NULL;
    m_device_context_handle     = NULL;
    m_rendering_context_handle  = NULL;

    m_window_style              = 0;
    m_window_extended_style     = 0;

    m_is_active                 = false;
    m_is_visible                = false;
    m_is_frame                  = true;

    m_state                     = TOGL_WINDOW_STATE_ID_NORMAL;
    m_prev_state                = m_state;

    m_is_win7                   = IsWindows7OrGreater() && !IsWindows8OrGreater();

    m_is_apply_fake_width               = false;
    m_is_enable_do_on_resize            = true;
    m_is_enable_change_state_at_resize  = true;

    m_dbg_message_id                    = 0;

    memset(m_char_utf16, 0, sizeof(m_char_utf16));

        
}

// ---

inline HWND TOGL_Window::ToHWND() {
    return m_window_handle;
}

inline void TOGL_Window::SetState(TOGL_WindowStateId state) {
    m_prev_state = m_state;
    m_state = state;
    if (m_state != m_prev_state && m_data.do_on_state_change) m_data.do_on_state_change(m_state);
}

// ---

inline void TOGL_Window::Push(bool& value, bool new_value_after_push) {
    m_on_off_stack_map[&value].push(value);
    value = new_value_after_push;
}

inline void TOGL_Window::Pop(bool& value) {
    auto stack = m_on_off_stack_map[&value];
    if (!stack.empty()) {
        value = stack.top();
        stack.pop();
    }
}

// ---

inline void TOGL_Window::ChangeArea(const TOGL_AreaIU16& area) {
    SetArea(GenerateWindowArea(area), TOGL_AREA_PART_ID_ALL, m_data.style & TOGL_WINDOW_STYLE_BIT_DRAW_AREA_ONLY);
}

inline HICON TOGL_Window::TryLoadIcon() {
    if (!m_data.icon_file_name.empty()) {
        return (HICON)LoadImageW(
            NULL,
            TOGL_ToUTF16(m_data.icon_file_name).c_str(),
            IMAGE_ICON,
            0, 0,
            LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED
        );
    } else if (m_data.icon_resource_id > 0) {
        return LoadIconW(m_instance_handle, MAKEINTRESOURCEW(m_data.icon_resource_id));
    } else {
        return NULL;
    }
}

inline int TOGL_Window::ExecuteMainLoop() {
    MSG msg = {};

    if (m_data.style & TOGL_WINDOW_STYLE_BIT_REDRAW_ON_CHANGE_OR_REQUEST) {

        while (GetMessageW(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        if (msg.message == WM_QUIT) {
            m_window_handle = NULL;
            return (int)msg.wParam;
        }

    } else {

        while (true) {
            if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    m_window_handle = NULL;
                    return (int)msg.wParam;
                }

                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            } else {
                Draw();
            }
        }
    }

    return EXIT_FAILURE;
}

inline void TOGL_Window::Draw() {
    if (m_data.special_debug.is_notify_draw_call) {
        TOGL_LogDebug("Window::Draw"); 
    }

    if (m_data.draw) m_data.draw();

    SwapBuffers(m_device_context_handle);
}

//--------------------------------------------------------------------------

inline TOGL_AreaIU16 TOGL_Window::GenerateWindowArea(const TOGL_AreaIU16& area) {
    TOGL_AreaIU16 window_area;

    const bool is_default = (area == TOGL_AreaIU16(0, 0, 0, 0));

    // --- Size --- //

    const TOGL_AreaIU16 desktop_area = TOGL_GetDesktopAreaNoTaskBar();

    window_area.width   = is_default ? (desktop_area.width / 2)     : area.width;
    window_area.height  = is_default ? (desktop_area.height / 2)    : area.height;

    // In a case of unreasonable values.
    if (window_area.width < 0)    window_area.width = 0;
    if (window_area.height < 1)   window_area.height = 1;

    if ((m_data.style & TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE) && !(m_data.style & TOGL_WINDOW_STYLE_BIT_DRAW_AREA_ONLY)) {
        const TOGL_AreaIU16 window_area_with_invisible_frame = GetWindowAreaFromDrawArea(window_area, m_window_style);

        window_area = m_window_area_corrector.RemoveInvisibleFrameFrom(window_area_with_invisible_frame, m_window_handle);
    }

    // --- Position --- //

    if (m_data.style & TOGL_WINDOW_STYLE_BIT_CENTERED) {
        window_area.x = (desktop_area.width - window_area.width) / 2;
        window_area.y = (desktop_area.height - window_area.height) / 2;
    } else {
        window_area.x = is_default ? ((desktop_area.width - window_area.width) / 2)     : area.x;
        window_area.y = is_default ? ((desktop_area.height - window_area.height) / 2)   : area.y;
    }

    // No need for additional adjustment for invisible window frame. 
    // Already done for both position and size in 'Size' section.

    // ---

    return window_area;
}

inline void TOGL_Window::Restore() {
    if (IsMaximized(m_window_handle)) {
        if (m_prev_state == TOGL_WINDOW_STATE_ID_WINDOWED_FULL_SCREENED) {
            Push(m_is_enable_do_on_resize, false);
            Push(m_is_enable_change_state_at_resize, false);

            ShowWindow(m_window_handle, SW_RESTORE);

            Pop(m_is_enable_change_state_at_resize);
            Pop(m_is_enable_do_on_resize);
        } else {
            ShowWindow(m_window_handle, SW_RESTORE);
        }
    }

    if (IsMinimized(m_window_handle)) {
        ShowWindow(m_window_handle, SW_RESTORE);
    }
      
    if (m_state == TOGL_WINDOW_STATE_ID_WINDOWED_FULL_SCREENED) {
        Push(m_is_enable_do_on_resize, false);
        Push(m_is_enable_change_state_at_resize, false);

        SetWindowLongPtrW(m_window_handle, GWL_STYLE,   m_window_style);
        SetWindowLongPtrW(m_window_handle, GWL_EXSTYLE, m_window_extended_style);
        ShowWindow(m_window_handle, SW_NORMAL);

        Pop(m_is_enable_change_state_at_resize);
        Pop(m_is_enable_do_on_resize);
    } 
}
    
inline void TOGL_Window::SetArea(const TOGL_AreaIU16& area, AreaPartId area_part_id, bool is_draw_area) {
    auto GetFlags = [](AreaPartId area_part_id) -> UINT {
        switch (area_part_id) {
        case TOGL_AREA_PART_ID_POSITION:          return SWP_NOSIZE;
        case TOGL_AREA_PART_ID_POSITION_OFFSET:   return SWP_NOSIZE;
        case TOGL_AREA_PART_ID_SIZE:              return SWP_NOMOVE;
        case TOGL_AREA_PART_ID_ALL:               return 0;
        }
        return 0;
    };

    const bool is_skip = IsMinimized(m_window_handle) && area_part_id == TOGL_AREA_PART_ID_POSITION_OFFSET;

    if (!is_skip) {
        if (!m_is_visible) {
            ShowWindow(m_window_handle, SW_SHOW);
        }

        Restore();
  
        TOGL_AreaIU16 raw_area;

        if (area_part_id == TOGL_AREA_PART_ID_POSITION_OFFSET) {
            raw_area = GetWindowArea(m_window_handle);
            raw_area.x += area.x;
            raw_area.y += area.y;
        } else if (is_draw_area) {
            raw_area = GetWindowAreaFromDrawArea(area, m_window_style);
        } else {
            raw_area = m_window_area_corrector.AddInvisibleFrameTo(area, m_window_handle);
        }

        // TODO: Solve redundant call of WM_SIZE when window restores from minimized 
        // and there is difference in window area between current one and previous one.
        //if (IsMinimized(m_window_handle)) {
        //    WINDOWPLACEMENT placement = {};
        //    if (GetWindowPlacement(m_window_handle, &placement) && MakeAreaIU16(placement.rcNormalPosition) != raw_area) {
        //        // Prevents to call wN_SIZE for previous window size stored in window restore data.
        //        Push(m_is_enable_do_on_resize, false);
        //        ShowWindow(m_window_handle, SW_RESTORE);
        //        Pop(m_is_enable_do_on_resize);
        //    } else {
        //        ShowWindow(m_window_handle, SW_RESTORE);
        //    }
        //}

        SetWindowPos(m_window_handle, HWND_TOP, raw_area.x, raw_area.y, raw_area.width, raw_area.height, GetFlags(area_part_id) | SWP_SHOWWINDOW);
    }  
}

//--------------------------------------------------------------------------

inline void TOGL_Window::Create(HWND window_handle) {
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion        = 1;
    pfd.dwFlags         = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType      = PFD_TYPE_RGBA;
    pfd.cColorBits      = 24;               
    pfd.cAlphaBits      = 8;                
    pfd.cDepthBits      = 32;               
    pfd.cStencilBits    = 8;                
    pfd.iLayerType      = PFD_MAIN_PLANE;

    m_device_context_handle = GetDC(window_handle);
    if (!m_device_context_handle) TOGL_LogFatalError(" Can not get device context.");

    int pfi = ChoosePixelFormat(m_device_context_handle, &pfd);
    if (!pfi) TOGL_LogFatalError(std::string() + "Can not choose pixel format. (windows error code:" + std::to_string(GetLastError()) + ")");

    BOOL result = SetPixelFormat(m_device_context_handle, pfi, &pfd);
    if (!result) TOGL_LogFatalError(std::string() + "Can not set pixel format. (windows error code:" + std::to_string(GetLastError()) + ")");

    // --- Displaying Format Info --- //

    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    int max_pfi = DescribePixelFormat(m_device_context_handle, pfi, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
    if (!max_pfi) TOGL_LogFatalError(std::string() + "Can not get pixel format. (windows error code:" + std::to_string(GetLastError()) + ")");

    TOGL_LogInfo(std::string() + "OpenGL Pixel Format:"
        " Red:"     + std::to_string(pfd.cRedBits) + 
        " Green:"   + std::to_string(pfd.cGreenBits) + 
        " Blue:"    + std::to_string(pfd.cBlueBits) + 
        " Alpha:"   + std::to_string(pfd.cAlphaBits) + 
        " Depth:"   + std::to_string(pfd.cDepthBits) + 
        " Stencil:" + std::to_string(pfd.cStencilBits));

    // --- Creates OpenGL Rendering Context --- //

    m_rendering_context_handle = wglCreateContext(m_device_context_handle);
    if (!m_rendering_context_handle) TOGL_LogFatalError(std::string() + "Can not create OpenGl Rendering Context. (windows error code:" + std::to_string(GetLastError()) + ")");

    if (!wglMakeCurrent(m_device_context_handle, m_rendering_context_handle)) {
        TOGL_LogFatalError("Can not set created OpenGl Rendering Context to be current.");
    }

    // --- Creates OpenGL Rendering Context with required minimum version --- //

    if (m_data.opengl_verion.major != 0 && m_data.opengl_verion.minor != 0) {
        HGLRC (WINAPI *togl_wglCreateContextAttribsARB)(HDC hDC, HGLRC hShareContext, const int* attribList) = (decltype(togl_wglCreateContextAttribsARB)) wglGetProcAddress("wglCreateContextAttribsARB");
        if (togl_wglCreateContextAttribsARB) {
            enum {
                // Added prefix TOGL_ to standard OpenGL constants.
                TOGL_WGL_CONTEXT_MAJOR_VERSION_ARB               = 0x2091,
                TOGL_WGL_CONTEXT_MINOR_VERSION_ARB               = 0x2092,
                TOGL_WGL_CONTEXT_LAYER_PLANE_ARB                 = 0x2093,
                TOGL_WGL_CONTEXT_FLAGS_ARB                       = 0x2094,
                TOGL_WGL_CONTEXT_PROFILE_MASK_ARB                = 0x9126,

                TOGL_WGL_CONTEXT_DEBUG_BIT_ARB                   = 0x0001,
                TOGL_WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB      = 0x0002,

                TOGL_WGL_CONTEXT_CORE_PROFILE_BIT_ARB            = 0x00000001,
                TOGL_WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB   = 0x00000002,

                TOGL_ERROR_INVALID_VERSION_ARB                   = 0x2095,
                TOGL_ERROR_INVALID_PROFILE_ARB                   = 0x2096,
            };
            GLint attribute_list[] =
            {
                TOGL_WGL_CONTEXT_MAJOR_VERSION_ARB, m_data.opengl_verion.major,
                TOGL_WGL_CONTEXT_MINOR_VERSION_ARB, m_data.opengl_verion.minor,
                TOGL_WGL_CONTEXT_FLAGS_ARB, TOGL_WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                0
            };

            HGLRC rendering_context_handle = togl_wglCreateContextAttribsARB(m_device_context_handle, 0, attribute_list);
            if (!rendering_context_handle) {
                TOGL_LogFatalError(std::string() + "Can not create OpenGl Rendering Context for version " + std::to_string(m_data.opengl_verion.major) + "." + std::to_string(m_data.opengl_verion.minor) + ".");
            }

            if (!wglMakeCurrent(m_device_context_handle, rendering_context_handle)) {
                TOGL_LogFatalError(std::string() + "Can not set created OpenGl Rendering Context for version " + std::to_string(m_data.opengl_verion.major) + "." + std::to_string(m_data.opengl_verion.minor) + " to be current.");
            }

            m_rendering_context_handle = rendering_context_handle ;

        } else {
            TOGL_LogFatalError("Can not load wglCreateContextAttribsARB function.");
        }
    }

    // --- Fetch OpenGL Versions --- //
    enum {
        // Added prefix TOGL_ to standard OpenGL constants.
        TOGL_GL_MAJOR_VERSION = 0x821B,
        TOGL_GL_MINOR_VERSION = 0x821C,
    };

    m_data.opengl_verion = {0, 0};

    glGetIntegerv(TOGL_GL_MAJOR_VERSION, &m_data.opengl_verion.major);
    glGetIntegerv(TOGL_GL_MINOR_VERSION, &m_data.opengl_verion.minor);

    if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) TOGL_LogInfo(std::string("OpenGl Version: ") + (const char*)glGetString(GL_VERSION));

    // ---
}

inline void TOGL_Window::Destroy() {
    if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
        TOGL_LogDebug("Window::Destroy");
    }

    if (m_data.do_on_destroy) m_data.do_on_destroy();

    wglMakeCurrent(NULL, NULL); 
    wglDeleteContext(m_rendering_context_handle);
    m_rendering_context_handle = NULL;

    ReleaseDC(m_window_handle, m_device_context_handle);
    m_device_context_handle = NULL;

    PostQuitMessage(0);
}

//--------------------------------------------------------------------------

inline DWORD TOGL_Window::GetWindowStyle_DrawAreaOnly() { 
    return 
        WS_POPUP
        // Commented options are for tests only.
        //| WS_THICKFRAME 
        //| WS_CAPTION 
        //| WS_SYSMENU 
        //| WS_MAXIMIZEBOX 
        //| WS_MINIMIZEBOX
        //| WS_BORDER 
        | WS_CLIPSIBLINGS 
        | WS_CLIPCHILDREN
        ; 
}
inline DWORD TOGL_Window::GetWindowExtendedStyle_DrawAreaOnly() { 
    return WS_EX_APPWINDOW; 
}

//--------------------------------------------------------------------------
    
inline TOGL_AreaIU16 TOGL_Window::GetWindowArea(HWND window_handle) {
    RECT r;
    if (GetWindowRect(window_handle, &r)) {
        return TOGL_MakeAreaIU16(r);
    }
    return TOGL_AreaIU16(0, 0, 0, 0);
}

inline TOGL_AreaIU16 TOGL_Window::GetWindowAreaFromDrawArea(const TOGL_AreaIU16& draw_area, DWORD window_style) {
    RECT rect = MakeRECT(draw_area);
    AdjustWindowRect(&rect, window_style, FALSE);
    return TOGL_MakeAreaIU16(rect);
}

//--------------------------------------------------------------------------

inline void TOGL_Window::HandleDoOnKeyboardKey(WPARAM w_param, LPARAM l_param) {
    if (m_data.do_on_key) {
        const _TOGL_InnerKey::VirtualKeyData& virtual_key_data = *((const _TOGL_InnerKey::VirtualKeyData*)(&l_param));

        const TOGL_KeyId key_id = _TOGL_InnerKey::GetKeyId(w_param);
        const bool is_down = virtual_key_data.trans_state == 0;

        TOGL_Extra extra = {};
        extra.count = virtual_key_data.count;

        const TOGL_PointI pos = GetCursorPosInDrawArea();
        extra.x = pos.x;
        extra.y = pos.y;

        extra.keyboard_side = _TOGL_InnerKey::GetKeyboardSide(key_id, virtual_key_data);

        m_data.do_on_key(key_id, is_down, extra);
    }
}


inline void TOGL_Window::HandleDoOnMouseKey(UINT message, WPARAM w_param, LPARAM l_param) {
    const bool is_down = _TOGL_InnerKey::IsMouseButtonDown(message);

    if (m_data.do_on_key) {
        const TOGL_KeyId key_id = _TOGL_InnerKey::GetMouseKeyId(message, w_param);

        TOGL_Extra extra = {};
        extra.count         = 1;
        extra.x             = GET_X_LPARAM(l_param);
        extra.y             = GET_Y_LPARAM(l_param);
        extra.keyboard_side = TOGL_KEYBOARD_SIDE_ID_NONE;

        m_data.do_on_key(key_id, is_down, extra);
    }

    // Tracks mouse button up message when mouse button is down and cursor leave window draw (client) area.
    if (is_down) {
        SetCapture(m_window_handle);
    } else if (GetCapture() == m_window_handle) {
        ReleaseCapture();
    }
}

template <unsigned NUNMBER_OF_CODE_UNITS>
inline void TOGL_Window::HandleUTF16_ToUTF8(const wchar_t (&char_utf16)[NUNMBER_OF_CODE_UNITS]) {
    if (m_data.do_on_char) {
        enum { MAX_NUM_UTF8_CODE_UNITS = 6 };

        uint8_t char_utf8[MAX_NUM_UTF8_CODE_UNITS];

        uint32_t number = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)char_utf16, NUNMBER_OF_CODE_UNITS, (LPSTR)char_utf8, MAX_NUM_UTF8_CODE_UNITS, NULL, NULL);

        for (uint32_t ix = 0; ix < number; ix++) {
            m_data.do_on_char(char_utf8[ix]);
        }
    }
}

//--------------------------------------------------------------------------
// InnerWindowProc
//--------------------------------------------------------------------------

inline LRESULT TOGL_Window::InnerWindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
    if (m_data.special_debug.is_notify_any_message) {
        TOGL_LogDebug(std::string() + "[" + WM_ToStr(message) + "] " + std::to_string(m_dbg_message_id++));
    }


    switch (message) {
    case WM_CREATE: 
        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
            TOGL_LogDebug("WM_CREATE"); 
        }

        Create(window_handle);
        return 0;

    case WM_DESTROY:
        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
            TOGL_LogDebug("WM_DESTROY"); 
        }

        Destroy();
        return 0;

    case WM_CLOSE:
        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
            TOGL_LogDebug("WM_CLOSE"); 
        }

        DestroyWindow(m_window_handle);
        return 0;

    case WM_PAINT:
        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
            TOGL_LogDebug("WM_PAINT"); 
        }

        Draw();

        ValidateRect(m_window_handle, NULL); // to decrease number of WM_PAINT messages

        return 0;

    // Used for debugging.
#if 0
    case WM_MOVING: {
        if (IsInLogLevel(LOG_LEVEL_INFO)) {
            std::string message = "WM_MOVING";
        
            RECT* drag_rect = (RECT*)(l_param); // in screen coordinates
        
            message += std::string() + " drag_rect=" 
                + std::to_string(drag_rect->left) + "," 
                + std::to_string(drag_rect->top) + ","
                + std::to_string(drag_rect->right) + ","
                + std::to_string(drag_rect->bottom);
        
            LogDebug(message);
        }
        
        return TRUE;
    }
#endif

    // Used for debugging.
#if 0
    case WM_WINDOWPOSCHANGING: {
        if (IsInLogLevel(LOG_LEVEL_INFO)) {
            std::string dbg_msg = "WM_WINDOWPOSCHANGING";
        
        
            const WINDOWPOS* wp = (const WINDOWPOS*)(l_param);
        
            dbg_msg += std::string() + " pos=" 
                + std::to_string(wp->x) + "," 
                + std::to_string(wp->y);
        
            LogDebug(dbg_msg);
        
        }
        return 0;
    }
#endif
         
    // Used for debugging.
#if 0
    case WM_GETMINMAXINFO: {
        if (IsInLogLevel(LOG_LEVEL_INFO)) {
            std::string dbg_msg = "WM_GETMINMAXINFO ";
        
            const MINMAXINFO* info = (const MINMAXINFO*)(l_param);
        
            dbg_msg += std::string() 
                + " MaxSize=" + std::to_string(info->ptMaxSize.x) + "," + std::to_string(info->ptMaxSize.y)
                + " ptMaxPosition=" + std::to_string(info->ptMaxPosition.x) + "," + std::to_string(info->ptMaxPosition.y)
                + " ptMinTrackSize=" + std::to_string(info->ptMinTrackSize.x) + "," + std::to_string(info->ptMinTrackSize.y)
                + " ptMaxTrackSize=" + std::to_string(info->ptMaxTrackSize.x) + "," + std::to_string(info->ptMaxTrackSize.y)
                ;
        
            LogDebug(dbg_msg);
        }
        break;
    }
#endif

    case WM_ENTERSIZEMOVE: {
        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
            TOGL_LogDebug("WM_ENTERSIZEMOVE");
        }

        return 0;
    }

    case WM_EXITSIZEMOVE: {
        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
            TOGL_LogDebug("WM_EXITSIZEMOVE");
        }

        return 0;
    }

    case WM_SIZING: {
        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
            std::string dbg_msg = "WM_SIZING";

            auto GetEdgeName = [](WPARAM w_param) -> std::string {
                switch (w_param) {
                case WMSZ_BOTTOM:       return "WMSZ_BOTTOM";
                case WMSZ_BOTTOMLEFT:   return "WMSZ_BOTTOMLEFT";
                case WMSZ_BOTTOMRIGHT:  return "WMSZ_BOTTOMRIGHT";
                case WMSZ_LEFT:         return "WMSZ_LEFT";
                case WMSZ_RIGHT:        return "WMSZ_RIGHT";
                case WMSZ_TOP:          return "WMSZ_TOP";
                case WMSZ_TOPLEFT:      return "WMSZ_TOPLEFT";
                case WMSZ_TOPRIGHT:     return "WMSZ_TOPRIGHT";
                }
                return std::string() + "(" + std::to_string(w_param) +")";
            };

            // Parameter w_param can contain other values than defined ones, for example: 9 when undocking window.
            dbg_msg += std::string() + " " + GetEdgeName(w_param);

            RECT* drag_rect = (RECT*)(l_param); // in screen coordinates

            dbg_msg += std::string() + " drag_rect=" 
                + std::to_string(drag_rect->left) + "," 
                + std::to_string(drag_rect->top) + ","
                + std::to_string(drag_rect->right) + ","
                + std::to_string(drag_rect->bottom);

            TOGL_LogDebug(dbg_msg);
        }

        return TRUE;
    }

    case WM_SIZE:
        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
            std::string dbg_msg = "WM_SIZE";

            const uint16_t width    = LOWORD(l_param);
            const uint16_t height        = HIWORD(l_param);

            dbg_msg += std::string() + " " + std::to_string(width) + " " + std::to_string(height);

            auto GetRequest = [](WPARAM w_param) -> std::string {
                switch (w_param) {
                case SIZE_MAXHIDE:      return "SIZE_MAXHIDE";
                case SIZE_MAXIMIZED:    return "SIZE_MAXIMIZED";
                case SIZE_MAXSHOW:      return "SIZE_MAXSHOW";
                case SIZE_MINIMIZED:    return "SIZE_MINIMIZED";
                case SIZE_RESTORED:     return "SIZE_RESTORED";
                }
                return std::string() + "(" + std::to_string(w_param) +")";
            };

            dbg_msg += std::string() + " " + GetRequest(w_param);

            if (m_is_apply_fake_width) dbg_msg += " fake_width=" + std::to_string(width - TOGL_WIDTH_CORRECTION_TO_FAKE);

            if (!m_is_enable_do_on_resize) dbg_msg += " without:do_on_resize";

            TOGL_LogDebug(dbg_msg);
        }

        m_is_visible = true;

        if (m_data.do_on_resize) {
            if (m_is_enable_do_on_resize) {
                if (m_is_apply_fake_width) {
                    m_data.do_on_resize(LOWORD(l_param) - TOGL_WIDTH_CORRECTION_TO_FAKE, HIWORD(l_param));
                } else {
                    m_data.do_on_resize(LOWORD(l_param), HIWORD(l_param));
                }
            }
        }

        if (m_is_enable_change_state_at_resize) {
            switch (w_param) {
            case SIZE_MAXIMIZED:  
                SetState(TOGL_WINDOW_STATE_ID_MAXIMIZED);
                break;
            case SIZE_MINIMIZED:    
                SetState(TOGL_WINDOW_STATE_ID_MINIMIZED);
                break;
            case SIZE_RESTORED:    
                SetState(TOGL_WINDOW_STATE_ID_NORMAL);
                break;
            }
        }

        return 0;

    case WM_SHOWWINDOW: {
        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
            std::string dbg_msg = "WM_SHOWWINDOW";

            dbg_msg += (w_param == TRUE) ? " SHOW" : " HIDE";

            auto GetStatusName = [](WPARAM l_param) -> std::string {
                switch (l_param) {
                case SW_OTHERUNZOOM:    return "SW_OTHERUNZOOM";
                case SW_OTHERZOOM:      return "SW_OTHERZOOM";
                case SW_PARENTCLOSING:  return "SW_PARENTCLOSING";
                case SW_PARENTOPENING:  return "SW_PARENTOPENING";
                }
                return std::string() + "(" + std::to_string(l_param) +")";
            };

            if (l_param) {
                dbg_msg += std::string() + " " + GetStatusName(l_param);
            }

            TOGL_LogDebug(dbg_msg);
        }

        const bool is_visible = (w_param == TRUE);

        if (is_visible != m_is_visible) {
            m_is_visible = is_visible;

            if (m_is_visible && m_data.do_on_show) {
                m_data.do_on_show();
            } else if (!m_is_visible && m_data.do_on_hide) {
                m_data.do_on_hide();
            }
        }

        return 0;
    }

    case WM_ERASEBKGND:
        // Tells DefWindowProc to not erase background. It's unnecessary since background is handled by OpenGL.
        return 1;

    case WM_ACTIVATE: {
        const bool is_active    = LOWORD(w_param) != WA_INACTIVE;
        const bool is_minimized = HIWORD(w_param);

        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
            std::string dbg_msg = "WM_ACTIVATE";

            auto GetActivationStateName = [](int state) -> std::string {
                switch (state) {
                case WA_ACTIVE:         return "WA_ACTIVE";
                case WA_CLICKACTIVE:    return "WA_CLICKACTIVE";
                case WA_INACTIVE:       return "WA_INACTIVE";
                }
                return std::string() + "(" + std::to_string(state) +")";
            };
            dbg_msg += std::string() + " " + GetActivationStateName(LOWORD(w_param));

            if (is_minimized) dbg_msg += " MINIMIZED";

            if (is_active != m_is_active) {
                dbg_msg += m_is_active ? " active->inactive" : " inactive->active";
            }

            TOGL_LogDebug(dbg_msg);
        }

        if (is_active != m_is_active) {
            m_is_active = is_active;

            if (is_active) {
                if (m_data.do_on_foreground) m_data.do_on_foreground(true);
            } else {
                if (m_data.do_on_foreground) m_data.do_on_foreground(false);
            }
        }

        return 0;    
    }

    // Used for debugging.
#if 0
    case WM_NCCALCSIZE: {
        if (IsInLogLevel(LOG_LEVEL_INFO)) {
            std::string dbg_msg = "WM_NCCALCSIZE";

            if (w_param == TRUE) {
                dbg_msg += " TRUE";

                NCCALCSIZE_PARAMS& params = *((NCCALCSIZE_PARAMS*)l_param);


                auto AreaToStr = [](const AreaIU16& area) {
                    return std::to_string(area.x) + "," 
                        + std::to_string(area.y) + ","
                        + std::to_string(area.width) + ","
                        + std::to_string(area.height);
                };

                dbg_msg += std::string() 
                    + " new=" + AreaToStr(MakeArea(params.rgrc[0]))
                    + " old=" + AreaToStr(MakeArea(params.rgrc[1]))
                    + " old_client=" + AreaToStr(MakeArea(params.rgrc[2]));

            } else {

                dbg_msg += " FALSE";
            }
            LogDebug(dbg_msg);
        }

        break;
    }
#endif

    case WM_ACTIVATEAPP:
        if (!w_param) {
            // Alternative workaround for disabled alt+tab in Windows 7.
            //if (m_is_win7) ShowWindow(m_window_handle, SW_SHOWMINIMIZED);
        }
        return 0;

    case WM_SYSCOMMAND:
        if (m_data.is_auto_sleep_blocked) {
            switch (w_param) {
            case SC_SCREENSAVE:        // Blocks screen saver.
            case SC_MONITORPOWER:   // Blocks entering to power save mode.
                return 0;                        
            }
        }
        break;                                
 
    case WM_TIMER:
        if (w_param == TOGL_DEFAULT_TIMER_ID) {
            if (m_data.do_on_time) m_data.do_on_time(m_data.timer_time_interval);
        }
        return 0;

    //////////////
    // Keyboard //
    //////////////
    case WM_SETFOCUS:
        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
            TOGL_LogDebug("WM_SETFOCUS");
        }
        return 0;

    case WM_KILLFOCUS:
        if (TOGL_IsLogLevelAtLeast(TOGL_LOG_LEVEL_INFO)) {
            TOGL_LogDebug("WM_KILLFOCUS");
        }
        return 0;

    case WM_KEYDOWN:
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_KEYDOWN   ";

            dbg_msg += _TOGL_InnerKey::WinApiKeyDataToStr(w_param, l_param);

            TOGL_LogDebug(dbg_msg);
        }
        HandleDoOnKeyboardKey(w_param, l_param);
        return 0;

    case WM_KEYUP:
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_KEYUP     ";

            dbg_msg += _TOGL_InnerKey::WinApiKeyDataToStr(w_param, l_param);

            TOGL_LogDebug(dbg_msg);
        }
        HandleDoOnKeyboardKey(w_param, l_param);
        return 0;

    case WM_SYSKEYDOWN:
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_SYSKEYDOWN";

            dbg_msg += _TOGL_InnerKey::WinApiKeyDataToStr(w_param, l_param);

            TOGL_LogDebug(dbg_msg);
        }
        HandleDoOnKeyboardKey(w_param, l_param);
        return 0;

    case WM_SYSKEYUP:
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_SYSKEYUP  ";

            dbg_msg += _TOGL_InnerKey::WinApiKeyDataToStr(w_param, l_param);

            TOGL_LogDebug(dbg_msg);
        }
        HandleDoOnKeyboardKey(w_param, l_param);
        return 0;

    case WM_CHAR: {
        if (m_data.special_debug.is_notify_character_message) {
            // test for utf-32:
            // in ahk: a::Send {U+24B62}
            // utf-16 output should be: D852, DF62
            // 
            // test for utf-32:
            // in ahk: a::Send {U+10348}
            // utf-16 output should be: D800, DF48
            // utf-8 output should be: F0, 90, 8D, 88

            std::string dbg_msg = "WM_CHAR ";

            const char c                = (char)w_param;
            const uint16_t code_utf16   = (uint16_t)w_param;
            const unsigned scan_code    = (unsigned(l_param) >> 16) & 0xFFFF;

            dbg_msg += "'";
            dbg_msg += c;
            dbg_msg += "'";
            dbg_msg += " utf8_code=" + std::to_string(code_utf16);
            dbg_msg += " scan_code=" + std::to_string(scan_code);

            TOGL_LogDebug(dbg_msg);
        }

        const uint16_t code_utf16 = (uint16_t)w_param;

        if (m_data.do_on_char_utf16) m_data.do_on_char_utf16(code_utf16);

        if ((0xDC00 & code_utf16) == 0xDC00) { // second code unit from two
            m_char_utf16[1] = code_utf16;

            int char_utf32 = (uint32_t(m_char_utf16[0]) & 0x0000'03FF) << 10;
            char_utf32 += (uint32_t(m_char_utf16[1]) & 0x0000'03FF) + 0x0001'0000;

            if (m_data.do_on_char_utf32) m_data.do_on_char_utf32(char_utf32);

            HandleUTF16_ToUTF8(m_char_utf16);

        } else if ((0xD800 & code_utf16) == 0xD800) { // first code unit from two
            m_char_utf16[0] = code_utf16;
        } else { // only one code unit
            if (m_data.do_on_char_utf32) m_data.do_on_char_utf32(code_utf16);

            HandleUTF16_ToUTF8({code_utf16});
        }

        return 0;
    }


    ///////////
    // Mouse //
    ///////////

    case WM_LBUTTONDOWN:
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_LBUTTONDOWN";

            dbg_msg += _TOGL_InnerKey::WinApiMouseButtonToStr(w_param, l_param);

            TOGL_LogDebug(dbg_msg);
        }
        HandleDoOnMouseKey(message, w_param, l_param);
        return 0;

    case WM_LBUTTONUP:
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_LBUTTONUP";

            dbg_msg += _TOGL_InnerKey::WinApiMouseButtonToStr(w_param, l_param);

            TOGL_LogDebug(dbg_msg);
        }
        HandleDoOnMouseKey(message, w_param, l_param);
        return 0;

    case WM_RBUTTONDOWN:   
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_RBUTTONDOWN";

            dbg_msg += _TOGL_InnerKey::WinApiMouseButtonToStr(w_param, l_param);

            TOGL_LogDebug(dbg_msg);
        }
        HandleDoOnMouseKey(message, w_param, l_param);
        return 0;

    case WM_RBUTTONUP: 
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_RBUTTONUP";

            dbg_msg += _TOGL_InnerKey::WinApiMouseButtonToStr(w_param, l_param);

            TOGL_LogDebug(dbg_msg);
        }
        HandleDoOnMouseKey(message, w_param, l_param);
        return 0;

    case WM_MBUTTONDOWN:   
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_MBUTTONDOWN";

            dbg_msg += _TOGL_InnerKey::WinApiMouseButtonToStr(w_param, l_param);

            TOGL_LogDebug(dbg_msg);
        }
        HandleDoOnMouseKey(message, w_param, l_param);
        return 0;

    case WM_MBUTTONUP: 
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_MBUTTONUP";

            dbg_msg += _TOGL_InnerKey::WinApiMouseButtonToStr(w_param, l_param);

            TOGL_LogDebug(dbg_msg);
        }
        HandleDoOnMouseKey(message, w_param, l_param);
        return 0;

    case WM_XBUTTONDOWN:
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_XBUTTONDOWN";

            if (HIWORD(w_param) == XBUTTON1) dbg_msg += " X1";
            if (HIWORD(w_param) == XBUTTON2) dbg_msg += " X2";

            dbg_msg += _TOGL_InnerKey::WinApiMouseButtonToStr(w_param, l_param);

            TOGL_LogDebug(dbg_msg);
        }
        HandleDoOnMouseKey(message, w_param, l_param);
        return 0;

    case WM_XBUTTONUP:
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_XBUTTONUP";

            if (HIWORD(w_param) == XBUTTON1) dbg_msg += " X1";
            if (HIWORD(w_param) == XBUTTON2) dbg_msg += " X2";

            dbg_msg += _TOGL_InnerKey::WinApiMouseButtonToStr(w_param, l_param);

            TOGL_LogDebug(dbg_msg);
        }
        HandleDoOnMouseKey(message, w_param, l_param);
        return 0;

    case WM_MOUSEWHEEL: {
        if (m_data.special_debug.is_notify_key_message) {
            std::string dbg_msg = "WM_MOUSEWHEEL";

            const int   delta   = GET_WHEEL_DELTA_WPARAM(w_param);
            const WORD  keys    = GET_KEYSTATE_WPARAM(w_param);

            POINT pos = {
                GET_X_LPARAM(l_param),
                GET_Y_LPARAM(l_param)
            };
            ScreenToClient(m_window_handle, &pos);


            dbg_msg += " delta=" + std::to_string(delta);
            dbg_msg += " x=" + std::to_string(pos.x);
            dbg_msg += " y=" + std::to_string(pos.y);

            if (keys & MK_CONTROL)   dbg_msg += " MK_CONTROL";
            if (keys & MK_LBUTTON)   dbg_msg += " MK_LBUTTON";
            if (keys & MK_MBUTTON)   dbg_msg += " MK_MBUTTON";
            if (keys & MK_RBUTTON)   dbg_msg += " MK_RBUTTON";
            if (keys & MK_SHIFT)     dbg_msg += " MK_SHIFT";
            if (keys & MK_XBUTTON1)  dbg_msg += " MK_XBUTTON1";
            if (keys & MK_XBUTTON2)  dbg_msg += " MK_XBUTTON2";

            TOGL_LogDebug(dbg_msg);
        }

        if (m_data.do_on_mouse_wheel_roll) {
            const int   delta   = GET_WHEEL_DELTA_WPARAM(w_param);

            POINT pos = {
                GET_X_LPARAM(l_param),
                GET_Y_LPARAM(l_param)
            };
            ScreenToClient(m_window_handle, &pos);

            m_data.do_on_mouse_wheel_roll(delta / WHEEL_DELTA, pos.x, pos.y);
        }
        return 0;
    }

    case WM_MOUSEMOVE: {
        if (m_data.special_debug.is_notify_mouse_move) {
            std::string dbg_msg = "WM_MOUSEMOVE";

            const int   x       = GET_X_LPARAM(l_param);
            const int   y       = GET_Y_LPARAM(l_param);
            const WORD  keys    = GET_KEYSTATE_WPARAM(w_param);

            dbg_msg += " x=" + std::to_string(x);
            dbg_msg += " y=" + std::to_string(y);

            if (keys & MK_CONTROL)   dbg_msg += " MK_CONTROL";
            if (keys & MK_LBUTTON)   dbg_msg += " MK_LBUTTON";
            if (keys & MK_MBUTTON)   dbg_msg += " MK_MBUTTON";
            if (keys & MK_RBUTTON)   dbg_msg += " MK_RBUTTON";
            if (keys & MK_SHIFT)     dbg_msg += " MK_SHIFT";
            if (keys & MK_XBUTTON1)  dbg_msg += " MK_XBUTTON1";
            if (keys & MK_XBUTTON2)  dbg_msg += " MK_XBUTTON2";

            TOGL_LogDebug(dbg_msg);
        }

        if (m_data.do_on_mouse_move) {
            m_data.do_on_mouse_move(GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param));
        }
        return 0;
    }
    } // switch

    return DefWindowProcW(window_handle, message, w_param, l_param);
}

inline LRESULT CALLBACK TOGL_Window::WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
    return TOGL_ToWindow().InnerWindowProc(window_handle, message, w_param, l_param);
}

//------------------------------------------------------------------------------

inline std::string TOGL_Window::WM_ToStr(UINT message) {
    switch (message) {
        TOGL_CASE_STR(WM_NULL);
        TOGL_CASE_STR(WM_CREATE);
        TOGL_CASE_STR(WM_DESTROY);
        TOGL_CASE_STR(WM_MOVE);
        TOGL_CASE_STR(WM_SIZE);
        TOGL_CASE_STR(WM_ACTIVATE);
        TOGL_CASE_STR(WM_SETFOCUS);
        TOGL_CASE_STR(WM_KILLFOCUS);
        TOGL_CASE_STR(WM_ENABLE);
        TOGL_CASE_STR(WM_SETREDRAW);
        TOGL_CASE_STR(WM_SETTEXT);
        TOGL_CASE_STR(WM_GETTEXT);
        TOGL_CASE_STR(WM_GETTEXTLENGTH);
        TOGL_CASE_STR(WM_PAINT);
        TOGL_CASE_STR(WM_CLOSE);
        TOGL_CASE_STR(WM_QUERYENDSESSION);
        TOGL_CASE_STR(WM_QUIT);
        TOGL_CASE_STR(WM_QUERYOPEN);
        TOGL_CASE_STR(WM_ERASEBKGND);
        TOGL_CASE_STR(WM_SYSCOLORCHANGE);
        TOGL_CASE_STR(WM_ENDSESSION);
        TOGL_CASE_STR(WM_SHOWWINDOW);
        //TOGL_CASE_STR(WM_CTLCOLOR);
        TOGL_CASE_STR(WM_WININICHANGE);
        TOGL_CASE_STR(WM_DEVMODECHANGE);
        TOGL_CASE_STR(WM_ACTIVATEAPP);
        TOGL_CASE_STR(WM_FONTCHANGE);
        TOGL_CASE_STR(WM_TIMECHANGE);
        TOGL_CASE_STR(WM_CANCELMODE);
        TOGL_CASE_STR(WM_SETCURSOR);
        TOGL_CASE_STR(WM_MOUSEACTIVATE);
        TOGL_CASE_STR(WM_CHILDACTIVATE);
        TOGL_CASE_STR(WM_QUEUESYNC);
        TOGL_CASE_STR(WM_GETMINMAXINFO);
        TOGL_CASE_STR(WM_PAINTICON);
        TOGL_CASE_STR(WM_ICONERASEBKGND);
        TOGL_CASE_STR(WM_NEXTDLGCTL);
        TOGL_CASE_STR(WM_SPOOLERSTATUS);
        TOGL_CASE_STR(WM_DRAWITEM);
        TOGL_CASE_STR(WM_MEASUREITEM);
        TOGL_CASE_STR(WM_DELETEITEM);
        TOGL_CASE_STR(WM_VKEYTOITEM);
        TOGL_CASE_STR(WM_CHARTOITEM);
        TOGL_CASE_STR(WM_SETFONT);
        TOGL_CASE_STR(WM_GETFONT);
        TOGL_CASE_STR(WM_SETHOTKEY);
        TOGL_CASE_STR(WM_GETHOTKEY);
        TOGL_CASE_STR(WM_QUERYDRAGICON);
        TOGL_CASE_STR(WM_COMPAREITEM);
        TOGL_CASE_STR(WM_GETOBJECT);
        TOGL_CASE_STR(WM_COMPACTING);
        TOGL_CASE_STR(WM_COMMNOTIFY);
        TOGL_CASE_STR(WM_WINDOWPOSCHANGING);
        TOGL_CASE_STR(WM_WINDOWPOSCHANGED);
        TOGL_CASE_STR(WM_POWER);
        //TOGL_CASE_STR(WM_COPYGLOBALDATA);
        TOGL_CASE_STR(WM_COPYDATA);
        TOGL_CASE_STR(WM_CANCELJOURNAL);
        TOGL_CASE_STR(WM_NOTIFY);
        TOGL_CASE_STR(WM_INPUTLANGCHANGEREQUEST);
        TOGL_CASE_STR(WM_INPUTLANGCHANGE);
        TOGL_CASE_STR(WM_TCARD);
        TOGL_CASE_STR(WM_HELP);
        TOGL_CASE_STR(WM_USERCHANGED);
        TOGL_CASE_STR(WM_NOTIFYFORMAT);
        TOGL_CASE_STR(WM_CONTEXTMENU);
        TOGL_CASE_STR(WM_STYLECHANGING);
        TOGL_CASE_STR(WM_STYLECHANGED);
        TOGL_CASE_STR(WM_DISPLAYCHANGE);
        TOGL_CASE_STR(WM_GETICON);
        TOGL_CASE_STR(WM_SETICON);
        TOGL_CASE_STR(WM_NCCREATE);
        TOGL_CASE_STR(WM_NCDESTROY);
        TOGL_CASE_STR(WM_NCCALCSIZE);
        TOGL_CASE_STR(WM_NCHITTEST);
        TOGL_CASE_STR(WM_NCPAINT);
        TOGL_CASE_STR(WM_NCACTIVATE);
        TOGL_CASE_STR(WM_GETDLGCODE);
        TOGL_CASE_STR(WM_SYNCPAINT);
        TOGL_CASE_STR(WM_NCMOUSEMOVE);
        TOGL_CASE_STR(WM_NCLBUTTONDOWN);
        TOGL_CASE_STR(WM_NCLBUTTONUP);
        TOGL_CASE_STR(WM_NCLBUTTONDBLCLK);
        TOGL_CASE_STR(WM_NCRBUTTONDOWN);
        TOGL_CASE_STR(WM_NCRBUTTONUP);
        TOGL_CASE_STR(WM_NCRBUTTONDBLCLK);
        TOGL_CASE_STR(WM_NCMBUTTONDOWN);
        TOGL_CASE_STR(WM_NCMBUTTONUP);
        TOGL_CASE_STR(WM_NCMBUTTONDBLCLK);
        TOGL_CASE_STR(WM_NCXBUTTONDOWN);
        TOGL_CASE_STR(WM_NCXBUTTONUP);
        TOGL_CASE_STR(WM_NCXBUTTONDBLCLK);
        TOGL_CASE_STR(EM_GETSEL);
        TOGL_CASE_STR(EM_SETSEL);
        TOGL_CASE_STR(EM_GETRECT);
        TOGL_CASE_STR(EM_SETRECT);
        TOGL_CASE_STR(EM_SETRECTNP);
        TOGL_CASE_STR(EM_SCROLL);
        TOGL_CASE_STR(EM_LINESCROLL);
        TOGL_CASE_STR(EM_SCROLLCARET);
        TOGL_CASE_STR(EM_GETMODIFY);
        TOGL_CASE_STR(EM_SETMODIFY);
        TOGL_CASE_STR(EM_GETLINECOUNT);
        TOGL_CASE_STR(EM_LINEINDEX);
        TOGL_CASE_STR(EM_SETHANDLE);
        TOGL_CASE_STR(EM_GETHANDLE);
        TOGL_CASE_STR(EM_GETTHUMB);
        TOGL_CASE_STR(EM_LINELENGTH);
        TOGL_CASE_STR(EM_REPLACESEL);
        //TOGL_CASE_STR(EM_SETFONT);
        TOGL_CASE_STR(EM_GETLINE);
        TOGL_CASE_STR(EM_LIMITTEXT);
        //TOGL_CASE_STR(EM_SETLIMITTEXT);
        TOGL_CASE_STR(EM_CANUNDO);
        TOGL_CASE_STR(EM_UNDO);
        TOGL_CASE_STR(EM_FMTLINES);
        TOGL_CASE_STR(EM_LINEFROMCHAR);
        //TOGL_CASE_STR(EM_SETWORDBREAK);
        TOGL_CASE_STR(EM_SETTABSTOPS);
        TOGL_CASE_STR(EM_SETPASSWORDCHAR);
        TOGL_CASE_STR(EM_EMPTYUNDOBUFFER);
        TOGL_CASE_STR(EM_GETFIRSTVISIBLELINE);
        TOGL_CASE_STR(EM_SETREADONLY);
        TOGL_CASE_STR(EM_SETWORDBREAKPROC);
        TOGL_CASE_STR(EM_GETWORDBREAKPROC);
        TOGL_CASE_STR(EM_GETPASSWORDCHAR);
        TOGL_CASE_STR(EM_SETMARGINS);
        TOGL_CASE_STR(EM_GETMARGINS);
        TOGL_CASE_STR(EM_GETLIMITTEXT);
        TOGL_CASE_STR(EM_POSFROMCHAR);
        TOGL_CASE_STR(EM_CHARFROMPOS);
        TOGL_CASE_STR(EM_SETIMESTATUS);
        TOGL_CASE_STR(EM_GETIMESTATUS);
        TOGL_CASE_STR(SBM_SETPOS);
        TOGL_CASE_STR(SBM_GETPOS);
        TOGL_CASE_STR(SBM_SETRANGE);
        TOGL_CASE_STR(SBM_GETRANGE);
        TOGL_CASE_STR(SBM_ENABLE_ARROWS);
        TOGL_CASE_STR(SBM_SETRANGEREDRAW);
        TOGL_CASE_STR(SBM_SETSCROLLINFO);
        TOGL_CASE_STR(SBM_GETSCROLLINFO);
        TOGL_CASE_STR(SBM_GETSCROLLBARINFO);
        TOGL_CASE_STR(BM_GETCHECK);
        TOGL_CASE_STR(BM_SETCHECK);
        TOGL_CASE_STR(BM_GETSTATE);
        TOGL_CASE_STR(BM_SETSTATE);
        TOGL_CASE_STR(BM_SETSTYLE);
        TOGL_CASE_STR(BM_CLICK);
        TOGL_CASE_STR(BM_GETIMAGE);
        TOGL_CASE_STR(BM_SETIMAGE);
        TOGL_CASE_STR(BM_SETDONTCLICK);
        TOGL_CASE_STR(WM_INPUT);
        TOGL_CASE_STR(WM_KEYDOWN);
        //TOGL_CASE_STR(WM_KEYFIRST);
        TOGL_CASE_STR(WM_KEYUP);
        TOGL_CASE_STR(WM_CHAR);
        TOGL_CASE_STR(WM_DEADCHAR);
        TOGL_CASE_STR(WM_SYSKEYDOWN);
        TOGL_CASE_STR(WM_SYSKEYUP);
        TOGL_CASE_STR(WM_SYSCHAR);
        TOGL_CASE_STR(WM_SYSDEADCHAR);
        //TOGL_CASE_STR(WM_UNICHAR / WM_KEYLAST);
        //TOGL_CASE_STR(WM_WNT_CONVERTREQUESTEX);
        //TOGL_CASE_STR(WM_CONVERTREQUEST);
        //TOGL_CASE_STR(WM_CONVERTRESULT);
        //TOGL_CASE_STR(WM_INTERIM);
        TOGL_CASE_STR(WM_IME_STARTCOMPOSITION);
        TOGL_CASE_STR(WM_IME_ENDCOMPOSITION);
        TOGL_CASE_STR(WM_IME_COMPOSITION);
        //TOGL_CASE_STR(WM_IME_KEYLAST);
        TOGL_CASE_STR(WM_INITDIALOG);
        TOGL_CASE_STR(WM_COMMAND);
        TOGL_CASE_STR(WM_SYSCOMMAND);
        TOGL_CASE_STR(WM_TIMER);
        TOGL_CASE_STR(WM_HSCROLL);
        TOGL_CASE_STR(WM_VSCROLL);
        TOGL_CASE_STR(WM_INITMENU);
        TOGL_CASE_STR(WM_INITMENUPOPUP);
        //TOGL_CASE_STR(WM_SYSTIMER);
        TOGL_CASE_STR(WM_MENUSELECT);
        TOGL_CASE_STR(WM_MENUCHAR);
        TOGL_CASE_STR(WM_ENTERIDLE);
        TOGL_CASE_STR(WM_MENURBUTTONUP);
        TOGL_CASE_STR(WM_MENUDRAG);
        TOGL_CASE_STR(WM_MENUGETOBJECT);
        TOGL_CASE_STR(WM_UNINITMENUPOPUP);
        TOGL_CASE_STR(WM_MENUCOMMAND);
        TOGL_CASE_STR(WM_CHANGEUISTATE);
        TOGL_CASE_STR(WM_UPDATEUISTATE);
        TOGL_CASE_STR(WM_QUERYUISTATE);
        //TOGL_CASE_STR(WM_LBTRACKPOINT);
        TOGL_CASE_STR(WM_CTLCOLORMSGBOX);
        TOGL_CASE_STR(WM_CTLCOLOREDIT);
        TOGL_CASE_STR(WM_CTLCOLORLISTBOX);
        TOGL_CASE_STR(WM_CTLCOLORBTN);
        TOGL_CASE_STR(WM_CTLCOLORDLG);
        TOGL_CASE_STR(WM_CTLCOLORSCROLLBAR);
        TOGL_CASE_STR(WM_CTLCOLORSTATIC);
        TOGL_CASE_STR(CB_GETEDITSEL);
        TOGL_CASE_STR(CB_LIMITTEXT);
        TOGL_CASE_STR(CB_SETEDITSEL);
        TOGL_CASE_STR(CB_ADDSTRING);
        TOGL_CASE_STR(CB_DELETESTRING);
        TOGL_CASE_STR(CB_DIR);
        TOGL_CASE_STR(CB_GETCOUNT);
        TOGL_CASE_STR(CB_GETCURSEL);
        TOGL_CASE_STR(CB_GETLBTEXT);
        TOGL_CASE_STR(CB_GETLBTEXTLEN);
        TOGL_CASE_STR(CB_INSERTSTRING);
        TOGL_CASE_STR(CB_RESETCONTENT);
        TOGL_CASE_STR(CB_FINDSTRING);
        TOGL_CASE_STR(CB_SELECTSTRING);
        TOGL_CASE_STR(CB_SETCURSEL);
        TOGL_CASE_STR(CB_SHOWDROPDOWN);
        TOGL_CASE_STR(CB_GETITEMDATA);
        TOGL_CASE_STR(CB_SETITEMDATA);
        TOGL_CASE_STR(CB_GETDROPPEDCONTROLRECT);
        TOGL_CASE_STR(CB_SETITEMHEIGHT);
        TOGL_CASE_STR(CB_GETITEMHEIGHT);
        TOGL_CASE_STR(CB_SETEXTENDEDUI);
        TOGL_CASE_STR(CB_GETEXTENDEDUI);
        TOGL_CASE_STR(CB_GETDROPPEDSTATE);
        TOGL_CASE_STR(CB_FINDSTRINGEXACT);
        TOGL_CASE_STR(CB_SETLOCALE);
        TOGL_CASE_STR(CB_GETLOCALE);
        TOGL_CASE_STR(CB_GETTOPINDEX);
        TOGL_CASE_STR(CB_SETTOPINDEX);
        TOGL_CASE_STR(CB_GETHORIZONTALEXTENT);
        TOGL_CASE_STR(CB_SETHORIZONTALEXTENT);
        TOGL_CASE_STR(CB_GETDROPPEDWIDTH);
        TOGL_CASE_STR(CB_SETDROPPEDWIDTH);
        TOGL_CASE_STR(CB_INITSTORAGE);
        //TOGL_CASE_STR(CB_MULTIPLEADDSTRING);
        TOGL_CASE_STR(CB_GETCOMBOBOXINFO);
        TOGL_CASE_STR(CB_MSGMAX);
        TOGL_CASE_STR(WM_MOUSEFIRST);
        //TOGL_CASE_STR(WM_MOUSEMOVE);
        TOGL_CASE_STR(WM_LBUTTONDOWN);
        TOGL_CASE_STR(WM_LBUTTONUP);
        TOGL_CASE_STR(WM_LBUTTONDBLCLK);
        TOGL_CASE_STR(WM_RBUTTONDOWN);
        TOGL_CASE_STR(WM_RBUTTONUP);
        TOGL_CASE_STR(WM_RBUTTONDBLCLK);
        TOGL_CASE_STR(WM_MBUTTONDOWN);
        TOGL_CASE_STR(WM_MBUTTONUP);
        TOGL_CASE_STR(WM_MBUTTONDBLCLK);
        TOGL_CASE_STR(WM_MOUSELAST);
        TOGL_CASE_STR(WM_MOUSEWHEEL);
        TOGL_CASE_STR(WM_XBUTTONDOWN);
        TOGL_CASE_STR(WM_XBUTTONUP);
        TOGL_CASE_STR(WM_XBUTTONDBLCLK);
        //TOGL_CASE_STR(WM_MOUSEHWHEEL);
        TOGL_CASE_STR(WM_PARENTNOTIFY);
        TOGL_CASE_STR(WM_ENTERMENULOOP);
        TOGL_CASE_STR(WM_EXITMENULOOP);
        TOGL_CASE_STR(WM_NEXTMENU);
        TOGL_CASE_STR(WM_SIZING);
        TOGL_CASE_STR(WM_CAPTURECHANGED);
        TOGL_CASE_STR(WM_MOVING);
        TOGL_CASE_STR(WM_POWERBROADCAST);
        TOGL_CASE_STR(WM_DEVICECHANGE);
        TOGL_CASE_STR(WM_MDICREATE);
        TOGL_CASE_STR(WM_MDIDESTROY);
        TOGL_CASE_STR(WM_MDIACTIVATE);
        TOGL_CASE_STR(WM_MDIRESTORE);
        TOGL_CASE_STR(WM_MDINEXT);
        TOGL_CASE_STR(WM_MDIMAXIMIZE);
        TOGL_CASE_STR(WM_MDITILE);
        TOGL_CASE_STR(WM_MDICASCADE);
        TOGL_CASE_STR(WM_MDIICONARRANGE);
        TOGL_CASE_STR(WM_MDIGETACTIVE);
        TOGL_CASE_STR(WM_MDISETMENU);
        TOGL_CASE_STR(WM_ENTERSIZEMOVE);
        TOGL_CASE_STR(WM_EXITSIZEMOVE);
        TOGL_CASE_STR(WM_DROPFILES);
        TOGL_CASE_STR(WM_MDIREFRESHMENU);
        //TOGL_CASE_STR(WM_IME_REPORT);
        TOGL_CASE_STR(WM_IME_SETCONTEXT);
        TOGL_CASE_STR(WM_IME_NOTIFY);
        TOGL_CASE_STR(WM_IME_CONTROL);
        TOGL_CASE_STR(WM_IME_COMPOSITIONFULL);
        TOGL_CASE_STR(WM_IME_SELECT);
        TOGL_CASE_STR(WM_IME_CHAR);
        TOGL_CASE_STR(WM_IME_REQUEST);
        //TOGL_CASE_STR(WM_IMEKEYDOWN);
        TOGL_CASE_STR(WM_IME_KEYDOWN);
        //TOGL_CASE_STR(WM_IMEKEYUP);
        TOGL_CASE_STR(WM_IME_KEYUP);
        TOGL_CASE_STR(WM_NCMOUSEHOVER);
        TOGL_CASE_STR(WM_MOUSEHOVER);
        TOGL_CASE_STR(WM_NCMOUSELEAVE);
        TOGL_CASE_STR(WM_MOUSELEAVE);
        TOGL_CASE_STR(WM_CUT);
        TOGL_CASE_STR(WM_COPY);
        TOGL_CASE_STR(WM_PASTE);
        TOGL_CASE_STR(WM_CLEAR);
        TOGL_CASE_STR(WM_UNDO);
        TOGL_CASE_STR(WM_RENDERFORMAT);
        TOGL_CASE_STR(WM_RENDERALLFORMATS);
        TOGL_CASE_STR(WM_DESTROYCLIPBOARD);
        TOGL_CASE_STR(WM_DRAWCLIPBOARD);
        TOGL_CASE_STR(WM_PAINTCLIPBOARD);
        TOGL_CASE_STR(WM_VSCROLLCLIPBOARD);
        TOGL_CASE_STR(WM_SIZECLIPBOARD);
        TOGL_CASE_STR(WM_ASKCBFORMATNAME);
        TOGL_CASE_STR(WM_CHANGECBCHAIN);
        TOGL_CASE_STR(WM_HSCROLLCLIPBOARD);
        TOGL_CASE_STR(WM_QUERYNEWPALETTE);
        TOGL_CASE_STR(WM_PALETTEISCHANGING);
        TOGL_CASE_STR(WM_PALETTECHANGED);
        TOGL_CASE_STR(WM_HOTKEY);
        TOGL_CASE_STR(WM_PRINT);
        TOGL_CASE_STR(WM_PRINTCLIENT);
        TOGL_CASE_STR(WM_APPCOMMAND);
        TOGL_CASE_STR(WM_HANDHELDFIRST);
        TOGL_CASE_STR(WM_HANDHELDLAST);
        TOGL_CASE_STR(WM_AFXFIRST);
        TOGL_CASE_STR(WM_AFXLAST);
        TOGL_CASE_STR(WM_PENWINFIRST);
        //TOGL_CASE_STR(WM_RCRESULT);
        //TOGL_CASE_STR(WM_HOOKRCRESULT);
        //TOGL_CASE_STR(WM_GLOBALRCCHANGE);
        //TOGL_CASE_STR(WM_PENMISCINFO);
        //TOGL_CASE_STR(WM_SKB);
        //TOGL_CASE_STR(WM_HEDITCTL);
        //TOGL_CASE_STR(WM_PENCTL);
        //TOGL_CASE_STR(WM_PENMISC);
        //TOGL_CASE_STR(WM_CTLINIT);
        //TOGL_CASE_STR(WM_PENEVENT);
        TOGL_CASE_STR(WM_PENWINLAST);
        //TOGL_CASE_STR(DDM_SETFMT);
        TOGL_CASE_STR(DM_GETDEFID);
        //TOGL_CASE_STR(NIN_SELECT);
        //TOGL_CASE_STR(TBM_GETPOS);
        //TOGL_CASE_STR(WM_PSD_PAGESETUPDLG);
        //TOGL_CASE_STR(WM_USER);
        //TOGL_CASE_STR(CBEM_INSERTITEMA);
        //TOGL_CASE_STR(DDM_DRAW);
        TOGL_CASE_STR(DM_SETDEFID);
        //TOGL_CASE_STR(HKM_SETHOTKEY);
        //TOGL_CASE_STR(PBM_SETRANGE);
        //TOGL_CASE_STR(RB_INSERTBANDA);
        //TOGL_CASE_STR(SB_SETTEXTA);
        //TOGL_CASE_STR(TB_ENABLEBUTTON);
        //TOGL_CASE_STR(TBM_GETRANGEMIN);
        //TOGL_CASE_STR(TTM_ACTIVATE);
        //TOGL_CASE_STR(WM_CHOOSEFONT_GETLOGFONT);
        //TOGL_CASE_STR(WM_PSD_FULLPAGERECT);
        //TOGL_CASE_STR(CBEM_SETIMAGELIST);
        //TOGL_CASE_STR(DDM_CLOSE);
        TOGL_CASE_STR(DM_REPOSITION);
        //TOGL_CASE_STR(HKM_GETHOTKEY);
        //TOGL_CASE_STR(PBM_SETPOS);
        //TOGL_CASE_STR(RB_DELETEBAND);
        //TOGL_CASE_STR(SB_GETTEXTA);
        //TOGL_CASE_STR(TB_CHECKBUTTON);
        //TOGL_CASE_STR(TBM_GETRANGEMAX);
        //TOGL_CASE_STR(WM_PSD_MINMARGINRECT);
        //TOGL_CASE_STR(CBEM_GETIMAGELIST);
        //TOGL_CASE_STR(DDM_BEGIN);
        //TOGL_CASE_STR(HKM_SETRULES);
        //TOGL_CASE_STR(PBM_DELTAPOS);
        //TOGL_CASE_STR(RB_GETBARINFO);
        //TOGL_CASE_STR(SB_GETTEXTLENGTHA);
        //TOGL_CASE_STR(TBM_GETTIC);
        //TOGL_CASE_STR(TB_PRESSBUTTON);
        //TOGL_CASE_STR(TTM_SETDELAYTIME);
        //TOGL_CASE_STR(WM_PSD_MARGINRECT);
        //TOGL_CASE_STR(CBEM_GETITEMA);
        //TOGL_CASE_STR(DDM_END);
        //TOGL_CASE_STR(PBM_SETSTEP);
        //TOGL_CASE_STR(RB_SETBARINFO);
        //TOGL_CASE_STR(SB_SETPARTS);
        //TOGL_CASE_STR(TB_HIDEBUTTON);
        //TOGL_CASE_STR(TBM_SETTIC);
        //TOGL_CASE_STR(TTM_ADDTOOLA);
        //TOGL_CASE_STR(WM_PSD_GREEKTEXTRECT);
        //TOGL_CASE_STR(CBEM_SETITEMA);
        //TOGL_CASE_STR(PBM_STEPIT);
        //TOGL_CASE_STR(TB_INDETERMINATE);
        //TOGL_CASE_STR(TBM_SETPOS);
        //TOGL_CASE_STR(TTM_DELTOOLA);
        //TOGL_CASE_STR(WM_PSD_ENVSTAMPRECT);
        //TOGL_CASE_STR(CBEM_GETCOMBOCONTROL);
        //TOGL_CASE_STR(PBM_SETRANGE32);
        //TOGL_CASE_STR(RB_SETBANDINFOA);
        //TOGL_CASE_STR(SB_GETPARTS);
        //TOGL_CASE_STR(TB_MARKBUTTON);
        //TOGL_CASE_STR(TBM_SETRANGE);
        //TOGL_CASE_STR(TTM_NEWTOOLRECTA);
        //TOGL_CASE_STR(WM_PSD_YAFULLPAGERECT);
        //TOGL_CASE_STR(CBEM_GETEDITCONTROL);
        //TOGL_CASE_STR(PBM_GETRANGE);
        //TOGL_CASE_STR(RB_SETPARENT);
        //TOGL_CASE_STR(SB_GETBORDERS);
        //TOGL_CASE_STR(TBM_SETRANGEMIN);
        //TOGL_CASE_STR(TTM_RELAYEVENT);
        //TOGL_CASE_STR(CBEM_SETEXSTYLE);
        //TOGL_CASE_STR(PBM_GETPOS);
        //TOGL_CASE_STR(RB_HITTEST);
        //TOGL_CASE_STR(SB_SETMINHEIGHT);
        //TOGL_CASE_STR(TBM_SETRANGEMAX);
        //TOGL_CASE_STR(TTM_GETTOOLINFOA);
        //TOGL_CASE_STR(CBEM_GETEXSTYLE);
        //TOGL_CASE_STR(CBEM_GETEXTENDEDSTYLE);
        //TOGL_CASE_STR(PBM_SETBARCOLOR);
        //TOGL_CASE_STR(RB_GETRECT);
        //TOGL_CASE_STR(SB_SIMPLE);
        //TOGL_CASE_STR(TB_ISBUTTONENABLED);
        //TOGL_CASE_STR(TBM_CLEARTICS);
        //TOGL_CASE_STR(TTM_SETTOOLINFOA);
        //TOGL_CASE_STR(CBEM_HASEDITCHANGED);
        //TOGL_CASE_STR(RB_INSERTBANDW);
        //TOGL_CASE_STR(SB_GETRECT);
        //TOGL_CASE_STR(TB_ISBUTTONCHECKED);
        //TOGL_CASE_STR(TBM_SETSEL);
        //TOGL_CASE_STR(TTM_HITTESTA);
        //TOGL_CASE_STR(WIZ_QUERYNUMPAGES);
        //TOGL_CASE_STR(CBEM_INSERTITEMW);
        //TOGL_CASE_STR(RB_SETBANDINFOW);
        //TOGL_CASE_STR(SB_SETTEXTW);
        //TOGL_CASE_STR(TB_ISBUTTONPRESSED);
        //TOGL_CASE_STR(TBM_SETSELSTART);
        //TOGL_CASE_STR(TTM_GETTEXTA);
        //TOGL_CASE_STR(WIZ_NEXT);
        //TOGL_CASE_STR(CBEM_SETITEMW);
        //TOGL_CASE_STR(RB_GETBANDCOUNT);
        //TOGL_CASE_STR(SB_GETTEXTLENGTHW);
        //TOGL_CASE_STR(TB_ISBUTTONHIDDEN);
        //TOGL_CASE_STR(TBM_SETSELEND);
        //TOGL_CASE_STR(TTM_UPDATETIPTEXTA);
        //TOGL_CASE_STR(WIZ_PREV);
        //TOGL_CASE_STR(CBEM_GETITEMW);
        //TOGL_CASE_STR(RB_GETROWCOUNT);
        //TOGL_CASE_STR(SB_GETTEXTW);
        //TOGL_CASE_STR(TB_ISBUTTONINDETERMINATE);
        //TOGL_CASE_STR(TTM_GETTOOLCOUNT);
        //TOGL_CASE_STR(CBEM_SETEXTENDEDSTYLE);
        //TOGL_CASE_STR(RB_GETROWHEIGHT);
        //TOGL_CASE_STR(SB_ISSIMPLE);
        //TOGL_CASE_STR(TB_ISBUTTONHIGHLIGHTED);
        //TOGL_CASE_STR(TBM_GETPTICS);
        //TOGL_CASE_STR(TTM_ENUMTOOLSA);
        //TOGL_CASE_STR(SB_SETICON);
        //TOGL_CASE_STR(TBM_GETTICPOS);
        //TOGL_CASE_STR(TTM_GETCURRENTTOOLA);
        //TOGL_CASE_STR(RB_IDTOINDEX);
        //TOGL_CASE_STR(SB_SETTIPTEXTA);
        //TOGL_CASE_STR(TBM_GETNUMTICS);
        //TOGL_CASE_STR(TTM_WINDOWFROMPOINT);
        //TOGL_CASE_STR(RB_GETTOOLTIPS);
        //TOGL_CASE_STR(SB_SETTIPTEXTW);
        //TOGL_CASE_STR(TBM_GETSELSTART);
        //TOGL_CASE_STR(TB_SETSTATE);
        //TOGL_CASE_STR(TTM_TRACKACTIVATE);
        //TOGL_CASE_STR(RB_SETTOOLTIPS);
        //TOGL_CASE_STR(SB_GETTIPTEXTA);
        //TOGL_CASE_STR(TB_GETSTATE);
        //TOGL_CASE_STR(TBM_GETSELEND);
        //TOGL_CASE_STR(TTM_TRACKPOSITION);
        //TOGL_CASE_STR(RB_SETBKCOLOR);
        //TOGL_CASE_STR(SB_GETTIPTEXTW);
        //TOGL_CASE_STR(TB_ADDBITMAP);
        //TOGL_CASE_STR(TBM_CLEARSEL);
        //TOGL_CASE_STR(TTM_SETTIPBKCOLOR);
        //TOGL_CASE_STR(RB_GETBKCOLOR);
        //TOGL_CASE_STR(SB_GETICON);
        //TOGL_CASE_STR(TB_ADDBUTTONSA);
        //TOGL_CASE_STR(TBM_SETTICFREQ);
        //TOGL_CASE_STR(TTM_SETTIPTEXTCOLOR);
        //TOGL_CASE_STR(RB_SETTEXTCOLOR);
        //TOGL_CASE_STR(TB_INSERTBUTTONA);
        //TOGL_CASE_STR(TBM_SETPAGESIZE);
        //TOGL_CASE_STR(TTM_GETDELAYTIME);
        //TOGL_CASE_STR(RB_GETTEXTCOLOR);
        //TOGL_CASE_STR(TB_DELETEBUTTON);
        //TOGL_CASE_STR(TBM_GETPAGESIZE);
        //TOGL_CASE_STR(TTM_GETTIPBKCOLOR);
        //TOGL_CASE_STR(RB_SIZETORECT);
        //TOGL_CASE_STR(TB_GETBUTTON);
        //TOGL_CASE_STR(TBM_SETLINESIZE);
        //TOGL_CASE_STR(TTM_GETTIPTEXTCOLOR);
        //TOGL_CASE_STR(RB_BEGINDRAG);
        //TOGL_CASE_STR(TB_BUTTONCOUNT);
        //TOGL_CASE_STR(TBM_GETLINESIZE);
        //TOGL_CASE_STR(TTM_SETMAXTIPWIDTH);
        //TOGL_CASE_STR(RB_ENDDRAG);
        //TOGL_CASE_STR(TB_COMMANDTOINDEX);
        //TOGL_CASE_STR(TBM_GETTHUMBRECT);
        //TOGL_CASE_STR(TTM_GETMAXTIPWIDTH);
        //TOGL_CASE_STR(RB_DRAGMOVE);
        //TOGL_CASE_STR(TBM_GETCHANNELRECT);
        //TOGL_CASE_STR(TB_SAVERESTOREA);
        //TOGL_CASE_STR(TTM_SETMARGIN);
        //TOGL_CASE_STR(RB_GETBARHEIGHT);
        //TOGL_CASE_STR(TB_CUSTOMIZE);
        //TOGL_CASE_STR(TBM_SETTHUMBLENGTH);
        //TOGL_CASE_STR(TTM_GETMARGIN);
        //TOGL_CASE_STR(RB_GETBANDINFOW);
        //TOGL_CASE_STR(TB_ADDSTRINGA);
        //TOGL_CASE_STR(TBM_GETTHUMBLENGTH);
        //TOGL_CASE_STR(TTM_POP);
        //TOGL_CASE_STR(RB_GETBANDINFOA);
        //TOGL_CASE_STR(TB_GETITEMRECT);
        //TOGL_CASE_STR(TBM_SETTOOLTIPS);
        //TOGL_CASE_STR(TTM_UPDATE);
        //TOGL_CASE_STR(RB_MINIMIZEBAND);
        //TOGL_CASE_STR(TB_BUTTONSTRUCTSIZE);
        //TOGL_CASE_STR(TBM_GETTOOLTIPS);
        //TOGL_CASE_STR(TTM_GETBUBBLESIZE);
        //TOGL_CASE_STR(RB_MAXIMIZEBAND);
        //TOGL_CASE_STR(TBM_SETTIPSIDE);
        //TOGL_CASE_STR(TB_SETBUTTONSIZE);
        //TOGL_CASE_STR(TTM_ADJUSTRECT);
        //TOGL_CASE_STR(TBM_SETBUDDY);
        //TOGL_CASE_STR(TB_SETBITMAPSIZE);
        //TOGL_CASE_STR(TTM_SETTITLEA);
        //TOGL_CASE_STR(MSG_FTS_JUMP_VA);
        //TOGL_CASE_STR(TB_AUTOSIZE);
        //TOGL_CASE_STR(TBM_GETBUDDY);
        //TOGL_CASE_STR(TTM_SETTITLEW);
        //TOGL_CASE_STR(RB_GETBANDBORDERS);
        //TOGL_CASE_STR(MSG_FTS_JUMP_QWORD);
        //TOGL_CASE_STR(RB_SHOWBAND);
        //TOGL_CASE_STR(TB_GETTOOLTIPS);
        //TOGL_CASE_STR(MSG_REINDEX_REQUEST);
        //TOGL_CASE_STR(TB_SETTOOLTIPS);
        //TOGL_CASE_STR(MSG_FTS_WHERE_IS_IT);
        //TOGL_CASE_STR(RB_SETPALETTE);
        //TOGL_CASE_STR(TB_SETPARENT);
        //TOGL_CASE_STR(RB_GETPALETTE);
        //TOGL_CASE_STR(RB_MOVEBAND);
        //TOGL_CASE_STR(TB_SETROWS);
        //TOGL_CASE_STR(TB_GETROWS);
        //TOGL_CASE_STR(TB_GETBITMAPFLAGS);
        //TOGL_CASE_STR(TB_SETCMDID);
        //TOGL_CASE_STR(RB_PUSHCHEVRON);
        //TOGL_CASE_STR(TB_CHANGEBITMAP);
        //TOGL_CASE_STR(TB_GETBITMAP);
        //TOGL_CASE_STR(MSG_GET_DEFFONT);
        //TOGL_CASE_STR(TB_GETBUTTONTEXTA);
        //TOGL_CASE_STR(TB_REPLACEBITMAP);
        //TOGL_CASE_STR(TB_SETINDENT);
        //TOGL_CASE_STR(TB_SETIMAGELIST);
        //TOGL_CASE_STR(TB_GETIMAGELIST);
        //TOGL_CASE_STR(TB_LOADIMAGES);
        //TOGL_CASE_STR(EM_CANPASTE);
        //TOGL_CASE_STR(TTM_ADDTOOLW);
        //TOGL_CASE_STR(EM_DISPLAYBAND);
        //TOGL_CASE_STR(TB_GETRECT);
        //TOGL_CASE_STR(TTM_DELTOOLW);
        //TOGL_CASE_STR(EM_EXGETSEL);
        //TOGL_CASE_STR(TB_SETHOTIMAGELIST);
        //TOGL_CASE_STR(TTM_NEWTOOLRECTW);
        //TOGL_CASE_STR(EM_EXLIMITTEXT);
        //TOGL_CASE_STR(TB_GETHOTIMAGELIST);
        //TOGL_CASE_STR(TTM_GETTOOLINFOW);
        //TOGL_CASE_STR(EM_EXLINEFROMCHAR);
        //TOGL_CASE_STR(TB_SETDISABLEDIMAGELIST);
        //TOGL_CASE_STR(TTM_SETTOOLINFOW);
        //TOGL_CASE_STR(EM_EXSETSEL);
        //TOGL_CASE_STR(TB_GETDISABLEDIMAGELIST);
        //TOGL_CASE_STR(TTM_HITTESTW);
        //TOGL_CASE_STR(EM_FINDTEXT);
        //TOGL_CASE_STR(TB_SETSTYLE);
        //TOGL_CASE_STR(TTM_GETTEXTW);
        //TOGL_CASE_STR(EM_FORMATRANGE);
        //TOGL_CASE_STR(TB_GETSTYLE);
        //TOGL_CASE_STR(TTM_UPDATETIPTEXTW);
        //TOGL_CASE_STR(EM_GETCHARFORMAT);
        //TOGL_CASE_STR(TB_GETBUTTONSIZE);
        //TOGL_CASE_STR(TTM_ENUMTOOLSW);
        //TOGL_CASE_STR(EM_GETEVENTMASK);
        //TOGL_CASE_STR(TB_SETBUTTONWIDTH);
        //TOGL_CASE_STR(TTM_GETCURRENTTOOLW);
        //TOGL_CASE_STR(EM_GETOLEINTERFACE);
        //TOGL_CASE_STR(TB_SETMAXTEXTROWS);
        //TOGL_CASE_STR(EM_GETPARAFORMAT);
        //TOGL_CASE_STR(TB_GETTEXTROWS);
        //TOGL_CASE_STR(EM_GETSELTEXT);
        //TOGL_CASE_STR(TB_GETOBJECT);
        //TOGL_CASE_STR(EM_HIDESELECTION);
        //TOGL_CASE_STR(TB_GETBUTTONINFOW);
        //TOGL_CASE_STR(EM_PASTESPECIAL);
        //TOGL_CASE_STR(TB_SETBUTTONINFOW);
        //TOGL_CASE_STR(EM_REQUESTRESIZE);
        //TOGL_CASE_STR(TB_GETBUTTONINFOA);
        //TOGL_CASE_STR(EM_SELECTIONTYPE);
        //TOGL_CASE_STR(TB_SETBUTTONINFOA);
        //TOGL_CASE_STR(EM_SETBKGNDCOLOR);
        //TOGL_CASE_STR(TB_INSERTBUTTONW);
        //TOGL_CASE_STR(EM_SETCHARFORMAT);
        //TOGL_CASE_STR(TB_ADDBUTTONSW);
        //TOGL_CASE_STR(EM_SETEVENTMASK);
        //TOGL_CASE_STR(TB_HITTEST);
        //TOGL_CASE_STR(EM_SETOLECALLBACK);
        //TOGL_CASE_STR(TB_SETDRAWTEXTFLAGS);
        //TOGL_CASE_STR(EM_SETPARAFORMAT);
        //TOGL_CASE_STR(TB_GETHOTITEM);
        //TOGL_CASE_STR(EM_SETTARGETDEVICE);
        //TOGL_CASE_STR(TB_SETHOTITEM);
        //TOGL_CASE_STR(EM_STREAMIN);
        //TOGL_CASE_STR(TB_SETANCHORHIGHLIGHT);
        //TOGL_CASE_STR(EM_STREAMOUT);
        //TOGL_CASE_STR(TB_GETANCHORHIGHLIGHT);
        //TOGL_CASE_STR(EM_GETTEXTRANGE);
        //TOGL_CASE_STR(TB_GETBUTTONTEXTW);
        //TOGL_CASE_STR(EM_FINDWORDBREAK);
        //TOGL_CASE_STR(TB_SAVERESTOREW);
        //TOGL_CASE_STR(EM_SETOPTIONS);
        //TOGL_CASE_STR(TB_ADDSTRINGW);
        //TOGL_CASE_STR(EM_GETOPTIONS);
        //TOGL_CASE_STR(TB_MAPACCELERATORA);
        //TOGL_CASE_STR(EM_FINDTEXTEX);
        //TOGL_CASE_STR(TB_GETINSERTMARK);
        //TOGL_CASE_STR(EM_GETWORDBREAKPROCEX);
        //TOGL_CASE_STR(TB_SETINSERTMARK);
        //TOGL_CASE_STR(EM_SETWORDBREAKPROCEX);
        //TOGL_CASE_STR(TB_INSERTMARKHITTEST);
        //TOGL_CASE_STR(EM_SETUNDOLIMIT);
        //TOGL_CASE_STR(TB_MOVEBUTTON);
        //TOGL_CASE_STR(TB_GETMAXSIZE);
        //TOGL_CASE_STR(EM_REDO);
        //TOGL_CASE_STR(TB_SETEXTENDEDSTYLE);
        //TOGL_CASE_STR(EM_CANREDO);
        //TOGL_CASE_STR(TB_GETEXTENDEDSTYLE);
        //TOGL_CASE_STR(EM_GETUNDONAME);
        //TOGL_CASE_STR(TB_GETPADDING);
        //TOGL_CASE_STR(EM_GETREDONAME);
        //TOGL_CASE_STR(TB_SETPADDING);
        //TOGL_CASE_STR(EM_STOPGROUPTYPING);
        //TOGL_CASE_STR(TB_SETINSERTMARKCOLOR);
        //TOGL_CASE_STR(EM_SETTEXTMODE);
        //TOGL_CASE_STR(TB_GETINSERTMARKCOLOR);
        //TOGL_CASE_STR(EM_GETTEXTMODE);
        //TOGL_CASE_STR(TB_MAPACCELERATORW);
        //TOGL_CASE_STR(EM_AUTOURLDETECT);
        //TOGL_CASE_STR(TB_GETSTRINGW);
        //TOGL_CASE_STR(EM_GETAUTOURLDETECT);
        //TOGL_CASE_STR(TB_GETSTRINGA);
        //TOGL_CASE_STR(EM_SETPALETTE);
        //TOGL_CASE_STR(EM_GETTEXTEX);
        //TOGL_CASE_STR(EM_GETTEXTLENGTHEX);
        //TOGL_CASE_STR(EM_SHOWSCROLLBAR);
        //TOGL_CASE_STR(EM_SETTEXTEX);
        //TOGL_CASE_STR(TAPI_REPLY);
        //TOGL_CASE_STR(ACM_OPENA);
        //TOGL_CASE_STR(BFFM_SETSTATUSTEXTA);
        //TOGL_CASE_STR(CDM_FIRST);
        //TOGL_CASE_STR(CDM_GETSPEC);
        //TOGL_CASE_STR(EM_SETPUNCTUATION);
        //TOGL_CASE_STR(IPM_CLEARADDRESS);
        //TOGL_CASE_STR(WM_CAP_UNICODE_START);
        //TOGL_CASE_STR(ACM_PLAY);
        //TOGL_CASE_STR(BFFM_ENABLEOK);
        //TOGL_CASE_STR(CDM_GETFILEPATH);
        //TOGL_CASE_STR(EM_GETPUNCTUATION);
        //TOGL_CASE_STR(IPM_SETADDRESS);
        //TOGL_CASE_STR(PSM_SETCURSEL);
        //TOGL_CASE_STR(UDM_SETRANGE);
        //TOGL_CASE_STR(WM_CHOOSEFONT_SETLOGFONT);
        //TOGL_CASE_STR(ACM_STOP);
        //TOGL_CASE_STR(BFFM_SETSELECTIONA);
        //TOGL_CASE_STR(CDM_GETFOLDERPATH);
        //TOGL_CASE_STR(EM_SETWORDWRAPMODE);
        //TOGL_CASE_STR(IPM_GETADDRESS);
        //TOGL_CASE_STR(PSM_REMOVEPAGE);
        //TOGL_CASE_STR(UDM_GETRANGE);
        //TOGL_CASE_STR(WM_CAP_SET_CALLBACK_ERRORW);
        //TOGL_CASE_STR(WM_CHOOSEFONT_SETFLAGS);
        //TOGL_CASE_STR(ACM_OPENW);
        //TOGL_CASE_STR(BFFM_SETSELECTIONW);
        //TOGL_CASE_STR(CDM_GETFOLDERIDLIST);
        //TOGL_CASE_STR(EM_GETWORDWRAPMODE);
        //TOGL_CASE_STR(IPM_SETRANGE);
        //TOGL_CASE_STR(PSM_ADDPAGE);
        //TOGL_CASE_STR(UDM_SETPOS);
        //TOGL_CASE_STR(WM_CAP_SET_CALLBACK_STATUSW);
        //TOGL_CASE_STR(BFFM_SETSTATUSTEXTW);
        //TOGL_CASE_STR(CDM_SETCONTROLTEXT);
        //TOGL_CASE_STR(EM_SETIMECOLOR);
        //TOGL_CASE_STR(IPM_SETFOCUS);
        //TOGL_CASE_STR(PSM_CHANGED);
        //TOGL_CASE_STR(UDM_GETPOS);
        //TOGL_CASE_STR(CDM_HIDECONTROL);
        //TOGL_CASE_STR(EM_GETIMECOLOR);
        //TOGL_CASE_STR(IPM_ISBLANK);
        //TOGL_CASE_STR(PSM_RESTARTWINDOWS);
        //TOGL_CASE_STR(UDM_SETBUDDY);
        //TOGL_CASE_STR(CDM_SETDEFEXT);
        //TOGL_CASE_STR(EM_SETIMEOPTIONS);
        //TOGL_CASE_STR(PSM_REBOOTSYSTEM);
        //TOGL_CASE_STR(UDM_GETBUDDY);
        //TOGL_CASE_STR(EM_GETIMEOPTIONS);
        //TOGL_CASE_STR(PSM_CANCELTOCLOSE);
        //TOGL_CASE_STR(UDM_SETACCEL);
        //TOGL_CASE_STR(EM_CONVPOSITION);
        //TOGL_CASE_STR(EM_CONVPOSITION);
        //TOGL_CASE_STR(PSM_QUERYSIBLINGS);
        //TOGL_CASE_STR(UDM_GETACCEL);
        //TOGL_CASE_STR(MCIWNDM_GETZOOM);
        //TOGL_CASE_STR(PSM_UNCHANGED);
        //TOGL_CASE_STR(UDM_SETBASE);
        //TOGL_CASE_STR(PSM_APPLY);
        //TOGL_CASE_STR(UDM_GETBASE);
        //TOGL_CASE_STR(PSM_SETTITLEA);
        //TOGL_CASE_STR(UDM_SETRANGE32);
        //TOGL_CASE_STR(PSM_SETWIZBUTTONS);
        //TOGL_CASE_STR(UDM_GETRANGE32);
        //TOGL_CASE_STR(WM_CAP_DRIVER_GET_NAMEW);
        //TOGL_CASE_STR(PSM_PRESSBUTTON);
        //TOGL_CASE_STR(UDM_SETPOS32);
        //TOGL_CASE_STR(WM_CAP_DRIVER_GET_VERSIONW);
        //TOGL_CASE_STR(PSM_SETCURSELID);
        //TOGL_CASE_STR(UDM_GETPOS32);
        //TOGL_CASE_STR(PSM_SETFINISHTEXTA);
        //TOGL_CASE_STR(PSM_GETTABCONTROL);
        //TOGL_CASE_STR(PSM_ISDIALOGMESSAGE);
        //TOGL_CASE_STR(MCIWNDM_REALIZE);
        //TOGL_CASE_STR(PSM_GETCURRENTPAGEHWND);
        //TOGL_CASE_STR(MCIWNDM_SETTIMEFORMATA);
        //TOGL_CASE_STR(PSM_INSERTPAGE);
        //TOGL_CASE_STR(EM_SETLANGOPTIONS);
        //TOGL_CASE_STR(MCIWNDM_GETTIMEFORMATA);
        //TOGL_CASE_STR(PSM_SETTITLEW);
        //TOGL_CASE_STR(WM_CAP_FILE_SET_CAPTURE_FILEW);
        //TOGL_CASE_STR(EM_GETLANGOPTIONS);
        //TOGL_CASE_STR(MCIWNDM_VALIDATEMEDIA);
        //TOGL_CASE_STR(PSM_SETFINISHTEXTW);
        //TOGL_CASE_STR(WM_CAP_FILE_GET_CAPTURE_FILEW);
        //TOGL_CASE_STR(EM_GETIMECOMPMODE);
        //TOGL_CASE_STR(EM_FINDTEXTW);
        //TOGL_CASE_STR(MCIWNDM_PLAYTO);
        //TOGL_CASE_STR(WM_CAP_FILE_SAVEASW);
        //TOGL_CASE_STR(EM_FINDTEXTEXW);
        //TOGL_CASE_STR(MCIWNDM_GETFILENAMEA);
        //TOGL_CASE_STR(EM_RECONVERSION);
        //TOGL_CASE_STR(MCIWNDM_GETDEVICEA);
        //TOGL_CASE_STR(PSM_SETHEADERTITLEA);
        //TOGL_CASE_STR(WM_CAP_FILE_SAVEDIBW);
        //TOGL_CASE_STR(EM_SETIMEMODEBIAS);
        //TOGL_CASE_STR(MCIWNDM_GETPALETTE);
        //TOGL_CASE_STR(PSM_SETHEADERTITLEW);
        //TOGL_CASE_STR(EM_GETIMEMODEBIAS);
        //TOGL_CASE_STR(MCIWNDM_SETPALETTE);
        //TOGL_CASE_STR(PSM_SETHEADERSUBTITLEA);
        //TOGL_CASE_STR(MCIWNDM_GETERRORA);
        //TOGL_CASE_STR(PSM_SETHEADERSUBTITLEW);
        //TOGL_CASE_STR(PSM_HWNDTOINDEX);
        //TOGL_CASE_STR(PSM_INDEXTOHWND);
        //TOGL_CASE_STR(MCIWNDM_SETINACTIVETIMER);
        //TOGL_CASE_STR(PSM_PAGETOINDEX);
        //TOGL_CASE_STR(PSM_INDEXTOPAGE);
        //TOGL_CASE_STR(DL_BEGINDRAG);
        //TOGL_CASE_STR(MCIWNDM_GETINACTIVETIMER);
        //TOGL_CASE_STR(PSM_IDTOINDEX);
        //TOGL_CASE_STR(DL_DRAGGING);
        //TOGL_CASE_STR(PSM_INDEXTOID);
        //TOGL_CASE_STR(DL_DROPPED);
        //TOGL_CASE_STR(PSM_GETRESULT);
        //TOGL_CASE_STR(DL_CANCELDRAG);
        //TOGL_CASE_STR(PSM_RECALCPAGESIZES);
        //TOGL_CASE_STR(MCIWNDM_GET_SOURCE);
        //TOGL_CASE_STR(MCIWNDM_PUT_SOURCE);
        //TOGL_CASE_STR(MCIWNDM_GET_DEST);
        //TOGL_CASE_STR(MCIWNDM_PUT_DEST);
        //TOGL_CASE_STR(MCIWNDM_CAN_PLAY);
        //TOGL_CASE_STR(MCIWNDM_CAN_WINDOW);
        //TOGL_CASE_STR(MCIWNDM_CAN_RECORD);
        //TOGL_CASE_STR(MCIWNDM_CAN_SAVE);
        //TOGL_CASE_STR(MCIWNDM_CAN_EJECT);
        //TOGL_CASE_STR(MCIWNDM_CAN_CONFIG);
        //TOGL_CASE_STR(IE_GETINK);
        //TOGL_CASE_STR(IE_MSGFIRST);
        //TOGL_CASE_STR(MCIWNDM_PALETTEKICK);
        //TOGL_CASE_STR(IE_SETINK);
        //TOGL_CASE_STR(IE_GETPENTIP);
        //TOGL_CASE_STR(IE_SETPENTIP);
        //TOGL_CASE_STR(IE_GETERASERTIP);
        //TOGL_CASE_STR(IE_SETERASERTIP);
        //TOGL_CASE_STR(IE_GETBKGND);
        //TOGL_CASE_STR(IE_SETBKGND);
        //TOGL_CASE_STR(IE_GETGRIDORIGIN);
        //TOGL_CASE_STR(IE_SETGRIDORIGIN);
        //TOGL_CASE_STR(IE_GETGRIDPEN);
        //TOGL_CASE_STR(IE_SETGRIDPEN);
        //TOGL_CASE_STR(IE_GETGRIDSIZE);
        //TOGL_CASE_STR(IE_SETGRIDSIZE);
        //TOGL_CASE_STR(IE_GETMODE);
        //TOGL_CASE_STR(IE_SETMODE);
        //TOGL_CASE_STR(IE_GETINKRECT);
        //TOGL_CASE_STR(WM_CAP_SET_MCI_DEVICEW);
        //TOGL_CASE_STR(WM_CAP_GET_MCI_DEVICEW);
        //TOGL_CASE_STR(WM_CAP_PAL_OPENW);
        //TOGL_CASE_STR(WM_CAP_PAL_SAVEW);
        //TOGL_CASE_STR(IE_GETAPPDATA);
        //TOGL_CASE_STR(IE_SETAPPDATA);
        //TOGL_CASE_STR(IE_GETDRAWOPTS);
        //TOGL_CASE_STR(IE_SETDRAWOPTS);
        //TOGL_CASE_STR(IE_GETFORMAT);
        //TOGL_CASE_STR(IE_SETFORMAT);
        //TOGL_CASE_STR(IE_GETINKINPUT);
        //TOGL_CASE_STR(IE_SETINKINPUT);
        //TOGL_CASE_STR(IE_GETNOTIFY);
        //TOGL_CASE_STR(IE_SETNOTIFY);
        //TOGL_CASE_STR(IE_GETRECOG);
        //TOGL_CASE_STR(IE_SETRECOG);
        //TOGL_CASE_STR(IE_GETSECURITY);
        //TOGL_CASE_STR(IE_SETSECURITY);
        //TOGL_CASE_STR(IE_GETSEL);
        //TOGL_CASE_STR(IE_SETSEL);
        //TOGL_CASE_STR(CDM_LAST);
        //TOGL_CASE_STR(EM_SETBIDIOPTIONS);
        //TOGL_CASE_STR(IE_DOCOMMAND);
        //TOGL_CASE_STR(MCIWNDM_NOTIFYMODE);
        //TOGL_CASE_STR(EM_GETBIDIOPTIONS);
        //TOGL_CASE_STR(IE_GETCOMMAND);
        //TOGL_CASE_STR(EM_SETTYPOGRAPHYOPTIONS);
        //TOGL_CASE_STR(IE_GETCOUNT);
        //TOGL_CASE_STR(EM_GETTYPOGRAPHYOPTIONS);
        //TOGL_CASE_STR(IE_GETGESTURE);
        //TOGL_CASE_STR(MCIWNDM_NOTIFYMEDIA);
        //TOGL_CASE_STR(EM_SETEDITSTYLE);
        //TOGL_CASE_STR(IE_GETMENU);
        //TOGL_CASE_STR(EM_GETEDITSTYLE);
        //TOGL_CASE_STR(IE_GETPAINTDC);
        //TOGL_CASE_STR(MCIWNDM_NOTIFYERROR);
        //TOGL_CASE_STR(IE_GETPDEVENT);
        //TOGL_CASE_STR(IE_GETSELCOUNT);
        //TOGL_CASE_STR(IE_GETSELITEMS);
        //TOGL_CASE_STR(IE_GETSTYLE);
        //TOGL_CASE_STR(MCIWNDM_SETTIMEFORMATW);
        //TOGL_CASE_STR(EM_OUTLINE);
        //TOGL_CASE_STR(MCIWNDM_GETTIMEFORMATW);
        //TOGL_CASE_STR(EM_GETSCROLLPOS);
        //TOGL_CASE_STR(EM_SETSCROLLPOS);
        //TOGL_CASE_STR(EM_SETSCROLLPOS);
        //TOGL_CASE_STR(EM_SETFONTSIZE);
        //TOGL_CASE_STR(EM_GETZOOM);
        //TOGL_CASE_STR(MCIWNDM_GETFILENAMEW);
        //TOGL_CASE_STR(EM_SETZOOM);
        //TOGL_CASE_STR(MCIWNDM_GETDEVICEW);
        //TOGL_CASE_STR(EM_GETVIEWKIND);
        //TOGL_CASE_STR(EM_SETVIEWKIND);
        //TOGL_CASE_STR(EM_GETPAGE);
        //TOGL_CASE_STR(MCIWNDM_GETERRORW);
        //TOGL_CASE_STR(EM_SETPAGE);
        //TOGL_CASE_STR(EM_GETHYPHENATEINFO);
        //TOGL_CASE_STR(EM_SETHYPHENATEINFO);
        //TOGL_CASE_STR(EM_GETPAGEROTATE);
        //TOGL_CASE_STR(EM_SETPAGEROTATE);
        //TOGL_CASE_STR(EM_GETCTFMODEBIAS);
        //TOGL_CASE_STR(EM_SETCTFMODEBIAS);
        //TOGL_CASE_STR(EM_GETCTFOPENSTATUS);
        //TOGL_CASE_STR(EM_SETCTFOPENSTATUS);
        //TOGL_CASE_STR(EM_GETIMECOMPTEXT);
        //TOGL_CASE_STR(EM_ISIME);
        //TOGL_CASE_STR(EM_GETIMEPROPERTY);
        //TOGL_CASE_STR(EM_GETQUERYRTFOBJ);
        //TOGL_CASE_STR(EM_SETQUERYRTFOBJ);
        //TOGL_CASE_STR(FM_GETFOCUS);
        //TOGL_CASE_STR(FM_GETDRIVEINFOA);
        //TOGL_CASE_STR(FM_GETSELCOUNT);
        //TOGL_CASE_STR(FM_GETSELCOUNTLFN);
        //TOGL_CASE_STR(FM_GETFILESELA);
        //TOGL_CASE_STR(FM_GETFILESELLFNA);
        //TOGL_CASE_STR(FM_REFRESH_WINDOWS);
        //TOGL_CASE_STR(FM_RELOAD_EXTENSIONS);
        //TOGL_CASE_STR(FM_GETDRIVEINFOW);
        //TOGL_CASE_STR(FM_GETFILESELW);
        //TOGL_CASE_STR(FM_GETFILESELLFNW);
        //TOGL_CASE_STR(WLX_WM_SAS);
        //TOGL_CASE_STR(SM_GETSELCOUNT);
        //TOGL_CASE_STR(UM_GETSELCOUNT);
        //TOGL_CASE_STR(WM_CPL_LAUNCH);
        //TOGL_CASE_STR(SM_GETSERVERSELA);
        //TOGL_CASE_STR(UM_GETUSERSELA);
        //TOGL_CASE_STR(WM_CPL_LAUNCHED);
        //TOGL_CASE_STR(SM_GETSERVERSELW);
        //TOGL_CASE_STR(UM_GETUSERSELW);
        //TOGL_CASE_STR(SM_GETCURFOCUSA);
        //TOGL_CASE_STR(UM_GETGROUPSELA);
        //TOGL_CASE_STR(SM_GETCURFOCUSW);
        //TOGL_CASE_STR(UM_GETGROUPSELW);
        //TOGL_CASE_STR(SM_GETOPTIONS);
        //TOGL_CASE_STR(UM_GETCURFOCUSA);
        //TOGL_CASE_STR(UM_GETCURFOCUSW);
        //TOGL_CASE_STR(UM_GETOPTIONS);
        //TOGL_CASE_STR(UM_GETOPTIONS2);
        //TOGL_CASE_STR(LVM_FIRST);
        //TOGL_CASE_STR(LVM_GETBKCOLOR);
        //TOGL_CASE_STR(LVM_SETBKCOLOR);
        //TOGL_CASE_STR(LVM_GETIMAGELIST);
        //TOGL_CASE_STR(LVM_SETIMAGELIST);
        //TOGL_CASE_STR(LVM_GETITEMCOUNT);
        //TOGL_CASE_STR(LVM_GETITEMA);
        //TOGL_CASE_STR(LVM_SETITEMA);
        //TOGL_CASE_STR(LVM_INSERTITEMA);
        //TOGL_CASE_STR(LVM_DELETEITEM);
        //TOGL_CASE_STR(LVM_DELETEALLITEMS);
        //TOGL_CASE_STR(LVM_GETCALLBACKMASK);
        //TOGL_CASE_STR(LVM_SETCALLBACKMASK);
        //TOGL_CASE_STR(LVM_GETNEXTITEM);
        //TOGL_CASE_STR(LVM_FINDITEMA);
        //TOGL_CASE_STR(LVM_GETITEMRECT);
        //TOGL_CASE_STR(LVM_SETITEMPOSITION);
        //TOGL_CASE_STR(LVM_GETITEMPOSITION);
        //TOGL_CASE_STR(LVM_GETSTRINGWIDTHA);
        //TOGL_CASE_STR(LVM_HITTEST);
        //TOGL_CASE_STR(LVM_ENSUREVISIBLE);
        //TOGL_CASE_STR(LVM_SCROLL);
        //TOGL_CASE_STR(LVM_REDRAWITEMS);
        //TOGL_CASE_STR(LVM_ARRANGE);
        //TOGL_CASE_STR(LVM_EDITLABELA);
        //TOGL_CASE_STR(LVM_GETEDITCONTROL);
        //TOGL_CASE_STR(LVM_GETCOLUMNA);
        //TOGL_CASE_STR(LVM_SETCOLUMNA);
        //TOGL_CASE_STR(LVM_INSERTCOLUMNA);
        //TOGL_CASE_STR(LVM_DELETECOLUMN);
        //TOGL_CASE_STR(LVM_GETCOLUMNWIDTH);
        //TOGL_CASE_STR(LVM_SETCOLUMNWIDTH);
        //TOGL_CASE_STR(LVM_GETHEADER);
        //TOGL_CASE_STR(LVM_CREATEDRAGIMAGE);
        //TOGL_CASE_STR(LVM_GETVIEWRECT);
        //TOGL_CASE_STR(LVM_GETTEXTCOLOR);
        //TOGL_CASE_STR(LVM_SETTEXTCOLOR);
        //TOGL_CASE_STR(LVM_GETTEXTBKCOLOR);
        //TOGL_CASE_STR(LVM_SETTEXTBKCOLOR);
        //TOGL_CASE_STR(LVM_GETTOPINDEX);
        //TOGL_CASE_STR(LVM_GETCOUNTPERPAGE);
        //TOGL_CASE_STR(LVM_GETORIGIN);
        //TOGL_CASE_STR(LVM_UPDATE);
        //TOGL_CASE_STR(LVM_SETITEMSTATE);
        //TOGL_CASE_STR(LVM_GETITEMSTATE);
        //TOGL_CASE_STR(LVM_GETITEMTEXTA);
        //TOGL_CASE_STR(LVM_SETITEMTEXTA);
        //TOGL_CASE_STR(LVM_SETITEMCOUNT);
        //TOGL_CASE_STR(LVM_SORTITEMS);
        //TOGL_CASE_STR(LVM_SETITEMPOSITION32);
        //TOGL_CASE_STR(LVM_GETSELECTEDCOUNT);
        //TOGL_CASE_STR(LVM_GETITEMSPACING);
        //TOGL_CASE_STR(LVM_GETISEARCHSTRINGA);
        //TOGL_CASE_STR(LVM_SETICONSPACING);
        //TOGL_CASE_STR(LVM_SETEXTENDEDLISTVIEWSTYLE);
        //TOGL_CASE_STR(LVM_GETEXTENDEDLISTVIEWSTYLE);
        //TOGL_CASE_STR(LVM_GETSUBITEMRECT);
        //TOGL_CASE_STR(LVM_SUBITEMHITTEST);
        //TOGL_CASE_STR(LVM_SETCOLUMNORDERARRAY);
        //TOGL_CASE_STR(LVM_GETCOLUMNORDERARRAY);
        //TOGL_CASE_STR(LVM_SETHOTITEM);
        //TOGL_CASE_STR(LVM_GETHOTITEM);
        //TOGL_CASE_STR(LVM_SETHOTCURSOR);
        //TOGL_CASE_STR(LVM_GETHOTCURSOR);
        //TOGL_CASE_STR(LVM_APPROXIMATEVIEWRECT);
        //TOGL_CASE_STR(LVM_SETWORKAREAS);
        //TOGL_CASE_STR(LVM_GETSELECTIONMARK);
        //TOGL_CASE_STR(LVM_SETSELECTIONMARK);
        //TOGL_CASE_STR(LVM_SETBKIMAGEA);
        //TOGL_CASE_STR(LVM_GETBKIMAGEA);
        //TOGL_CASE_STR(LVM_GETWORKAREAS);
        //TOGL_CASE_STR(LVM_SETHOVERTIME);
        //TOGL_CASE_STR(LVM_GETHOVERTIME);
        //TOGL_CASE_STR(LVM_GETNUMBEROFWORKAREAS);
        //TOGL_CASE_STR(LVM_SETTOOLTIPS);
        //TOGL_CASE_STR(LVM_GETITEMW);
        //TOGL_CASE_STR(LVM_SETITEMW);
        //TOGL_CASE_STR(LVM_INSERTITEMW);
        //TOGL_CASE_STR(LVM_GETTOOLTIPS);
        //TOGL_CASE_STR(LVM_FINDITEMW);
        //TOGL_CASE_STR(LVM_GETSTRINGWIDTHW);
        //TOGL_CASE_STR(LVM_GETCOLUMNW);
        //TOGL_CASE_STR(LVM_SETCOLUMNW);
        //TOGL_CASE_STR(LVM_INSERTCOLUMNW);
        //TOGL_CASE_STR(LVM_GETITEMTEXTW);
        //TOGL_CASE_STR(LVM_SETITEMTEXTW);
        //TOGL_CASE_STR(LVM_GETISEARCHSTRINGW);
        //TOGL_CASE_STR(LVM_EDITLABELW);
        //TOGL_CASE_STR(LVM_GETBKIMAGEW);
        //TOGL_CASE_STR(LVM_SETSELECTEDCOLUMN);
        //TOGL_CASE_STR(LVM_SETTILEWIDTH);
        //TOGL_CASE_STR(LVM_SETVIEW);
        //TOGL_CASE_STR(LVM_GETVIEW);
        //TOGL_CASE_STR(LVM_INSERTGROUP);
        //TOGL_CASE_STR(LVM_SETGROUPINFO);
        //TOGL_CASE_STR(LVM_GETGROUPINFO);
        //TOGL_CASE_STR(LVM_REMOVEGROUP);
        //TOGL_CASE_STR(LVM_MOVEGROUP);
        //TOGL_CASE_STR(LVM_MOVEITEMTOGROUP);
        //TOGL_CASE_STR(LVM_SETGROUPMETRICS);
        //TOGL_CASE_STR(LVM_GETGROUPMETRICS);
        //TOGL_CASE_STR(LVM_ENABLEGROUPVIEW);
        //TOGL_CASE_STR(LVM_SORTGROUPS);
        //TOGL_CASE_STR(LVM_INSERTGROUPSORTED);
        //TOGL_CASE_STR(LVM_REMOVEALLGROUPS);
        //TOGL_CASE_STR(LVM_HASGROUP);
        //TOGL_CASE_STR(LVM_SETTILEVIEWINFO);
        //TOGL_CASE_STR(LVM_GETTILEVIEWINFO);
        //TOGL_CASE_STR(LVM_SETTILEINFO);
        //TOGL_CASE_STR(LVM_GETTILEINFO);
        //TOGL_CASE_STR(LVM_SETINSERTMARK);
        //TOGL_CASE_STR(LVM_GETINSERTMARK);
        //TOGL_CASE_STR(LVM_INSERTMARKHITTEST);
        //TOGL_CASE_STR(LVM_GETINSERTMARKRECT);
        //TOGL_CASE_STR(LVM_SETINSERTMARKCOLOR);
        //TOGL_CASE_STR(LVM_GETINSERTMARKCOLOR);
        //TOGL_CASE_STR(LVM_SETINFOTIP);
        //TOGL_CASE_STR(LVM_GETSELECTEDCOLUMN);
        //TOGL_CASE_STR(LVM_ISGROUPVIEWENABLED);
        //TOGL_CASE_STR(LVM_GETOUTLINECOLOR);
        //TOGL_CASE_STR(LVM_SETOUTLINECOLOR);
        //TOGL_CASE_STR(LVM_CANCELEDITLABEL);
        //TOGL_CASE_STR(LVM_MAPINDEXTOID);
        //TOGL_CASE_STR(LVM_MAPIDTOINDEX);
        //TOGL_CASE_STR(LVM_ISITEMVISIBLE);
        //TOGL_CASE_STR(LVM_GETEMPTYTEXT);
        //TOGL_CASE_STR(LVM_GETFOOTERRECT);
        //TOGL_CASE_STR(LVM_GETFOOTERINFO);
        //TOGL_CASE_STR(LVM_GETFOOTERITEMRECT);
        //TOGL_CASE_STR(LVM_GETFOOTERITEM);
        //TOGL_CASE_STR(LVM_GETITEMINDEXRECT);
        //TOGL_CASE_STR(LVM_SETITEMINDEXSTATE);
        //TOGL_CASE_STR(LVM_GETNEXTITEMINDEX);
        //TOGL_CASE_STR(OCM__BASE);
        //TOGL_CASE_STR(LVM_SETUNICODEFORMAT);
        //TOGL_CASE_STR(LVM_GETUNICODEFORMAT);
        //TOGL_CASE_STR(OCM_CTLCOLOR);
        //TOGL_CASE_STR(OCM_DRAWITEM);
        //TOGL_CASE_STR(OCM_MEASUREITEM);
        //TOGL_CASE_STR(OCM_DELETEITEM);
        //TOGL_CASE_STR(OCM_VKEYTOITEM);
        //TOGL_CASE_STR(OCM_CHARTOITEM);
        //TOGL_CASE_STR(OCM_COMPAREITEM);
        //TOGL_CASE_STR(OCM_NOTIFY);
        //TOGL_CASE_STR(OCM_COMMAND);
        //TOGL_CASE_STR(OCM_HSCROLL);
        //TOGL_CASE_STR(OCM_VSCROLL);
        //TOGL_CASE_STR(OCM_CTLCOLORMSGBOX);
        //TOGL_CASE_STR(OCM_CTLCOLOREDIT);
        //TOGL_CASE_STR(OCM_CTLCOLORLISTBOX);
        //TOGL_CASE_STR(OCM_CTLCOLORBTN);
        //TOGL_CASE_STR(OCM_CTLCOLORDLG);
        //TOGL_CASE_STR(OCM_CTLCOLORSCROLLBAR);
        //TOGL_CASE_STR(OCM_CTLCOLORSTATIC);
        //TOGL_CASE_STR(OCM_PARENTNOTIFY);
        TOGL_CASE_STR(WM_APP);
        //TOGL_CASE_STR(WM_RASDIALEVENT);
    }
    return std::string() + "(" + std::to_string(message) +")";
}

#endif // TRIVIALOPENGL_WINDOW_H_
