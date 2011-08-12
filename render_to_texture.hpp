//
//  render_to_texture.hpp
//  gl_util_test
//

#ifndef gl_util_test_render_to_texture_hpp
#define gl_util_test_render_to_texture_hpp

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

class RenderToTexture {
	int tex_width;
	int tex_height;
	GLuint tex_name;
	GLuint depth_tex_name;
	GLuint frame_buff_name;
	
	void gl_error () {
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			std::cerr << "ERROR: " << gluErrorString(err) << std::endl;
		}
	}
	
public:
	RenderToTexture (int w, int h) : tex_width(w), tex_height(h),
	tex_name(0), depth_tex_name(0), frame_buff_name(0)
	{ }
	
	int width () { return tex_width; }
	int height () { return tex_height; }
	
	void initFBO () {
		glGenTextures(1, &tex_name);
		glBindTexture(GL_TEXTURE_2D, tex_name);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		gl_error();
		glGenTextures(1, &depth_tex_name);
		glBindTexture(GL_TEXTURE_2D, depth_tex_name);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, tex_width, tex_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		gl_error();
		glGenFramebuffers(1, &frame_buff_name);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buff_name);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_name, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex_name, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		gl_error();
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "error" << std::endl;
		}
		gl_error();
	}
	
	void start () {
		if (frame_buff_name == 0) {
			std::cerr << "not initialized." << std::endl;
			return;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buff_name);
		glViewport(0, 0, tex_width, tex_height);
	}
	void end () {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void draw_as_texture (double minX, double minY, double maxX, double maxY) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_name);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0);	glVertex2d(minX, minY);
		glTexCoord2d(1.0, 0.0);	glVertex2d(maxX, minY);
		glTexCoord2d(1.0, 1.0);	glVertex2d(maxX, maxY);
		glTexCoord2d(0.0, 1.0);	glVertex2d(minX, maxY);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};
class RenderToTexture_drawing {
	RenderToTexture& ref_r2t;
public:
	RenderToTexture_drawing (RenderToTexture& r2t) : ref_r2t(r2t)
	{
		ref_r2t.start();
	}
	~RenderToTexture_drawing () {
		ref_r2t.end();
	}
};


#endif
