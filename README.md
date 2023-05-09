# TrivialOpenGL
A simple C++ header-only library that gives the ability to create an window with OpenGL context to draw in.
Includes support for drawing text.

---

## Informations
- Targeted Platform: Windows 10
- Targeted Language: C++11
- Uses Libraries: WinApi, OpenGL
- Required Dependencies: OpenGL32.lib

---

## What is included in project
Contains:
- Source code of library.
- Unit test.
- Examples.

---

## Requirements
To be able to compile an outer project, which includes this library, `OpenGL32.lib` needs to be included to the outer project.

---

## HOWTO: Proper clone repository with Git
Run `git clone --recurse-submodules https://github.com/underwatergrasshopper/TrivialOpenGL.git`
. This command will clone repository, including it's submodules.

---

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

## Builds and Tests results

Compiler: **MSVC** (automated)

| Test Platform | Target Platform | Environment | Architecture  | Build and Test |
|-|-|-|-|-|
| Windows Server 2022, Windows Server 2019 | Windows 10 |  Visual Studio 2022, Visual Studio  2019 | x86, x86_64 | [![Build and Test](https://github.com/underwatergrasshopper/TrivialOpenGL/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/underwatergrasshopper/TrivialOpenGL/actions/workflows/build_and_test.yml) |

 Compiler: **LLVM CLang MinGW** (automated)

| Test Platform | Target Platform | Environment | Architecture | Build and Test |
|-|-|-|-|-|
| Windows Server 2022 | Windows 10 | MinGW-W64 (LLVM Clang 15.0.0) | x86, x86_64 | [![Build and Test (MinGW)](https://github.com/underwatergrasshopper/TrivialOpenGL/actions/workflows/build_and_test_mingw.yml/badge.svg)](https://github.com/underwatergrasshopper/TrivialOpenGL/actions/workflows/build_and_test_mingw.yml) |

