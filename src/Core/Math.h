#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <cmath>
#include <random>

namespace Math
{
	inline const float PI = 3.14159265359f;

	inline std::mt19937& Rng()
	{
		static std::mt19937 rng{ std::random_device{}() };
		return rng;
	}
}

#endif // MATH_H
