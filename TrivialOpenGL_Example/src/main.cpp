/**
* @file TrivialOpenGL.h
* @author underwatergrasshopper
*/

#include "ExampleSupport.h"

#include "SimpleTriangle.h"
#include "AnimatedTriangle.h"
#include "IconFromResource.h"
#include "IconFromFile.h"
#include "SimpleText.h"
#include "ColoredText.h"
#include "UnicodeText.h"
#include "FormatedText.h"
#include "TextBox.h"
#include "BorderlessWindow.h"

//------------------------------------------------------------------------------

enum {
    DEF_FONT_SIZE   = 16,   // in pixels
    TEXT_PADDING    = 10,   // in pixels
};

namespace {
    TOGL_Window&            s_window                        = TOGL_ToWindow();
    TOGL_SizeU16            s_resolution;
    bool                    s_is_client;
    TestImage               s_test_image;
    bool                    s_is_display_mose_move_data;

    DelayedActionManager    s_actions;

    TOGL_FontInfo           s_font_info = {
        "Courier New",
        DEF_FONT_SIZE,
        TOGL_FONT_SIZE_UNIT_ID_PIXELS,
        TOGL_FONT_STYLE_ID_NORMAL,
        TOGL_FONT_CHAR_SET_ID_ENGLISH,
        0,
        0
    };

    TestFont                s_test_font;
    FPS                     s_fps;

    TOGL_FineText           s_text;

    uint32_t                s_glyph_spacing                 = 0;
    uint32_t                s_line_spacing                  = 0;
};

void Reset() {
    s_resolution                    = {800, 400};
    s_is_client                     = false;
    s_is_display_mose_move_data     = false;

    s_actions.Reset();

    s_font_info = {
        "Courier New",
        DEF_FONT_SIZE,
        TOGL_FONT_SIZE_UNIT_ID_PIXELS,
        TOGL_FONT_STYLE_ID_NORMAL,
        TOGL_FONT_CHAR_SET_ID_ENGLISH,
        0,
        0
    };

    s_text.Clear();

    s_glyph_spacing                 = 0;
    s_line_spacing                  = 0;
}

//------------------------------------------------------------------------------

void LoadFont(const std::string& text = "") {
    TOGL_ResetTextDrawer();
    TOGL_ResetTextAdjuster();

    TOGL_LoadFont(s_font_info);

    if (TOGL_IsFontOk()) {
        printf("Font Loaded, size=%dpx.\n", TOGL_GetFontHeight());
    } else {
        printf("Font Error: %s\n", TOGL_GetFontErrMsg().c_str());
    }
    fflush(stdout);

    if (!text.empty()) {
        s_text = TOGL_TextAdjuster().AdjustText(TOGL_ToGlobalFont(), text);
    }
}

void SetInfoText(const std::string& text) {
    s_text = TOGL_AdjustText(text);
}

void DrawInfoText(const std::string& text = "") {
    const int           height  = TOGL_GetDrawAreaSize().height - TOGL_ToGlobalFont().GetHeight();
    const TOGL_PointI   pos     = {TEXT_PADDING, height};
    const TOGL_Color4U8 color   = {255, 255, 255, 255};

    if (!text.empty()) {
        TOGL_AdjustAndRenderText(pos, color, text);
    } else {
        TOGL_RenderText(pos, color, s_text);
    }
}

void SetInfoTextMaxWidth(uint32_t viewport_width) {
    TOGL_SetLineWrapWidth(viewport_width - TEXT_PADDING * 2);
}

