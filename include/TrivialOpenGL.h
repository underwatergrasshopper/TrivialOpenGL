/**
* @file TrivialOpenGL.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_H_
#define TRIVIALOPENGL_H_

// Lib dependency: OpenGL32.lib

#include "TrivialOpenGL_Utility.h"
#include "TrivialOpenGL_Window.h"

//==========================================================================
// Declarations
//==========================================================================

namespace TrivialOpenGL {

    // Get access to window singleton.
    Window& ToWindow();

    // Creates and runs window.
    int Run(const Data& data);

} // namespace TrivialOpenGL

//==========================================================================
// Definitions
//==========================================================================

namespace TrivialOpenGL {

    inline Window& ToWindow() {
        return Window::To();
    }

    inline int Run(const Data& data) {
        return ToWindow().Run(data);
    }

} // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_H_
