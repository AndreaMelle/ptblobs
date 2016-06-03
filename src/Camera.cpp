#include <Camera.h>

using namespace sdfedit;
using namespace sdfedit::Camera;

Perspective::Perspective()
{

}

Perspective::Perspective(glm::vec3 const& origin, glm::vec3 const& dir, glm::vec3 const& up, float vertFovRadians, float width, float height)
	: m_origin(origin)
	, m_width(width)
	, m_height(height)
	, m_vertFovRadians(vertFovRadians)
{
	assert(m_width > 0);
	assert(m_height > 0);
	assert(m_vertFovRadians > 0);

	m_aspectRatio = m_width / m_height;

	m_dir = glm::normalize(dir);
	m_up = glm::normalize(up);

	m_worldRight = glm::cross(m_dir, m_up);
	m_worldUp = glm::cross(m_worldRight, m_dir);

	m_worldRightAtUnitDist = m_aspectRatio * std::tanf(m_vertFovRadians * 0.5f) * m_worldRight;
	m_worldUpAtUnitDist = std::tanf(m_vertFovRadians * 0.5f) * m_worldUp;
}

Perspective::~Perspective() { }

glm::vec3 Perspective::getWorldForward() const { return m_dir;  }
glm::vec3 Perspective::getWorldRight() const { return m_worldRight; }
glm::vec3 Perspective::getWorldUp() const { return m_worldUp; }
float Perspective::getAspectRatio() const { return m_aspectRatio; }

Geometry::Ray Perspective::screenPixelToRay(glm::vec2 const& pixel) const
{
	glm::vec3 dx = m_worldRightAtUnitDist * (2.0f * (pixel.x + 0.5f) / m_width - 1.0f);
	glm::vec3 dy = m_worldUpAtUnitDist * (2.0f * (pixel.y + 0.5f) / m_height - 1.0f);

	glm::vec3 d = glm::normalize(m_dir + dx + dy);

	return Geometry::Ray(m_origin, d);
}
