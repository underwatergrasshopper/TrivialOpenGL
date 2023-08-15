/**
* @file UnicodeText.cpp
* @author underwatergrasshopper
*/

#include "UnicodeText.h"

#include <stdio.h>
#include <string>
#include <TrivialOpenGL.h>

enum : uint16_t {
    FONT_SIZE = 16,   // in pixels
};

static TOGL_SizeU16 s_size  = {800, 400};
static std::string  s_text  = ""; 

static void Resize(uint16_t width, uint16_t height) {
    glViewport(0, 0, width, height);

    // When window size changes, text size stays same.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, 1, -1);

    s_size = {width, height};
}

int RunUnicodeText() {
    TOGL_Data data = {};

    data.window_name        = "Unicode Text";
    data.area               = {0, 0, s_size.width, s_size.height};
    // Ignores data.area.x and data.area.y coordinates 
    // and centers window in middle of work area (screen area excluding task bar).
    data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
    // Instead setting window size with data.area.width and data.area.height, 
    // sets window draw area size with data.area.width and data.area.height.
    data.style              |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE;

    data.do_on_create = []() {
        TOGL_SizeU16 size = TOGL_GetDrawAreaSize();
        Resize(size.width, size.height);

        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

        // Sets draw area for drawing text. 
        // Origin of coordinates system is at left-bottom corner of draw area.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, s_size.width, 0, s_size.height, 1, -1);

        // Temporal font for displaying load message.
        puts("Loading \"Courier New\" for English...");
        fflush(stdout);

        TOGL_LoadFont(
            "Courier New", 
            FONT_SIZE, 
            TOGL_FONT_SIZE_UNIT_ID_PIXELS, 
            TOGL_FONT_STYLE_ID_NORMAL, 
            TOGL_FONT_CHAR_SET_ID_ENGLISH
        );

        if (!TOGL_IsFontOk()) {
            printf("Error: %s.", TOGL_GetFontErrMsg().c_str());
            fflush(stdout);
        } else {
            puts("Loaded \"Courier New\" for English.");
            fflush(stdout);
        }

        s_text = "Wait. Loading unicode font (Plane 0) ...";
        TOGL_DrawNow();

        // Loading actual unicode font.
        puts("Loading \"Courier New\" for Unicode Plane 0 (0000h-FFFFh)...");
        fflush(stdout);

        TOGL_LoadFont(
            "Courier New", 
            FONT_SIZE, 
            TOGL_FONT_SIZE_UNIT_ID_PIXELS, 
            TOGL_FONT_STYLE_ID_NORMAL, 
            TOGL_FONT_CHAR_SET_ID_RANGE_0000_FFFF
        );

        if (!TOGL_IsFontOk()) {
            printf("Error: %s.", TOGL_GetFontErrMsg().c_str());
            fflush(stdout);
        } else {
            puts("Loaded \"Courier New\" for Unicode Plane 0 (0000h-FFFFh).");
            fflush(stdout);
        }

        s_text = u8"Unicode text (Plane 0): \u0444\u3400\u5016\u9D9B.\nX - Exit.";

        // ---

        puts("X - Exit");
        fflush(stdout);
    };

    data.do_on_destroy = []() {
        TOGL_UnloadFont();

        puts("Bye. Bye.");
        fflush(stdout);
    };

    data.do_on_resize = [](uint16_t width, uint16_t height) {
        Resize(width, height);
    };

    data.draw = []() {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        TOGL_RenderText({0, s_size.height - FONT_SIZE}, {255, 255, 255, 255}, s_text);
    };

    data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
        if (key_id == 'X' && !is_down) {
            TOGL_RequestClose();
        }
    };

    return TOGL_Run(data);
}