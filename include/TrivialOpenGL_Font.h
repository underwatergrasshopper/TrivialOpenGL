/**
* @file TrivialOpenGL_Font.h
* @author underwatergrasshopper
*/

#ifndef TRIVIALOPENGL_FONT_H_
#define TRIVIALOPENGL_FONT_H_

#include "TrivialOpenGL_Window.h"

namespace TrivialOpenGL {
	enum FontSizeUnit {
		FONT_SIZE_UNIT_PIXELS,
		FONT_SIZE_UNIT_POINTS,
	};

    //enum FontStyle {
    //    FONT_STYLE_NORMAL,
    //    FONT_STYLE_BOLD,
    //};
    // 
    //// Ranges are from unicode space.
    //enum FontCharSet {
    //    FONT_CHAR_SET_ENGLISH,
    //
    //    // Note: Font might not have all glyphs from this range.
    //    FONT_CHAR_SET_RANGE_0000_FFFF,
    //};

	struct FontInfo {
		std::string		name;
		uint32_t		size;
		FontSizeUnit	size_unit;
		FontStyle		style;
		FontCharSet		char_set;
	};

	class Font {
	public:
		Font() {
			Initialize();
		}
		virtual ~Font() {
            Unload();
		}

		void Load(const std::string& name, uint32_t size, FontSizeUnit size_unit, FontStyle style, FontCharSet char_set) {
			Unload();

			m_info = {name, size, size_unit, style, char_set};

			WindowInnerAccessor& window_inner_accessor = ToWindow();
			HWND window_handle = window_inner_accessor.ToHWND();

			m_device_context_handle = GetDC(window_handle);

			m_glyph_height = ToPixels(size, size_unit);

			HFONT font_handle = CreateFontW(
                    m_glyph_height,                    
                    0, 0, 0,                            
                    (style == FONT_STYLE_BOLD) ? FW_BOLD : FW_NORMAL,
                    FALSE, FALSE, FALSE,
                    SolveCreateFontCharSet(char_set),
                    OUT_TT_PRECIS,
                    CLIP_DEFAULT_PRECIS,
                    ANTIALIASED_QUALITY,
                    FF_DONTCARE | DEFAULT_PITCH,
                    ToUTF16(name).c_str());  

			if (font_handle == NULL) {
				AddErrMsg("Can not create font source.");
			} else {
                HFONT old_font_handle = (HFONT)SelectObject(m_device_context_handle, font_handle); 

                // --- Gets Font Metrics --- //
                TEXTMETRICW metric;
                GetTextMetricsW(m_device_context_handle, &metric);
                m_height            = metric.tmHeight;
                m_ascent            = metric.tmAscent;
                m_descent           = metric.tmDescent;
                m_internal_leading  = metric.tmInternalLeading;

                // --- Gets Code Ranges --- //

                DWORD buffer_size = GetFontUnicodeRanges(m_device_context_handle, NULL);
                BYTE* buffer = new BYTE[buffer_size];

                GLYPHSET* glyphset = (GLYPHSET*)buffer;
                GetFontUnicodeRanges(m_device_context_handle, glyphset);
                // debug
                //togl_print_i32(glyphset->cbThis);
                //togl_print_i32(glyphset->flAccel);
                //togl_print_i32(glyphset->cGlyphsSupported);
                //togl_print_i32(glyphset->cRanges);

                switch (char_set) {
                case FONT_CHAR_SET_ENGLISH:
                    m_code_ranges.push_back({0x0020, 0x007E});  
                    //m_code_ranges.push_back({0x25A1, 0x25A1});  // Unknown Character Indicator #1: WHITE SQUARE
                    //m_code_ranges.push_back({0xFFFD, 0xFFFD});  // Unknown Character Indicator #2: REPLACEMENT CHARACTER
                    break;
                case FONT_CHAR_SET_RANGE_0000_FFFF:
                    m_code_ranges.push_back({0x0000, 0xFFFF});
                    break;
                default:
                    for (uint32_t ix = 0; ix < glyphset->cRanges; ++ix) {
                        const uint32_t from = glyphset->ranges[ix].wcLow;
                        const uint32_t to   = from + glyphset->ranges[ix].cGlyphs - 1;
                        m_code_ranges.push_back({from, to});
                    }
                    break;
                }

                for (const auto& code_range : m_code_ranges) printf("[%04X..%04X]\n", code_range.from, code_range.to); // debug

                delete[] buffer;

                // --- Generates Inner Font Bitmap  --- //

                GLsizei list_range	= GetListRange(char_set);
                GLint	list_base	= glGenLists(list_range);
                if (list_base == 0) {
                    AddErrMsg("Error: Can not generate display list.");
                } else {
                    bool is_success = wglUseFontBitmapsW(m_device_context_handle, 0, list_range, list_base);

                    // Workaround for strange behavior. For POPUP window first call of wglUseFontBitmapsA fail with GetError() = 0.
                    // Second call, right after first, seams to succeed.
                    if (!is_success) is_success = wglUseFontBitmapsA(m_device_context_handle, 0, list_range, list_base);

                    if (!is_success) {
                        AddErrMsg("Error: Can not font bitmap.");
                    }

					if (IsOk()) {
						GenerateFontTextures(list_base, list_range, 1024, 1024);
					}

					glDeleteLists(list_base, list_range);
                }

                // ---

                SelectObject(m_device_context_handle, old_font_handle);

				DeleteObject(font_handle);
			}

			ReleaseDC(window_handle, m_device_context_handle);
            m_device_context_handle = NULL;
		} 

