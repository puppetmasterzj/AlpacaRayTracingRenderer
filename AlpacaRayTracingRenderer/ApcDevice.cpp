#include "ApcDevice.h"

ApcDevice::ApcDevice()
{
}


ApcDevice::~ApcDevice()
{
}

void ApcDevice::InitDevice(HDC hdc, int screenWidth, int screenHeight)
{
	screenHDC = hdc;
	deviceWidth = screenWidth;
	deviceHeight = screenHeight;
	
	zBuffer = new float*[deviceHeight];
	for (int i = 0; i < deviceHeight; i++)
	{
		zBuffer[i] = new float[deviceWidth];
	}
}

void ApcDevice::ReleaseDevice()
{
	for (int i = 0; i < deviceHeight; i++)
	{
		delete[] zBuffer[i];
	}
}


inline void ApcDevice::DrawPixel(int x, int y, const Color& color)
{
	SetPixel(screenHDC, x, y, RGB(255 * color.r, 255 * color.g, 255 * color.b));
}

void ApcDevice::Clear()
{
	BitBlt(screenHDC, 0, 0, deviceWidth, deviceHeight, NULL, NULL, NULL, BLACKNESS);
	//ClearZ
	for (int i = 0; i < deviceHeight; i++)
	{
		for (int j = 0; j < deviceWidth; j++)
		{
			zBuffer[i][j] = 0.0f;
		}
	}
}

void ApcDevice::DoRender()
{
	Vector3 rayOri(400.0f, 200.0f, 0.0f);
	Vector3 sphereCenter(400.0f, 200.0f, -25.0f);
	float sphereRadius = 20.0f;

	for (int i = 0; i < deviceHeight; i++)
	{
		for (int j = 0; j < deviceWidth; j++)
		{
			Vector3 rayDir(j, i, -50.0f);
			Ray ray(rayOri, rayDir);
			if (HitSphere(sphereCenter, sphereRadius, ray))
				DrawPixel(j, i, Color(1, 0, 0, 1));
			else
				DrawPixel(j, i, Color(1, 1, 1, 1));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//球体方程设为球心C（xc,yc,zc），半径R，向量A + B * t
//判断一点P xyz是否在球上或内，(x - cx)(x - cx) + (y - cy)(y - cy) + (z - cz)(z - cz) = R * R
//即dot((P - C), (P - C)) = R * R
//代入向量方程 dot((A + Bt - C), (A + Bt - C)) = R * R
//t*t*dot(B,B) + 2t*dot(B, A - C) + dot（A - C， A - C）- R * R = 0
//设a = dot(B,B), b = 2dot（B，A - C）, c = dot(A - C, A - C) - R * R
//一元二次方程求根公式 b ^ 2 - 4ac >= 0 有根相交
//////////////////////////////////////////////////////////////////////////
bool ApcDevice::HitSphere(const Vector3& center, float radius, const Ray& ray)
{
	Vector3 ac = ray.ori - center; //A - C
	float a = Vector3::Dot(ray.dir, ray.dir); //a
	float b = 2.0f * Vector3::Dot(ray.dir, ac);//b
	float c = Vector3::Dot(ac, ac) - radius * radius;
	float result = b * b - 4 * a * c;
	return result > 0;
}