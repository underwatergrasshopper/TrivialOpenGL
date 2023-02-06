#include <stdio.h>

#include <string>
#include <set>

#include <TrivialOpenGL.h>
#include "Resource.h"

//------------------------------------------------------------------------------

static TOGL::SizeI  s_resolution;
static bool         s_is_client;

void DrawRectangle(int x, int y, int width, int height) {
    glBegin(GL_TRIANGLE_FAN);

    glVertex2i(x,           y);
    glVertex2i(x + width,   y);
    glVertex2i(x + width,   y + height);
    glVertex2i(y,           y + height);

    glEnd();
};

void DisplayWindowInfo() {
    RECT client, window, border;

    HWND window_handle = GetForegroundWindow();

    GetWindowRect(window_handle, &window);
    GetClientRect(window_handle , &client);
    MapWindowPoints(window_handle, NULL, (LPPOINT)&client, 2);

    border.left     = client.left - window.left;
    border.top      = client.top - window.top;
    border.right    = window.right - client.right;
    border.bottom   = window.bottom - client.bottom;

    puts("---");

    togl_print_i32(client.left);
    togl_print_i32(client.top);
    togl_print_i32(client.right);
    togl_print_i32(client.bottom);

    togl_print_i32(window.left);
    togl_print_i32(window.top);
    togl_print_i32(window.right);
    togl_print_i32(window.bottom);

    togl_print_i32(border.left);
    togl_print_i32(border.top);
    togl_print_i32(border.right);
    togl_print_i32(border.bottom);
    puts("");

    const TOGL::SizeI screen_size = TOGL::GetScreenSize();
    togl_print_i32(screen_size.width);
    togl_print_i32(screen_size.height);

    const TOGL::SizeI work_area_size = TOGL::GetDesktopAreaSizeNoTaskBar();
    togl_print_i32(work_area_size.width);
    togl_print_i32(work_area_size.height);
    puts("");

    const TOGL::AreaI window_area = TOGL::ToWindow().GetArea();
    togl_print_i32(window_area.x);
    togl_print_i32(window_area.y);
    togl_print_i32(window_area.width);
    togl_print_i32(window_area.height);
    puts("");

    const TOGL::AreaI window_draw_area = TOGL::ToWindow().GetDrawArea();
    togl_print_i32(window_draw_area.x);
    togl_print_i32(window_draw_area.y);
    togl_print_i32(window_draw_area.width);
    togl_print_i32(window_draw_area.height);
    puts("");

    puts("Window Center Check:");
    togl_print_i32(window_area.width + (window_area.x * 2));
    togl_print_i32(window_area.height + (window_area.y * 2));
    togl_print_i32(work_area_size.width);
    togl_print_i32(work_area_size.height);
}

