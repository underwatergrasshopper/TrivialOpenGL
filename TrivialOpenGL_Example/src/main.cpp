/**
* @file TrivialOpenGL.h
* @author underwatergrasshopper
*/

#include <stdio.h>

#include <string>
#include <set>

#include <TimeLapse.h>
#include <TrivialOpenGL.h>

#include "Resource.h"

//------------------------------------------------------------------------------

void DrawRectangle(int x, int y, int width, int height) {
    glBegin(GL_TRIANGLE_FAN);

    glVertex2i(x,           y);
    glVertex2i(x + width,   y);
    glVertex2i(x + width,   y + height);
    glVertex2i(y,           y + height);

    glEnd();
};

void DrawTriangle(float x, float y, float scale, float angle) {
    glPushMatrix();

    glTranslatef(x, y, 0);
    glRotatef(angle, 0, 0, 1);
    glScalef(scale, scale, 1);

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

void DrawInfoText(const std::string& text) {
    const auto list = TOGL::Split(text, '\n');

    int y = TOGL::ToWindow().GetDrawAreaSize().height;
    for (const auto& line : list) {
        y -= TOGL::ToWindow().GetTextSize(line).height;
        TOGL::ToWindow().RenderText(10, y, 255, 255, 255, 255, line);
    }
}

class TestImage {
public:
    TestImage() {
        m_interval      = 0.1;   
        m_speed         = 10;  

        m_step          = m_speed * m_interval;   

        m_accumulator   = 0; 
        m_angle         = 0;    
    }

    virtual ~TestImage() {

    }

    void Initialize(uint16_t width, uint16_t height) {
        Initialize({width, height});
    }

    void Initialize(const TOGL::SizeU16& size) {
        m_size = size;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, m_size.width, 0, m_size.height, 1, -1);

        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

        m_time_lapse.Reset();
    }

    // angle - in degrees
    void Draw(float triangle_angle = 0) {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // frame
        glColor3f(1, 0, 0);
        DrawRectangle(0, 0, m_size.width, m_size.height);

        // background
        glColor3f(0, 0, 0.5);
        DrawRectangle(1, 1, m_size.width - 2, m_size.height - 2);

        // triangle
        DrawTriangle(m_size.width / 2.0f, m_size.height / 2.0f, m_size.height / 3.0f, triangle_angle);
    }

    void Animate() {
        glClear(GL_COLOR_BUFFER_BIT);
        
        m_time_lapse.Update();
        m_accumulator += m_time_lapse.Get();

        if (m_accumulator > m_interval) {
            while (m_accumulator > m_interval) {
                m_angle += (int)m_step;
                m_accumulator -= m_interval;
            }
            m_angle %= 360;
        }

        Draw((float)m_angle);
    }

    void Resize(uint16_t width, uint16_t height) {
        glViewport(0, 0, width,  height ? height : 1);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, 0, height, 1, -1);

        m_size = {width, height};

        TOGL::ToWindow().RequestRedraw();
    }

private:
    TOGL::SizeU16   m_size;

    double          m_interval;         // in seconds
    double          m_speed;            // in degrees per second
    double          m_step;             // in degrees

    TimeLapseF64    m_time_lapse;
    double          m_accumulator;      // time in seconds
    int             m_angle;            // in degrees
};

void PrintWindowStates() {
    puts("NHMmFf");
    printf("%s%s%s%s%s%s\n", 
        TOGL::ToWindow().IsNormal() ? "+" : " ",
        TOGL::ToWindow().IsVisible() == false ? "+" : " ",
        TOGL::ToWindow().IsWindowMaximized() ? "+" : " ",
        TOGL::ToWindow().IsWindowMinimized() ? "+" : " ",
        " ",
        TOGL::ToWindow().IsWindowedFullScreen() ? "+" : " ");
}

void DisplayWindowInfo() {
    GLint data[2];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, data);
    printf("max viewport width: %d\n", data[0]);
    printf("max viewport height: %d\n", data[1]);

    togl_print_i32(GetSystemMetrics(SM_CXMINTRACK));
    togl_print_i32(GetSystemMetrics(SM_CYMINTRACK));
    togl_print_i32(GetSystemMetrics(SM_CXMAXTRACK));
    togl_print_i32(GetSystemMetrics(SM_CYMAXTRACK));

    puts("---");

    RECT client, window, border;

    HWND window_handle = GetForegroundWindow();

    GetWindowRect(window_handle, &window);
    GetClientRect(window_handle , &client);
    MapWindowPoints(window_handle, NULL, (LPPOINT)&client, 2);

    border.left     = client.left - window.left;
    border.top      = client.top - window.top;
    border.right    = window.right - client.right;
    border.bottom   = window.bottom - client.bottom;

    

    togl_print_i32(client.left);
    togl_print_i32(client.top);
    togl_print_i32(client.right);
    togl_print_i32(client.bottom);

    togl_print_i32(window.left);
    togl_print_i32(window.top);
    togl_print_i32(window.right);
    togl_print_i32(window.bottom);

    togl_print_i32(border.left);
    togl_print_i32(border.top);
    togl_print_i32(border.right);
    togl_print_i32(border.bottom);
    puts("");

    const TOGL::SizeU16 screen_size = TOGL::GetScreenSize();
    togl_print_i32(screen_size.width);
    togl_print_i32(screen_size.height);

    const TOGL::SizeU16 work_area_size = TOGL::GetDesktopAreaSizeNoTaskBar();
    togl_print_i32(work_area_size.width);
    togl_print_i32(work_area_size.height);
    puts("");

    const TOGL::AreaIU16 window_area = TOGL::ToWindow().GetArea();
    togl_print_i32(window_area.x);
    togl_print_i32(window_area.y);
    togl_print_i32(window_area.width);
    togl_print_i32(window_area.height);
    puts("");

    const TOGL::AreaIU16 window_draw_area = TOGL::ToWindow().GetDrawArea();
    togl_print_i32(window_draw_area.x);
    togl_print_i32(window_draw_area.y);
    togl_print_i32(window_draw_area.width);
    togl_print_i32(window_draw_area.height);
    puts("");

    puts("Window Center Check:");
    togl_print_i32(window_area.width + (window_area.x * 2));
    togl_print_i32(window_area.height + (window_area.y * 2));
    togl_print_i32(work_area_size.width);
    togl_print_i32(work_area_size.height);

    PrintWindowStates();
}

//------------------------------------------------------------------------------

class DelayedActionManager {
public:
    DelayedActionManager() {}
    virtual ~DelayedActionManager() {}

