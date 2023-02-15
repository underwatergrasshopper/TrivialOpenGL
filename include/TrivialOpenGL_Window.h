/**
* @file TrivialOpenGL.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_WINDOW_H_
#define TRIVIALOPENGL_WINDOW_H_

#include <GL\GL.h>
#include <string>

#include <VersionHelpers.h>

#include "TrivialOpenGL_Utility.h"

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

    enum WindowState {
        WINDOW_STATE_NORMAL,
        WINDOW_STATE_MAXIMIZED,
        WINDOW_STATE_MINIMIZED,
    };

    enum {
        LOG_LEVEL_ERROR        = 0,
        LOG_LEVEL_INFO         = 1,
        LOG_LEVEL_DEBUG        = 2,
    };

    struct SpecialDebug {
        bool is_notify_each_window_message  = false;
        bool is_notify_each_draw_call       = false;
    };

    struct Data {
        // Encoding: ASCII or UTF8.
        std::string     window_name         = "Window";

        // Window area.
        // If x is DEF then created window will be centered on X axis.
        // If y is DEF then created window will be centered on Y axis.
        // If width is DEF then created window will have with equal to half width of working area (desktop area excluding task bar area).
        // If height is DEF then created window will have with equal to half height of working area (desktop area excluding task bar area).
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


        // LOG_LEVEL_ERROR         - error messages only
        // LOG_LEVEL_INFO          - info and error messages
        // LOG_LEVEL_DEBUG         - debug, info and error messages
        uint32_t        log_level          = LOG_LEVEL_INFO;
        SpecialDebug    special_debug      = {}; // Warning!!! Can slowdown application significantly.   

        // Is called right after window is created.
        void (*do_on_create)()                                          = nullptr;

        // Is called right before window is closed.
        void (*do_on_destroy)()                                         = nullptr;

        // Is called each time when window content needs to be redrawn.
        void (*display)()                                               = nullptr;

        void (*do_on_key_down_raw)(WPARAM w_param, LPARAM l_param)      = nullptr;
        void (*do_on_key_up_raw)(WPARAM w_param, LPARAM l_param)        = nullptr;
 
        void (*do_on_resize)(uint32_t width, uint32_t height)             = nullptr;

    };

    // It's a singleton.
    class Window {
    public:
        Window();
        virtual ~Window();

        // ---

        // Creates and runs window.
        int Run(const Data& data);

        // If called from inside of do_on_{...} function, then close window after exiting from current do_on_{...} function.
        void RequestClose();

        // If called from inside of do_on_{...} function, then redraws window after exiting from current do_on_{...} function.
        void RequestRedraw();

        // ---

        // Moves window to position in screen coordinates.
        void MoveTo(int x, int y);
        void MoveTo(const PointI& pos);

        // Moves from current position by offset (x, y).
        void MoveBy(int x, int y);
        void MoveBy(const PointI& pos);

        // Resizes window and keeps current window position.
        void SetSize(int width, int height);
        void SetSize(const SizeI& size);

        // Moves and resizes window area.
        void SetArea(int x, int y, int width, int height);
        void SetArea(const AreaI& area);

        void SetDrawAreaPos(int x, int y);
        void SetDrawAreaPos(const PointI& pos);

        // Resizes window draw area and keeps current window position.
        // width        Window draw area width.
        // height       Window draw area height.
        void SetDrawAreaSize(int width, int height);
        void SetDrawAreaSize(const SizeI& draw_area_size);

        void SetDrawArea(int x, int y, int width, int height);
        void SetDrawArea(const AreaI& area);

        // Puts window in center of desktop area excluding task bar area.
        void Center();

        // Changes area by applying style from data parameter which was provided to Run function.
        void ChangeArea(const AreaI& area);

        // ---
        
        // Returns window left-top corner position in screen coordinates.
        PointI GetPos() const;

        // Returns window size.
        SizeI GetSize() const;

        // Returns window area in screen coordinates.
        AreaI GetArea() const;

        // Returns draw area top-left corner position in screen coordinates.
        PointI GetDrawAreaPos() const;

        // Returns draw area size.
        SizeI GetDrawAreaSize() const;

        // Returns draw area in screen coordinates.
        AreaI GetDrawArea() const;

        // ---

        void Hide();
        void Show();
        bool IsVisible() const;

        // ---

        void ChangeStateTo(WindowState state);

        void Restore();
        void Minimize();
        void Maximize();
        void GoWindowedFullScreen();

        WindowState GetState() const;

        bool IsNormal() const;
        bool IsMinimized() const;
        bool IsMaximized() const;
        bool IsWindowedFullScreen() const;

        // ---

        uint32_t GetDebugLevel() const;
        Version GetOpenGL_Version() const;

        // ---

        // Get access to singleton instance.
        static Window& To();

    private:
        enum class AreaPartId {
            POSITION,
            SIZE,
            ALL
        };

        // Window client area width extension to force functional windowed full screen window (reduced flashing and be able alt+tab in Windows 7).
        enum {
            WIDTH_EXTENTION = 1
        };

        void SingletonCheck();
        HICON TryLoadIcon();
        int ExecuteMainLoop();

        void Display();

        AreaI GenerateWindowArea(const AreaI& area);
        void SetArea(const AreaI& area, AreaPartId area_part_id, bool is_client_area);

        void Create(HWND window_handle);
        void Destroy();
        void Paint();
        void Close();

        static DWORD GetWindowStyle_DrawAreaOnly();
        static DWORD GetWindowExtendedStyle_DrawAreaOnly();

        static AreaI GetWindowArea(HWND window_handle);
        static AreaI GetWindowAreaFromDrawArea(const AreaI& draw_area, DWORD window_style);


        LRESULT InnerWindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);
        static LRESULT CALLBACK WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

        static std::string WM_ToStr(UINT message);

        Data        m_data;

        HINSTANCE   m_instance_handle;
        HWND        m_window_handle;
        HDC         m_device_context_handle;
        HGLRC       m_rendering_context_handle;

        DWORD       m_window_style;
        DWORD       m_window_extended_style;

        AreaI       m_last_window_area;

        bool        m_is_active;
        bool        m_is_visible;
        WindowState m_state;

        bool        m_is_dragging;  // moving or resizing by holding mouse button
        bool        m_is_stretching_by_edge; // sizing by edge
        bool        m_is_size_restored_while_dragging; 
        bool        m_is_docked;    // by docking window and sizing to part of screen by dragging

        bool        m_force_undock;

        bool        m_is_win7;

        bool        m_is_apply_fake_width;
        bool        m_is_do_on_resize_enabled;
        uint64_t    m_dbg_message_id;

        WindowAreaCorrector m_window_area_corrector;
    };

} // namespace TrivialOpenGL

//==========================================================================
// Definitions
//==========================================================================

namespace TrivialOpenGL {

    //--------------------------------------------------------------------------
    // Constructor, Destructor
    //--------------------------------------------------------------------------

    inline Window::Window() {
        SingletonCheck();

        m_data                      = {};

        m_instance_handle           = NULL;
        m_window_handle             = NULL;
        m_device_context_handle     = NULL;
        m_rendering_context_handle  = NULL;

        m_window_style              = 0;
        m_window_extended_style     = 0;

        m_is_active                 = false;
        m_is_visible                = false;
        m_state                     = WINDOW_STATE_NORMAL;

         
        m_is_dragging               = false;
        m_is_stretching_by_edge     = false;
        m_is_size_restored_while_dragging  = false;
        m_is_docked                 = false;

        m_force_undock              = false;

        m_is_win7                   = IsWindows7OrGreater() && !IsWindows8OrGreater();

        m_is_apply_fake_width       = false;
        m_is_do_on_resize_enabled   = true;

        m_dbg_message_id            = 0;
    }

    inline Window::~Window() {

    }

    //--------------------------------------------------------------------------
    // Run
    //--------------------------------------------------------------------------

    inline int Window::Run(const Data& data) {
        m_data = data;

        if (!m_data.do_on_create)       m_data.do_on_create         = []() {};
        if (!m_data.do_on_destroy)      m_data.do_on_destroy        = []() {};
        if (!m_data.display)            m_data.display              = []() {};

        if (!m_data.do_on_key_down_raw) m_data.do_on_key_down_raw   = [](WPARAM w_param, LPARAM l_param) {};
        if (!m_data.do_on_key_up_raw)   m_data.do_on_key_up_raw     = [](WPARAM w_param, LPARAM l_param) {};

        if (!m_data.do_on_resize)         m_data.do_on_resize           = [](uint32_t width, uint32_t height) {};

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

        ShowWindow(m_window_handle, SW_SHOW);
        SetForegroundWindow(m_window_handle);
        SetFocus(m_window_handle);

        // For tests only.
        //m_window_area_corrector.DisableComposition();

        // Here because, actual window area can be fetched by DwmGetWindowAttribute only after SW_SHOW.
        ChangeArea(m_data.area);

        m_data.do_on_create();

        UpdateWindow(m_window_handle);

        return ExecuteMainLoop();
    }

    inline void Window::RequestClose() {
        DestroyWindow(m_window_handle);
    }

    inline void Window::RequestRedraw() {
        InvalidateRect(m_window_handle, NULL, FALSE);
    }

    //--------------------------------------------------------------------------
    // Position, Size, Area
    //--------------------------------------------------------------------------

    inline void Window::MoveTo(int x, int y) {
        SetArea({x, y, 0, 0}, AreaPartId::POSITION, false);
    }
    inline void Window::MoveTo(const PointI& pos) {
        MoveTo(pos.x, pos.y);
    }

    inline void Window::MoveBy(int x, int y) {
        const PointI curr_pos = GetArea().GetPos();

        MoveTo(curr_pos.x + x, curr_pos.y + y);
    }
    inline void Window::MoveBy(const PointI& pos) {
        MoveBy(pos.x, pos.y);
    }

    inline void Window::SetSize(int width, int height) {
        SetArea({0, 0, width, height}, AreaPartId::SIZE, false);
    }
    inline void Window::SetSize(const SizeI& size) {
        SetSize(size.width, size.height);
    }

    inline void Window::SetArea(int x, int y, int width, int height) {
        SetArea({x, y, width, height}, AreaPartId::ALL, false);
    }
    inline void Window::SetArea(const AreaI& area) {
        SetArea(area.x, area.y, area.width, area.height);
    }

    //--------------------------------------------------------------------------

    inline void Window::SetDrawAreaPos(int x, int y) {
        SetArea({x, y, 0, 0}, AreaPartId::POSITION, true);
    }
    inline void Window::SetDrawAreaPos(const PointI& pos) {
        SetDrawAreaPos(pos.x, pos.y);
    }

    inline void Window::SetDrawAreaSize(int width, int height) {
        SetArea({0, 0, width, height}, AreaPartId::SIZE, true);
    }
    inline void Window::SetDrawAreaSize(const SizeI& size) {
        SetDrawAreaSize(size.width, size.height);
    }

    inline void Window::SetDrawArea(int x, int y, int width, int height) {
        SetArea({x, y, width, height}, AreaPartId::ALL, true);
    }
    inline void Window::SetDrawArea(const AreaI& area) {
        SetDrawArea(area.x, area.y, area.width, area.height);
    }

    inline void Window::Center() {
        const AreaI desktop_area = GetDesktopAreaNoTaskBar();

        AreaI window_area = GetArea();

        window_area.x = (desktop_area.width - window_area.width) / 2;
        window_area.y = (desktop_area.height - window_area.height) / 2;

        MoveTo(window_area.GetPos());
    }

    inline void Window::ChangeArea(const AreaI& area) {
        SetArea(GenerateWindowArea(area), AreaPartId::ALL, m_data.style & StyleBit::DRAW_AREA_ONLY);
    }

    //--------------------------------------------------------------------------

    inline PointI Window::GetPos() const {
        return GetArea().GetPos();
    }

    inline SizeI Window::GetSize() const {
        return GetArea().GetSize();
    }

    inline AreaI Window::GetArea() const {
        AreaI area = GetWindowArea(m_window_handle);

        // Workaround.
        if (IsWindowedFullScreen()) area.width -= WIDTH_EXTENTION;


        return m_window_area_corrector.RemoveInvisibleFrameFrom(area, m_window_handle);
    }

    inline PointI Window::GetDrawAreaPos() const {
        return GetDrawArea().GetPos();
    }

    inline SizeI Window::GetDrawAreaSize() const {
        return GetDrawArea().GetSize();
    }

    inline AreaI Window::GetDrawArea() const {
        RECT r;
        if (GetClientRect(m_window_handle, &r) && ClientToScreen(m_window_handle, (POINT*)&r) && ClientToScreen(m_window_handle, (POINT*)&r.right)) {
            AreaI area = MakeArea(r);

            // Workaround.
            if (IsWindowedFullScreen()) area.width -= WIDTH_EXTENTION;

            return area;
        }
        return AreaI(0, 0, 0, 0);
    }

    //--------------------------------------------------------------------------
    // State
    //--------------------------------------------------------------------------

    inline void Window::ChangeStateTo(WindowState state) {
        switch (state) {
        case WINDOW_STATE_NORMAL: {
            ShowWindow(m_window_handle, SW_RESTORE);
            break;
        }
        case WINDOW_STATE_MINIMIZED: {
            ShowWindow(m_window_handle, SW_MINIMIZE);
            break;
        }
        case WINDOW_STATE_MAXIMIZED: {
            ShowWindow(m_window_handle, SW_MAXIMIZE);
            break;
        }
        } // switch end


#if 0
        // Exits from windowed fullscreen.
        if (m_state == WindowState::WINDOWED_FULL_SCREENED) {
            SetWindowLongPtrW(m_window_handle, GWL_STYLE,   m_window_style);
            SetWindowLongPtrW(m_window_handle, GWL_EXSTYLE, m_window_extended_style);
        }

        switch (state) {
        case WindowState::NORMAL:
            m_force_undock = true;
            if (m_state != WindowState::WINDOWED_FULL_SCREENED) {
                ShowWindow(m_window_handle, SW_NORMAL);
            }
            SetWindowPos(m_window_handle, HWND_TOP, m_last_window_area.x, m_last_window_area.y, m_last_window_area.width, m_last_window_area.height, SWP_SHOWWINDOW);
            m_force_undock = false;
            break;

        case WindowState::HIDDEN:
            SetWindowPos(m_window_handle, HWND_TOP, m_last_window_area.x, m_last_window_area.y, m_last_window_area.width, m_last_window_area.height, SWP_SHOWWINDOW);
            ShowWindow(m_window_handle, SW_HIDE);
            break;

        case WindowState::MINIMIZED:
            ShowWindow(m_window_handle, SW_MINIMIZE);
            break;

        case WindowState::MAXIMIZED: 
            if (m_data.style & StyleBit::DRAW_AREA_ONLY) {
                const SizeI work_area_size = GetDesktopAreaSizeNoTaskBar();

                SetWindowPos(m_window_handle, HWND_TOP, 0, 0, work_area_size.width, work_area_size.height, SWP_SHOWWINDOW);

                m_state = WindowState::MAXIMIZED;
            } else {
                ShowWindow(m_window_handle, SW_MAXIMIZE);
            }
            break;

        case WindowState::WINDOWED_FULL_SCREENED: 
            m_is_do_on_resize_enabled = false;
            SetWindowLongPtrW(m_window_handle, GWL_STYLE, GetWindowStyle_DrawAreaOnly());
            SetWindowLongPtrW(m_window_handle, GWL_EXSTYLE, GetWindowExtendedStyle_DrawAreaOnly());
            m_is_do_on_resize_enabled = true;

            // ---
            
            // Workaround. To omit situation in Windows 7 where alt+tab not working when window is borderless and covers exactly whole screen.
            RECT screen_rect = MakeRECT(AreaI({}, GetScreenSize()));
            AdjustWindowRectEx(&screen_rect, GetWindowStyle_DrawAreaOnly(), FALSE, GetWindowExtendedStyle_DrawAreaOnly());

            const AreaI screen_area = MakeArea(screen_rect);

            m_is_apply_fake_width = true;
            SetWindowPos(m_window_handle, HWND_TOP, screen_area.x, screen_area.y, screen_area.width + WIDTH_EXTENTION, screen_area.height, SWP_SHOWWINDOW);
            m_is_apply_fake_width = false;

            // ---

            m_state = WindowState::WINDOWED_FULL_SCREENED;

            break;
        }
#endif
    }

    inline void Window::Hide() {
        ShowWindow(m_window_handle, SW_HIDE);
    }

    inline void Window::Show() {
        ShowWindow(m_window_handle, SW_SHOW);
    }

    inline bool Window::IsVisible() const {
        return m_is_visible;
    }



    inline void Window::Restore() {
        ChangeStateTo(WINDOW_STATE_NORMAL);
    }

    inline void Window::Minimize() {
        ChangeStateTo(WINDOW_STATE_MINIMIZED);
    }

    inline void Window::Maximize() {
        ChangeStateTo(WINDOW_STATE_MAXIMIZED);
    }

    inline void Window::GoWindowedFullScreen() {
        //ChangeStateTo(WindowState::WINDOWED_FULL_SCREENED);
    }

    inline WindowState Window::GetState() const {
        return m_state;
    }

    inline bool Window::IsNormal() const {
        return GetState() == WINDOW_STATE_NORMAL;
    }

    inline bool Window::IsMinimized() const {
        return GetState() == WINDOW_STATE_MINIMIZED;
    }

    inline bool Window::IsMaximized() const {
        return GetState() == WINDOW_STATE_MAXIMIZED;
    }

    inline bool Window::IsWindowedFullScreen() const {
        return false; //GetState() == WindowState::WINDOWED_FULL_SCREENED;
    }

    //--------------------------------------------------------------------------
    // Get Generic
    //--------------------------------------------------------------------------

    inline uint32_t Window::GetDebugLevel() const { 
        return m_data.log_level; 
    }

    inline Version Window::GetOpenGL_Version() const {
        return m_data.opengl_verion;
    }

    inline Window& Window::To() {
        return Static<Window>::To();
    }

    //--------------------------------------------------------------------------
    // Private
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
                return (int)msg.wParam;
            }

        } else {

            while (true) {
                if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
                    if (msg.message == WM_QUIT) {
                        return (int)msg.wParam;
                    }

                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                } else {
                    Display();
                }
            }

        }

        return EXIT_FAILURE;
    }
    
    inline void Window::Display() {
        if (m_data.special_debug.is_notify_each_draw_call) {
            LogDebug("Window::Display"); 
        }

        m_data.display();

        SwapBuffers(m_device_context_handle);
    }

    inline AreaI Window::GenerateWindowArea(const AreaI& area) {
        AreaI window_area;

        // --- Size --- //

        const AreaI desktop_area = GetDesktopAreaNoTaskBar();

        window_area.width   = (area.width != DEF)   ? area.width    : (desktop_area.width / 2);
        window_area.height  = (area.height != DEF)  ? area.height   : (desktop_area.height / 2);

        // In a case of unreasonable values.
        if (window_area.width < 0)    window_area.width = 0;
        if (window_area.height < 1)   window_area.height = 1;

        if ((m_data.style & StyleBit::DRAW_AREA_SIZE) && !(m_data.style & StyleBit::DRAW_AREA_ONLY)) {
            const AreaI window_area_with_invisible_frame = GetWindowAreaFromDrawArea(window_area, m_window_style);

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
    
    inline void Window::SetArea(const AreaI& area, AreaPartId area_part_id, bool is_draw_area) {
        //if (GetState() != WINDOW_STATE_NORMAL) ChangeStateTo(WINDOW_STATE_NORMAL);

        auto GetFlags = [](AreaPartId area_part_id) -> UINT {
            switch (area_part_id) {
            case AreaPartId::POSITION:  return SWP_NOSIZE;
            case AreaPartId::SIZE:      return SWP_NOMOVE;
            case AreaPartId::ALL:       return 0;
            }
        };

        m_last_window_area = 
            is_draw_area ? 
            GetWindowAreaFromDrawArea(area, m_window_style) : 
            m_window_area_corrector.AddInvisibleFrameTo(area, m_window_handle);

        SetWindowPos(m_window_handle, HWND_TOP, m_last_window_area.x, m_last_window_area.y, m_last_window_area.width, m_last_window_area.height, GetFlags(area_part_id));
    }

    //--------------------------------------------------------------------------

    inline DWORD Window::GetWindowStyle_DrawAreaOnly() { 
        return 
            WS_POPUP 
            // Commented for tests.
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
    
    inline AreaI Window::GetWindowArea(HWND window_handle) {
        RECT r;
        if (GetWindowRect(window_handle, &r)) {
            return MakeArea(r);
        }
        return AreaI(0, 0, 0, 0);
    }

    inline AreaI Window::GetWindowAreaFromDrawArea(const AreaI& draw_area, DWORD window_style) {
        RECT rect = MakeRECT(draw_area);
        AdjustWindowRect(&rect, window_style, FALSE);
        return MakeArea(rect);
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
            HGLRC (*wglCreateContextAttribsARB)(HDC hDC, HGLRC hShareContext, const int* attribList) = (decltype(wglCreateContextAttribsARB)) wglGetProcAddress("wglCreateContextAttribsARB");
            if (wglCreateContextAttribsARB) {
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

                HGLRC rendering_context_handle = wglCreateContextAttribsARB(m_device_context_handle, 0, attribute_list);
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

        m_data.opengl_verion = {0, 0};
        void (*glGetIntegerv)(GLenum pname, GLint* data) = (decltype(glGetIntegerv)) wglGetProcAddress("glGetIntegerv");
        if (glGetIntegerv) {
            enum {
                // Added prefix TOGL_ to standard OpenGL constants.
                TOGL_GL_MAJOR_VERSION = 0x821B,
                TOGL_GL_MINOR_VERSION = 0x821C,
            };
            glGetIntegerv(TOGL_GL_MAJOR_VERSION, &m_data.opengl_verion.major);
            glGetIntegerv(TOGL_GL_MINOR_VERSION, &m_data.opengl_verion.minor);
        } else {
            sscanf_s((const char*)glGetString(GL_VERSION), "%d.%d", &m_data.opengl_verion.major, &m_data.opengl_verion.minor);
        }

        if (m_data.log_level >= LOG_LEVEL_INFO) LogInfo(std::string("OpenGl Version: ") + (const char*)glGetString(GL_VERSION));
    }

    inline void Window::Destroy() {
        if (m_data.log_level >= LOG_LEVEL_DEBUG) {
            LogDebug("Window::Destroy");
        }

        m_data.do_on_destroy();

        wglMakeCurrent(NULL, NULL); 
        wglDeleteContext(m_rendering_context_handle);
        ReleaseDC(m_window_handle, m_device_context_handle);
        PostQuitMessage(0);
    }

    //--------------------------------------------------------------------------

    inline LRESULT Window::InnerWindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
        if (m_data.special_debug.is_notify_each_window_message) {
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

            Display();

            ValidateRect(m_window_handle, NULL); // to decrease number of WM_PAINT messages

            return 0;

        case WM_KEYDOWN:
            m_data.do_on_key_down_raw(w_param, l_param);
            return 0;

        case WM_KEYUP:
           m_data.do_on_key_up_raw(w_param, l_param);
            return 0;

        //case WM_MOVING: {
        //    if (m_data.log_level >= LOG_LEVEL_DEBUG) {
        //        std::string message = "WM_MOVING";
        //
        //        RECT* drag_rect = (RECT*)(l_param); // in screen coordinates
        //
        //        message += std::string() + " drag_rect=" 
        //            + std::to_string(drag_rect->left) + "," 
        //            + std::to_string(drag_rect->top) + ","
        //            + std::to_string(drag_rect->right) + ","
        //            + std::to_string(drag_rect->bottom);
        //
        //        LogDebug(message);
        //    }
        //
        //    RECT window_rect;
        //    GetWindowRect(m_window_handle, &window_rect);
        //    m_last_window_area = MakeArea(window_rect);
        //
        //    return TRUE;
        //}

        //case WM_WINDOWPOSCHANGING: {
        //    if (m_data.log_level >= LOG_LEVEL_DEBUG) {
        //        std::string dbg_msg = "WM_WINDOWPOSCHANGING";
        //
        //
        //        const WINDOWPOS* wp = (const WINDOWPOS*)(l_param);
        //
        //        dbg_msg += std::string() + " pos=" 
        //            + std::to_string(wp->x) + "," 
        //            + std::to_string(wp->y);
        //
        //        LogDebug(dbg_msg);
        //
        //    }
        //    return 0;
        //}
         
        //case WM_GETMINMAXINFO: {
        //    if (m_data.log_level >= LOG_LEVEL_DEBUG) {
        //        std::string dbg_msg = "WM_GETMINMAXINFO ";
        //
        //        const MINMAXINFO* info = (const MINMAXINFO*)(l_param);
        //
        //        dbg_msg += std::string() 
        //            + " MaxSize=" + std::to_string(info->ptMaxSize.x) + "," + std::to_string(info->ptMaxSize.y)
        //            + " ptMaxPosition=" + std::to_string(info->ptMaxPosition.x) + "," + std::to_string(info->ptMaxPosition.y)
        //            + " ptMinTrackSize=" + std::to_string(info->ptMinTrackSize.x) + "," + std::to_string(info->ptMinTrackSize.y)
        //            + " ptMaxTrackSize=" + std::to_string(info->ptMaxTrackSize.x) + "," + std::to_string(info->ptMaxTrackSize.y)
        //            ;
        //
        //        LogDebug(dbg_msg);
        //
        //    }
        //    break;
        //}

        case WM_ENTERSIZEMOVE: {
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
                LogDebug("WM_ENTERSIZEMOVE");
            }

            m_is_dragging                       = true;
            m_is_stretching_by_edge             = false;
            m_is_size_restored_while_dragging   = false;

            return 0;
        }

        case WM_EXITSIZEMOVE: {
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
                LogDebug("WM_EXITSIZEMOVE");
            }

            if (!m_is_stretching_by_edge && m_is_size_restored_while_dragging) {
                m_is_docked = true;
                if (m_data.log_level >= LOG_LEVEL_DEBUG) LogDebug("docked");
            }

            if (!m_is_docked) {
                RECT window_rect;
                GetWindowRect(m_window_handle, &window_rect);
                m_last_window_area = MakeArea(window_rect);
            }

            m_is_dragging = false;

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

                dbg_msg += std::string() + " " + GetEdgeName(w_param);

                RECT* drag_rect = (RECT*)(l_param); // in screen coordinates

                dbg_msg += std::string() + " drag_rect=" 
                    + std::to_string(drag_rect->left) + "," 
                    + std::to_string(drag_rect->top) + ","
                    + std::to_string(drag_rect->right) + ","
                    + std::to_string(drag_rect->bottom);

                LogDebug(dbg_msg);
            }

            auto IsByEdgeOtCorner = [](WPARAM w_param) -> bool {
                switch (w_param) {
                case WMSZ_BOTTOM:       return true;
                case WMSZ_BOTTOMLEFT:   return true;
                case WMSZ_BOTTOMRIGHT:  return true;
                case WMSZ_LEFT:         return true;
                case WMSZ_RIGHT:        return true;
                case WMSZ_TOP:          return true;
                case WMSZ_TOPLEFT:      return true;
                case WMSZ_TOPRIGHT:     return true;
                }
                return false;
            };

            // Parameter w_param can contain other values than defined ones, for example: 9 when undocking window.
            const bool is_by_endge_or_corner = IsByEdgeOtCorner(w_param);

            if (m_is_dragging && is_by_endge_or_corner) {
                m_is_stretching_by_edge = true;
            }

            if (m_is_docked && !is_by_endge_or_corner) {
                m_is_docked = false;
                if (m_data.log_level >= LOG_LEVEL_DEBUG) LogDebug("undocked");
            }

            return TRUE;
        }

        case WM_SIZE:
            if (m_data.log_level >= LOG_LEVEL_DEBUG) {
                std::string dbg_msg = "WM_SIZE";

                const int width     = LOWORD(l_param);
                const int height    = HIWORD(l_param);

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

                if (m_is_apply_fake_width) dbg_msg += " fake_width=" + std::to_string(width - WIDTH_EXTENTION);

                if (!m_is_do_on_resize_enabled) dbg_msg += " without:do_on_resize";

                LogDebug(dbg_msg);
            }

            if (m_is_dragging) {
                if (w_param == SIZE_RESTORED) m_is_size_restored_while_dragging = true;
            } else {
                if (m_is_docked) {
                    const bool is_minimize                  = w_param == SIZE_MINIMIZED;
                    const bool is_maximize_from_minimize    = w_param == SIZE_RESTORED && m_state == WINDOW_STATE_MINIMIZED;

                    if (!(is_minimize || is_maximize_from_minimize) || m_force_undock) {
                        m_is_docked = false;
                        if (m_data.log_level >= LOG_LEVEL_DEBUG) LogDebug("undocked");
                    }
                }
            }

            if (m_is_do_on_resize_enabled) {
                if (m_is_apply_fake_width) {
                    m_data.do_on_resize(LOWORD(l_param) - WIDTH_EXTENTION, HIWORD(l_param));
                } else {
                    m_data.do_on_resize(LOWORD(l_param), HIWORD(l_param));
                }
            }

            switch (w_param) {
            case SIZE_MAXIMIZED:    m_state = WINDOW_STATE_MAXIMIZED; break;
            case SIZE_MINIMIZED:    m_state = WINDOW_STATE_MINIMIZED; break;
            case SIZE_RESTORED:     m_state = WINDOW_STATE_NORMAL; break;
            }

            return 0;

        case WM_SHOWWINDOW:
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

            m_is_visible = (w_param == TRUE);

            return 0;

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
                    //SetForegroundWindow(m_window_handle);
                    // do_on_foreground(true)
                } else {
                    // do_on_foreground(false)
                }
            }

            return 0;	
        }
        case WM_ACTIVATEAPP:
            if (!w_param) {
                // Old workaround for disabled alt+tab in Windows 7.
                //if (m_is_win7) ShowWindow(m_window_handle, SW_SHOWMINIMIZED);
            }
            return 0;

        case WM_SYSCOMMAND:
            switch (w_param) {
            case SC_SCREENSAVE:					// No screen saver.
            case SC_MONITORPOWER:				// No entering to power save mode.
                return 0;						
            }
            break;								
        }

        return DefWindowProcW(window_handle, message, w_param, l_param);
    }

    inline LRESULT CALLBACK Window::WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
        return To().InnerWindowProc(window_handle, message, w_param, l_param);
    }

#ifdef TOGL_INNER_CASE_STR
#error TOGL_INNER_CASE_STR is already defined.
#endif

#define TOGL_INNER_CASE_STR(name) case name: return #name

    inline std::string Window::WM_ToStr(UINT message) {
        switch (message) {
        TOGL_INNER_CASE_STR(WM_NULL);
        TOGL_INNER_CASE_STR(WM_CREATE);
        TOGL_INNER_CASE_STR(WM_DESTROY);
        TOGL_INNER_CASE_STR(WM_MOVE);
        TOGL_INNER_CASE_STR(WM_SIZE);
        TOGL_INNER_CASE_STR(WM_ACTIVATE);
        TOGL_INNER_CASE_STR(WM_SETFOCUS);
        TOGL_INNER_CASE_STR(WM_KILLFOCUS);
        TOGL_INNER_CASE_STR(WM_ENABLE);
        TOGL_INNER_CASE_STR(WM_SETREDRAW);
        TOGL_INNER_CASE_STR(WM_SETTEXT);
        TOGL_INNER_CASE_STR(WM_GETTEXT);
        TOGL_INNER_CASE_STR(WM_GETTEXTLENGTH);
        TOGL_INNER_CASE_STR(WM_PAINT);
        TOGL_INNER_CASE_STR(WM_CLOSE);
        TOGL_INNER_CASE_STR(WM_QUERYENDSESSION);
        TOGL_INNER_CASE_STR(WM_QUIT);
        TOGL_INNER_CASE_STR(WM_QUERYOPEN);
        TOGL_INNER_CASE_STR(WM_ERASEBKGND);
        TOGL_INNER_CASE_STR(WM_SYSCOLORCHANGE);
        TOGL_INNER_CASE_STR(WM_ENDSESSION);
        TOGL_INNER_CASE_STR(WM_SHOWWINDOW);
        //TOGL_INNER_CASE_STR(WM_CTLCOLOR);
        TOGL_INNER_CASE_STR(WM_WININICHANGE);
        TOGL_INNER_CASE_STR(WM_DEVMODECHANGE);
        TOGL_INNER_CASE_STR(WM_ACTIVATEAPP);
        TOGL_INNER_CASE_STR(WM_FONTCHANGE);
        TOGL_INNER_CASE_STR(WM_TIMECHANGE);
        TOGL_INNER_CASE_STR(WM_CANCELMODE);
        TOGL_INNER_CASE_STR(WM_SETCURSOR);
        TOGL_INNER_CASE_STR(WM_MOUSEACTIVATE);
        TOGL_INNER_CASE_STR(WM_CHILDACTIVATE);
        TOGL_INNER_CASE_STR(WM_QUEUESYNC);
        TOGL_INNER_CASE_STR(WM_GETMINMAXINFO);
        TOGL_INNER_CASE_STR(WM_PAINTICON);
        TOGL_INNER_CASE_STR(WM_ICONERASEBKGND);
        TOGL_INNER_CASE_STR(WM_NEXTDLGCTL);
        TOGL_INNER_CASE_STR(WM_SPOOLERSTATUS);
        TOGL_INNER_CASE_STR(WM_DRAWITEM);
        TOGL_INNER_CASE_STR(WM_MEASUREITEM);
        TOGL_INNER_CASE_STR(WM_DELETEITEM);
        TOGL_INNER_CASE_STR(WM_VKEYTOITEM);
        TOGL_INNER_CASE_STR(WM_CHARTOITEM);
        TOGL_INNER_CASE_STR(WM_SETFONT);
        TOGL_INNER_CASE_STR(WM_GETFONT);
        TOGL_INNER_CASE_STR(WM_SETHOTKEY);
        TOGL_INNER_CASE_STR(WM_GETHOTKEY);
        TOGL_INNER_CASE_STR(WM_QUERYDRAGICON);
        TOGL_INNER_CASE_STR(WM_COMPAREITEM);
        TOGL_INNER_CASE_STR(WM_GETOBJECT);
        TOGL_INNER_CASE_STR(WM_COMPACTING);
        TOGL_INNER_CASE_STR(WM_COMMNOTIFY);
        TOGL_INNER_CASE_STR(WM_WINDOWPOSCHANGING);
        TOGL_INNER_CASE_STR(WM_WINDOWPOSCHANGED);
        TOGL_INNER_CASE_STR(WM_POWER);
        //TOGL_INNER_CASE_STR(WM_COPYGLOBALDATA);
        TOGL_INNER_CASE_STR(WM_COPYDATA);
        TOGL_INNER_CASE_STR(WM_CANCELJOURNAL);
        TOGL_INNER_CASE_STR(WM_NOTIFY);
        TOGL_INNER_CASE_STR(WM_INPUTLANGCHANGEREQUEST);
        TOGL_INNER_CASE_STR(WM_INPUTLANGCHANGE);
        TOGL_INNER_CASE_STR(WM_TCARD);
        TOGL_INNER_CASE_STR(WM_HELP);
        TOGL_INNER_CASE_STR(WM_USERCHANGED);
        TOGL_INNER_CASE_STR(WM_NOTIFYFORMAT);
        TOGL_INNER_CASE_STR(WM_CONTEXTMENU);
        TOGL_INNER_CASE_STR(WM_STYLECHANGING);
        TOGL_INNER_CASE_STR(WM_STYLECHANGED);
        TOGL_INNER_CASE_STR(WM_DISPLAYCHANGE);
        TOGL_INNER_CASE_STR(WM_GETICON);
        TOGL_INNER_CASE_STR(WM_SETICON);
        TOGL_INNER_CASE_STR(WM_NCCREATE);
        TOGL_INNER_CASE_STR(WM_NCDESTROY);
        TOGL_INNER_CASE_STR(WM_NCCALCSIZE);
        TOGL_INNER_CASE_STR(WM_NCHITTEST);
        TOGL_INNER_CASE_STR(WM_NCPAINT);
        TOGL_INNER_CASE_STR(WM_NCACTIVATE);
        TOGL_INNER_CASE_STR(WM_GETDLGCODE);
        TOGL_INNER_CASE_STR(WM_SYNCPAINT);
        TOGL_INNER_CASE_STR(WM_NCMOUSEMOVE);
        TOGL_INNER_CASE_STR(WM_NCLBUTTONDOWN);
        TOGL_INNER_CASE_STR(WM_NCLBUTTONUP);
        TOGL_INNER_CASE_STR(WM_NCLBUTTONDBLCLK);
        TOGL_INNER_CASE_STR(WM_NCRBUTTONDOWN);
        TOGL_INNER_CASE_STR(WM_NCRBUTTONUP);
        TOGL_INNER_CASE_STR(WM_NCRBUTTONDBLCLK);
        TOGL_INNER_CASE_STR(WM_NCMBUTTONDOWN);
        TOGL_INNER_CASE_STR(WM_NCMBUTTONUP);
        TOGL_INNER_CASE_STR(WM_NCMBUTTONDBLCLK);
        TOGL_INNER_CASE_STR(WM_NCXBUTTONDOWN);
        TOGL_INNER_CASE_STR(WM_NCXBUTTONUP);
        TOGL_INNER_CASE_STR(WM_NCXBUTTONDBLCLK);
        TOGL_INNER_CASE_STR(EM_GETSEL);
        TOGL_INNER_CASE_STR(EM_SETSEL);
        TOGL_INNER_CASE_STR(EM_GETRECT);
        TOGL_INNER_CASE_STR(EM_SETRECT);
        TOGL_INNER_CASE_STR(EM_SETRECTNP);
        TOGL_INNER_CASE_STR(EM_SCROLL);
        TOGL_INNER_CASE_STR(EM_LINESCROLL);
        TOGL_INNER_CASE_STR(EM_SCROLLCARET);
        TOGL_INNER_CASE_STR(EM_GETMODIFY);
        TOGL_INNER_CASE_STR(EM_SETMODIFY);
        TOGL_INNER_CASE_STR(EM_GETLINECOUNT);
        TOGL_INNER_CASE_STR(EM_LINEINDEX);
        TOGL_INNER_CASE_STR(EM_SETHANDLE);
        TOGL_INNER_CASE_STR(EM_GETHANDLE);
        TOGL_INNER_CASE_STR(EM_GETTHUMB);
        TOGL_INNER_CASE_STR(EM_LINELENGTH);
        TOGL_INNER_CASE_STR(EM_REPLACESEL);
        //TOGL_INNER_CASE_STR(EM_SETFONT);
        TOGL_INNER_CASE_STR(EM_GETLINE);
        TOGL_INNER_CASE_STR(EM_LIMITTEXT);
        //TOGL_INNER_CASE_STR(EM_SETLIMITTEXT);
        TOGL_INNER_CASE_STR(EM_CANUNDO);
        TOGL_INNER_CASE_STR(EM_UNDO);
        TOGL_INNER_CASE_STR(EM_FMTLINES);
        TOGL_INNER_CASE_STR(EM_LINEFROMCHAR);
        //TOGL_INNER_CASE_STR(EM_SETWORDBREAK);
        TOGL_INNER_CASE_STR(EM_SETTABSTOPS);
        TOGL_INNER_CASE_STR(EM_SETPASSWORDCHAR);
        TOGL_INNER_CASE_STR(EM_EMPTYUNDOBUFFER);
        TOGL_INNER_CASE_STR(EM_GETFIRSTVISIBLELINE);
        TOGL_INNER_CASE_STR(EM_SETREADONLY);
        TOGL_INNER_CASE_STR(EM_SETWORDBREAKPROC);
        TOGL_INNER_CASE_STR(EM_GETWORDBREAKPROC);
        TOGL_INNER_CASE_STR(EM_GETPASSWORDCHAR);
        TOGL_INNER_CASE_STR(EM_SETMARGINS);
        TOGL_INNER_CASE_STR(EM_GETMARGINS);
        TOGL_INNER_CASE_STR(EM_GETLIMITTEXT);
        TOGL_INNER_CASE_STR(EM_POSFROMCHAR);
        TOGL_INNER_CASE_STR(EM_CHARFROMPOS);
        TOGL_INNER_CASE_STR(EM_SETIMESTATUS);
        TOGL_INNER_CASE_STR(EM_GETIMESTATUS);
        TOGL_INNER_CASE_STR(SBM_SETPOS);
        TOGL_INNER_CASE_STR(SBM_GETPOS);
        TOGL_INNER_CASE_STR(SBM_SETRANGE);
        TOGL_INNER_CASE_STR(SBM_GETRANGE);
        TOGL_INNER_CASE_STR(SBM_ENABLE_ARROWS);
        TOGL_INNER_CASE_STR(SBM_SETRANGEREDRAW);
        TOGL_INNER_CASE_STR(SBM_SETSCROLLINFO);
        TOGL_INNER_CASE_STR(SBM_GETSCROLLINFO);
        TOGL_INNER_CASE_STR(SBM_GETSCROLLBARINFO);
        TOGL_INNER_CASE_STR(BM_GETCHECK);
        TOGL_INNER_CASE_STR(BM_SETCHECK);
        TOGL_INNER_CASE_STR(BM_GETSTATE);
        TOGL_INNER_CASE_STR(BM_SETSTATE);
        TOGL_INNER_CASE_STR(BM_SETSTYLE);
        TOGL_INNER_CASE_STR(BM_CLICK);
        TOGL_INNER_CASE_STR(BM_GETIMAGE);
        TOGL_INNER_CASE_STR(BM_SETIMAGE);
        TOGL_INNER_CASE_STR(BM_SETDONTCLICK);
        TOGL_INNER_CASE_STR(WM_INPUT);
        TOGL_INNER_CASE_STR(WM_KEYDOWN);
        //TOGL_INNER_CASE_STR(WM_KEYFIRST);
        TOGL_INNER_CASE_STR(WM_KEYUP);
        TOGL_INNER_CASE_STR(WM_CHAR);
        TOGL_INNER_CASE_STR(WM_DEADCHAR);
        TOGL_INNER_CASE_STR(WM_SYSKEYDOWN);
        TOGL_INNER_CASE_STR(WM_SYSKEYUP);
        TOGL_INNER_CASE_STR(WM_SYSCHAR);
        TOGL_INNER_CASE_STR(WM_SYSDEADCHAR);
        //TOGL_INNER_CASE_STR(WM_UNICHAR / WM_KEYLAST);
        //TOGL_INNER_CASE_STR(WM_WNT_CONVERTREQUESTEX);
        //TOGL_INNER_CASE_STR(WM_CONVERTREQUEST);
        //TOGL_INNER_CASE_STR(WM_CONVERTRESULT);
        //TOGL_INNER_CASE_STR(WM_INTERIM);
        TOGL_INNER_CASE_STR(WM_IME_STARTCOMPOSITION);
        TOGL_INNER_CASE_STR(WM_IME_ENDCOMPOSITION);
        TOGL_INNER_CASE_STR(WM_IME_COMPOSITION);
        //TOGL_INNER_CASE_STR(WM_IME_KEYLAST);
        TOGL_INNER_CASE_STR(WM_INITDIALOG);
        TOGL_INNER_CASE_STR(WM_COMMAND);
        TOGL_INNER_CASE_STR(WM_SYSCOMMAND);
        TOGL_INNER_CASE_STR(WM_TIMER);
        TOGL_INNER_CASE_STR(WM_HSCROLL);
        TOGL_INNER_CASE_STR(WM_VSCROLL);
        TOGL_INNER_CASE_STR(WM_INITMENU);
        TOGL_INNER_CASE_STR(WM_INITMENUPOPUP);
        //TOGL_INNER_CASE_STR(WM_SYSTIMER);
        TOGL_INNER_CASE_STR(WM_MENUSELECT);
        TOGL_INNER_CASE_STR(WM_MENUCHAR);
        TOGL_INNER_CASE_STR(WM_ENTERIDLE);
        TOGL_INNER_CASE_STR(WM_MENURBUTTONUP);
        TOGL_INNER_CASE_STR(WM_MENUDRAG);
        TOGL_INNER_CASE_STR(WM_MENUGETOBJECT);
        TOGL_INNER_CASE_STR(WM_UNINITMENUPOPUP);
        TOGL_INNER_CASE_STR(WM_MENUCOMMAND);
        TOGL_INNER_CASE_STR(WM_CHANGEUISTATE);
        TOGL_INNER_CASE_STR(WM_UPDATEUISTATE);
        TOGL_INNER_CASE_STR(WM_QUERYUISTATE);
        //TOGL_INNER_CASE_STR(WM_LBTRACKPOINT);
        TOGL_INNER_CASE_STR(WM_CTLCOLORMSGBOX);
        TOGL_INNER_CASE_STR(WM_CTLCOLOREDIT);
        TOGL_INNER_CASE_STR(WM_CTLCOLORLISTBOX);
        TOGL_INNER_CASE_STR(WM_CTLCOLORBTN);
        TOGL_INNER_CASE_STR(WM_CTLCOLORDLG);
        TOGL_INNER_CASE_STR(WM_CTLCOLORSCROLLBAR);
        TOGL_INNER_CASE_STR(WM_CTLCOLORSTATIC);
        TOGL_INNER_CASE_STR(CB_GETEDITSEL);
        TOGL_INNER_CASE_STR(CB_LIMITTEXT);
        TOGL_INNER_CASE_STR(CB_SETEDITSEL);
        TOGL_INNER_CASE_STR(CB_ADDSTRING);
        TOGL_INNER_CASE_STR(CB_DELETESTRING);
        TOGL_INNER_CASE_STR(CB_DIR);
        TOGL_INNER_CASE_STR(CB_GETCOUNT);
        TOGL_INNER_CASE_STR(CB_GETCURSEL);
        TOGL_INNER_CASE_STR(CB_GETLBTEXT);
        TOGL_INNER_CASE_STR(CB_GETLBTEXTLEN);
        TOGL_INNER_CASE_STR(CB_INSERTSTRING);
        TOGL_INNER_CASE_STR(CB_RESETCONTENT);
        TOGL_INNER_CASE_STR(CB_FINDSTRING);
        TOGL_INNER_CASE_STR(CB_SELECTSTRING);
        TOGL_INNER_CASE_STR(CB_SETCURSEL);
        TOGL_INNER_CASE_STR(CB_SHOWDROPDOWN);
        TOGL_INNER_CASE_STR(CB_GETITEMDATA);
        TOGL_INNER_CASE_STR(CB_SETITEMDATA);
        TOGL_INNER_CASE_STR(CB_GETDROPPEDCONTROLRECT);
        TOGL_INNER_CASE_STR(CB_SETITEMHEIGHT);
        TOGL_INNER_CASE_STR(CB_GETITEMHEIGHT);
        TOGL_INNER_CASE_STR(CB_SETEXTENDEDUI);
        TOGL_INNER_CASE_STR(CB_GETEXTENDEDUI);
        TOGL_INNER_CASE_STR(CB_GETDROPPEDSTATE);
        TOGL_INNER_CASE_STR(CB_FINDSTRINGEXACT);
        TOGL_INNER_CASE_STR(CB_SETLOCALE);
        TOGL_INNER_CASE_STR(CB_GETLOCALE);
        TOGL_INNER_CASE_STR(CB_GETTOPINDEX);
        TOGL_INNER_CASE_STR(CB_SETTOPINDEX);
        TOGL_INNER_CASE_STR(CB_GETHORIZONTALEXTENT);
        TOGL_INNER_CASE_STR(CB_SETHORIZONTALEXTENT);
        TOGL_INNER_CASE_STR(CB_GETDROPPEDWIDTH);
        TOGL_INNER_CASE_STR(CB_SETDROPPEDWIDTH);
        TOGL_INNER_CASE_STR(CB_INITSTORAGE);
        //TOGL_INNER_CASE_STR(CB_MULTIPLEADDSTRING);
        TOGL_INNER_CASE_STR(CB_GETCOMBOBOXINFO);
        TOGL_INNER_CASE_STR(CB_MSGMAX);
        TOGL_INNER_CASE_STR(WM_MOUSEFIRST);
        //TOGL_INNER_CASE_STR(WM_MOUSEMOVE);
        TOGL_INNER_CASE_STR(WM_LBUTTONDOWN);
        TOGL_INNER_CASE_STR(WM_LBUTTONUP);
        TOGL_INNER_CASE_STR(WM_LBUTTONDBLCLK);
        TOGL_INNER_CASE_STR(WM_RBUTTONDOWN);
        TOGL_INNER_CASE_STR(WM_RBUTTONUP);
        TOGL_INNER_CASE_STR(WM_RBUTTONDBLCLK);
        TOGL_INNER_CASE_STR(WM_MBUTTONDOWN);
        TOGL_INNER_CASE_STR(WM_MBUTTONUP);
        TOGL_INNER_CASE_STR(WM_MBUTTONDBLCLK);
        TOGL_INNER_CASE_STR(WM_MOUSELAST);
        TOGL_INNER_CASE_STR(WM_MOUSEWHEEL);
        TOGL_INNER_CASE_STR(WM_XBUTTONDOWN);
        TOGL_INNER_CASE_STR(WM_XBUTTONUP);
        TOGL_INNER_CASE_STR(WM_XBUTTONDBLCLK);
        //TOGL_INNER_CASE_STR(WM_MOUSEHWHEEL);
        TOGL_INNER_CASE_STR(WM_PARENTNOTIFY);
        TOGL_INNER_CASE_STR(WM_ENTERMENULOOP);
        TOGL_INNER_CASE_STR(WM_EXITMENULOOP);
        TOGL_INNER_CASE_STR(WM_NEXTMENU);
        TOGL_INNER_CASE_STR(WM_SIZING);
        TOGL_INNER_CASE_STR(WM_CAPTURECHANGED);
        TOGL_INNER_CASE_STR(WM_MOVING);
        TOGL_INNER_CASE_STR(WM_POWERBROADCAST);
        TOGL_INNER_CASE_STR(WM_DEVICECHANGE);
        TOGL_INNER_CASE_STR(WM_MDICREATE);
        TOGL_INNER_CASE_STR(WM_MDIDESTROY);
        TOGL_INNER_CASE_STR(WM_MDIACTIVATE);
        TOGL_INNER_CASE_STR(WM_MDIRESTORE);
        TOGL_INNER_CASE_STR(WM_MDINEXT);
        TOGL_INNER_CASE_STR(WM_MDIMAXIMIZE);
        TOGL_INNER_CASE_STR(WM_MDITILE);
        TOGL_INNER_CASE_STR(WM_MDICASCADE);
        TOGL_INNER_CASE_STR(WM_MDIICONARRANGE);
        TOGL_INNER_CASE_STR(WM_MDIGETACTIVE);
        TOGL_INNER_CASE_STR(WM_MDISETMENU);
        TOGL_INNER_CASE_STR(WM_ENTERSIZEMOVE);
        TOGL_INNER_CASE_STR(WM_EXITSIZEMOVE);
        TOGL_INNER_CASE_STR(WM_DROPFILES);
        TOGL_INNER_CASE_STR(WM_MDIREFRESHMENU);
        //TOGL_INNER_CASE_STR(WM_IME_REPORT);
        TOGL_INNER_CASE_STR(WM_IME_SETCONTEXT);
        TOGL_INNER_CASE_STR(WM_IME_NOTIFY);
        TOGL_INNER_CASE_STR(WM_IME_CONTROL);
        TOGL_INNER_CASE_STR(WM_IME_COMPOSITIONFULL);
        TOGL_INNER_CASE_STR(WM_IME_SELECT);
        TOGL_INNER_CASE_STR(WM_IME_CHAR);
        TOGL_INNER_CASE_STR(WM_IME_REQUEST);
        //TOGL_INNER_CASE_STR(WM_IMEKEYDOWN);
        TOGL_INNER_CASE_STR(WM_IME_KEYDOWN);
        //TOGL_INNER_CASE_STR(WM_IMEKEYUP);
        TOGL_INNER_CASE_STR(WM_IME_KEYUP);
        TOGL_INNER_CASE_STR(WM_NCMOUSEHOVER);
        TOGL_INNER_CASE_STR(WM_MOUSEHOVER);
        TOGL_INNER_CASE_STR(WM_NCMOUSELEAVE);
        TOGL_INNER_CASE_STR(WM_MOUSELEAVE);
        TOGL_INNER_CASE_STR(WM_CUT);
        TOGL_INNER_CASE_STR(WM_COPY);
        TOGL_INNER_CASE_STR(WM_PASTE);
        TOGL_INNER_CASE_STR(WM_CLEAR);
        TOGL_INNER_CASE_STR(WM_UNDO);
        TOGL_INNER_CASE_STR(WM_RENDERFORMAT);
        TOGL_INNER_CASE_STR(WM_RENDERALLFORMATS);
        TOGL_INNER_CASE_STR(WM_DESTROYCLIPBOARD);
        TOGL_INNER_CASE_STR(WM_DRAWCLIPBOARD);
        TOGL_INNER_CASE_STR(WM_PAINTCLIPBOARD);
        TOGL_INNER_CASE_STR(WM_VSCROLLCLIPBOARD);
        TOGL_INNER_CASE_STR(WM_SIZECLIPBOARD);
        TOGL_INNER_CASE_STR(WM_ASKCBFORMATNAME);
        TOGL_INNER_CASE_STR(WM_CHANGECBCHAIN);
        TOGL_INNER_CASE_STR(WM_HSCROLLCLIPBOARD);
        TOGL_INNER_CASE_STR(WM_QUERYNEWPALETTE);
        TOGL_INNER_CASE_STR(WM_PALETTEISCHANGING);
        TOGL_INNER_CASE_STR(WM_PALETTECHANGED);
        TOGL_INNER_CASE_STR(WM_HOTKEY);
        TOGL_INNER_CASE_STR(WM_PRINT);
        TOGL_INNER_CASE_STR(WM_PRINTCLIENT);
        TOGL_INNER_CASE_STR(WM_APPCOMMAND);
        TOGL_INNER_CASE_STR(WM_HANDHELDFIRST);
        TOGL_INNER_CASE_STR(WM_HANDHELDLAST);
        TOGL_INNER_CASE_STR(WM_AFXFIRST);
        TOGL_INNER_CASE_STR(WM_AFXLAST);
        TOGL_INNER_CASE_STR(WM_PENWINFIRST);
        //TOGL_INNER_CASE_STR(WM_RCRESULT);
        //TOGL_INNER_CASE_STR(WM_HOOKRCRESULT);
        //TOGL_INNER_CASE_STR(WM_GLOBALRCCHANGE);
        //TOGL_INNER_CASE_STR(WM_PENMISCINFO);
        //TOGL_INNER_CASE_STR(WM_SKB);
        //TOGL_INNER_CASE_STR(WM_HEDITCTL);
        //TOGL_INNER_CASE_STR(WM_PENCTL);
        //TOGL_INNER_CASE_STR(WM_PENMISC);
        //TOGL_INNER_CASE_STR(WM_CTLINIT);
        //TOGL_INNER_CASE_STR(WM_PENEVENT);
        TOGL_INNER_CASE_STR(WM_PENWINLAST);
        //TOGL_INNER_CASE_STR(DDM_SETFMT);
        TOGL_INNER_CASE_STR(DM_GETDEFID);
        //TOGL_INNER_CASE_STR(NIN_SELECT);
        //TOGL_INNER_CASE_STR(TBM_GETPOS);
        //TOGL_INNER_CASE_STR(WM_PSD_PAGESETUPDLG);
        //TOGL_INNER_CASE_STR(WM_USER);
        //TOGL_INNER_CASE_STR(CBEM_INSERTITEMA);
        //TOGL_INNER_CASE_STR(DDM_DRAW);
        TOGL_INNER_CASE_STR(DM_SETDEFID);
        //TOGL_INNER_CASE_STR(HKM_SETHOTKEY);
        //TOGL_INNER_CASE_STR(PBM_SETRANGE);
        //TOGL_INNER_CASE_STR(RB_INSERTBANDA);
        //TOGL_INNER_CASE_STR(SB_SETTEXTA);
        //TOGL_INNER_CASE_STR(TB_ENABLEBUTTON);
        //TOGL_INNER_CASE_STR(TBM_GETRANGEMIN);
        //TOGL_INNER_CASE_STR(TTM_ACTIVATE);
        //TOGL_INNER_CASE_STR(WM_CHOOSEFONT_GETLOGFONT);
        //TOGL_INNER_CASE_STR(WM_PSD_FULLPAGERECT);
        //TOGL_INNER_CASE_STR(CBEM_SETIMAGELIST);
        //TOGL_INNER_CASE_STR(DDM_CLOSE);
        TOGL_INNER_CASE_STR(DM_REPOSITION);
        //TOGL_INNER_CASE_STR(HKM_GETHOTKEY);
        //TOGL_INNER_CASE_STR(PBM_SETPOS);
        //TOGL_INNER_CASE_STR(RB_DELETEBAND);
        //TOGL_INNER_CASE_STR(SB_GETTEXTA);
        //TOGL_INNER_CASE_STR(TB_CHECKBUTTON);
        //TOGL_INNER_CASE_STR(TBM_GETRANGEMAX);
        //TOGL_INNER_CASE_STR(WM_PSD_MINMARGINRECT);
        //TOGL_INNER_CASE_STR(CBEM_GETIMAGELIST);
        //TOGL_INNER_CASE_STR(DDM_BEGIN);
        //TOGL_INNER_CASE_STR(HKM_SETRULES);
        //TOGL_INNER_CASE_STR(PBM_DELTAPOS);
        //TOGL_INNER_CASE_STR(RB_GETBARINFO);
        //TOGL_INNER_CASE_STR(SB_GETTEXTLENGTHA);
        //TOGL_INNER_CASE_STR(TBM_GETTIC);
        //TOGL_INNER_CASE_STR(TB_PRESSBUTTON);
        //TOGL_INNER_CASE_STR(TTM_SETDELAYTIME);
        //TOGL_INNER_CASE_STR(WM_PSD_MARGINRECT);
        //TOGL_INNER_CASE_STR(CBEM_GETITEMA);
        //TOGL_INNER_CASE_STR(DDM_END);
        //TOGL_INNER_CASE_STR(PBM_SETSTEP);
        //TOGL_INNER_CASE_STR(RB_SETBARINFO);
        //TOGL_INNER_CASE_STR(SB_SETPARTS);
        //TOGL_INNER_CASE_STR(TB_HIDEBUTTON);
        //TOGL_INNER_CASE_STR(TBM_SETTIC);
        //TOGL_INNER_CASE_STR(TTM_ADDTOOLA);
        //TOGL_INNER_CASE_STR(WM_PSD_GREEKTEXTRECT);
        //TOGL_INNER_CASE_STR(CBEM_SETITEMA);
        //TOGL_INNER_CASE_STR(PBM_STEPIT);
        //TOGL_INNER_CASE_STR(TB_INDETERMINATE);
        //TOGL_INNER_CASE_STR(TBM_SETPOS);
        //TOGL_INNER_CASE_STR(TTM_DELTOOLA);
        //TOGL_INNER_CASE_STR(WM_PSD_ENVSTAMPRECT);
        //TOGL_INNER_CASE_STR(CBEM_GETCOMBOCONTROL);
        //TOGL_INNER_CASE_STR(PBM_SETRANGE32);
        //TOGL_INNER_CASE_STR(RB_SETBANDINFOA);
        //TOGL_INNER_CASE_STR(SB_GETPARTS);
        //TOGL_INNER_CASE_STR(TB_MARKBUTTON);
        //TOGL_INNER_CASE_STR(TBM_SETRANGE);
        //TOGL_INNER_CASE_STR(TTM_NEWTOOLRECTA);
        //TOGL_INNER_CASE_STR(WM_PSD_YAFULLPAGERECT);
        //TOGL_INNER_CASE_STR(CBEM_GETEDITCONTROL);
        //TOGL_INNER_CASE_STR(PBM_GETRANGE);
        //TOGL_INNER_CASE_STR(RB_SETPARENT);
        //TOGL_INNER_CASE_STR(SB_GETBORDERS);
        //TOGL_INNER_CASE_STR(TBM_SETRANGEMIN);
        //TOGL_INNER_CASE_STR(TTM_RELAYEVENT);
        //TOGL_INNER_CASE_STR(CBEM_SETEXSTYLE);
        //TOGL_INNER_CASE_STR(PBM_GETPOS);
        //TOGL_INNER_CASE_STR(RB_HITTEST);
        //TOGL_INNER_CASE_STR(SB_SETMINHEIGHT);
        //TOGL_INNER_CASE_STR(TBM_SETRANGEMAX);
        //TOGL_INNER_CASE_STR(TTM_GETTOOLINFOA);
        //TOGL_INNER_CASE_STR(CBEM_GETEXSTYLE);
        //TOGL_INNER_CASE_STR(CBEM_GETEXTENDEDSTYLE);
        //TOGL_INNER_CASE_STR(PBM_SETBARCOLOR);
        //TOGL_INNER_CASE_STR(RB_GETRECT);
        //TOGL_INNER_CASE_STR(SB_SIMPLE);
        //TOGL_INNER_CASE_STR(TB_ISBUTTONENABLED);
        //TOGL_INNER_CASE_STR(TBM_CLEARTICS);
        //TOGL_INNER_CASE_STR(TTM_SETTOOLINFOA);
        //TOGL_INNER_CASE_STR(CBEM_HASEDITCHANGED);
        //TOGL_INNER_CASE_STR(RB_INSERTBANDW);
        //TOGL_INNER_CASE_STR(SB_GETRECT);
        //TOGL_INNER_CASE_STR(TB_ISBUTTONCHECKED);
        //TOGL_INNER_CASE_STR(TBM_SETSEL);
        //TOGL_INNER_CASE_STR(TTM_HITTESTA);
        //TOGL_INNER_CASE_STR(WIZ_QUERYNUMPAGES);
        //TOGL_INNER_CASE_STR(CBEM_INSERTITEMW);
        //TOGL_INNER_CASE_STR(RB_SETBANDINFOW);
        //TOGL_INNER_CASE_STR(SB_SETTEXTW);
        //TOGL_INNER_CASE_STR(TB_ISBUTTONPRESSED);
        //TOGL_INNER_CASE_STR(TBM_SETSELSTART);
        //TOGL_INNER_CASE_STR(TTM_GETTEXTA);
        //TOGL_INNER_CASE_STR(WIZ_NEXT);
        //TOGL_INNER_CASE_STR(CBEM_SETITEMW);
        //TOGL_INNER_CASE_STR(RB_GETBANDCOUNT);
        //TOGL_INNER_CASE_STR(SB_GETTEXTLENGTHW);
        //TOGL_INNER_CASE_STR(TB_ISBUTTONHIDDEN);
        //TOGL_INNER_CASE_STR(TBM_SETSELEND);
        //TOGL_INNER_CASE_STR(TTM_UPDATETIPTEXTA);
        //TOGL_INNER_CASE_STR(WIZ_PREV);
        //TOGL_INNER_CASE_STR(CBEM_GETITEMW);
        //TOGL_INNER_CASE_STR(RB_GETROWCOUNT);
        //TOGL_INNER_CASE_STR(SB_GETTEXTW);
        //TOGL_INNER_CASE_STR(TB_ISBUTTONINDETERMINATE);
        //TOGL_INNER_CASE_STR(TTM_GETTOOLCOUNT);
        //TOGL_INNER_CASE_STR(CBEM_SETEXTENDEDSTYLE);
        //TOGL_INNER_CASE_STR(RB_GETROWHEIGHT);
        //TOGL_INNER_CASE_STR(SB_ISSIMPLE);
        //TOGL_INNER_CASE_STR(TB_ISBUTTONHIGHLIGHTED);
        //TOGL_INNER_CASE_STR(TBM_GETPTICS);
        //TOGL_INNER_CASE_STR(TTM_ENUMTOOLSA);
        //TOGL_INNER_CASE_STR(SB_SETICON);
        //TOGL_INNER_CASE_STR(TBM_GETTICPOS);
        //TOGL_INNER_CASE_STR(TTM_GETCURRENTTOOLA);
        //TOGL_INNER_CASE_STR(RB_IDTOINDEX);
        //TOGL_INNER_CASE_STR(SB_SETTIPTEXTA);
        //TOGL_INNER_CASE_STR(TBM_GETNUMTICS);
        //TOGL_INNER_CASE_STR(TTM_WINDOWFROMPOINT);
        //TOGL_INNER_CASE_STR(RB_GETTOOLTIPS);
        //TOGL_INNER_CASE_STR(SB_SETTIPTEXTW);
        //TOGL_INNER_CASE_STR(TBM_GETSELSTART);
        //TOGL_INNER_CASE_STR(TB_SETSTATE);
        //TOGL_INNER_CASE_STR(TTM_TRACKACTIVATE);
        //TOGL_INNER_CASE_STR(RB_SETTOOLTIPS);
        //TOGL_INNER_CASE_STR(SB_GETTIPTEXTA);
        //TOGL_INNER_CASE_STR(TB_GETSTATE);
        //TOGL_INNER_CASE_STR(TBM_GETSELEND);
        //TOGL_INNER_CASE_STR(TTM_TRACKPOSITION);
        //TOGL_INNER_CASE_STR(RB_SETBKCOLOR);
        //TOGL_INNER_CASE_STR(SB_GETTIPTEXTW);
        //TOGL_INNER_CASE_STR(TB_ADDBITMAP);
        //TOGL_INNER_CASE_STR(TBM_CLEARSEL);
        //TOGL_INNER_CASE_STR(TTM_SETTIPBKCOLOR);
        //TOGL_INNER_CASE_STR(RB_GETBKCOLOR);
        //TOGL_INNER_CASE_STR(SB_GETICON);
        //TOGL_INNER_CASE_STR(TB_ADDBUTTONSA);
        //TOGL_INNER_CASE_STR(TBM_SETTICFREQ);
        //TOGL_INNER_CASE_STR(TTM_SETTIPTEXTCOLOR);
        //TOGL_INNER_CASE_STR(RB_SETTEXTCOLOR);
        //TOGL_INNER_CASE_STR(TB_INSERTBUTTONA);
        //TOGL_INNER_CASE_STR(TBM_SETPAGESIZE);
        //TOGL_INNER_CASE_STR(TTM_GETDELAYTIME);
        //TOGL_INNER_CASE_STR(RB_GETTEXTCOLOR);
        //TOGL_INNER_CASE_STR(TB_DELETEBUTTON);
        //TOGL_INNER_CASE_STR(TBM_GETPAGESIZE);
        //TOGL_INNER_CASE_STR(TTM_GETTIPBKCOLOR);
        //TOGL_INNER_CASE_STR(RB_SIZETORECT);
        //TOGL_INNER_CASE_STR(TB_GETBUTTON);
        //TOGL_INNER_CASE_STR(TBM_SETLINESIZE);
        //TOGL_INNER_CASE_STR(TTM_GETTIPTEXTCOLOR);
        //TOGL_INNER_CASE_STR(RB_BEGINDRAG);
        //TOGL_INNER_CASE_STR(TB_BUTTONCOUNT);
        //TOGL_INNER_CASE_STR(TBM_GETLINESIZE);
        //TOGL_INNER_CASE_STR(TTM_SETMAXTIPWIDTH);
        //TOGL_INNER_CASE_STR(RB_ENDDRAG);
        //TOGL_INNER_CASE_STR(TB_COMMANDTOINDEX);
        //TOGL_INNER_CASE_STR(TBM_GETTHUMBRECT);
        //TOGL_INNER_CASE_STR(TTM_GETMAXTIPWIDTH);
        //TOGL_INNER_CASE_STR(RB_DRAGMOVE);
        //TOGL_INNER_CASE_STR(TBM_GETCHANNELRECT);
        //TOGL_INNER_CASE_STR(TB_SAVERESTOREA);
        //TOGL_INNER_CASE_STR(TTM_SETMARGIN);
        //TOGL_INNER_CASE_STR(RB_GETBARHEIGHT);
        //TOGL_INNER_CASE_STR(TB_CUSTOMIZE);
        //TOGL_INNER_CASE_STR(TBM_SETTHUMBLENGTH);
        //TOGL_INNER_CASE_STR(TTM_GETMARGIN);
        //TOGL_INNER_CASE_STR(RB_GETBANDINFOW);
        //TOGL_INNER_CASE_STR(TB_ADDSTRINGA);
        //TOGL_INNER_CASE_STR(TBM_GETTHUMBLENGTH);
        //TOGL_INNER_CASE_STR(TTM_POP);
        //TOGL_INNER_CASE_STR(RB_GETBANDINFOA);
        //TOGL_INNER_CASE_STR(TB_GETITEMRECT);
        //TOGL_INNER_CASE_STR(TBM_SETTOOLTIPS);
        //TOGL_INNER_CASE_STR(TTM_UPDATE);
        //TOGL_INNER_CASE_STR(RB_MINIMIZEBAND);
        //TOGL_INNER_CASE_STR(TB_BUTTONSTRUCTSIZE);
        //TOGL_INNER_CASE_STR(TBM_GETTOOLTIPS);
        //TOGL_INNER_CASE_STR(TTM_GETBUBBLESIZE);
        //TOGL_INNER_CASE_STR(RB_MAXIMIZEBAND);
        //TOGL_INNER_CASE_STR(TBM_SETTIPSIDE);
        //TOGL_INNER_CASE_STR(TB_SETBUTTONSIZE);
        //TOGL_INNER_CASE_STR(TTM_ADJUSTRECT);
        //TOGL_INNER_CASE_STR(TBM_SETBUDDY);
        //TOGL_INNER_CASE_STR(TB_SETBITMAPSIZE);
        //TOGL_INNER_CASE_STR(TTM_SETTITLEA);
        //TOGL_INNER_CASE_STR(MSG_FTS_JUMP_VA);
        //TOGL_INNER_CASE_STR(TB_AUTOSIZE);
        //TOGL_INNER_CASE_STR(TBM_GETBUDDY);
        //TOGL_INNER_CASE_STR(TTM_SETTITLEW);
        //TOGL_INNER_CASE_STR(RB_GETBANDBORDERS);
        //TOGL_INNER_CASE_STR(MSG_FTS_JUMP_QWORD);
        //TOGL_INNER_CASE_STR(RB_SHOWBAND);
        //TOGL_INNER_CASE_STR(TB_GETTOOLTIPS);
        //TOGL_INNER_CASE_STR(MSG_REINDEX_REQUEST);
        //TOGL_INNER_CASE_STR(TB_SETTOOLTIPS);
        //TOGL_INNER_CASE_STR(MSG_FTS_WHERE_IS_IT);
        //TOGL_INNER_CASE_STR(RB_SETPALETTE);
        //TOGL_INNER_CASE_STR(TB_SETPARENT);
        //TOGL_INNER_CASE_STR(RB_GETPALETTE);
        //TOGL_INNER_CASE_STR(RB_MOVEBAND);
        //TOGL_INNER_CASE_STR(TB_SETROWS);
        //TOGL_INNER_CASE_STR(TB_GETROWS);
        //TOGL_INNER_CASE_STR(TB_GETBITMAPFLAGS);
        //TOGL_INNER_CASE_STR(TB_SETCMDID);
        //TOGL_INNER_CASE_STR(RB_PUSHCHEVRON);
        //TOGL_INNER_CASE_STR(TB_CHANGEBITMAP);
        //TOGL_INNER_CASE_STR(TB_GETBITMAP);
        //TOGL_INNER_CASE_STR(MSG_GET_DEFFONT);
        //TOGL_INNER_CASE_STR(TB_GETBUTTONTEXTA);
        //TOGL_INNER_CASE_STR(TB_REPLACEBITMAP);
        //TOGL_INNER_CASE_STR(TB_SETINDENT);
        //TOGL_INNER_CASE_STR(TB_SETIMAGELIST);
        //TOGL_INNER_CASE_STR(TB_GETIMAGELIST);
        //TOGL_INNER_CASE_STR(TB_LOADIMAGES);
        //TOGL_INNER_CASE_STR(EM_CANPASTE);
        //TOGL_INNER_CASE_STR(TTM_ADDTOOLW);
        //TOGL_INNER_CASE_STR(EM_DISPLAYBAND);
        //TOGL_INNER_CASE_STR(TB_GETRECT);
        //TOGL_INNER_CASE_STR(TTM_DELTOOLW);
        //TOGL_INNER_CASE_STR(EM_EXGETSEL);
        //TOGL_INNER_CASE_STR(TB_SETHOTIMAGELIST);
        //TOGL_INNER_CASE_STR(TTM_NEWTOOLRECTW);
        //TOGL_INNER_CASE_STR(EM_EXLIMITTEXT);
        //TOGL_INNER_CASE_STR(TB_GETHOTIMAGELIST);
        //TOGL_INNER_CASE_STR(TTM_GETTOOLINFOW);
        //TOGL_INNER_CASE_STR(EM_EXLINEFROMCHAR);
        //TOGL_INNER_CASE_STR(TB_SETDISABLEDIMAGELIST);
        //TOGL_INNER_CASE_STR(TTM_SETTOOLINFOW);
        //TOGL_INNER_CASE_STR(EM_EXSETSEL);
        //TOGL_INNER_CASE_STR(TB_GETDISABLEDIMAGELIST);
        //TOGL_INNER_CASE_STR(TTM_HITTESTW);
        //TOGL_INNER_CASE_STR(EM_FINDTEXT);
        //TOGL_INNER_CASE_STR(TB_SETSTYLE);
        //TOGL_INNER_CASE_STR(TTM_GETTEXTW);
        //TOGL_INNER_CASE_STR(EM_FORMATRANGE);
        //TOGL_INNER_CASE_STR(TB_GETSTYLE);
        //TOGL_INNER_CASE_STR(TTM_UPDATETIPTEXTW);
        //TOGL_INNER_CASE_STR(EM_GETCHARFORMAT);
        //TOGL_INNER_CASE_STR(TB_GETBUTTONSIZE);
        //TOGL_INNER_CASE_STR(TTM_ENUMTOOLSW);
        //TOGL_INNER_CASE_STR(EM_GETEVENTMASK);
        //TOGL_INNER_CASE_STR(TB_SETBUTTONWIDTH);
        //TOGL_INNER_CASE_STR(TTM_GETCURRENTTOOLW);
        //TOGL_INNER_CASE_STR(EM_GETOLEINTERFACE);
        //TOGL_INNER_CASE_STR(TB_SETMAXTEXTROWS);
        //TOGL_INNER_CASE_STR(EM_GETPARAFORMAT);
        //TOGL_INNER_CASE_STR(TB_GETTEXTROWS);
        //TOGL_INNER_CASE_STR(EM_GETSELTEXT);
        //TOGL_INNER_CASE_STR(TB_GETOBJECT);
        //TOGL_INNER_CASE_STR(EM_HIDESELECTION);
        //TOGL_INNER_CASE_STR(TB_GETBUTTONINFOW);
        //TOGL_INNER_CASE_STR(EM_PASTESPECIAL);
        //TOGL_INNER_CASE_STR(TB_SETBUTTONINFOW);
        //TOGL_INNER_CASE_STR(EM_REQUESTRESIZE);
        //TOGL_INNER_CASE_STR(TB_GETBUTTONINFOA);
        //TOGL_INNER_CASE_STR(EM_SELECTIONTYPE);
        //TOGL_INNER_CASE_STR(TB_SETBUTTONINFOA);
        //TOGL_INNER_CASE_STR(EM_SETBKGNDCOLOR);
        //TOGL_INNER_CASE_STR(TB_INSERTBUTTONW);
        //TOGL_INNER_CASE_STR(EM_SETCHARFORMAT);
        //TOGL_INNER_CASE_STR(TB_ADDBUTTONSW);
        //TOGL_INNER_CASE_STR(EM_SETEVENTMASK);
        //TOGL_INNER_CASE_STR(TB_HITTEST);
        //TOGL_INNER_CASE_STR(EM_SETOLECALLBACK);
        //TOGL_INNER_CASE_STR(TB_SETDRAWTEXTFLAGS);
        //TOGL_INNER_CASE_STR(EM_SETPARAFORMAT);
        //TOGL_INNER_CASE_STR(TB_GETHOTITEM);
        //TOGL_INNER_CASE_STR(EM_SETTARGETDEVICE);
        //TOGL_INNER_CASE_STR(TB_SETHOTITEM);
        //TOGL_INNER_CASE_STR(EM_STREAMIN);
        //TOGL_INNER_CASE_STR(TB_SETANCHORHIGHLIGHT);
        //TOGL_INNER_CASE_STR(EM_STREAMOUT);
        //TOGL_INNER_CASE_STR(TB_GETANCHORHIGHLIGHT);
        //TOGL_INNER_CASE_STR(EM_GETTEXTRANGE);
        //TOGL_INNER_CASE_STR(TB_GETBUTTONTEXTW);
        //TOGL_INNER_CASE_STR(EM_FINDWORDBREAK);
        //TOGL_INNER_CASE_STR(TB_SAVERESTOREW);
        //TOGL_INNER_CASE_STR(EM_SETOPTIONS);
        //TOGL_INNER_CASE_STR(TB_ADDSTRINGW);
        //TOGL_INNER_CASE_STR(EM_GETOPTIONS);
        //TOGL_INNER_CASE_STR(TB_MAPACCELERATORA);
        //TOGL_INNER_CASE_STR(EM_FINDTEXTEX);
        //TOGL_INNER_CASE_STR(TB_GETINSERTMARK);
        //TOGL_INNER_CASE_STR(EM_GETWORDBREAKPROCEX);
        //TOGL_INNER_CASE_STR(TB_SETINSERTMARK);
        //TOGL_INNER_CASE_STR(EM_SETWORDBREAKPROCEX);
        //TOGL_INNER_CASE_STR(TB_INSERTMARKHITTEST);
        //TOGL_INNER_CASE_STR(EM_SETUNDOLIMIT);
        //TOGL_INNER_CASE_STR(TB_MOVEBUTTON);
        //TOGL_INNER_CASE_STR(TB_GETMAXSIZE);
        //TOGL_INNER_CASE_STR(EM_REDO);
        //TOGL_INNER_CASE_STR(TB_SETEXTENDEDSTYLE);
        //TOGL_INNER_CASE_STR(EM_CANREDO);
        //TOGL_INNER_CASE_STR(TB_GETEXTENDEDSTYLE);
        //TOGL_INNER_CASE_STR(EM_GETUNDONAME);
        //TOGL_INNER_CASE_STR(TB_GETPADDING);
        //TOGL_INNER_CASE_STR(EM_GETREDONAME);
        //TOGL_INNER_CASE_STR(TB_SETPADDING);
        //TOGL_INNER_CASE_STR(EM_STOPGROUPTYPING);
        //TOGL_INNER_CASE_STR(TB_SETINSERTMARKCOLOR);
        //TOGL_INNER_CASE_STR(EM_SETTEXTMODE);
        //TOGL_INNER_CASE_STR(TB_GETINSERTMARKCOLOR);
        //TOGL_INNER_CASE_STR(EM_GETTEXTMODE);
        //TOGL_INNER_CASE_STR(TB_MAPACCELERATORW);
        //TOGL_INNER_CASE_STR(EM_AUTOURLDETECT);
        //TOGL_INNER_CASE_STR(TB_GETSTRINGW);
        //TOGL_INNER_CASE_STR(EM_GETAUTOURLDETECT);
        //TOGL_INNER_CASE_STR(TB_GETSTRINGA);
        //TOGL_INNER_CASE_STR(EM_SETPALETTE);
        //TOGL_INNER_CASE_STR(EM_GETTEXTEX);
        //TOGL_INNER_CASE_STR(EM_GETTEXTLENGTHEX);
        //TOGL_INNER_CASE_STR(EM_SHOWSCROLLBAR);
        //TOGL_INNER_CASE_STR(EM_SETTEXTEX);
        //TOGL_INNER_CASE_STR(TAPI_REPLY);
        //TOGL_INNER_CASE_STR(ACM_OPENA);
        //TOGL_INNER_CASE_STR(BFFM_SETSTATUSTEXTA);
        //TOGL_INNER_CASE_STR(CDM_FIRST);
        //TOGL_INNER_CASE_STR(CDM_GETSPEC);
        //TOGL_INNER_CASE_STR(EM_SETPUNCTUATION);
        //TOGL_INNER_CASE_STR(IPM_CLEARADDRESS);
        //TOGL_INNER_CASE_STR(WM_CAP_UNICODE_START);
        //TOGL_INNER_CASE_STR(ACM_PLAY);
        //TOGL_INNER_CASE_STR(BFFM_ENABLEOK);
        //TOGL_INNER_CASE_STR(CDM_GETFILEPATH);
        //TOGL_INNER_CASE_STR(EM_GETPUNCTUATION);
        //TOGL_INNER_CASE_STR(IPM_SETADDRESS);
        //TOGL_INNER_CASE_STR(PSM_SETCURSEL);
        //TOGL_INNER_CASE_STR(UDM_SETRANGE);
        //TOGL_INNER_CASE_STR(WM_CHOOSEFONT_SETLOGFONT);
        //TOGL_INNER_CASE_STR(ACM_STOP);
        //TOGL_INNER_CASE_STR(BFFM_SETSELECTIONA);
        //TOGL_INNER_CASE_STR(CDM_GETFOLDERPATH);
        //TOGL_INNER_CASE_STR(EM_SETWORDWRAPMODE);
        //TOGL_INNER_CASE_STR(IPM_GETADDRESS);
        //TOGL_INNER_CASE_STR(PSM_REMOVEPAGE);
        //TOGL_INNER_CASE_STR(UDM_GETRANGE);
        //TOGL_INNER_CASE_STR(WM_CAP_SET_CALLBACK_ERRORW);
        //TOGL_INNER_CASE_STR(WM_CHOOSEFONT_SETFLAGS);
        //TOGL_INNER_CASE_STR(ACM_OPENW);
        //TOGL_INNER_CASE_STR(BFFM_SETSELECTIONW);
        //TOGL_INNER_CASE_STR(CDM_GETFOLDERIDLIST);
        //TOGL_INNER_CASE_STR(EM_GETWORDWRAPMODE);
        //TOGL_INNER_CASE_STR(IPM_SETRANGE);
        //TOGL_INNER_CASE_STR(PSM_ADDPAGE);
        //TOGL_INNER_CASE_STR(UDM_SETPOS);
        //TOGL_INNER_CASE_STR(WM_CAP_SET_CALLBACK_STATUSW);
        //TOGL_INNER_CASE_STR(BFFM_SETSTATUSTEXTW);
        //TOGL_INNER_CASE_STR(CDM_SETCONTROLTEXT);
        //TOGL_INNER_CASE_STR(EM_SETIMECOLOR);
        //TOGL_INNER_CASE_STR(IPM_SETFOCUS);
        //TOGL_INNER_CASE_STR(PSM_CHANGED);
        //TOGL_INNER_CASE_STR(UDM_GETPOS);
        //TOGL_INNER_CASE_STR(CDM_HIDECONTROL);
        //TOGL_INNER_CASE_STR(EM_GETIMECOLOR);
        //TOGL_INNER_CASE_STR(IPM_ISBLANK);
        //TOGL_INNER_CASE_STR(PSM_RESTARTWINDOWS);
        //TOGL_INNER_CASE_STR(UDM_SETBUDDY);
        //TOGL_INNER_CASE_STR(CDM_SETDEFEXT);
        //TOGL_INNER_CASE_STR(EM_SETIMEOPTIONS);
        //TOGL_INNER_CASE_STR(PSM_REBOOTSYSTEM);
        //TOGL_INNER_CASE_STR(UDM_GETBUDDY);
        //TOGL_INNER_CASE_STR(EM_GETIMEOPTIONS);
        //TOGL_INNER_CASE_STR(PSM_CANCELTOCLOSE);
        //TOGL_INNER_CASE_STR(UDM_SETACCEL);
        //TOGL_INNER_CASE_STR(EM_CONVPOSITION);
        //TOGL_INNER_CASE_STR(EM_CONVPOSITION);
        //TOGL_INNER_CASE_STR(PSM_QUERYSIBLINGS);
        //TOGL_INNER_CASE_STR(UDM_GETACCEL);
        //TOGL_INNER_CASE_STR(MCIWNDM_GETZOOM);
        //TOGL_INNER_CASE_STR(PSM_UNCHANGED);
        //TOGL_INNER_CASE_STR(UDM_SETBASE);
        //TOGL_INNER_CASE_STR(PSM_APPLY);
        //TOGL_INNER_CASE_STR(UDM_GETBASE);
        //TOGL_INNER_CASE_STR(PSM_SETTITLEA);
        //TOGL_INNER_CASE_STR(UDM_SETRANGE32);
        //TOGL_INNER_CASE_STR(PSM_SETWIZBUTTONS);
        //TOGL_INNER_CASE_STR(UDM_GETRANGE32);
        //TOGL_INNER_CASE_STR(WM_CAP_DRIVER_GET_NAMEW);
        //TOGL_INNER_CASE_STR(PSM_PRESSBUTTON);
        //TOGL_INNER_CASE_STR(UDM_SETPOS32);
        //TOGL_INNER_CASE_STR(WM_CAP_DRIVER_GET_VERSIONW);
        //TOGL_INNER_CASE_STR(PSM_SETCURSELID);
        //TOGL_INNER_CASE_STR(UDM_GETPOS32);
        //TOGL_INNER_CASE_STR(PSM_SETFINISHTEXTA);
        //TOGL_INNER_CASE_STR(PSM_GETTABCONTROL);
        //TOGL_INNER_CASE_STR(PSM_ISDIALOGMESSAGE);
        //TOGL_INNER_CASE_STR(MCIWNDM_REALIZE);
        //TOGL_INNER_CASE_STR(PSM_GETCURRENTPAGEHWND);
        //TOGL_INNER_CASE_STR(MCIWNDM_SETTIMEFORMATA);
        //TOGL_INNER_CASE_STR(PSM_INSERTPAGE);
        //TOGL_INNER_CASE_STR(EM_SETLANGOPTIONS);
        //TOGL_INNER_CASE_STR(MCIWNDM_GETTIMEFORMATA);
        //TOGL_INNER_CASE_STR(PSM_SETTITLEW);
        //TOGL_INNER_CASE_STR(WM_CAP_FILE_SET_CAPTURE_FILEW);
        //TOGL_INNER_CASE_STR(EM_GETLANGOPTIONS);
        //TOGL_INNER_CASE_STR(MCIWNDM_VALIDATEMEDIA);
        //TOGL_INNER_CASE_STR(PSM_SETFINISHTEXTW);
        //TOGL_INNER_CASE_STR(WM_CAP_FILE_GET_CAPTURE_FILEW);
        //TOGL_INNER_CASE_STR(EM_GETIMECOMPMODE);
        //TOGL_INNER_CASE_STR(EM_FINDTEXTW);
        //TOGL_INNER_CASE_STR(MCIWNDM_PLAYTO);
        //TOGL_INNER_CASE_STR(WM_CAP_FILE_SAVEASW);
        //TOGL_INNER_CASE_STR(EM_FINDTEXTEXW);
        //TOGL_INNER_CASE_STR(MCIWNDM_GETFILENAMEA);
        //TOGL_INNER_CASE_STR(EM_RECONVERSION);
        //TOGL_INNER_CASE_STR(MCIWNDM_GETDEVICEA);
        //TOGL_INNER_CASE_STR(PSM_SETHEADERTITLEA);
        //TOGL_INNER_CASE_STR(WM_CAP_FILE_SAVEDIBW);
        //TOGL_INNER_CASE_STR(EM_SETIMEMODEBIAS);
        //TOGL_INNER_CASE_STR(MCIWNDM_GETPALETTE);
        //TOGL_INNER_CASE_STR(PSM_SETHEADERTITLEW);
        //TOGL_INNER_CASE_STR(EM_GETIMEMODEBIAS);
        //TOGL_INNER_CASE_STR(MCIWNDM_SETPALETTE);
        //TOGL_INNER_CASE_STR(PSM_SETHEADERSUBTITLEA);
        //TOGL_INNER_CASE_STR(MCIWNDM_GETERRORA);
        //TOGL_INNER_CASE_STR(PSM_SETHEADERSUBTITLEW);
        //TOGL_INNER_CASE_STR(PSM_HWNDTOINDEX);
        //TOGL_INNER_CASE_STR(PSM_INDEXTOHWND);
        //TOGL_INNER_CASE_STR(MCIWNDM_SETINACTIVETIMER);
        //TOGL_INNER_CASE_STR(PSM_PAGETOINDEX);
        //TOGL_INNER_CASE_STR(PSM_INDEXTOPAGE);
        //TOGL_INNER_CASE_STR(DL_BEGINDRAG);
        //TOGL_INNER_CASE_STR(MCIWNDM_GETINACTIVETIMER);
        //TOGL_INNER_CASE_STR(PSM_IDTOINDEX);
        //TOGL_INNER_CASE_STR(DL_DRAGGING);
        //TOGL_INNER_CASE_STR(PSM_INDEXTOID);
        //TOGL_INNER_CASE_STR(DL_DROPPED);
        //TOGL_INNER_CASE_STR(PSM_GETRESULT);
        //TOGL_INNER_CASE_STR(DL_CANCELDRAG);
        //TOGL_INNER_CASE_STR(PSM_RECALCPAGESIZES);
        //TOGL_INNER_CASE_STR(MCIWNDM_GET_SOURCE);
        //TOGL_INNER_CASE_STR(MCIWNDM_PUT_SOURCE);
        //TOGL_INNER_CASE_STR(MCIWNDM_GET_DEST);
        //TOGL_INNER_CASE_STR(MCIWNDM_PUT_DEST);
        //TOGL_INNER_CASE_STR(MCIWNDM_CAN_PLAY);
        //TOGL_INNER_CASE_STR(MCIWNDM_CAN_WINDOW);
        //TOGL_INNER_CASE_STR(MCIWNDM_CAN_RECORD);
        //TOGL_INNER_CASE_STR(MCIWNDM_CAN_SAVE);
        //TOGL_INNER_CASE_STR(MCIWNDM_CAN_EJECT);
        //TOGL_INNER_CASE_STR(MCIWNDM_CAN_CONFIG);
        //TOGL_INNER_CASE_STR(IE_GETINK);
        //TOGL_INNER_CASE_STR(IE_MSGFIRST);
        //TOGL_INNER_CASE_STR(MCIWNDM_PALETTEKICK);
        //TOGL_INNER_CASE_STR(IE_SETINK);
        //TOGL_INNER_CASE_STR(IE_GETPENTIP);
        //TOGL_INNER_CASE_STR(IE_SETPENTIP);
        //TOGL_INNER_CASE_STR(IE_GETERASERTIP);
        //TOGL_INNER_CASE_STR(IE_SETERASERTIP);
        //TOGL_INNER_CASE_STR(IE_GETBKGND);
        //TOGL_INNER_CASE_STR(IE_SETBKGND);
        //TOGL_INNER_CASE_STR(IE_GETGRIDORIGIN);
        //TOGL_INNER_CASE_STR(IE_SETGRIDORIGIN);
        //TOGL_INNER_CASE_STR(IE_GETGRIDPEN);
        //TOGL_INNER_CASE_STR(IE_SETGRIDPEN);
        //TOGL_INNER_CASE_STR(IE_GETGRIDSIZE);
        //TOGL_INNER_CASE_STR(IE_SETGRIDSIZE);
        //TOGL_INNER_CASE_STR(IE_GETMODE);
        //TOGL_INNER_CASE_STR(IE_SETMODE);
        //TOGL_INNER_CASE_STR(IE_GETINKRECT);
        //TOGL_INNER_CASE_STR(WM_CAP_SET_MCI_DEVICEW);
        //TOGL_INNER_CASE_STR(WM_CAP_GET_MCI_DEVICEW);
        //TOGL_INNER_CASE_STR(WM_CAP_PAL_OPENW);
        //TOGL_INNER_CASE_STR(WM_CAP_PAL_SAVEW);
        //TOGL_INNER_CASE_STR(IE_GETAPPDATA);
        //TOGL_INNER_CASE_STR(IE_SETAPPDATA);
        //TOGL_INNER_CASE_STR(IE_GETDRAWOPTS);
        //TOGL_INNER_CASE_STR(IE_SETDRAWOPTS);
        //TOGL_INNER_CASE_STR(IE_GETFORMAT);
        //TOGL_INNER_CASE_STR(IE_SETFORMAT);
        //TOGL_INNER_CASE_STR(IE_GETINKINPUT);
        //TOGL_INNER_CASE_STR(IE_SETINKINPUT);
        //TOGL_INNER_CASE_STR(IE_GETNOTIFY);
        //TOGL_INNER_CASE_STR(IE_SETNOTIFY);
        //TOGL_INNER_CASE_STR(IE_GETRECOG);
        //TOGL_INNER_CASE_STR(IE_SETRECOG);
        //TOGL_INNER_CASE_STR(IE_GETSECURITY);
        //TOGL_INNER_CASE_STR(IE_SETSECURITY);
        //TOGL_INNER_CASE_STR(IE_GETSEL);
        //TOGL_INNER_CASE_STR(IE_SETSEL);
        //TOGL_INNER_CASE_STR(CDM_LAST);
        //TOGL_INNER_CASE_STR(EM_SETBIDIOPTIONS);
        //TOGL_INNER_CASE_STR(IE_DOCOMMAND);
        //TOGL_INNER_CASE_STR(MCIWNDM_NOTIFYMODE);
        //TOGL_INNER_CASE_STR(EM_GETBIDIOPTIONS);
        //TOGL_INNER_CASE_STR(IE_GETCOMMAND);
        //TOGL_INNER_CASE_STR(EM_SETTYPOGRAPHYOPTIONS);
        //TOGL_INNER_CASE_STR(IE_GETCOUNT);
        //TOGL_INNER_CASE_STR(EM_GETTYPOGRAPHYOPTIONS);
        //TOGL_INNER_CASE_STR(IE_GETGESTURE);
        //TOGL_INNER_CASE_STR(MCIWNDM_NOTIFYMEDIA);
        //TOGL_INNER_CASE_STR(EM_SETEDITSTYLE);
        //TOGL_INNER_CASE_STR(IE_GETMENU);
        //TOGL_INNER_CASE_STR(EM_GETEDITSTYLE);
        //TOGL_INNER_CASE_STR(IE_GETPAINTDC);
        //TOGL_INNER_CASE_STR(MCIWNDM_NOTIFYERROR);
        //TOGL_INNER_CASE_STR(IE_GETPDEVENT);
        //TOGL_INNER_CASE_STR(IE_GETSELCOUNT);
        //TOGL_INNER_CASE_STR(IE_GETSELITEMS);
        //TOGL_INNER_CASE_STR(IE_GETSTYLE);
        //TOGL_INNER_CASE_STR(MCIWNDM_SETTIMEFORMATW);
        //TOGL_INNER_CASE_STR(EM_OUTLINE);
        //TOGL_INNER_CASE_STR(MCIWNDM_GETTIMEFORMATW);
        //TOGL_INNER_CASE_STR(EM_GETSCROLLPOS);
        //TOGL_INNER_CASE_STR(EM_SETSCROLLPOS);
        //TOGL_INNER_CASE_STR(EM_SETSCROLLPOS);
        //TOGL_INNER_CASE_STR(EM_SETFONTSIZE);
        //TOGL_INNER_CASE_STR(EM_GETZOOM);
        //TOGL_INNER_CASE_STR(MCIWNDM_GETFILENAMEW);
        //TOGL_INNER_CASE_STR(EM_SETZOOM);
        //TOGL_INNER_CASE_STR(MCIWNDM_GETDEVICEW);
        //TOGL_INNER_CASE_STR(EM_GETVIEWKIND);
        //TOGL_INNER_CASE_STR(EM_SETVIEWKIND);
        //TOGL_INNER_CASE_STR(EM_GETPAGE);
        //TOGL_INNER_CASE_STR(MCIWNDM_GETERRORW);
        //TOGL_INNER_CASE_STR(EM_SETPAGE);
        //TOGL_INNER_CASE_STR(EM_GETHYPHENATEINFO);
        //TOGL_INNER_CASE_STR(EM_SETHYPHENATEINFO);
        //TOGL_INNER_CASE_STR(EM_GETPAGEROTATE);
        //TOGL_INNER_CASE_STR(EM_SETPAGEROTATE);
        //TOGL_INNER_CASE_STR(EM_GETCTFMODEBIAS);
        //TOGL_INNER_CASE_STR(EM_SETCTFMODEBIAS);
        //TOGL_INNER_CASE_STR(EM_GETCTFOPENSTATUS);
        //TOGL_INNER_CASE_STR(EM_SETCTFOPENSTATUS);
        //TOGL_INNER_CASE_STR(EM_GETIMECOMPTEXT);
        //TOGL_INNER_CASE_STR(EM_ISIME);
        //TOGL_INNER_CASE_STR(EM_GETIMEPROPERTY);
        //TOGL_INNER_CASE_STR(EM_GETQUERYRTFOBJ);
        //TOGL_INNER_CASE_STR(EM_SETQUERYRTFOBJ);
        //TOGL_INNER_CASE_STR(FM_GETFOCUS);
        //TOGL_INNER_CASE_STR(FM_GETDRIVEINFOA);
        //TOGL_INNER_CASE_STR(FM_GETSELCOUNT);
        //TOGL_INNER_CASE_STR(FM_GETSELCOUNTLFN);
        //TOGL_INNER_CASE_STR(FM_GETFILESELA);
        //TOGL_INNER_CASE_STR(FM_GETFILESELLFNA);
        //TOGL_INNER_CASE_STR(FM_REFRESH_WINDOWS);
        //TOGL_INNER_CASE_STR(FM_RELOAD_EXTENSIONS);
        //TOGL_INNER_CASE_STR(FM_GETDRIVEINFOW);
        //TOGL_INNER_CASE_STR(FM_GETFILESELW);
        //TOGL_INNER_CASE_STR(FM_GETFILESELLFNW);
        //TOGL_INNER_CASE_STR(WLX_WM_SAS);
        //TOGL_INNER_CASE_STR(SM_GETSELCOUNT);
        //TOGL_INNER_CASE_STR(UM_GETSELCOUNT);
        //TOGL_INNER_CASE_STR(WM_CPL_LAUNCH);
        //TOGL_INNER_CASE_STR(SM_GETSERVERSELA);
        //TOGL_INNER_CASE_STR(UM_GETUSERSELA);
        //TOGL_INNER_CASE_STR(WM_CPL_LAUNCHED);
        //TOGL_INNER_CASE_STR(SM_GETSERVERSELW);
        //TOGL_INNER_CASE_STR(UM_GETUSERSELW);
        //TOGL_INNER_CASE_STR(SM_GETCURFOCUSA);
        //TOGL_INNER_CASE_STR(UM_GETGROUPSELA);
        //TOGL_INNER_CASE_STR(SM_GETCURFOCUSW);
        //TOGL_INNER_CASE_STR(UM_GETGROUPSELW);
        //TOGL_INNER_CASE_STR(SM_GETOPTIONS);
        //TOGL_INNER_CASE_STR(UM_GETCURFOCUSA);
        //TOGL_INNER_CASE_STR(UM_GETCURFOCUSW);
        //TOGL_INNER_CASE_STR(UM_GETOPTIONS);
        //TOGL_INNER_CASE_STR(UM_GETOPTIONS2);
        //TOGL_INNER_CASE_STR(LVM_FIRST);
        //TOGL_INNER_CASE_STR(LVM_GETBKCOLOR);
        //TOGL_INNER_CASE_STR(LVM_SETBKCOLOR);
        //TOGL_INNER_CASE_STR(LVM_GETIMAGELIST);
        //TOGL_INNER_CASE_STR(LVM_SETIMAGELIST);
        //TOGL_INNER_CASE_STR(LVM_GETITEMCOUNT);
        //TOGL_INNER_CASE_STR(LVM_GETITEMA);
        //TOGL_INNER_CASE_STR(LVM_SETITEMA);
        //TOGL_INNER_CASE_STR(LVM_INSERTITEMA);
        //TOGL_INNER_CASE_STR(LVM_DELETEITEM);
        //TOGL_INNER_CASE_STR(LVM_DELETEALLITEMS);
        //TOGL_INNER_CASE_STR(LVM_GETCALLBACKMASK);
        //TOGL_INNER_CASE_STR(LVM_SETCALLBACKMASK);
        //TOGL_INNER_CASE_STR(LVM_GETNEXTITEM);
        //TOGL_INNER_CASE_STR(LVM_FINDITEMA);
        //TOGL_INNER_CASE_STR(LVM_GETITEMRECT);
        //TOGL_INNER_CASE_STR(LVM_SETITEMPOSITION);
        //TOGL_INNER_CASE_STR(LVM_GETITEMPOSITION);
        //TOGL_INNER_CASE_STR(LVM_GETSTRINGWIDTHA);
        //TOGL_INNER_CASE_STR(LVM_HITTEST);
        //TOGL_INNER_CASE_STR(LVM_ENSUREVISIBLE);
        //TOGL_INNER_CASE_STR(LVM_SCROLL);
        //TOGL_INNER_CASE_STR(LVM_REDRAWITEMS);
        //TOGL_INNER_CASE_STR(LVM_ARRANGE);
        //TOGL_INNER_CASE_STR(LVM_EDITLABELA);
        //TOGL_INNER_CASE_STR(LVM_GETEDITCONTROL);
        //TOGL_INNER_CASE_STR(LVM_GETCOLUMNA);
        //TOGL_INNER_CASE_STR(LVM_SETCOLUMNA);
        //TOGL_INNER_CASE_STR(LVM_INSERTCOLUMNA);
        //TOGL_INNER_CASE_STR(LVM_DELETECOLUMN);
        //TOGL_INNER_CASE_STR(LVM_GETCOLUMNWIDTH);
        //TOGL_INNER_CASE_STR(LVM_SETCOLUMNWIDTH);
        //TOGL_INNER_CASE_STR(LVM_GETHEADER);
        //TOGL_INNER_CASE_STR(LVM_CREATEDRAGIMAGE);
        //TOGL_INNER_CASE_STR(LVM_GETVIEWRECT);
        //TOGL_INNER_CASE_STR(LVM_GETTEXTCOLOR);
        //TOGL_INNER_CASE_STR(LVM_SETTEXTCOLOR);
        //TOGL_INNER_CASE_STR(LVM_GETTEXTBKCOLOR);
        //TOGL_INNER_CASE_STR(LVM_SETTEXTBKCOLOR);
        //TOGL_INNER_CASE_STR(LVM_GETTOPINDEX);
        //TOGL_INNER_CASE_STR(LVM_GETCOUNTPERPAGE);
        //TOGL_INNER_CASE_STR(LVM_GETORIGIN);
        //TOGL_INNER_CASE_STR(LVM_UPDATE);
        //TOGL_INNER_CASE_STR(LVM_SETITEMSTATE);
        //TOGL_INNER_CASE_STR(LVM_GETITEMSTATE);
        //TOGL_INNER_CASE_STR(LVM_GETITEMTEXTA);
        //TOGL_INNER_CASE_STR(LVM_SETITEMTEXTA);
        //TOGL_INNER_CASE_STR(LVM_SETITEMCOUNT);
        //TOGL_INNER_CASE_STR(LVM_SORTITEMS);
        //TOGL_INNER_CASE_STR(LVM_SETITEMPOSITION32);
        //TOGL_INNER_CASE_STR(LVM_GETSELECTEDCOUNT);
        //TOGL_INNER_CASE_STR(LVM_GETITEMSPACING);
        //TOGL_INNER_CASE_STR(LVM_GETISEARCHSTRINGA);
        //TOGL_INNER_CASE_STR(LVM_SETICONSPACING);
        //TOGL_INNER_CASE_STR(LVM_SETEXTENDEDLISTVIEWSTYLE);
        //TOGL_INNER_CASE_STR(LVM_GETEXTENDEDLISTVIEWSTYLE);
        //TOGL_INNER_CASE_STR(LVM_GETSUBITEMRECT);
        //TOGL_INNER_CASE_STR(LVM_SUBITEMHITTEST);
        //TOGL_INNER_CASE_STR(LVM_SETCOLUMNORDERARRAY);
        //TOGL_INNER_CASE_STR(LVM_GETCOLUMNORDERARRAY);
        //TOGL_INNER_CASE_STR(LVM_SETHOTITEM);
        //TOGL_INNER_CASE_STR(LVM_GETHOTITEM);
        //TOGL_INNER_CASE_STR(LVM_SETHOTCURSOR);
        //TOGL_INNER_CASE_STR(LVM_GETHOTCURSOR);
        //TOGL_INNER_CASE_STR(LVM_APPROXIMATEVIEWRECT);
        //TOGL_INNER_CASE_STR(LVM_SETWORKAREAS);
        //TOGL_INNER_CASE_STR(LVM_GETSELECTIONMARK);
        //TOGL_INNER_CASE_STR(LVM_SETSELECTIONMARK);
        //TOGL_INNER_CASE_STR(LVM_SETBKIMAGEA);
        //TOGL_INNER_CASE_STR(LVM_GETBKIMAGEA);
        //TOGL_INNER_CASE_STR(LVM_GETWORKAREAS);
        //TOGL_INNER_CASE_STR(LVM_SETHOVERTIME);
        //TOGL_INNER_CASE_STR(LVM_GETHOVERTIME);
        //TOGL_INNER_CASE_STR(LVM_GETNUMBEROFWORKAREAS);
        //TOGL_INNER_CASE_STR(LVM_SETTOOLTIPS);
        //TOGL_INNER_CASE_STR(LVM_GETITEMW);
        //TOGL_INNER_CASE_STR(LVM_SETITEMW);
        //TOGL_INNER_CASE_STR(LVM_INSERTITEMW);
        //TOGL_INNER_CASE_STR(LVM_GETTOOLTIPS);
        //TOGL_INNER_CASE_STR(LVM_FINDITEMW);
        //TOGL_INNER_CASE_STR(LVM_GETSTRINGWIDTHW);
        //TOGL_INNER_CASE_STR(LVM_GETCOLUMNW);
        //TOGL_INNER_CASE_STR(LVM_SETCOLUMNW);
        //TOGL_INNER_CASE_STR(LVM_INSERTCOLUMNW);
        //TOGL_INNER_CASE_STR(LVM_GETITEMTEXTW);
        //TOGL_INNER_CASE_STR(LVM_SETITEMTEXTW);
        //TOGL_INNER_CASE_STR(LVM_GETISEARCHSTRINGW);
        //TOGL_INNER_CASE_STR(LVM_EDITLABELW);
        //TOGL_INNER_CASE_STR(LVM_GETBKIMAGEW);
        //TOGL_INNER_CASE_STR(LVM_SETSELECTEDCOLUMN);
        //TOGL_INNER_CASE_STR(LVM_SETTILEWIDTH);
        //TOGL_INNER_CASE_STR(LVM_SETVIEW);
        //TOGL_INNER_CASE_STR(LVM_GETVIEW);
        //TOGL_INNER_CASE_STR(LVM_INSERTGROUP);
        //TOGL_INNER_CASE_STR(LVM_SETGROUPINFO);
        //TOGL_INNER_CASE_STR(LVM_GETGROUPINFO);
        //TOGL_INNER_CASE_STR(LVM_REMOVEGROUP);
        //TOGL_INNER_CASE_STR(LVM_MOVEGROUP);
        //TOGL_INNER_CASE_STR(LVM_MOVEITEMTOGROUP);
        //TOGL_INNER_CASE_STR(LVM_SETGROUPMETRICS);
        //TOGL_INNER_CASE_STR(LVM_GETGROUPMETRICS);
        //TOGL_INNER_CASE_STR(LVM_ENABLEGROUPVIEW);
        //TOGL_INNER_CASE_STR(LVM_SORTGROUPS);
        //TOGL_INNER_CASE_STR(LVM_INSERTGROUPSORTED);
        //TOGL_INNER_CASE_STR(LVM_REMOVEALLGROUPS);
        //TOGL_INNER_CASE_STR(LVM_HASGROUP);
        //TOGL_INNER_CASE_STR(LVM_SETTILEVIEWINFO);
        //TOGL_INNER_CASE_STR(LVM_GETTILEVIEWINFO);
        //TOGL_INNER_CASE_STR(LVM_SETTILEINFO);
        //TOGL_INNER_CASE_STR(LVM_GETTILEINFO);
        //TOGL_INNER_CASE_STR(LVM_SETINSERTMARK);
        //TOGL_INNER_CASE_STR(LVM_GETINSERTMARK);
        //TOGL_INNER_CASE_STR(LVM_INSERTMARKHITTEST);
        //TOGL_INNER_CASE_STR(LVM_GETINSERTMARKRECT);
        //TOGL_INNER_CASE_STR(LVM_SETINSERTMARKCOLOR);
        //TOGL_INNER_CASE_STR(LVM_GETINSERTMARKCOLOR);
        //TOGL_INNER_CASE_STR(LVM_SETINFOTIP);
        //TOGL_INNER_CASE_STR(LVM_GETSELECTEDCOLUMN);
        //TOGL_INNER_CASE_STR(LVM_ISGROUPVIEWENABLED);
        //TOGL_INNER_CASE_STR(LVM_GETOUTLINECOLOR);
        //TOGL_INNER_CASE_STR(LVM_SETOUTLINECOLOR);
        //TOGL_INNER_CASE_STR(LVM_CANCELEDITLABEL);
        //TOGL_INNER_CASE_STR(LVM_MAPINDEXTOID);
        //TOGL_INNER_CASE_STR(LVM_MAPIDTOINDEX);
        //TOGL_INNER_CASE_STR(LVM_ISITEMVISIBLE);
        //TOGL_INNER_CASE_STR(LVM_GETEMPTYTEXT);
        //TOGL_INNER_CASE_STR(LVM_GETFOOTERRECT);
        //TOGL_INNER_CASE_STR(LVM_GETFOOTERINFO);
        //TOGL_INNER_CASE_STR(LVM_GETFOOTERITEMRECT);
        //TOGL_INNER_CASE_STR(LVM_GETFOOTERITEM);
        //TOGL_INNER_CASE_STR(LVM_GETITEMINDEXRECT);
        //TOGL_INNER_CASE_STR(LVM_SETITEMINDEXSTATE);
        //TOGL_INNER_CASE_STR(LVM_GETNEXTITEMINDEX);
        //TOGL_INNER_CASE_STR(OCM__BASE);
        //TOGL_INNER_CASE_STR(LVM_SETUNICODEFORMAT);
        //TOGL_INNER_CASE_STR(LVM_GETUNICODEFORMAT);
        //TOGL_INNER_CASE_STR(OCM_CTLCOLOR);
        //TOGL_INNER_CASE_STR(OCM_DRAWITEM);
        //TOGL_INNER_CASE_STR(OCM_MEASUREITEM);
        //TOGL_INNER_CASE_STR(OCM_DELETEITEM);
        //TOGL_INNER_CASE_STR(OCM_VKEYTOITEM);
        //TOGL_INNER_CASE_STR(OCM_CHARTOITEM);
        //TOGL_INNER_CASE_STR(OCM_COMPAREITEM);
        //TOGL_INNER_CASE_STR(OCM_NOTIFY);
        //TOGL_INNER_CASE_STR(OCM_COMMAND);
        //TOGL_INNER_CASE_STR(OCM_HSCROLL);
        //TOGL_INNER_CASE_STR(OCM_VSCROLL);
        //TOGL_INNER_CASE_STR(OCM_CTLCOLORMSGBOX);
        //TOGL_INNER_CASE_STR(OCM_CTLCOLOREDIT);
        //TOGL_INNER_CASE_STR(OCM_CTLCOLORLISTBOX);
        //TOGL_INNER_CASE_STR(OCM_CTLCOLORBTN);
        //TOGL_INNER_CASE_STR(OCM_CTLCOLORDLG);
        //TOGL_INNER_CASE_STR(OCM_CTLCOLORSCROLLBAR);
        //TOGL_INNER_CASE_STR(OCM_CTLCOLORSTATIC);
        //TOGL_INNER_CASE_STR(OCM_PARENTNOTIFY);
        TOGL_INNER_CASE_STR(WM_APP);
        //TOGL_INNER_CASE_STR(WM_RASDIALEVENT);
        }
        return std::string() + "(" + std::to_string(message) +")";
    }

#undef TOGL_INNER_CASE_STR
}



#endif // TRIVIALOPENGL_WINDOW_H_
