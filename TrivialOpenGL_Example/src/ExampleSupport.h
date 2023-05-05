/**
* @file ExampleSupport.h
* @author underwatergrasshopper
*/

#ifndef EXAMPLESUPPORT_H_
#define EXAMPLESUPPORT_H_

#include <stdio.h>

#include <string>
#include <set>

#include <TimeLapse.h>
#include <TrivialOpenGL.h>

#include "Resource.h"

//------------------------------------------------------------------------------

void DrawRectangle(int x, int y, int width, int height);
void DrawTriangle(float x, float y, float scale, float angle);
void PrintWindowStates();
void DisplayWindowInfo();

//------------------------------------------------------------------------------

// Saves pixel data as BMP image.
// file_name            - Full file name with bmp extension.
// pixel_data           - Array of pixels. Each pixel occupies 4 bytes of data. 
//                        Each byte of pixel corresponds to specific channel (in order): red, green, blue, alpha.
// width                - Number of pixel in row of image.
// height               - Number of pixel in column of image.  
// Returns true if image has been saved to file. 
bool SaveAsBMP(const std::string& file_name, const uint8_t* pixel_data, uint32_t width, uint32_t height, bool is_reverse_rows = true);

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

void DisplayCharsets();
void DisplayUnicodeRangesForCurrentFont(HDC device_context_handle);

//------------------------------------------------------------------------------

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

    void Initialize(const TOGL_SizeU16& size) {
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

        TOGL_RequestRedraw();
    }

    TOGL_SizeU16 GetSize() const {
        return m_size;
    }

private:
    TOGL_SizeU16   m_size;

    double          m_interval;         // in seconds
    double          m_speed;            // in degrees per second
    double          m_step;             // in degrees

    TimeLapseF64    m_time_lapse;
    double          m_accumulator;      // time in seconds
    int             m_angle;            // in degrees
};

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
            for (const auto& option : RemoveEmpty(TOGL_Split(raw_options, ' '))) options.insert(option);

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

    uint32_t Get() const {
        return m_fps;
    }

private:
    uint32_t        m_frame_count;
    uint32_t        m_fps;
    TimeLapseF64    m_time_lapse;
};


#endif // EXAMPLESUPPORT_H_

