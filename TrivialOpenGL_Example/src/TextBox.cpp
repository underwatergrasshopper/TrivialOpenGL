/**
* @file TextBox.cpp
* @author underwatergrasshopper
*/

#include "TextBox.h"

#include <stdio.h>
#include <string>
#include <TrivialOpenGL.h>

namespace {
    enum : uint16_t {
        FONT_SIZE   = 32, // in pixels
        PADDING     = 40, // in pixels
    };

    TOGL_SizeU16    s_size              = {600, 600};
    std::string     s_text;
    bool            s_is_show_prompt;
    std::string     s_prompt            = "|";

    void DrawRectangle(int x, int y, int width, int height) {
        glBegin(GL_TRIANGLE_FAN);

        glVertex2i(x,           y);
        glVertex2i(x + width,   y);
        glVertex2i(x + width,   y + height);
        glVertex2i(x,           y + height);

        glEnd();
    };

    void DrawRectangle(const TOGL_AreaI& area) {
        DrawRectangle(area.x, area.y, area.width, area.height);
    };

    TOGL_AreaI Pad(const TOGL_AreaI& area, int padding) {
        return {
            area.x + padding, 
            area.y + padding, 
            area.width - padding * 2, 
            area.height - padding * 2
        };
    }

    // max_text_line_length - in pixels
    std::string GetPrompt() {
        if (s_is_show_prompt) {
            // If prompt would is outside of text box, then move to new line.
            if (TOGL_AdjustAndGetTextSize(s_text).height < TOGL_AdjustAndGetTextSize(s_text + s_prompt).height) {
                return "\n" + s_prompt;
            }
            return s_prompt;
        }
        return "";
    }

    TOGL_FineText GetAdjustedTextWithPrompt() {
        std::string text = s_text + s_prompt;

        // Adjust text to specific max line length and replaces tabs with spaces counting from beginning of the line.
        TOGL_FineText fine_text = TOGL_ToGlobalTextAdjuster().AdjustText(TOGL_ToGlobalFont(), text);

        if (!s_is_show_prompt) {
            // Removes prompt but do not change text align. 
            // This prevents text jumping from one line to another when prompt appears.
            auto& container = fine_text.ToElementContainers().back();
            if (container.IsTypeId(TOGL_FINE_TEXT_ELEMENT_TYPE_ID_TEXT)) {
                std::wstring last_text = container.GetText();
                container.SetText(last_text.substr(0, last_text.length() - 1));
            }
        }

        return fine_text;
    }
}

int RunTextBox() {
    TOGL_Data data = {};

    data.window_name        = "Formated Text";
    data.area               = {0, 0, s_size.width, s_size.height};
    // Ignores data.area.x and data.area.y coordinates 
    // and centers window in middle of work area (screen area excluding task bar).
    data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
    // Instead setting window size with data.area.width and data.area.height, 
    // sets window draw area size with data.area.width and data.area.height.
    data.style              |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE;

    data.timer_time_interval = 500;

    data.do_on_create = []() {
        glClearColor(0.2f, 0.2f, 0.5f, 1.0f);

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

        puts("Escape - Exit");
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

        const TOGL_AreaI text_area = Pad(TOGL_AreaI(0, 0, s_size.width, s_size.height), PADDING);

        // Draws border.
        glColor3f(0.8f, 0.8f, 0.8f);
        DrawRectangle(Pad(text_area, -6)); // offset from text to border of text box

        // Draws text area.
        glColor3f(0.2f, 0.2f, 0.2f);
        DrawRectangle(Pad(text_area, -5)); // offset from text to text box edge

        // Changes max text line width in pixels after which, words are wrapped.
        TOGL_SetLineWrapWidth(text_area.width);

        TOGL_RenderText({PADDING, s_size.height - FONT_SIZE - PADDING}, {200, 200, 200, 255}, GetAdjustedTextWithPrompt());
    };

    data.do_on_time = [](uint32_t timer_time_interval) {
        s_is_show_prompt = !s_is_show_prompt;
    };

    data.do_on_char = [](char c) {
        if (c == '\b') {
            if (!s_text.empty()) s_text.resize(s_text.size() - 1);
        } else if (c == '\r') {
            s_text += '\n';
        } else if (isprint(c) || c == '\t') {
            s_text += c;
        }
    };

    data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
        if (key_id == TOGL_KEY_ID_ESCAPE && !is_down) {
            TOGL_RequestClose();
        }
    };

    return TOGL_Run(data);
}