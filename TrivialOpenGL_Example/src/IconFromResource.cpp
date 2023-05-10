/**
* @file Icon.h
* @author underwatergrasshopper
*/

#include "IconFromResource.h"

#include <stdio.h>
#include <TrivialOpenGL.h>

#include "Resource.h"

int RunIconFromResource() {
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