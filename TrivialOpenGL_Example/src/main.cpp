/**
* @file TrivialOpenGL.h
* @author underwatergrasshopper
*/

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

void DrawInfoText(const std::string& text) {
    const auto list = TOGL::Split(text, '\n');

    int y = TOGL::ToWindow().GetDrawAreaSize().height;
    for (const auto& line : list) {
        y -= TOGL::ToWindow().GetTextSize(line).height;
        TOGL::ToWindow().RenderText(10, y, 255, 255, 255, 255, line);
    }
}

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

class DelayedActionManager {
public:
    DelayedActionManager() {}
    virtual ~DelayedActionManager() {}

    using RunFnP_T = void (*)();

    // delay - in seconds
    void Add(double delay, RunFnP_T run) {
        m_actions.push_back({delay, run});
        m_time_lapse.Reset();
    }

    void Run() {
        m_time_lapse.Update();

        if (!m_actions.empty()) {
            Action& action = m_actions[0];

            action.delay -= m_time_lapse.Get();
            if (action.delay <= 0.0) {
                action.run();
                m_actions.erase(m_actions.begin());
            }
        }
    }
private:
    struct Action {
        double      delay;
        RunFnP_T    run;
    };

    std::vector<Action>     m_actions;
    TimeLapseF64            m_time_lapse;
};

//------------------------------------------------------------------------------

enum {
    FONT_SIZE = 16, // in pixels
};

static TOGL::Window&    s_window                    = TOGL::ToWindow();
static TOGL::SizeU16    s_resolution;
static bool             s_is_client;
static TestImage        s_test_image;
static bool             s_is_display_mose_move_data;

static DelayedActionManager s_actions;

//------------------------------------------------------------------------------



class ExampleManager {
public:
    using RunExampleFnP_T = int (*)(const std::string& name, const std::set<std::string>& options);

    ExampleManager() : m_buffer{} {}
    virtual ~ExampleManager() {}

    void SetDefaultExample(const std::string& name) {
        m_default_example_name = name;
    }

    void AddExample(const std::string& name, const std::set<std::string>& all_options, const std::set<std::string>& default_options, RunExampleFnP_T run) {
        m_examples.push_back({name, all_options, default_options, run});
    }

    int Run(const std::vector<std::string>& arguments) {
        int result = 0;

        if (arguments.size() > 0) {
            std::string name = arguments[0];

            std::set<std::string> options;
            for (size_t index = 1; index < arguments.size(); ++index) options.insert(arguments[index]);

            Example* example = Find(name);
            if (!example) {
                puts("");
                printf("Example Error: Can not find example with name \"%s\".\n", name.c_str());
                result = EXIT_FAILURE;
            } else {
                result = example->run(name, options);
            }
        } else {
            result = Run();
        }

        return result;
    }

    int Run() {
        int result = 0;

        while (true) {
            puts("");
            puts("--- Example Manager ---");
            for (const auto& example : m_examples) printf("%s, ", example.name.c_str());
            printf("%s", "\n\n");

            printf("(e=exit, d=%s)\n", m_default_example_name.c_str());
            printf("%s", "Example: ");
            fflush(stdout);

            std::string name;

            if (!GetStdIn(name)) {
                puts("");
                puts("Example Error: Can not read argument.");
                result = EXIT_FAILURE;
                break;
            } 
            puts("");

            if (name == "d") {
                name = m_default_example_name;
            } else if (name == "e") {
                result = EXIT_SUCCESS;
                break;
            } 

            Example* example = Find(name);
            if (!example) {
                printf("Example Error: Can not find example with name \"%s\".\n", name.c_str());
                continue;
            }

            for (const auto& option : example->all_options) printf("%s, ", option.c_str());
            printf("%s", "\n\n");

            std::string raw_default_options;
            for (const auto& option : example->default_options) raw_default_options += option + " ";

            printf("(e=exit, d=%s)\n", raw_default_options.c_str());
            printf("%s", "Options: ");
            fflush(stdout);

            std::string raw_options;

            if (!GetStdIn(raw_options)) {
                puts("");
                puts("Example Error: Can not read arguments.");
                continue;
            } 
            puts("");

            if (raw_options == "d") {
                raw_options = raw_default_options;
            } else if (raw_options == "e") {
                result = EXIT_SUCCESS;
                break;
            } 

            std::set<std::string> options;
            for (const auto& option : RemoveEmpty(TOGL::Split(raw_options, ' '))) options.insert(option);

            auto IsContainAllIn = [](const std::set<std::string>& elements, const std::set<std::string>& container) -> std::string {
                for (const auto& element : elements) {
                    if (container.find(element) == container.end()) {
                        return element;
                    }
                }
                return "";
            };

            const std::string missing_option = IsContainAllIn(options, example->all_options);
            if (!missing_option.empty()) {
                printf("Example Error: Unknown option \"%s\".\n", missing_option.c_str());
                continue;
            }

            result = example->run(name, options);
            if (result != EXIT_SUCCESS) {
                printf("Example Error: Example returned error code %d.\n", result);
            }
        }

        return result;
    }


private:
    enum { BUFFER_SIZE = 4096 };

