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
	//	FONT_STYLE_NORMAL,
	//	FONT_STYLE_BOLD,
	//};
    //
	//enum FontCharSet {
	//	FONT_CHAR_SET_ASCII,
    //
    //    // Note: Font might not have all glyphs from this range.
    //    FONT_CHAR_SET_UNICODE_0000_FFFF,
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

                for (uint32_t ix = 0; ix < glyphset->cRanges; ++ix) {
                    const uint32_t from = glyphset->ranges[ix].wcLow;
                    const uint32_t to = from + glyphset->ranges[ix].cGlyphs - 1;
                    m_code_ranges.push_back({from, to});

                    //printf("[%04X..%04X]\n", from, to); // debug
                }
                m_code_ranges.push_back({0xFFFD, 0xFFFD});
                

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

        void RenderGlyph(uint32_t code) {
            if (code < m_font_data.size()) {
                GlyphData& glyph_data = m_font_data[code];

                if (glyph_data.tex_obj != 0) {
                    glPushAttrib(GL_TEXTURE_BIT);
                    glPushAttrib(GL_ENABLE_BIT);
                    glPushAttrib(GL_COLOR_BUFFER_BIT);
                    glPushAttrib(GL_LIST_BIT);

                    glBindTexture(GL_TEXTURE_2D, glyph_data.tex_obj);
                    glEnable(GL_TEXTURE_2D);

                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

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

                    glPopAttrib();
                    glPopAttrib();
                    glPopAttrib();
                    glPopAttrib();
                } else {
                    // TODO: render default glyph
                }
            } else {
                // TODO: render default glyph
            }
        }

        SizeU GetGlyphSize(uint32_t code) const {
            if (code < m_font_data.size()) {
                return {m_font_data[code].width, m_glyph_height};
            }
            return {};
        }

		bool IsOk() const {
			return m_err_msg.empty();
		}

		std::string GetErrMsg() const {
			return m_err_msg;
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
            enum {
                TOGL_GL_FRAMEBUFFER_EXT             = 0x8D40,
                TOGL_GL_FRAMEBUFFER_BINDING         = 0x8CA6,
                TOGL_GL_COLOR_ATTACHMENT0_EXT       = 0x8CE0,
                TOGL_GL_FRAMEBUFFER_COMPLETE_EXT    = 0x8CD5,
            };

			void (APIENTRY *togl_glGenFramebuffersEXT)(GLsizei n, GLuint *framebuffers) = nullptr;
            void (APIENTRY *togl_glDeleteFramebuffersEXT)(GLsizei n, const GLuint *framebuffers) = nullptr;
            void (APIENTRY *togl_glBindFramebufferEXT)(GLenum target, GLuint framebuffer) = nullptr;
            void (APIENTRY *togl_glFramebufferTexture2DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) = nullptr;
            GLenum (APIENTRY* togl_glCheckFramebufferStatusEXT)(GLenum target) = nullptr;

			Load(togl_glGenFramebuffersEXT, "glGenFramebuffersEXT");
			Load(togl_glDeleteFramebuffersEXT, "glDeleteFramebuffersEXT");
			Load(togl_glBindFramebufferEXT, "glBindFramebufferEXT");
			Load(togl_glFramebufferTexture2DEXT, "glFramebufferTexture2DEXT");
			Load(togl_glCheckFramebufferStatusEXT, "glCheckFramebufferStatusEXT");

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

			if (IsOk()) {
                GLuint fbo      = 0;
                GLuint prev_fbo = 0;
                GLuint tex_obj  = 0;

                togl_glGenFramebuffersEXT(1, &fbo);
                glGetIntegerv(TOGL_GL_FRAMEBUFFER_BINDING, (GLint*)&prev_fbo);
                togl_glBindFramebufferEXT(TOGL_GL_FRAMEBUFFER_EXT, fbo);

                glPushAttrib(GL_ENABLE_BIT);

                glGenTextures(1, &tex_obj);
                glBindTexture(GL_TEXTURE_2D, tex_obj);
                glDisable(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

                togl_glFramebufferTexture2DEXT(TOGL_GL_FRAMEBUFFER_EXT, TOGL_GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex_obj, 0);
                    
                if (togl_glCheckFramebufferStatusEXT(TOGL_GL_FRAMEBUFFER_EXT) != TOGL_GL_FRAMEBUFFER_COMPLETE_EXT) {
                    AddErrMsg("Frame buffer is not complete.");
                } else {
                    FillTexture(list_base, list_range, width, height, tex_obj);
                }
                
                togl_glFramebufferTexture2DEXT(TOGL_GL_FRAMEBUFFER_EXT, TOGL_GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 0, 0);

                glPopAttrib();

                togl_glBindFramebufferEXT(TOGL_GL_FRAMEBUFFER_EXT, prev_fbo);
                togl_glDeleteFramebuffersEXT(1, &fbo);

                //InnerUtility::SaveTextureAsBMP(m_info.name + ".bmp", tex_obj); // debug
            }
		}

        void FillTexture(GLint list_base, GLsizei list_range, uint16_t width, uint16_t height, GLuint tex_obj) {
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

            m_font_data.resize(list_range, {});

            auto SolveAndRenderGlyphToTexture = [&](uint32_t from, uint32_t to) {
                for (uint32_t code = from; code <= to; ++code) {

                    const SizeU16 size = GetCharSize((wchar_t)code);

                    if ((pos.x + size.width) >= width) {
                        pos.x = 0;

                        if ((pos.y - m_glyph_height) <= 0) {
                            break;
                        }
                        
                        pos.y -= m_glyph_height;
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
            };

            const bool is_full_range = false;
            if (is_full_range) {
                SolveAndRenderGlyphToTexture(0, uint32_t(list_range));
            } else {
                for (const CodeRange& code_range : m_code_ranges) {
                    SolveAndRenderGlyphToTexture(code_range.from, code_range.to);
                }
            }

            m_tex_objs.push_back(tex_obj);

            m_is_loaded = true;

            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
                    
            glPopAttrib();
            glPopAttrib();
            glPopAttrib();
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
			case FONT_CHAR_SET_ASCII:				return ANSI_CHARSET;
			case FONT_CHAR_SET_UNICODE_0000_FFFF:	return ANSI_CHARSET;
			}
            return 0;
		}

		static GLsizei GetListRange(FontCharSet char_set) {
            switch (char_set) {
            case FONT_CHAR_SET_ASCII:               return 128;     // 0x80
            case FONT_CHAR_SET_UNICODE_0000_FFFF:   return 65535;   // 0xFFFF
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

        std::vector<GlyphData>  m_font_data;
        std::vector<GLuint>     m_tex_objs;

		bool			        m_is_loaded;
		std::string		        m_err_msg;
	};

}; // namespace TrivialOpenGL

#endif // TRIVIALOPENGL_FONT_H_
