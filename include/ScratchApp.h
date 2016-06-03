#ifndef __SDFEDIT_SCRATCH_APP_H__
#define __SDFEDIT_SCRATCH_APP_H__

#include <BaseApp.h>
#include <Primitives.h>
#include <Camera.h>

namespace sdfedit
{
	class ScratchApp : public BaseApp
	{
	public:
		virtual void settings();
		virtual void setup();
		virtual void update(double dt);
		virtual void teardown();

		virtual void onResize(int width, int height);
		virtual void onKeyboarEvent(int key, int scancode, int action, int mods);
	
	private:
		GLsizei			m_screenTexWidth;
		GLsizei			m_screenTexHeight;
		GLuint			m_screenTex;
		unsigned char*  m_screenBuffer;
		float			m_clearColor[4];
		GLuint			m_fullscreenTexQuadProgram;
		GLuint			m_fullscreenQuadVao;

		Camera::Perspective m_cam;
		SDFScene m_scene;

	};
}

#endif //__SDFEDIT_SCRATCH_APP_H__