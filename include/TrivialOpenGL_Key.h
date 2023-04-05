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

        KEY_ID_ESCAPE,
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

        KEY_ID_0,
        KEY_ID_1,
        KEY_ID_2,
        KEY_ID_3,
        KEY_ID_4,
        KEY_ID_5,
        KEY_ID_6,
        KEY_ID_7,
        KEY_ID_8,
        KEY_ID_9,
        KEY_ID_A,
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

#if 0
        // Experimental. For tests only.
        KEY_ID_LEFT_SHIFT, 
        KEY_ID_RIGHT_SHIFT,
        KEY_ID_LEFT_CONTROL, 
        KEY_ID_RIGHT_CONTROL,
        KEY_ID_LEFT_ALT,
        KEY_ID_RIGHT_ALT,
#endif
    };

    struct VirtualKeyData {
        // For more info check lParam for WM_KEYDOWN and WM_KEYUP on msdn.
        uint32_t	count		    : 16,
                    scan_code       : 8,
                    is_ext          : 1,
                    reserved1       : 4,
                    context_code    : 1,
                    prev_state      : 1,
                    trans_state     : 1;
    };

    KeyId VK_CodeToKeyId(int vk_code, const VirtualKeyData& data);
    std::string KeyIdToStr(KeyId key_id);
    std::string VK_CodeToStr(int vk_code); 
    std::string WinApiKeyDataToStr(WPARAM w_param, LPARAM l_param);

    inline KeyId VK_CodeToKeyId(int vk_code, const VirtualKeyData& data) {
        switch (vk_code) {
        case VK_CANCEL:         return KEY_ID_BREAK;                  
        case VK_BACK:           return KEY_ID_BACKSPACE;              
        case VK_TAB:            return KEY_ID_TAB;                    
        case VK_RETURN:         return KEY_ID_ENTER; 

#if 0
        // Experimental. For tests only.
        // Detection if shift, control, alt key was left or right.
        case VK_SHIFT: {
            const int vk_code_ext = MapVirtualKeyA(data.scan_code, MAPVK_VSC_TO_VK_EX);

            if (vk_code_ext == VK_LSHIFT) return KEY_ID_LEFT_SHIFT;
            return KEY_ID_RIGHT_SHIFT;  
        }
                            
        case VK_CONTROL: {
            if (data.is_ext == 0) return KEY_ID_LEFT_CONTROL;
            return KEY_ID_RIGHT_CONTROL;   
        }

        case VK_MENU: {
            if (data.is_ext == 0) return KEY_ID_LEFT_ALT;
            return KEY_ID_RIGHT_ALT;   
        }          
#endif
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
        case VK_OEM_COMMA:	    return KEY_ID_COMMA;            // ,
        case VK_OEM_PERIOD:	    return KEY_ID_DOT;              // .
        case VK_OEM_MINUS:	    return KEY_ID_DASH;             // -
        case VK_OEM_PLUS:	    return KEY_ID_EQUAL;            // =

        case VK_NUMLOCK:        return KEY_ID_NUMLOCK;                
        case VK_SCROLL:         return KEY_ID_SCROLL_LOCK;      
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

        TOGL_INNER_CASE_STR(VK_OEM_1); 
        TOGL_INNER_CASE_STR(VK_OEM_2); 
        TOGL_INNER_CASE_STR(VK_OEM_3); 
        TOGL_INNER_CASE_STR(VK_OEM_4); 
        TOGL_INNER_CASE_STR(VK_OEM_5); 
        TOGL_INNER_CASE_STR(VK_OEM_6); 
        TOGL_INNER_CASE_STR(VK_OEM_7); 
        TOGL_INNER_CASE_STR(VK_OEM_COMMA); 
        TOGL_INNER_CASE_STR(VK_OEM_PERIOD); 
        TOGL_INNER_CASE_STR(VK_OEM_MINUS); 
        TOGL_INNER_CASE_STR(VK_OEM_PLUS); 

        TOGL_INNER_CASE_STR(VK_NUMLOCK);       
        TOGL_INNER_CASE_STR(VK_SCROLL);        
        TOGL_INNER_CASE_STR(VK_LSHIFT);        
        TOGL_INNER_CASE_STR(VK_RSHIFT);        
        TOGL_INNER_CASE_STR(VK_LCONTROL);      
        TOGL_INNER_CASE_STR(VK_RCONTROL);      
        TOGL_INNER_CASE_STR(VK_LMENU);         
        TOGL_INNER_CASE_STR(VK_RMENU);   
        }
        if ((vk_code >= '0' && vk_code <= '9') || (vk_code >= 'A' && vk_code <= 'Z')) return std::string(1, (char)vk_code); 
        return std::string() + "(" + std::to_string(vk_code) + ")";
    }

    inline std::string KeyIdToStr(KeyId key_id) {
        switch (key_id) {
        TOGL_INNER_CASE_STR(KEY_ID_UNKNOWN);  
        TOGL_INNER_CASE_STR(KEY_ID_BREAK);                  
        TOGL_INNER_CASE_STR(KEY_ID_BACKSPACE);              
        TOGL_INNER_CASE_STR(KEY_ID_TAB);                    
        TOGL_INNER_CASE_STR(KEY_ID_ENTER);                  
        TOGL_INNER_CASE_STR(KEY_ID_SHIFT);                  
        TOGL_INNER_CASE_STR(KEY_ID_CONTROL);                
        TOGL_INNER_CASE_STR(KEY_ID_ALT);                    
        TOGL_INNER_CASE_STR(KEY_ID_PAUSE);                  
        TOGL_INNER_CASE_STR(KEY_ID_CAPS_LOCK);              
        TOGL_INNER_CASE_STR(KEY_ID_ESCAPE);                 
        TOGL_INNER_CASE_STR(KEY_ID_SPACE);                  
        TOGL_INNER_CASE_STR(KEY_ID_PAGE_UP);                
        TOGL_INNER_CASE_STR(KEY_ID_PAGE_DOWN);              
        TOGL_INNER_CASE_STR(KEY_ID_END);                    
        TOGL_INNER_CASE_STR(KEY_ID_HOME);                   
        TOGL_INNER_CASE_STR(KEY_ID_ARROW_LEFT);             
        TOGL_INNER_CASE_STR(KEY_ID_ARROW_UP);               
        TOGL_INNER_CASE_STR(KEY_ID_ARROW_RIGHT);            
        TOGL_INNER_CASE_STR(KEY_ID_ARROW_DOWN);             
        TOGL_INNER_CASE_STR(KEY_ID_PRINT);                  
        TOGL_INNER_CASE_STR(KEY_ID_PRINT_SCREEN);           
        TOGL_INNER_CASE_STR(KEY_ID_INSERT);                 
        TOGL_INNER_CASE_STR(KEY_ID_DELETE);                 
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_0);               
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_1);               
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_2);               
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_3);               
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_4);               
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_5);               
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_6);               
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_7);               
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_8);               
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_9);               
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_MULTIPLY);        
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_ADD);             
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_SEPARATOR);       
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_SUBTRACT);        
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_DECIMAL);         
        TOGL_INNER_CASE_STR(KEY_ID_NUMPAD_DIVIDE);          
        TOGL_INNER_CASE_STR(KEY_ID_F1);                     
        TOGL_INNER_CASE_STR(KEY_ID_F2);                     
        TOGL_INNER_CASE_STR(KEY_ID_F3);                     
        TOGL_INNER_CASE_STR(KEY_ID_F4);                     
        TOGL_INNER_CASE_STR(KEY_ID_F5);                     
        TOGL_INNER_CASE_STR(KEY_ID_F6);                     
        TOGL_INNER_CASE_STR(KEY_ID_F7);                     
        TOGL_INNER_CASE_STR(KEY_ID_F8);                     
        TOGL_INNER_CASE_STR(KEY_ID_F9);                     
        TOGL_INNER_CASE_STR(KEY_ID_F10);                    
        TOGL_INNER_CASE_STR(KEY_ID_F11);                    
        TOGL_INNER_CASE_STR(KEY_ID_F12);                    
        TOGL_INNER_CASE_STR(KEY_ID_F13);                    
        TOGL_INNER_CASE_STR(KEY_ID_F14);                    
        TOGL_INNER_CASE_STR(KEY_ID_F15);                    
        TOGL_INNER_CASE_STR(KEY_ID_F16);                    
        TOGL_INNER_CASE_STR(KEY_ID_F17);                    
        TOGL_INNER_CASE_STR(KEY_ID_F18);                    
        TOGL_INNER_CASE_STR(KEY_ID_F19);                    
        TOGL_INNER_CASE_STR(KEY_ID_F20);                    
        TOGL_INNER_CASE_STR(KEY_ID_F21);                    
        TOGL_INNER_CASE_STR(KEY_ID_F22);                    
        TOGL_INNER_CASE_STR(KEY_ID_F23);                    
        TOGL_INNER_CASE_STR(KEY_ID_F24);   
        TOGL_INNER_CASE_STR(KEY_ID_0);	
        TOGL_INNER_CASE_STR(KEY_ID_1);	
        TOGL_INNER_CASE_STR(KEY_ID_2);	
        TOGL_INNER_CASE_STR(KEY_ID_3);	
        TOGL_INNER_CASE_STR(KEY_ID_4);	
        TOGL_INNER_CASE_STR(KEY_ID_5);	
        TOGL_INNER_CASE_STR(KEY_ID_6);	
        TOGL_INNER_CASE_STR(KEY_ID_7);	
        TOGL_INNER_CASE_STR(KEY_ID_8);	
        TOGL_INNER_CASE_STR(KEY_ID_9);	
        TOGL_INNER_CASE_STR(KEY_ID_A);	
        TOGL_INNER_CASE_STR(KEY_ID_B);	
        TOGL_INNER_CASE_STR(KEY_ID_C);	
        TOGL_INNER_CASE_STR(KEY_ID_D);	
        TOGL_INNER_CASE_STR(KEY_ID_E);	
        TOGL_INNER_CASE_STR(KEY_ID_F);	
        TOGL_INNER_CASE_STR(KEY_ID_G);	
        TOGL_INNER_CASE_STR(KEY_ID_H);	
        TOGL_INNER_CASE_STR(KEY_ID_I);	
        TOGL_INNER_CASE_STR(KEY_ID_J);	
        TOGL_INNER_CASE_STR(KEY_ID_K);	
        TOGL_INNER_CASE_STR(KEY_ID_L);	
        TOGL_INNER_CASE_STR(KEY_ID_M);	
        TOGL_INNER_CASE_STR(KEY_ID_N);	
        TOGL_INNER_CASE_STR(KEY_ID_O);	
        TOGL_INNER_CASE_STR(KEY_ID_P);	
        TOGL_INNER_CASE_STR(KEY_ID_Q);	
        TOGL_INNER_CASE_STR(KEY_ID_R);	
        TOGL_INNER_CASE_STR(KEY_ID_S);	
        TOGL_INNER_CASE_STR(KEY_ID_T);	
        TOGL_INNER_CASE_STR(KEY_ID_U);	
        TOGL_INNER_CASE_STR(KEY_ID_V);	
        TOGL_INNER_CASE_STR(KEY_ID_W);	
        TOGL_INNER_CASE_STR(KEY_ID_X);	
        TOGL_INNER_CASE_STR(KEY_ID_Y);	
        TOGL_INNER_CASE_STR(KEY_ID_Z);

        TOGL_INNER_CASE_STR(KEY_ID_SEMICOLON);
        TOGL_INNER_CASE_STR(KEY_ID_FORWARD_SLASH);
        TOGL_INNER_CASE_STR(KEY_ID_ACUTE);
        TOGL_INNER_CASE_STR(KEY_ID_OPEN_BRACKET);
        TOGL_INNER_CASE_STR(KEY_ID_BACK_SLASH);
        TOGL_INNER_CASE_STR(KEY_ID_CLOSE_BRACKET);
        TOGL_INNER_CASE_STR(KEY_ID_APOSTROPHE);
        TOGL_INNER_CASE_STR(KEY_ID_COMMA);
        TOGL_INNER_CASE_STR(KEY_ID_DOT);
        TOGL_INNER_CASE_STR(KEY_ID_DASH);
        TOGL_INNER_CASE_STR(KEY_ID_EQUAL);

        TOGL_INNER_CASE_STR(KEY_ID_NUMLOCK);                
        TOGL_INNER_CASE_STR(KEY_ID_SCROLL_LOCK);   
