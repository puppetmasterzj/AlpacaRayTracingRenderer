/*******************************************************************
 * \file LambertMaterial.h
 *
 * \author puppet_master
 * \date 2018/07/11
 * \https://blog.csdn.net/puppet_master
 * Âþ·´Éä²ÄÖÊ
 *******************************************************************/
#pragma once
#include "Material.h"

class LambertMaterial : public Material
{
public:
	LambertMaterial(const Color& al) : Material(al) {};

	virtual bool Scatter(const Ray& ray, const HitResult& result, Color& attenuation, Ray& scatterRay) const
	{
		Vector3 target = result.position + result.normal + Math::RandomVectorInUnitSphere();
		scatterRay = Ray(result.position, target - result.position);
		attenuation = albedo;
		return true;
	}
};

