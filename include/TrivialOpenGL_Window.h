/**
* @file TrivialOpenGL.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_WINDOW_H_
#define TRIVIALOPENGL_WINDOW_H_

#include "TrivialOpenGL_Headers.h"
#include "TrivialOpenGL_Utility.h"
#include "TrivialOpenGL_Key.h"

//==========================================================================
// Declarations
//==========================================================================

namespace TrivialOpenGL {

    enum {
        DEF = -1,
    };

    struct StyleBit {
        using Field = uint32_t;
        enum {
            NO_RESIZE                       = 0x0001,
            NO_MAXIMIZE                     = 0x0002,
            CENTERED                        = 0x0004,
            DRAW_AREA_SIZE                  = 0x0008,
            DRAW_AREA_ONLY                  = 0x0010,
            REDRAW_ON_CHANGE_OR_REQUEST     = 0x0020,
        };
    };

    enum class WindowState {
        NORMAL,
        MAXIMIZED,
        MINIMIZED,
        WINDOWED_FULL_SCREENED,
    };

    enum {
        LOG_LEVEL_ERROR        = 0,
        LOG_LEVEL_INFO         = 1,
        LOG_LEVEL_DEBUG        = 2,
    };

    struct SpecialDebug {
        bool is_notify_any_message          = false;
        bool is_notify_draw_call            = false;
        bool is_notify_mouse_move           = false;
        bool is_notify_key_message          = false;
        bool is_notify_character_message    = false;
    };

    enum WindowOption {
        WINDOW_OPTION_AUTO_SLEEP_MODE,
    };

    struct Data {
        // Encoding: ASCII or UTF8.
        std::string     window_name         = "Window";

        // Window area.
        // If x is DEF then created window will be centered on X axis.
        // If y is DEF then created window will be centered on Y axis.
        // If width is DEF then created window will have with equal to half width of working area (desktop area excluding task bar area).
        // If with is not DEF end less than 0 then it's considered as 0.
        // If height is DEF then created window will have with equal to half height of working area (desktop area excluding task bar area).
        // If height is not DEF end less than 1 then it's considered as 1. 
        // Actual window width or height might be higher, depends on operating system minimal window size.
        AreaI           area                = {DEF, DEF, DEF, DEF};

        // NO_RESIZE                      - Window wont have resize frame.
        // NO_MAXIMIZE                    - Window wont have maximize button.
        // CENTERED                       - Window will be centered at middle of working area (desktop area excluding task bar area).
        // DRAW_AREA_SIZE                 - Width and height from 'area' variable will set size of window draw area instead whole window area.
        // DRAW_AREA_ONLY                 - Window will contain only draw area (without borders and title bar).
        // REDRAW_ON_CHANGE_OR_REQUEST    - Window will only be redrawing on change or call of Window::RequestDraw method.
        StyleBit::Field style               = 0;

        // Tries create OpenGL Rendering Context which support to at least this version, with compatibility to all previous versions.
        // If opengl_version.major and opengl_version.minor is DEF then creates for any available OpenGL version. Can be checked by GetOpenGL_Version().
        Version         opengl_verion       = {DEF, DEF};

        // File name of icon image file (.ico). 
        // Loaded icon will be presented on window title bar and on task bar.

        // If not empty string, then will load icon image (.ico). 
        // Will be visible on window title bar and task bar.
        // Encoding: ASCII or UTF8.
        std::string     icon_file_name      = "";

        // (Optional) Resource id of icon file. 
        // Id must be different than 0 and icon_file_name must be an empty string.
        // Loaded icon will be presented on window title bar and at application button on task bar. 

        // Resource identifier.
        // If not 0, then will use icon image from resources identified by this variable as application icon, title bar icon and task bar icon.
        uint16_t        icon_resource_id    = 0;       


        // If not zero then this is a time interval for which do_on_time(elapsed_time) callback function will be called. 
        uint32_t        timer_time_interval = 0; // in milliseconds

        // true - disables auto entering to screen save mode and to any power save mode.
        bool            is_auto_sleep_blocked   = false;

        // LOG_LEVEL_ERROR         - error messages only
        // LOG_LEVEL_INFO          - info and error messages
        // LOG_LEVEL_DEBUG         - debug, info and error messages
        uint32_t        log_level           = LOG_LEVEL_INFO;

        // To enable special debugging messages.
        SpecialDebug    special_debug       = {}; // Warning!!! Can slowdown application significantly.   

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
        void (*do_on_key)(KeyId key_id, bool is_down, const Extra& extra)   = nullptr;

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
        void (*do_on_state_change)(WindowState window_state)                = nullptr;

        void (*do_on_show)()                                                = nullptr;
        void (*do_on_hide)()                                                = nullptr;

        void (*do_on_foreground)(bool is_gain)                              = nullptr;   

        // time_interval - in milliseconds
        void (*do_on_time)(uint32_t time_interval)                          = nullptr;
    };

    // Get access to window singleton.
    class Window;
    Window& ToWindow();

    // Creates and runs window.
    int Run(const Data& data);

    class WindowInnerAccessor {
    public:
        friend class FontDataGenerator;
    protected:
        virtual ~WindowInnerAccessor() {}
    private:
        virtual HWND ToHWND() = 0;
    };

    // It's a singleton.
    class Window : public WindowInnerAccessor {
    public:
        friend Global<Window>;

        virtual ~Window();

        // ---

        // Creates and runs window.
        int Run(const Data& data);

        // If called from inside of do_on_{...} function, then close window after exiting from current do_on_{...} function.
        void RequestClose();

        // If called from inside of do_on_{...} function, then redraws window after exiting from current do_on_{...} function.
        void RequestRedraw();

        // ---
        
        // WINDOW_OPTION_AUTO_SLEEP_MODE     - true - if window at foreground then can automatically enter screen saver mode or any sleep mode,
        //                                     false - if window at foreground then can not automatically enter screen saver mode or any sleep mode.
        void SetOption(WindowOption window_option, bool is_enabled);
        bool IsEnabled(WindowOption window_option) const;

        // ---

        // Moves window to position in screen coordinates.
        void MoveTo(int x, int y, bool is_draw_area = false);
        void MoveTo(const PointI& pos, bool is_draw_area = false);

        // Moves from current position by offset (x, y).
        void MoveBy(int x, int y);
        void MoveBy(const PointI& offset);

        // Resizes window and keeps current window position.
        void SetSize(uint16_t width, uint16_t height, bool is_draw_area = false);
        void SetSize(const SizeU16& size, bool is_draw_area = false);

        // Moves and resizes window area.
        void SetArea(int x, int y, uint16_t width, uint16_t height, bool is_draw_area = false);
        void SetArea(const AreaIU16& area, bool is_draw_area = false);

        // Puts window in center of desktop area excluding task bar area.

        // If StyleBit::DRAW_AREA_SIZE then width and height correspond to draw area.
        void Center(uint16_t width, uint16_t height);
        void Center(const SizeU16& size);

        void Center(uint16_t width, uint16_t height, bool is_draw_area_size);
        void Center(const SizeU16& size, bool is_draw_area_size);

        // ---
        
        //// Returns window left-top corner position in screen coordinates.
        PointI GetPos() const;
        
        // Returns window size.
        SizeU16 GetSize() const;
        
        // Returns window area in screen coordinates.
        AreaIU16 GetArea() const;
        
        // Returns draw area top-left corner position in screen coordinates.
        PointI GetDrawAreaPos() const;
        
        // Returns draw area size.
        SizeU16 GetDrawAreaSize() const;
        
        // Returns draw area in screen coordinates.
        AreaIU16 GetDrawArea() const;

        // ---

        void Hide();
        void Show();
        bool IsVisible() const;

        // ---

        void Minimize();
        void Maximize();
        void GoWindowedFullScreen();

        WindowState GetState() const;

        bool IsNormal() const;
        bool IsWindowMinimized() const;
        bool IsWindowMaximized() const;
        bool IsWindowedFullScreen() const;

        // Moves window to foreground.
        void GoForeground();
        bool IsForeground() const;

        // ---

        StyleBit::Field GetStyle() const;

        PointI GetCursorPosInDrawArea() const;

        void SetLogLevel(uint32_t log_level);
        uint32_t GetLogLevel() const;

        Version GetOpenGL_Version() const;

        // ---

        std::string GetLoadFontErrMsg() const;

        // Special characters are ignored (for example: '\n', '\t').
        SizeU16 GetTextSize(const std::string& text) const;

        uint32_t GetFontDescent() const;

        // ---

        // Get access to singleton instance.
        static Window& To();

    private:
        enum class AreaPartId {
            POSITION,
            POSITION_OFFSET,
            SIZE,
            ALL
        };

        // Window client area width extension to force functional windowed full screen window (reduced flashing and be able alt+tab in Windows 7).
        enum {
            WIDTH_CORRECTION_TO_FAKE        = 1,
            DEFAULT_TIMER_ID                = 1,
            MAX_NUM_OF_UTF16_CODE_UNITS     = 2,
        };

        Window();
        TOGL_NO_COPY(Window);

        HWND ToHWND() override final;

        void SetState(WindowState state);
        void Push(bool& value, bool new_value_after_push);
        void Pop(bool& value);

        // Changes area by applying style from data parameter which was provided to Run function.
        void ChangeArea(const AreaI& area);

        void SingletonCheck();
        HICON TryLoadIcon();
        int ExecuteMainLoop();

        void Draw();

        AreaIU16 GenerateWindowArea(const AreaI& area);
        void Restore();
        void SetArea(const AreaIU16& area, AreaPartId area_part_id, bool is_client_area);

        void Create(HWND window_handle);
        void Destroy();
        void Paint();
        void Close();

        static DWORD GetWindowStyle_DrawAreaOnly();
        static DWORD GetWindowExtendedStyle_DrawAreaOnly();

        static AreaIU16 GetWindowArea(HWND window_handle);
        static AreaIU16 GetWindowAreaFromDrawArea(const AreaIU16& draw_area, DWORD window_style);

        void HandleDoOnKeyboardKey(WPARAM w_param, LPARAM l_param);
        void HandleDoOnMouseKey(UINT message, WPARAM w_param, LPARAM l_param);

        template <unsigned NUNMBER_OF_CODE_UNITS>
        void HandleUTF16_ToUTF8(const wchar_t (&char_utf16)[NUNMBER_OF_CODE_UNITS]);

        LRESULT InnerWindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);
        static LRESULT CALLBACK WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

        

        Data            m_data;

        HINSTANCE       m_instance_handle;
        HWND            m_window_handle;
        HDC             m_device_context_handle;
        HGLRC           m_rendering_context_handle;

        DWORD           m_window_style;
        DWORD           m_window_extended_style;

        bool            m_is_active;
        bool            m_is_visible;
        bool            m_is_frame;

        WindowState     m_state;
        WindowState     m_prev_state;

        bool            m_is_win7;

        bool            m_is_apply_fake_width;
        bool            m_is_enable_do_on_resize;
        bool            m_is_enable_change_state_at_resize;

        uint64_t        m_dbg_message_id;

        InnerUtility::WindowAreaCorrector   m_window_area_corrector;

        std::map<bool*, std::stack<bool>>   m_on_off_stack_map;

        wchar_t                             m_char_utf16[MAX_NUM_OF_UTF16_CODE_UNITS];
    };

} // namespace TrivialOpenGL

//==========================================================================
// Definitions
//==========================================================================

namespace TrivialOpenGL {

    inline Window& ToWindow() {
        return Window::To();
    }

    inline int Run(const Data& data) {
        return ToWindow().Run(data);
    }

    //--------------------------------------------------------------------------
    // Constructor, Destructor
    //--------------------------------------------------------------------------

    inline Window::Window() {
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

        m_state                     = WindowState::NORMAL;
        m_prev_state                = m_state;

        m_is_win7                   = IsWindows7OrGreater() && !IsWindows8OrGreater();

        m_is_apply_fake_width               = false;
        m_is_enable_do_on_resize            = true;
        m_is_enable_change_state_at_resize  = true;

        m_dbg_message_id            = 0;

        memset(m_char_utf16, 0, sizeof(m_char_utf16));

        
    }

    inline Window::~Window() {

    }

    //--------------------------------------------------------------------------
    // Run
    //--------------------------------------------------------------------------

    inline int Window::Run(const Data& data) {
        m_data = data;

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
            LogFatalError("Error TOGLW::Window::Run: Cannot create window class.");
        }
        m_window_style = WS_OVERLAPPEDWINDOW;
        if (m_data.style & StyleBit::NO_RESIZE)     m_window_style &= ~WS_THICKFRAME;
        if (m_data.style & StyleBit::NO_MAXIMIZE)   m_window_style &= ~WS_MAXIMIZEBOX;
        if (m_data.style & StyleBit::DRAW_AREA_ONLY) {
            m_window_style          = GetWindowStyle_DrawAreaOnly();
            m_window_extended_style = GetWindowExtendedStyle_DrawAreaOnly();
        }

        m_window_handle = CreateWindowExW(
            m_window_extended_style,
            WINDOW_CLASS_NAME,
            ToUTF16(m_data.window_name).c_str(),
            m_window_style,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL,
            NULL,
            m_instance_handle,
            NULL
        );
                
        if (!m_window_handle) {
            LogFatalError("Error TOGLW::Window::Run: Cannot create window.");
        }

        if (m_data.timer_time_interval > 0) {
            const UINT_PTR result = SetTimer(m_window_handle, DEFAULT_TIMER_ID, m_data.timer_time_interval, NULL);

            if (!result) LogFatalError(std::string() + "Error TOGLW::Window::Create: Can not set timer. (windows error code:" + std::to_string(GetLastError()) + ")");
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

    inline void Window::RequestClose() {
        DestroyWindow(m_window_handle);
    }

    inline void Window::RequestRedraw() {
        InvalidateRect(m_window_handle, NULL, FALSE);
    }

    //--------------------------------------------------------------------------
    // Option
    //--------------------------------------------------------------------------

    inline void Window::SetOption(WindowOption window_option, bool is_enabled) {
        switch (window_option) {
        case WINDOW_OPTION_AUTO_SLEEP_MODE:
            m_data.is_auto_sleep_blocked = !is_enabled;
            break;
        }
    }

    inline bool Window::IsEnabled(WindowOption window_option) const {
        switch (window_option) {
        case WINDOW_OPTION_AUTO_SLEEP_MODE: return !m_data.is_auto_sleep_blocked;
        }
    }

    //--------------------------------------------------------------------------
    // Position, Size, Area
    //--------------------------------------------------------------------------

    inline void Window::MoveTo(int x, int y, bool is_draw_area) {
        SetArea({x, y, 0, 0}, AreaPartId::POSITION, is_draw_area);
    }
    inline void Window::MoveTo(const PointI& pos, bool is_draw_area) {
        MoveTo(pos.x, pos.y, is_draw_area);
    }

    inline void Window::MoveBy(int x, int y) {
        SetArea({x, y, 0, 0}, AreaPartId::POSITION_OFFSET, false);
    }
    inline void Window::MoveBy(const PointI& offset) {
        MoveBy(offset.x, offset.y);
    }

    inline void Window::SetSize(uint16_t width, uint16_t height, bool is_draw_area) {
        SetArea({0, 0, width, height}, AreaPartId::SIZE, is_draw_area);
    }
    inline void Window::SetSize(const SizeU16& size, bool is_draw_area) {
        SetSize(size.width, size.height, is_draw_area);
    }

    inline void Window::SetArea(int x, int y, uint16_t width, uint16_t height, bool is_draw_area) {
        SetArea({x, y, width, height}, AreaPartId::ALL, is_draw_area);
    }
    inline void Window::SetArea(const AreaIU16& area, bool is_draw_area) {
        SetArea(area.x, area.y, area.width, area.height, is_draw_area);
    }

    //--------------------------------------------------------------------------

    inline void Window::Center(const SizeU16& size) {
        Center(size, m_data.style & StyleBit::DRAW_AREA_SIZE);
    }

    inline void Window::Center(uint16_t width, uint16_t height) {
        Center(width, height, m_data.style & StyleBit::DRAW_AREA_SIZE);
    }

    inline void Window::Center(const SizeU16& size, bool is_draw_area_size) {
        const AreaIU16 desktop_area = GetDesktopAreaNoTaskBar();

        AreaIU16 window_area = AreaIU16({}, size);

        Restore();

        if (is_draw_area_size) {
            window_area = GetWindowAreaFromDrawArea(window_area, m_window_style);
            window_area = m_window_area_corrector.RemoveInvisibleFrameFrom(window_area, m_window_handle);
        }

        window_area.x = (desktop_area.width - window_area.width) / 2;
        window_area.y = (desktop_area.height - window_area.height) / 2;
        
        SetArea(window_area);
    }

    inline void Window::Center(uint16_t width, uint16_t height, bool is_draw_area_size) {
        Center(SizeU16(width, height), is_draw_area_size);
    }


    inline void Window::ChangeArea(const AreaI& area) {
        SetArea(GenerateWindowArea(area), AreaPartId::ALL, m_data.style & StyleBit::DRAW_AREA_ONLY);
    }

    //--------------------------------------------------------------------------

    inline PointI Window::GetPos() const {
        return GetArea().GetPos();
    }
    
    inline SizeU16 Window::GetSize() const {
        return GetArea().GetSize();
    }
    
    inline AreaIU16 Window::GetArea() const {
        if (m_state == WindowState::MINIMIZED) {
            return AreaIU16(0, 0, 0, 0);
        }

        AreaIU16 area = GetWindowArea(m_window_handle);
    
        // Workaround.
        if (IsWindowedFullScreen()) area.width -= WIDTH_CORRECTION_TO_FAKE;
    
        return m_window_area_corrector.RemoveInvisibleFrameFrom(area, m_window_handle);
    }
    
    inline PointI Window::GetDrawAreaPos() const {
        return GetDrawArea().GetPos();
    }
    
    inline SizeU16 Window::GetDrawAreaSize() const {
        return GetDrawArea().GetSize();
    }
    
    inline AreaIU16 Window::GetDrawArea() const {
        RECT r;
        if (m_state == WindowState::MINIMIZED) {
            return AreaIU16(0, 0, 0, 0);
        }

        if (GetClientRect(m_window_handle, &r) && ClientToScreen(m_window_handle, (POINT*)&r) && ClientToScreen(m_window_handle, (POINT*)&r.right)) {
            AreaIU16 area = MakeAreaIU16(r);
    
            // Workaround.
            if (IsWindowedFullScreen()) area.width -= WIDTH_CORRECTION_TO_FAKE;
    
            return area;
        }
        return AreaIU16(0, 0, 0, 0);
    }

    //--------------------------------------------------------------------------
    // State
    //--------------------------------------------------------------------------

    inline void Window::Hide() {
        ShowWindow(m_window_handle, SW_HIDE);
    }

    inline void Window::Show() {
        ShowWindow(m_window_handle, SW_SHOW);
    }

    inline bool Window::IsVisible() const {
        return m_is_visible;
    }

    inline void Window::Minimize() {
        if (m_state == WindowState::WINDOWED_FULL_SCREENED) {
            Push(m_is_enable_do_on_resize, false);
            Push(m_is_enable_change_state_at_resize, false);

            SetWindowLongPtrW(m_window_handle, GWL_STYLE,   m_window_style);
            SetWindowLongPtrW(m_window_handle, GWL_EXSTYLE, m_window_extended_style);
            ShowWindow(m_window_handle, SW_NORMAL);

            Pop(m_is_enable_change_state_at_resize);
            Pop(m_is_enable_do_on_resize);
        } else if (m_state == WindowState::MAXIMIZED) {
            Push(m_is_enable_do_on_resize, false);
            Push(m_is_enable_change_state_at_resize, false);

            ShowWindow(m_window_handle, SW_RESTORE);

            Pop(m_is_enable_change_state_at_resize);
            Pop(m_is_enable_do_on_resize);
        }

        ShowWindow(m_window_handle, SW_MINIMIZE);
    }

    inline void Window::Maximize() {
        if (!m_is_visible) {
            ShowWindow(m_window_handle, SW_SHOW);
        }

        if (m_state == WindowState::WINDOWED_FULL_SCREENED) {
            Push(m_is_enable_do_on_resize, false);
            Push(m_is_enable_change_state_at_resize, false);

            SetWindowLongPtrW(m_window_handle, GWL_STYLE,   m_window_style);
            SetWindowLongPtrW(m_window_handle, GWL_EXSTYLE, m_window_extended_style);

            Pop(m_is_enable_change_state_at_resize);
            Pop(m_is_enable_do_on_resize);
        } else if (m_state == WindowState::MINIMIZED) {
            ShowWindow(m_window_handle, SW_RESTORE);
        }

        if (m_data.style & StyleBit::DRAW_AREA_ONLY) {
            const SizeU16 work_area_size = GetDesktopAreaSizeNoTaskBar();

            Push(m_is_enable_change_state_at_resize, false);

            SetWindowPos(m_window_handle, HWND_TOP, 0, 0, work_area_size.width, work_area_size.height, SWP_SHOWWINDOW);

            Pop(m_is_enable_change_state_at_resize);

            SetState(WindowState::MAXIMIZED);
        } else {
            ShowWindow(m_window_handle, SW_MAXIMIZE);
        }
    }

    inline void Window::GoWindowedFullScreen() {
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
        RECT screen_rectangle = MakeRECT(AreaIU16({}, GetScreenSize()));
        AdjustWindowRectEx(&screen_rectangle, GetWindowStyle_DrawAreaOnly(), FALSE, GetWindowExtendedStyle_DrawAreaOnly());

        const AreaIU16 screen_area = MakeAreaIU16(screen_rectangle);

        Push(m_is_enable_change_state_at_resize, false);
        m_is_apply_fake_width = true;

        SetWindowPos(m_window_handle, HWND_TOP, screen_area.x, screen_area.y, screen_area.width + WIDTH_CORRECTION_TO_FAKE, screen_area.height, SWP_SHOWWINDOW);

        m_is_apply_fake_width = false;
        Pop(m_is_enable_change_state_at_resize);

        SetState(WindowState::WINDOWED_FULL_SCREENED);
    }

    inline WindowState Window::GetState() const {
        return m_state;
    }

    inline bool Window::IsNormal() const {
        return GetState() == WindowState::NORMAL;
    }

    inline bool Window::IsWindowMinimized() const {
        return GetState() == WindowState::MINIMIZED;
    }

    inline bool Window::IsWindowMaximized() const {
        return GetState() == WindowState::MAXIMIZED;
    }

    inline bool Window::IsWindowedFullScreen() const {
        return GetState() == WindowState::WINDOWED_FULL_SCREENED;
    }

    inline void Window::GoForeground() {
        SetForegroundWindow(m_window_handle);
    }

    inline bool Window::IsForeground() const {
        return GetForegroundWindow() == m_window_handle;
    }

    //--------------------------------------------------------------------------

    inline StyleBit::Field Window::GetStyle() const {
        return m_data.style;
    }

    inline PointI Window::GetCursorPosInDrawArea() const {
        POINT pos;
        if (GetCursorPos(&pos) && ScreenToClient(m_window_handle, &pos)) {
            return { pos.x, pos.y };
        }
        return {};
    }

    inline void Window::SetLogLevel(uint32_t log_level) {
        m_data.log_level = log_level;
    }

    inline uint32_t Window::GetLogLevel() const { 
        return m_data.log_level; 
    }

    inline Version Window::GetOpenGL_Version() const {
        return m_data.opengl_verion;
    }

    //--------------------------------------------------------------------------

    inline Window& Window::To() {
        return Global<Window>::To();
    }

    //--------------------------------------------------------------------------
    // Private
    //--------------------------------------------------------------------------

    inline void Window::SetState(WindowState state) {
        m_prev_state = m_state;
        m_state = state;
        if (m_state != m_prev_state && m_data.do_on_state_change) m_data.do_on_state_change(m_state);
    }

    inline void Window::Push(bool& value, bool new_value_after_push) {
        m_on_off_stack_map[&value].push(value);
        value = new_value_after_push;
    }

    inline void Window::Pop(bool& value) {
        auto stack = m_on_off_stack_map[&value];
        if (!stack.empty()) {
            value = stack.top();
            stack.pop();
        }
    }
    
    //--------------------------------------------------------------------------

    inline void Window::SingletonCheck() {
        static bool is_instance_exists = false;
        if (is_instance_exists) {
            LogFatalError("Window::SingletonCheck: Can't be more than one instance of Window class.");
        }
        is_instance_exists = true;
    }

    inline HICON Window::TryLoadIcon() {
        if (!m_data.icon_file_name.empty()) {
            return (HICON)LoadImageW(
                NULL,
                ToUTF16(m_data.icon_file_name).c_str(),
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

    inline int Window::ExecuteMainLoop() {
        MSG msg = {};

        if (m_data.style & StyleBit::REDRAW_ON_CHANGE_OR_REQUEST) {

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
    
    inline void Window::Draw() {
        if (m_data.special_debug.is_notify_draw_call) {
            LogDebug("Window::Draw"); 
        }

        if (m_data.draw) m_data.draw();

        SwapBuffers(m_device_context_handle);
    }

    inline AreaIU16 Window::GenerateWindowArea(const AreaI& area) {
        AreaIU16 window_area;

        // --- Size --- //

        const AreaIU16 desktop_area = GetDesktopAreaNoTaskBar();

        window_area.width   = (area.width != DEF)   ? area.width    : (desktop_area.width / 2);
        window_area.height  = (area.height != DEF)  ? area.height   : (desktop_area.height / 2);

        // In a case of unreasonable values.
        if (window_area.width < 0)    window_area.width = 0;
        if (window_area.height < 1)   window_area.height = 1;

        if ((m_data.style & StyleBit::DRAW_AREA_SIZE) && !(m_data.style & StyleBit::DRAW_AREA_ONLY)) {
            const AreaIU16 window_area_with_invisible_frame = GetWindowAreaFromDrawArea(window_area, m_window_style);

            window_area = m_window_area_corrector.RemoveInvisibleFrameFrom(window_area_with_invisible_frame, m_window_handle);
        }

        // --- Position --- //

        if (m_data.style & StyleBit::CENTERED) {
            window_area.x = (desktop_area.width - window_area.width) / 2;
            window_area.y = (desktop_area.height - window_area.height) / 2;
        } else {
            window_area.x = (area.x != DEF) ? area.x : ((desktop_area.width - window_area.width) / 2);
            window_area.y = (area.y != DEF) ? area.y : ((desktop_area.height - window_area.height) / 2);
        }

        // No need for additional adjustment for invisible window frame. 
        // Already done for both position and size in 'Size' section.

        // ---

        return window_area;
    }

    inline void Window::Restore() {
        if (IsMaximized(m_window_handle)) {
            if (m_prev_state == WindowState::WINDOWED_FULL_SCREENED) {
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
      
        if (m_state == WindowState::WINDOWED_FULL_SCREENED) {
            Push(m_is_enable_do_on_resize, false);
            Push(m_is_enable_change_state_at_resize, false);

            SetWindowLongPtrW(m_window_handle, GWL_STYLE,   m_window_style);
            SetWindowLongPtrW(m_window_handle, GWL_EXSTYLE, m_window_extended_style);
            ShowWindow(m_window_handle, SW_NORMAL);

            Pop(m_is_enable_change_state_at_resize);
            Pop(m_is_enable_do_on_resize);
        } 
    }
    
    inline void Window::SetArea(const AreaIU16& area, AreaPartId area_part_id, bool is_draw_area) {
        auto GetFlags = [](AreaPartId area_part_id) -> UINT {
            switch (area_part_id) {
            case AreaPartId::POSITION:          return SWP_NOSIZE;
            case AreaPartId::POSITION_OFFSET:   return SWP_NOSIZE;
            case AreaPartId::SIZE:              return SWP_NOMOVE;
            case AreaPartId::ALL:               return 0;
            }
            return 0;
        };

        const bool is_skip = IsMinimized(m_window_handle) && area_part_id == AreaPartId::POSITION_OFFSET;

        if (!is_skip) {
            if (!m_is_visible) {
                ShowWindow(m_window_handle, SW_SHOW);
            }

            Restore();
  
            AreaIU16 raw_area;

            if (area_part_id == AreaPartId::POSITION_OFFSET) {
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

    inline DWORD Window::GetWindowStyle_DrawAreaOnly() { 
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
    inline DWORD Window::GetWindowExtendedStyle_DrawAreaOnly() { 
        return WS_EX_APPWINDOW; 
    }
    
    inline AreaIU16 Window::GetWindowArea(HWND window_handle) {
        RECT r;
        if (GetWindowRect(window_handle, &r)) {
            return MakeAreaIU16(r);
        }
        return AreaIU16(0, 0, 0, 0);
    }

    inline AreaIU16 Window::GetWindowAreaFromDrawArea(const AreaIU16& draw_area, DWORD window_style) {
        RECT rect = MakeRECT(draw_area);
        AdjustWindowRect(&rect, window_style, FALSE);
        return MakeAreaIU16(rect);
    }

    inline HWND Window::ToHWND() {
        return m_window_handle;
    }
    
    //--------------------------------------------------------------------------

    inline void Window::Create(HWND window_handle) {
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
        if (!m_device_context_handle) LogFatalError("Window::Create: Can not get device context.");

        int pfi = ChoosePixelFormat(m_device_context_handle, &pfd);
        if (!pfi) LogFatalError(std::string() + "Window::Create: Can not choose pixel format. (windows error code:" + std::to_string(GetLastError()) + ")");

        BOOL result = SetPixelFormat(m_device_context_handle, pfi, &pfd);
        if (!result) LogFatalError(std::string() + "Window::Create: Can not set pixel format. (windows error code:" + std::to_string(GetLastError()) + ")");

        // --- Displaying Format Info --- //

        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        int max_pfi = DescribePixelFormat(m_device_context_handle, pfi, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
        if (!max_pfi) LogFatalError(std::string() + "Window::Create: Can not get pixel format. (windows error code:" + std::to_string(GetLastError()) + ")");

        LogInfo(std::string() + "OpenGL Pixel Format:"
            " Red:"     + std::to_string(pfd.cRedBits) + 
            " Green:"   + std::to_string(pfd.cGreenBits) + 
            " Blue:"    + std::to_string(pfd.cBlueBits) + 
            " Alpha:"   + std::to_string(pfd.cAlphaBits) + 
            " Depth:"   + std::to_string(pfd.cDepthBits) + 
            " Stencil:" + std::to_string(pfd.cStencilBits));

        // --- Creates OpenGL Rendering Context --- //

        m_rendering_context_handle = wglCreateContext(m_device_context_handle);
        if (!m_rendering_context_handle) LogFatalError(std::string() + "Window::Create: Can not create OpenGl Rendering Context. (windows error code:" + std::to_string(GetLastError()) + ")");

        if (!wglMakeCurrent(m_device_context_handle, m_rendering_context_handle)) {
            LogFatalError("Window::Create: Can not set created OpenGl Rendering Context to be current.");
        }

        if ((m_data.opengl_verion.major == DEF || m_data.opengl_verion.minor == DEF) && m_data.opengl_verion.major != m_data.opengl_verion.minor) {
            LogFatalError("Window::Create: Incorrect OpenGL version is provided.");
        }

        // --- Creates OpenGL Rendering Context with required minimum version --- //

        if (m_data.opengl_verion.major != DEF && m_data.opengl_verion.minor != DEF) {
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
                    LogFatalError(std::string() + "Window::Create: Can not create OpenGl Rendering Context for version " + std::to_string(m_data.opengl_verion.major) + "." + std::to_string(m_data.opengl_verion.minor) + ".");
                }

                if (!wglMakeCurrent(m_device_context_handle, rendering_context_handle)) {
                    LogFatalError(std::string() + "Window::Create: Can not set created OpenGl Rendering Context for version " + std::to_string(m_data.opengl_verion.major) + "." + std::to_string(m_data.opengl_verion.minor) + " to be current.");
                }

                m_rendering_context_handle = rendering_context_handle ;

            } else {
                LogFatalError("Window::Create: Can not load wglCreateContextAttribsARB function.");
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

        if (m_data.log_level >= LOG_LEVEL_INFO) LogInfo(std::string("OpenGl Version: ") + (const char*)glGetString(GL_VERSION));

        // ---
    }

    inline void Window::Destroy() {
        if (m_data.log_level >= LOG_LEVEL_DEBUG) {
            LogDebug("Window::Destroy");
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

    inline void Window::HandleDoOnKeyboardKey(WPARAM w_param, LPARAM l_param) {
        if (m_data.do_on_key) {
            const InnerKey::VirtualKeyData& virtual_key_data = *((const InnerKey::VirtualKeyData*)(&l_param));

            const KeyId key_id = InnerKey::GetKeyId(w_param);
            const bool is_down = virtual_key_data.trans_state == 0;

            Extra extra = {};
            extra.count = virtual_key_data.count;

            const PointI pos = GetCursorPosInDrawArea();
            extra.x = pos.x;
            extra.y = pos.y;

            extra.keyboard_side = InnerKey::GetKeyboardSide(key_id, virtual_key_data);

            m_data.do_on_key(key_id, is_down, extra);
        }
    }


    inline void Window::HandleDoOnMouseKey(UINT message, WPARAM w_param, LPARAM l_param) {
        const bool is_down = InnerKey::IsMouseButtonDown(message);

        if (m_data.do_on_key) {
            const KeyId key_id = InnerKey::GetMouseKeyId(message, w_param);

            Extra extra = {};
            extra.count         = 1;
            extra.x             = GET_X_LPARAM(l_param);
            extra.y             = GET_Y_LPARAM(l_param);
            extra.keyboard_side = KEYBOARD_SIDE_NONE;

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
    inline void Window::HandleUTF16_ToUTF8(const wchar_t (&char_utf16)[NUNMBER_OF_CODE_UNITS]) {
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

    inline LRESULT Window::InnerWindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
        if (m_data.special_debug.is_notify_any_message) {
            LogDebug(std::string() + "[" + WM_ToStr(message) + "] " + std::to_string(m_dbg_message_id++));
        }


        switch (message) {
        case WM_CREATE: 
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
                LogDebug("WM_CREATE"); 
            }

            Create(window_handle);
            return 0;

        case WM_DESTROY:
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
                LogDebug("WM_DESTROY"); 
            }

            Destroy();
            return 0;

        case WM_CLOSE:
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
                LogDebug("WM_CLOSE"); 
            }

            DestroyWindow(m_window_handle);
            return 0;

        case WM_PAINT:
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
                LogDebug("WM_PAINT"); 
            }

            Draw();

            ValidateRect(m_window_handle, NULL); // to decrease number of WM_PAINT messages

            return 0;

        // Used for debugging.
#if 0
        case WM_MOVING: {
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
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
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
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
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
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
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
                LogDebug("WM_ENTERSIZEMOVE");
            }

            return 0;
        }

        case WM_EXITSIZEMOVE: {
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
                LogDebug("WM_EXITSIZEMOVE");
            }

            return 0;
        }

        case WM_SIZING: {
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
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

                LogDebug(dbg_msg);
            }

            return TRUE;
        }

        case WM_SIZE:
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
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

                if (m_is_apply_fake_width) dbg_msg += " fake_width=" + std::to_string(width - WIDTH_CORRECTION_TO_FAKE);

                if (!m_is_enable_do_on_resize) dbg_msg += " without:do_on_resize";

                LogDebug(dbg_msg);
            }

            m_is_visible = true;

            if (m_data.do_on_resize) {
                if (m_is_enable_do_on_resize) {
                    if (m_is_apply_fake_width) {
                        m_data.do_on_resize(LOWORD(l_param) - WIDTH_CORRECTION_TO_FAKE, HIWORD(l_param));
                    } else {
                        m_data.do_on_resize(LOWORD(l_param), HIWORD(l_param));
                    }
                }
            }

            if (m_is_enable_change_state_at_resize) {
                switch (w_param) {
                case SIZE_MAXIMIZED:  
                    SetState(WindowState::MAXIMIZED);
                    break;
                case SIZE_MINIMIZED:    
                    SetState(WindowState::MINIMIZED);
                    break;
                case SIZE_RESTORED:    
                    SetState(WindowState::NORMAL);
                    break;
                }
            }

            return 0;

        case WM_SHOWWINDOW: {
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
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

                LogDebug(dbg_msg);
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

            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
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

                LogDebug(dbg_msg);
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
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
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
            if (w_param == DEFAULT_TIMER_ID) {
                if (m_data.do_on_time) m_data.do_on_time(m_data.timer_time_interval);
            }
            return 0;

        //////////////
        // Keyboard //
        //////////////
        case WM_SETFOCUS:
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
                LogDebug("WM_SETFOCUS");
            }
            return 0;

        case WM_KILLFOCUS:
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
                LogDebug("WM_KILLFOCUS");
            }
            return 0;

        case WM_KEYDOWN:
            if (m_data.special_debug.is_notify_key_message) {
                std::string dbg_msg = "WM_KEYDOWN   ";

                dbg_msg += InnerKey::WinApiKeyDataToStr(w_param, l_param);

                LogDebug(dbg_msg);
            }
            HandleDoOnKeyboardKey(w_param, l_param);
            return 0;

        case WM_KEYUP:
            if (m_data.special_debug.is_notify_key_message) {
                std::string dbg_msg = "WM_KEYUP     ";

                dbg_msg += InnerKey::WinApiKeyDataToStr(w_param, l_param);

                LogDebug(dbg_msg);
            }
            HandleDoOnKeyboardKey(w_param, l_param);
            return 0;

        case WM_SYSKEYDOWN:
            if (m_data.special_debug.is_notify_key_message) {
                std::string dbg_msg = "WM_SYSKEYDOWN";

                dbg_msg += InnerKey::WinApiKeyDataToStr(w_param, l_param);

                LogDebug(dbg_msg);
            }
            HandleDoOnKeyboardKey(w_param, l_param);
            return 0;

        case WM_SYSKEYUP:
            if (m_data.special_debug.is_notify_key_message) {
                std::string dbg_msg = "WM_SYSKEYUP  ";

                dbg_msg += InnerKey::WinApiKeyDataToStr(w_param, l_param);

                LogDebug(dbg_msg);
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

                LogDebug(dbg_msg);
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

                dbg_msg += InnerKey::WinApiMouseButtonToStr(w_param, l_param);

                LogDebug(dbg_msg);
            }
            HandleDoOnMouseKey(message, w_param, l_param);
            return 0;

        case WM_LBUTTONUP:
            if (m_data.special_debug.is_notify_key_message) {
                std::string dbg_msg = "WM_LBUTTONUP";

                dbg_msg += InnerKey::WinApiMouseButtonToStr(w_param, l_param);

                LogDebug(dbg_msg);
            }
            HandleDoOnMouseKey(message, w_param, l_param);
            return 0;

        case WM_RBUTTONDOWN:   
            if (m_data.special_debug.is_notify_key_message) {
                std::string dbg_msg = "WM_RBUTTONDOWN";

                dbg_msg += InnerKey::WinApiMouseButtonToStr(w_param, l_param);

                LogDebug(dbg_msg);
            }
            HandleDoOnMouseKey(message, w_param, l_param);
            return 0;

        case WM_RBUTTONUP: 
            if (m_data.special_debug.is_notify_key_message) {
                std::string dbg_msg = "WM_RBUTTONUP";

                dbg_msg += InnerKey::WinApiMouseButtonToStr(w_param, l_param);

                LogDebug(dbg_msg);
            }
            HandleDoOnMouseKey(message, w_param, l_param);
            return 0;

        case WM_MBUTTONDOWN:   
            if (m_data.special_debug.is_notify_key_message) {
                std::string dbg_msg = "WM_MBUTTONDOWN";

                dbg_msg += InnerKey::WinApiMouseButtonToStr(w_param, l_param);

                LogDebug(dbg_msg);
            }
            HandleDoOnMouseKey(message, w_param, l_param);
            return 0;

        case WM_MBUTTONUP: 
            if (m_data.special_debug.is_notify_key_message) {
                std::string dbg_msg = "WM_MBUTTONUP";

                dbg_msg += InnerKey::WinApiMouseButtonToStr(w_param, l_param);

                LogDebug(dbg_msg);
            }
            HandleDoOnMouseKey(message, w_param, l_param);
            return 0;

        case WM_XBUTTONDOWN:
            if (m_data.special_debug.is_notify_key_message) {
                std::string dbg_msg = "WM_XBUTTONDOWN";

                if (HIWORD(w_param) == XBUTTON1) dbg_msg += " X1";
                if (HIWORD(w_param) == XBUTTON2) dbg_msg += " X2";

                dbg_msg += InnerKey::WinApiMouseButtonToStr(w_param, l_param);

                LogDebug(dbg_msg);
            }
            HandleDoOnMouseKey(message, w_param, l_param);
            return 0;

        case WM_XBUTTONUP:
            if (m_data.special_debug.is_notify_key_message) {
                std::string dbg_msg = "WM_XBUTTONUP";

                if (HIWORD(w_param) == XBUTTON1) dbg_msg += " X1";
                if (HIWORD(w_param) == XBUTTON2) dbg_msg += " X2";

                dbg_msg += InnerKey::WinApiMouseButtonToStr(w_param, l_param);

                LogDebug(dbg_msg);
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

                LogDebug(dbg_msg);
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

                LogDebug(dbg_msg);
            }

            if (m_data.do_on_mouse_move) {
                m_data.do_on_mouse_move(GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param));
            }
            return 0;
        }
        } // switch

        return DefWindowProcW(window_handle, message, w_param, l_param);
    }

    inline LRESULT CALLBACK Window::WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
        return To().InnerWindowProc(window_handle, message, w_param, l_param);
    }

} // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_WINDOW_H_
