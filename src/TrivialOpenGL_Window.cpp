/**
* @file TrivialOpenGL_Window.cpp
* @author underwatergrasshopper
*/

#include "../include/TrivialOpenGL_Window.h"


namespace TrivialOpenGL {
    //--------------------------------------------------------------------------
    // Run
    //--------------------------------------------------------------------------

    int Window::Run(const Data& data) {
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
        if (m_data.style & STYLE_BIT_NO_RESIZE)     m_window_style &= ~WS_THICKFRAME;
        if (m_data.style & STYLE_BIT_NO_MAXIMIZE)   m_window_style &= ~WS_MAXIMIZEBOX;
        if (m_data.style & STYLE_BIT_DRAW_AREA_ONLY) {
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
    
} // namespace TrivialOpenGL