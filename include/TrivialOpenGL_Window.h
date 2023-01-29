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

    struct Data {
        // Encoding: ASCII or UTF8.
        std::string     window_name         = "Window";

        // If x is DEF then created window will be centered on X axis.
        // If y is DEF then created window will be centered on Y axis.
        // If width is DEF then created window will have with equal to half width of the screen.
        // If height is DEF then created window will have with equal to half height of the screen.
        AreaI           area                = {DEF, DEF, DEF, DEF};

        StyleBit::Field style               = 0;

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

    };

    class Window {
    public:
        Window() {
            SingletonCheck();

            m_data                      = {};

            m_instance_handle           = NULL;
            m_window_handle             = NULL;
            m_device_context_handle     = NULL;
            m_rendering_context_handle  = NULL;

            m_window_style              = 0;
            m_window_extended_style     = 0;

            m_is_enqueued_to_destroy        = false;
        }

        virtual ~Window() {}

        // Moves window to position in screen coordinates.
        void MoveTo(int x, int y) {
            SetWindowPos(m_window_handle, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        }
        void MoveTo(const PointI& pos) {
            MoveTo(pos.x, pos.y);
        }

        // Resizes window and keeps current window position.
        void Resize(int width, int height) {
            SetWindowPos(m_window_handle, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER);
        }
        void Resize(const SizeI& size) {
            Resize(size.width, size.height);
        }

        // Resizes window draw area and keeps current window position.
        // width        Window draw area width.
        // height       Window draw area height.
        void ResizeDrawArea(int width, int height) {
            const SizeI window_size = GetWindowAreaFromDrawArea(AreaI(0, 0, width, height), m_window_style).GetSize();

            SetWindowPos(m_window_handle, 0, 0, 0, window_size.width, window_size.height, SWP_NOMOVE | SWP_NOOWNERZORDER);
        }
        void ResizeDrawArea(const SizeI& draw_area_size) {
            ResizeDrawArea(draw_area_size.width, draw_area_size.height);
        }

        // Moves and resizes window area.
        void MoveToAndResize(int x, int y, int width, int height) {
            SetWindowPos(m_window_handle, 0, x, y, width, height, SWP_NOOWNERZORDER);
        }
        void MoveToAndResize(const AreaI& area) {
            MoveToAndResize(area.x, area.y, area.width, area.height);
        }

        // Puts window in top most position in z-order.
        void BringToTop() {
            BringWindowToTop(m_window_handle);
        }

        // Centers window in desktop area excluding task bar area.
        void Center() {
            const AreaI desktop_area = GetMonitorDesktopAreaNoTaskBar();

            AreaI window_area = GetWindowArea(m_window_handle);

            window_area.x = (desktop_area.width - window_area.width) / 2;
            window_area.y = (desktop_area.height - window_area.height) / 2;

            MoveTo(window_area.GetPoint());
        }

        // Changes area by applying style from data parameter which was provided to Run function.
        void ChangeArea(const AreaI& area) {
            MoveToAndResize(GenerateWindowArea(area));
        }

        int Run(const Data& data) {
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

            if (m_data.do_on_create) m_data.do_on_create();

            ShowWindow(m_window_handle, SW_SHOW);
            UpdateWindow(m_window_handle);

            return ExecuteMainLoop();
        }

        void MarkToClose() {
            m_is_enqueued_to_destroy = true;
        }

        uint32_t GetDebugLevel() const { return m_data.info_level; }

    private:
        static LRESULT CALLBACK WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

        void SingletonCheck() {
            static bool is_instance_exists = false;
            if (is_instance_exists) {
                LogFatalError("Error TOGL::Window::SingletonCheck: Can't be more than one instance of Window class.");
            }
            is_instance_exists = true;
        }

        int ExecuteMainLoop() {
            MSG msg = {};

            if (m_data.style & StyleBit::REDRAW_ON_REQUEST_ONLY) {
                while (GetMessageW(&msg, NULL, 0, 0)) {
                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                }
                if (msg.message == WM_QUIT) return (int)msg.wParam;
                DestroyWindowIfEnqueued();

            } else {
                while (true) {
                    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
                        if (msg.message == WM_QUIT) return (int)msg.wParam;

                        TranslateMessage(&msg);
                        DispatchMessageW(&msg);
                    }
                    Display();
                    DestroyWindowIfEnqueued();
                }
            }

            return EXIT_FAILURE;
        }

        void DestroyWindowIfEnqueued() {
            if (m_is_enqueued_to_destroy) {
                DestroyWindow(m_window_handle);
                m_is_enqueued_to_destroy = false;
            }
        }

        HICON TryLoadIcon() {
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

        static AreaI GetMonitorDesktopAreaNoTaskBar() {
            RECT rc;
            SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
            return MakeArea(rc);
        }

        static AreaI GetWindowArea(HWND window_handle) {
            RECT r;
            if (GetWindowRect(window_handle, &r)) {
                return MakeArea(r);
            }
            return AreaI(0, 0, 0, 0);
        }

        AreaI GenerateWindowArea(const AreaI& area) {
            AreaI window_area = area;

            const AreaI desktop_area = GetMonitorDesktopAreaNoTaskBar();

            // === Solve Size === //

            if (area.width == DEF)  window_area.width     = desktop_area.width / 2;
            if (area.height == DEF) window_area.height    = desktop_area.height / 2;

            if ((m_data.style & StyleBit::CLIENT_SIZE) && !(m_data.style & StyleBit::CLIENT_ONLY)) {
                window_area.SetSize(GetWindowAreaFromDrawArea(window_area, m_window_style).GetSize());

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

        static AreaI GetWindowAreaFromDrawArea(const AreaI& draw_area, DWORD window_style) {
            RECT rect = MakeRECT(draw_area);
            AdjustWindowRect(&rect, window_style, FALSE);
            return MakeArea(rect);
        }

        void Create(HWND window_handle) {
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

            LogInfo(std::string() + "Info TOGLW Pixel Format:"
                " Red:"     + std::to_string(pfd.cRedBits) + 
                " Green:"   + std::to_string(pfd.cGreenBits) + 
                " Blue:"    + std::to_string(pfd.cBlueBits) + 
                " Alpha:"   + std::to_string(pfd.cAlphaBits) + 
                " Depth:"   + std::to_string(pfd.cDepthBits) + 
                " Stencil:" + std::to_string(pfd.cStencilBits));
            // ---

            m_rendering_context_handle = wglCreateContext(m_device_context_handle);
            if (!m_rendering_context_handle) LogFatalError(std::string() + "Error TOGL::Window::Create: Can not create opengl rendering context. (windows error code:" + std::to_string(GetLastError()) + ")");

            wglMakeCurrent(m_device_context_handle, m_rendering_context_handle);
        }

        void Destroy() {
            if (m_data.info_level > 1) {
                puts("TOGLW::Window::Destroy"); 
                fflush(stdout);
            }

            if (m_data.do_on_destroy) m_data.do_on_destroy();

            wglMakeCurrent(NULL, NULL); 
            wglDeleteContext(m_rendering_context_handle);
            ReleaseDC(m_window_handle, m_device_context_handle);
            PostQuitMessage(0);
        }

        void Paint() {
            if (m_data.info_level > 2) {
                puts("TOGLW::Window::Paint"); 
                fflush(stdout);
            }

            Display();

            ValidateRect(m_window_handle, NULL); // to decrease number of WM_PAINT messages
        }

        void Display() {
            if (m_data.info_level > 3) {
                puts("TOGLW::Window::Display"); 
                fflush(stdout);
            }

            if (m_data.display) m_data.display();

            SwapBuffers(m_device_context_handle);
        }

        void Close() {
            if (m_data.info_level > 1) {
                puts("TOGLW::Window::Close"); 
                fflush(stdout);
            }

            DestroyWindow(m_window_handle);
        }

        Data        m_data;

        HINSTANCE   m_instance_handle;
        HWND        m_window_handle;
        HDC         m_device_context_handle;
        HGLRC       m_rendering_context_handle;

        DWORD       m_window_style;
        DWORD       m_window_extended_style;

        bool        m_is_enqueued_to_destroy;
    };

    inline Window& ToWindow() {
        return Static<Window>::To();
    }

    inline int Run(const Data& data = {}) {
        return ToWindow().Run(data);
    }

} // namespace TrivialOpenGL


namespace TrivialOpenGL {
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
                if (ToWindow().m_data.do_on_key_down_raw) ToWindow().m_data.do_on_key_down_raw(w_param, l_param);
                return 0;

            case WM_KEYUP:
                if (ToWindow().m_data.do_on_key_up_raw) ToWindow().m_data.do_on_key_up_raw(w_param, l_param);
                return 0;

            case WM_ERASEBKGND:
                // Tells DefWindowProc to not erase background. It's unnecessary since background is handled by OpenGL.
                return 1;
        }
        return DefWindowProcW(window_handle, message, w_param, l_param);
    }
}

#endif // TRIVIALOPENGL_WINDOW_H_
