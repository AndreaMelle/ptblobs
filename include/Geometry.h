#ifndef __SDFEDIT_GEOMETRY_H__
#define __SDFEDIT_GEOMETRY_H__

#include <Common.h>

namespace sdfedit
{
	namespace Geometry
	{
		class Ray
		{
		public:
			Ray(){}
			Ray(glm::vec3 const& origin, glm::vec3 const& dir);
			virtual ~Ray();

			glm::vec3 operator()(float t) const;

		private:
			glm::vec3 m_origin;
			glm::vec3 m_dir;
		};
	}
}

#endif //__SDFEDIT_GEOMETRY_H__