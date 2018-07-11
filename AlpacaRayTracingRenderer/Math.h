/*******************************************************************
 * \file Math.h
 *
 * \author puppet_master
 * \date 2018/07/11
 * \https://blog.csdn.net/puppet_master
 * 数学工具函数
 *******************************************************************/
#pragma once
#include "Stdfx.h"
#include "Vector3.h"

class Math
{
public:
	static float Random01()
	{
		return rand() / float(RAND_MAX);
	}

	static Vector3 RandomVectorInUnitSphere()
	{
		Vector3 pos;
		do
		{
			pos = Vector3(Random01(), Random01(), Random01()) * 2.0f - Vector3(1.0, 1.0, 1.0);
		} while (pos.Magnitude() >= 1.0f);
		return pos;
	}

	static Vector3 Reflect(const Vector3& vec, const Vector3& normal)
	{
		return vec - normal * 2.0f * Vector3::Dot(vec, normal);
	}

};

