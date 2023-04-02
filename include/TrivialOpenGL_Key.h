/**
* @file TrivialOpenGL_Key.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_KEY_H_
#define TRIVIALOPENGL_KEY_H_

#include "TrivialOpenGL_Utility.h"

namespace TrivialOpenGL {

    enum KeyId {
        KEY_ID_UNKNOWN,

        KEY_ID_BREAK,            
        KEY_ID_BACKSPACE,
        KEY_ID_TAB,
        KEY_ID_ENTER,  
        KEY_ID_SHIFT,
        KEY_ID_CONTROL,
        KEY_ID_ALT, 
        KEY_ID_PAUSE,
        KEY_ID_CAPS_LOCK,       
        KEY_ID_ESCAPE,
        KEY_ID_SPACE,
        KEY_ID_PAGE_UP,               
        KEY_ID_PAGE_DOWN,       
        KEY_ID_END, 
        KEY_ID_HOME,  
        KEY_ID_ARROW_LEFT,  
        KEY_ID_ARROW_UP, 
        KEY_ID_ARROW_RIGHT,
        KEY_ID_ARROW_DOWN,
        KEY_ID_PRINT,
        KEY_ID_PRINT_SCREEN,
        KEY_ID_INSERT,
        KEY_ID_DELETE,
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
        KEY_ID_LEFT_SHIFT, 
        KEY_ID_RIGHT_SHIFT,
        KEY_ID_LEFT_CONTROL, 
        KEY_ID_RIGHT_CONTROL,
        KEY_ID_LEFT_ALT,
        KEY_ID_RIGHT_ALT,
    };

    KeyId VK_CodeToKeyId(int vk_code);
    std::string VK_CodeToStr(int vk_code); 

    inline KeyId VK_CodeToKeyId(int vk_code) {
        switch (vk_code) {
        case VK_CANCEL:           return KEY_ID_BREAK;                  
        case VK_BACK:             return KEY_ID_BACKSPACE;              
        case VK_TAB:              return KEY_ID_TAB;                    
        case VK_RETURN:           return KEY_ID_ENTER;                  
        case VK_SHIFT:            return KEY_ID_SHIFT;                  
        case VK_CONTROL:          return KEY_ID_CONTROL;                
        case VK_MENU:             return KEY_ID_ALT;                    
        case VK_PAUSE:            return KEY_ID_PAUSE;                  
        case VK_CAPITAL:          return KEY_ID_CAPS_LOCK;              
        case VK_ESCAPE:           return KEY_ID_ESCAPE;                 
        case VK_SPACE:            return KEY_ID_SPACE;                  
        case VK_PRIOR:            return KEY_ID_PAGE_UP;                
        case VK_NEXT:             return KEY_ID_PAGE_DOWN;              
        case VK_END:              return KEY_ID_END;                    
        case VK_HOME:             return KEY_ID_HOME;                   
        case VK_LEFT:             return KEY_ID_ARROW_LEFT;             
        case VK_UP:               return KEY_ID_ARROW_UP;               
        case VK_RIGHT:            return KEY_ID_ARROW_RIGHT;            
        case VK_DOWN:             return KEY_ID_ARROW_DOWN;             
        case VK_PRINT:            return KEY_ID_PRINT;                  
        case VK_SNAPSHOT:         return KEY_ID_PRINT_SCREEN;           
        case VK_INSERT:           return KEY_ID_INSERT;                 
        case VK_DELETE:           return KEY_ID_DELETE;                 
        case VK_NUMPAD0:          return KEY_ID_NUMPAD_0;               
        case VK_NUMPAD1:          return KEY_ID_NUMPAD_1;               
        case VK_NUMPAD2:          return KEY_ID_NUMPAD_2;               
        case VK_NUMPAD3:          return KEY_ID_NUMPAD_3;               
        case VK_NUMPAD4:          return KEY_ID_NUMPAD_4;               
        case VK_NUMPAD5:          return KEY_ID_NUMPAD_5;               
        case VK_NUMPAD6:          return KEY_ID_NUMPAD_6;               
        case VK_NUMPAD7:          return KEY_ID_NUMPAD_7;               
        case VK_NUMPAD8:          return KEY_ID_NUMPAD_8;               
        case VK_NUMPAD9:          return KEY_ID_NUMPAD_9;               
        case VK_MULTIPLY:         return KEY_ID_NUMPAD_MULTIPLY;        
        case VK_ADD:              return KEY_ID_NUMPAD_ADD;             
        case VK_SEPARATOR:        return KEY_ID_NUMPAD_SEPARATOR;       
        case VK_SUBTRACT:         return KEY_ID_NUMPAD_SUBTRACT;        
        case VK_DECIMAL:          return KEY_ID_NUMPAD_DECIMAL;         
        case VK_DIVIDE:           return KEY_ID_NUMPAD_DIVIDE;          
        case VK_F1:               return KEY_ID_F1;                     
        case VK_F2:               return KEY_ID_F2;                     
        case VK_F3:               return KEY_ID_F3;                     
        case VK_F4:               return KEY_ID_F4;                     
        case VK_F5:               return KEY_ID_F5;                     
        case VK_F6:               return KEY_ID_F6;                     
        case VK_F7:               return KEY_ID_F7;                     
        case VK_F8:               return KEY_ID_F8;                     
        case VK_F9:               return KEY_ID_F9;                     
        case VK_F10:              return KEY_ID_F10;                    
        case VK_F11:              return KEY_ID_F11;                    
        case VK_F12:              return KEY_ID_F12;                    
        case VK_F13:              return KEY_ID_F13;                    
        case VK_F14:              return KEY_ID_F14;                    
        case VK_F15:              return KEY_ID_F15;                    
        case VK_F16:              return KEY_ID_F16;                    
        case VK_F17:              return KEY_ID_F17;                    
        case VK_F18:              return KEY_ID_F18;                    
        case VK_F19:              return KEY_ID_F19;                    
        case VK_F20:              return KEY_ID_F20;                    
        case VK_F21:              return KEY_ID_F21;                    
        case VK_F22:              return KEY_ID_F22;                    
        case VK_F23:              return KEY_ID_F23;                    
        case VK_F24:              return KEY_ID_F24;                    
        case VK_NUMLOCK:          return KEY_ID_NUMLOCK;                
        case VK_SCROLL:           return KEY_ID_SCROLL_LOCK;            
        case VK_LSHIFT:           return KEY_ID_LEFT_SHIFT;             
        case VK_RSHIFT:           return KEY_ID_RIGHT_SHIFT;            
        case VK_LCONTROL:         return KEY_ID_LEFT_CONTROL;           
        case VK_RCONTROL:         return KEY_ID_RIGHT_CONTROL;          
        case VK_LMENU:            return KEY_ID_LEFT_ALT;               
        case VK_RMENU:            return KEY_ID_RIGHT_ALT;    
        }
        return KEY_ID_UNKNOWN;
    }

#ifdef TOGL_INNER_CASE_STR
#error TOGL_INNER_CASE_STR is already defined.
#endif

#define TOGL_INNER_CASE_STR(name) case name: return #name

    inline std::string VK_CodeToStr(int vk_code) {
        switch (vk_code) {
        TOGL_INNER_CASE_STR(VK_CANCEL);        
        TOGL_INNER_CASE_STR(VK_BACK);          
        TOGL_INNER_CASE_STR(VK_TAB);           
        TOGL_INNER_CASE_STR(VK_RETURN);        
        TOGL_INNER_CASE_STR(VK_SHIFT);         
        TOGL_INNER_CASE_STR(VK_CONTROL);       
        TOGL_INNER_CASE_STR(VK_MENU);          
        TOGL_INNER_CASE_STR(VK_PAUSE);         
        TOGL_INNER_CASE_STR(VK_CAPITAL);       
        TOGL_INNER_CASE_STR(VK_ESCAPE);        
        TOGL_INNER_CASE_STR(VK_SPACE);         
        TOGL_INNER_CASE_STR(VK_PRIOR);         
        TOGL_INNER_CASE_STR(VK_NEXT);          
        TOGL_INNER_CASE_STR(VK_END);           
        TOGL_INNER_CASE_STR(VK_HOME);          
        TOGL_INNER_CASE_STR(VK_LEFT);          
        TOGL_INNER_CASE_STR(VK_UP);            
        TOGL_INNER_CASE_STR(VK_RIGHT);         
        TOGL_INNER_CASE_STR(VK_DOWN);          
        TOGL_INNER_CASE_STR(VK_PRINT);         
        TOGL_INNER_CASE_STR(VK_SNAPSHOT);      
        TOGL_INNER_CASE_STR(VK_INSERT);        
        TOGL_INNER_CASE_STR(VK_DELETE);        
        TOGL_INNER_CASE_STR(VK_NUMPAD0);       
        TOGL_INNER_CASE_STR(VK_NUMPAD1);       
        TOGL_INNER_CASE_STR(VK_NUMPAD2);       
        TOGL_INNER_CASE_STR(VK_NUMPAD3);       
        TOGL_INNER_CASE_STR(VK_NUMPAD4);       
        TOGL_INNER_CASE_STR(VK_NUMPAD5);       
        TOGL_INNER_CASE_STR(VK_NUMPAD6);       
        TOGL_INNER_CASE_STR(VK_NUMPAD7);       
        TOGL_INNER_CASE_STR(VK_NUMPAD8);       
        TOGL_INNER_CASE_STR(VK_NUMPAD9);       
        TOGL_INNER_CASE_STR(VK_MULTIPLY);      
        TOGL_INNER_CASE_STR(VK_ADD);           
        TOGL_INNER_CASE_STR(VK_SEPARATOR);     
        TOGL_INNER_CASE_STR(VK_SUBTRACT);      
        TOGL_INNER_CASE_STR(VK_DECIMAL);       
        TOGL_INNER_CASE_STR(VK_DIVIDE);        
        TOGL_INNER_CASE_STR(VK_F1);            
        TOGL_INNER_CASE_STR(VK_F2);            
        TOGL_INNER_CASE_STR(VK_F3);            
        TOGL_INNER_CASE_STR(VK_F4);            
        TOGL_INNER_CASE_STR(VK_F5);            
        TOGL_INNER_CASE_STR(VK_F6);            
        TOGL_INNER_CASE_STR(VK_F7);            
        TOGL_INNER_CASE_STR(VK_F8);            
        TOGL_INNER_CASE_STR(VK_F9);            
        TOGL_INNER_CASE_STR(VK_F10);           
        TOGL_INNER_CASE_STR(VK_F11);           
        TOGL_INNER_CASE_STR(VK_F12);           
        TOGL_INNER_CASE_STR(VK_F13);           
        TOGL_INNER_CASE_STR(VK_F14);           
        TOGL_INNER_CASE_STR(VK_F15);           
        TOGL_INNER_CASE_STR(VK_F16);           
        TOGL_INNER_CASE_STR(VK_F17);           
        TOGL_INNER_CASE_STR(VK_F18);           
        TOGL_INNER_CASE_STR(VK_F19);           
        TOGL_INNER_CASE_STR(VK_F20);           
        TOGL_INNER_CASE_STR(VK_F21);           
        TOGL_INNER_CASE_STR(VK_F22);           
        TOGL_INNER_CASE_STR(VK_F23);           
        TOGL_INNER_CASE_STR(VK_F24);           
        TOGL_INNER_CASE_STR(VK_NUMLOCK);       
        TOGL_INNER_CASE_STR(VK_SCROLL);        
        TOGL_INNER_CASE_STR(VK_LSHIFT);        
        TOGL_INNER_CASE_STR(VK_RSHIFT);        
        TOGL_INNER_CASE_STR(VK_LCONTROL);      
        TOGL_INNER_CASE_STR(VK_RCONTROL);      
        TOGL_INNER_CASE_STR(VK_LMENU);         
        TOGL_INNER_CASE_STR(VK_RMENU);   
        }
        return std::string() + "(" + std::to_string(vk_code) + ")";
    }
#undef TOGL_INNER_CASE_STR

} // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_KEY_H_

