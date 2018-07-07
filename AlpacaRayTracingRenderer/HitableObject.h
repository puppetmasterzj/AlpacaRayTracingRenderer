/*******************************************************************
 * \file HitableObject.h
 *
 * \author puppet_master
 * \date 2018/07/07
 * \https://blog.csdn.net/puppet_master
 * ���Ա����߻��еĶ������
 *******************************************************************/
#pragma once
#include "Ray.h"

struct HitResult
{
	float t;
	Vector3 position;
	Vector3 normal;
};

class HitableObject
{
public:
	virtual bool Hit(const Ray& ray, float min_t, float max_t, HitResult& result) const = 0;
};

