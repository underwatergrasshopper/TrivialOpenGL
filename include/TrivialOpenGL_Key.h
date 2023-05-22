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
    TOGL_KEY_IDUNKNOWN              = 0,

    // Same id as ascii. Literals can be used instead of key id. For example: '0', '1', ..., '0', 'A', 'B', ..., 'Z'.
    TOGL_KEY_ID_0                   = '0',  // same id as ascii code for characters: 0-9
    TOGL_KEY_ID_1,
    TOGL_KEY_ID_2,
    TOGL_KEY_ID_3,
    TOGL_KEY_ID_4,
    TOGL_KEY_ID_5,
    TOGL_KEY_ID_6,
    TOGL_KEY_ID_7,
    TOGL_KEY_ID_8,
    TOGL_KEY_ID_9,

    TOGL_KEY_ID_A                   = 'A', // same id as ascii code for characters: A-Z
    TOGL_KEY_ID_B,
    TOGL_KEY_ID_C,
    TOGL_KEY_ID_D,
    TOGL_KEY_ID_E,
    TOGL_KEY_ID_F,
    TOGL_KEY_ID_G,
    TOGL_KEY_ID_H,
    TOGL_KEY_ID_I,
    TOGL_KEY_ID_J,
    TOGL_KEY_ID_K,
    TOGL_KEY_ID_L,
    TOGL_KEY_ID_M,
    TOGL_KEY_ID_N,
    TOGL_KEY_ID_O,
    TOGL_KEY_ID_P,
    TOGL_KEY_ID_Q,
    TOGL_KEY_ID_R,
    TOGL_KEY_ID_S,
    TOGL_KEY_ID_T,
    TOGL_KEY_ID_U,
    TOGL_KEY_ID_V,
    TOGL_KEY_ID_W,
    TOGL_KEY_ID_X,
    TOGL_KEY_ID_Y,
    TOGL_KEY_ID_Z,

    TOGL_KEY_IDESCAPE               = 256, // out of ascii scope, no id collision with ascii code
    TOGL_KEY_IDENTER,  
    TOGL_KEY_IDCAPS_LOCK,   
    TOGL_KEY_IDTAB,    
    TOGL_KEY_IDBACKSPACE,
    TOGL_KEY_IDSPACE,

    TOGL_KEY_IDSHIFT,
    TOGL_KEY_IDCONTROL,
    TOGL_KEY_IDALT, 

    TOGL_KEY_IDPAGE_UP,               
    TOGL_KEY_IDPAGE_DOWN,       
    TOGL_KEY_IDEND, 
    TOGL_KEY_IDHOME,  

    TOGL_KEY_IDARROW_LEFT,  
    TOGL_KEY_IDARROW_UP, 
    TOGL_KEY_IDARROW_RIGHT,
    TOGL_KEY_IDARROW_DOWN,

    TOGL_KEY_IDINSERT,
    TOGL_KEY_IDDELETE,
    TOGL_KEY_IDBREAK,        
    TOGL_KEY_IDPAUSE,
    TOGL_KEY_IDPRINT,
    TOGL_KEY_IDPRINT_SCREEN,

    TOGL_KEY_IDNUMPAD_0,
    TOGL_KEY_IDNUMPAD_1,
    TOGL_KEY_IDNUMPAD_2,
    TOGL_KEY_IDNUMPAD_3,
    TOGL_KEY_IDNUMPAD_4,
    TOGL_KEY_IDNUMPAD_5,
    TOGL_KEY_IDNUMPAD_6,
    TOGL_KEY_IDNUMPAD_7,
    TOGL_KEY_IDNUMPAD_8,
    TOGL_KEY_IDNUMPAD_9,
    TOGL_KEY_IDNUMPAD_MULTIPLY,
    TOGL_KEY_IDNUMPAD_ADD,
    TOGL_KEY_IDNUMPAD_SEPARATOR,
    TOGL_KEY_IDNUMPAD_SUBTRACT,
    TOGL_KEY_IDNUMPAD_DECIMAL,
    TOGL_KEY_IDNUMPAD_DIVIDE,

    TOGL_KEY_IDSEMICOLON, 
    TOGL_KEY_IDFORWARD_SLASH,
    TOGL_KEY_IDACUTE,
    TOGL_KEY_IDOPEN_BRACKET,
    TOGL_KEY_IDBACK_SLASH,
    TOGL_KEY_IDCLOSE_BRACKET,
    TOGL_KEY_IDAPOSTROPHE,
    TOGL_KEY_IDCOMMA,
    TOGL_KEY_IDDOT,
    TOGL_KEY_IDDASH,
    TOGL_KEY_IDEQUAL,

    TOGL_KEY_ID_F1,
    TOGL_KEY_ID_F2,
    TOGL_KEY_ID_F3,
    TOGL_KEY_ID_F4,
    TOGL_KEY_ID_F5,
    TOGL_KEY_ID_F6,
    TOGL_KEY_ID_F7,
    TOGL_KEY_ID_F8,
    TOGL_KEY_ID_F9,
    TOGL_KEY_ID_F10,
    TOGL_KEY_ID_F11,
    TOGL_KEY_ID_F12,
    TOGL_KEY_ID_F13,
    TOGL_KEY_ID_F14,
    TOGL_KEY_ID_F15,
    TOGL_KEY_ID_F16,
    TOGL_KEY_ID_F17,
    TOGL_KEY_ID_F18,
    TOGL_KEY_ID_F19,
    TOGL_KEY_ID_F20,
    TOGL_KEY_ID_F21,
    TOGL_KEY_ID_F22,
    TOGL_KEY_ID_F23,
    TOGL_KEY_ID_F24,

    TOGL_KEY_IDNUMLOCK,
    TOGL_KEY_IDSCROLL_LOCK,

    TOGL_KEY_IDLEFT_MOUSE_BUTTON,
    TOGL_KEY_IDMIDDLE_MOUSE_BUTTON,
    TOGL_KEY_IDRIGHT_MOUSE_BUTTON,
    TOGL_KEY_IDX1_MOUSE_BUTTON,
    TOGL_KEY_IDX2_MOUSE_BUTTON,
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
    // Note: Used for shift, control, alt keys. For ids: TOGL_KEY_IDSHIFT, TOGL_KEY_IDCONTROL, TOGL_KEY_IDALT.
    TOGL_KeyboardSideId    keyboard_side   = TOGL_KEYBOARD_SIDE_ID_NONE;
};

