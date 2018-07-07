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
				//������ָ����ײ��Ϊ���߷���
				Vector3 hitPoint = ray.GetPointOnRay(result);
				Vector3 normal = (hitPoint - sphereCenter);
				normal.Normalize();
				//ת����01������ӻ�
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
//���巽����Ϊ����C��xc,yc,zc�����뾶R������A + B * t
//�ж�һ��P xyz�Ƿ������ϻ��ڣ�(x - cx)(x - cx) + (y - cy)(y - cy) + (z - cz)(z - cz) = R * R
//��dot((P - C), (P - C)) = R * R
//������������ dot((A + Bt - C), (A + Bt - C)) = R * R
//t*t*dot(B,B) + 2t*dot(B, A - C) + dot��A - C�� A - C��- R * R = 0
//��a = dot(B,B), b = 2dot��B��A - C��, c = dot(A - C, A - C) - R * R
//һԪ���η��������ʽ b ^ 2 - 4ac >= 0 �и��ཻ
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
	//�����ȥ��С��Ϊ��һ���ཻ��
	return (-b - sqrt(result)) / (2.0 * a);
}