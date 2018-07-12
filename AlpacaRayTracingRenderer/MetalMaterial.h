/*******************************************************************
 * \file MetalMaterial.h
 *
 * \author puppet_master
 * \date 2018/07/12
 * \https://blog.csdn.net/puppet_master
 * ½ðÊô·´Éä²ÄÖÊ
 *******************************************************************/
#pragma once
#include "Material.h"

class MetalMaterial : public Material
{
public :
	MetalMaterial(const Color& al) : Material(al) {};

	virtual bool Scatter(const Ray& ray, const HitResult& result, Color& attenuation, Ray& scatterRay) const
	{
		Vector3 vec = ray.dir;
		Vector3 normal = result.normal;
		vec.Normalize();
		normal.Normalize();
		Vector3 reflect = Math::Reflect(vec, normal);

		scatterRay = Ray(result.position, reflect + Math::RandomVectorInUnitSphere());
		attenuation = albedo;
		return Vector3::Dot(reflect, normal) > 0;
	}

};