    using RunFnP_T = void (*)();

    // delay - in seconds
    void Add(double delay, RunFnP_T run) {
        m_actions.push_back({delay, run});
        m_time_lapse.Reset();
    }

    void Run() {
        m_time_lapse.Update();

        if (!m_actions.empty()) {
            Action& action = m_actions[0];

            action.delay -= m_time_lapse.Get();
            if (action.delay <= 0.0) {
                action.run();
                m_actions.erase(m_actions.begin());
            }
        }
    }
private:
    struct Action {
        double      delay;
        RunFnP_T    run;
    };

    std::vector<Action>     m_actions;
    TimeLapseF64            m_time_lapse;
};

//------------------------------------------------------------------------------

enum {
    FONT_SIZE = 16, // in pixels
};

static TOGL::Window&    s_window                    = TOGL::ToWindow();
static TOGL::SizeU16    s_resolution;
static bool             s_is_client;
static TestImage        s_test_image;
static bool             s_is_display_mose_move_data;

static DelayedActionManager s_actions;

TOGL::FontStyle         s_font_style    = TOGL::FONT_STYLE_NORMAL;
TOGL::FontCharSet       s_font_char_set = TOGL::FONT_CHAR_SET_ENGLISH;

TOGL::Font              s_font;

//------------------------------------------------------------------------------



class ExampleManager {
public:
    using RunExampleFnP_T = int (*)(const std::string& name, const std::set<std::string>& options);

    ExampleManager() : m_buffer{} {}
    virtual ~ExampleManager() {}

    void SetDefaultExample(const std::string& name) {
        m_default_example_name = name;
    }

    void AddExample(const std::string& name, const std::set<std::string>& all_options, const std::set<std::string>& default_options, RunExampleFnP_T run) {
        m_examples.push_back({name, all_options, default_options, run});
    }

    int Run(const std::vector<std::string>& arguments) {
        int result = 0;

        if (arguments.size() > 0) {
            std::string name = arguments[0];

            std::set<std::string> options;
            for (size_t index = 1; index < arguments.size(); ++index) options.insert(arguments[index]);

            Example* example = Find(name);
            if (!example) {
                puts("");
                printf("Example Error: Can not find example with name \"%s\".\n", name.c_str());
                result = EXIT_FAILURE;
            } else {
                result = example->run(name, options);
            }
        } else {
            result = Run();
        }

        return result;
    }

    int Run() {
        int result = 0;

        while (true) {
            puts("");
            puts("--- Example Manager ---");
            for (const auto& example : m_examples) printf("%s, ", example.name.c_str());
            printf("%s", "\n\n");

            printf("(e=exit, d=%s)\n", m_default_example_name.c_str());
            printf("%s", "Example: ");
            fflush(stdout);

            std::string name;

            if (!GetStdIn(name)) {
                puts("");
                puts("Example Error: Can not read argument.");
                result = EXIT_FAILURE;
                break;
            } 
            puts("");

            if (name == "d") {
                name = m_default_example_name;
            } else if (name == "e") {
                result = EXIT_SUCCESS;
                break;
            } 

            Example* example = Find(name);
            if (!example) {
                printf("Example Error: Can not find example with name \"%s\".\n", name.c_str());
                continue;
            }

            for (const auto& option : example->all_options) printf("%s, ", option.c_str());
            printf("%s", "\n\n");

            std::string raw_default_options;
            for (const auto& option : example->default_options) raw_default_options += option + " ";

            printf("(e=exit, d=%s)\n", raw_default_options.c_str());
            printf("%s", "Options: ");
            fflush(stdout);

            std::string raw_options;

            if (!GetStdIn(raw_options)) {
                puts("");
                puts("Example Error: Can not read arguments.");
                continue;
            } 
            puts("");

            if (raw_options == "d") {
                raw_options = raw_default_options;
            } else if (raw_options == "e") {
                result = EXIT_SUCCESS;
                break;
            } 

            std::set<std::string> options;
            for (const auto& option : RemoveEmpty(TOGL::Split(raw_options, ' '))) options.insert(option);

            auto IsContainAllIn = [](const std::set<std::string>& elements, const std::set<std::string>& container) -> std::string {
                for (const auto& element : elements) {
                    if (container.find(element) == container.end()) {
                        return element;
                    }
                }
                return "";
            };

            const std::string missing_option = IsContainAllIn(options, example->all_options);
            if (!missing_option.empty()) {
                printf("Example Error: Unknown option \"%s\".\n", missing_option.c_str());
                continue;
            }

            result = example->run(name, options);
            if (result != EXIT_SUCCESS) {
                printf("Example Error: Example returned error code %d.\n", result);
            }
        }

        return result;
    }


private:
    enum { BUFFER_SIZE = 4096 };

    struct Example {
        std::string             name;
        std::set<std::string>   all_options;
        std::set<std::string>   default_options;

        RunExampleFnP_T         run;
    };

    // Removes empty strings.
    std::vector<std::string> RemoveEmpty(std::vector<std::string> poluted_arguments) {
        std::vector<std::string> arguments;
        for (const auto& argument : poluted_arguments) if (!argument.empty()) arguments.push_back(argument);
        return arguments;
    }

    Example* Find(const std::string& name) {
        for (auto& example : m_examples) {
            if (example.name == name) return &example;
        }
        return nullptr;
    }

    bool GetStdIn(std::string& in_text) {
        memset(m_buffer, 0, sizeof(m_buffer));

        if (!fgets(m_buffer, BUFFER_SIZE - 1, stdin)) {
            puts("Example Error: Can not read arguments.");

            in_text = "";
            return false;
        }

        // Removes new line character from string end.
        size_t size = strlen(m_buffer);
        if (size > 0 && m_buffer[size - 1] == '\n') m_buffer[size - 1] = '\0';

        in_text = std::string(m_buffer);
        return true;
    }

    std::vector<Example>    m_examples;
    std::string             m_default_example_name;
    char                    m_buffer[BUFFER_SIZE];
};

//------------------------------------------------------------------------------

