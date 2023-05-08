/**
* @file TrivialOpenGL_Key.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_KEY_H_
#define TRIVIALOPENGL_KEY_H_

#include "TrivialOpenGL_Headers.h"
#include "TrivialOpenGL_Utility.h"

//==============================================================================
// Declarations
//==============================================================================

enum TOGL_KeyId {
    KEY_ID_UNKNOWN              = 0,

    // Same id as ascii. Literals can be used instead of key id. For example: '0', '1', ..., '0', 'A', 'B', ..., 'Z'.
    KEY_ID_0                    = '0',  // same id as ascii code for characters: 0-9
    KEY_ID_1,
    KEY_ID_2,
    KEY_ID_3,
    KEY_ID_4,
    KEY_ID_5,
    KEY_ID_6,
    KEY_ID_7,
    KEY_ID_8,
    KEY_ID_9,

    KEY_ID_A                    = 'A', // same id as ascii code for characters: A-Z
    KEY_ID_B,
    KEY_ID_C,
    KEY_ID_D,
    KEY_ID_E,
    KEY_ID_F,
    KEY_ID_G,
    KEY_ID_H,
    KEY_ID_I,
    KEY_ID_J,
    KEY_ID_K,
    KEY_ID_L,
    KEY_ID_M,
    KEY_ID_N,
    KEY_ID_O,
    KEY_ID_P,
    KEY_ID_Q,
    KEY_ID_R,
    KEY_ID_S,
    KEY_ID_T,
    KEY_ID_U,
    KEY_ID_V,
    KEY_ID_W,
    KEY_ID_X,
    KEY_ID_Y,
    KEY_ID_Z,

    KEY_ID_ESCAPE               = 256, // out of ascii scope, no id collision with ascii code
    KEY_ID_ENTER,  
    KEY_ID_CAPS_LOCK,   
    KEY_ID_TAB,    
    KEY_ID_BACKSPACE,
    KEY_ID_SPACE,

    KEY_ID_SHIFT,
    KEY_ID_CONTROL,
    KEY_ID_ALT, 

    KEY_ID_PAGE_UP,               
    KEY_ID_PAGE_DOWN,       
    KEY_ID_END, 
    KEY_ID_HOME,  

    KEY_ID_ARROW_LEFT,  
    KEY_ID_ARROW_UP, 
    KEY_ID_ARROW_RIGHT,
    KEY_ID_ARROW_DOWN,

    KEY_ID_INSERT,
    KEY_ID_DELETE,
    KEY_ID_BREAK,        
    KEY_ID_PAUSE,
    KEY_ID_PRINT,
    KEY_ID_PRINT_SCREEN,

    KEY_ID_NUMPAD_0,
    KEY_ID_NUMPAD_1,
    KEY_ID_NUMPAD_2,
    KEY_ID_NUMPAD_3,
    KEY_ID_NUMPAD_4,
    KEY_ID_NUMPAD_5,
    KEY_ID_NUMPAD_6,
    KEY_ID_NUMPAD_7,
    KEY_ID_NUMPAD_8,
    KEY_ID_NUMPAD_9,
    KEY_ID_NUMPAD_MULTIPLY,
    KEY_ID_NUMPAD_ADD,
    KEY_ID_NUMPAD_SEPARATOR,
    KEY_ID_NUMPAD_SUBTRACT,
    KEY_ID_NUMPAD_DECIMAL,
    KEY_ID_NUMPAD_DIVIDE,

    KEY_ID_SEMICOLON, 
    KEY_ID_FORWARD_SLASH,
    KEY_ID_ACUTE,
    KEY_ID_OPEN_BRACKET,
    KEY_ID_BACK_SLASH,
    KEY_ID_CLOSE_BRACKET,
    KEY_ID_APOSTROPHE,
    KEY_ID_COMMA,
    KEY_ID_DOT,
    KEY_ID_DASH,
    KEY_ID_EQUAL,

    KEY_ID_F1,
    KEY_ID_F2,
    KEY_ID_F3,
    KEY_ID_F4,
    KEY_ID_F5,
    KEY_ID_F6,
    KEY_ID_F7,
    KEY_ID_F8,
    KEY_ID_F9,
    KEY_ID_F10,
    KEY_ID_F11,
    KEY_ID_F12,
    KEY_ID_F13,
    KEY_ID_F14,
    KEY_ID_F15,
    KEY_ID_F16,
    KEY_ID_F17,
    KEY_ID_F18,
    KEY_ID_F19,
    KEY_ID_F20,
    KEY_ID_F21,
    KEY_ID_F22,
    KEY_ID_F23,
    KEY_ID_F24,

    KEY_ID_NUMLOCK,
    KEY_ID_SCROLL_LOCK,

    KEY_ID_LEFT_MOUSE_BUTTON,
    KEY_ID_MIDDLE_MOUSE_BUTTON,
    KEY_ID_RIGHT_MOUSE_BUTTON,
    KEY_ID_X1_MOUSE_BUTTON,
    KEY_ID_X2_MOUSE_BUTTON,
};

enum TOGL_KeyboardSideId {
    TOGL_KEYBOARD_SIDE_ID_NONE       = 0,
    TOGL_KEYBOARD_SIDE_ID_LEFT       = 1,
    TOGL_KEYBOARD_SIDE_ID_RIGHT      = 2,
};

struct TOGL_Extra {
    // Repeat count of key stroke.
    uint32_t        count           = 0;

    // Cursor position in window draw area.
    int             x               = 0;
    int             y               = 0;

    // Stores information if pressed key was from left or right side of keyboard.
    // If side of keyboard doesn't matter or when key exists only on one side, then contains KEYBOARD_SIDE_NONE. 
    // Note: Used for shift, control, alt keys. For ids: KEY_ID_SHIFT, KEY_ID_CONTROL, KEY_ID_ALT.
    TOGL_KeyboardSideId    keyboard_side   = TOGL_KEYBOARD_SIDE_ID_NONE;
};

std::string TOGL_KeyIdToStr(TOGL_KeyId key_id);
std::string TOGL_KeyboardSideToStr(TOGL_KeyboardSideId side);
std::string TOGL_ExtraToStr(const TOGL_Extra& extra);

// Returns true if key is toggled. For Example: KEY_ID_CAPS_LOCK, KEY_ID_INSERT.
bool TOGL_IsKeyToggled(TOGL_KeyId key_id);
    
//------------------------------------------------------------------------------

// Content this class is for this library inner purpose only.
class _TOGL_InnerKey {
public:
    friend class TOGL_Window;
    friend bool TOGL_IsKeyToggled(TOGL_KeyId key_id);

private:
    struct VirtualKeyData {
        // For more info check lParam for WM_KEYDOWN and WM_KEYUP on msdn.
        uint32_t    count           : 16,
                    scan_code       : 8,
                    is_ext          : 1,
                    reserved1       : 4,
                    context_code    : 1,
                    prev_state      : 1,
                    trans_state     : 1;
    };

    static TOGL_KeyId GetKeyId(WPARAM w_param);
    static TOGL_KeyId GetMouseKeyId(UINT message, WPARAM w_param);
    static std::string WinApiKeyDataToStr(WPARAM w_param, LPARAM l_param);
    static std::string WinApiMouseButtonToStr(WPARAM w_param, LPARAM l_param);
    static bool IsMouseButtonDown(UINT message);

    static TOGL_KeyboardSideId GetKeyboardSide(TOGL_KeyId key_id, const VirtualKeyData& virtual_key_data);
    static int GetVirtualKeyCode(TOGL_KeyId key_id);
    static std::string VK_CodeToStr(int vk_code); 
};

//==============================================================================
// Definitions
//==============================================================================

inline std::string TOGL_KeyIdToStr(TOGL_KeyId key_id) {
    switch (key_id) {
    TOGL_CASE_STR(KEY_ID_UNKNOWN);  
    TOGL_CASE_STR(KEY_ID_BREAK);                  
    TOGL_CASE_STR(KEY_ID_BACKSPACE);              
    TOGL_CASE_STR(KEY_ID_TAB);                    
    TOGL_CASE_STR(KEY_ID_ENTER);                  
    TOGL_CASE_STR(KEY_ID_SHIFT);                  
    TOGL_CASE_STR(KEY_ID_CONTROL);                
    TOGL_CASE_STR(KEY_ID_ALT);                    
    TOGL_CASE_STR(KEY_ID_PAUSE);                  
    TOGL_CASE_STR(KEY_ID_CAPS_LOCK);              
    TOGL_CASE_STR(KEY_ID_ESCAPE);                 
    TOGL_CASE_STR(KEY_ID_SPACE);                  
    TOGL_CASE_STR(KEY_ID_PAGE_UP);                
    TOGL_CASE_STR(KEY_ID_PAGE_DOWN);              
    TOGL_CASE_STR(KEY_ID_END);                    
    TOGL_CASE_STR(KEY_ID_HOME);                   
    TOGL_CASE_STR(KEY_ID_ARROW_LEFT);             
    TOGL_CASE_STR(KEY_ID_ARROW_UP);               
    TOGL_CASE_STR(KEY_ID_ARROW_RIGHT);            
    TOGL_CASE_STR(KEY_ID_ARROW_DOWN);             
    TOGL_CASE_STR(KEY_ID_PRINT);                  
    TOGL_CASE_STR(KEY_ID_PRINT_SCREEN);           
    TOGL_CASE_STR(KEY_ID_INSERT);                 
    TOGL_CASE_STR(KEY_ID_DELETE);                 
    TOGL_CASE_STR(KEY_ID_NUMPAD_0);               
    TOGL_CASE_STR(KEY_ID_NUMPAD_1);               
    TOGL_CASE_STR(KEY_ID_NUMPAD_2);               
    TOGL_CASE_STR(KEY_ID_NUMPAD_3);               
    TOGL_CASE_STR(KEY_ID_NUMPAD_4);               
    TOGL_CASE_STR(KEY_ID_NUMPAD_5);               
    TOGL_CASE_STR(KEY_ID_NUMPAD_6);               
    TOGL_CASE_STR(KEY_ID_NUMPAD_7);               
    TOGL_CASE_STR(KEY_ID_NUMPAD_8);               
    TOGL_CASE_STR(KEY_ID_NUMPAD_9);               
    TOGL_CASE_STR(KEY_ID_NUMPAD_MULTIPLY);        
    TOGL_CASE_STR(KEY_ID_NUMPAD_ADD);             
    TOGL_CASE_STR(KEY_ID_NUMPAD_SEPARATOR);       
    TOGL_CASE_STR(KEY_ID_NUMPAD_SUBTRACT);        
    TOGL_CASE_STR(KEY_ID_NUMPAD_DECIMAL);         
    TOGL_CASE_STR(KEY_ID_NUMPAD_DIVIDE);          
    TOGL_CASE_STR(KEY_ID_F1);                     
    TOGL_CASE_STR(KEY_ID_F2);                     
    TOGL_CASE_STR(KEY_ID_F3);                     
    TOGL_CASE_STR(KEY_ID_F4);                     
    TOGL_CASE_STR(KEY_ID_F5);                     
    TOGL_CASE_STR(KEY_ID_F6);                     
    TOGL_CASE_STR(KEY_ID_F7);                     
    TOGL_CASE_STR(KEY_ID_F8);                     
    TOGL_CASE_STR(KEY_ID_F9);                     
    TOGL_CASE_STR(KEY_ID_F10);                    
    TOGL_CASE_STR(KEY_ID_F11);                    
    TOGL_CASE_STR(KEY_ID_F12);                    
    TOGL_CASE_STR(KEY_ID_F13);                    
    TOGL_CASE_STR(KEY_ID_F14);                    
    TOGL_CASE_STR(KEY_ID_F15);                    
    TOGL_CASE_STR(KEY_ID_F16);                    
    TOGL_CASE_STR(KEY_ID_F17);                    
    TOGL_CASE_STR(KEY_ID_F18);                    
    TOGL_CASE_STR(KEY_ID_F19);                    
    TOGL_CASE_STR(KEY_ID_F20);                    
    TOGL_CASE_STR(KEY_ID_F21);                    
    TOGL_CASE_STR(KEY_ID_F22);                    
    TOGL_CASE_STR(KEY_ID_F23);                    
    TOGL_CASE_STR(KEY_ID_F24);   
    TOGL_CASE_STR(KEY_ID_0);    
    TOGL_CASE_STR(KEY_ID_1);    
    TOGL_CASE_STR(KEY_ID_2);    
    TOGL_CASE_STR(KEY_ID_3);    
    TOGL_CASE_STR(KEY_ID_4);    
    TOGL_CASE_STR(KEY_ID_5);    
    TOGL_CASE_STR(KEY_ID_6);    
    TOGL_CASE_STR(KEY_ID_7);    
    TOGL_CASE_STR(KEY_ID_8);    
    TOGL_CASE_STR(KEY_ID_9);    
    TOGL_CASE_STR(KEY_ID_A);    
    TOGL_CASE_STR(KEY_ID_B);    
    TOGL_CASE_STR(KEY_ID_C);    
    TOGL_CASE_STR(KEY_ID_D);    
    TOGL_CASE_STR(KEY_ID_E);    
    TOGL_CASE_STR(KEY_ID_F);    
    TOGL_CASE_STR(KEY_ID_G);    
    TOGL_CASE_STR(KEY_ID_H);    
    TOGL_CASE_STR(KEY_ID_I);    
    TOGL_CASE_STR(KEY_ID_J);    
    TOGL_CASE_STR(KEY_ID_K);    
    TOGL_CASE_STR(KEY_ID_L);    
    TOGL_CASE_STR(KEY_ID_M);    
    TOGL_CASE_STR(KEY_ID_N);    
    TOGL_CASE_STR(KEY_ID_O);    
    TOGL_CASE_STR(KEY_ID_P);    
    TOGL_CASE_STR(KEY_ID_Q);    
    TOGL_CASE_STR(KEY_ID_R);    
    TOGL_CASE_STR(KEY_ID_S);    
    TOGL_CASE_STR(KEY_ID_T);    
    TOGL_CASE_STR(KEY_ID_U);    
    TOGL_CASE_STR(KEY_ID_V);    
    TOGL_CASE_STR(KEY_ID_W);    
    TOGL_CASE_STR(KEY_ID_X);    
    TOGL_CASE_STR(KEY_ID_Y);    
    TOGL_CASE_STR(KEY_ID_Z);

    TOGL_CASE_STR(KEY_ID_SEMICOLON);
    TOGL_CASE_STR(KEY_ID_FORWARD_SLASH);
    TOGL_CASE_STR(KEY_ID_ACUTE);
    TOGL_CASE_STR(KEY_ID_OPEN_BRACKET);
    TOGL_CASE_STR(KEY_ID_BACK_SLASH);
    TOGL_CASE_STR(KEY_ID_CLOSE_BRACKET);
    TOGL_CASE_STR(KEY_ID_APOSTROPHE);
    TOGL_CASE_STR(KEY_ID_COMMA);
    TOGL_CASE_STR(KEY_ID_DOT);
    TOGL_CASE_STR(KEY_ID_DASH);
    TOGL_CASE_STR(KEY_ID_EQUAL);

    TOGL_CASE_STR(KEY_ID_NUMLOCK);                
    TOGL_CASE_STR(KEY_ID_SCROLL_LOCK);   
              
    TOGL_CASE_STR(KEY_ID_LEFT_MOUSE_BUTTON);   
    TOGL_CASE_STR(KEY_ID_MIDDLE_MOUSE_BUTTON);   
    TOGL_CASE_STR(KEY_ID_RIGHT_MOUSE_BUTTON);   
    TOGL_CASE_STR(KEY_ID_X1_MOUSE_BUTTON);   
    TOGL_CASE_STR(KEY_ID_X2_MOUSE_BUTTON);   
    }
    return std::string() + "(" + std::to_string(key_id) + ")";
}

inline std::string TOGL_KeyboardSideToStr(TOGL_KeyboardSideId side) {
    switch(side) {
    TOGL_CASE_STR(TOGL_KEYBOARD_SIDE_ID_NONE);
    TOGL_CASE_STR(TOGL_KEYBOARD_SIDE_ID_LEFT);
    TOGL_CASE_STR(TOGL_KEYBOARD_SIDE_ID_RIGHT);
    }
    return "?";
}

inline std::string _TOGL_InnerKey::WinApiKeyDataToStr(WPARAM w_param, LPARAM l_param) {
    const VirtualKeyData& virtual_key_data = *((const VirtualKeyData*)(&l_param));

    auto PadWithSpaces = [](const std::string& text, uint32_t num_of_spaces) {
        if (text.length() > 0 && text.length() < num_of_spaces) return text + std::string(num_of_spaces - text.length(), ' ');
        return text;

    };

    enum { MAX_LENGTH_OF_NAME = 16 };

    std::string text = std::string()
        + " vk_code="          + PadWithSpaces(VK_CodeToStr((int)w_param), MAX_LENGTH_OF_NAME)
        + " key_id="           + PadWithSpaces(TOGL_KeyIdToStr(GetKeyId(w_param)), MAX_LENGTH_OF_NAME)
        + ", count="           + std::to_string(virtual_key_data.count)
        + ", scan_code="       + std::to_string(virtual_key_data.scan_code)
        + ", is_ext="          + std::to_string(virtual_key_data.is_ext)
        + ", reserved1="       + std::to_string(virtual_key_data.reserved1)
        + ", context_code="    + std::to_string(virtual_key_data.context_code)
        + ", prev_state="      + std::to_string(virtual_key_data.prev_state)
        + ", trans_state="     + std::to_string(virtual_key_data.trans_state);

    return text;
}

inline std::string _TOGL_InnerKey::WinApiMouseButtonToStr(WPARAM w_param, LPARAM l_param) {
    const int x = GET_X_LPARAM(l_param);
    const int y = GET_Y_LPARAM(l_param);

    std::string text;
    text += " x=" + std::to_string(x);
    text += " y=" + std::to_string(y);

    WORD other_button_down_field = LOWORD(w_param);

    if (other_button_down_field & MK_CONTROL)   text += " MK_CONTROL";
    if (other_button_down_field & MK_LBUTTON)   text += " MK_LBUTTON";
    if (other_button_down_field & MK_MBUTTON)   text += " MK_MBUTTON";
    if (other_button_down_field & MK_RBUTTON)   text += " MK_RBUTTON";
    if (other_button_down_field & MK_SHIFT)     text += " MK_SHIFT";
    if (other_button_down_field & MK_XBUTTON1)  text += " MK_XBUTTON1";
    if (other_button_down_field & MK_XBUTTON2)  text += " MK_XBUTTON2";

    return text;
}

inline std::string TOGL_ExtraToStr(const TOGL_Extra& extra) {
    std::string text;

    text += "{count=" + std::to_string(extra.count);
    text += ", x=" + std::to_string(extra.x);
    text += ", y=" + std::to_string(extra.y);
    text += ", side=" + TOGL_KeyboardSideToStr(extra.keyboard_side);
    text += "}";

    return text;
};

inline bool TOGL_IsKeyToggled(TOGL_KeyId key_id) {
    enum { TOGGLE_BIT = 0x0001 };
    return GetKeyState(_TOGL_InnerKey::GetVirtualKeyCode(key_id)) & TOGGLE_BIT;
}

//--------------------------------------------------------------------------

inline TOGL_KeyId _TOGL_InnerKey::GetKeyId(WPARAM w_param) {
    switch (w_param) {
    case VK_CANCEL:         return KEY_ID_BREAK;                  
    case VK_BACK:           return KEY_ID_BACKSPACE;              
    case VK_TAB:            return KEY_ID_TAB;                    
    case VK_RETURN:         return KEY_ID_ENTER; 
    case VK_SHIFT:          return KEY_ID_SHIFT; 
    case VK_CONTROL:        return KEY_ID_CONTROL; 
    case VK_MENU:           return KEY_ID_ALT; 

    case VK_PAUSE:          return KEY_ID_PAUSE;                  
    case VK_CAPITAL:        return KEY_ID_CAPS_LOCK;              
    case VK_ESCAPE:         return KEY_ID_ESCAPE;                 
    case VK_SPACE:          return KEY_ID_SPACE;                  
    case VK_PRIOR:          return KEY_ID_PAGE_UP;                
    case VK_NEXT:           return KEY_ID_PAGE_DOWN;              
    case VK_END:            return KEY_ID_END;                    
    case VK_HOME:           return KEY_ID_HOME;                   
    case VK_LEFT:           return KEY_ID_ARROW_LEFT;             
    case VK_UP:             return KEY_ID_ARROW_UP;               
    case VK_RIGHT:          return KEY_ID_ARROW_RIGHT;            
    case VK_DOWN:           return KEY_ID_ARROW_DOWN;             
    case VK_PRINT:          return KEY_ID_PRINT;                  
    case VK_SNAPSHOT:       return KEY_ID_PRINT_SCREEN;           
    case VK_INSERT:         return KEY_ID_INSERT;                 
    case VK_DELETE:         return KEY_ID_DELETE;                 
    case VK_NUMPAD0:        return KEY_ID_NUMPAD_0;               
    case VK_NUMPAD1:        return KEY_ID_NUMPAD_1;               
    case VK_NUMPAD2:        return KEY_ID_NUMPAD_2;               
    case VK_NUMPAD3:        return KEY_ID_NUMPAD_3;               
    case VK_NUMPAD4:        return KEY_ID_NUMPAD_4;               
    case VK_NUMPAD5:        return KEY_ID_NUMPAD_5;               
    case VK_NUMPAD6:        return KEY_ID_NUMPAD_6;               
    case VK_NUMPAD7:        return KEY_ID_NUMPAD_7;               
    case VK_NUMPAD8:        return KEY_ID_NUMPAD_8;               
    case VK_NUMPAD9:        return KEY_ID_NUMPAD_9;               
    case VK_MULTIPLY:       return KEY_ID_NUMPAD_MULTIPLY;        
    case VK_ADD:            return KEY_ID_NUMPAD_ADD;             
    case VK_SEPARATOR:      return KEY_ID_NUMPAD_SEPARATOR;       
    case VK_SUBTRACT:       return KEY_ID_NUMPAD_SUBTRACT;        
    case VK_DECIMAL:        return KEY_ID_NUMPAD_DECIMAL;         
    case VK_DIVIDE:         return KEY_ID_NUMPAD_DIVIDE;          
    case VK_F1:             return KEY_ID_F1;                     
    case VK_F2:             return KEY_ID_F2;                     
    case VK_F3:             return KEY_ID_F3;                     
    case VK_F4:             return KEY_ID_F4;                     
    case VK_F5:             return KEY_ID_F5;                     
    case VK_F6:             return KEY_ID_F6;                     
    case VK_F7:             return KEY_ID_F7;                     
    case VK_F8:             return KEY_ID_F8;                     
    case VK_F9:             return KEY_ID_F9;                     
    case VK_F10:            return KEY_ID_F10;                    
    case VK_F11:            return KEY_ID_F11;                    
    case VK_F12:            return KEY_ID_F12;                    
    case VK_F13:            return KEY_ID_F13;                    
    case VK_F14:            return KEY_ID_F14;                    
    case VK_F15:            return KEY_ID_F15;                    
    case VK_F16:            return KEY_ID_F16;                    
    case VK_F17:            return KEY_ID_F17;                    
    case VK_F18:            return KEY_ID_F18;                    
    case VK_F19:            return KEY_ID_F19;                    
    case VK_F20:            return KEY_ID_F20;                    
    case VK_F21:            return KEY_ID_F21;                    
    case VK_F22:            return KEY_ID_F22;                    
    case VK_F23:            return KEY_ID_F23;                    
    case VK_F24:            return KEY_ID_F24;   
    case '0':               return KEY_ID_0;    
    case '1':               return KEY_ID_1;    
    case '2':               return KEY_ID_2;    
    case '3':               return KEY_ID_3;    
    case '4':               return KEY_ID_4;    
    case '5':               return KEY_ID_5;    
    case '6':               return KEY_ID_6;    
    case '7':               return KEY_ID_7;    
    case '8':               return KEY_ID_8;    
    case '9':               return KEY_ID_9;    
    case 'A':               return KEY_ID_A;    
    case 'B':               return KEY_ID_B;    
    case 'C':               return KEY_ID_C;    
    case 'D':               return KEY_ID_D;    
    case 'E':               return KEY_ID_E;    
    case 'F':               return KEY_ID_F;    
    case 'G':               return KEY_ID_G;    
    case 'H':               return KEY_ID_H;    
    case 'I':               return KEY_ID_I;    
    case 'J':               return KEY_ID_J;    
    case 'K':               return KEY_ID_K;    
    case 'L':               return KEY_ID_L;    
    case 'M':               return KEY_ID_M;    
    case 'N':               return KEY_ID_N;    
    case 'O':               return KEY_ID_O;    
    case 'P':               return KEY_ID_P;    
    case 'Q':               return KEY_ID_Q;    
    case 'R':               return KEY_ID_R;    
    case 'S':               return KEY_ID_S;    
    case 'T':               return KEY_ID_T;    
    case 'U':               return KEY_ID_U;    
    case 'V':               return KEY_ID_V;    
    case 'W':               return KEY_ID_W;    
    case 'X':               return KEY_ID_X;    
    case 'Y':               return KEY_ID_Y;    
    case 'Z':               return KEY_ID_Z;

    case VK_OEM_1:          return KEY_ID_SEMICOLON;        // ;
    case VK_OEM_2:          return KEY_ID_FORWARD_SLASH;    // /
    case VK_OEM_3:          return KEY_ID_ACUTE;            // `
    case VK_OEM_4:          return KEY_ID_OPEN_BRACKET;     // [
    case VK_OEM_5:          return KEY_ID_BACK_SLASH;       
    case VK_OEM_6:          return KEY_ID_CLOSE_BRACKET;    // ]
    case VK_OEM_7:          return KEY_ID_APOSTROPHE;       // '
    case VK_OEM_COMMA:        return KEY_ID_COMMA;            // ,
    case VK_OEM_PERIOD:        return KEY_ID_DOT;              // .
    case VK_OEM_MINUS:        return KEY_ID_DASH;             // -
    case VK_OEM_PLUS:        return KEY_ID_EQUAL;            // =

    case VK_NUMLOCK:        return KEY_ID_NUMLOCK;                
    case VK_SCROLL:         return KEY_ID_SCROLL_LOCK;      
    }
    return KEY_ID_UNKNOWN;
}

inline int _TOGL_InnerKey::GetVirtualKeyCode(TOGL_KeyId key_id) {
    switch (key_id) {
    case KEY_ID_BREAK:                  return VK_CANCEL;                           
    case KEY_ID_BACKSPACE:              return VK_BACK;           
    case KEY_ID_TAB:                    return VK_TAB;            
    case KEY_ID_ENTER:                  return VK_RETURN;         
    case KEY_ID_SHIFT:                  return VK_SHIFT;          
    case KEY_ID_CONTROL:                return VK_CONTROL;        
    case KEY_ID_ALT:                    return VK_MENU;           

    case KEY_ID_PAUSE:                  return VK_PAUSE;          
    case KEY_ID_CAPS_LOCK:              return VK_CAPITAL;        
    case KEY_ID_ESCAPE:                 return VK_ESCAPE;         
    case KEY_ID_SPACE:                  return VK_SPACE;          
    case KEY_ID_PAGE_UP:                return VK_PRIOR;          
    case KEY_ID_PAGE_DOWN:              return VK_NEXT;           
    case KEY_ID_END:                    return VK_END;            
    case KEY_ID_HOME:                   return VK_HOME;           
    case KEY_ID_ARROW_LEFT:             return VK_LEFT;           
    case KEY_ID_ARROW_UP:               return VK_UP;             
    case KEY_ID_ARROW_RIGHT:            return VK_RIGHT;          
    case KEY_ID_ARROW_DOWN:             return VK_DOWN;           
    case KEY_ID_PRINT:                  return VK_PRINT;          
    case KEY_ID_PRINT_SCREEN:           return VK_SNAPSHOT;       
    case KEY_ID_INSERT:                 return VK_INSERT;         
    case KEY_ID_DELETE:                 return VK_DELETE;         
    case KEY_ID_NUMPAD_0:               return VK_NUMPAD0;        
    case KEY_ID_NUMPAD_1:               return VK_NUMPAD1;        
    case KEY_ID_NUMPAD_2:               return VK_NUMPAD2;        
    case KEY_ID_NUMPAD_3:               return VK_NUMPAD3;        
    case KEY_ID_NUMPAD_4:               return VK_NUMPAD4;        
    case KEY_ID_NUMPAD_5:               return VK_NUMPAD5;        
    case KEY_ID_NUMPAD_6:               return VK_NUMPAD6;        
    case KEY_ID_NUMPAD_7:               return VK_NUMPAD7;        
    case KEY_ID_NUMPAD_8:               return VK_NUMPAD8;        
    case KEY_ID_NUMPAD_9:               return VK_NUMPAD9;        
    case KEY_ID_NUMPAD_MULTIPLY:        return VK_MULTIPLY;       
    case KEY_ID_NUMPAD_ADD:             return VK_ADD;            
    case KEY_ID_NUMPAD_SEPARATOR:       return VK_SEPARATOR;      
    case KEY_ID_NUMPAD_SUBTRACT:        return VK_SUBTRACT;       
    case KEY_ID_NUMPAD_DECIMAL:         return VK_DECIMAL;        
    case KEY_ID_NUMPAD_DIVIDE:          return VK_DIVIDE;         
    case KEY_ID_F1:                     return VK_F1;             
    case KEY_ID_F2:                     return VK_F2;             
    case KEY_ID_F3:                     return VK_F3;             
    case KEY_ID_F4:                     return VK_F4;             
    case KEY_ID_F5:                     return VK_F5;             
    case KEY_ID_F6:                     return VK_F6;             
    case KEY_ID_F7:                     return VK_F7;             
    case KEY_ID_F8:                     return VK_F8;             
    case KEY_ID_F9:                     return VK_F9;             
    case KEY_ID_F10:                    return VK_F10;            
    case KEY_ID_F11:                    return VK_F11;            
    case KEY_ID_F12:                    return VK_F12;            
    case KEY_ID_F13:                    return VK_F13;            
    case KEY_ID_F14:                    return VK_F14;            
    case KEY_ID_F15:                    return VK_F15;            
    case KEY_ID_F16:                    return VK_F16;            
    case KEY_ID_F17:                    return VK_F17;            
    case KEY_ID_F18:                    return VK_F18;            
    case KEY_ID_F19:                    return VK_F19;            
    case KEY_ID_F20:                    return VK_F20;            
    case KEY_ID_F21:                    return VK_F21;            
    case KEY_ID_F22:                    return VK_F22;            
    case KEY_ID_F23:                    return VK_F23;            
    case KEY_ID_F24:                    return VK_F24;            
    case KEY_ID_0:                      return '0';               
    case KEY_ID_1:                      return '1';               
    case KEY_ID_2:                      return '2';               
    case KEY_ID_3:                      return '3';               
    case KEY_ID_4:                      return '4';               
    case KEY_ID_5:                      return '5';               
    case KEY_ID_6:                      return '6';               
    case KEY_ID_7:                      return '7';               
    case KEY_ID_8:                      return '8';               
    case KEY_ID_9:                      return '9';               
    case KEY_ID_A:                      return 'A';               
    case KEY_ID_B:                      return 'B';               
    case KEY_ID_C:                      return 'C';               
    case KEY_ID_D:                      return 'D';               
    case KEY_ID_E:                      return 'E';               
    case KEY_ID_F:                      return 'F';               
    case KEY_ID_G:                      return 'G';               
    case KEY_ID_H:                      return 'H';               
    case KEY_ID_I:                      return 'I';               
    case KEY_ID_J:                      return 'J';               
    case KEY_ID_K:                      return 'K';               
    case KEY_ID_L:                      return 'L';               
    case KEY_ID_M:                      return 'M';               
    case KEY_ID_N:                      return 'N';               
    case KEY_ID_O:                      return 'O';               
    case KEY_ID_P:                      return 'P';               
    case KEY_ID_Q:                      return 'Q';               
    case KEY_ID_R:                      return 'R';               
    case KEY_ID_S:                      return 'S';               
    case KEY_ID_T:                      return 'T';               
    case KEY_ID_U:                      return 'U';               
    case KEY_ID_V:                      return 'V';               
    case KEY_ID_W:                      return 'W';               
    case KEY_ID_X:                      return 'X';               
    case KEY_ID_Y:                      return 'Y';               
    case KEY_ID_Z:                      return 'Z';               

    case KEY_ID_SEMICOLON:              return VK_OEM_1;          
    case KEY_ID_FORWARD_SLASH:          return VK_OEM_2;          
    case KEY_ID_ACUTE:                  return VK_OEM_3;          
    case KEY_ID_OPEN_BRACKET:           return VK_OEM_4;          
    case KEY_ID_BACK_SLASH:             return VK_OEM_5;          
    case KEY_ID_CLOSE_BRACKET:          return VK_OEM_6;          
    case KEY_ID_APOSTROPHE:             return VK_OEM_7;          
    case KEY_ID_COMMA:                  return VK_OEM_COMMA;        
    case KEY_ID_DOT:                    return VK_OEM_PERIOD;        
    case KEY_ID_DASH:                   return VK_OEM_MINUS;        
    case KEY_ID_EQUAL:                  return VK_OEM_PLUS;        

    case KEY_ID_NUMLOCK:                return VK_NUMLOCK;        
    case KEY_ID_SCROLL_LOCK:            return VK_SCROLL;   

    case KEY_ID_UNKNOWN:
    default:                            return 0;
    }
}

inline TOGL_KeyId _TOGL_InnerKey::GetMouseKeyId(UINT message, WPARAM w_param) {
    switch (message) {
    case WM_LBUTTONDOWN:    
    case WM_LBUTTONUP:      
        return KEY_ID_LEFT_MOUSE_BUTTON;

    case WM_RBUTTONDOWN:    
    case WM_RBUTTONUP:      
        return KEY_ID_RIGHT_MOUSE_BUTTON;

    case WM_MBUTTONDOWN:    
    case WM_MBUTTONUP:      
        return KEY_ID_MIDDLE_MOUSE_BUTTON;

    case WM_XBUTTONUP:
    case WM_XBUTTONDOWN:   
        if (HIWORD(w_param) == XBUTTON1) return KEY_ID_X1_MOUSE_BUTTON;
        if (HIWORD(w_param) == XBUTTON2) return KEY_ID_X2_MOUSE_BUTTON;
        return KEY_ID_UNKNOWN;
    } // switch

    return KEY_ID_UNKNOWN;
}

inline bool _TOGL_InnerKey::IsMouseButtonDown(UINT message) {
    switch (message) {
    case WM_LBUTTONDOWN:    
    case WM_RBUTTONDOWN:    
    case WM_MBUTTONDOWN:    
    case WM_XBUTTONUP:
        return true;
    } // switch

    return false;
}

inline TOGL_KeyboardSideId _TOGL_InnerKey::GetKeyboardSide(TOGL_KeyId key_id, const VirtualKeyData& virtual_key_data) {
    switch(key_id) {
    case KEY_ID_SHIFT: {
        const int vk_code_ext = MapVirtualKeyA(virtual_key_data.scan_code, MAPVK_VSC_TO_VK_EX);
        if (vk_code_ext == VK_LSHIFT) return TOGL_KEYBOARD_SIDE_ID_LEFT; 
        if (vk_code_ext == VK_RSHIFT) return TOGL_KEYBOARD_SIDE_ID_RIGHT; 
        return TOGL_KEYBOARD_SIDE_ID_NONE;
    } 
    case KEY_ID_CONTROL: 
        if (virtual_key_data.is_ext) return TOGL_KEYBOARD_SIDE_ID_RIGHT; 
        return TOGL_KEYBOARD_SIDE_ID_LEFT; 

    case KEY_ID_ALT: 
        if (virtual_key_data.is_ext) return TOGL_KEYBOARD_SIDE_ID_RIGHT; 
        return TOGL_KEYBOARD_SIDE_ID_LEFT; 
    default: 
        return TOGL_KEYBOARD_SIDE_ID_NONE;
    } // switch
};

inline std::string _TOGL_InnerKey::VK_CodeToStr(int vk_code) {
    switch (vk_code) {
    TOGL_CASE_STR(VK_CANCEL);        
    TOGL_CASE_STR(VK_BACK);          
    TOGL_CASE_STR(VK_TAB);           
    TOGL_CASE_STR(VK_RETURN);        
    TOGL_CASE_STR(VK_SHIFT);         
    TOGL_CASE_STR(VK_CONTROL);       
    TOGL_CASE_STR(VK_MENU);          
    TOGL_CASE_STR(VK_PAUSE);         
    TOGL_CASE_STR(VK_CAPITAL);       
    TOGL_CASE_STR(VK_ESCAPE);        
    TOGL_CASE_STR(VK_SPACE);         
    TOGL_CASE_STR(VK_PRIOR);         
    TOGL_CASE_STR(VK_NEXT);          
    TOGL_CASE_STR(VK_END);           
    TOGL_CASE_STR(VK_HOME);          
    TOGL_CASE_STR(VK_LEFT);          
    TOGL_CASE_STR(VK_UP);            
    TOGL_CASE_STR(VK_RIGHT);         
    TOGL_CASE_STR(VK_DOWN);          
    TOGL_CASE_STR(VK_PRINT);         
    TOGL_CASE_STR(VK_SNAPSHOT);      
    TOGL_CASE_STR(VK_INSERT);        
    TOGL_CASE_STR(VK_DELETE);        
    TOGL_CASE_STR(VK_NUMPAD0);       
    TOGL_CASE_STR(VK_NUMPAD1);       
    TOGL_CASE_STR(VK_NUMPAD2);       
    TOGL_CASE_STR(VK_NUMPAD3);       
    TOGL_CASE_STR(VK_NUMPAD4);       
    TOGL_CASE_STR(VK_NUMPAD5);       
    TOGL_CASE_STR(VK_NUMPAD6);       
    TOGL_CASE_STR(VK_NUMPAD7);       
    TOGL_CASE_STR(VK_NUMPAD8);       
    TOGL_CASE_STR(VK_NUMPAD9);       
    TOGL_CASE_STR(VK_MULTIPLY);      
    TOGL_CASE_STR(VK_ADD);           
    TOGL_CASE_STR(VK_SEPARATOR);     
    TOGL_CASE_STR(VK_SUBTRACT);      
    TOGL_CASE_STR(VK_DECIMAL);       
    TOGL_CASE_STR(VK_DIVIDE);        
    TOGL_CASE_STR(VK_F1);            
    TOGL_CASE_STR(VK_F2);            
    TOGL_CASE_STR(VK_F3);            
    TOGL_CASE_STR(VK_F4);            
    TOGL_CASE_STR(VK_F5);            
    TOGL_CASE_STR(VK_F6);            
    TOGL_CASE_STR(VK_F7);            
    TOGL_CASE_STR(VK_F8);            
    TOGL_CASE_STR(VK_F9);            
    TOGL_CASE_STR(VK_F10);           
    TOGL_CASE_STR(VK_F11);           
    TOGL_CASE_STR(VK_F12);           
    TOGL_CASE_STR(VK_F13);           
    TOGL_CASE_STR(VK_F14);           
    TOGL_CASE_STR(VK_F15);           
    TOGL_CASE_STR(VK_F16);           
    TOGL_CASE_STR(VK_F17);           
    TOGL_CASE_STR(VK_F18);           
    TOGL_CASE_STR(VK_F19);           
    TOGL_CASE_STR(VK_F20);           
    TOGL_CASE_STR(VK_F21);           
    TOGL_CASE_STR(VK_F22);           
    TOGL_CASE_STR(VK_F23);           
    TOGL_CASE_STR(VK_F24);   

    TOGL_CASE_STR(VK_OEM_1); 
    TOGL_CASE_STR(VK_OEM_2); 
    TOGL_CASE_STR(VK_OEM_3); 
    TOGL_CASE_STR(VK_OEM_4); 
    TOGL_CASE_STR(VK_OEM_5); 
    TOGL_CASE_STR(VK_OEM_6); 
    TOGL_CASE_STR(VK_OEM_7); 
    TOGL_CASE_STR(VK_OEM_COMMA); 
    TOGL_CASE_STR(VK_OEM_PERIOD); 
    TOGL_CASE_STR(VK_OEM_MINUS); 
    TOGL_CASE_STR(VK_OEM_PLUS); 

    TOGL_CASE_STR(VK_NUMLOCK);       
    TOGL_CASE_STR(VK_SCROLL);        
    TOGL_CASE_STR(VK_LSHIFT);        
    TOGL_CASE_STR(VK_RSHIFT);        
    TOGL_CASE_STR(VK_LCONTROL);      
    TOGL_CASE_STR(VK_RCONTROL);      
    TOGL_CASE_STR(VK_LMENU);         
    TOGL_CASE_STR(VK_RMENU);   
    }
    if ((vk_code >= '0' && vk_code <= '9') || (vk_code >= 'A' && vk_code <= 'Z')) return std::string(1, (char)vk_code); 
    return std::string() + "(" + std::to_string(vk_code) + ")";
}

#endif // TRIVIALOPENGL_KEY_H_

