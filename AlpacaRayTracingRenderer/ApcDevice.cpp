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


inline void ApcDevice::DrawPixel(int x, int y, const Vector3& vec)
{
	SetPixel(screenHDC, x, y, RGB(255 * vec.x, 255 * vec.y, 255 * vec.z));
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
	Vector3 rayOri(0.0f, 0.0f, 0.0f);
	Vector3 bottomLeft(-2.0f, -1.0f, -1.0f);
	Vector3 horizontal(4.0f, 0.0f, 0.0f);
	Vector3 vertical(0.0f, 2.0f, 0.0f);

	Vector3 sphereCenter(0.0f, 0.0f, -1.0f);
	float sphereRadius = 0.5f;

	for (int i = 0; i < deviceHeight; i++)
	{
		for (int j = 0; j < deviceWidth; j++)
		{
			float u = float(j) / float(deviceWidth);
			float v = float(i) / float(deviceHeight);

			Vector3 rayDir = bottomLeft + horizontal * u + vertical * v;
			Ray ray(rayOri, rayDir);
			float result = HitSphere(sphereCenter, sphereRadius, ray);
			if (result > 0)
			{
				//从球心指向碰撞点为法线方向
				Vector3 hitPoint = ray.GetPointOnRay(result);
				Vector3 normal = (hitPoint - sphereCenter);
				normal.Normalize();
				//转化到01区间可视化
				normal = normal * 0.5f + Vector3(0.5f, 0.5f, 0.5f);
				DrawPixel(j, i, normal);
			}
			else
			{
				DrawPixel(j, i, Color(1, 1, 1, 1));
			}
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
float ApcDevice::HitSphere(const Vector3& center, float radius, const Ray& ray)
{
	Vector3 ac = ray.ori - center; //A - C
	float a = Vector3::Dot(ray.dir, ray.dir); //a
	float b = 2.0f * Vector3::Dot(ray.dir, ac);//b
	float c = Vector3::Dot(ac, ac) - radius * radius;
	float result = b * b - 4 * a * c;
	if (result < 0)
		return -1.0f;
	//求根，去最小根为第一个相交点
	return (-b - sqrt(result)) / (2.0 * a);
}