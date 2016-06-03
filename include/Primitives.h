#ifndef __SDFEDIT_PRIMITIVES_H__
#define __SDFEDIT_PRIMITIVES_H__

#include <Common.h>
#include <Utils.h>
#include <Geometry.h>

namespace sdfedit
{
	namespace Primitives
	{
		/*
		 * Interface for a primitive described by a signed distance function
		 */
		class SDFPrimitive
		{
		public:
			typedef std::shared_ptr<SDFPrimitive> Ptr;

			SDFPrimitive() {}
			virtual ~SDFPrimitive() {}
			
			/*
			 * Evaluates the signed distance from the primitive to the given point
			 */
			virtual float operator()(glm::vec3 const& p) const = 0;
		};

		class SignedBox : public SDFPrimitive
		{
		public:
			SignedBox(glm::vec3 const& extents) : m_extents(extents) {}
			virtual ~SignedBox() {}

			virtual float operator()(glm::vec3 const& p) const
			{
				glm::vec3 d = glm::abs(p) - m_extents;
				return glm::min(glm::max(d.x, glm::max(d.y, d.z)), 0.0f) + glm::length(glm::max(d, 0.0f));
			}

		private:
			glm::vec3 m_extents;
		};

		class SignedSphere : public SDFPrimitive
		{
		public:
			SignedSphere(float radius) : m_radius(radius) {}
			virtual ~SignedSphere() {}

			virtual float operator()(glm::vec3 const& p) const
			{
				return glm::length(p) - m_radius;
			}

		private:
			float m_radius;
		};
	}

	namespace Operations
	{
		/*
		* Base class for an operation
		*/
		class SDFOperation
		{
		public:
			typedef std::shared_ptr<SDFOperation> Ptr;

			SDFOperation() {}
			virtual ~SDFOperation() {}

			/*
			* Evaluates the operation
			*/
			virtual float operator()(float lhs, float rhs) const = 0;
		};

		/*
		* Soft-min union operation. Is there a soft-max too?
		*/
		class BlendOp : public SDFOperation
		{
		public:
			typedef std::shared_ptr<BlendOp> Ptr;

			BlendOp();
			virtual ~BlendOp(){}
			
			virtual float operator()(float lhs, float rhs) const;

		private:
			math::smin_func m_blendFunc;
			float m_sminFactor;
		};

		/* Simple add operation */
		class Union : public SDFOperation
		{
		public:
			typedef std::shared_ptr<Union> Ptr;

			Union(){}
			virtual ~Union(){}

			virtual float operator()(float lhs, float rhs) const
			{
				return glm::min(lhs, rhs);
			}
		};

		/* Simple subtract operation */
		class Subtraction : public SDFOperation
		{
		public:
			typedef std::shared_ptr<Subtraction> Ptr;

			Subtraction(){}
			virtual ~Subtraction(){}

			virtual float operator()(float lhs, float rhs) const
			{
				return glm::max(lhs, -rhs);
			}
		};

		/* Simple intersect operation */
		class Intersection : public SDFOperation
		{
		public:
			typedef std::shared_ptr<Intersection> Ptr;

			Intersection(){}
			virtual ~Intersection(){}

			virtual float operator()(float lhs, float rhs) const
			{
				return glm::max(lhs, rhs);
			}
		};
	}

	class SDFTransform
	{
	public:
		typedef std::shared_ptr<SDFTransform> Ptr;

		SDFTransform();
		SDFTransform(glm::mat4 const& m);
		virtual ~SDFTransform() {}

		glm::vec3 operator()(glm::vec3 const& p) const;
		void setTransform(glm::mat4 const& m);

		static SDFTransform::Ptr Identity() { return std::make_shared<SDFTransform>(); }

	private:
		glm::mat4 m_transform;
		glm::mat4 m_inverseTransform;
	};

	class SDFEdit
	{
	public:
		typedef std::shared_ptr<SDFEdit> Ptr;
		typedef std::vector<SDFEdit::Ptr> Vector;
		typedef Vector::iterator Iterator;
		typedef Vector::const_iterator ConstIterator;


		SDFEdit(Primitives::SDFPrimitive::Ptr const& primitive, Operations::SDFOperation::Ptr const& operation, SDFTransform::Ptr const& transform = SDFTransform::Identity());
		virtual ~SDFEdit();

		float evaluate(float lhs, glm::vec3 const& rhs);

	private:
		SDFEdit() {};
		Primitives::SDFPrimitive::Ptr	m_primtive;
		Operations::SDFOperation::Ptr	m_operation;
		SDFTransform::Ptr					m_transform;
	};

	class SDFScene
	{
	public:
		SDFScene();

		/*
		 * Similar to a primitive,
		 * we can query the distance field at a point for entire scene
		 */
		float evaluate(glm::vec3 const& p) const;

		/*
		 * Casts a ray into the scene
		 * Returns whether the ray intersects the scene
		 * Writes out the ray parametric t value at which intersection occurs
		 */
		bool cast(Geometry::Ray const& ray, float& t) const;

		/*
		 * Compute gradient of scene's SDF at given point
		 */
		glm::vec3 gradient(glm::vec3 const& p) const;

	private:
		SDFEdit::Vector m_edits;

		// We start marching at this ray t (acts like near "plane" (sphere) )
		static const float kDefaultRayMin;
			
		// We marched too far along ray, we stop (acts like far "plane" (sphere) )
		static const float kDefaultRayMax;

		// When a point on the ray is this close to a surface, we consider it intersection
		static const float kDefaultPrecision;

		// Maximum number of steps performed by ray marching while attempting to find a surface intersecting the ray
		static const int   kDefaultDepth;

		// Epsilon used to evalate gradient around a point
		static const glm::vec3 kDefaultGradientEps;
	};
}

#endif //__PRIMITIVES_H__