//------------------------------------------------------------------------------

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

    //ForceFlag("MOVE_AND_RESIZE");
    ForceFlag("WINDOW_STATE");
    //ForceFlag("OPENGL_VERSION");

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
        data.style              = TOGL::StyleBit::DRAW_AREA_SIZE;
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);

    } else if (IsFlag("CLIENT_ONLY")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example CLIENT_ONLY";
        data.area               = {TOGL::DEF, TOGL::DEF, 400, 200};
        data.style              = TOGL::StyleBit::DRAW_AREA_ONLY;
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);
    } else if (IsFlag("REDRAW_ON_CHANGE_OR_REQUEST")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example REDRAW_ON_CHANGE_OR_REQUEST";
        data.style              = TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        data.icon_resource_id   = ICON_ID;
        data.info_level         = 3;

        return TOGL::Run(data);

    } else if (IsFlag("DRAW_TRIANGLE")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example DRAW_TRIANGLE";
        data.style              = TOGL::StyleBit::DRAW_AREA_SIZE;
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

    } else if (IsFlag("MOVE_AND_RESIZE")) {

        s_resolution = {600, 300};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example MOVE_AND_RESIZE";
        //data.style              |= TOGL::StyleBit::DRAW_AREA_SIZE;
        //data.style              |= TOGL::StyleBit::DRAW_AREA_ONLY;
        //data.style              |= TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        //data.style              |= TOGL::StyleBit::NO_RESIZE;
        //data.style              |= TOGL::StyleBit::NO_MAXIMIZE;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        data.icon_resource_id   = ICON_ID;
        data.info_level         = 3;

        data.do_on_create = []() {
            glLoadIdentity();
            glOrtho(0, s_resolution.width, 0, s_resolution.height, 1, -1);
            glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        };

        data.display = []() {
            glClear(GL_COLOR_BUFFER_BIT);

            const TOGL::SizeI& size = s_resolution;

            glColor3f(1, 0, 0);
            DrawRectangle(0, 0, size.width, size.height);
            glColor3f(0, 0, 0.5);
            DrawRectangle(1, 1, size.width - 2, size.height - 2);

            glPushMatrix();

            glTranslatef(size.width / 2.0f, size.height / 2.0f, 0);
            glScalef(100, 100, 1);

            glBegin(GL_TRIANGLES);

            glColor3f(1, 0, 0);
            glVertex2f(-0.5, -0.5);

            glColor3f(0, 1, 0);
            glVertex2f(0.5, -0.5);

            glColor3f(0, 0, 1);
            glVertex2f(0, 0.5);

            glEnd();

            glPopMatrix();
        };

        data.do_on_key_up_raw = [](WPARAM w_param, LPARAM l_param) {
            auto& window = TOGL::ToWindow();

            if (!s_is_client) {
                switch (w_param) {
                case '1':       window.MoveTo(0, 0); break;
                case '2':       window.MoveTo(10, 100); break;

                case '3':       window.SetSize(400, 200); break;
                case '4':       window.SetSize(800, 400); break;
                     
                case '5':       window.SetArea(100, 10, 800, 400); break;
                case '6':       window.SetArea(window.GetArea()); break;
                case '7':       window.SetArea(TOGL::GetDesktopAreaNoTaskBar()); break;
                case '8':       window.SetArea({{}, TOGL::GetScreenSize()}); break;
                     
                case '9':       window.ChangeArea({100, 10, 800, 400}); break;

                case 'C':       window.Center(); break;

                case VK_LEFT:   window.MoveBy(-30, 0); break;
                case VK_RIGHT:  window.MoveBy(30, 0); break;

                case 'X':       window.RequestClose(); break;

                case 'I':       DisplayWindowInfo(); break;

                case 'T':       s_is_client = !s_is_client; break;
                }
            } else {
                switch (w_param) {
                case '1':       window.SetDrawAreaPos(0, 0); break;
                case '2':       window.SetDrawAreaPos(10, 100); break;
                                
                case '3':       window.SetDrawAreaSize(400, 200); break;
                case '4':       window.SetDrawAreaSize(800, 400); break;
                                
                case '5':       window.SetDrawArea(100, 10, 800, 400); break;
                case '6':       window.SetDrawArea(window.GetDrawArea()); break;
                case '7':       window.SetDrawArea(TOGL::GetDesktopAreaNoTaskBar()); break;
                case '8':       window.SetDrawArea({{}, TOGL::GetScreenSize()}); break;
                                
                case '9':       window.ChangeArea({100, 10, 800, 400}); break;
                                
                case 'C':       window.Center(); break;

                case VK_LEFT:   window.MoveBy(-30, 0); break;
                case VK_RIGHT:  window.MoveBy(30, 0); break;

                case 'X':       window.RequestClose(); break;

                case 'I':       DisplayWindowInfo(); break;

                case 'T':       s_is_client = !s_is_client; break;
                }
            }
        };

        data.do_on_size = [](uint32_t width, uint32_t height) {
            glViewport(0, 0, width,  height ? height : 1);

            glLoadIdentity();
            glOrtho(0, width, 0, height, 1, -1);

            s_resolution = TOGL::SizeI(width, height);

            TOGL::ToWindow().RequestRedraw();
        };

        return TOGL::Run(data);

    } else if (IsFlag("WINDOW_STATE")) {
        s_resolution = {600, 300};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example WINDOW_STATE";
        //data.style              |= TOGL::StyleBit::DRAW_AREA_SIZE;
        data.style              |= TOGL::StyleBit::DRAW_AREA_ONLY;
        //data.style              |= TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        //data.style              |= TOGL::StyleBit::NO_RESIZE;
        //data.style              |= TOGL::StyleBit::NO_MAXIMIZE;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        data.icon_resource_id   = ICON_ID;
        data.info_level         = 3;

        data.do_on_create = []() {
            glLoadIdentity();
            glOrtho(0, s_resolution.width, 0, s_resolution.height, 1, -1);
            glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        };

        data.display = []() {
            glClear(GL_COLOR_BUFFER_BIT);

            const TOGL::SizeI& size = s_resolution;

            glColor3f(1, 0, 0);
            DrawRectangle(0, 0, size.width, size.height);
            glColor3f(0, 0, 0.5);
            DrawRectangle(1, 1, size.width - 2, size.height - 2);

            glPushMatrix();

            glTranslatef(size.width / 2.0f, size.height / 2.0f, 0);
            glScalef(100, 100, 1);

            glBegin(GL_TRIANGLES);

            glColor3f(1, 0, 0);
            glVertex2f(-0.5, -0.5);

            glColor3f(0, 1, 0);
            glVertex2f(0.5, -0.5);

            glColor3f(0, 0, 1);
            glVertex2f(0, 0.5);

            glEnd();

            glPopMatrix();
        };

        data.do_on_key_up_raw = [](WPARAM w_param, LPARAM l_param) {
            switch (w_param) {

            case 'X': TOGL::ToWindow().RequestClose(); break;
            case 'R': TOGL::ToWindow().RequestRedraw(); break;
            case 'T': 
                Sleep(3000);
                TOGL::ToWindow().Top(); 
                break;
                
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
                TOGL::ToWindow().MakeWindowedFullScreen(); 
                break;

            case '6':
                TOGL::ToWindow().Hide();
                Sleep(1000);
                TOGL::ToWindow().MakeWindowedFullScreen(); 
                break;

            case '7':
                TOGL::ToWindow().SetArea(100, 50, 300, 600);
                break;

            case '8': {
                DEVMODEW dev_mode = {};
                dev_mode.dmSize = sizeof(DEVMODEW);

                dev_mode.dmDisplayFixedOutput   = DMDFO_DEFAULT; // DMDFO_CENTER, DMDFO_STRETCH, DMDFO_DEFAULT
                dev_mode.dmPelsWidth            = 640; 
                dev_mode.dmPelsHeight           = 480; 
                // only specific resolutions will work
                //dev_mode.dmPelsWidth            = 1280; 
                //dev_mode.dmPelsHeight           = 720; 
                dev_mode.dmFields               = DM_DISPLAYFIXEDOUTPUT | DM_PELSWIDTH | DM_PELSHEIGHT;

                // ... or ...
                //if (!EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, &dev_mode)) puts("Can not get Display Settings.");

                LONG result = ChangeDisplaySettingsW(&dev_mode, CDS_FULLSCREEN);
                togl_print_i32(result);

                break;
            }

            case '9':
                ChangeDisplaySettingsW(NULL, CDS_RESET);
                break;

            case 'I': 
                DisplayWindowInfo(); 
                break;
            }
        };

        data.do_on_size = [](uint32_t width, uint32_t height) {
            glViewport(0, 0, width,  height ? height : 1);

            glLoadIdentity();
            glOrtho(0, width, 0, height, 1, -1);

            s_resolution = TOGL::SizeI(width, height);

            TOGL::ToWindow().RequestRedraw();
        };

        return TOGL::Run(data);

    } else if (IsFlag("OPENGL_VERSION")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example OPENGL_VERSION";
        data.opengl_verion      = {3, 3};
        data.info_level         = 3;

        data.do_on_create = []() {
            auto version = TOGL::ToWindow().GetOpenGL_Version();
            printf("%d.%d\n", version.major, version.minor);

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

    } else {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example";
        data.icon_resource_id   = ICON_ID;
        data.info_level         = 3;

        return TOGL::Run(data);
    }

}