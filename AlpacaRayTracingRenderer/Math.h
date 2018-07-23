/*******************************************************************
 * \file Math.h
 *
 * \author puppet_master
 * \date 2018/07/11
 * \https://blog.csdn.net/puppet_master
 * ��ѧ���ߺ���
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

	//refractRatio = sin��' / sin�� 
	static bool Refract(const Vector3& vec, const Vector3& normal, float refractRatio, Vector3& refractDir)
	{
		Vector3 inVec = vec;
		inVec.Normalize();
		float dt = Vector3::Dot(inVec, normal); //cos��
		float s2 = 1.0 - dt * dt; // sin�� ^ 2
		float st2 = refractRatio * refractRatio * s2;// sin�ȡ� ^ 2 = refractRato ^ 2 * sin�� ^ 2
		float cost2 = 1 - st2;
		if (cost2 > 0)
		{
			refractDir = (inVec - normal * dt) * refractRatio - normal * sqrt(cost2);
			return true;
		}
		return false;
	}

	//���Ʒ������
	static float Schlick(float cosine, float refractRatio)
	{
		float r = (1 - refractRatio) / (1 + refractRatio);
		r = r * r;
		return r + (1 - r) * pow((1 - cosine), 5);
	}

};
const float PI = 3.14;