#if 0
        // Experimental. For tests only.
        TOGL_INNER_CASE_STR(KEY_ID_LEFT_SHIFT);             
        TOGL_INNER_CASE_STR(KEY_ID_RIGHT_SHIFT);            
        TOGL_INNER_CASE_STR(KEY_ID_LEFT_CONTROL);           
        TOGL_INNER_CASE_STR(KEY_ID_RIGHT_CONTROL);          
        TOGL_INNER_CASE_STR(KEY_ID_LEFT_ALT);               
        TOGL_INNER_CASE_STR(KEY_ID_RIGHT_ALT);   
#endif
        }
        return std::string() + "(" + std::to_string(key_id) + ")";
    }


#undef TOGL_INNER_CASE_STR

    inline std::string WinApiKeyDataToStr(WPARAM w_param, LPARAM l_param) {
        const VirtualKeyData& virtual_key_data = *((const VirtualKeyData*)(&l_param));

        auto PadWithSpaces = [](const std::string& text, uint32_t num_of_spaces) {
            if (text.length() > 0 && text.length() < num_of_spaces) return text + std::string(num_of_spaces - text.length(), ' ');
            return text;

        };

        enum { MAX_LENGTH_OF_NAME = 16 };

        std::string text = std::string()
            + " vk_code="          + PadWithSpaces(VK_CodeToStr((int)w_param), MAX_LENGTH_OF_NAME)
            + " key_id="           + PadWithSpaces(KeyIdToStr(VK_CodeToKeyId((int)w_param, virtual_key_data)), MAX_LENGTH_OF_NAME)
            + ", count="           + std::to_string(virtual_key_data.count)
            + ", scan_code="       + std::to_string(virtual_key_data.scan_code)
            + ", is_ext="          + std::to_string(virtual_key_data.is_ext)
            + ", reserved1="       + std::to_string(virtual_key_data.reserved1)
            + ", context_code="    + std::to_string(virtual_key_data.context_code)
            + ", prev_state="      + std::to_string(virtual_key_data.prev_state)
            + ", trans_state="     + std::to_string(virtual_key_data.trans_state);

        return text;
    }

} // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_KEY_H_

