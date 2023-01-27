/**
* @file TrivialOpenGL.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_H_
#define TRIVIALOPENGL_H_

#include <string>

#include "TrivialOpenGL_Utility.h"

namespace TrivialOpenGL {

    enum {
        DEF = -1,
    };

    struct StyleBit {
        using Field = uint32_t;
        enum {
            NO_RESIZE               = 0x0001,
            NO_MAXIMIZE             = 0x0002,
            CENTERED                = 0x0004,
            CLIENT_SIZE             = 0x0008,
            CLIENT_ONLY             = 0x000F,
        };
    };

    struct Data {
        // Encoding: ASCII or UTF8.
        std::string     window_name     = "";

        // If x is DEF then created window will be centered on X axis.
        // If y is DEF then created window will be centered on Y axis.
        // If width is DEF then created window will have with equal to half width of the screen.
        // If height is DEF then created window will have with equal to half height of the screen.
        AreaI           area            = {DEF, DEF, DEF, DEF};

        StyleBit::Field style           = 0;
    };

    class Window {
    public:
        Window() {}
        virtual ~Window() {}

        int Run(const Data& data) {
            return 0;
        }
    private:
    };

    inline Window& ToWindow() {
        return Static<Window>::To();
    }

    inline int Run(const Data& data) {
        return ToWindow().Run(data);
    }

} // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_H_
