#include "ApcDevice.h"
#include "Sphere.h"
#include "Math.h"
#include "LambertMaterial.h"
#include "MetalMaterial.h"
#include "RefractMaterial.h"

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
	//Sphere* sphere1 = new Sphere(Vector3(0.0f, -0.5f, -1.0f), 0.4f); sphere1->material = new LambertMaterial(Color(0.8f, 0.3f, 0.5f, 1));
	//Sphere* sphere2 = new Sphere(Vector3(1.0f, -0.5f, -1.0f), 0.4f); sphere2->material = new MetalMaterial(Color(0.8f, 0.8f, 0.8f, 1));
	//Sphere* sphere3 = new Sphere(Vector3(1, 0, -1.0f), 0.9f); sphere3->material = new RefractMaterial(Color(0.8f, 0.8f, 0.8f, 1), 1.5);
	//Sphere* sphere5 = new Sphere(Vector3(1, 0, -2.0f), 0.8f); sphere5->material = new RefractMaterial(Color(0.8f, 0.8f, 0.8f, 1), 1.5);
	//Sphere* sphere6 = new Sphere(Vector3(1, 0, -2.0f), -0.75f); sphere6->material = new RefractMaterial(Color(0.8f, 0.8f, 0.8f, 1), 1.5);
	//Sphere* sphere4 = new Sphere(Vector3(0, 100.0f, -1.0f), 100); sphere4->material = new LambertMaterial(Color(0.8, 0.6, 0.2, 1));
	//hitableObjects.push_back(sphere4);
	//hitableObjects.push_back(sphere1);
	//hitableObjects.push_back(sphere2);
	//hitableObjects.push_back(sphere3);
	//hitableObjects.push_back(sphere5);
	//hitableObjects.push_back(sphere6);
	//camera.ori = Vector3(0.0f, -0.5f, 0.0f);

	Vector3 camPos = Vector3(2, 2, 3);
	Vector3 lookPos = Vector3(1, 0, 0);
	float focalDis = (camPos - lookPos).Magnitude();
	float aperture = 0.1;
	camera = new Camera(camPos, lookPos, Vector3(0, 1, 0), 60, float(deviceWidth) / float(deviceHeight), aperture, focalDis);

	InitObject();
}

void ApcDevice::InitObject()
{
	int sphereCount = 4;
	Sphere* ground = new Sphere(Vector3(0, 100.0f, -1.0f), 100); ground->material = new LambertMaterial(Color(0.8, 0.6, 0.2, 1));
	hitableObjects.push_back(ground);

	for (int i = 0; i < sphereCount; i++)
	{
		for (int j = 0; j < sphereCount; j++)
		{
			Sphere* sphere = new Sphere(Vector3(i * 2 - sphereCount, j * 2 - sphereCount, -1.0f), 0.5);
			Color color(Math::Random01(), Math::Random01(), Math::Random01(), 1.0);
			if ((int)(Math::Random01() * 100) % 2 == 0)
				sphere->material = new MetalMaterial(color);
			else
				sphere->material = new LambertMaterial(color);
			hitableObjects.push_back(sphere);
		}
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

Color ApcDevice::Render(const Ray& ray, int depth)
{
	HitResult result;
	if (HitDetect(ray, 0.001f, 10000.0f, result))
	{
		Ray scatterRay;
		Color attenuation;
		if (depth < depthCount && result.material->Scatter(ray, result, attenuation, scatterRay))
		{
			return Render(scatterRay, ++depth) * attenuation;
		}
		else
		{
			return Color(0, 0, 0, 0);
		}
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
			for (int k = 0; k < iteratorCount; k++)
			{
				float u = float(j + Math::Random01()) / float(deviceWidth);
				float v = float(i + Math::Random01()) / float(deviceHeight);

				Ray ray = camera->GetRay(u, v);
				color = color + Render(ray, 0);
			}
			color = color / iteratorCount;

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
			tempResult.material = (*it)->material;
			result = tempResult;
		}
	}
	return hit;
}