std::string TOGL_KeyIdToStr(TOGL_KeyId key_id);
std::string TOGL_KeyboardSideToStr(TOGL_KeyboardSideId side);
std::string TOGL_ExtraToStr(const TOGL_Extra& extra);

// Returns true if key is toggled. For Example: TOGL_KEY_IDCAPS_LOCK, TOGL_KEY_IDINSERT.
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
    TOGL_CASE_STR(TOGL_KEY_IDUNKNOWN);  
    TOGL_CASE_STR(TOGL_KEY_IDBREAK);                  
    TOGL_CASE_STR(TOGL_KEY_IDBACKSPACE);              
    TOGL_CASE_STR(TOGL_KEY_IDTAB);                    
    TOGL_CASE_STR(TOGL_KEY_IDENTER);                  
    TOGL_CASE_STR(TOGL_KEY_IDSHIFT);                  
    TOGL_CASE_STR(TOGL_KEY_IDCONTROL);                
    TOGL_CASE_STR(TOGL_KEY_IDALT);                    
    TOGL_CASE_STR(TOGL_KEY_IDPAUSE);                  
    TOGL_CASE_STR(TOGL_KEY_IDCAPS_LOCK);              
    TOGL_CASE_STR(TOGL_KEY_IDESCAPE);                 
    TOGL_CASE_STR(TOGL_KEY_IDSPACE);                  
    TOGL_CASE_STR(TOGL_KEY_IDPAGE_UP);                
    TOGL_CASE_STR(TOGL_KEY_IDPAGE_DOWN);              
    TOGL_CASE_STR(TOGL_KEY_IDEND);                    
    TOGL_CASE_STR(TOGL_KEY_IDHOME);                   
    TOGL_CASE_STR(TOGL_KEY_IDARROW_LEFT);             
    TOGL_CASE_STR(TOGL_KEY_IDARROW_UP);               
    TOGL_CASE_STR(TOGL_KEY_IDARROW_RIGHT);            
    TOGL_CASE_STR(TOGL_KEY_IDARROW_DOWN);             
    TOGL_CASE_STR(TOGL_KEY_IDPRINT);                  
    TOGL_CASE_STR(TOGL_KEY_IDPRINT_SCREEN);           
    TOGL_CASE_STR(TOGL_KEY_IDINSERT);                 
    TOGL_CASE_STR(TOGL_KEY_IDDELETE);                 
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_0);               
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_1);               
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_2);               
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_3);               
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_4);               
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_5);               
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_6);               
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_7);               
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_8);               
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_9);               
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_MULTIPLY);        
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_ADD);             
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_SEPARATOR);       
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_SUBTRACT);        
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_DECIMAL);         
    TOGL_CASE_STR(TOGL_KEY_IDNUMPAD_DIVIDE);          
    TOGL_CASE_STR(TOGL_KEY_ID_F1);                     
    TOGL_CASE_STR(TOGL_KEY_ID_F2);                     
    TOGL_CASE_STR(TOGL_KEY_ID_F3);                     
    TOGL_CASE_STR(TOGL_KEY_ID_F4);                     
    TOGL_CASE_STR(TOGL_KEY_ID_F5);                     
    TOGL_CASE_STR(TOGL_KEY_ID_F6);                     
    TOGL_CASE_STR(TOGL_KEY_ID_F7);                     
    TOGL_CASE_STR(TOGL_KEY_ID_F8);                     
    TOGL_CASE_STR(TOGL_KEY_ID_F9);                     
    TOGL_CASE_STR(TOGL_KEY_ID_F10);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F11);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F12);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F13);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F14);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F15);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F16);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F17);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F18);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F19);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F20);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F21);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F22);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F23);                    
    TOGL_CASE_STR(TOGL_KEY_ID_F24);   
    TOGL_CASE_STR(TOGL_KEY_ID_0);    
    TOGL_CASE_STR(TOGL_KEY_ID_1);    
    TOGL_CASE_STR(TOGL_KEY_ID_2);    
    TOGL_CASE_STR(TOGL_KEY_ID_3);    
    TOGL_CASE_STR(TOGL_KEY_ID_4);    
    TOGL_CASE_STR(TOGL_KEY_ID_5);    
    TOGL_CASE_STR(TOGL_KEY_ID_6);    
    TOGL_CASE_STR(TOGL_KEY_ID_7);    
    TOGL_CASE_STR(TOGL_KEY_ID_8);    
    TOGL_CASE_STR(TOGL_KEY_ID_9);    
    TOGL_CASE_STR(TOGL_KEY_ID_A);    
    TOGL_CASE_STR(TOGL_KEY_ID_B);    
    TOGL_CASE_STR(TOGL_KEY_ID_C);    
    TOGL_CASE_STR(TOGL_KEY_ID_D);    
    TOGL_CASE_STR(TOGL_KEY_ID_E);    
    TOGL_CASE_STR(TOGL_KEY_ID_F);    
    TOGL_CASE_STR(TOGL_KEY_ID_G);    
    TOGL_CASE_STR(TOGL_KEY_ID_H);    
    TOGL_CASE_STR(TOGL_KEY_ID_I);    
    TOGL_CASE_STR(TOGL_KEY_ID_J);    
    TOGL_CASE_STR(TOGL_KEY_ID_K);    
    TOGL_CASE_STR(TOGL_KEY_ID_L);    
    TOGL_CASE_STR(TOGL_KEY_ID_M);    
    TOGL_CASE_STR(TOGL_KEY_ID_N);    
    TOGL_CASE_STR(TOGL_KEY_ID_O);    
    TOGL_CASE_STR(TOGL_KEY_ID_P);    
    TOGL_CASE_STR(TOGL_KEY_ID_Q);    
    TOGL_CASE_STR(TOGL_KEY_ID_R);    
    TOGL_CASE_STR(TOGL_KEY_ID_S);    
    TOGL_CASE_STR(TOGL_KEY_ID_T);    
    TOGL_CASE_STR(TOGL_KEY_ID_U);    
    TOGL_CASE_STR(TOGL_KEY_ID_V);    
    TOGL_CASE_STR(TOGL_KEY_ID_W);    
    TOGL_CASE_STR(TOGL_KEY_ID_X);    
    TOGL_CASE_STR(TOGL_KEY_ID_Y);    
    TOGL_CASE_STR(TOGL_KEY_ID_Z);

    TOGL_CASE_STR(TOGL_KEY_IDSEMICOLON);
    TOGL_CASE_STR(TOGL_KEY_IDFORWARD_SLASH);
    TOGL_CASE_STR(TOGL_KEY_IDACUTE);
    TOGL_CASE_STR(TOGL_KEY_IDOPEN_BRACKET);
    TOGL_CASE_STR(TOGL_KEY_IDBACK_SLASH);
    TOGL_CASE_STR(TOGL_KEY_IDCLOSE_BRACKET);
    TOGL_CASE_STR(TOGL_KEY_IDAPOSTROPHE);
    TOGL_CASE_STR(TOGL_KEY_IDCOMMA);
    TOGL_CASE_STR(TOGL_KEY_IDDOT);
    TOGL_CASE_STR(TOGL_KEY_IDDASH);
    TOGL_CASE_STR(TOGL_KEY_IDEQUAL);

    TOGL_CASE_STR(TOGL_KEY_IDNUMLOCK);                
    TOGL_CASE_STR(TOGL_KEY_IDSCROLL_LOCK);   
              
    TOGL_CASE_STR(TOGL_KEY_IDLEFT_MOUSE_BUTTON);   
    TOGL_CASE_STR(TOGL_KEY_IDMIDDLE_MOUSE_BUTTON);   
    TOGL_CASE_STR(TOGL_KEY_IDRIGHT_MOUSE_BUTTON);   
    TOGL_CASE_STR(TOGL_KEY_IDX1_MOUSE_BUTTON);   
    TOGL_CASE_STR(TOGL_KEY_IDX2_MOUSE_BUTTON);   
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
    case VK_CANCEL:         return TOGL_KEY_IDBREAK;                  
    case VK_BACK:           return TOGL_KEY_IDBACKSPACE;              
    case VK_TAB:            return TOGL_KEY_IDTAB;                    
    case VK_RETURN:         return TOGL_KEY_IDENTER; 
    case VK_SHIFT:          return TOGL_KEY_IDSHIFT; 
    case VK_CONTROL:        return TOGL_KEY_IDCONTROL; 
    case VK_MENU:           return TOGL_KEY_IDALT; 

    case VK_PAUSE:          return TOGL_KEY_IDPAUSE;                  
    case VK_CAPITAL:        return TOGL_KEY_IDCAPS_LOCK;              
    case VK_ESCAPE:         return TOGL_KEY_IDESCAPE;                 
    case VK_SPACE:          return TOGL_KEY_IDSPACE;                  
    case VK_PRIOR:          return TOGL_KEY_IDPAGE_UP;                
    case VK_NEXT:           return TOGL_KEY_IDPAGE_DOWN;              
    case VK_END:            return TOGL_KEY_IDEND;                    
    case VK_HOME:           return TOGL_KEY_IDHOME;                   
    case VK_LEFT:           return TOGL_KEY_IDARROW_LEFT;             
    case VK_UP:             return TOGL_KEY_IDARROW_UP;               
    case VK_RIGHT:          return TOGL_KEY_IDARROW_RIGHT;            
    case VK_DOWN:           return TOGL_KEY_IDARROW_DOWN;             
    case VK_PRINT:          return TOGL_KEY_IDPRINT;                  
    case VK_SNAPSHOT:       return TOGL_KEY_IDPRINT_SCREEN;           
    case VK_INSERT:         return TOGL_KEY_IDINSERT;                 
    case VK_DELETE:         return TOGL_KEY_IDDELETE;                 
    case VK_NUMPAD0:        return TOGL_KEY_IDNUMPAD_0;               
    case VK_NUMPAD1:        return TOGL_KEY_IDNUMPAD_1;               
    case VK_NUMPAD2:        return TOGL_KEY_IDNUMPAD_2;               
    case VK_NUMPAD3:        return TOGL_KEY_IDNUMPAD_3;               
    case VK_NUMPAD4:        return TOGL_KEY_IDNUMPAD_4;               
    case VK_NUMPAD5:        return TOGL_KEY_IDNUMPAD_5;               
    case VK_NUMPAD6:        return TOGL_KEY_IDNUMPAD_6;               
    case VK_NUMPAD7:        return TOGL_KEY_IDNUMPAD_7;               
    case VK_NUMPAD8:        return TOGL_KEY_IDNUMPAD_8;               
    case VK_NUMPAD9:        return TOGL_KEY_IDNUMPAD_9;               
    case VK_MULTIPLY:       return TOGL_KEY_IDNUMPAD_MULTIPLY;        
    case VK_ADD:            return TOGL_KEY_IDNUMPAD_ADD;             
    case VK_SEPARATOR:      return TOGL_KEY_IDNUMPAD_SEPARATOR;       
    case VK_SUBTRACT:       return TOGL_KEY_IDNUMPAD_SUBTRACT;        
    case VK_DECIMAL:        return TOGL_KEY_IDNUMPAD_DECIMAL;         
    case VK_DIVIDE:         return TOGL_KEY_IDNUMPAD_DIVIDE;          
    case VK_F1:             return TOGL_KEY_ID_F1;                     
    case VK_F2:             return TOGL_KEY_ID_F2;                     
    case VK_F3:             return TOGL_KEY_ID_F3;                     
    case VK_F4:             return TOGL_KEY_ID_F4;                     
    case VK_F5:             return TOGL_KEY_ID_F5;                     
    case VK_F6:             return TOGL_KEY_ID_F6;                     
    case VK_F7:             return TOGL_KEY_ID_F7;                     
    case VK_F8:             return TOGL_KEY_ID_F8;                     
    case VK_F9:             return TOGL_KEY_ID_F9;                     
    case VK_F10:            return TOGL_KEY_ID_F10;                    
    case VK_F11:            return TOGL_KEY_ID_F11;                    
    case VK_F12:            return TOGL_KEY_ID_F12;                    
    case VK_F13:            return TOGL_KEY_ID_F13;                    
    case VK_F14:            return TOGL_KEY_ID_F14;                    
    case VK_F15:            return TOGL_KEY_ID_F15;                    
    case VK_F16:            return TOGL_KEY_ID_F16;                    
    case VK_F17:            return TOGL_KEY_ID_F17;                    
    case VK_F18:            return TOGL_KEY_ID_F18;                    
    case VK_F19:            return TOGL_KEY_ID_F19;                    
    case VK_F20:            return TOGL_KEY_ID_F20;                    
    case VK_F21:            return TOGL_KEY_ID_F21;                    
    case VK_F22:            return TOGL_KEY_ID_F22;                    
    case VK_F23:            return TOGL_KEY_ID_F23;                    
    case VK_F24:            return TOGL_KEY_ID_F24;   
    case '0':               return TOGL_KEY_ID_0;    
    case '1':               return TOGL_KEY_ID_1;    
    case '2':               return TOGL_KEY_ID_2;    
    case '3':               return TOGL_KEY_ID_3;    
    case '4':               return TOGL_KEY_ID_4;    
    case '5':               return TOGL_KEY_ID_5;    
    case '6':               return TOGL_KEY_ID_6;    
    case '7':               return TOGL_KEY_ID_7;    
    case '8':               return TOGL_KEY_ID_8;    
    case '9':               return TOGL_KEY_ID_9;    
    case 'A':               return TOGL_KEY_ID_A;    
    case 'B':               return TOGL_KEY_ID_B;    
    case 'C':               return TOGL_KEY_ID_C;    
    case 'D':               return TOGL_KEY_ID_D;    
    case 'E':               return TOGL_KEY_ID_E;    
    case 'F':               return TOGL_KEY_ID_F;    
    case 'G':               return TOGL_KEY_ID_G;    
    case 'H':               return TOGL_KEY_ID_H;    
    case 'I':               return TOGL_KEY_ID_I;    
    case 'J':               return TOGL_KEY_ID_J;    
    case 'K':               return TOGL_KEY_ID_K;    
    case 'L':               return TOGL_KEY_ID_L;    
    case 'M':               return TOGL_KEY_ID_M;    
    case 'N':               return TOGL_KEY_ID_N;    
    case 'O':               return TOGL_KEY_ID_O;    
    case 'P':               return TOGL_KEY_ID_P;    
    case 'Q':               return TOGL_KEY_ID_Q;    
    case 'R':               return TOGL_KEY_ID_R;    
    case 'S':               return TOGL_KEY_ID_S;    
    case 'T':               return TOGL_KEY_ID_T;    
    case 'U':               return TOGL_KEY_ID_U;    
    case 'V':               return TOGL_KEY_ID_V;    
    case 'W':               return TOGL_KEY_ID_W;    
    case 'X':               return TOGL_KEY_ID_X;    
    case 'Y':               return TOGL_KEY_ID_Y;    
    case 'Z':               return TOGL_KEY_ID_Z;

    case VK_OEM_1:          return TOGL_KEY_IDSEMICOLON;        // ;
    case VK_OEM_2:          return TOGL_KEY_IDFORWARD_SLASH;    // /
    case VK_OEM_3:          return TOGL_KEY_IDACUTE;            // `
    case VK_OEM_4:          return TOGL_KEY_IDOPEN_BRACKET;     // [
    case VK_OEM_5:          return TOGL_KEY_IDBACK_SLASH;       
    case VK_OEM_6:          return TOGL_KEY_IDCLOSE_BRACKET;    // ]
    case VK_OEM_7:          return TOGL_KEY_IDAPOSTROPHE;       // '
    case VK_OEM_COMMA:      return TOGL_KEY_IDCOMMA;            // ,
    case VK_OEM_PERIOD:     return TOGL_KEY_IDDOT;              // .
    case VK_OEM_MINUS:      return TOGL_KEY_IDDASH;             // -
    case VK_OEM_PLUS:       return TOGL_KEY_IDEQUAL;            // =

    case VK_NUMLOCK:        return TOGL_KEY_IDNUMLOCK;                
    case VK_SCROLL:         return TOGL_KEY_IDSCROLL_LOCK;      
    }
    return TOGL_KEY_IDUNKNOWN;
}

