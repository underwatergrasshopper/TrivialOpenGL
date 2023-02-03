/**
* @file TrivialOpenGL.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_WINDOW_H_
#define TRIVIALOPENGL_WINDOW_H_

#include <GL\GL.h>
#include <string>

#include "TrivialOpenGL_Utility.h"

namespace TrivialOpenGL {

    enum {
        DEF = -1,
    };

    struct StyleBit {
        using Field = uint32_t;
        enum {
            NO_RESIZE               = 0x0001,
            NO_MAXIMIZE             = 0x0002,
            CENTERED                = 0x0004,
            CLIENT_SIZE             = 0x0008,
            CLIENT_ONLY             = 0x0010,
            REDRAW_ON_REQUEST_ONLY  = 0x0020,
        };
    };

    enum class WindowState {
        NORMAL,
        HIDDEN,
        MAXIMIZED,
        MINIMIZED,
        FULL_SCREEN
    };

    struct Data {
        // Encoding: ASCII or UTF8.
        std::string     window_name         = "Window";

        // If x is DEF then created window will be centered on X axis.
        // If y is DEF then created window will be centered on Y axis.
        // If width is DEF then created window will have with equal to half width of the screen.
        // If height is DEF then created window will have with equal to half height of the screen.
        AreaI           area                = {DEF, DEF, DEF, DEF};

        StyleBit::Field style               = 0;

        // Tries create OpenGL Rendering Context which support to at least this version, with compatibility to all previous versions.
        // If opengl_version.major and opengl_version.minor is DEF then creates for any available OpenGL version. Can be checked by GetOpenGL_Version().
        Version         opengl_verion       = {DEF, DEF};

        // (Optional) File name of icon image file (.ico). 
        // Loaded icon will be presented on window title bar and on task bar.
        std::string     icon_file_name      = "";

        // (Optional) Resource id of icon file. 
        // Id must be different than 0 and icon_file_name must be an empty string.
        // Loaded icon will be presented on window title bar and at application button on task bar. 
        uint16_t        icon_resource_id    = 0;       

        // 0 - error
        // 1 - info
        // 2 - debug
        // 3 - deep debug; Warning!!! Can slowdown application significantly.
        uint32_t        info_level          = 0;

        // Is called right before window is opened.
        void (*do_on_create)()                                          = nullptr;

        // Is called right after window is closed.
        void (*do_on_destroy)()                                         = nullptr;

        // Is called each time when window content needs to be redrawn.
        void (*display)()                                               = nullptr;

        void (*do_on_key_down_raw)(WPARAM w_param, LPARAM l_param)      = nullptr;
        void (*do_on_key_up_raw)(WPARAM w_param, LPARAM l_param)        = nullptr;
 
        void (*do_on_size)(uint32_t width, uint32_t height)             = nullptr;

    };

    // Corrects window position and size to removes invisible borders in Windows 10. 
    class WindowAreaCorrection {
    public:
        WindowAreaCorrection() {
            m_dwmapi_lib_handle = LoadLibraryA("Dwmapi.dll");
            if (m_dwmapi_lib_handle) {
                m_dwm_get_window_attribute = (decltype(m_dwm_get_window_attribute)) GetProcAddress(m_dwmapi_lib_handle, "DwmGetWindowAttribute");
            } else {
                m_dwm_get_window_attribute = nullptr;
            }
        }
        virtual ~WindowAreaCorrection() {
            FreeLibrary(m_dwmapi_lib_handle);
        }

        AreaI Get(HWND window_handle) const {
            AreaI area = {};

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

                area = {-frame.left, -frame.top, frame.left + frame.right, frame.top + frame.bottom};
            }

            return area;
        }

        AreaI Apply(const AreaI& area, HWND window_hangle) const {
            const AreaI correction = Get(window_hangle);
            return {
                area.x      + correction.x,
                area.y      + correction.y,
                area.width  + correction.width,
                area.height + correction.height
            };
        }

        SizeI Apply(const SizeI& size, HWND window_hangle) const {
            const AreaI correction = Get(window_hangle);
            return {
                size.width  + correction.width,
                size.height + correction.height
            };
        }

        PointI Apply(const PointI& pos, HWND window_hangle) const {
            const AreaI correction = Get(window_hangle);
            return {
                pos.x + correction.x,
                pos.y + correction.y,
            };
        }

        AreaI Revert(const AreaI& area, HWND window_hangle) const {
            const AreaI correction = Get(window_hangle);
            return {
                area.x      - correction.x,
                area.y      - correction.y,
                area.width  - correction.width,
                area.height - correction.height
            };
        }

        SizeI Revert(const SizeI& size, HWND window_hangle) const {
            const AreaI correction = Get(window_hangle);
            return {
                size.width  - correction.width,
                size.height - correction.height
            };
        }

        PointI Revert(const PointI& pos, HWND window_hangle) const {
            const AreaI correction = Get(window_hangle);
            return {
                pos.x - correction.x,
                pos.y - correction.y,
            };
        }
    private:
        HMODULE  m_dwmapi_lib_handle;
        HRESULT (*m_dwm_get_window_attribute)(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute);
    };

    class Window {
    public:
        Window();
        virtual ~Window();

        // ---

        int Run(const Data& data);

        void MarkToClose();
        void MarkToRedraw();

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

        // Changes area by applying style from data parameter which was provided to Run function.
        void ChangeArea(const AreaI& area);

        // Puts window in top most position in z-order.
        void Top();

        // Centers window in desktop area excluding task bar area.
        void Center();

        AreaI GetArea() const;
        AreaI GetDrawArea() const;
        SizeI GetDrawAreaSize() const;

        // ---

        // Ignores MAXIMIZED when REDRAW_ON_REQUEST_ONLY and CLIENT_ONLY.
        // Ignores FULL_SCREEN when REDRAW_ON_REQUEST_ONLY.
        void ChangeState(WindowState state);

        void Hide();
        void Restore();
        void Minimize();
        void Maximize();
        void MakeFullScreen();

        WindowState GetState() const;

        bool IsNormal() const;
        bool IsHidden() const;
        bool IsMinimized() const;
        bool IsMaximized() const;
        bool IsFullScreen() const;

        // ---

        uint32_t GetDebugLevel() const;
        Version GetOpenGL_Version() const;

        // ---
        // On your responsibility.
        HWND GetHWND();
        HDC GetWindowDC();

    private:
        void SingletonCheck();
        int ExecuteMainLoop();
        HICON TryLoadIcon();

        AreaI GenerateWindowArea(const AreaI& area);

        void Create(HWND window_handle);
        void Destroy();
        void Paint();
        void Display();
        void Close();

        static AreaI GetWindowArea(HWND window_handle); // Warning: Do not correct area.
        static AreaI GetWindowAreaFromDrawArea(const AreaI& draw_area, DWORD window_style);
        static LRESULT CALLBACK WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

        Data        m_data;

        HINSTANCE   m_instance_handle;
        HWND        m_window_handle;
        HDC         m_device_context_handle;
        HGLRC       m_rendering_context_handle;

        DWORD       m_window_style;
        DWORD       m_window_extended_style;

        WindowState m_state;

        WindowAreaCorrection m_area_correction;
    };

    inline Window& ToWindow() {
        return Static<Window>::To();
    }

    inline int Run(const Data& data = {}) {
        return ToWindow().Run(data);
    }

} // namespace TrivialOpenGL


namespace TrivialOpenGL {

    inline Window::Window() {
        SingletonCheck();

        m_data                      = {};

        m_instance_handle           = NULL;
        m_window_handle             = NULL;
        m_device_context_handle     = NULL;
        m_rendering_context_handle  = NULL;

        m_window_style              = 0;
        m_window_extended_style     = 0;
    }

    inline Window::~Window() {

    }

    inline void Window::MoveTo(int x, int y) {
        MoveTo({x, y});
    }
    inline void Window::MoveTo(const PointI& pos) {
        const PointI correct_pos = m_area_correction.Apply(pos, m_window_handle);
        SetWindowPos(m_window_handle, HWND_TOP, correct_pos.x, correct_pos.y, 0, 0, SWP_NOSIZE);
    }

    inline void Window::SetDrawAreaPos(int x, int y) {
        SetDrawAreaPos({x, y});
    }
    inline void Window::SetDrawAreaPos(const PointI& pos) {
        const PointI window_pos = GetWindowAreaFromDrawArea(AreaI(pos, {1, 1}), m_window_style).GetPos();
        SetWindowPos(m_window_handle, HWND_TOP, window_pos.x, window_pos.y, 0, 0, SWP_NOSIZE);
    }

    inline void Window::MoveBy(int x, int y) {
        const PointI curr_pos = GetArea().GetPos();

        MoveTo(curr_pos.x + x, curr_pos.y + y);
    }
    inline void Window::MoveBy(const PointI& pos) {
        MoveBy(pos.x, pos.y);
    }

    inline void Window::SetSize(int width, int height) {
        SetSize({width, height});
    }
    inline void Window::SetSize(const SizeI& size) {
        const SizeI correct_size = m_area_correction.Apply(size, m_window_handle);
        SetWindowPos(m_window_handle, HWND_TOP, 0, 0, correct_size.width, correct_size.height, SWP_NOMOVE);
    }

    inline void Window::SetDrawAreaSize(int width, int height) {
        SetDrawAreaSize({width, height});
    }
    inline void Window::SetDrawAreaSize(const SizeI& size) {
        const SizeI window_size = GetWindowAreaFromDrawArea(AreaI({}, size), m_window_style).GetSize();
        SetWindowPos(m_window_handle, HWND_TOP, 0, 0, window_size.width, window_size.height, SWP_NOMOVE);
    }

    inline void Window::SetArea(int x, int y, int width, int height) {
        SetArea({x, y, width, height});
    }
    inline void Window::SetArea(const AreaI& area) {
        const AreaI correct_area = m_area_correction.Apply(area, m_window_handle);
        SetWindowPos(m_window_handle, HWND_TOP, correct_area.x, correct_area.y, correct_area.width, correct_area.height, 0);
    }

    inline void Window::SetDrawArea(int x, int y, int width, int height) {
        SetDrawArea({x, y, width, height});
    }
    inline void Window::SetDrawArea(const AreaI& area) {
        const AreaI window_area = GetWindowAreaFromDrawArea(area, m_window_style);
        SetWindowPos(m_window_handle, HWND_TOP, window_area.x, window_area.y, window_area.width, window_area.height, 0);
    }

    inline void Window::Top() {
        BringWindowToTop(m_window_handle);
    }

    inline void Window::Center() {
        const AreaI desktop_area = GetDesktopAreaNoTaskBar();

        AreaI window_area = GetArea();

        window_area.x = (desktop_area.width - window_area.width) / 2;
        window_area.y = (desktop_area.height - window_area.height) / 2;

        MoveTo(window_area.GetPos());
    }

    inline void Window::ChangeArea(const AreaI& area) {
        const AreaI window_area = GenerateWindowArea(area);
        if (!(m_data.style & StyleBit::CLIENT_ONLY) && !(m_data.style & StyleBit::CLIENT_SIZE)) {
            SetArea(window_area);
        } else {
            SetWindowPos(m_window_handle, HWND_TOP, window_area.x, window_area.y, window_area.width, window_area.height, 0);
        }
    }

    inline void Window::Hide() {
        ChangeState(WindowState::HIDDEN);
    }

    inline void Window::Restore() {
        ChangeState(WindowState::NORMAL);
    }

    inline void Window::Minimize() {
        ChangeState(WindowState::MINIMIZED);
    }

    inline void Window::Maximize() {
        ChangeState(WindowState::MAXIMIZED);
    }

    inline void Window::MakeFullScreen() {
        ChangeState(WindowState::FULL_SCREEN);
    }

    inline WindowState Window::GetState() const {
        return m_state;
    }

    inline bool Window::IsNormal() const {
        return GetState() == WindowState::NORMAL;
    }

    inline bool Window::IsHidden() const {
        return GetState() == WindowState::HIDDEN;
    }

    inline bool Window::IsMinimized() const {
        return GetState() == WindowState::MINIMIZED;
    }

    inline bool Window::IsMaximized() const {
        return GetState() == WindowState::MAXIMIZED;
    }

    inline bool Window::IsFullScreen() const {
        return GetState() == WindowState::FULL_SCREEN;
    }

    inline AreaI Window::GetArea() const {
        return m_area_correction.Revert(GetWindowArea(m_window_handle), m_window_handle);
    }

    inline AreaI Window::GetDrawArea() const {
        RECT r;
        if (GetClientRect(m_window_handle, &r)) {
            return MakeArea(r);
        }
        return AreaI(0, 0, 0, 0);
    }

    inline SizeI Window::GetDrawAreaSize() const {
        return GetDrawArea().GetSize();
    }


    inline void Window::MarkToClose() {
        DestroyWindow(m_window_handle);
    }

    inline void Window::MarkToRedraw() {
        InvalidateRect(m_window_handle, NULL, FALSE);
    }

    inline uint32_t Window::GetDebugLevel() const { 
        return m_data.info_level; 
    }

    inline HWND Window::GetHWND() {
        return m_window_handle;
    }

    inline HDC Window::GetWindowDC() {
        return m_device_context_handle;
    }

    inline Version Window::GetOpenGL_Version() const {
        return m_data.opengl_verion;
    }

    inline void Window::ChangeState(WindowState state) {
        // Forbidden combination of states and styles. 
        // Window do not redraw its content in some cases. 
        // Window loses focus and locks at top most z-position.
        if (state == WindowState::MAXIMIZED && (m_data.style & StyleBit::REDRAW_ON_REQUEST_ONLY) && (m_data.style & StyleBit::CLIENT_ONLY)) return; 
        if (state == WindowState::FULL_SCREEN && (m_data.style & StyleBit::REDRAW_ON_REQUEST_ONLY)) return; 

        if (m_state != WindowState::NORMAL) {
            ShowWindow(m_window_handle, SW_NORMAL);

            if (m_state == WindowState::FULL_SCREEN) {
                SetWindowLongPtrA(m_window_handle, GWL_STYLE, m_window_style);
                SetWindowLongPtrA(m_window_handle, GWL_EXSTYLE, m_window_extended_style);

                if (state == WindowState::HIDDEN) {
                    // Just for testing.
                    //SetWindowPos(m_window_handle, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);
                    ShowWindow(m_window_handle, SW_NORMAL);
                }
            }
        }

        switch (state) {
        case WindowState::NORMAL:
            ShowWindow(m_window_handle, SW_NORMAL);
            break;
        case WindowState::HIDDEN:
            ShowWindow(m_window_handle, SW_HIDE);
            break;
        case WindowState::MINIMIZED:
            ShowWindow(m_window_handle, SW_MINIMIZE);
            break;
        case WindowState::MAXIMIZED: 
            ShowWindow(m_window_handle, SW_MAXIMIZE);
            break;
        case WindowState::FULL_SCREEN:
            SetWindowLongPtrA(m_window_handle, GWL_STYLE, WS_POPUP);
            SetWindowLongPtrA(m_window_handle, GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_APPWINDOW);
            ShowWindow(m_window_handle, SW_MAXIMIZE);
            break;
        }

        m_state = state;
    }

    inline int Window::Run(const Data& data) {
        m_data = data;

        if (!m_data.do_on_create)   m_data.do_on_create     = []() {};
        if (!m_data.do_on_destroy)  m_data.do_on_destroy    = []() {};
        if (!m_data.display)        m_data.display          = []() {};

        if (!m_data.do_on_key_down_raw) m_data.do_on_key_down_raw   = [](WPARAM w_param, LPARAM l_param) {};
        if (!m_data.do_on_key_up_raw)   m_data.do_on_key_up_raw     = [](WPARAM w_param, LPARAM l_param) {};

        if (!m_data.do_on_size)         m_data.do_on_size           = [](uint32_t width, uint32_t height) {};

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
        if (m_data.style & StyleBit::CLIENT_ONLY) {
            m_window_extended_style = WS_EX_TOOLWINDOW | WS_EX_APPWINDOW;
            m_window_style          = WS_POPUP;
        }

        const AreaI window_area = GenerateWindowArea(m_data.area);

        m_window_handle = CreateWindowExW(
            m_window_extended_style,
            WINDOW_CLASS_NAME,
            ToUTF16(m_data.window_name).c_str(),
            m_window_style,
            window_area.x, window_area.y, window_area.width, window_area.height,
            NULL,
            NULL,
            m_instance_handle,
            NULL
        );

        if (!m_window_handle) {
            LogFatalError("Error TOGLW::Window::Run: Cannot create window.");
        }

        m_data.do_on_create();

        ShowWindow(m_window_handle, SW_SHOW);

        if (!(m_data.style & StyleBit::CLIENT_ONLY) && !(m_data.style & StyleBit::CLIENT_SIZE)) {
            // Here because, actual window area can be fetched by DwmGetWindowAttribute only after SW_SHOW.
            SetArea(window_area); 

            if (m_data.style & StyleBit::CENTERED ) Center();
        }

        UpdateWindow(m_window_handle);

        return ExecuteMainLoop();
    }

    inline LRESULT CALLBACK Window::WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
        switch (message) {
            case WM_CREATE: 
                ToWindow().Create(window_handle);
                return 0;

            case WM_DESTROY:
                ToWindow().Destroy();
                return 0;

            case WM_CLOSE:
                ToWindow().Close();
                return 0;

            case WM_PAINT:
                ToWindow().Paint();
                return 0;

            case WM_KEYDOWN:
                ToWindow().m_data.do_on_key_down_raw(w_param, l_param);
                return 0;

            case WM_KEYUP:
                ToWindow().m_data.do_on_key_up_raw(w_param, l_param);
                return 0;

            case WM_SIZE:
                ToWindow().m_data.do_on_size(LOWORD(l_param), HIWORD(l_param));
                return 0;

            case WM_ERASEBKGND:
                // Tells DefWindowProc to not erase background. It's unnecessary since background is handled by OpenGL.
                return 1;
        }
        return DefWindowProcW(window_handle, message, w_param, l_param);
    }

    inline void Window::SingletonCheck() {
        static bool is_instance_exists = false;
        if (is_instance_exists) {
            LogFatalError("Error TOGL::Window::SingletonCheck: Can't be more than one instance of Window class.");
        }
        is_instance_exists = true;
    }

    inline int Window::ExecuteMainLoop() {
        MSG msg = {};

        if (m_data.style & StyleBit::REDRAW_ON_REQUEST_ONLY) {
            while (GetMessageW(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            if (msg.message == WM_QUIT) return (int)msg.wParam;

        } else {
            while (true) {
                while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
                    if (msg.message == WM_QUIT) return (int)msg.wParam;

                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                }
                Display();
            }
        }

        return EXIT_FAILURE;
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

    inline AreaI Window::GetWindowArea(HWND window_handle) {
        RECT r;
        if (GetWindowRect(window_handle, &r)) {
            return MakeArea(r);
        }
        return AreaI(0, 0, 0, 0);
    }

    inline AreaI Window::GenerateWindowArea(const AreaI& area) {
        AreaI window_area = area;

        const AreaI desktop_area = GetDesktopAreaNoTaskBar();

        // === Solve Size === //

        if (area.width == DEF)  window_area.width     = desktop_area.width / 2;
        if (area.height == DEF) window_area.height    = desktop_area.height / 2;

        if ((m_data.style & StyleBit::CLIENT_SIZE) && !(m_data.style & StyleBit::CLIENT_ONLY)) {
            window_area.SetSize(GetWindowAreaFromDrawArea(AreaI({}, window_area.GetSize()), m_window_style).GetSize());

            if (window_area.width < 0)    window_area.width = 0;
            if (window_area.height < 1)   window_area.height = 1;
        }

        // === Solve Position === //

        if (m_data.style & StyleBit::CENTERED) {
            window_area.x = (desktop_area.width - window_area.width) / 2;
            window_area.y = (desktop_area.height - window_area.height) / 2;
        } else {
            if (area.x == DEF) window_area.x = (desktop_area.width - window_area.width) / 2;
            if (area.y == DEF) window_area.y = (desktop_area.height - window_area.height) / 2;
        }

        // ===

        return window_area;
    }

    inline AreaI Window::GetWindowAreaFromDrawArea(const AreaI& draw_area, DWORD window_style) {
        RECT rect = MakeRECT(draw_area);
        AdjustWindowRect(&rect, window_style, FALSE);
        return MakeArea(rect);
    }

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
        if (!m_device_context_handle) LogFatalError("Error TOGL::Window::Create: Can not get device context.");

        int pfi = ChoosePixelFormat(m_device_context_handle, &pfd);
        if (!pfi) LogFatalError(std::string() + "Error TOGL::Window::Create: Can not choose pixel format. (windows error code:" + std::to_string(GetLastError()) + ")");

        BOOL result = SetPixelFormat(m_device_context_handle, pfi, &pfd);
        if (!result) LogFatalError(std::string() + "Error TOGL::Window::Create: Can not set pixel format. (windows error code:" + std::to_string(GetLastError()) + ")");

        // --- Displaying Format Info --- //

        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        int max_pfi = DescribePixelFormat(m_device_context_handle, pfi, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
        if (!max_pfi) LogFatalError(std::string() + "Error TOGL::Window::Create: Can not get pixel format. (windows error code:" + std::to_string(GetLastError()) + ")");

        LogInfo(std::string() + "(TOGL) OpenGL Pixel Format:"
            " Red:"     + std::to_string(pfd.cRedBits) + 
            " Green:"   + std::to_string(pfd.cGreenBits) + 
            " Blue:"    + std::to_string(pfd.cBlueBits) + 
            " Alpha:"   + std::to_string(pfd.cAlphaBits) + 
            " Depth:"   + std::to_string(pfd.cDepthBits) + 
            " Stencil:" + std::to_string(pfd.cStencilBits));

        // --- Creates OpenGL Rendering Context --- //

        m_rendering_context_handle = wglCreateContext(m_device_context_handle);
        if (!m_rendering_context_handle) LogFatalError(std::string() + "Error TOGL::Window::Create: Can not create OpenGl Rendering Context. (windows error code:" + std::to_string(GetLastError()) + ")");

        if (!wglMakeCurrent(m_device_context_handle, m_rendering_context_handle)) {
            LogFatalError(std::string() + "Error TOGL::Window::Create: Can not set created OpenGl Rendering Context to be current.");
        }

        if ((m_data.opengl_verion.major == DEF || m_data.opengl_verion.minor == DEF) && m_data.opengl_verion.major != m_data.opengl_verion.minor) {
            LogFatalError(std::string() + "Error TOGL::Window::Create: Incorrect OpenGL version is provided.");
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
                    LogFatalError(std::string() + "Error TOGL::Window::Create: Can not create OpenGl Rendering Context for version " + std::to_string(m_data.opengl_verion.major) + "." + std::to_string(m_data.opengl_verion.minor) + ".");
                }

                if (!wglMakeCurrent(m_device_context_handle, rendering_context_handle)) {
                    LogFatalError(std::string() + "Error TOGL::Window::Create: Can not set created OpenGl Rendering Context for version " + std::to_string(m_data.opengl_verion.major) + "." + std::to_string(m_data.opengl_verion.minor) + " to be current.");
                }

                m_rendering_context_handle = rendering_context_handle ;

            } else {
                LogFatalError("Error TOGL::Window::Create: Can not load wglCreateContextAttribsARB function.");
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

        LogInfo(std::string("(TOGL) OpenGl Version: ") + (const char*)glGetString(GL_VERSION));
    }

    inline void Window::Destroy() {
        if (m_data.info_level > 1) {
            puts("TOGLW::Window::Destroy"); 
            fflush(stdout);
        }

        m_data.do_on_destroy();

        wglMakeCurrent(NULL, NULL); 
        wglDeleteContext(m_rendering_context_handle);
        ReleaseDC(m_window_handle, m_device_context_handle);
        PostQuitMessage(0);
    }

    inline void Window::Paint() {
        if (m_data.info_level > 2) {
            puts("TOGLW::Window::Paint"); 
            fflush(stdout);
        }

        Display();

        ValidateRect(m_window_handle, NULL); // to decrease number of WM_PAINT messages
    }

    inline void Window::Display() {
        if (m_data.info_level > 3) {
            puts("TOGLW::Window::Display"); 
            fflush(stdout);
        }

        m_data.display();

        SwapBuffers(m_device_context_handle);
    }

    inline void Window::Close() {
        if (m_data.info_level > 1) {
            puts("TOGLW::Window::Close"); 
            fflush(stdout);
        }

        DestroyWindow(m_window_handle);
    }

}

#endif // TRIVIALOPENGL_WINDOW_H_