// Saves pixel data as BMP image.
// file_name            - Full file name with bmp extension.
// pixel_data           - Array of pixels. Each pixel occupies 4 bytes of data. 
//                        Each byte of pixel corresponds to specific channel (in order): red, green, blue, alpha.
// width                - Number of pixel in row of image.
// height               - Number of pixel in column of image.  
// Returns true if image has been saved to file. 
bool SaveAsBMP(const std::string& file_name, const uint8_t* pixel_data, uint32_t width, uint32_t height, bool is_reverse_rows = true) {
    bool is_success = false; 

    if (width != 0 && height != 0) {
        enum {
            PIXEL_SIZE = 4, // in bytes
        };

        const uint32_t pixel_data_size      = width * height * PIXEL_SIZE;
        const uint32_t header_size          = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPV5HEADER);
        const uint32_t file_size            = header_size + pixel_data_size;

        uint8_t* file_data = new uint8_t[file_size];
        memset(file_data, 0, file_size);

        BITMAPFILEHEADER* fh = (BITMAPFILEHEADER*) file_data;
        fh->bfType          = 0x4D42; // bmp file signature = {'B', 'M'}
        fh->bfSize          = file_size;
        fh->bfOffBits       = header_size;

        BITMAPV5HEADER* ih = (BITMAPV5HEADER*)(file_data + sizeof(BITMAPFILEHEADER));
        ih->bV5Size         = sizeof(BITMAPV5HEADER);
        ih->bV5Width        = width;
        ih->bV5Height       = height;
        ih->bV5Planes       = 1;
        ih->bV5BitCount     = 32;
        ih->bV5Compression  = BI_BITFIELDS;
        ih->bV5SizeImage    = pixel_data_size;
        ih->bV5RedMask      = 0xFF000000;
        ih->bV5GreenMask    = 0x00FF0000;
        ih->bV5BlueMask     = 0x0000FF00;
        ih->bV5AlphaMask    = 0x000000FF;
        ih->bV5CSType       = LCS_sRGB;
        ih->bV5Intent       = LCS_GM_GRAPHICS;

        uint8_t* file_pixel_data = (uint8_t*)(file_data + header_size);
        uint32_t ix = 0;

        const uint32_t row_size = width * PIXEL_SIZE; // in bytes

        for (uint32_t column_ix = 0; column_ix < height; ++column_ix) { // reverse order of rows
            const uint8_t* column =  pixel_data + row_size * (is_reverse_rows ? (height - 1 - column_ix) : column_ix);

            for (uint32_t pixel_ix = 0; pixel_ix < width; ++pixel_ix) {
                const uint8_t* pixel = column + pixel_ix * PIXEL_SIZE;

                // reverse order of channels
                file_pixel_data[ix++] = pixel[3];
                file_pixel_data[ix++] = pixel[2];
                file_pixel_data[ix++] = pixel[1];
                file_pixel_data[ix++] = pixel[0];
            }
        }

        FILE* file = NULL;
        is_success = _wfopen_s(&file, TOGL::ToUTF16(file_name).c_str(), L"wb") == 0 && file;

        if (is_success) {
            is_success = fwrite(file_data, 1, file_size, file) == file_size;
            fclose(file);
        }

        delete[] file_data;
    }

    return is_success;
}

//------------------------------------------------------------------------------


template <typename Type>
void DisplayBits(const Type& value) {
    enum {
        NUM_OF_BITS_IN_BYTE = 8,
    };
    const Type num_of_bits = sizeof(Type) * NUM_OF_BITS_IN_BYTE;

    for (uint32_t ix = 0; ix < num_of_bits; ++ix) {
        if (ix != 0 && ix % 4 == 0) printf("%s", "`");

        bool is = value & Type(1 << (num_of_bits - (ix + 1)));
        printf("%s", is ? "1" : "0");
    }
}

constexpr auto DisplayBits8  = DisplayBits<uint8_t>;
constexpr auto DisplayBits16 = DisplayBits<uint16_t>;
constexpr auto DisplayBits32 = DisplayBits<uint32_t>;

#define DISPLAY_BITS_32(variable) { printf("%32s ", #variable); DisplayBits32(variable); puts(""); } (void)0


void DisplayCharsets() {
    DISPLAY_BITS_32(ANSI_CHARSET);
    DISPLAY_BITS_32(DEFAULT_CHARSET);
    DISPLAY_BITS_32(SYMBOL_CHARSET);
    DISPLAY_BITS_32(SHIFTJIS_CHARSET);
    DISPLAY_BITS_32(HANGEUL_CHARSET);
    DISPLAY_BITS_32(HANGUL_CHARSET);
    DISPLAY_BITS_32(GB2312_CHARSET);
    DISPLAY_BITS_32(CHINESEBIG5_CHARSET);
    DISPLAY_BITS_32(OEM_CHARSET);
    DISPLAY_BITS_32(JOHAB_CHARSET);
    DISPLAY_BITS_32(HEBREW_CHARSET);
    DISPLAY_BITS_32(ARABIC_CHARSET);
    DISPLAY_BITS_32(GREEK_CHARSET);
    DISPLAY_BITS_32(TURKISH_CHARSET);
    DISPLAY_BITS_32(VIETNAMESE_CHARSET);
    DISPLAY_BITS_32(THAI_CHARSET);
    DISPLAY_BITS_32(EASTEUROPE_CHARSET);
    DISPLAY_BITS_32(RUSSIAN_CHARSET);
    puts("");
    DISPLAY_BITS_32(FS_LATIN1);
    DISPLAY_BITS_32(FS_LATIN2);
    DISPLAY_BITS_32(FS_CYRILLIC);
    DISPLAY_BITS_32(FS_GREEK);
    DISPLAY_BITS_32(FS_TURKISH); 
    DISPLAY_BITS_32(FS_HEBREW);
    DISPLAY_BITS_32(FS_ARABIC);
    DISPLAY_BITS_32(FS_BALTIC);
    DISPLAY_BITS_32(FS_VIETNAMESE);
    DISPLAY_BITS_32(FS_THAI);
    DISPLAY_BITS_32(FS_JISJAPAN);   
    DISPLAY_BITS_32(FS_CHINESESIMP);
    DISPLAY_BITS_32(FS_WANSUNG);    
    DISPLAY_BITS_32(FS_CHINESETRAD);
    DISPLAY_BITS_32(FS_JOHAB);      
    DISPLAY_BITS_32(FS_SYMBOL); 
}

