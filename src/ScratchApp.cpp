#include <ScratchApp.h>
#include <Shader.h>

using namespace sdfedit;

void ScratchApp::settings()
{
	//setWindowSize(1600, 1200);
}

void ScratchApp::setup()
{
	/*
	 * We need a full screen quad VAO and a pass through shader
	 * No we don't, magic modern opengl, just use the damn indices :)
	 * Works only for core profile >= 330
	 * http://renderingpipeline.com/2012/03/attribute-less-rendering/
	 * We only need a shader
	 * TODO: move to factory
	 */
	glGenVertexArrays(1, &m_fullscreenQuadVao);
	glBindVertexArray(m_fullscreenQuadVao);
	glBindVertexArray(0);

	GLuint shaders[2];
	shaders[0] = Shader::FromFile("../../Shaders/fullscreen_quad.vert", GL_VERTEX_SHADER);
	shaders[1] = Shader::FromFile("../../Shaders/fullscreen_quad.frag", GL_FRAGMENT_SHADER);
	m_fullscreenTexQuadProgram = Shader::Link(shaders, 2, true);

	/*
	 * Make a texture big enough to render the whole screen on it
	 * Allocate CPU memory for it too
	 */

	m_screenTexWidth = static_cast<GLsizei>(getFramebufferWidth());
	m_screenTexHeight = static_cast<GLsizei>(getFramebufferHeight());

	glGenTextures(1, &m_screenTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_screenTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_screenTexWidth, m_screenTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	size_t screenBufferSize = 3 * m_screenTexWidth * m_screenTexHeight * sizeof(unsigned char);
	m_screenBuffer = (unsigned char*)malloc(screenBufferSize);
	assert(m_screenBuffer != nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	/*
	 * In this test we generate a ray per-pixel from the camera
	 * All rays have same origin, but different directions
	 */
	m_cam = Camera::Perspective(glm::vec3(0, 0, -5.0f), glm::vec3(0, 0, 1.0f), glm::vec3(0, 1.0f, 0), glm::radians(60.0f), static_cast<float>(m_screenTexWidth), static_cast<float>(m_screenTexHeight));
	glm::vec3 l = glm::normalize(glm::vec3(-0.6f, 0.7f, -0.5f));

	Geometry::Ray ray;
	float t, diff;
	bool result;
	glm::vec3 albedo(80.0f, 150.0f, 30.0f);
	glm::vec3 color;

	double timeStart = glfwGetTime();

	for (int y = 0; y < m_screenTexHeight; ++y)
	{
		for (int x = 0; x < m_screenTexWidth; ++x)
		{
			ray = m_cam.screenPixelToRay(glm::vec2(x, y));
			result = m_scene.cast(ray, t);

			if (result)
			{
				// some basic half-lambert diffuse illumination
				glm::vec3 p = ray(t);
				glm::vec3 n = m_scene.gradient(p);
				diff = 0.5f + 0.5f * glm::dot(n, l);

				color = diff * albedo;

				m_screenBuffer[3 * (y * m_screenTexWidth + x) + 0] = static_cast<unsigned char>(color.r);
				m_screenBuffer[3 * (y * m_screenTexWidth + x) + 1] = static_cast<unsigned char>(color.g);
				m_screenBuffer[3 * (y * m_screenTexWidth + x) + 2] = static_cast<unsigned char>(color.b);
			}
			else
			{
				m_screenBuffer[3 * (y * m_screenTexWidth + x) + 0] = 255;
				m_screenBuffer[3 * (y * m_screenTexWidth + x) + 1] = 255;
				m_screenBuffer[3 * (y * m_screenTexWidth + x) + 2] = 255;
			}
		}
	}

	double timeEnd = glfwGetTime();

	std::cout << "Image generated in " << 1000.0 * (timeEnd - timeStart) << " ms.\n.";

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_screenTex);

	// TODO: opengl 4 supports pixel buffer objects, which are shared memory and are fast!
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_screenTexWidth, m_screenTexHeight, GL_RGB, GL_UNSIGNED_BYTE, (void*)m_screenBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	m_clearColor[0] = 0.0f; m_clearColor[1] = 0.0f; m_clearColor[2] = 0.0f; m_clearColor[3] = 1.0f;

}

void ScratchApp::update(double dt)
{
	glClearBufferfv(GL_COLOR, 0, m_clearColor);

	GLuint err = glGetError();

	glUseProgram(m_fullscreenTexQuadProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_screenTex);

	err = glGetError();

	glBindVertexArray(m_fullscreenQuadVao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	err = glGetError();


	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void ScratchApp::teardown()
{
	glDeleteVertexArrays(1, &m_fullscreenQuadVao);
	glDeleteTextures(1, &m_screenTex);
	free(m_screenBuffer);
}

void ScratchApp::onResize(int width, int height)
{

}

void ScratchApp::onKeyboarEvent(int key, int scancode, int action, int mods)
{

}