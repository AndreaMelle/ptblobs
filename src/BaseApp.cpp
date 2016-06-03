#include <BaseApp.h>

using namespace sdfedit;

BaseApp* BaseApp::g_app = nullptr;

BaseApp::BaseApp()
	: m_windowWidth(kDefaultWindowWidth)
	, m_windowHeight(kDefaultWindowHeight)
	, m_framebufferWidth(0)
	, m_framebufferHeight(0)
	, m_window(nullptr)
	, m_windowTitle("GLFW Basic App")
	, _exit(false)
{

}

BaseApp::~BaseApp()
{
	m_window = nullptr;
}

void BaseApp::_platform_init()
{
	if (!glfwInit())
	{
		throw new std::exception("Platform init exception");
	}
}

void BaseApp::_private_settings()
{
	glfwSetErrorCallback(BaseApp::_error_callback);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->settings();
}

void BaseApp::_private_setup()
{
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_windowTitle.c_str(), NULL, NULL);

	if (m_window == nullptr)
	{
		this->_exit = true;
		glfwTerminate();
		throw new std::exception("App could not create window and OpenGL context");
	}

	glfwMakeContextCurrent(m_window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	glfwSetKeyCallback(m_window, BaseApp::_key_callback);
	glfwSetFramebufferSizeCallback(m_window, BaseApp::_framebuffer_resize_callback);

	glfwGetFramebufferSize(m_window, &this->m_framebufferWidth, &this->m_framebufferHeight);

	this->setup();
}

void BaseApp::_private_update()
{
	double t = glfwGetTime();
	
	glfwPollEvents();
	
	//TODO: compute delta

	this->update(0);

	glfwSwapBuffers(m_window);

	_exit = glfwWindowShouldClose(m_window) == GL_TRUE ? true : false;
}

void BaseApp::_private_teardown()
{
	this->teardown();

	if (m_window != nullptr)
	{
		glfwDestroyWindow(m_window);
	}

	glfwTerminate();
}

void BaseApp::_error_callback(int error, const char* description)
{
	std::cerr << description << "\n";
}

void BaseApp::_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else
	{
		g_app->onKeyboarEvent(key, scancode, action, mods);
	}
}

void BaseApp::_framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
	if (window == g_app->m_window)
	{
		g_app->m_framebufferWidth = width;
		g_app->m_framebufferHeight = height;
		g_app->onResize(width, height);
	}
}