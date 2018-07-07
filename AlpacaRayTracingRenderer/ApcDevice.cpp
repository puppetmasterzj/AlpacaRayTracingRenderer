#include "ApcDevice.h"
#include "Sphere.h"

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

	//暂时在此处添加对象
	Sphere* sphere1 = new Sphere(Vector3(0.0f, 0.0f, -1.0f), 0.5f);
	hitableObjects.push_back(sphere1);

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

	for (int i = 0; i < deviceHeight; i++)
	{
		for (int j = 0; j < deviceWidth; j++)
		{
			float u = float(j) / float(deviceWidth);
			float v = float(i) / float(deviceHeight);

			Vector3 rayDir = bottomLeft + horizontal * u + vertical * v;
			Ray ray(rayOri, rayDir);
			HitResult result;
			bool hit = HitDetect(ray, 0.0f, 10000.0f, result);
			if (hit)
			{
				Vector3 normal = result.normal * 0.5f + Vector3(0.5f, 0.5f, 0.5f);
				DrawPixel(j, i, normal);
			}
			else
			{
				DrawPixel(j, i, Color(1, 1, 1, 1));
			}
		}
	}
}

bool ApcDevice::HitDetect(const Ray& ray, float min_t, float max_t, HitResult& result)
{
	bool hit = false;
	float cloest = max_t;
	HitResult tempResult;

	for (HitableObjList::iterator it = hitableObjects.begin(); it != hitableObjects.end(); ++it)
	{
		if ((*it)->Hit(ray, min_t, max_t, tempResult))
		{
			hit = true;
			cloest = tempResult.t;
			result = tempResult;
		}
	}
	return hit;
}