# TrivialOpenGL
A simple C++ header-only library that gives the ability to create an window with OpenGL context to draw in.
Includes support for drawing text.

---

## Informations
- Targeted Platform: Windows 7/10
- Targeted Language: C++11
- Uses Libraries: WinApi, OpenGL
- Required Dependencies: OpenGL32.lib

---

## What is included in project
Contains:
- source code of library (`include/`),
- unit tests (`TrivialOpenGL_Test/`),
- examples (`TrivialOpenGL_Example/`).

---

## Requirements
To be able to compile an outer project, which includes this library, `OpenGL32.lib` needs to be included to the outer project.

---

## HOWTO: Proper clone repository with Git
Run `git clone --recurse-submodules https://github.com/underwatergrasshopper/TrivialOpenGL.git`
. This command will clone repository, including it's submodules.

---
## HOWTO: Build/Run with MinGW
Requirements:
- Installed MinGW for both 32 and 64 bit versions.

Procedure:
- Make `MinGW_MakeCache.bat` and put inside
  ```
  set MINGW32_BIN_PATH=<path32>
  set MINGW64_BIN_PATH=<path64>
  ```
  where `path32`is path to bin folder of 32 bit version of MinGW and `path64`is path to bin folder of 64 bit version of MinGW.
- Run `MinGW_Make.bat <action> <mode> <arch>` where
  ```
  <action>
      run
      build
      clean
  <mode>
      Release
      Debug
  <arch>
      32
      64
  ```
  .

  Example: `MinGW_Make.bat run Release 64`.

## HOWTO: Run tests
### From Visual Studio 2019/2022
Available Configurations: `Release`, `Debug`. 

Available Platforms: `x86`, `x64`.

Procedure:
- From `Solution Explorer` select `TrivialOpenGL_Test`.
- From context menu select `Debug` > `Start Without Debugging`.

### From Test.bat
Requirements:
- Installed Visual Studio 2019/2022.

Procedure:
- Run `Test.bat` from `TrivialOpenGL` folder.

### From MinGW_Test.bat
Requirements:
- Installed MinGW for both 32 and 64 bit versions.

Procedure:
- Make `MinGW_MakeCache.bat` and put inside
  ```
  set MINGW32_BIN_PATH=<path32>
  set MINGW64_BIN_PATH=<path64>
  ```
  where `path32`is path to bin folder of 32 bit version of MinGW and `path64`is path to bin folder of 64 bit version of MinGW.
- Run `MinGW_Test.bat` from `TrivialOpenGL` folder.

---

## HOWTO: Run Examples
### From Visual Studio 2019/2022
Available Configurations: `Release`, `Debug`. 

Available Platforms: `x86`, `x64`.

Procedure:
- From `Solution Explorer` select `TrivialOpenGL_Example`.
- From context menu select `Debug` > `Start Without Debugging`.
- From started command window select example by typing it's name from printed columns (for example: `window_state`).
- From another printed columns select one or multiple options separated by single space (for example: `no_maximize no_resize`).
- From created window select key to press from displayed legend, if available.

Recommended examples to select:
- `window_state`
- `font` (if window size is changed, then text will align)

---

## Builds and Tests results

Compiler: **MSVC** (automated)

