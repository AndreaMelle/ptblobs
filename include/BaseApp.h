#ifndef __SDFEDIT_BASAPP_H__
#define __SDFEDIT_BASAPP_H__

#include <Common.h>

namespace sdfedit
{
	template<class App>
	int RunApp(int argc, char** argv)
	{
		int result = EXIT_SUCCESS;

		try
		{
			App::_platform_init();
		}
		catch (std::exception ex)
		{
			std::cerr << ex.what() << "\n";
			std::cerr << "Cannot initialize platform.\n";
			return EXIT_FAILURE;
		}

		App::g_app = new App();

		try
		{
			App::g_app->_private_settings();
			App::g_app->_private_setup();
		}
		catch (std::exception ex)
		{
			std::cerr << ex.what() << "\n";
			std::cerr << "Cannot initialize app.\n";
			result = EXIT_FAILURE;
		}

		while (!App::g_app->_exit)
		{
			App::g_app->_private_update();
		}

		App::g_app->_private_teardown();

		delete App::g_app;
		App::g_app = nullptr;
		return result;
	}

	class BaseApp
	{
	public:
		template<class App>
		friend int RunApp(int argc, char** argv);
		
		BaseApp();
		virtual ~BaseApp();

		virtual void settings() {}
		virtual void setup() = 0;
		virtual void update(double dt) = 0;
		virtual void teardown() {}

		virtual void onResize(int width, int height) {}
		virtual void onKeyboarEvent(int key, int scancode, int action, int mods) {}

		static BaseApp* Get() { return g_app; }
		int getFramebufferWidth() { return m_framebufferWidth; }
		int getFramebufferHeight() { return m_framebufferHeight; }

	protected:
		void setWindowSize(int width, int height)
		{
			// At the moment we only allow to set size before context creation
			assert(m_window == nullptr);
			m_windowWidth = width;
			m_windowHeight = height;
		}

	private:

		static void _platform_init();
		static void _error_callback(int error, const char* description);
		static void _key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void _framebuffer_resize_callback(GLFWwindow* window, int width, int height);

		/*
		 * Gives app a chance to set default settings before allowing customization via public method override
		 */
		void _private_settings();
		void _private_setup();
		void _private_update();
		void _private_teardown();

	private:
		static BaseApp* g_app;

		static const int kDefaultWindowWidth = 800;
		static const int kDefaultWindowHeight = 600;

		GLFWwindow* m_window;
		int			m_windowWidth;
		int			m_windowHeight;
		int			m_framebufferWidth;
		int			m_framebufferHeight;
		std::string m_windowTitle;

		bool		_exit;
	};
}

#endif //__SDFEDIT_BASAPP_H__