inline int _TOGL_InnerKey::GetVirtualKeyCode(TOGL_KeyId key_id) {
    switch (key_id) {
    case TOGL_KEY_IDBREAK:                  return VK_CANCEL;                           
    case TOGL_KEY_IDBACKSPACE:              return VK_BACK;           
    case TOGL_KEY_IDTAB:                    return VK_TAB;            
    case TOGL_KEY_IDENTER:                  return VK_RETURN;         
    case TOGL_KEY_IDSHIFT:                  return VK_SHIFT;          
    case TOGL_KEY_IDCONTROL:                return VK_CONTROL;        
    case TOGL_KEY_IDALT:                    return VK_MENU;           

    case TOGL_KEY_IDPAUSE:                  return VK_PAUSE;          
    case TOGL_KEY_IDCAPS_LOCK:              return VK_CAPITAL;        
    case TOGL_KEY_IDESCAPE:                 return VK_ESCAPE;         
    case TOGL_KEY_IDSPACE:                  return VK_SPACE;          
    case TOGL_KEY_IDPAGE_UP:                return VK_PRIOR;          
    case TOGL_KEY_IDPAGE_DOWN:              return VK_NEXT;           
    case TOGL_KEY_IDEND:                    return VK_END;            
    case TOGL_KEY_IDHOME:                   return VK_HOME;           
    case TOGL_KEY_IDARROW_LEFT:             return VK_LEFT;           
    case TOGL_KEY_IDARROW_UP:               return VK_UP;             
    case TOGL_KEY_IDARROW_RIGHT:            return VK_RIGHT;          
    case TOGL_KEY_IDARROW_DOWN:             return VK_DOWN;           
    case TOGL_KEY_IDPRINT:                  return VK_PRINT;          
    case TOGL_KEY_IDPRINT_SCREEN:           return VK_SNAPSHOT;       
    case TOGL_KEY_IDINSERT:                 return VK_INSERT;         
    case TOGL_KEY_IDDELETE:                 return VK_DELETE;         
    case TOGL_KEY_IDNUMPAD_0:               return VK_NUMPAD0;        
    case TOGL_KEY_IDNUMPAD_1:               return VK_NUMPAD1;        
    case TOGL_KEY_IDNUMPAD_2:               return VK_NUMPAD2;        
    case TOGL_KEY_IDNUMPAD_3:               return VK_NUMPAD3;        
    case TOGL_KEY_IDNUMPAD_4:               return VK_NUMPAD4;        
    case TOGL_KEY_IDNUMPAD_5:               return VK_NUMPAD5;        
    case TOGL_KEY_IDNUMPAD_6:               return VK_NUMPAD6;        
    case TOGL_KEY_IDNUMPAD_7:               return VK_NUMPAD7;        
    case TOGL_KEY_IDNUMPAD_8:               return VK_NUMPAD8;        
    case TOGL_KEY_IDNUMPAD_9:               return VK_NUMPAD9;        
    case TOGL_KEY_IDNUMPAD_MULTIPLY:        return VK_MULTIPLY;       
    case TOGL_KEY_IDNUMPAD_ADD:             return VK_ADD;            
    case TOGL_KEY_IDNUMPAD_SEPARATOR:       return VK_SEPARATOR;      
    case TOGL_KEY_IDNUMPAD_SUBTRACT:        return VK_SUBTRACT;       
    case TOGL_KEY_IDNUMPAD_DECIMAL:         return VK_DECIMAL;        
    case TOGL_KEY_IDNUMPAD_DIVIDE:          return VK_DIVIDE;         
    case TOGL_KEY_ID_F1:                    return VK_F1;             
    case TOGL_KEY_ID_F2:                    return VK_F2;             
    case TOGL_KEY_ID_F3:                    return VK_F3;             
    case TOGL_KEY_ID_F4:                    return VK_F4;             
    case TOGL_KEY_ID_F5:                    return VK_F5;             
    case TOGL_KEY_ID_F6:                    return VK_F6;             
    case TOGL_KEY_ID_F7:                    return VK_F7;             
    case TOGL_KEY_ID_F8:                    return VK_F8;             
    case TOGL_KEY_ID_F9:                    return VK_F9;             
    case TOGL_KEY_ID_F10:                   return VK_F10;            
    case TOGL_KEY_ID_F11:                   return VK_F11;            
    case TOGL_KEY_ID_F12:                   return VK_F12;            
    case TOGL_KEY_ID_F13:                   return VK_F13;            
    case TOGL_KEY_ID_F14:                   return VK_F14;            
    case TOGL_KEY_ID_F15:                   return VK_F15;            
    case TOGL_KEY_ID_F16:                   return VK_F16;            
    case TOGL_KEY_ID_F17:                   return VK_F17;            
    case TOGL_KEY_ID_F18:                   return VK_F18;            
    case TOGL_KEY_ID_F19:                   return VK_F19;            
    case TOGL_KEY_ID_F20:                   return VK_F20;            
    case TOGL_KEY_ID_F21:                   return VK_F21;            
    case TOGL_KEY_ID_F22:                   return VK_F22;            
    case TOGL_KEY_ID_F23:                   return VK_F23;            
    case TOGL_KEY_ID_F24:                   return VK_F24;            
    case TOGL_KEY_ID_0:                     return '0';               
    case TOGL_KEY_ID_1:                     return '1';               
    case TOGL_KEY_ID_2:                     return '2';               
    case TOGL_KEY_ID_3:                     return '3';               
    case TOGL_KEY_ID_4:                     return '4';               
    case TOGL_KEY_ID_5:                     return '5';               
    case TOGL_KEY_ID_6:                     return '6';               
    case TOGL_KEY_ID_7:                     return '7';               
    case TOGL_KEY_ID_8:                     return '8';               
    case TOGL_KEY_ID_9:                     return '9';               
    case TOGL_KEY_ID_A:                     return 'A';               
    case TOGL_KEY_ID_B:                     return 'B';               
    case TOGL_KEY_ID_C:                     return 'C';               
    case TOGL_KEY_ID_D:                     return 'D';               
    case TOGL_KEY_ID_E:                     return 'E';               
    case TOGL_KEY_ID_F:                     return 'F';               
    case TOGL_KEY_ID_G:                     return 'G';               
    case TOGL_KEY_ID_H:                     return 'H';               
    case TOGL_KEY_ID_I:                     return 'I';               
    case TOGL_KEY_ID_J:                     return 'J';               
    case TOGL_KEY_ID_K:                     return 'K';               
    case TOGL_KEY_ID_L:                     return 'L';               
    case TOGL_KEY_ID_M:                     return 'M';               
    case TOGL_KEY_ID_N:                     return 'N';               
    case TOGL_KEY_ID_O:                     return 'O';               
    case TOGL_KEY_ID_P:                     return 'P';               
    case TOGL_KEY_ID_Q:                     return 'Q';               
    case TOGL_KEY_ID_R:                     return 'R';               
    case TOGL_KEY_ID_S:                     return 'S';               
    case TOGL_KEY_ID_T:                     return 'T';               
    case TOGL_KEY_ID_U:                     return 'U';               
    case TOGL_KEY_ID_V:                     return 'V';               
    case TOGL_KEY_ID_W:                     return 'W';               
    case TOGL_KEY_ID_X:                     return 'X';               
    case TOGL_KEY_ID_Y:                     return 'Y';               
    case TOGL_KEY_ID_Z:                     return 'Z';               

    case TOGL_KEY_IDSEMICOLON:              return VK_OEM_1;          
    case TOGL_KEY_IDFORWARD_SLASH:          return VK_OEM_2;          
    case TOGL_KEY_IDACUTE:                  return VK_OEM_3;          
    case TOGL_KEY_IDOPEN_BRACKET:           return VK_OEM_4;          
    case TOGL_KEY_IDBACK_SLASH:             return VK_OEM_5;          
    case TOGL_KEY_IDCLOSE_BRACKET:          return VK_OEM_6;          
    case TOGL_KEY_IDAPOSTROPHE:             return VK_OEM_7;          
    case TOGL_KEY_IDCOMMA:                  return VK_OEM_COMMA;        
    case TOGL_KEY_IDDOT:                    return VK_OEM_PERIOD;        
    case TOGL_KEY_IDDASH:                   return VK_OEM_MINUS;        
    case TOGL_KEY_IDEQUAL:                  return VK_OEM_PLUS;        

    case TOGL_KEY_IDNUMLOCK:                return VK_NUMLOCK;        
    case TOGL_KEY_IDSCROLL_LOCK:            return VK_SCROLL;   

    case TOGL_KEY_IDUNKNOWN:
    default:                                return 0;
    }
}

