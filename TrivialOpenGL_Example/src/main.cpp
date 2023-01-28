#include <stdio.h>
#include <TrivialOpenGL.h>

int main() {
    TOGL::Data data = {};

    data.window_name        = "TrivialOpenGL_Example";
    data.icon_file_name     = "..\\..\\..\\..\\TrivialOpenGL_Example\\assets\\icon.ico";

    return TOGL::Run(data);
}