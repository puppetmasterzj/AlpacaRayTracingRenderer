/*******************************************************************
 * \file Camera.h
 *
 * \author puppet_master
 * \date 2018/07/07
 * \https://blog.csdn.net/puppet_master
 * Ïà»úÀà£¬¿¹¾â³İ
 *******************************************************************/
#pragma once
#include "Ray.h"

class Camera
{
public:
	Vector3 ori;
	Vector3 bottomLeft;
	Vector3 horizontal;
	Vector3 vertical;
	float lensRadius;
	Vector3 u, v, w;

	//fov:vDir
	Camera(const Vector3& cameraPos, const Vector3& lookAtPos, const Vector3& upDir, float fov, float aspect, float aperture, float focusDist)
	{
		float fovAngle = fov * PI / 180.0;
		float halfHeight = tan(0.5 * fovAngle);
		float halfWidth = aspect * halfHeight;

		lensRadius = aperture / 2;

		ori = cameraPos;
		w = cameraPos - lookAtPos;
		w.Normalize();
		u = Vector3::Cross(upDir, w);
		u.Normalize();
		v = Vector3::Cross(w, u);

		bottomLeft = Vector3(-halfWidth, -halfHeight, -1.0f);
		bottomLeft = ori - u * halfWidth * focusDist - v * halfHeight * focusDist - w * focusDist;
		horizontal = u * 2 * halfWidth * focusDist;
		vertical = v * 2 * halfHeight * focusDist;
	}

	Ray GetRay(float s, float t)
	{
		Vector3 rd = RandomInUnitDisk() * lensRadius;
		Vector3 offset = u * rd.x + v * rd.y;
		return Ray(ori + offset, bottomLeft + horizontal * s + vertical * t - ori - offset);
	}

	Vector3 RandomInUnitDisk()
	{
		Vector3 p;
		do 
		{
			p = Vector3(Math::Random01(), Math::Random01(), 0) * 2.0 - Vector3(1, 1, 0);
		} while (Vector3::Dot(p, p) >= 1.0);
		return p;
	}
};
