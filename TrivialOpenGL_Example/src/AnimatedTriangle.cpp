/**
* @file AnimatedTriangle.cpp
* @author underwatergrasshopper
*/

#include "AnimatedTriangle.h"

#include <windows.h>
#include <stdio.h>
#include <TrivialOpenGL.h>


namespace {     
    // Elapsed Time
    LARGE_INTEGER           s_frequency;    // in performance ticks
    LARGE_INTEGER           s_begin;        // in performance ticks
    LARGE_INTEGER           s_end;          // in performance ticks
    double                  s_time;         // in seconds

    // Triangle
    double                  s_angle;        // in degrees
}

void ResetTime() {
    QueryPerformanceFrequency(&s_frequency); 
    QueryPerformanceCounter(&s_begin);
    s_end = s_begin;
}

// min_time - in seconds
bool UpdateTime(double min_time) {
    QueryPerformanceCounter(&s_end);

    s_time = double(s_end.QuadPart - s_begin.QuadPart) / s_frequency.QuadPart;
    if (s_time < min_time) {
        s_time = 0;
        return false;
    } else {
        s_begin = s_end;
        return true;
    }
}

// Returns time in seconds
double GetTime() {
    return s_time;
}

int RunAnimatedTriangle() {
    TOGL_Data data = {};

    data.window_name        = "Animated Triangle";
    data.area               = {0, 0, 800, 400};
    // Ignores data.area.x and data.area.y coordinates 
    // and centers window in middle of work area (screen area excluding task bar).
    data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
    // Instead setting window size with data.area.width and data.area.height, 
    // sets window draw area size with data.area.width and data.area.height.
    data.style              |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE;

    data.do_on_create = []() {
        auto size = TOGL_GetDrawAreaSize();
        glViewport(0, 0, size.width, size.height);

        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

        ResetTime();

        puts("X - Exit");
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
        // Update
        if (UpdateTime(0.01)) {
            // Triangle will rotate at speed of 10 degrees per second.
            s_angle += GetTime() * 10;
            s_angle = fmod(s_angle, 360.0);
        }

        // Draw
        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();
        glRotated(s_angle, 0, 0, 1);

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

    data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
        if (key_id == 'X' && !is_down) {
            TOGL_RequestClose();
        }
    };

    return TOGL_Run(data);
}