inline TOGL_KeyId _TOGL_InnerKey::GetMouseKeyId(UINT message, WPARAM w_param) {
    switch (message) {
    case WM_LBUTTONDOWN:    
    case WM_LBUTTONUP:      
        return TOGL_KEY_IDLEFT_MOUSE_BUTTON;

    case WM_RBUTTONDOWN:    
    case WM_RBUTTONUP:      
        return TOGL_KEY_IDRIGHT_MOUSE_BUTTON;

    case WM_MBUTTONDOWN:    
    case WM_MBUTTONUP:      
        return TOGL_KEY_IDMIDDLE_MOUSE_BUTTON;

    case WM_XBUTTONUP:
    case WM_XBUTTONDOWN:   
        if (HIWORD(w_param) == XBUTTON1) return TOGL_KEY_IDX1_MOUSE_BUTTON;
        if (HIWORD(w_param) == XBUTTON2) return TOGL_KEY_IDX2_MOUSE_BUTTON;
        return TOGL_KEY_IDUNKNOWN;
    } // switch

    return TOGL_KEY_IDUNKNOWN;
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
    case TOGL_KEY_IDSHIFT: {
        const int vk_code_ext = MapVirtualKeyA(virtual_key_data.scan_code, MAPVK_VSC_TO_VK_EX);
        if (vk_code_ext == VK_LSHIFT) return TOGL_KEYBOARD_SIDE_ID_LEFT; 
        if (vk_code_ext == VK_RSHIFT) return TOGL_KEYBOARD_SIDE_ID_RIGHT; 
        return TOGL_KEYBOARD_SIDE_ID_NONE;
    } 
    case TOGL_KEY_IDCONTROL: 
        if (virtual_key_data.is_ext) return TOGL_KEYBOARD_SIDE_ID_RIGHT; 
        return TOGL_KEYBOARD_SIDE_ID_LEFT; 

    case TOGL_KEY_IDALT: 
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