		void Unload() {
			m_code_ranges.clear();
            m_font_data.clear();

            for (auto& tex_obj : m_tex_objs) {
                glDeleteTextures(1, &tex_obj);
            }
            m_tex_objs.clear();

            m_err_msg	= "";

			Initialize();
		}
		bool IsLoaded() const {
			return m_is_loaded;
		}

        void RenderBegin() {
            glPushAttrib(GL_TEXTURE_BIT);
            glPushAttrib(GL_ENABLE_BIT);
            glPushAttrib(GL_COLOR_BUFFER_BIT);
            glPushAttrib(GL_LIST_BIT);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
        }

        void RenderEnd() {
            glPopAttrib();
            glPopAttrib();
            glPopAttrib();
            glPopAttrib();
        }

        void RenderGlyph(uint32_t code) {
            auto RenderRectangle = [](int width, int height) {
                glDisable(GL_TEXTURE_2D);

                glBegin(GL_TRIANGLE_FAN);
                glVertex2i(0, 0);
                glVertex2i(width, 0);
                glVertex2i(width, height);
                glVertex2i(0, height);
                glEnd();
            };

            auto it = m_font_data.find(code);

            if (it == m_font_data.end()) it = m_font_data.find(L'\u25A1'); // unknown character = WHITE SQUARE
            if (it == m_font_data.end()) it = m_font_data.find(L'\uFFFD'); // unknown character = REPLACEMENT CHARACTER

            if (it != m_font_data.end()) {
                const GlyphData& glyph_data = it->second;

                if (glyph_data.tex_obj != 0) {
                    glBindTexture(GL_TEXTURE_2D, glyph_data.tex_obj);
                    glEnable(GL_TEXTURE_2D);

                    glBegin(GL_TRIANGLE_FAN);
                    glTexCoord2d(glyph_data.x1, glyph_data.y1);
                    glVertex2i(0, 0);

                    glTexCoord2d(glyph_data.x2, glyph_data.y1);
                    glVertex2i(glyph_data.width, 0);

                    glTexCoord2d(glyph_data.x2, glyph_data.y2);
                    glVertex2i(glyph_data.width, m_glyph_height);
                
                    glTexCoord2d(glyph_data.x1, glyph_data.y2);
                    glVertex2i(0, m_glyph_height);
                    glEnd();
                } else {
                    // Replacement for Unknown Character 
                    RenderRectangle(glyph_data.width, m_glyph_height);
                }
            } else {
                // Replacement for Unknown Character 
                // Unknown character needs to have width for other characters to be properly spaced when drawn.
                GlyphData& glyph_data = m_font_data[code];
                glyph_data.tex_obj = 0;
                glyph_data.width = m_glyph_height;

                RenderRectangle(glyph_data.width, m_glyph_height);
            }
        } 

        // Renders array of glyphs. 
        // Special characters (like '\n', '\t', ... and so on) are interpreted as "unknown characters".
        // text         - Encoding format: UTF8.
        void RenderGlyphs(const std::string& text) {
            const std::wstring text_utf16 = ToUTF16(text);

            RenderBegin();
            int x = 0;
            for (const uint32_t& code : text_utf16) {
                glPushMatrix();
                glTranslatef(float(x), 0, 0);
                RenderGlyph(code);
                x += GetGlyphSize(code).width;
                glPopMatrix();
            }
            RenderEnd();
        }

