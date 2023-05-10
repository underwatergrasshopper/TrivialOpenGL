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
- Source code of library (`include/`).
- Unit tests (`TrivialOpenGL_Test/`).
- Examples (`TrivialOpenGL_Example/`).

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

## Examples

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

### Icon from Resource
Icon imported this way will be visible on:
- executable
- window title bar
- task bar

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