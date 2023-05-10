/**
* @file IconFromFile.cpp
* @author underwatergrasshopper
*/

#include "IconFromFile.h"

#include <stdio.h>
#include <TrivialOpenGL.h>

int RunIconFromFile() {
        TOGL_Data data = {};

        data.window_name        = "Icon from File";
        // Icon will show on:
        // - window title bar
        // - task bar
        data.icon_file_name     = "..\\..\\..\\..\\TrivialOpenGL_Example\\assets\\icon.ico";

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