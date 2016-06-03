#include <Geometry.h>

using namespace sdfedit;
using namespace sdfedit::Geometry;

Ray::Ray(glm::vec3 const& origin, glm::vec3 const& dir)
	: m_origin(origin)
	, m_dir(dir)
{

}

Ray::~Ray()
{

}

glm::vec3 Ray::operator()(float t) const
{
	return (m_origin + t * m_dir);
}