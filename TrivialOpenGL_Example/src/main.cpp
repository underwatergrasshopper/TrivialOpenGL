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

    ForceFlag("DRAW_TRIANGLE");

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
        data.style              = TOGL::StyleBit::CLIENT_ONLY;
        data.area               = {TOGL::DEF, TOGL::DEF, 600, 300};
        data.icon_resource_id   = ICON_ID;
        data.info_level         = 3;

        data.do_on_create = []() {
            glClearColor(0.0, 0.0, 0.2, 1.0);
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
                case 'P': TOGL::ToWindow().MoveTo({0, 0});   break;
                case 'M': TOGL::ToWindow().MoveTo({10, 100});   break;
                case 'S': TOGL::ToWindow().Resize({400, 200});  break;
                case 'B': TOGL::ToWindow().Resize({800, 400});  break;
                case 'R': TOGL::ToWindow().MoveToAndResize({100, 10, 800, 400});  break;
                case 'A': TOGL::ToWindow().ChangeArea({100, 10, 800, 400});  break;
                case 'C': TOGL::ToWindow().Center();            break;
                case 'T': TOGL::ToWindow().BringToTop();        break;
                case 'X': TOGL::ToWindow().MarkToClose();       break;
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