| Test Platform | Target Platform | Environment | Architecture  | Build and Test |
|-|-|-|-|-|
| Windows Server 2022, Windows Server 2019 | Windows 10 |  Visual Studio 2022, Visual Studio  2019 | x86, x86_64 | [![Build and Test](https://github.com/underwatergrasshopper/TrivialOpenGL/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/underwatergrasshopper/TrivialOpenGL/actions/workflows/build_and_test.yml) |

 Compiler: **LLVM CLang MinGW** (automated)

| Test Platform | Target Platform | Environment | Architecture | Build and Test |
|-|-|-|-|-|
| Windows Server 2022 | Windows 10 | MinGW-W64 (LLVM Clang 15.0.0) | x86, x86_64 | [![Build and Test (MinGW)](https://github.com/underwatergrasshopper/TrivialOpenGL/actions/workflows/build_and_test_mingw.yml/badge.svg)](https://github.com/underwatergrasshopper/TrivialOpenGL/actions/workflows/build_and_test_mingw.yml) |

---

## Code Examples
- [Simple Triangle](#simple-triangle)
- [Borderless Window](#borderless-window)
- [Animated Triangle](#animated-triangle)
- [Icon from Resource](#icon-from-resource)
- [Icon from File](#icon-from-file)
- [Simple Text](#simple-text)
- [Colored Text](#colored-text)
- [Unicode Text](#unicode-text)
- [Formated Text](#formated-text)
- [Text Box](#text-box)

---

### Simple Triangle

`main.cpp`
```c++
#include <stdio.h>
#include <TrivialOpenGL.h>

int main() {
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
        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

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
```
[Back to: Code Examples](#code-examples)

---

### Borderless Window
Borderless window is created by adding `TOGL_WINDOW_STYLE_BIT_DRAW_AREA_ONLY` to `data.style`.
Window borders and title bar is removed in this case.

Press `X` or `Escape` to close window.

`main.cpp`
```c++
#include <stdio.h>
#include <TrivialOpenGL.h>

int main() {
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
```
[Back to: Code Examples](#code-examples)

---

### Animated Triangle

`main.cpp`
```c++
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

int main() {
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
```
[Back to: Code Examples](#code-examples)

---

### Icon from Resource
Icon imported this way will be visible on:
- executable file,
- window title bar,
- task bar.

`Resource.h`
```c++
#ifndef RESOURCE_H_
#define RESOURCE_H_

#define ICON_ID 1001

#endif // RESOURCE_H_
```

`Resource.rc`
```c++
#include "Resource.h"

ICON_ID ICON "icon.ico"
```

`main.cpp`
```c++
#include <stdio.h>
#include <TrivialOpenGL.h>

#include "Resource.h"

int main() {
    TOGL_Data data = {};

    data.window_name        = "Icon from Resource";
    // Icon will show on:
    // - executable file
    // - window title bar
    // - task bar
    data.icon_resource_id   = ICON_ID;

    data.do_on_create = []() {
        puts("X - Exit");
        fflush(stdout);
    };

    data.do_on_destroy = []() {
        puts("Bye. Bye.");
        fflush(stdout);
    };

    data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
        if (key_id == 'X' && !is_down) {
            TOGL_RequestClose();
        }
    };

    return TOGL_Run(data);
}
```
[Back to: Code Examples](#code-examples)

---

### Icon from File
Icon imported this way will be visible on:
- window title bar,
- task bar.

It will not show on executable file.

`main.cpp`
```c++
#include <stdio.h>
#include <TrivialOpenGL.h>

int main() {
    TOGL_Data data = {};

    data.window_name        = "Icon from File";
    // Icon will show on:
    // - window title bar
    // - task bar
    data.icon_file_name     = "icon.ico";

    data.do_on_create = []() {
        puts("X - Exit");
        fflush(stdout);
    };

    data.do_on_destroy = []() {
        puts("Bye. Bye.");
        fflush(stdout);
    };

    data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
        if (key_id == 'X' && !is_down) {
            TOGL_RequestClose();
        }
    };

    return TOGL_Run(data);
}
```
[Back to: Code Examples](#code-examples)

---

### Simple Text
`main.cpp`
```c++
#include <stdio.h>
#include <TrivialOpenGL.h>

enum : uint16_t {
    FONT_SIZE = 16,   // in pixels
};

static TOGL_SizeU16 s_size = {800, 400};

int main() {
    TOGL_Data data = {};

    data.window_name        = "Simple Text";
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
        }

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

        TOGL_RenderText(
            {0, s_size.height - FONT_SIZE}, 
            {255, 255, 255, 255}, 
            "This is simple text.\nX - Exit."
        );
    };

    data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
        if (key_id == 'X' && !is_down) {
            TOGL_RequestClose();
        }
    };

    return TOGL_Run(data);
}
```
[Back to: Code Examples](#code-examples)

### Colored Text
`main.cpp`
```c++
#include <stdio.h>
#include <TrivialOpenGL.h>

enum : uint16_t {
    FONT_SIZE = 16,   // in pixels
};

static TOGL_SizeU16 s_size = {800, 400};

int main() {
    TOGL_Data data = {};

    data.window_name        = "Colored Text";
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
        }

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

        const auto text = TOGL_FineText(
            "Text with default color.\n", 
            TOGL_Color4U8(255, 0, 0, 255), "Red text. ", 
            TOGL_Color4U8(0, 255, 0, 255), "Greed text. ", 
            TOGL_Color4U8(0, 0, 255, 255), "Blue text. ", 
            TOGL_Color4U8(255, 255, 255, 127), "Transparent text. ");

        TOGL_RenderText({0, s_size.height - FONT_SIZE}, {255, 255, 255, 255}, text);
    };

    data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
        if (key_id == 'X' && !is_down) {
            TOGL_RequestClose();
        }
    };

    return TOGL_Run(data);
}
```
[Back to: Code Examples](#code-examples)

### Unicode Text
Unicode font in this example is loaded with very high number of code points (range 0000h to FFFFh).
It might take some time to load font. To inform user about this, font from smaller range is loaded (ASCII) first, 
to display message about loading unicode font.

`main.cpp`
```c++
#include <stdio.h>
#include <string>
#include <TrivialOpenGL.h>

enum : uint16_t {
    FONT_SIZE = 16,   // in pixels
};

static TOGL_SizeU16 s_size  = {800, 400};
static std::string  s_text  = ""; 

int main() {
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
        
        TOGL_RenderText({0, s_size.height - FONT_SIZE}, {255, 255, 255, 255}, s_text);
    };

    data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
        if (key_id == 'X' && !is_down) {
            TOGL_RequestClose();
        }
    };

    return TOGL_Run(data);
}
```
[Back to: Code Examples](#code-examples)

### Formated Text
Makes text to fit in specific area (only on width axis) and normalizes tab vertical alignment.

When window width is changed, then text alignment is adjusted to fit new window width.

Word wrapping works in two stages:
- if any part of word is outside max text line width (`TOGL_SetLineWrapWidth`), then whole word is moved to next line;
- if word doesn't fit in the line, then is split to multiple strings of character which each will fit in following lines.

`TOGL_SetNumberOfSpacesInTab` - All tabs are replaced by provided number of spaces. 
Tabbed text is adjusted by counting adjust from beginning of line.

`TOGL_SetLineWrapWidth` - Changes max text line width in pixels which after word are wrapped.

`TOGL_AdjustAndRenderText` - Formats text using information provided by `TOGL_SetNumberOfSpacesInTab` and `TOGL_SetLineWrapWidth`.

`main.cpp`
```c++
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

int main() {
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
```
[Back to: Code Examples](#code-examples)

### Text Box

`main.cpp`
```c++
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
            std::wstring last_text = container.GetText();
            container.SetText(last_text.substr(0, last_text.length() - 1));
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
        } else if (isprint(c)) {
            s_text += c;
        }
    };

    data.do_on_key = [](TOGL_KeyId key_id, bool is_down, const TOGL_Extra& extra) {
        if (key_id == TOGL_KEY_IDESCAPE && !is_down) {
            TOGL_RequestClose();
        }
    };

    return TOGL_Run(data);
}
```
[Back to: Code Examples](#code-examples)