    struct Example {
        std::string             name;
        std::set<std::string>   all_options;
        std::set<std::string>   default_options;

        RunExampleFnP_T         run;
    };

    // Removes empty strings.
    std::vector<std::string> RemoveEmpty(std::vector<std::string> poluted_arguments) {
        std::vector<std::string> arguments;
        for (const auto& argument : poluted_arguments) if (!argument.empty()) arguments.push_back(argument);
        return arguments;
    }

    Example* Find(const std::string& name) {
        for (auto& example : m_examples) {
            if (example.name == name) return &example;
        }
        return nullptr;
    }

    bool GetStdIn(std::string& in_text) {
        memset(m_buffer, 0, sizeof(m_buffer));

        if (!fgets(m_buffer, BUFFER_SIZE - 1, stdin)) {
            puts("Example Error: Can not read arguments.");

            in_text = "";
            return false;
        }

        // Removes new line character from string end.
        size_t size = strlen(m_buffer);
        if (size > 0 && m_buffer[size - 1] == '\n') m_buffer[size - 1] = '\0';

        in_text = std::string(m_buffer);
        return true;
    }

    std::vector<Example>    m_examples;
    std::string             m_default_example_name;
    char                    m_buffer[BUFFER_SIZE];
};


//------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    std::vector<std::string> arguments;
    for (size_t index = 1; index < argc; ++index) {
        arguments.push_back(argv[index]);
    }

    ExampleManager example_manager;

    example_manager.SetDefaultExample("window_state");

    ////////////////////////////////////////////////////////////////////////////////
    // icon
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("icon", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.icon_file_name     = "..\\..\\..\\..\\TrivialOpenGL_Example\\assets\\icon.ico";

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // icon_resource
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("icon_resource", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // custom_size
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("custom_size", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, 400, 800};

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // custom_area
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("custom_area", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {100, 200, 400, 800};

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // no_area
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("no_area", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, 0, 0};

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // no_resize
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("no_resize", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.style              = TOGL::StyleBit::NO_RESIZE;

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // no_maximize
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("no_maximize", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.style              = TOGL::StyleBit::NO_MAXIMIZE;

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // no_maximize_and_resize
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("no_maximize_and_resize", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.style              = TOGL::StyleBit::NO_MAXIMIZE | TOGL::StyleBit::NO_RESIZE;

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // center
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("center", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, 400, 800};
        data.style              = TOGL::StyleBit::CENTERED;

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // draw_area_size
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("draw_area_size", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, 400, 200};
        data.style              = TOGL::StyleBit::DRAW_AREA_SIZE;

        return TOGL::Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // draw_area_only
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("draw_area_only", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, 400, 200};
        data.style              = TOGL::StyleBit::DRAW_AREA_ONLY;

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // redraw_on_change_or_request
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("redraw_on_change_or_request", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.style              = TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        data.log_level          = 3;

        return TOGL::Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // draw_triangle
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("draw_triangle", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.icon_resource_id   = ICON_ID;
        data.log_level          = 3;

        data.do_on_create = []() {
            glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            glViewport(0, 0, width, height);
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
    });

    ////////////////////////////////////////////////////////////////////////////////
    // move_and_resize
    ////////////////////////////////////////////////////////////////////////////////

    const std::set<std::string> all_options = {
        "draw_area_size", "draw_area_only", "redraw_on_change_or_request", "no_resize", "no_maximize",
        "notify_any_message", "notify_draw_call", "notify_mouse_move ", "notify_key_message", "notify_character_message", "no_debug"
    };

    example_manager.AddExample("move_and_resize", all_options, {}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {600, 300};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        data.icon_resource_id   = ICON_ID;

        if (!IsOption("no_debug"))                      data.log_level = TOGL::LOG_LEVEL_DEBUG;

        if (IsOption("draw_area_size"))                 data.style |= TOGL::StyleBit::DRAW_AREA_SIZE;
        if (IsOption("draw_area_only"))                 data.style |= TOGL::StyleBit::DRAW_AREA_ONLY;
        if (IsOption("redraw_on_change_or_request"))    data.style |= TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        if (IsOption("no_resize"))                      data.style |= TOGL::StyleBit::NO_RESIZE;
        if (IsOption("no_maximize"))                    data.style |= TOGL::StyleBit::NO_MAXIMIZE;

        if (IsOption("notify_any_message"))             data.special_debug.is_notify_any_message        = true;
        if (IsOption("notify_draw_call"))               data.special_debug.is_notify_draw_call          = true;
        if (IsOption("notify_mouse_move"))              data.special_debug.is_notify_mouse_move         = true;
        if (IsOption("notify_key_message"))             data.special_debug.is_notify_key_message        = true;
        if (IsOption("notify_character_message"))       data.special_debug.is_notify_character_message  = true;

        data.do_on_create = []() {
            s_test_image.Initialize(TOGL::ToWindow().GetDrawAreaSize());

            if (!TOGL::ToWindow().LoadFont("Courier New", FONT_SIZE)) {
                puts("Error: Can not load font.");
            } else {
                puts("Font loaded.");
            }
        };

        data.draw = []() {
            s_test_image.Animate();

            DrawInfoText(
                std::string() +
                "1 - MoveTo(0, 0)\n"
                "2 - MoveTo(10, 100)\n"
                "3 - SetSize(400, 200)\n"
                "4 - SetSize(800, 400)\n"
                "5 - SetArea(100, 10, 800, 400)\n"
                "6 - SetArea(window_area/draw_area)\n"
                "7 - SetArea(desktop_area_no_taskbar)\n"
                "8 - SetArea(screen_area)\n"
                "C - Center(" + std::to_string(s_resolution.width) + ", " + std::to_string(s_resolution.width) + ")\n"
                "Arrow Left     - MoveBy(-30, 0)\n"
                "Arrow Right    - MoveBy(30, 0)\n"
                "X - RequestClose()\n"
                "I - Display Window Info\n"
                "T - Swap target window_area/draw_area\n"
            );
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

                    case TOGL::KEY_ID_C:            window.Center(s_resolution, s_is_client); break;

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

                    case TOGL::KEY_ID_C:            window.Center(s_resolution, s_is_client); break;

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

    });

    ////////////////////////////////////////////////////////////////////////////////
    // window_state
    ////////////////////////////////////////////////////////////////////////////////

    auto Combine = [](const std::set<std::string>& l, const std::set<std::string>& r) {
        std::set<std::string> temp = l;
        temp.insert(r.begin(), r.end());
        return temp;
    };

    std::set<std::string> window_state_all_options = Combine(all_options, {"full_screen_at_start"});

    example_manager.AddExample("window_state", window_state_all_options, {"draw_area_size"}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {800, 400};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        data.icon_resource_id   = ICON_ID;

        if (!IsOption("no_debug"))                      data.log_level = TOGL::LOG_LEVEL_DEBUG;

        if (IsOption("draw_area_size"))                 data.style |= TOGL::StyleBit::DRAW_AREA_SIZE;
        if (IsOption("draw_area_only"))                 data.style |= TOGL::StyleBit::DRAW_AREA_ONLY;
        if (IsOption("redraw_on_change_or_request"))    data.style |= TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        if (IsOption("no_resize"))                      data.style |= TOGL::StyleBit::NO_RESIZE;
        if (IsOption("no_maximize"))                    data.style |= TOGL::StyleBit::NO_MAXIMIZE;

        if (IsOption("notify_any_message"))             data.special_debug.is_notify_any_message        = true;
        if (IsOption("notify_draw_call"))               data.special_debug.is_notify_draw_call          = true;
        if (IsOption("notify_mouse_move"))              data.special_debug.is_notify_mouse_move         = true;
        if (IsOption("notify_key_message"))             data.special_debug.is_notify_key_message        = true;
        if (IsOption("notify_character_message"))       data.special_debug.is_notify_character_message  = true;

        if (IsOption("full_screen_at_start")) {
            data.do_on_create = []() {
                s_test_image.Initialize(TOGL::ToWindow().GetDrawAreaSize());

                TOGL::ToWindow().GoWindowedFullScreen();

                if (!TOGL::ToWindow().LoadFont("Courier New", FONT_SIZE)) {
                    puts("Error: Can not load font.");
                } else {
                    puts("Font loaded.");
                }
            };
        } else {
            data.do_on_create = []() {
                s_test_image.Initialize(TOGL::ToWindow().GetDrawAreaSize());

                if (!TOGL::ToWindow().LoadFont("Courier New", FONT_SIZE)) {
                    puts("Error: Can not load font.");
                } else {
                    puts("Font loaded.");
                }
            };
        }

        data.draw = []() {
            s_test_image.Animate();

            DrawInfoText(
                std::string() +
                "X - Exit\n"
                "R - Redraw\n"
                "C - Center(" + std::to_string(s_resolution.width) + ", " + std::to_string(s_resolution.width) + ") (as window size)\n"
                "V - Center(" + std::to_string(s_resolution.width) + ", " + std::to_string(s_resolution.width) + ") (as draw area size)\n"
                "F - Background -> Foreground -> Minimize -> Center -> Hide -> Show (wait until finish)\n"
                "Arrows - Move\n"
                "0 - Hide -> Show -> Hide -> SetSize -> Hide -> Minimize -> Hide -> Maximize (wait until finish)\n"
                "1 - Hide -> Show\n"
                "2 - Minimize -> Center\n"
                "W - Minimize -> MoveBy (should not restore from minimize)\n"
                "3 - Center(" + std::to_string(s_resolution.width) + ", " + std::to_string(s_resolution.width) + ")\n"
                "4 - Maximize\n"
                "5 - GoWindowedFullScreen\n"
                "6 - Hide -> GoWindowedFullScreen\n"
                "7 - SetArea(100, 50, 300, 600)\n"
                "8 - Native Full Screen (experimental)\n"
                "9 - restore from Native Full Screen (experimental)\n"
                "I - Display Info\n"
                "D - on/off TOGL Debug\n"
            );

            s_actions.Run();
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            s_test_image.Resize(width, height);

            std::string message = "Resize: ";
            message += std::to_string(width) + " " + std::to_string(height);
            puts(message.c_str());
        };

        data.do_on_state_change = [](TOGL::WindowState window_state) {
            std::string message = "Changed Window State to: ";

            switch (window_state) {
            case TOGL::WindowState::NORMAL:                 message += "NORMAL"; break;
            case TOGL::WindowState::MAXIMIZED:              message += "MAXIMIZED"; break;
            case TOGL::WindowState::MINIMIZED:              message += "MINIMIZED"; break;
            case TOGL::WindowState::WINDOWED_FULL_SCREENED: message += "WINDOWED_FULL_SCREENED"; break;
            }

            puts(message.c_str());
        };

        data.do_on_show = []() {
            puts("On Show");
        };

        data.do_on_hide = []() {
            puts("On Hide");
        };

        data.do_on_foreground = [](bool is_gain) {
            puts(is_gain ? "On Foreground Gain" : "On Foreground Lose");
        };

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            if (!is_down) {

                switch (key_id) {

                case 'X':               TOGL::ToWindow().RequestClose(); break;
                case TOGL::KEY_ID_R:    TOGL::ToWindow().RequestRedraw(); break;
                case TOGL::KEY_ID_C:    TOGL::ToWindow().Center(s_resolution, false); break;
                case TOGL::KEY_ID_V:    TOGL::ToWindow().Center(s_resolution, true); break;

                case 'F':
                    s_actions.Add(0, [](){
                        // Moves behind window to foreground.
                        SetForegroundWindow(GetNextWindow(GetForegroundWindow(), GW_HWNDNEXT));
                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL::ToWindow().GoForeground();
                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL::ToWindow().Minimize();
                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL::ToWindow().Center(s_resolution);
                        //TOGL::ToWindow().GoForeground(); // redundant
                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL::ToWindow().Hide();
                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL::ToWindow().Show();
                        //TOGL::ToWindow().GoForeground(); // redundant
                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        puts("Finished");
                    });
                    break;

                case TOGL::KEY_ID_ARROW_LEFT:
                    TOGL::ToWindow().MoveBy(-30, 0);
                    break;

                case TOGL::KEY_ID_ARROW_RIGHT:
                    TOGL::ToWindow().MoveBy(30, 0);
                    break;

                case TOGL::KEY_ID_ARROW_UP:
                    TOGL::ToWindow().MoveBy(0, -30);
                    break;

                case TOGL::KEY_ID_ARROW_DOWN:
                    TOGL::ToWindow().MoveBy(0, 30);
                    break;

                case TOGL::KEY_ID_0: 
                    s_actions.Add(0, [](){
                        puts("--- Show --");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();

                    });

                    s_actions.Add(1, [](){

                        puts("---");
                        TOGL::ToWindow().Show(); 
                        PrintWindowStates();
                    });

                    s_actions.Add(1, [](){

                        puts("--- Restore ---");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();
                    });

                    s_actions.Add(1, [](){

                        puts("---");
                        TOGL::ToWindow().SetSize(s_resolution); 
                        PrintWindowStates();
                    });

                    s_actions.Add(1, [](){

                        puts("--- Minimize ---");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();
                    });

                    s_actions.Add(1, [](){

                        puts("---");
                        TOGL::ToWindow().Minimize(); 
                        PrintWindowStates();

                    });

                    s_actions.Add(1, [](){

                        puts("--- Maximize ---");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();

                    });

                    s_actions.Add(1, [](){

                        puts("---");
                        TOGL::ToWindow().Maximize(); 
                        PrintWindowStates();

                        puts("Finished");
                    });
                    break;


                case '1':
                    s_actions.Add(0, [](){
                        puts("---");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();

                        togl_print_i32(TOGL::ToWindow().GetArea().x);
                        togl_print_i32(TOGL::ToWindow().GetArea().y);
                        togl_print_i32(TOGL::ToWindow().GetArea().width);
                        togl_print_i32(TOGL::ToWindow().GetArea().height);

                        togl_print_i32(TOGL::ToWindow().IsForeground());
                    });

                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL::ToWindow().Show(); 

                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        PrintWindowStates();
                    });
                    break;

                case TOGL::KEY_ID_2: 
                    s_actions.Add(0, [](){
                        puts("---");
                        TOGL::ToWindow().Minimize();
                        PrintWindowStates();

                        togl_print_i32(TOGL::ToWindow().GetArea().x);
                        togl_print_i32(TOGL::ToWindow().GetArea().y);
                        togl_print_i32(TOGL::ToWindow().GetArea().width);
                        togl_print_i32(TOGL::ToWindow().GetArea().height);
                    });

                    s_actions.Add(1, [](){
                        puts("---");

                        TOGL::ToWindow().Center(s_resolution);
                        PrintWindowStates();
                    });
                    break;

                case TOGL::KEY_ID_W: 
                    s_actions.Add(0, [](){
                        puts("---");
                        TOGL::ToWindow().Minimize();
                        PrintWindowStates();

                        togl_print_i32(TOGL::ToWindow().GetArea().x);
                        togl_print_i32(TOGL::ToWindow().GetArea().y);
                        togl_print_i32(TOGL::ToWindow().GetArea().width);
                        togl_print_i32(TOGL::ToWindow().GetArea().height);
                    });

                    s_actions.Add(1, [](){
                        TOGL::ToWindow().MoveBy(30, 0);
                        PrintWindowStates();
                    });
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
                    s_actions.Add(0, [](){
                        puts("---");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();
                    });

                    s_actions.Add(1, [](){
                        TOGL::ToWindow().GoWindowedFullScreen(); 
                        PrintWindowStates();
                    });
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

    });

    ////////////////////////////////////////////////////////////////////////////////
    // opengl_version
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("opengl_version", {"wrong"}, {}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        if (IsOption("wrong")) {
            data.opengl_verion      = {3, 7};
        } else {
            data.opengl_verion      = {3, 3};
        }
        data.log_level          = TOGL::LOG_LEVEL_DEBUG;

        data.do_on_create = []() {
            auto version = TOGL::ToWindow().GetOpenGL_Version();
            printf("%d.%d\n", version.major, version.minor);

            glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            glViewport(0, 0, width, height);
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

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            if (!is_down && key_id == 'X') TOGL::ToWindow().RequestClose();
        };

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // keyboard_and_mouse
    ////////////////////////////////////////////////////////////////////////////////
    
    example_manager.AddExample("keyboard_and_mouse", all_options, {}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {600, 300};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        data.icon_resource_id   = ICON_ID;

        if (!IsOption("no_debug"))                      data.log_level = TOGL::LOG_LEVEL_DEBUG;

        if (IsOption("draw_area_size"))                 data.style |= TOGL::StyleBit::DRAW_AREA_SIZE;
        if (IsOption("draw_area_only"))                 data.style |= TOGL::StyleBit::DRAW_AREA_ONLY;
        if (IsOption("redraw_on_change_or_request"))    data.style |= TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        if (IsOption("no_resize"))                      data.style |= TOGL::StyleBit::NO_RESIZE;
        if (IsOption("no_maximize"))                    data.style |= TOGL::StyleBit::NO_MAXIMIZE;

        if (IsOption("notify_any_message"))             data.special_debug.is_notify_any_message        = true;
        if (IsOption("notify_draw_call"))               data.special_debug.is_notify_draw_call          = true;
        if (IsOption("notify_mouse_move"))              data.special_debug.is_notify_mouse_move         = true;
        if (IsOption("notify_key_message"))             data.special_debug.is_notify_key_message        = true;
        if (IsOption("notify_character_message"))       data.special_debug.is_notify_character_message  = true;

        data.do_on_create = []() {
            s_test_image.Initialize(TOGL::ToWindow().GetDrawAreaSize());

            if (!TOGL::ToWindow().LoadFont("Courier New", FONT_SIZE)) {
                puts("Error: Can not load font.");
            } else {
                puts("Font loaded.");
            }
        };

        data.draw = []() {
            s_test_image.Animate();
            DrawInfoText(
                "S - Enable/Disable display mouse move data\n"
                "D - Switch Log Level Debug/Info\n"
                "I - Display Keyboard and Mouse State Info\n"
                "X - Exit\n"
            );
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
                case 'X':
                    TOGL::ToWindow().RequestClose();
                    break;

                case 'S':
                    s_is_display_mose_move_data = !s_is_display_mose_move_data;
                    if (s_is_display_mose_move_data)    {
                        puts("Enabled display mouse move data");
                    } else {
                        puts("Disabled display mouse move data");
                    } 

                    break;
                case 'D':
                    if (TOGL::ToWindow().GetLogLevel() == TOGL::LOG_LEVEL_DEBUG) {
                        TOGL::ToWindow().SetLogLevel(TOGL::LOG_LEVEL_INFO);
                        puts("Log Level: DEBUG -> INFO");
                    } else {
                        TOGL::ToWindow().SetLogLevel(TOGL::LOG_LEVEL_DEBUG);
                        puts("Log Level: INFO -> DEBUG");
                    }
                    break;

                case 'I': {
                    DisplayWindowInfo();

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

                default:
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

        data.do_on_char = [](char code) {
            printf("On Char UTF8: '%c' %02hhX\n", code, uint8_t((int)code));
        };

        data.do_on_char_utf16 = [](wchar_t code) {
            printf("On Char UTF16: %04hX\n", uint16_t((int)code));
        };

        data.do_on_char_utf32 = [](int code) {
            printf("On Char UTF32: %08X\n", uint32_t(code));
        };

        return TOGL::Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // timer
    ////////////////////////////////////////////////////////////////////////////////
    
    example_manager.AddExample("timer", {"100ms", "500ms", "1s"}, {"500ms"}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {600, 300};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        data.icon_resource_id   = ICON_ID;
        data.log_level          = TOGL::LOG_LEVEL_DEBUG;

        if (IsOption("100ms"))  data.timer_time_interval = 100;
        if (IsOption("500ms"))  data.timer_time_interval = 500;
        if (IsOption("1s"))     data.timer_time_interval = 1000;
        
        data.do_on_create = []() {
            s_test_image.Initialize(TOGL::ToWindow().GetDrawAreaSize());
        };

        data.draw = []() {
            s_test_image.Animate();
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            s_test_image.Resize(width, height);
        };

        data.do_on_time = [](uint32_t time_interval) {
            printf("Do On Time (%dms)\n", time_interval);
        };

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            if (key_id == 'X' && !is_down) TOGL::ToWindow().RequestClose();
        };

        return TOGL::Run(data);

    });

    //------------------------------------------------------------------------------

    return example_manager.Run(arguments);
}