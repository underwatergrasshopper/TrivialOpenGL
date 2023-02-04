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
    GetWindowRect(TOGL::ToWindow().GetHWND(), &window);
    GetClientRect(TOGL::ToWindow().GetHWND(), &client);
    MapWindowPoints(TOGL::ToWindow().GetHWND(), NULL, (LPPOINT)&client, 2);

    border.left     = client.left - window.left;
    border.top      = client.top - window.top;
    border.right    = window.right - client.right;
    border.bottom   = window.bottom - client.bottom;

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

    togl_print_i32(TOGL::GetScreenSize().width);
    togl_print_i32(TOGL::GetScreenSize().height);
    togl_print_i32(TOGL::GetDesktopAreaSizeNoTaskBar().width);
    togl_print_i32(TOGL::GetDesktopAreaSizeNoTaskBar().height);
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

    ForceFlag("MOVE_AND_RESIZE");

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
        data.style              = 0;
        data.style              |= TOGL::StyleBit::DRAW_AREA_SIZE;
        //data.style              |= TOGL::StyleBit::CLIENT_ONLY;
        //data.style              |= TOGL::StyleBit::REDRAW_ON_REQUEST_ONLY;
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

                case 'X':       window.MarkToClose(); break;

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

                case 'X':       window.MarkToClose(); break;

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

            TOGL::ToWindow().MarkToRedraw();
        };

        return TOGL::Run(data);

    } else if (IsFlag("WINDOW_STATE")) {
        s_resolution = {600, 300};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example MOVE_AND_RESIZE";
        // data.style              = TOGL::StyleBit::CLIENT_SIZE;
        // data.style              = TOGL::StyleBit::CLIENT_ONLY;
        data.style              = TOGL::StyleBit::REDRAW_ON_REQUEST_ONLY;
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


                
            case 'Z': TOGL::ToWindow().MoveTo({0, 0}); break;
            case 'M': TOGL::ToWindow().MoveTo({10, 100}); break;
            case 'S': TOGL::ToWindow().SetSize({400, 200}); break;
            case 'B': TOGL::ToWindow().SetSize({800, 400}); break;
            case 'R': TOGL::ToWindow().SetArea({100, 10, 800, 400}); break;
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

            case '7':
                break;

            case 'I': {
                RECT client, window, border;
                GetWindowRect(TOGL::ToWindow().GetHWND(), &window);
                GetClientRect(TOGL::ToWindow().GetHWND(), &client);
                MapWindowPoints(TOGL::ToWindow().GetHWND(), NULL, (LPPOINT)&client, 2);

                border.left     = client.left - window.left;
                border.top      = client.top - window.top;
                border.right    = window.right - client.right;
                border.bottom   = window.bottom - client.bottom;

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
            }
        };

        data.do_on_size = [](uint32_t width, uint32_t height) {
            glViewport(0, 0, width,  height ? height : 1);

            glLoadIdentity();
            glOrtho(0, width, 0, height, 1, -1);

            s_resolution = TOGL::SizeI(width, height);

            TOGL::ToWindow().MarkToRedraw();
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

    } else if (IsFlag("FULL_SCREEN")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example FULL_SCREEN";
        //data.style              = TOGL::StyleBit::CLIENT_ONLY | TOGL::StyleBit::CLIENT_SIZE;// | TOGL::StyleBit::REDRAW_ON_REQUEST_ONLY;
        //data.style              = TOGL::StyleBit::REDRAW_ON_REQUEST_ONLY;
        //data.area               = {TOGL::DEF, TOGL::DEF, 600, 300};
        data.area               = TOGL::GetDesktopAreaNoTaskBar();
        data.info_level         = 3;

        struct TestResolution {
            int width, height;
        };
        TOGL::Static<TestResolution>::To() = {600, 300};

        data.do_on_create = []() {
            glLoadIdentity();
            glOrtho(0, 600, 0, 300, 1, -1);
            glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

            //TOGL::ToWindow().MakeFullScreen(); 
            //TOGL::ToWindow().MarkToRedraw();
        };

        data.display = []() {
            glClear(GL_COLOR_BUFFER_BIT);

            auto DrawRectangle = [](int x, int y, int width, int height) {
                glBegin(GL_TRIANGLE_FAN);
                glVertex2i(x, y);
                glVertex2i(x + width, y);
                glVertex2i(x + width, y + height);
                glVertex2i(y, y + height);
                glEnd();
            };

            auto& size = TOGL::Static<TestResolution>::To();

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
            case 'Z': TOGL::ToWindow().MoveTo({0, 0}); break;
            case 'M': TOGL::ToWindow().MoveTo({10, 100}); break;
            case 'S': TOGL::ToWindow().SetSize({400, 200}); break;
            case 'B': TOGL::ToWindow().SetSize({800, 400}); break;
            case 'R': TOGL::ToWindow().SetArea({100, 10, 800, 400}); break;
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
            case 'V': TOGL::ToWindow().MarkToRedraw(); break;

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

            case '7':
                TOGL::ToWindow().SetArea(0, 0, TOGL::GetDesktopAreaSizeNoTaskBar().width, TOGL::GetDesktopAreaSizeNoTaskBar().height);
                break;
            }
        };

        data.do_on_size = [](uint32_t width, uint32_t height) {
            glViewport(0, 0, width,  height ? height : 1);

            glLoadIdentity();
            glOrtho(0, width, 0, height, 1, -1);

            TOGL::Static<TestResolution>::To() = {(int)width, (int)height};
            TOGL::ToWindow().MarkToRedraw();
        
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