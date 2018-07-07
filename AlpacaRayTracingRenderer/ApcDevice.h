/*******************************************************************
 * \file ApcDevice.h
 *
 * \author puppet_master
 * \date 2018/07/07
 * \https://blog.csdn.net/puppet_master
 * 光线追踪渲染器主要渲染功能
 *******************************************************************/
#pragma once
#include "Stdfx.h"
#include "Color.h"
#include "Ray.h"


class ApcDevice
{
private:
	int deviceWidth;
	int deviceHeight;
	HDC screenHDC;
	float** zBuffer;

public:
	ApcDevice();
	~ApcDevice();

	void InitDevice(HDC hdc, int screenWidth, int screenHeight);
	void ReleaseDevice();
	void Clear();

	void DoRender();

	void DrawPixel(int x, int y, const Color& color);
	void DrawPixel(int x, int y, const Vector3& vec);

	float HitSphere(const Vector3& center, float radius, const Ray& ray);
	
};

