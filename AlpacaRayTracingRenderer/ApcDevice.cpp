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
	Sphere* sphere2 = new Sphere(Vector3(1.0f, 0.0f, -1.0f), 0.5f);
	Sphere* sphere3 = new Sphere(Vector3(2.0f, 0.0f, -1.0f), 0.5f);
	hitableObjects.push_back(sphere1);
	hitableObjects.push_back(sphere2);
	hitableObjects.push_back(sphere3);
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

Vector3 ApcDevice::RandomVectorInUnitSphere()
{
	Vector3 pos;
	do 
	{
		pos = Vector3(Random01(), Random01(), Random01()) * 2.0f - Vector3(1.0, 1.0, 1.0);
	} while (pos.Magnitude() >= 1.0f);
	return pos;
}

Color ApcDevice::Render(const Ray& ray, int count)
{
	HitResult result;
	if (HitDetect(ray, 0.0f, 10000.0f, result) && count < 5)
	{
		Vector3 target = result.position + result.normal + RandomVectorInUnitSphere();
		Ray newRay(result.position, target - result.position);
		return Render(newRay, ++count) * 0.5f;
	}
	else
	{
		return Color(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void ApcDevice::DoRender()
{
	for (int i = 0; i < deviceHeight; i++)
	{
		for (int j = 0; j < deviceWidth; j++)
		{
			Color color(0, 0, 0, 1);
			for (int k = 0; k < 10; k++)
			{
				float u = float(j + Random01()) / float(deviceWidth);
				float v = float(i + Random01()) / float(deviceHeight);

				Ray ray = camera.GetRay(u, v);
				color = color + Render(ray, 0);
			}
			color = color / 10.0f;

			DrawPixel(j, i, color);
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