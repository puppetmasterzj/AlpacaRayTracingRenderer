#pragma once
/*******************************************************************
 * \file Ray.h
 *
 * \author puppet_master
 * \date 2018/07/07
 * \https://blog.csdn.net/puppet_master
 * …‰œﬂ¿‡
 *******************************************************************/
#include "Vector3.h"
class Ray
{
public:
	Vector3 ori;
	Vector3 dir;

	Ray(const Vector3& oriPos, const Vector3& dirVec) { ori = oriPos; dir = dirVec; }

	Vector3 GetPointOnRay(float t) const { return ori + dir * t; }
};