        SizeU GetGlyphSize(uint32_t code) const {
            auto it = m_font_data.find(code);

            if (it == m_font_data.end()) it = m_font_data.find(L'\u25A1'); // unknown character = WHITE SQUARE
            if (it == m_font_data.end()) it = m_font_data.find(L'\uFFFD'); // unknown character = REPLACEMENT CHARACTER

            if (it != m_font_data.end()) {
                return {it->second.width, m_glyph_height};
            }
            return {};
        }

		bool IsOk() const {
			return m_err_msg.empty();
		}

		std::string GetErrMsg() const {
			return m_err_msg;
		}

        void SaveAsBMP(const std::string& path) {
            std::string file_name_prefix = path;

            if (file_name_prefix.length() > 0 && file_name_prefix[file_name_prefix.length() - 1] != '/' && file_name_prefix[file_name_prefix.length() - 1] != '\\') {
                file_name_prefix += "/";
            }

            file_name_prefix += m_info.name;

            for (size_t ix = 0; ix < m_tex_objs.size(); ++ix) {
                std::string file_name = file_name_prefix;
                if (ix > 0) file_name += " (" + std::to_string(ix + 1) + ")";
                file_name += ".bmp";
                SaveTextureAsBMP(file_name, m_tex_objs[ix]);
            }
        }

	private:
		TOGL_NO_COPY(Font);

		struct CodeRange {
            uint32_t from;
            uint32_t to;
        };

        struct GlyphData {
            int         x;
            int         y;
            uint16_t    width;
            GLuint      tex_obj;

            double      x1;
            double      y1;
            double      x2;
            double      y2;
        };

        class FrameBuffer {
        public:
            FrameBuffer(uint16_t width, uint16_t height) {
                Load(m_glGenFramebuffersEXT, "glGenFramebuffersEXT");
			    Load(m_glDeleteFramebuffersEXT, "glDeleteFramebuffersEXT");
			    Load(m_glBindFramebufferEXT, "glBindFramebufferEXT");
			    Load(m_glFramebufferTexture2DEXT, "glFramebufferTexture2DEXT");
			    Load(m_glCheckFramebufferStatusEXT, "glCheckFramebufferStatusEXT");
                    
                m_width     = width;
                m_height    = height;

			    if (IsOk()) {
				    GLint max_viewport_size[2] = {};
                    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, max_viewport_size);
                    GLint max_texture_size = 0;
                    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

                    if (width > max_viewport_size[0] || width > max_texture_size || height > max_viewport_size[1] || height > max_texture_size) {
                        AddErrMsg(std::string() + "Value of width or/and height are to big. ("
                            "max_viewport_width=" + std::to_string(max_viewport_size[0]) + 
                            ", max_viewport_height=" + std::to_string(max_viewport_size[1]) + 
                            ", max_texture_width=" + std::to_string(max_texture_size) + 
                            ", max_texture_height=" + std::to_string(max_texture_size) + 
                            ")"
                        );
                    }
			    }

                m_fbo       = 0;
                m_prev_fbo  = 0;

			    if (IsOk()) {
                    m_glGenFramebuffersEXT(1, &m_fbo);

                    glGetIntegerv(TOGL_GL_FRAMEBUFFER_BINDING, (GLint*)&m_prev_fbo);
                    m_glBindFramebufferEXT(TOGL_GL_FRAMEBUFFER_EXT, m_fbo);
                }

                glPushAttrib(GL_ENABLE_BIT);
                glPushAttrib(GL_TEXTURE_BIT);
            }

            virtual ~FrameBuffer() {
                if (IsOk()) m_glFramebufferTexture2DEXT(TOGL_GL_FRAMEBUFFER_EXT, TOGL_GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 0, 0);

                glPopAttrib();
                glPopAttrib();

                if (IsOk()) {
                    m_glBindFramebufferEXT(TOGL_GL_FRAMEBUFFER_EXT, m_prev_fbo);
                    m_glDeleteFramebuffersEXT(1, &m_fbo);
                }
            }