//------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    std::vector<std::string> arguments;
    for (int index = 1; index < argc; ++index) {
        arguments.push_back(argv[index]);
    }

    ExampleManager example_manager;

    example_manager.SetDefaultExample("window_state");

    ////////////////////////////////////////////////////////////////////////////////
    // icon
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("icon", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.icon_file_name     = "..\\..\\..\\..\\TrivialOpenGL_Example\\assets\\icon.ico";

        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // icon_resource
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("icon_resource", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.icon_resource_id   = ICON_ID;

        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // custom_size
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("custom_size", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, 400, 800};
        data.style              = TOGL_WINDOW_STYLE_BIT_CENTERED;

        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // custom_area
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("custom_area", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {100, 200, 400, 800};

        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // no_area
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("no_area", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, 0, 0};

        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // no_resize
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("no_resize", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.style              = TOGL_WINDOW_STYLE_BIT_NO_RESIZE;

        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // no_maximize
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("no_maximize", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.style              = TOGL_WINDOW_STYLE_BIT_NO_MAXIMIZE;

        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // no_maximize_and_resize
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("no_maximize_and_resize", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.style              = TOGL_WINDOW_STYLE_BIT_NO_MAXIMIZE | TOGL_WINDOW_STYLE_BIT_NO_RESIZE;

        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // center
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("center", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, 400, 800};
        data.style              = TOGL_WINDOW_STYLE_BIT_CENTERED;

        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // draw_area_size
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("draw_area_size", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, 400, 200};
        data.style              = 0;
        data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
        data.style              |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE;

        return TOGL_Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // draw_area_only
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("draw_area_only", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, 400, 200};
        data.style              = 0;
        data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
        data.style              |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_ONLY;

        data.do_on_create = []() {
            puts("X or Escape - Exit");
            fflush(stdout);
        };

        data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
            if ((key_id == 'X' || key_id == TOGL_KEY_ID_ESCAPE) && !is_down) {
                TOGL_RequestClose();
            }
        };

        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // redraw_on_change_or_request
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("redraw_on_change_or_request", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.style              = TOGL_WINDOW_STYLE_BIT_REDRAW_ON_CHANGE_OR_REQUEST;
        data.log_level          = TOGL_LOG_LEVEL_DEBUG;

        return TOGL_Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // draw_triangle
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("draw_triangle", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        Reset();

        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.icon_resource_id   = ICON_ID;
        data.log_level          = TOGL_LOG_LEVEL_DEBUG;

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

        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // simple_triangle
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("simple_triangle", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        return RunSimpleTriangle();
    });

    ////////////////////////////////////////////////////////////////////////////////
    // animated_triangle
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("animated_triangle", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        return RunAnimatedTriangle();
    });

    ////////////////////////////////////////////////////////////////////////////////
    // icon_from_resource
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("icon_from_resource", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        return RunIconFromResource();
    });

    ////////////////////////////////////////////////////////////////////////////////
    // icon_from_file
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("icon_from_file", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        return RunIconFromFile();
    });

    ////////////////////////////////////////////////////////////////////////////////
    // simple_text
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("simple_text", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        return RunSimpleText();
    });

    ////////////////////////////////////////////////////////////////////////////////
    // colored_text
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("colored_text", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        return RunColoredText();
    });

    ////////////////////////////////////////////////////////////////////////////////
    // unicode_text
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("unicode_text", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        return RunUnicodeText();
    });

    ////////////////////////////////////////////////////////////////////////////////
    // formated_text
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("formated_text", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        return RunFormatedText();
    });

    ////////////////////////////////////////////////////////////////////////////////
    // text_box
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("text_box", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        return RunTextBox();
    });

    ////////////////////////////////////////////////////////////////////////////////
    // borderless_window
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("borderless_window", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        return RunBorderlessWindow();
    });

    ////////////////////////////////////////////////////////////////////////////////
    // move_and_resize
    ////////////////////////////////////////////////////////////////////////////////

    auto Combine = [](const std::set<std::string>& l, const std::set<std::string>& r) {
        std::set<std::string> temp = l;
        temp.insert(r.begin(), r.end());
        return temp;
    };

    const std::set<std::string> all_options = {
        "draw_area_size", "draw_area_only", "redraw_on_change_or_request", "no_resize", "no_maximize",
        "notify_any_message", "notify_draw_call", "notify_mouse_move ", "notify_key_message", "notify_character_message", "no_debug"
    };

    example_manager.AddExample("move_and_resize", all_options, {}, [](const std::string& name, const std::set<std::string>& options) {
        Reset();

        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {600, 300};

        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, s_resolution.width, s_resolution.height};
        data.style              = 0;
        data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
        data.icon_resource_id   = ICON_ID;

        if (!IsOption("no_debug"))                      data.log_level = TOGL_LOG_LEVEL_DEBUG;

        if (IsOption("draw_area_size"))                 data.style |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE;
        if (IsOption("draw_area_only"))                 data.style |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_ONLY;
        if (IsOption("redraw_on_change_or_request"))    data.style |= TOGL_WINDOW_STYLE_BIT_REDRAW_ON_CHANGE_OR_REQUEST;
        if (IsOption("no_resize"))                      data.style |= TOGL_WINDOW_STYLE_BIT_NO_RESIZE;
        if (IsOption("no_maximize"))                    data.style |= TOGL_WINDOW_STYLE_BIT_NO_MAXIMIZE;

        if (IsOption("notify_any_message"))             data.special_debug.is_notify_any_message        = true;
        if (IsOption("notify_draw_call"))               data.special_debug.is_notify_draw_call          = true;
        if (IsOption("notify_mouse_move"))              data.special_debug.is_notify_mouse_move         = true;
        if (IsOption("notify_key_message"))             data.special_debug.is_notify_key_message        = true;
        if (IsOption("notify_character_message"))       data.special_debug.is_notify_character_message  = true;

        data.do_on_create = []() {
            s_test_image.Initialize(TOGL_GetDrawAreaSize());

            LoadFont();
        };

        data.draw = []() {
            s_test_image.Animate();

            DrawInfoText(
                std::string() +
                "FPS: " + std::to_string(s_fps.Measure()) + "\n"
                "1 - MoveTo(0, 0)\n"
                "2 - MoveTo(10, 100)\n"
                "3 - SetSize(400, 200)\n"
                "4 - SetSize(800, 400)\n"
                "5 - SetArea(100, 10, 800, 400)\n"
                "6 - SetArea(window_area/draw_area)\n"
                "7 - SetArea(desktop_area_no_taskbar)\n"
                "8 - SetArea(screen_area)\n"
                "C - Center(" + std::to_string(s_resolution.width) + ", " + std::to_string(s_resolution.height) + ")\n"
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

        data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
            auto& window = TOGL_ToWindow();

            if (!is_down) {
                if (!s_is_client) {
                    switch (key_id) {
                    case TOGL_KEY_ID_1:           window.MoveTo(0, 0); break;
                    case TOGL_KEY_ID_2:           window.MoveTo(10, 100); break;
                                    
                    case TOGL_KEY_ID_3:           window.Resize(400, 200); break;
                    case TOGL_KEY_ID_4:           window.Resize(800, 400); break;
                                    
                    case TOGL_KEY_ID_5:           window.SetArea(100, 10, 800, 400); break;
                    case TOGL_KEY_ID_6:           window.SetArea(window.GetArea()); break;
                    case TOGL_KEY_ID_7:           window.SetArea(TOGL_GetDesktopAreaNoTaskBar()); break;
                    case TOGL_KEY_ID_8:           window.SetArea({{}, TOGL_GetScreenSize()}); break;

                    case TOGL_KEY_ID_C:           window.Center(s_resolution, s_is_client); break;

                    case TOGL_KEY_ID_ARROW_LEFT:   window.MoveBy(-30, 0); break;
                    case TOGL_KEY_ID_ARROW_RIGHT:  window.MoveBy(30, 0); break;

                    case TOGL_KEY_ID_X:           window.RequestClose(); break;
                                   
                    case TOGL_KEY_ID_I:           DisplayWindowInfo(); break;
                                   
                    case TOGL_KEY_ID_T:           s_is_client = !s_is_client; break;
                    default: break;
                    }
                } else {
                    switch (key_id) {
                    case TOGL_KEY_ID_1:           window.MoveTo(0, 0, true); break;
                    case TOGL_KEY_ID_2:           window.MoveTo(10, 100, true); break;

                    case TOGL_KEY_ID_3:           window.Resize(400, 200, true); break;
                    case TOGL_KEY_ID_4:           window.Resize(800, 400, true); break;

                    case TOGL_KEY_ID_5:           window.SetArea(100, 10, 800, 400, false); break;
                    case TOGL_KEY_ID_6:           window.SetArea(window.GetDrawArea(), false); break;
                    case TOGL_KEY_ID_7:           window.SetArea(TOGL_GetDesktopAreaNoTaskBar(), false); break;
                    case TOGL_KEY_ID_8:           window.SetArea({{}, TOGL_GetScreenSize()}, false); break;

                    case TOGL_KEY_ID_C:           window.Center(s_resolution, s_is_client); break;

                    case TOGL_KEY_ID_ARROW_LEFT:   window.MoveBy(-30, 0); break;
                    case TOGL_KEY_ID_ARROW_RIGHT:  window.MoveBy(30, 0); break;

                    case TOGL_KEY_ID_X:           window.RequestClose(); break;

                    case TOGL_KEY_ID_I:           DisplayWindowInfo(); break;

                    case TOGL_KEY_ID_T:           s_is_client = !s_is_client; break;
                    default: break;
                    }
                }
            }
        };

        return TOGL_Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // font
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("font", Combine(all_options, {"unicode", "bold", "glyph_spacing_5", "line_spacing_5", "arial"}), {}, [](const std::string& name, const std::set<std::string>& options) {
        Reset();

        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {800, 400};

        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, s_resolution.width, s_resolution.height};
        data.style              = 0;
        data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
        data.icon_resource_id   = ICON_ID;

        if (!IsOption("no_debug"))                      data.log_level = TOGL_LOG_LEVEL_DEBUG;

        if (IsOption("draw_area_size"))                 data.style |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE;
        if (IsOption("draw_area_only"))                 data.style |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_ONLY;
        if (IsOption("redraw_on_change_or_request"))    data.style |= TOGL_WINDOW_STYLE_BIT_REDRAW_ON_CHANGE_OR_REQUEST;
        if (IsOption("no_resize"))                      data.style |= TOGL_WINDOW_STYLE_BIT_NO_RESIZE;
        if (IsOption("no_maximize"))                    data.style |= TOGL_WINDOW_STYLE_BIT_NO_MAXIMIZE;

        if (IsOption("notify_any_message"))             data.special_debug.is_notify_any_message        = true;
        if (IsOption("notify_draw_call"))               data.special_debug.is_notify_draw_call          = true;
        if (IsOption("notify_mouse_move"))              data.special_debug.is_notify_mouse_move         = true;
        if (IsOption("notify_key_message"))             data.special_debug.is_notify_key_message        = true;
        if (IsOption("notify_character_message"))       data.special_debug.is_notify_character_message  = true;
        
        if (IsOption("arial"))              s_font_info.name = "Arial";
        if (IsOption("unicode"))            s_font_info.unicode_range_group = TOGL_FONT_CHAR_SET_ID_RANGE_0000_FFFF;
        if (IsOption("bold"))               s_font_info.style = TOGL_FONT_STYLE_ID_BOLD;
        if (IsOption("glyph_spacing_5"))    s_font_info.distance_between_glyphs = 5;
        if (IsOption("line_spacing_5"))     s_font_info.distance_between_lines = 5;

        data.do_on_create = []() {
            s_test_image.Initialize(TOGL_GetDrawAreaSize());

            LoadFont();

            TOGL_ToGlobalFont().SaveAsBMP();
        };

        data.draw = []() {
            TOGL_Font& font = TOGL_ToGlobalFont();
            
            s_test_image.Animate();
            
            TOGL_Window& window = TOGL_ToWindow();

            glColor3f(1, 0.5, 0);
            glPushMatrix();
            glTranslatef(50, 100, 0);
            glScaled(3, 3, 1);
            font.RenderGlyphs(u8"Somme text. Xj\u3400\u5016\u9D9B\u0001\U00024B62");
            glPopMatrix();

            TOGL_TextDrawer     text_drawer;
            TOGL_TextAdjuster   text_adjuster;
            
            //text_drawer.SetOrientation(TEXT_DRAWER_ORIENTATION_LEFT_TOP);

            text_drawer.SetPos(50, 100);
            text_drawer.SetColor(255, 0, 0, 255);

            text_drawer.RenderText(font, u8"Some text. Xj\u3400\u5016\u9D9B\u0001\U00024B62\nNew Line.");
            text_drawer.RenderText(font, u8"\nAnd another line.");
            text_drawer.RenderText(font, u8"\n\tTab.\b");

            TOGL_FineText text = TOGL_FineText(
                TOGL_Color4U8(0, 255, 0, 255),
                u8"Xj\n",
                TOGL_Color4U8(255, 0, 0, 255),
                u8"Xj\n",
                TOGL_Color4U8(0, 0, 0, 255),
                u8"Some text. \u3400\u5016\u9D9B\u0001\U00024B62. Many words in line. Many words in line. Many words in line. Many words in line. Many words in line. Many words in line.\n"
                u8"Many words in line with \ttab.\n"
                u8"Many words in line with i\ttab.\n"
                u8"Many words in line with ii\ttab.\n"
                u8"Many words in line with iii\ttab.\n"
                u8"Many words in line with iiii\ttab.\n"
                u8"New line. "
                u8"\tTab.Long                                 line. "
                u8"Very-long-text-to-split-apart. ");

            text_adjuster.SetLineWrapWidth(s_test_image.GetSize().width - 10);
            text = text_adjuster.AdjustText(font, text);

            const TOGL_SizeU text_size = text_drawer.GetTextSize(font, text);

            glColor3f(1, 1, 1);
            DrawRectangle(0, s_test_image.GetSize().height - text_size.height, s_test_image.GetSize().width - 10, text_size.height);

            text_drawer.SetPos(0 , s_test_image.GetSize().height - font.GetHeight());
            
            text_drawer.SetColor(255, 0, 0, 255);
            text_drawer.RenderText(font, text);
            
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            s_test_image.Resize(width, height);
        };

        data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
            auto& window = TOGL_ToWindow();

            if (!is_down && key_id == 'X') window.RequestClose();
            if (!is_down && key_id == 'R') window.Center(500, 500);
        };
        
        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // window_state
    ////////////////////////////////////////////////////////////////////////////////

    std::set<std::string> window_state_all_options = Combine(all_options, {"full_screen_at_start"});

    example_manager.AddExample("window_state", window_state_all_options, {"draw_area_size"}, [](const std::string& name, const std::set<std::string>& options) {
        Reset();

        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {800, 400};

        TOGL_Data data = {};

        data.window_name            = "TrivialOpenGL_Example ";
        data.window_name            += name;
        data.area                   = {0, 0, s_resolution.width, s_resolution.height};
        data.style                  = 0;
        data.style                  |= TOGL_WINDOW_STYLE_BIT_CENTERED;
        data.icon_resource_id       = ICON_ID;
        data.timer_time_interval    = 1000;

        if (!IsOption("no_debug"))                      data.log_level = TOGL_LOG_LEVEL_DEBUG;

        if (IsOption("draw_area_size"))                 data.style |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE;
        if (IsOption("draw_area_only"))                 data.style |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_ONLY;
        if (IsOption("redraw_on_change_or_request"))    data.style |= TOGL_WINDOW_STYLE_BIT_REDRAW_ON_CHANGE_OR_REQUEST;
        if (IsOption("no_resize"))                      data.style |= TOGL_WINDOW_STYLE_BIT_NO_RESIZE;
        if (IsOption("no_maximize"))                    data.style |= TOGL_WINDOW_STYLE_BIT_NO_MAXIMIZE;

        if (IsOption("notify_any_message"))             data.special_debug.is_notify_any_message        = true;
        if (IsOption("notify_draw_call"))               data.special_debug.is_notify_draw_call          = true;
        if (IsOption("notify_mouse_move"))              data.special_debug.is_notify_mouse_move         = true;
        if (IsOption("notify_key_message"))             data.special_debug.is_notify_key_message        = true;
        if (IsOption("notify_character_message"))       data.special_debug.is_notify_character_message  = true;

        if (IsOption("full_screen_at_start")) {
            data.do_on_create = []() {
                s_test_image.Initialize(TOGL_GetDrawAreaSize());

                TOGL_GoWindowedFullScreen();

                LoadFont();
            };
        } else {
            data.do_on_create = []() {
                s_test_image.Initialize(TOGL_GetDrawAreaSize());

                LoadFont();
            };
        }

        data.do_on_time = [](uint32_t time_interval) {

        };

        data.draw = []() {
            s_fps.Measure();

            s_test_image.Animate();

            DrawInfoText(
                std::string() +
                "FPS: " + std::to_string(s_fps.Get()) + "\n"
                "X - Exit\n"
                "R - Redraw\n"
                "C - Center(" + std::to_string(s_resolution.width) + ", " + std::to_string(s_resolution.height) + ") (as window size)\n"
                "V - Center(" + std::to_string(s_resolution.width) + ", " + std::to_string(s_resolution.height) + ") (as draw area size)\n"
                "F - Background -> Foreground -> Minimize -> Center -> Hide -> Show (wait until finish)\n"
                "Arrows - Move\n"
                "0 - Hide -> Show -> Hide -> SetSize -> Hide -> Minimize -> Hide -> Maximize -> Hide ->\n"
                "    Windowed Full Screen -> Hide -> Center(" + std::to_string(s_resolution.width) + ", " + std::to_string(s_resolution.height) + ") (wait until finish)\n"
                "1 - Hide -> Show\n"
                "2 - Minimize -> Center\n"
                "W - Minimize -> MoveBy (should not restore from minimize)\n"
                "3 - Center(" + std::to_string(s_resolution.width) + ", " + std::to_string(s_resolution.height) + ")\n"
                "4 - Maximize\n"
                "5 - GoWindowedFullScreen\n"
                "6 - Hide -> GoWindowedFullScreen\n"
                "7 - SetArea(100, 50, 300, 600)\n"
                // NOTE: Disabled for now. Uncomment for internal test only. Might be implemented in future.
                //"8 - Native Full Screen (experimental)\n"
                //"9 - Restore from Native Full Screen (experimental)\n"
                "I - Display Info\n"
                "D - on/off TOGL Debug\n"
            );

            s_actions.Run();
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            s_test_image.Resize(width, height);
            SetInfoTextMaxWidth(width);

            std::string message = "Resize: ";
            message += std::to_string(width) + " " + std::to_string(height);
            puts(message.c_str());
        };

        data.do_on_state_change = [](TOGL_WindowStateId window_state) {
            std::string message = "Changed Window State to: ";

            switch (window_state) {
            case TOGL_WINDOW_STATE_ID_NORMAL:                 message += "NORMAL"; break;
            case TOGL_WINDOW_STATE_ID_MAXIMIZED:              message += "MAXIMIZED"; break;
            case TOGL_WINDOW_STATE_ID_MINIMIZED:              message += "MINIMIZED"; break;
            case TOGL_WINDOW_STATE_ID_WINDOWED_FULL_SCREENED: message += "WINDOWED_FULL_SCREENED"; break;
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

        data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
            if (!is_down) {

                switch (key_id) {

                case 'X':               TOGL_RequestClose(); break;
                case TOGL_KEY_ID_R:     TOGL_RequestDraw(); break;
                case TOGL_KEY_ID_C:     TOGL_Center(s_resolution, false); break;
                case TOGL_KEY_ID_V:     TOGL_Center(s_resolution, true); break;

                case 'F':
                    s_actions.Add(0, [](){
                        // Moves behind window to foreground.
                        SetForegroundWindow(GetNextWindow(GetForegroundWindow(), GW_HWNDNEXT));
                        togl_print_i32(TOGL_IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL_GoForeground();
                        togl_print_i32(TOGL_IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL_Minimize();
                        togl_print_i32(TOGL_IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL_Center(s_resolution);
                        //GoForeground(); // redundant
                        togl_print_i32(TOGL_IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL_Hide();
                        togl_print_i32(TOGL_IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL_Show();
                        //GoForeground(); // redundant
                        togl_print_i32(TOGL_IsForeground());

                        puts("Finished");
                    });
                    break;

                case TOGL_KEY_ID_ARROW_LEFT:
                    TOGL_MoveBy(-30, 0);
                    break;

                case TOGL_KEY_ID_ARROW_RIGHT:
                    TOGL_MoveBy(30, 0);
                    break;

                case TOGL_KEY_ID_ARROW_UP:
                    TOGL_MoveBy(0, -30);
                    break;

                case TOGL_KEY_ID_ARROW_DOWN:
                    TOGL_MoveBy(0, 30);
                    break;

                case TOGL_KEY_ID_0: 
                    s_actions.Add(0, [](){
                        puts("--- Show --");
                        TOGL_Hide();
                        PrintWindowStates();
                    });
                    
                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL_Show(); 
                        PrintWindowStates();
                    });
                    
                    s_actions.Add(1, [](){
                        puts("--- Restore ---");
                        TOGL_Hide();
                        PrintWindowStates();
                    });
                    
                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL_Resize(s_resolution, TOGL_GetStyle() & TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE); 
                        PrintWindowStates();
                    });
                    
                    s_actions.Add(1, [](){
                        puts("--- Minimize ---");
                        TOGL_Hide();
                        PrintWindowStates();
                    });
                    
                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL_Minimize(); 
                        PrintWindowStates();
                    
                    });

                    s_actions.Add(1, [](){
                        puts("--- Maximize ---");
                        TOGL_Hide();
                        PrintWindowStates();

                    });

                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL_Maximize(); 
                        PrintWindowStates();
                    });

                    s_actions.Add(1, [](){
                        puts("--- Windowed Full Screen ---");
                        TOGL_Hide();
                        PrintWindowStates();

                    });

                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL_GoWindowedFullScreen(); 
                        PrintWindowStates();
                    });

                    s_actions.Add(1, [](){
                        puts("--- Center ---");
                        TOGL_Hide();
                        PrintWindowStates();

                    });

                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL_Center(s_resolution); 
                        PrintWindowStates();

                        puts("Finished");
                    });
                    break;


                case '1':
                    s_actions.Add(0, [](){
                        puts("---");
                        TOGL_Hide();
                        PrintWindowStates();

                        togl_print_i32(TOGL_GetArea().x);
                        togl_print_i32(TOGL_GetArea().y);
                        togl_print_i32(TOGL_GetArea().width);
                        togl_print_i32(TOGL_GetArea().height);

                        togl_print_i32(TOGL_IsForeground());
                    });

                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL_Show(); 

                        togl_print_i32(TOGL_IsForeground());

                        PrintWindowStates();
                    });
                    break;

                case TOGL_KEY_ID_2: 
                    s_actions.Add(0, [](){
                        puts("---");
                        TOGL_Minimize();
                        PrintWindowStates();

                        togl_print_i32(TOGL_GetArea().x);
                        togl_print_i32(TOGL_GetArea().y);
                        togl_print_i32(TOGL_GetArea().width);
                        togl_print_i32(TOGL_GetArea().height);
                    });

                    s_actions.Add(1, [](){
                        puts("---");

                        TOGL_Center(s_resolution);
                        PrintWindowStates();
                    });
                    break;

                case TOGL_KEY_ID_W: 
                    s_actions.Add(0, [](){
                        puts("---");
                        TOGL_Minimize();
                        PrintWindowStates();

                        togl_print_i32(TOGL_GetArea().x);
                        togl_print_i32(TOGL_GetArea().y);
                        togl_print_i32(TOGL_GetArea().width);
                        togl_print_i32(TOGL_GetArea().height);
                    });

                    s_actions.Add(1, [](){
                        TOGL_MoveBy(30, 0);
                        PrintWindowStates();
                    });
                    break;

                case TOGL_KEY_ID_3:
                    puts("---");
                    TOGL_Center(s_resolution);
                    PrintWindowStates();
                    break;

                case TOGL_KEY_ID_4:
                    puts("---");
                    TOGL_Maximize(); 
                    PrintWindowStates();
                    break;

                case TOGL_KEY_ID_5:
                    puts("---");
                    TOGL_GoWindowedFullScreen(); 
                    PrintWindowStates();
                    break;

                case TOGL_KEY_ID_6:
                    s_actions.Add(0, [](){
                        puts("---");
                        TOGL_Hide();
                        PrintWindowStates();
                    });

                    s_actions.Add(1, [](){
                        TOGL_GoWindowedFullScreen(); 
                        PrintWindowStates();
                    });
                    break;

                case TOGL_KEY_ID_7:
                    puts("---");
                    TOGL_SetArea(100, 50, 300, 600);
                    PrintWindowStates();
                    break;

                // NOTE: Disabled for now. Uncomment for internal test only. Might be implemented in future.
                //case TOGL_KEY_ID_8: {
                //    // Warning!!! Experimental.
                //    DEVMODEW dev_mode = {};
                //    dev_mode.dmSize = sizeof(DEVMODEW);
                //
                //    dev_mode.dmDisplayFixedOutput   = DMDFO_DEFAULT; // DMDFO_CENTER, DMDFO_STRETCH, DMDFO_DEFAULT
                //    dev_mode.dmPelsWidth            = 640; 
                //    dev_mode.dmPelsHeight           = 480; 
                //    // only specific resolutions will work
                //    //dev_mode.dmPelsWidth            = 1280; 
                //    //dev_mode.dmPelsHeight           = 720; 
                //    dev_mode.dmFields               = DM_DISPLAYFIXEDOUTPUT | DM_PELSWIDTH | DM_PELSHEIGHT;
                //
                //    // ... or ...
                //    //if (!EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, &dev_mode)) puts("Can not get Display Settings.");
                //
                //    LONG result = ChangeDisplaySettingsW(&dev_mode, CDS_FULLSCREEN);
                //    togl_print_i32(result);
                //
                //    break;
                //}
                //
                //case TOGL_KEY_ID_9:
                //    // Warning!!! Experimental.
                //    ChangeDisplaySettingsW(NULL, CDS_RESET);
                //    break;

                case TOGL_KEY_ID_I: 
                    DisplayWindowInfo(); 
                    break;

                case TOGL_KEY_ID_D: 
                    if (TOGL_GetLogLevel() == TOGL_LOG_LEVEL_DEBUG) {
                        TOGL_SetLogLevel(TOGL_LOG_LEVEL_ERROR);
                    } else {
                        TOGL_SetLogLevel(TOGL_LOG_LEVEL_DEBUG);
                    }
                    break;
                default: break;
                }
            }
        };

        return TOGL_Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // opengl_version
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("opengl_version", {"wrong"}, {}, [](const std::string& name, const std::set<std::string>& options) {
        Reset();

        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        if (IsOption("wrong")) {
            data.opengl_verion      = {3, 7};
        } else {
            data.opengl_verion      = {3, 3};
        }
        data.log_level          = TOGL_LOG_LEVEL_DEBUG;

        data.do_on_create = []() {
            auto version = TOGL_GetOpenGL_Version();
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

        data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
            if (!is_down && key_id == 'X') TOGL_RequestClose();
        };

        return TOGL_Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // keyboard_and_mouse
    ////////////////////////////////////////////////////////////////////////////////
    
    example_manager.AddExample("keyboard_and_mouse", all_options, {}, [](const std::string& name, const std::set<std::string>& options) {
        Reset();

        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {600, 300};

        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, s_resolution.width, s_resolution.height};
        data.style              = 0;
        data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
        data.icon_resource_id   = ICON_ID;

        if (!IsOption("no_debug"))                      data.log_level = TOGL_LOG_LEVEL_DEBUG;

        if (IsOption("draw_area_size"))                 data.style |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_SIZE;
        if (IsOption("draw_area_only"))                 data.style |= TOGL_WINDOW_STYLE_BIT_DRAW_AREA_ONLY;
        if (IsOption("redraw_on_change_or_request"))    data.style |= TOGL_WINDOW_STYLE_BIT_REDRAW_ON_CHANGE_OR_REQUEST;
        if (IsOption("no_resize"))                      data.style |= TOGL_WINDOW_STYLE_BIT_NO_RESIZE;
        if (IsOption("no_maximize"))                    data.style |= TOGL_WINDOW_STYLE_BIT_NO_MAXIMIZE;

        if (IsOption("notify_any_message"))             data.special_debug.is_notify_any_message        = true;
        if (IsOption("notify_draw_call"))               data.special_debug.is_notify_draw_call          = true;
        if (IsOption("notify_mouse_move"))              data.special_debug.is_notify_mouse_move         = true;
        if (IsOption("notify_key_message"))             data.special_debug.is_notify_key_message        = true;
        if (IsOption("notify_character_message"))       data.special_debug.is_notify_character_message  = true;

        data.do_on_create = []() {
            s_test_image.Initialize(TOGL_GetDrawAreaSize());

            LoadFont();
        };

        data.draw = []() {
            s_test_image.Animate();
            DrawInfoText(
                std::string() +
                "FPS: " + std::to_string(s_fps.Measure()) + "\n"
                "S - Enable/Disable display mouse move data\n"
                "D - Switch Log Level Debug/Info\n"
                "I - Display Keyboard and Mouse State Info\n"
                "X - Exit\n"
            );
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            s_test_image.Resize(width, height);
        };

        data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
            std::string message = "Key: ";

            message += TOGL_KeyIdToStr(key_id);
            message += is_down ? " down" : " up";
            message += TOGL_ExtraToStr(extra);

            puts(message.c_str());

            if (is_down) {
                switch (key_id) {
                case 'X':
                    TOGL_RequestClose();
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
                    if (TOGL_GetLogLevel() == TOGL_LOG_LEVEL_DEBUG) {
                        TOGL_SetLogLevel(TOGL_LOG_LEVEL_INFO);
                        puts("Log Level: DEBUG -> INFO");
                    } else {
                        TOGL_SetLogLevel(TOGL_LOG_LEVEL_DEBUG);
                        puts("Log Level: INFO -> DEBUG");
                    }
                    break;

                case 'I': {
                    DisplayWindowInfo();

                    togl_print_i32(TOGL_IsKeyToggled(TOGL_KEY_ID_CAPS_LOCK));
                    togl_print_i32(TOGL_IsKeyToggled(TOGL_KEY_ID_INSERT));
                    togl_print_i32(TOGL_IsKeyToggled(TOGL_KEY_ID_NUMLOCK));

                    const TOGL_PointI cursor_pos_in_screen = TOGL_GetCursorPosInScreen();
                    togl_print_i32(cursor_pos_in_screen.x);
                    togl_print_i32(cursor_pos_in_screen.y);

                    const TOGL_PointI cursor_pos_in_draw_area = TOGL_GetCursorPosInDrawArea();
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

        return TOGL_Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // timer
    ////////////////////////////////////////////////////////////////////////////////
    
    example_manager.AddExample("timer", {"100ms", "500ms", "1s"}, {"500ms"}, [](const std::string& name, const std::set<std::string>& options) {
        Reset();

        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {600, 300};

        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, s_resolution.width, s_resolution.height};
        data.style              = 0;
        data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
        data.icon_resource_id   = ICON_ID;
        data.log_level          = TOGL_LOG_LEVEL_DEBUG;

        if (IsOption("100ms"))  data.timer_time_interval = 100;
        if (IsOption("500ms"))  data.timer_time_interval = 500;
        if (IsOption("1s"))     data.timer_time_interval = 1000;
        
        data.do_on_create = []() {
            s_test_image.Initialize(TOGL_GetDrawAreaSize());
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

        data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
            if (key_id == 'X' && !is_down) TOGL_RequestClose();
        };

        return TOGL_Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // test_font
    ////////////////////////////////////////////////////////////////////////////////
    
    example_manager.AddExample("test_font", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        Reset();

        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {800, 400};

        TOGL_Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, s_resolution.width, s_resolution.height};
        data.style              = 0;
        data.style              |= TOGL_WINDOW_STYLE_BIT_CENTERED;
        data.icon_resource_id   = ICON_ID;
        data.log_level          = TOGL_LOG_LEVEL_DEBUG;
        
        data.do_on_create = []() {
            s_test_font.Create(s_resolution.width, s_resolution.height);

            const uint8_t image[] = {
                // first row
                255, 0, 0, 255,         // red
                255, 255, 0, 255,       // yellow
                255, 255, 255, 255,     // white

                // second row
                255, 255, 255, 63,      // very transparent white
                0, 255, 0, 255,         // green
                255, 255, 255, 127,     // transparent white
            };

            if (SaveAsBMP("test.bmp", image, 3, 2)) {
                puts("Saved to BMP.");
            } else {
                puts("Can not save to BMP.");
            }
        };

        data.do_on_destroy = []() {
            s_test_font.Destroy();
        };

        data.draw = []() {
            s_test_font.Render();
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            glViewport(0, 0, width, height);
        };

        data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
            if (key_id == 'X' && !is_down) TOGL_RequestClose();
        };

        return TOGL_Run(data);
    });

    //------------------------------------------------------------------------------

    return example_manager.Run(arguments);
}