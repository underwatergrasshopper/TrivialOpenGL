/**
* @file SimpleTriangle.cpp
* @author underwatergrasshopper
*/

#include "SimpleTriangle.h"

#include <stdio.h>
#include <TrivialOpenGL.h>

int RunSimpleTriangle() {
    TOGL_Data data = {};

    data.window_name        = "Simple Triangle";
    data.area               = {0, 0, 800, 400};
    // Ignores data.area.x and data.area.y coordinates 
    // and centers window in middle of work area (screen area excluding task bar).
    data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
    // Instead setting window size with data.area.width and data.area.height, 
    // sets window draw area size with data.area.width and data.area.height.
    data.style              |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE;

    data.do_on_create = []() {
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        auto size = TOGL_GetDrawAreaSize();
        glViewport(0, 0, size.width, size.height);

        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

        puts("X - Exit");
        fflush(stdout);
    };

    data.do_on_destroy = []() {
        glPopAttrib();

        puts("Bye. Bye.");
        fflush(stdout);
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

    data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
        if (key_id == 'X' && !is_down) {
            TOGL_RequestClose();
        }
    };

    return TOGL_Run(data);
}