/*******************************************************************
 * \file Sphere.h
 *
 * \author puppet_master
 * \date 2018/07/07
 * \https://blog.csdn.net/puppet_master
 * ÇòÌå
 *******************************************************************/
#pragma once
#include "HitableObject.h"
class Sphere :
	public HitableObject
{
public:
	Vector3 center;
	float radius;
public:
	Sphere() {};
	Sphere(const Vector3& c, float r) : center(c), radius(r) {};

	virtual bool Hit(const Ray& ray, float min_t, float max_t, HitResult& result) const;
	void FillHitResult(float t, const Ray& ray, HitResult& result) const;
};

