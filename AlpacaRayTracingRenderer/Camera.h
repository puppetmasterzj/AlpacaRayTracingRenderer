/*******************************************************************
 * \file Camera.h
 *
 * \author puppet_master
 * \date 2018/07/07
 * \https://blog.csdn.net/puppet_master
 * œ‡ª˙¿‡£¨øπæ‚≥›
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

	Camera()
	{
		ori = Vector3(1.0f, 0.0f, 0.0f);
		bottomLeft = Vector3(-2.0f, -1.0f, -1.0f);
		horizontal = Vector3(4.0f, 0.0f, 0.0f);
		vertical = Vector3(0.0f, 2.0f, 0.0f);
	}

	Ray GetRay(float u, float v) { return Ray(ori, bottomLeft + horizontal * u + vertical * v - ori); }
};
