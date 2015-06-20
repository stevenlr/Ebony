#ifndef EBONY_UTILS_MATHS_H_
#define EBONY_UTILS_MATHS_H_

namespace ebony {

	const double PI_D = 3.14159265359;
	const float PI_F = 3.14159265359f;

	template<typename T>
	inline T lerp(T x1, T x2, T y1, T y2, T x)
	{
		return (x - x1) / (x2 - x1) * (y2 - y1) + y1;
	}

}
#endif
