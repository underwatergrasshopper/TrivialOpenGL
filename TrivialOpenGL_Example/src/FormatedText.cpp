/**
* @file FormatedText.cpp
* @author underwatergrasshopper
*/

#include "FormatedText.h"

#include <stdio.h>
#include <string>
#include <TrivialOpenGL.h>

enum : uint16_t {
    FONT_SIZE   = 16, // in pixels
    PADDING     = 10, // in pixels
};

static TOGL_SizeU16     s_size = {600, 600};
static TOGL_FineText    s_text;

static void DrawRectangle(int x, int y, int width, int height) {
    glBegin(GL_TRIANGLE_FAN);

    glVertex2i(x,           y);
    glVertex2i(x + width,   y);
    glVertex2i(x + width,   y + height);
    glVertex2i(x,           y + height);

    glEnd();
};

static void DrawRectangle(const TOGL_AreaI& area) {
    DrawRectangle(area.x, area.y, area.width, area.height);
};

int RunFormatedText() {
    TOGL_Data data = {};

    data.window_name        = "Formated Text";
    data.area               = {0, 0, s_size.width, s_size.height};
    // Ignores data.area.x and data.area.y coordinates 
    // and centers window in middle of work area (screen area excluding task bar).
    data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
    // Instead setting window size with data.area.width and data.area.height, 
    // sets window draw area size with data.area.width and data.area.height.
    data.style              |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE;

    data.do_on_create = []() {
        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

        // Sets draw area for drawing text. 
        // Origin of coordinates system is at left-bottom corner of draw area.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, s_size.width, 0, s_size.height, 1, -1);

        TOGL_LoadFont("Arial", FONT_SIZE, TOGL_FONT_SIZE_UNIT_ID_PIXELS, TOGL_FONT_STYLE_ID_NORMAL, TOGL_FONT_CHAR_SET_ID_ENGLISH);

        if (!TOGL_IsFontOk()) {
            printf("Error: %s.", TOGL_GetFontErrMsg().c_str());
            fflush(stdout);
        }

        // All tabs are replaced by provided number of spaces. 
        // Tabbed text is adjusted by counting from beginning of line.
        TOGL_SetNumberOfSpacesInTab(4);

        s_text = TOGL_FineText(
            TOGL_Color4U8(100, 200, 100, 255),
            "By changing width of window, text will be aligned to new width.\n",
            TOGL_Color4U8(200, 200, 200, 255),
            "Some sentence text. Some sentence text. Some sentence text. "
            "Some sentence text. Some sentence text. Some sentence text.\n"
            "Some sentence text. \ttab\n"
            "Some sentence text. i\ttab\n"
            "Some sentence text. ii\ttab\n"
            "Some sentence text. iii\ttab\n"
            "Some sentence text. iiii\ttab\n"
            "Some sentence text. iiiii\ttab\n"
            "Some sentence text. iiiiii\ttab\n"
            "Some sentence text. iiiiiii\ttab\n"
            "Some sentence text. some-simple-very-long-word-to-cut-into-many-pieces\n",
            TOGL_Color4U8(200, 100, 100, 255),
            "X - Exit."
        );

        puts("X - Exit");
        fflush(stdout);
    };

    data.do_on_destroy = []() {
        TOGL_UnloadFont();

        puts("Bye. Bye.");
        fflush(stdout);
    };

    data.do_on_resize = [](uint16_t width, uint16_t height) {
        glViewport(0, 0, width, height);

        // When window size changes, text size stays same.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, 0, height, 1, -1);

        s_size = {width, height};
    };

    data.draw = []() {
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        const TOGL_AreaI text_area = {PADDING, PADDING, s_size.width - PADDING * 2, s_size.height - PADDING * 2};

        // Draws border.
        glColor3f(1, 0, 0);
        DrawRectangle(text_area.x - 1, text_area.y - 1, text_area.width + 2, text_area.height + 2);

        // Draws text area.
        glColor3f(0.2f, 0.2f, 0.2f);
        DrawRectangle(text_area); 

        // Changes max text line width in pixels after which, words are wrapped.
        TOGL_SetLineWrapWidth(text_area.width);

        // Formats text using information provided by TOGL_SetNumberOfSpacesInTab and TOGL_SetLineWrapWidth.
        TOGL_AdjustAndRenderText({PADDING, s_size.height - FONT_SIZE - PADDING}, {0, 0, 0, 255}, s_text);
    };

    data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
        if (key_id == 'X' && !is_down) {
            TOGL_RequestClose();
        }
    };

    return TOGL_Run(data);
}