            GLuint GenAndBindTex() {
                GLuint tex_obj = 0;

			    if (IsOk()) {
                    glGenTextures(1, &tex_obj);
                    glBindTexture(GL_TEXTURE_2D, tex_obj);
                    glDisable(GL_TEXTURE_2D);

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

                    m_glFramebufferTexture2DEXT(TOGL_GL_FRAMEBUFFER_EXT, TOGL_GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex_obj, 0);
                    
                    if (m_glCheckFramebufferStatusEXT(TOGL_GL_FRAMEBUFFER_EXT) != TOGL_GL_FRAMEBUFFER_COMPLETE_EXT) {
                        AddErrMsg("Frame buffer is not complete.");

                        m_glFramebufferTexture2DEXT(TOGL_GL_FRAMEBUFFER_EXT, TOGL_GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 0, 0);
                        glDeleteTextures(1, &tex_obj);
                        tex_obj = 0;
                    } 
                }

                return tex_obj;
            }

            bool IsOk() const {
                return m_err_msg.empty();
            }

            std::string GetErrMsg() const {
                return m_err_msg;
            }
        private:
            enum {
                TOGL_GL_FRAMEBUFFER_EXT             = 0x8D40,
                TOGL_GL_FRAMEBUFFER_BINDING         = 0x8CA6,
                TOGL_GL_COLOR_ATTACHMENT0_EXT       = 0x8CE0,
                TOGL_GL_FRAMEBUFFER_COMPLETE_EXT    = 0x8CD5,
            };

            void AddErrMsg(const std::string& err_msg) {
			    if (!m_err_msg.empty()) m_err_msg += "\n";
			    m_err_msg += "Font FrameBuffer Error: ";
			    m_err_msg += err_msg;
		    }

	        template <typename Type>
            void Load(Type& function, const std::string& function_name) {
                function = (Type)wglGetProcAddress(function_name.c_str());
                if (!function) {
                    AddErrMsg(std::string() + "Can not load function: \"" + function_name + "\".");
                }
            }

			void (APIENTRY *m_glGenFramebuffersEXT)(GLsizei n, GLuint *framebuffers);
            void (APIENTRY *m_glDeleteFramebuffersEXT)(GLsizei n, const GLuint *framebuffers);
            void (APIENTRY *m_glBindFramebufferEXT)(GLenum target, GLuint framebuffer);
            void (APIENTRY *m_glFramebufferTexture2DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
            GLenum (APIENTRY* m_glCheckFramebufferStatusEXT)(GLenum target);

            uint16_t    m_width;
            uint16_t    m_height;

            GLuint      m_fbo;
            GLuint      m_prev_fbo;

            std::string m_err_msg;
        };

		void Initialize() {
            m_device_context_handle = NULL;
			m_info		= {};

			m_glyph_height = 0;

			m_height		= 0;
			m_ascent		= 0;
			m_descent		= 0;
			m_internal_leading = 0;

			m_is_loaded = false;
		}

		void GenerateFontTextures(GLint list_base, GLsizei list_range, uint16_t width, uint16_t height) {
            FrameBuffer frame_buffer(width, height);
            GLuint tex_obj = frame_buffer.GenAndBindTex();

            if (frame_buffer.IsOk()) {
                m_tex_objs.push_back(tex_obj);

                glPushAttrib(GL_VIEWPORT_BIT);
                glViewport(0, 0, width, height);

                glPushAttrib(GL_MATRIX_MODE);
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                glOrtho(0, width, 0, height, 1, -1);
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();

                glPushAttrib(GL_COLOR_BUFFER_BIT);
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

                glClear(GL_COLOR_BUFFER_BIT);

                int y = int(height) - int(m_glyph_height);
                PointI pos = {0, y};
 
                for (const CodeRange& code_range : m_code_ranges) {
                    for (uint32_t code = code_range.from; code <= code_range.to; ++code) {

                        const SizeU16 size = GetCharSize((wchar_t)code);

                        if ((pos.x + size.width) >= width) {
                            pos.x = 0;

                            if ((pos.y - m_glyph_height) <= 0) {
                                // run out of space, generate next texture
                                tex_obj = frame_buffer.GenAndBindTex();

                                if (!frame_buffer.IsOk()) {
                                    MergErrMsg(frame_buffer.GetErrMsg());
                                    break;
                                }
                                glClear(GL_COLOR_BUFFER_BIT);

                                m_tex_objs.push_back(tex_obj);

                                //printf("T%d [%04X]\n", (int)m_tex_objs.size() - 1, code); // debug

                                pos = {0, y};
                            } else {
                                pos.y -= m_glyph_height;
                            }
                        }

                        auto ToTexSpace = [](uint16_t pos, uint16_t size) -> double {
                            return pos / double(size);
                        };

                        GlyphData glyph_data = {};

                        glyph_data.x = pos.x;
                        glyph_data.y = pos.y;
                        glyph_data.width = size.width;
                        glyph_data.tex_obj = tex_obj;

                        glyph_data.x1 = ToTexSpace(pos.x, width);
                        glyph_data.y1 = ToTexSpace(pos.y - m_descent, height);
                        glyph_data.x2 = ToTexSpace(pos.x + size.width, width);
                        glyph_data.y2 = ToTexSpace(pos.y - m_descent + m_glyph_height, height);

                        m_font_data[code] = glyph_data;
              
                        RenderGlyphToTexture(list_base, pos.x, pos.y, (wchar_t)code);
                    
                        pos.x += size.width;
                    }
                }
  
                m_is_loaded = true;

                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glPopMatrix();
                    
                glPopAttrib();
                glPopAttrib();
                glPopAttrib();
            }
		}

