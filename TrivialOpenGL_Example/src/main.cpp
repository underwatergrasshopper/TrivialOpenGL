#include <stdio.h>

#include <string>
#include <set>

#include <TimeLapse.h>

#include <TrivialOpenGL.h>

#include "Resource.h"

//------------------------------------------------------------------------------

void DrawRectangle(int x, int y, int width, int height) {
    glBegin(GL_TRIANGLE_FAN);

    glVertex2i(x,           y);
    glVertex2i(x + width,   y);
    glVertex2i(x + width,   y + height);
    glVertex2i(y,           y + height);

    glEnd();
};

void DrawTriangle(float x, float y, float scale, float angle) {
    glPushMatrix();

    glTranslatef(x, y, 0);
    glRotatef(angle, 0, 0, 1);
    glScalef(scale, scale, 1);

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

class TestImage {
public:
    TestImage() {
        m_interval      = 0.1;   
        m_speed         = 10;  

        m_step          = m_speed * m_interval;   

        m_accumulator   = 0; 
        m_angle         = 0;    
    }

    virtual ~TestImage() {

    }

    void Initialize(uint16_t width, uint16_t height) {
        Initialize({width, height});
    }

    void Initialize(const TOGL::SizeU16& size) {
        m_size = size;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, m_size.width, 0, m_size.height, 1, -1);

        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

        m_time_lapse.Reset();
    }

    // angle - in degrees
    void Draw(float triangle_angle = 0) {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // frame
        glColor3f(1, 0, 0);
        DrawRectangle(0, 0, m_size.width, m_size.height);

        // background
        glColor3f(0, 0, 0.5);
        DrawRectangle(1, 1, m_size.width - 2, m_size.height - 2);

        // triangle
        DrawTriangle(m_size.width / 2.0f, m_size.height / 2.0f, m_size.height / 3.0f, triangle_angle);
    }

    void Animate() {
        m_time_lapse.Update();
        m_accumulator += m_time_lapse.Get();

        if (m_accumulator > m_interval) {
            while (m_accumulator > m_interval) {
                m_angle += (int)m_step;
                m_accumulator -= m_interval;
            }
            m_angle %= 360;
        }

        Draw((float)m_angle);
    }

    void Resize(uint16_t width, uint16_t height) {
        glViewport(0, 0, width,  height ? height : 1);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, 0, height, 1, -1);

        m_size = {width, height};

        TOGL::ToWindow().RequestRedraw();
    }

private:
    TOGL::SizeU16   m_size;

    double          m_interval;         // in seconds
    double          m_speed;            // in degrees per second
    double          m_step;             // in degrees

    TimeLapseF64    m_time_lapse;
    double          m_accumulator;      // time in seconds
    int             m_angle;            // in degrees
};

void PrintWindowStates() {
    puts("NHMmFf");
    printf("%s%s%s%s%s%s\n", 
        TOGL::ToWindow().IsNormal() ? "+" : " ",
        TOGL::ToWindow().IsVisible() == false ? "+" : " ",
        TOGL::ToWindow().IsWindowMaximized() ? "+" : " ",
        TOGL::ToWindow().IsWindowMinimized() ? "+" : " ",
        " ",
        TOGL::ToWindow().IsWindowedFullScreen() ? "+" : " ");
}

void DisplayWindowInfo() {
    togl_print_i32(GetSystemMetrics(SM_CXMINTRACK));
    togl_print_i32(GetSystemMetrics(SM_CYMINTRACK));
    togl_print_i32(GetSystemMetrics(SM_CXMAXTRACK));
    togl_print_i32(GetSystemMetrics(SM_CYMAXTRACK));

    puts("---");

    RECT client, window, border;

    HWND window_handle = GetForegroundWindow();

    GetWindowRect(window_handle, &window);
    GetClientRect(window_handle , &client);
    MapWindowPoints(window_handle, NULL, (LPPOINT)&client, 2);

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
    puts("");

    const TOGL::SizeU16 screen_size = TOGL::GetScreenSize();
    togl_print_i32(screen_size.width);
    togl_print_i32(screen_size.height);

    const TOGL::SizeU16 work_area_size = TOGL::GetDesktopAreaSizeNoTaskBar();
    togl_print_i32(work_area_size.width);
    togl_print_i32(work_area_size.height);
    puts("");

    const TOGL::AreaIU16 window_area = TOGL::ToWindow().GetArea();
    togl_print_i32(window_area.x);
    togl_print_i32(window_area.y);
    togl_print_i32(window_area.width);
    togl_print_i32(window_area.height);
    puts("");

    const TOGL::AreaIU16 window_draw_area = TOGL::ToWindow().GetDrawArea();
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

    PrintWindowStates();
}

