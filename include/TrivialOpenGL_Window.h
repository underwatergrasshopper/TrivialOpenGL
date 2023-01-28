/**
* @file TrivialOpenGL.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_WINDOW_H_
#define TRIVIALOPENGL_WINDOW_H_

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
            CLIENT_ONLY             = 0x000F,
        };
    };

    struct Data {
        // Encoding: ASCII or UTF8.
        std::string     window_name     = "Window";

        // If x is DEF then created window will be centered on X axis.
        // If y is DEF then created window will be centered on Y axis.
        // If width is DEF then created window will have with equal to half width of the screen.
        // If height is DEF then created window will have with equal to half height of the screen.
        AreaI           area            = {DEF, DEF, DEF, DEF};

        StyleBit::Field style           = 0;

        // Is called after creation of OpenGL Rendering Context and before calling display function.
        void (*create)()                = nullptr;

        // Is called right before destruction of OpenGL Rendering Context.
        void (*destroy)()               = nullptr;

        // Is called each time when window content needs to be redrawn.
        void (*display)()               = nullptr;
    };

    class Window {
    public:
        Window() {
            SingletonCheck();

            m_data              = {};
            m_instance_handle   = NULL;
            m_window_handle     = NULL;
        }
        virtual ~Window() {}

        static LRESULT CALLBACK WindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
            switch (message) {
            case WM_CREATE: 

                return 0;
            
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

            case WM_CLOSE:
                DestroyWindow(window_handle);
                return 0;

            case WM_PAINT:
                return 0;

            case WM_ERASEBKGND:
                // Tells DefWindowProc to not erase background. It's unnecessery since background is handled by OpenGL.
                return 1;
            }
            return DefWindowProc(window_handle, message, w_param, l_param);
        }

        int Run(const Data& data) {
            m_data = data;

            m_instance_handle = GetModuleHandleW(NULL);

            constexpr wchar_t WINDOW_CLASS_NAME[] = L"TrivialOpenGL_WindowClass";

            WNDCLASSEXW wc = {};
            wc.cbSize           = sizeof(WNDCLASSEXW);
            wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wc.lpfnWndProc      = WindowProc;
            wc.cbClsExtra       = 0;
            wc.cbWndExtra       = 0;
            wc.hInstance        = m_instance_handle;
            wc.hIcon            = NULL;
            // wc.hIcon            = (HICON)LoadImage(
            //     NULL,
            //     _T("icon.ico"),
            //     IMAGE_ICON,
            //     0, 0,
            //     LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED
            // );
            wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
            wc.hbrBackground    = NULL;
            wc.lpszMenuName     = NULL;
            wc.lpszClassName    = WINDOW_CLASS_NAME;
            wc.hIconSm          = NULL;

            if (!RegisterClassExW(&wc)) {
                LogFatalError("Error TOGLW::Window::Run: Cannot create window class.");
            }

            m_window_handle = CreateWindowExW(
                0,
                WINDOW_CLASS_NAME,
                ToUTF16(m_data.window_name).c_str(),
                WS_OVERLAPPEDWINDOW,
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

            MSG msg = {};
            while (GetMessageW(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }

            return (int)msg.wParam;
        }
    private:
        void SingletonCheck() {
            static bool is_instance_exists = false;
            if (is_instance_exists) {
                LogFatalError("Error TOGL::Window::Window: Can't be more than one instance of Window class.");
            }
            is_instance_exists = true;
        }

        Data        m_data;
        HINSTANCE   m_instance_handle;
        HWND        m_window_handle;
    };

    inline Window& ToWindow() {
        return Static<Window>::To();
    }

    inline int Run(const Data& data = {}) {
        return ToWindow().Run(data);
    }

} // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_WINDOW_H_
