/**
* @file BorderlessWindow.cpp
* @author underwatergrasshopper
*/

#include "BorderlessWindow.h"

#include <stdio.h>
#include <TrivialOpenGL.h>

int RunBorderlessWindow() {
    TOGL_Data data = {};

    data.window_name        = "Borderless Window";
    data.area               = {0, 0, 800, 400};
    // Ignores data.area.x and data.area.y coordinates 
    // and centers window in middle of work area (screen area excluding task bar).
    data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
    // Window will have draw area only (no borders, no title bar).
    data.style              |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_ONLY;

    data.do_on_create = []() {
        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

        puts("X or Escape - Exit");
        fflush(stdout);
    };

    data.do_on_destroy = []() {
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
        if ((key_id == 'X' || key_id == TOGL_KEY_IDESCAPE) && !is_down) {
            TOGL_RequestClose();
        }
    };

    return TOGL_Run(data);
}