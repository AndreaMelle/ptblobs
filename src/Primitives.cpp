#include <Primitives.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace sdfedit;
using namespace sdfedit::Primitives;
using namespace sdfedit::Operations;

const float SDFScene::kDefaultRayMin = 1.0f;
const float SDFScene::kDefaultRayMax = 10.0f;
const float SDFScene::kDefaultPrecision = 0.002f;
const int	SDFScene::kDefaultDepth = 50;
const glm::vec3 SDFScene::kDefaultGradientEps = glm::vec3(0.001f, 0.0f, 0.0f);

SDFScene::SDFScene()
{
	auto sphereRight = std::make_shared<SignedSphere>(1.0f);
	auto sphereLeft = std::make_shared<SignedSphere>(1.0f);
	auto sphereUp = std::make_shared<SignedSphere>(1.0f);
	auto box = std::make_shared<SignedBox>(glm::vec3(1.0f, 0.6f, 0.6f));

	auto sphereRightTranslate = std::make_shared<SDFTransform>(glm::translate(glm::mat4(), glm::vec3(-0.75f, 0.0f, 0.0f)));
	auto sphereLeftTranslate = std::make_shared<SDFTransform>(glm::translate(glm::mat4(), glm::vec3(0.75f, 0.0f, 0.0f)));
	auto sphereUpTranslate = std::make_shared<SDFTransform>(glm::translate(glm::mat4(), glm::vec3(0.0f, 1.5f, 0.0f)));
	auto boxTranslate = std::make_shared<SDFTransform>(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.75f, 0.0f)));

	auto blend = std::make_shared<BlendOp>();

	auto add = std::make_shared<Union>();
	auto sub = std::make_shared<Subtraction>();

	m_edits.push_back(std::make_shared<SDFEdit>(sphereRight, add, sphereRightTranslate));
	m_edits.push_back(std::make_shared<SDFEdit>(sphereLeft, blend, sphereLeftTranslate));
	m_edits.push_back(std::make_shared<SDFEdit>(box, blend, boxTranslate));
	m_edits.push_back(std::make_shared<SDFEdit>(sphereUp, sub, sphereUpTranslate));

}

float SDFScene::evaluate(glm::vec3 const& p) const
{
	SDFEdit::ConstIterator it = m_edits.begin();

	float d = std::numeric_limits<float>::infinity();

	for (it; it < m_edits.end(); ++it)
	{
		d = (*it)->evaluate(d, p);
	}

	return d;
}

bool SDFScene::cast(Geometry::Ray const& ray, float& t) const
{
	t = kDefaultRayMin;

	for (int i = 0; i < kDefaultDepth; ++i)
	{
		float d = evaluate(ray(t));

		if (d < kDefaultPrecision)
		{
			return true;
		}

		if (t > kDefaultRayMax)
		{
			return false;
		}

		t += d;
	}

	return false;
}

glm::vec3 SDFScene::gradient(glm::vec3 const& p) const
{
	return glm::normalize(glm::vec3(
		evaluate(p + kDefaultGradientEps.xyy) - evaluate(p - kDefaultGradientEps.xyy),
		evaluate(p + kDefaultGradientEps.yxy) - evaluate(p - kDefaultGradientEps.yxy),
		evaluate(p + kDefaultGradientEps.yyx) - evaluate(p - kDefaultGradientEps.yyx)
	));
}

SDFTransform::SDFTransform()
	: m_transform(glm::mat4())
	, m_inverseTransform(glm::mat4())
{
}

SDFTransform::SDFTransform(glm::mat4 const& m)
	: m_transform(m)
{
	m_inverseTransform = glm::inverse(m_transform);
}

glm::vec3 SDFTransform::operator()(glm::vec3 const& p) const
{
	glm::vec4 q = m_inverseTransform * glm::vec4(p, 1.0f);
	return q.xyz;
}

void SDFTransform::setTransform(glm::mat4 const& m)
{
	m_transform = m;
	m_inverseTransform = glm::inverse(m);
}

BlendOp::BlendOp()
	: m_blendFunc(math::smin_poly)
	, m_sminFactor(math::kSMinPolyFactorDefault)
{

}


float BlendOp::operator()(float lhs, float rhs) const
{
	return m_blendFunc(lhs, rhs, m_sminFactor);
}

SDFEdit::SDFEdit(Primitives::SDFPrimitive::Ptr const& primitive, Operations::SDFOperation::Ptr const& operation, SDFTransform::Ptr const& transform)
	: m_primtive(primitive)
	, m_operation(operation)
	, m_transform(transform)
{
	assert(m_primtive != nullptr);
	assert(m_operation != nullptr);
	assert(m_transform != nullptr);
}

SDFEdit::~SDFEdit()
{
}

float SDFEdit::evaluate(float lhs, glm::vec3 const& rhs)
{
	glm::vec3 p = m_transform->operator()(rhs);
	float d = m_primtive->operator()(p);
	return m_operation->operator()(lhs, d);
}