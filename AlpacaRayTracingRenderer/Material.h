/*******************************************************************
 * \file Material.h
 *
 * \author puppet_master
 * \date 2018/07/11
 * \https://blog.csdn.net/puppet_master
 * ���ʣ��������淴����
 *******************************************************************/
#pragma once
#include "HitableObject.h"
#include "Color.h"
#include "Math.h"
struct HitResult;

class Material
{
public:
	Material(const Color& al) : albedo(al) {};
	virtual bool Scatter(const Ray& ray, const HitResult& result, Color& attenuation, Ray& scatterRay) const = 0;

	Color albedo;
};
