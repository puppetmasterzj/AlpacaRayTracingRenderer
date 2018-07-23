/*******************************************************************
 * \file RefractMaterial.h
 *
 * \author puppet_master
 * \date 2018/07/21
 * \https://blog.csdn.net/puppet_master
 * 折射材质
 *******************************************************************/
#pragma once
#include "Material.h"
class RefractMaterial : public Material
{
private:
	float refractRatio;
public:
	RefractMaterial(const Color& albedo, float ratio) : Material(albedo), refractRatio(ratio) {};

	virtual bool Scatter(const Ray& ray, const HitResult& result, Color& attenuation, Ray& scatterRay) const
	{
		Vector3 normal;
		Vector3 refractDir;
		float refractFactor;
		Vector3 dir = ray.dir;
		attenuation = Color(1.0, 1.0, 1.0, 1.0);
		float cosine = Vector3::Dot(dir, result.normal) / dir.Magnitude();
		float reflect_prob;

		Vector3 reflectDir = Math::Reflect(ray.dir, result.normal);

		//以球体为例，是从球体向外的出射光线
		if (Vector3::Dot(dir, result.normal) > 0)
		{
			normal = Vector3(-result.normal.x, -result.normal.y, -result.normal.z);
			refractFactor = refractRatio;
			cosine = refractRatio * cosine;
		}
		else //从外界射进球体内部的光线
		{
			normal = result.normal;
			refractFactor = 1.0 / refractRatio;
			cosine = -cosine;
		}

		if (Math::Refract(ray.dir, normal, refractFactor, refractDir))
		{
			reflect_prob = Math::Schlick(cosine, refractRatio);
		}
		else
		{
			scatterRay = Ray(result.position, reflectDir);
			reflect_prob = 1.0;
		}

		if (Math::Random01() < reflect_prob)
		{
			scatterRay = Ray(result.position, reflectDir);
		}
		else
		{
			scatterRay = Ray(result.position, refractDir);
		}

		return true;
	}
};
