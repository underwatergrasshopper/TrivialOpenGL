/**
* @file ExampleSupport.cpp
* @author underwatergrasshopper
*/

#include "ExampleSupport.h"


void DrawRectangle(int x, int y, int width, int height) {
    glBegin(GL_TRIANGLE_FAN);

    glVertex2i(x,           y);
    glVertex2i(x + width,   y);
    glVertex2i(x + width,   y + height);
    glVertex2i(x,           y + height);

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

void PrintWindowStates() {
    puts("NHMmFf");
    printf("%s%s%s%s%s%s\n", 
        TOGL_IsNormal() ? "+" : " ",
        TOGL_IsVisible() == false ? "+" : " ",
        TOGL_IsMaximized() ? "+" : " ",
        TOGL_IsMinimized() ? "+" : " ",
        " ",
        TOGL_IsFullScreen() ? "+" : " ");
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

    const TOGL_SizeU16 screen_size = TOGL_GetScreenSize();
    togl_print_i32(screen_size.width);
    togl_print_i32(screen_size.height);

    const TOGL_SizeU16 work_area_size = TOGL_GetDesktopAreaSizeNoTaskBar();
    togl_print_i32(work_area_size.width);
    togl_print_i32(work_area_size.height);
    puts("");

    const TOGL_AreaIU16 window_area = TOGL_GetArea();
    togl_print_i32(window_area.x);
    togl_print_i32(window_area.y);
    togl_print_i32(window_area.width);
    togl_print_i32(window_area.height);
    puts("");

    const TOGL_AreaIU16 window_draw_area = TOGL_GetDrawArea();
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

bool SaveAsBMP(const std::string& file_name, const uint8_t* pixel_data, uint32_t width, uint32_t height, bool is_reverse_rows) {
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
        is_success = _wfopen_s(&file, TOGL_ToUTF16(file_name).c_str(), L"wb") == 0 && file;

        if (is_success) {
            is_success = fwrite(file_data, 1, file_size, file) == file_size;
            fclose(file);
        }

        delete[] file_data;
    }

    return is_success;
}

//------------------------------------------------------------------------------

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