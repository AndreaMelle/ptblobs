#ifndef __SDFEDIT_UTILS_H__
#define __SDFEDIT_UTILS_H__

namespace sdfedit
{
	namespace math
	{
		typedef std::function<float(float a, float b, float k)> smin_func;
		//typedef float smin_func(float a, float b, float k);

		const float kSMinExpFactorDefault = 32.0f;
		const float kSMinPolyFactorDefault = 0.5f;
		const float kSMinPowFactorDefault = 8.0f;

		// exponential smooth min (k = 32);
		inline float smin_exp(float a, float b, float k = kSMinExpFactorDefault)
		{
			float res = std::expf(-k * a) + std::expf(-k * b);
			return -std::logf(res) / k;
		}

		// polynomial smooth min (k = 0.1);
		inline float smin_poly(float a, float b, float k = kSMinPolyFactorDefault)
		{
			float h = glm::clamp(0.5f + 0.5f * (b - a) / k, 0.0f, 1.0f);
			return glm::mix(b, a, h) - k * h * (1.0f - h);
		}

		// power smooth min (k = 8);
		inline float smin_pow(float a, float b, float k = kSMinPowFactorDefault)
		{
			a = std::powf(a, k); b = std::powf(b, k);
			return std::powf((a*b) / (a + b), 1.0f / k);
		}
	}
}

#endif //__SDFEDIT_UTILS_H__