//------------------------------------------------------------------------------

static TOGL::SizeU16    s_resolution;
static bool             s_is_client;
static TestImage        s_test_image;
static bool             s_is_display_mose_move_data;

//------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    std::set<std::string> flags;

    for (size_t index = 1; index < argc; ++index) {
        flags.insert(argv[index]);
    }

    auto IsFlag = [&flags](const std::string& flag) { 
        return flags.find(flag) != flags.end(); 
    };

    auto ForceFlag = [&flags](const std::string& flag) { 
        flags.insert(flag);
    };

    if (flags.size() == 0) {
        //ForceFlag("MOVE_AND_RESIZE");
        ForceFlag("WINDOW_STATE");
        //ForceFlag("OPENGL_VERSION");

        //ForceFlag("KEYBOARD");
    }

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
        data.log_level         = 3;

        return TOGL::Run(data);

    } else if (IsFlag("DRAW_TRIANGLE")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example DRAW_TRIANGLE";
        data.style              = TOGL::StyleBit::DRAW_AREA_SIZE;
        data.icon_resource_id   = ICON_ID;
        data.log_level         = 3;

        data.do_on_create = []() {
            glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        };

        data.draw = []() {
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
        data.log_level         = TOGL::LOG_LEVEL_DEBUG;

        data.do_on_create = []() {
            s_test_image.Initialize(s_resolution);
        };

        data.draw = []() {
            s_test_image.Animate();
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            s_test_image.Resize(width, height);
        };

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            auto& window = TOGL::ToWindow();

            if (!is_down) {
                if (!s_is_client) {
                    switch (key_id) {
                    case TOGL::KEY_ID_1:            window.MoveTo(0, 0); break;
                    case TOGL::KEY_ID_2:            window.MoveTo(10, 100); break;

                    case TOGL::KEY_ID_3:            window.SetSize(400, 200); break;
                    case TOGL::KEY_ID_4:            window.SetSize(800, 400); break;
                     
                    case TOGL::KEY_ID_5:            window.SetArea(100, 10, 800, 400); break;
                    case TOGL::KEY_ID_6:            window.SetArea(window.GetArea()); break;
                    case TOGL::KEY_ID_7:            window.SetArea(TOGL::GetDesktopAreaNoTaskBar()); break;
                    case TOGL::KEY_ID_8:            window.SetArea({{}, TOGL::GetScreenSize()}); break;

                    case TOGL::KEY_ID_C:            window.Center(s_resolution); break;

                    case TOGL::KEY_ID_ARROW_LEFT:   window.MoveBy(-30, 0); break;
                    case TOGL::KEY_ID_ARROW_RIGHT:  window.MoveBy(30, 0); break;

                    case TOGL::KEY_ID_X:            window.RequestClose(); break;

                    case TOGL::KEY_ID_I:            DisplayWindowInfo(); break;

                    case TOGL::KEY_ID_T:            s_is_client = !s_is_client; break;
                    default: break;
                    }
                } else {
                    switch (key_id) {
                    case TOGL::KEY_ID_1:            window.MoveTo(0, 0, true); break;
                    case TOGL::KEY_ID_2:            window.MoveTo(10, 100, true); break;

                    case TOGL::KEY_ID_3:            window.SetSize(400, 200, true); break;
                    case TOGL::KEY_ID_4:            window.SetSize(800, 400, true); break;

                    case TOGL::KEY_ID_5:            window.SetArea(100, 10, 800, 400, false); break;
                    case TOGL::KEY_ID_6:            window.SetArea(window.GetDrawArea(), false); break;
                    case TOGL::KEY_ID_7:            window.SetArea(TOGL::GetDesktopAreaNoTaskBar(), false); break;
                    case TOGL::KEY_ID_8:            window.SetArea({{}, TOGL::GetScreenSize()}, false); break;

                    case TOGL::KEY_ID_C:            window.Center(s_resolution); break;

                    case TOGL::KEY_ID_ARROW_LEFT:   window.MoveBy(-30, 0); break;
                    case TOGL::KEY_ID_ARROW_RIGHT:  window.MoveBy(30, 0); break;

                    case TOGL::KEY_ID_X:            window.RequestClose(); break;

                    case TOGL::KEY_ID_I:            DisplayWindowInfo(); break;

                    case TOGL::KEY_ID_T:            s_is_client = !s_is_client; break;
                    default: break;
                    }
                }
            }
        };

        return TOGL::Run(data);

    } else if (IsFlag("WINDOW_STATE")) {
        s_resolution = {600, 300};

        const TOGL::SizeU16 screen_size = TOGL::GetScreenSize();

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example WINDOW_STATE";
        //data.style              |= TOGL::StyleBit::DRAW_AREA_SIZE;
        //data.style              |= TOGL::StyleBit::DRAW_AREA_ONLY;
        //data.style              |= TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        //data.style              |= TOGL::StyleBit::NO_RESIZE;
        //data.style              |= TOGL::StyleBit::NO_MAXIMIZE;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        //data.area               = {TOGL::DEF, TOGL::DEF, screen_size.width, screen_size.height};
        data.icon_resource_id   = ICON_ID;
        data.log_level          = TOGL::LOG_LEVEL_DEBUG;

        data.do_on_create = []() {
            s_test_image.Initialize(s_resolution);

            // TOGL::ToWindow().GoWindowedFullScreen();
        };

        data.draw = []() {
            s_test_image.Animate();
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            s_test_image.Resize(width, height);
        };

        data.do_on_state_change = [](TOGL::WindowState window_state) {
            std::string message = "Changed Window State to: ";

            switch (window_state) {
            case TOGL::WindowState::NORMAL: message += "NORMAL"; break;
            case TOGL::WindowState::MAXIMIZED: message += "MAXIMIZED"; break;
            case TOGL::WindowState::MINIMIZED: message += "MINIMIZED"; break;
            case TOGL::WindowState::WINDOWED_FULL_SCREENED: message += "WINDOWED_FULL_SCREENED"; break;
            }

            puts(message.c_str());
        };

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            if (!is_down) {

                switch (key_id) {

                case 'X':               TOGL::ToWindow().RequestClose(); break;
                case TOGL::KEY_ID_R:    TOGL::ToWindow().RequestRedraw(); break;
                case TOGL::KEY_ID_C:    TOGL::ToWindow().Center(s_resolution); break;
                case TOGL::KEY_ID_V:    TOGL::ToWindow().Center(s_resolution, true); break;

                case TOGL::KEY_ID_0: 
                    puts("--- Show --");
                    TOGL::ToWindow().Hide();
                    PrintWindowStates();
                
                    Sleep(1000);
                
                    puts("---");
                    TOGL::ToWindow().Show(); 
                    PrintWindowStates();
                
                    Sleep(1000);
                
                    puts("--- Restore ---");
                    TOGL::ToWindow().Hide();
                    PrintWindowStates();
                
                    Sleep(1000);
                
                    puts("---");
                    TOGL::ToWindow().SetSize(s_resolution); 
                    PrintWindowStates();
                
                    Sleep(1000);
                
                    puts("--- Minimize ---");
                    TOGL::ToWindow().Hide();
                    PrintWindowStates();
                
                    Sleep(1000);
                
                    puts("---");
                    TOGL::ToWindow().Minimize(); 
                    PrintWindowStates();
                
                    Sleep(1000);
                
                    puts("--- Maximize ---");
                    TOGL::ToWindow().Hide();
                    PrintWindowStates();
                
                    Sleep(1000);
                
                    puts("---");
                    TOGL::ToWindow().Maximize(); 
                    PrintWindowStates();

                    break;
        
                
                case '1':
                    puts("---");
                    TOGL::ToWindow().Hide();
                    PrintWindowStates();

                    togl_print_i32(TOGL::ToWindow().GetArea().x);
                    togl_print_i32(TOGL::ToWindow().GetArea().y);
                    togl_print_i32(TOGL::ToWindow().GetArea().width);
                    togl_print_i32(TOGL::ToWindow().GetArea().height);

                    Sleep(1000);

                    puts("---");
                    TOGL::ToWindow().Show(); 
                    //TOGL::ToWindow().Center(s_resolution);
                
                    PrintWindowStates();
                    break;

                case TOGL::KEY_ID_2: 
                    puts("---");
                    TOGL::ToWindow().Minimize();
                    PrintWindowStates();

                    togl_print_i32(TOGL::ToWindow().GetArea().x);
                    togl_print_i32(TOGL::ToWindow().GetArea().y);
                    togl_print_i32(TOGL::ToWindow().GetArea().width);
                    togl_print_i32(TOGL::ToWindow().GetArea().height);

                    Sleep(1000);

                    puts("---");
  
                    TOGL::ToWindow().Center(s_resolution);
                    PrintWindowStates();
                    break;

                case TOGL::KEY_ID_3:
                    puts("---");
                    TOGL::ToWindow().Center(s_resolution);
                    PrintWindowStates();
                    break;

                case TOGL::KEY_ID_4:
                    puts("---");
                    TOGL::ToWindow().Maximize(); 
                    PrintWindowStates();
                    break;

                case TOGL::KEY_ID_5:
                    puts("---");
                    TOGL::ToWindow().GoWindowedFullScreen(); 
                    PrintWindowStates();
                    break;

                case TOGL::KEY_ID_6:
                    puts("---");
                    TOGL::ToWindow().Hide();
                    PrintWindowStates();

                    Sleep(1000);

                    TOGL::ToWindow().GoWindowedFullScreen(); 
                    PrintWindowStates();
                    break;

                case TOGL::KEY_ID_7:
                    puts("---");
                    TOGL::ToWindow().SetArea(100, 50, 300, 600);
                    PrintWindowStates();
                    break;

                case TOGL::KEY_ID_8: {
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

                case TOGL::KEY_ID_9:
                    ChangeDisplaySettingsW(NULL, CDS_RESET);
                    break;

                case TOGL::KEY_ID_I: 
                    DisplayWindowInfo(); 
                    break;

                case TOGL::KEY_ID_D: 
                    if (TOGL::ToWindow().GetLogLevel() == TOGL::LOG_LEVEL_DEBUG) {
                        TOGL::ToWindow().SetLogLevel(TOGL::LOG_LEVEL_ERROR);
                    } else {
                        TOGL::ToWindow().SetLogLevel(TOGL::LOG_LEVEL_DEBUG);
                    }
                    break;
                default: break;
                }
            }
        };

        return TOGL::Run(data);

    } else if (IsFlag("OPENGL_VERSION")) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example OPENGL_VERSION";
        data.opengl_verion      = {3, 3};
        data.log_level         = 3;

        data.do_on_create = []() {
            auto version = TOGL::ToWindow().GetOpenGL_Version();
            printf("%d.%d\n", version.major, version.minor);

            glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        };

        data.draw = []() {
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

    } else if (IsFlag("KEYBOARD")) {
        s_resolution = {600, 300};

        const TOGL::SizeU16 screen_size = TOGL::GetScreenSize();

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example WINDOW_STATE";
        //data.style              |= TOGL::StyleBit::DRAW_AREA_SIZE;
        //data.style              |= TOGL::StyleBit::DRAW_AREA_ONLY;
        //data.style              |= TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        //data.style              |= TOGL::StyleBit::NO_RESIZE;
        //data.style              |= TOGL::StyleBit::NO_MAXIMIZE;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        //data.area               = {TOGL::DEF, TOGL::DEF, screen_size.width, screen_size.height};
        data.icon_resource_id   = ICON_ID;
        data.log_level          = TOGL::LOG_LEVEL_DEBUG;

        data.do_on_create = []() {
            s_test_image.Initialize(s_resolution);

            // TOGL::ToWindow().GoWindowedFullScreen();
        };

        data.draw = []() {
            s_test_image.Animate();
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            s_test_image.Resize(width, height);
        };

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            std::string message = "Key: ";

            message += TOGL::KeyIdToStr(key_id);
            message += is_down ? " down" : " up";
            message += TOGL::ExtraToStr(extra);

            puts(message.c_str());

            if (is_down) {
                switch (key_id) {
                case 'I': {
                    togl_print_i32(TOGL::IsKeyToggled(TOGL::KEY_ID_CAPS_LOCK));
                    togl_print_i32(TOGL::IsKeyToggled(TOGL::KEY_ID_INSERT));
                    togl_print_i32(TOGL::IsKeyToggled(TOGL::KEY_ID_NUMLOCK));

                    const TOGL::PointI cursor_pos_in_screen = TOGL::GetCursorPosInScreen();
                    togl_print_i32(cursor_pos_in_screen.x);
                    togl_print_i32(cursor_pos_in_screen.y);

                    const TOGL::PointI cursor_pos_in_draw_area = TOGL::ToWindow().GetCursorPosInDrawArea();
                    togl_print_i32(cursor_pos_in_draw_area.x);
                    togl_print_i32(cursor_pos_in_draw_area.y);
                    break;
                }

                case 'S':
                    s_is_display_mose_move_data = !s_is_display_mose_move_data;
                    if (s_is_display_mose_move_data)    puts("enabled display mouse move data");
                    if (!s_is_display_mose_move_data)   puts("disabled display mouse move data");
                    break;

                } // switch
            }
        };

        data.do_on_mouse_wheel_roll = [](int step_count, int x, int y) {
            std::string message = "Mouse Wheel:";

            message += " step_count=" + std::to_string(step_count);
            message += " x=" + std::to_string(x);
            message += " y=" + std::to_string(y);

            puts(message.c_str());
        };

        data.do_on_mouse_move = [](int x, int y) {
            if (s_is_display_mose_move_data) {
                std::string message = "Mouse Move:";

                message += " x=" + std::to_string(x);
                message += " y=" + std::to_string(y);

                puts(message.c_str());
            }
        };

        return TOGL::Run(data);

    } else {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example";
        data.icon_resource_id   = ICON_ID;
        data.log_level         = 3;

        return TOGL::Run(data);
    }

}