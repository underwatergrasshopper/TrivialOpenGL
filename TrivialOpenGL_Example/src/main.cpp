#include <stdio.h>

#include <string>
#include <set>

#include <TrivialOpenGL.h>
#include "Resource.h"

int main(int argc, char *argv[]) {
    std::set<std::string> flags;

    for (size_t index = 0; index < argc; ++index) {
        flags.insert(argv[index]);
    }

    auto IsFlag = [&flags](const std::string& flag) { 
        return flags.find(flag) != flags.end(); 
    };

    auto ForceFlag = [&flags](const std::string& flag) { 
        flags.insert(flag);
    };

    ForceFlag("WINDOW_FUNCTIONS");

    if (IsFlag("ICON")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ICON";
        data.icon_file_name     = "..\\..\\..\\..\\TrivialOpenGL_Example\\assets\\icon.ico";

        return TOGL::Run(data);
    } else if (IsFlag("ICON_RESOURCE")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ICON_RESOURCE";
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);
    } else if (IsFlag("CUSTOM_SIZE")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example CUSTOM_SIZE";
        data.area               = {TOGL::DEF, TOGL::DEF, 400, 800};
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);
    } else if (IsFlag("CUSTOM_AREA")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example CUSTOM_AREA";
        data.area               = {100, 200, 400, 800};
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);
    } else if (IsFlag("NO_AREA")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example NO_AREA";
        data.area               = {0, 0, 0, 0};
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);
    } else if (IsFlag("NO_RESIZE")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example NO_RESIZE";
        data.style              = TOGL::StyleBit::NO_RESIZE;
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);
    } else if (IsFlag("NO_MAXIMIZE")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example NO_MAXIMIZE";
        data.style              = TOGL::StyleBit::NO_MAXIMIZE;
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);

    } else if (IsFlag("NO_MAXIMIZE_NO_RESIZE")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example NO_MAXIMIZE";
        data.style              = TOGL::StyleBit::NO_MAXIMIZE | TOGL::StyleBit::NO_RESIZE;
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);

    } else if (IsFlag("CENTERED")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example CENTERED";
        data.area               = {0, 0, 400, 800};
        data.style              = TOGL::StyleBit::CENTERED;
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);

    } else if (IsFlag("CLIENT_SIZE")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example CLIENT_SIZE";
        data.area               = {TOGL::DEF, TOGL::DEF, 400, 200};
        data.style              = TOGL::StyleBit::CLIENT_SIZE;
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);

    } else if (IsFlag("CLIENT_ONLY")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example CLIENT_ONLY";
        data.area               = {TOGL::DEF, TOGL::DEF, 400, 200};
        data.style              = TOGL::StyleBit::CLIENT_ONLY;
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);
    } else if (IsFlag("REDRAW_ON_REQUEST_ONLY")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example REDRAW_ON_REQUEST_ONLY";
        data.style              = TOGL::StyleBit::REDRAW_ON_REQUEST_ONLY;
        data.icon_resource_id   = ICON_ID;
        data.info_level         = 3;

        return TOGL::Run(data);

    } else if (IsFlag("DRAW_TRIANGLE")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example DRAW_TRIANGLE";
        data.style              = TOGL::StyleBit::CLIENT_SIZE;
        data.icon_resource_id   = ICON_ID;
        data.info_level         = 3;

        data.do_on_create = []() {
            glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        };

        data.display = []() {
            glClear(GL_COLOR_BUFFER_BIT);

            glBegin(GL_TRIANGLES);

            glColor3f(1, 0, 0);
            glVertex2f(-0.5, -0.5);

            glColor3f(0, 1, 0);
            glVertex2f(0.5, -0.5);

            glColor3f(0, 0, 1);
            glVertex2f(0, 0.5);

            glEnd();

        };

        return TOGL::Run(data);

    } else if (IsFlag("WINDOW_FUNCTIONS")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example DRAW_TRIANGLE";
        // data.style              = TOGL::StyleBit::CLIENT_SIZE;
        // data.style              = TOGL::StyleBit::CLIENT_ONLY;
        data.area               = {TOGL::DEF, TOGL::DEF, 600, 300};
        data.icon_resource_id   = ICON_ID;
        data.info_level         = 3;

        data.do_on_create = []() {
            glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        };



        data.display = []() {
            glClear(GL_COLOR_BUFFER_BIT);

            glBegin(GL_TRIANGLES);

            glColor3f(1, 0, 0);
            glVertex2f(-0.5, -0.5);

            glColor3f(0, 1, 0);
            glVertex2f(0.5, -0.5);

            glColor3f(0, 0, 1);
            glVertex2f(0, 0.5);

            glEnd();



        };

        data.do_on_key_up_raw = [](WPARAM w_param, LPARAM l_param) {
            switch (w_param) {
            case 'Z': TOGL::ToWindow().MoveTo({0, 0}); break;
            case 'M': TOGL::ToWindow().MoveTo({10, 100}); break;
            case 'S': TOGL::ToWindow().Resize({400, 200}); break;
            case 'B': TOGL::ToWindow().Resize({800, 400}); break;
            case 'R': TOGL::ToWindow().MoveToAndResize({100, 10, 800, 400}); break;
            case 'A': TOGL::ToWindow().ChangeArea({100, 10, 800, 400}); break;
            case 'C': TOGL::ToWindow().Center(); break;
            case 'T': 
                Sleep(3000);
                TOGL::ToWindow().Top(); 
                break;
            case 'X': TOGL::ToWindow().MarkToClose(); break;
            case 'P': TOGL::ToWindow().MarkToRedraw(); break;
            case VK_LEFT: TOGL::ToWindow().MoveBy({-30, 0}); break;
            case VK_RIGHT: TOGL::ToWindow().MoveBy({30, 0}); break;

            case '1': 
                TOGL::ToWindow().Hide();
                Sleep(1000);
                TOGL::ToWindow().Restore(); 
                break;

            case '2': 
                TOGL::ToWindow().Minimize();
                Sleep(1000);
                TOGL::ToWindow().Restore(); 
                break;

            case '3':
                TOGL::ToWindow().Restore(); 
                break;

            case '4':
                TOGL::ToWindow().Maximize(); 
                break;

            case '5':
                TOGL::ToWindow().MakeFullScreen(); 
                break;

            case '6':
                TOGL::ToWindow().Hide();
                Sleep(1000);
                TOGL::ToWindow().MakeFullScreen(); 
                break;

            case 'I': {
                togl_print_i32(GetSystemMetrics(SM_CXSIZEFRAME));
                togl_print_i32(GetSystemMetrics(SM_CYSIZEFRAME));
                togl_print_i32(GetSystemMetrics(SM_CXFOCUSBORDER));
                togl_print_i32(GetSystemMetrics(SM_CYFOCUSBORDER));
                togl_print_i32(GetSystemMetrics(SM_CXPADDEDBORDER));
                // togl_print_i32(GetSystemMetrics(SM_CYPADDEDBORDER));
                togl_print_i32(GetSystemMetrics(SM_CXBORDER));
                togl_print_i32(GetSystemMetrics(SM_CYBORDER));
                togl_print_i32(GetSystemMetrics(SM_CXEDGE));
                togl_print_i32(GetSystemMetrics(SM_CYEDGE));
                togl_print_i32(GetSystemMetrics(SM_CXDLGFRAME));
                togl_print_i32(GetSystemMetrics(SM_CYDLGFRAME));

                togl_print_i32(GetSystemMetrics(SM_CYMENU));
                togl_print_i32(GetSystemMetrics(SM_CYCAPTION));

                NONCLIENTMETRICSW m;
                m.cbSize = sizeof(NONCLIENTMETRICSW);
                SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, 0, &m, 0);

                togl_print_i32(m.iBorderWidth);
                togl_print_i32(m.iPaddedBorderWidth);
                togl_print_i32(m.iScrollWidth);
                togl_print_i32(m.iScrollHeight);
                togl_print_i32(m.iCaptionWidth);
                togl_print_i32(m.iCaptionHeight);
                togl_print_i32(m.iSmCaptionWidth);
                togl_print_i32(m.iSmCaptionHeight);
                togl_print_i32(m.iMenuWidth);
                togl_print_i32(m.iMenuHeight);

                // window borders

                RECT client, window, border;
                GetWindowRect(TOGL::ToWindow().GetHWND(), &window);
                GetClientRect(TOGL::ToWindow().GetHWND(), &client);
                MapWindowPoints(TOGL::ToWindow().GetHWND(), NULL, (LPPOINT)&client, 2);

                border.left     = client.left - window.left;
                border.top      = client.top - window.top;
                border.right    = window.right - client.right;
                border.bottom   = window.bottom - client.bottom;

                togl_print_i32(window.left);
                togl_print_i32(window.top);
                togl_print_i32(window.right);
                togl_print_i32(window.bottom);

                togl_print_i32(border.left);
                togl_print_i32(border.top);
                togl_print_i32(border.right);
                togl_print_i32(border.bottom);

                // RECT win10_invisible_border = {7, 0, 7, 7};


                {
                    HMODULE lib_handle = LoadLibraryA("Dwmapi.dll");
                    if (lib_handle) {
                        HRESULT (*DwmGetWindowAttribute)(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute) = nullptr;

                        DwmGetWindowAttribute = (decltype(DwmGetWindowAttribute)) GetProcAddress(lib_handle, "DwmGetWindowAttribute");
                        if (DwmGetWindowAttribute) {

                            enum { DWMWA_EXTENDED_FRAME_BOUNDS = 9 };
                            RECT actual_window;

                            // Note: To return correct values, must be called after ShowWindow(window_handle, SW_SHOW).
                            DwmGetWindowAttribute(TOGL::ToWindow().GetHWND(), DWMWA_EXTENDED_FRAME_BOUNDS, &actual_window, sizeof(RECT));

                            togl_print_i32(actual_window.left);
                            togl_print_i32(actual_window.top);
                            togl_print_i32(actual_window.right);
                            togl_print_i32(actual_window.bottom);

                            togl_print_i32(actual_window.left - window.left);
                            togl_print_i32(actual_window.top - window.top);
                            togl_print_i32(actual_window.right - window.right);
                            togl_print_i32(actual_window.bottom - window.bottom);
                        }
                        FreeLibrary(lib_handle);
                    }
                }
                togl_print_i32(TOGL::GetScreenSize().width);
                togl_print_i32(TOGL::GetScreenSize().height);
                togl_print_i32(TOGL::ToWindow().GetArea().x);
                togl_print_i32(TOGL::ToWindow().GetArea().y);
                togl_print_i32(TOGL::ToWindow().GetArea().width);
                togl_print_i32(TOGL::ToWindow().GetArea().height);
                togl_print_i32(TOGL::ToWindow().GetDrawArea().x);
                togl_print_i32(TOGL::ToWindow().GetDrawArea().y);
                togl_print_i32(TOGL::ToWindow().GetDrawArea().width);
                togl_print_i32(TOGL::ToWindow().GetDrawArea().height);
                togl_print_i32(TOGL::ToWindow().GetDrawAreaSize().width);
                togl_print_i32(TOGL::ToWindow().GetDrawAreaSize().height);
                break;
            }
            };
        };

        return TOGL::Run(data);

    } else {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example";
        data.icon_resource_id   = ICON_ID;
        data.info_level         = 3;

        return TOGL::Run(data);
    }

}