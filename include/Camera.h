#ifndef __SDFEDIT_CAMERA_H__
#define __SDFEDIT_CAMERA_H__

#include <Common.h>
#include <Geometry.h>

namespace sdfedit
{
	namespace Camera
	{
		class Perspective
		{
		public:
			Perspective();
			Perspective(glm::vec3 const& origin,
				glm::vec3 const& dir,
				glm::vec3 const& up,
				float vertFovRadians,
				float width,
				float height);

			virtual ~Perspective();

			/*
			 * Given a pixel in screen coordinates,
			 * returns a ray starting at origin passing through the image plane's pixel position.
			 * Accepts subpixel values.
			 */
			Geometry::Ray screenPixelToRay(glm::vec2 const& pixel) const;

			/* 
			 * Access methods to world-space forward, right and up camera vectors
			 */
			glm::vec3 getWorldForward() const;
			glm::vec3 getWorldRight() const;
			glm::vec3 getWorldUp() const;

			float getAspectRatio() const;

		private:
			glm::vec3 m_origin;
			glm::vec3 m_dir;
			glm::vec3 m_up;
			
			float	  m_vertFovRadians;
			float	  m_aspectRatio;
			float     m_width;
			float	  m_height;

			glm::vec3 m_worldRight;
			glm::vec3 m_worldUp;
			glm::vec3 m_worldRightAtUnitDist;
			glm::vec3 m_worldUpAtUnitDist;

		};
	}
}

#endif //__SDFEDIT_CAMERA_H__