        void RenderGlyphToTexture(GLint list_base, int x, int y, wchar_t c) {
            glPushAttrib(GL_ENABLE_BIT);
            glPushAttrib(GL_COLOR_BUFFER_BIT);
            glPushAttrib(GL_LIST_BIT);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
                
            glColor4ub(255, 255, 255, 255);
            glRasterPos2i(x, y);

            glListBase(list_base);
            glCallLists(1, GL_UNSIGNED_SHORT, &c);

            glPopAttrib();
            glPopAttrib();
            glPopAttrib();
        }

        SizeU16 GetCharSize(wchar_t c) const {
            SIZE size;
            if (GetTextExtentPoint32W(m_device_context_handle, &c, 1, &size)) {
                return SizeU16(size.cx, size.cy);
            }
            return {};
        }

		void AddErrMsg(const std::string& err_msg) {
			if (!m_err_msg.empty()) m_err_msg += "\n";
			m_err_msg += "Font Error: ";
			m_err_msg += err_msg;
		}

		void MergErrMsg(const std::string& err_msg) {
			if (!m_err_msg.empty()) m_err_msg += "\n";
			m_err_msg += err_msg;
		}

		static uint32_t PointsToPixels(uint32_t points) {
			return points * 4 / 3;
		}

		static uint32_t ToPixels(uint32_t size, FontSizeUnit size_unit) {
			switch (size_unit) {
			case FONT_SIZE_UNIT_PIXELS: return size;
			case FONT_SIZE_UNIT_POINTS: return PointsToPixels(size);
			}
            return 0;
		}

		static DWORD SolveCreateFontCharSet(FontCharSet char_set) {
			switch (char_set) {
			case FONT_CHAR_SET_ENGLISH:				return ANSI_CHARSET;
			case FONT_CHAR_SET_RANGE_0000_FFFF:	return ANSI_CHARSET;
			}
            return 0;
		}

		static GLsizei GetListRange(FontCharSet char_set) {
            switch (char_set) {
            case FONT_CHAR_SET_ENGLISH:             return 128;     // 0x80
            case FONT_CHAR_SET_RANGE_0000_FFFF:   return 65535;   // 0xFFFF
            }
            return 0;
        }

		template <typename Type>
        void Load(Type& function, const std::string& function_name) {
            function = (Type)wglGetProcAddress(function_name.c_str());
            if (!function) {
                AddErrMsg(std::string() + "Can not load function: \"" + function_name + "\".");
            }
        }

        HDC                     m_device_context_handle;
		FontInfo				m_info;
		uint32_t				m_glyph_height;         // in pixels

        uint32_t				m_height;               // in pixels
        uint32_t				m_ascent;               // in pixels
        uint32_t				m_descent;              // in pixels
        uint32_t				m_internal_leading;     // in pixels

        std::vector<CodeRange>	m_code_ranges;

        std::map<uint32_t, GlyphData>  m_font_data;
        std::vector<GLuint>     m_tex_objs;

		bool			        m_is_loaded;
		std::string		        m_err_msg;
	};

}; // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_FONT_H_
