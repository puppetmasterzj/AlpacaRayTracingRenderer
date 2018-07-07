#include "Sphere.h"

//////////////////////////////////////////////////////////////////////////
//球体方程设为球心C（xc,yc,zc），半径R，向量A + B * t
//判断一点P xyz是否在球上或内，(x - cx)(x - cx) + (y - cy)(y - cy) + (z - cz)(z - cz) = R * R
//即dot((P - C), (P - C)) = R * R
//代入向量方程 dot((A + Bt - C), (A + Bt - C)) = R * R
//t*t*dot(B,B) + 2t*dot(B, A - C) + dot（A - C， A - C）- R * R = 0
//设a = dot(B,B), b = 2dot（B，A - C）, c = dot(A - C, A - C) - R * R
//一元二次方程求根公式 b ^ 2 - 4ac >= 0 有根相交
//////////////////////////////////////////////////////////////////////////
bool Sphere::Hit(const Ray& ray, float min_t, float max_t, HitResult& result) const
{
	Vector3 ac = ray.ori - center; //A - C
	float a = Vector3::Dot(ray.dir, ray.dir); //a
	float b = 2.0f * Vector3::Dot(ray.dir, ac);//b
	float c = Vector3::Dot(ac, ac) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant > 0)
	{
		//优先近处交点
		float t = (-b - sqrt(discriminant)) / (2.0 * a);
		if (t > min_t && t < max_t)
		{
			FillHitResult(t, ray, result);
			return true;
		}
		t = (-b + sqrt(discriminant)) / (2.0 * a);
		if (t > min_t && t < max_t)
		{
			FillHitResult(t, ray, result);
			return true;
		}
	}
	return false;
}

inline void Sphere::FillHitResult(float t, const Ray& ray, HitResult& result) const
{
	result.t = t;
	result.position = ray.GetPointOnRay(t);
	result.normal = (result.position - center) / radius;
}