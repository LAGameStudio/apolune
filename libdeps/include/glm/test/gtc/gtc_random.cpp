///////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2012 G-Truc Creation (www.g-truc.net)
///////////////////////////////////////////////////////////////////////////////////////////////////
// Created : 2011-09-19
// Updated : 2011-09-19
// Licence : This source is under MIT licence
// File    : test/gtc/random.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/epsilon.hpp>
#include <iostream>

int test_linearRand()
{
	int Error = 0;

	{
		float ResultFloat = 0.0f;
		double ResultDouble = 0.0f;
		for(std::size_t i = 0; i < 100000; ++i)
		{
			ResultFloat += glm::linearRand(-1.0f, 1.0f);
			ResultDouble += glm::linearRand(-1.0, 1.0);
		}

		Error += glm::epsilonEqual(ResultFloat, 0.0f, 0.0001f);
		Error += glm::epsilonEqual(ResultDouble, 0.0, 0.0001);
		assert(!Error);
	}

	return Error;
}

int test_circularRand()
{
	int Error = 0;

	{
		std::size_t Max = 100000;
		float ResultFloat = 0.0f;
		double ResultDouble = 0.0f;
		double Radius = 2.0f;

		for(std::size_t i = 0; i < Max; ++i)
		{
			ResultFloat += glm::length(glm::circularRand(1.0f));
			ResultDouble += glm::length(glm::circularRand(Radius));
		}

		Error += glm::epsilonEqual(ResultFloat, float(Max), 0.01f) ? 0 : 1;
		Error += glm::epsilonEqual(ResultDouble, double(Max) * double(Radius), 0.01) ? 0 : 1;
		assert(!Error);
	}

	return Error;
}

int test_sphericalRand()
{
	int Error = 0;

	{
		std::size_t Max = 100000;
		float ResultFloatA = 0.0f;
		float ResultFloatB = 0.0f;
		float ResultFloatC = 0.0f;
		double ResultDoubleA = 0.0f;
		double ResultDoubleB = 0.0f;
		double ResultDoubleC = 0.0f;

		for(std::size_t i = 0; i < Max; ++i)
		{
			ResultFloatA += glm::length(glm::sphericalRand(1.0f));
			ResultDoubleA += glm::length(glm::sphericalRand(1.0));
			ResultFloatB += glm::length(glm::sphericalRand(2.0f));
			ResultDoubleB += glm::length(glm::sphericalRand(2.0));
			ResultFloatC += glm::length(glm::sphericalRand(3.0f));
			ResultDoubleC += glm::length(glm::sphericalRand(3.0));
		}

		Error += glm::epsilonEqual(ResultFloatA, float(Max), 0.01f) ? 0 : 1;
		Error += glm::epsilonEqual(ResultDoubleA, double(Max), 0.0001) ? 0 : 1;
		Error += glm::epsilonEqual(ResultFloatB, float(Max * 2), 0.01f) ? 0 : 1;
		Error += glm::epsilonEqual(ResultDoubleB, double(Max * 2), 0.0001) ? 0 : 1;
		Error += glm::epsilonEqual(ResultFloatC, float(Max * 3), 0.01f) ? 0 : 1;
		Error += glm::epsilonEqual(ResultDoubleC, double(Max * 3), 0.01) ? 0 : 1;
		assert(!Error);
	}

	return Error;
}

int test_diskRand()
{
	int Error = 0;

	{
		float ResultFloat = 0.0f;
		double ResultDouble = 0.0f;

		for(std::size_t i = 0; i < 100000; ++i)
		{
			ResultFloat += glm::length(glm::diskRand(2.0f));
			ResultDouble += glm::length(glm::diskRand(2.0));
		}

		Error += ResultFloat < 200000.f ? 0 : 1;
		Error += ResultDouble < 200000.0 ? 0 : 1;
		assert(!Error);
	}

	return Error;
}

int test_ballRand()
{
	int Error = 0;

	{
		float ResultFloat = 0.0f;
		double ResultDouble = 0.0f;

		for(std::size_t i = 0; i < 100000; ++i)
		{
			ResultFloat += glm::length(glm::ballRand(2.0f));
			ResultDouble += glm::length(glm::ballRand(2.0));
		}

		Error += ResultFloat < 200000.f ? 0 : 1;
		Error += ResultDouble < 200000.0 ? 0 : 1;
		assert(!Error);
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += test_linearRand();
	Error += test_circularRand();
	Error += test_sphericalRand();
	Error += test_diskRand();
	Error += test_ballRand();

	return Error;
}