void DisplayUnicodeRangesForCurrentFont(HDC device_context_handle) {
    TEXTMETRICW metric;
    GetTextMetricsW(device_context_handle, &metric);
    togl_print_i32(metric.tmFirstChar);
    togl_print_i32(metric.tmLastChar);
    togl_print_i32(metric.tmCharSet);

    DWORD buffer_size = GetFontUnicodeRanges(device_context_handle, NULL);
    BYTE* buffer = new BYTE[buffer_size];

    GLYPHSET* glyphset = (GLYPHSET*)buffer;
    GetFontUnicodeRanges(device_context_handle, glyphset);

    for (uint32_t ix = 0; ix < glyphset->cRanges; ++ix) {
        const uint32_t from = glyphset->ranges[ix].wcLow;
        const uint32_t to = from + glyphset->ranges[ix].cGlyphs - 1;
        printf("[%04X..%04X]\n", from, to);
    }

    delete[] buffer;
}

class TestFont {
public:

    TestFont() {
        m_font_handle               = NULL;
        m_window_handle             = NULL;
        m_device_contect_handle     = NULL;

        m_list_range                = 0;
        m_list_base                 = 0;

        m_descent                   = 0;
    }
    virtual ~TestFont() {}

    void Create(uint32_t width, uint32_t height) {
        m_window_handle             = GetForegroundWindow();
        m_device_contect_handle     = GetDC(m_window_handle); 

        m_font_handle = CreateFontW(
            32,                    
            0, 0, 0,                            
            FW_NORMAL,
            FALSE, FALSE, FALSE,

            // ANSI_CHARSET - for W version of function should by unicode to range 0020..FFFC (hex)
            // CHINESEBIG5_CHARSET
            ANSI_CHARSET,               

            OUT_TT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            ANTIALIASED_QUALITY,
            FF_DONTCARE | DEFAULT_PITCH,
            L"Courier New");

        HFONT prev_font = (HFONT)SelectObject(m_device_contect_handle, m_font_handle); 

        DisplayUnicodeRangesForCurrentFont(m_device_contect_handle);

        TEXTMETRICW metric;
        GetTextMetricsW(m_device_contect_handle, &metric);
        m_list_range = metric.tmLastChar;
        m_descent = metric.tmDescent;

        togl_print_i32(metric.tmHeight);
        togl_print_i32(metric.tmAscent);
        togl_print_i32(metric.tmDescent);
        togl_print_i32(metric.tmInternalLeading);
        togl_print_i32(metric.tmExternalLeading);

        // For outline fonts.
        //OUTLINETEXTMETRICW ol_metric = {}; 
        //ol_metric.otmSize = sizeof(OUTLINETEXTMETRICW);
        //GetOutlineTextMetricsW(m_device_contect_handle, sizeof(OUTLINETEXTMETRICW), &ol_metric);
        //togl_print_i32(ol_metric.otmAscent);
        //togl_print_i32(ol_metric.otmDescent);
        //togl_print_i32(ol_metric.otmMacAscent);
        //togl_print_i32(ol_metric.otmMacDescent);
        //togl_print_i32(ol_metric.otmMacLineGap);


        m_list_base = glGenLists(m_list_range);
        togl_print_i32(m_list_base);

        bool is_success = wglUseFontBitmapsW(m_device_contect_handle, 0, m_list_range, m_list_base);
        togl_print_i32(is_success);

        SelectObject(m_device_contect_handle, prev_font); 

        // ---

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, 0, height, 1, -1);

        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    }

    void Destroy() {
        glDeleteLists(m_list_base, m_list_range);
        ReleaseDC(m_window_handle, m_device_contect_handle);
        DeleteObject(m_font_handle);
    }

    void Render() {
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        DrawTriangle(100, 100, 100, 0);

        // ---

        glPushAttrib(GL_LIST_BIT);
        glListBase(m_list_base);
    
        // for unicode would be something like ...
        //const char32_t text[] = U"\u015B Some Text \U00024B62";
        //glCallLists((GLsizei)sizeof(text), GL_UNSIGNED_INT, text);

        {
            glColor3f(1, 1, 1);
            glRasterPos2i(100, 100);

            const wchar_t text[] = L"\u015B Some text \u0444 \uFEA2 \uFF86 \uAC37";
            glCallLists((GLsizei)wcslen(text), GL_UNSIGNED_SHORT, text);
        }
        {
            glColor3f(1, 1, 1);
            glRasterPos2i(0, 50);

            const wchar_t text[] = L"\u015AXj";
            glCallLists((GLsizei)wcslen(text), GL_UNSIGNED_SHORT, text);
        }

        {
            glColor3f(1, 1, 1);
            glRasterPos2i(0, 0 + m_descent);

            const wchar_t text[] = L"\u015AXj";
            glCallLists((GLsizei)wcslen(text), GL_UNSIGNED_SHORT, text);
        }
        glPopAttrib();
    }

private:
    HFONT   m_font_handle;
    HWND    m_window_handle;
    HDC     m_device_contect_handle;

    GLsizei m_list_range;
    GLint   m_list_base;

    int     m_descent;
};

static TestFont s_test_font;

//------------------------------------------------------------------------------

class FPS {
public:
    FPS() {
        m_frame_count   = 0;
        m_fps           = 0;

        m_time_lapse.Reset();
    }
    virtual ~FPS() {}

    uint32_t Measure() {
        m_frame_count += 1;
        if (m_time_lapse.UpdateIfAbove(1.0)) {
              m_fps = uint32_t(m_frame_count / m_time_lapse.Get());
              m_frame_count = 0; 
        }
        return m_fps;
    }

private:
    uint32_t        m_frame_count;
    uint32_t        m_fps;
    TimeLapseF64    m_time_lapse;
};

static FPS s_fps;

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

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.icon_file_name     = "..\\..\\..\\..\\TrivialOpenGL_Example\\assets\\icon.ico";

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // icon_resource
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("icon_resource", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.icon_resource_id   = ICON_ID;

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // custom_size
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("custom_size", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, 400, 800};

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // custom_area
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("custom_area", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {100, 200, 400, 800};

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // no_area
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("no_area", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, 0, 0};

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // no_resize
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("no_resize", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.style              = TOGL::StyleBit::NO_RESIZE;

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // no_maximize
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("no_maximize", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.style              = TOGL::StyleBit::NO_MAXIMIZE;

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // no_maximize_and_resize
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("no_maximize_and_resize", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.style              = TOGL::StyleBit::NO_MAXIMIZE | TOGL::StyleBit::NO_RESIZE;

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // center
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("center", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {0, 0, 400, 800};
        data.style              = TOGL::StyleBit::CENTERED;

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // draw_area_size
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("draw_area_size", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, 400, 200};
        data.style              = TOGL::StyleBit::DRAW_AREA_SIZE;

        return TOGL::Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // draw_area_only
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("draw_area_only", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, 400, 200};
        data.style              = TOGL::StyleBit::DRAW_AREA_ONLY;

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // redraw_on_change_or_request
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("redraw_on_change_or_request", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.style              = TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        data.log_level          = 3;

        return TOGL::Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // draw_triangle
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("draw_triangle", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.icon_resource_id   = ICON_ID;
        data.log_level          = 3;

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

        return TOGL::Run(data);
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
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {600, 300};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        data.icon_resource_id   = ICON_ID;

        if (!IsOption("no_debug"))                      data.log_level = TOGL::LOG_LEVEL_DEBUG;

        if (IsOption("draw_area_size"))                 data.style |= TOGL::StyleBit::DRAW_AREA_SIZE;
        if (IsOption("draw_area_only"))                 data.style |= TOGL::StyleBit::DRAW_AREA_ONLY;
        if (IsOption("redraw_on_change_or_request"))    data.style |= TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        if (IsOption("no_resize"))                      data.style |= TOGL::StyleBit::NO_RESIZE;
        if (IsOption("no_maximize"))                    data.style |= TOGL::StyleBit::NO_MAXIMIZE;

        if (IsOption("notify_any_message"))             data.special_debug.is_notify_any_message        = true;
        if (IsOption("notify_draw_call"))               data.special_debug.is_notify_draw_call          = true;
        if (IsOption("notify_mouse_move"))              data.special_debug.is_notify_mouse_move         = true;
        if (IsOption("notify_key_message"))             data.special_debug.is_notify_key_message        = true;
        if (IsOption("notify_character_message"))       data.special_debug.is_notify_character_message  = true;

        data.do_on_create = []() {
            s_test_image.Initialize(TOGL::ToWindow().GetDrawAreaSize());

            if (!TOGL::ToWindow().LoadFont("Courier New", FONT_SIZE)) {
                puts("Error: Can not load font.");
                puts(TOGL::ToWindow().GetLoadFontErrMsg().c_str());
            } else {
                puts("Font loaded.");
            }
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

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            auto& window = TOGL::ToWindow();

            if (!is_down) {
                if (!s_is_client) {
                    switch (key_id) {
                    case TOGL::KEY_ID_1:            window.MoveTo(0, 0); break;
                    case TOGL::KEY_ID_2:            window.MoveTo(10, 100); break;

                    case TOGL::KEY_ID_3:            window.SetSize(400, 200); break;
                    case TOGL::KEY_ID_4:            window.SetSize(800, 400); break;

                    case TOGL::KEY_ID_5:            window.SetArea(100, 10, 800, 400); break;
                    case TOGL::KEY_ID_6:            window.SetArea(window.GetArea()); break;
                    case TOGL::KEY_ID_7:            window.SetArea(TOGL::GetDesktopAreaNoTaskBar()); break;
                    case TOGL::KEY_ID_8:            window.SetArea({{}, TOGL::GetScreenSize()}); break;

                    case TOGL::KEY_ID_C:            window.Center(s_resolution, s_is_client); break;

                    case TOGL::KEY_ID_ARROW_LEFT:   window.MoveBy(-30, 0); break;
                    case TOGL::KEY_ID_ARROW_RIGHT:  window.MoveBy(30, 0); break;

                    case TOGL::KEY_ID_X:            window.RequestClose(); break;

                    case TOGL::KEY_ID_I:            DisplayWindowInfo(); break;

                    case TOGL::KEY_ID_T:            s_is_client = !s_is_client; break;
                    default: break;
                    }
                } else {
                    switch (key_id) {
                    case TOGL::KEY_ID_1:            window.MoveTo(0, 0, true); break;
                    case TOGL::KEY_ID_2:            window.MoveTo(10, 100, true); break;

                    case TOGL::KEY_ID_3:            window.SetSize(400, 200, true); break;
                    case TOGL::KEY_ID_4:            window.SetSize(800, 400, true); break;

                    case TOGL::KEY_ID_5:            window.SetArea(100, 10, 800, 400, false); break;
                    case TOGL::KEY_ID_6:            window.SetArea(window.GetDrawArea(), false); break;
                    case TOGL::KEY_ID_7:            window.SetArea(TOGL::GetDesktopAreaNoTaskBar(), false); break;
                    case TOGL::KEY_ID_8:            window.SetArea({{}, TOGL::GetScreenSize()}, false); break;

                    case TOGL::KEY_ID_C:            window.Center(s_resolution, s_is_client); break;

                    case TOGL::KEY_ID_ARROW_LEFT:   window.MoveBy(-30, 0); break;
                    case TOGL::KEY_ID_ARROW_RIGHT:  window.MoveBy(30, 0); break;

                    case TOGL::KEY_ID_X:            window.RequestClose(); break;

                    case TOGL::KEY_ID_I:            DisplayWindowInfo(); break;

                    case TOGL::KEY_ID_T:            s_is_client = !s_is_client; break;
                    default: break;
                    }
                }
            }
        };

        return TOGL::Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // font
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("font", Combine(all_options, {"unicode", "bold"}), {}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {600, 300};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        data.icon_resource_id   = ICON_ID;

        if (!IsOption("no_debug"))                      data.log_level = TOGL::LOG_LEVEL_DEBUG;

        if (IsOption("draw_area_size"))                 data.style |= TOGL::StyleBit::DRAW_AREA_SIZE;
        if (IsOption("draw_area_only"))                 data.style |= TOGL::StyleBit::DRAW_AREA_ONLY;
        if (IsOption("redraw_on_change_or_request"))    data.style |= TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        if (IsOption("no_resize"))                      data.style |= TOGL::StyleBit::NO_RESIZE;
        if (IsOption("no_maximize"))                    data.style |= TOGL::StyleBit::NO_MAXIMIZE;

        if (IsOption("notify_any_message"))             data.special_debug.is_notify_any_message        = true;
        if (IsOption("notify_draw_call"))               data.special_debug.is_notify_draw_call          = true;
        if (IsOption("notify_mouse_move"))              data.special_debug.is_notify_mouse_move         = true;
        if (IsOption("notify_key_message"))             data.special_debug.is_notify_key_message        = true;
        if (IsOption("notify_character_message"))       data.special_debug.is_notify_character_message  = true;

        s_font_char_set = IsOption("unicode") ? TOGL::FONT_CHAR_SET_RANGE_0000_FFFF : TOGL::FONT_CHAR_SET_ENGLISH;
        s_font_style    = IsOption("bold") ? TOGL::FONT_STYLE_BOLD : TOGL::FONT_STYLE_NORMAL;

        data.do_on_create = []() {
            s_test_image.Initialize(TOGL::ToWindow().GetDrawAreaSize());

            if (!TOGL::ToWindow().LoadFont("Courier New", FONT_SIZE, s_font_style, s_font_char_set)) {
                puts("Error: Can not load font.");
                puts(TOGL::ToWindow().GetLoadFontErrMsg().c_str());
            } else {
                puts("Font loaded.");
            }
            //TOGL::ToWindow().SaveFont("courier_new.bmp", 1024, 1024);

            s_font.Load("Courier New", FONT_SIZE, TOGL::FONT_SIZE_UNIT_PIXELS, s_font_style, s_font_char_set);

            if (!s_font.IsOk()) {
                puts(s_font.GetErrMsg().c_str());
            }

            s_font.SaveAsBMP("");
        };

        data.draw = []() {
            s_test_image.Animate();
            
            TOGL::Window& window = TOGL::ToWindow();
            
            int y = window.GetDrawArea().height - FONT_SIZE;
            window.RenderText(50, y, 255, 255, 255, 255, std::string() + "FPS: " + std::to_string(s_fps.Measure()));
            
            y -= FONT_SIZE;
            window.RenderTextASCII(50, y, 255, 0, 0, 255, "Some ASCII text.");
            
            y -= FONT_SIZE;
            window.RenderText(50, y, 0, 255, 0, 255, u8"Some UNICODE text \u015B \u0444 \uFEA2 \uFF86 \uAC37 \u015A \u4B14 jx");
            
            y -= FONT_SIZE;
            window.RenderText(50, y, 255, 255, 255, 127, "Some transparent text.");
            
            window.RenderText(0, 0, 0, 255, 0, 255, u8"\u015Ajx");
            window.RenderText(100, window.GetFontDescent(), 0, 255, 0, 255, u8"\u015Ajx"); // do not cut underline of text

            glColor3f(1, 0.5, 0);
            glPushMatrix();
            glTranslatef(50, 100, 0);
            glScaled(3, 3, 1);
            s_font.RenderGlyphs(u8"Somme text. Xj\u3400\u5016\u9D9B\u0001");
            glPopMatrix();
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            s_test_image.Resize(width, height);
        };

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            auto& window = TOGL::ToWindow();

            if (!is_down && key_id == 'X') window.RequestClose();
        };
        
        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // window_state
    ////////////////////////////////////////////////////////////////////////////////

    std::set<std::string> window_state_all_options = Combine(all_options, {"full_screen_at_start"});

    example_manager.AddExample("window_state", window_state_all_options, {"draw_area_size"}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {800, 400};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        data.icon_resource_id   = ICON_ID;

        if (!IsOption("no_debug"))                      data.log_level = TOGL::LOG_LEVEL_DEBUG;

        if (IsOption("draw_area_size"))                 data.style |= TOGL::StyleBit::DRAW_AREA_SIZE;
        if (IsOption("draw_area_only"))                 data.style |= TOGL::StyleBit::DRAW_AREA_ONLY;
        if (IsOption("redraw_on_change_or_request"))    data.style |= TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        if (IsOption("no_resize"))                      data.style |= TOGL::StyleBit::NO_RESIZE;
        if (IsOption("no_maximize"))                    data.style |= TOGL::StyleBit::NO_MAXIMIZE;

        if (IsOption("notify_any_message"))             data.special_debug.is_notify_any_message        = true;
        if (IsOption("notify_draw_call"))               data.special_debug.is_notify_draw_call          = true;
        if (IsOption("notify_mouse_move"))              data.special_debug.is_notify_mouse_move         = true;
        if (IsOption("notify_key_message"))             data.special_debug.is_notify_key_message        = true;
        if (IsOption("notify_character_message"))       data.special_debug.is_notify_character_message  = true;

        if (IsOption("full_screen_at_start")) {
            data.do_on_create = []() {
                s_test_image.Initialize(TOGL::ToWindow().GetDrawAreaSize());

                TOGL::ToWindow().GoWindowedFullScreen();

                if (!TOGL::ToWindow().LoadFont("Courier New", FONT_SIZE)) {
                    puts("Error: Can not load font.");
                    puts(TOGL::ToWindow().GetLoadFontErrMsg().c_str());
                } else {
                    puts("Font loaded.");
                }
            };
        } else {
            data.do_on_create = []() {
                s_test_image.Initialize(TOGL::ToWindow().GetDrawAreaSize());

                if (!TOGL::ToWindow().LoadFont("Courier New", FONT_SIZE)) {
                    puts("Error: Can not load font.");
                    puts(TOGL::ToWindow().GetLoadFontErrMsg().c_str());
                } else {
                    puts("Font loaded.");
                }
            };
        }

        data.draw = []() {
            s_test_image.Animate();

            DrawInfoText(
                std::string() +
                "FPS: " + std::to_string(s_fps.Measure()) + "\n"
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
                "8 - Native Full Screen (experimental)\n"
                "9 - restore from Native Full Screen (experimental)\n"
                "I - Display Info\n"
                "D - on/off TOGL Debug\n"
            );

            s_actions.Run();
        };

        data.do_on_resize = [](uint16_t width, uint16_t height) {
            s_test_image.Resize(width, height);

            std::string message = "Resize: ";
            message += std::to_string(width) + " " + std::to_string(height);
            puts(message.c_str());
        };

        data.do_on_state_change = [](TOGL::WindowState window_state) {
            std::string message = "Changed Window State to: ";

            switch (window_state) {
            case TOGL::WindowState::NORMAL:                 message += "NORMAL"; break;
            case TOGL::WindowState::MAXIMIZED:              message += "MAXIMIZED"; break;
            case TOGL::WindowState::MINIMIZED:              message += "MINIMIZED"; break;
            case TOGL::WindowState::WINDOWED_FULL_SCREENED: message += "WINDOWED_FULL_SCREENED"; break;
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

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            if (!is_down) {

                switch (key_id) {

                case 'X':               TOGL::ToWindow().RequestClose(); break;
                case TOGL::KEY_ID_R:    TOGL::ToWindow().RequestRedraw(); break;
                case TOGL::KEY_ID_C:    TOGL::ToWindow().Center(s_resolution, false); break;
                case TOGL::KEY_ID_V:    TOGL::ToWindow().Center(s_resolution, true); break;

                case 'F':
                    s_actions.Add(0, [](){
                        // Moves behind window to foreground.
                        SetForegroundWindow(GetNextWindow(GetForegroundWindow(), GW_HWNDNEXT));
                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL::ToWindow().GoForeground();
                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL::ToWindow().Minimize();
                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL::ToWindow().Center(s_resolution);
                        //TOGL::ToWindow().GoForeground(); // redundant
                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL::ToWindow().Hide();
                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        puts("---");
                    });

                    s_actions.Add(1, [](){

                        TOGL::ToWindow().Show();
                        //TOGL::ToWindow().GoForeground(); // redundant
                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        puts("Finished");
                    });
                    break;

                case TOGL::KEY_ID_ARROW_LEFT:
                    TOGL::ToWindow().MoveBy(-30, 0);
                    break;

                case TOGL::KEY_ID_ARROW_RIGHT:
                    TOGL::ToWindow().MoveBy(30, 0);
                    break;

                case TOGL::KEY_ID_ARROW_UP:
                    TOGL::ToWindow().MoveBy(0, -30);
                    break;

                case TOGL::KEY_ID_ARROW_DOWN:
                    TOGL::ToWindow().MoveBy(0, 30);
                    break;

                case TOGL::KEY_ID_0: 
                    s_actions.Add(0, [](){
                        puts("--- Show --");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();
                    });
                    
                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL::ToWindow().Show(); 
                        PrintWindowStates();
                    });
                    
                    s_actions.Add(1, [](){
                        puts("--- Restore ---");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();
                    });
                    
                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL::ToWindow().SetSize(s_resolution, TOGL::ToWindow().GetStyle() & TOGL::StyleBit::DRAW_AREA_SIZE); 
                        PrintWindowStates();
                    });
                    
                    s_actions.Add(1, [](){
                        puts("--- Minimize ---");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();
                    });
                    
                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL::ToWindow().Minimize(); 
                        PrintWindowStates();
                    
                    });

                    s_actions.Add(1, [](){
                        puts("--- Maximize ---");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();

                    });

                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL::ToWindow().Maximize(); 
                        PrintWindowStates();
                    });

                    s_actions.Add(1, [](){
                        puts("--- Windowed Full Screen ---");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();

                    });

                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL::ToWindow().GoWindowedFullScreen(); 
                        PrintWindowStates();
                    });

                    s_actions.Add(1, [](){
                        puts("--- Center ---");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();

                    });

                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL::ToWindow().Center(s_resolution); 
                        PrintWindowStates();

                        puts("Finished");
                    });
                    break;


                case '1':
                    s_actions.Add(0, [](){
                        puts("---");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();

                        togl_print_i32(TOGL::ToWindow().GetArea().x);
                        togl_print_i32(TOGL::ToWindow().GetArea().y);
                        togl_print_i32(TOGL::ToWindow().GetArea().width);
                        togl_print_i32(TOGL::ToWindow().GetArea().height);

                        togl_print_i32(TOGL::ToWindow().IsForeground());
                    });

                    s_actions.Add(1, [](){
                        puts("---");
                        TOGL::ToWindow().Show(); 

                        togl_print_i32(TOGL::ToWindow().IsForeground());

                        PrintWindowStates();
                    });
                    break;

                case TOGL::KEY_ID_2: 
                    s_actions.Add(0, [](){
                        puts("---");
                        TOGL::ToWindow().Minimize();
                        PrintWindowStates();

                        togl_print_i32(TOGL::ToWindow().GetArea().x);
                        togl_print_i32(TOGL::ToWindow().GetArea().y);
                        togl_print_i32(TOGL::ToWindow().GetArea().width);
                        togl_print_i32(TOGL::ToWindow().GetArea().height);
                    });

                    s_actions.Add(1, [](){
                        puts("---");

                        TOGL::ToWindow().Center(s_resolution);
                        PrintWindowStates();
                    });
                    break;

                case TOGL::KEY_ID_W: 
                    s_actions.Add(0, [](){
                        puts("---");
                        TOGL::ToWindow().Minimize();
                        PrintWindowStates();

                        togl_print_i32(TOGL::ToWindow().GetArea().x);
                        togl_print_i32(TOGL::ToWindow().GetArea().y);
                        togl_print_i32(TOGL::ToWindow().GetArea().width);
                        togl_print_i32(TOGL::ToWindow().GetArea().height);
                    });

                    s_actions.Add(1, [](){
                        TOGL::ToWindow().MoveBy(30, 0);
                        PrintWindowStates();
                    });
                    break;

                case TOGL::KEY_ID_3:
                    puts("---");
                    TOGL::ToWindow().Center(s_resolution);
                    PrintWindowStates();
                    break;

                case TOGL::KEY_ID_4:
                    puts("---");
                    TOGL::ToWindow().Maximize(); 
                    PrintWindowStates();
                    break;

                case TOGL::KEY_ID_5:
                    puts("---");
                    TOGL::ToWindow().GoWindowedFullScreen(); 
                    PrintWindowStates();
                    break;

                case TOGL::KEY_ID_6:
                    s_actions.Add(0, [](){
                        puts("---");
                        TOGL::ToWindow().Hide();
                        PrintWindowStates();
                    });

                    s_actions.Add(1, [](){
                        TOGL::ToWindow().GoWindowedFullScreen(); 
                        PrintWindowStates();
                    });
                    break;

                case TOGL::KEY_ID_7:
                    puts("---");
                    TOGL::ToWindow().SetArea(100, 50, 300, 600);
                    PrintWindowStates();
                    break;

                case TOGL::KEY_ID_8: {
                    DEVMODEW dev_mode = {};
                    dev_mode.dmSize = sizeof(DEVMODEW);

                    dev_mode.dmDisplayFixedOutput   = DMDFO_DEFAULT; // DMDFO_CENTER, DMDFO_STRETCH, DMDFO_DEFAULT
                    dev_mode.dmPelsWidth            = 640; 
                    dev_mode.dmPelsHeight           = 480; 
                    // only specific resolutions will work
                    //dev_mode.dmPelsWidth            = 1280; 
                    //dev_mode.dmPelsHeight           = 720; 
                    dev_mode.dmFields               = DM_DISPLAYFIXEDOUTPUT | DM_PELSWIDTH | DM_PELSHEIGHT;

                    // ... or ...
                    //if (!EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, &dev_mode)) puts("Can not get Display Settings.");

                    LONG result = ChangeDisplaySettingsW(&dev_mode, CDS_FULLSCREEN);
                    togl_print_i32(result);

                    break;
                }

                case TOGL::KEY_ID_9:
                    ChangeDisplaySettingsW(NULL, CDS_RESET);
                    break;

                case TOGL::KEY_ID_I: 
                    DisplayWindowInfo(); 
                    break;

                case TOGL::KEY_ID_D: 
                    if (TOGL::ToWindow().GetLogLevel() == TOGL::LOG_LEVEL_DEBUG) {
                        TOGL::ToWindow().SetLogLevel(TOGL::LOG_LEVEL_ERROR);
                    } else {
                        TOGL::ToWindow().SetLogLevel(TOGL::LOG_LEVEL_DEBUG);
                    }
                    break;
                default: break;
                }
            }
        };

        return TOGL::Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // opengl_version
    ////////////////////////////////////////////////////////////////////////////////

    example_manager.AddExample("opengl_version", {"wrong"}, {}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        if (IsOption("wrong")) {
            data.opengl_verion      = {3, 7};
        } else {
            data.opengl_verion      = {3, 3};
        }
        data.log_level          = TOGL::LOG_LEVEL_DEBUG;

        data.do_on_create = []() {
            auto version = TOGL::ToWindow().GetOpenGL_Version();
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

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            if (!is_down && key_id == 'X') TOGL::ToWindow().RequestClose();
        };

        return TOGL::Run(data);
    });

    ////////////////////////////////////////////////////////////////////////////////
    // keyboard_and_mouse
    ////////////////////////////////////////////////////////////////////////////////
    
    example_manager.AddExample("keyboard_and_mouse", all_options, {}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {600, 300};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        data.icon_resource_id   = ICON_ID;

        if (!IsOption("no_debug"))                      data.log_level = TOGL::LOG_LEVEL_DEBUG;

        if (IsOption("draw_area_size"))                 data.style |= TOGL::StyleBit::DRAW_AREA_SIZE;
        if (IsOption("draw_area_only"))                 data.style |= TOGL::StyleBit::DRAW_AREA_ONLY;
        if (IsOption("redraw_on_change_or_request"))    data.style |= TOGL::StyleBit::REDRAW_ON_CHANGE_OR_REQUEST;
        if (IsOption("no_resize"))                      data.style |= TOGL::StyleBit::NO_RESIZE;
        if (IsOption("no_maximize"))                    data.style |= TOGL::StyleBit::NO_MAXIMIZE;

        if (IsOption("notify_any_message"))             data.special_debug.is_notify_any_message        = true;
        if (IsOption("notify_draw_call"))               data.special_debug.is_notify_draw_call          = true;
        if (IsOption("notify_mouse_move"))              data.special_debug.is_notify_mouse_move         = true;
        if (IsOption("notify_key_message"))             data.special_debug.is_notify_key_message        = true;
        if (IsOption("notify_character_message"))       data.special_debug.is_notify_character_message  = true;

        data.do_on_create = []() {
            s_test_image.Initialize(TOGL::ToWindow().GetDrawAreaSize());


            if (!TOGL::ToWindow().LoadFont("Courier New", FONT_SIZE)) {
                puts("Error: Can not load font.");
                puts(TOGL::ToWindow().GetLoadFontErrMsg().c_str());
            } else {
                puts("Font loaded.");
            }
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

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            std::string message = "Key: ";

            message += TOGL::KeyIdToStr(key_id);
            message += is_down ? " down" : " up";
            message += TOGL::ExtraToStr(extra);

            puts(message.c_str());

            if (is_down) {
                switch (key_id) {
                case 'X':
                    TOGL::ToWindow().RequestClose();
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
                    if (TOGL::ToWindow().GetLogLevel() == TOGL::LOG_LEVEL_DEBUG) {
                        TOGL::ToWindow().SetLogLevel(TOGL::LOG_LEVEL_INFO);
                        puts("Log Level: DEBUG -> INFO");
                    } else {
                        TOGL::ToWindow().SetLogLevel(TOGL::LOG_LEVEL_DEBUG);
                        puts("Log Level: INFO -> DEBUG");
                    }
                    break;

                case 'I': {
                    DisplayWindowInfo();

                    togl_print_i32(TOGL::IsKeyToggled(TOGL::KEY_ID_CAPS_LOCK));
                    togl_print_i32(TOGL::IsKeyToggled(TOGL::KEY_ID_INSERT));
                    togl_print_i32(TOGL::IsKeyToggled(TOGL::KEY_ID_NUMLOCK));

                    const TOGL::PointI cursor_pos_in_screen = TOGL::GetCursorPosInScreen();
                    togl_print_i32(cursor_pos_in_screen.x);
                    togl_print_i32(cursor_pos_in_screen.y);

                    const TOGL::PointI cursor_pos_in_draw_area = TOGL::ToWindow().GetCursorPosInDrawArea();
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

        return TOGL::Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // timer
    ////////////////////////////////////////////////////////////////////////////////
    
    example_manager.AddExample("timer", {"100ms", "500ms", "1s"}, {"500ms"}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {600, 300};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        data.icon_resource_id   = ICON_ID;
        data.log_level          = TOGL::LOG_LEVEL_DEBUG;

        if (IsOption("100ms"))  data.timer_time_interval = 100;
        if (IsOption("500ms"))  data.timer_time_interval = 500;
        if (IsOption("1s"))     data.timer_time_interval = 1000;
        
        data.do_on_create = []() {
            s_test_image.Initialize(TOGL::ToWindow().GetDrawAreaSize());
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

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            if (key_id == 'X' && !is_down) TOGL::ToWindow().RequestClose();
        };

        return TOGL::Run(data);

    });

    ////////////////////////////////////////////////////////////////////////////////
    // test_font
    ////////////////////////////////////////////////////////////////////////////////
    
    example_manager.AddExample("test_font", {}, {}, [](const std::string& name, const std::set<std::string>& options) {
        auto IsOption = [&options](const std::string& option) { return options.find(option) != options.end(); };

        s_resolution = {800, 400};

        TOGL::Data data = {};

        data.window_name        = "TrivialOpenGL_Example ";
        data.window_name        += name;
        data.area               = {TOGL::DEF, TOGL::DEF, s_resolution.width, s_resolution.height};
        data.icon_resource_id   = ICON_ID;
        data.log_level          = TOGL::LOG_LEVEL_DEBUG;
        
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

        data.do_on_key = [](TOGL::KeyId key_id, bool is_down, const TOGL::Extra& extra) {
            if (key_id == 'X' && !is_down) TOGL::ToWindow().RequestClose();
        };

        return TOGL::Run(data);
    });

    //------------------------------------------------------------------------------

    return example_manager.Run(arguments);
}