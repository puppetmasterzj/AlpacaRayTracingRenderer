/*******************************************************************
 * \file ApcDevice.h
 *
 * \author puppet_master
 * \date 2018/05/20���ע��
 * \https://blog.csdn.net/puppet_master
 * \��������Ⱦ��AlpacaSoftRenderer����Ҫʵ��
 * \����MVP�������ɣ��ӿ�ӳ�䣬�򵥲ü�����Ȳ��ԣ���դ�������ݲ�ֵ
 * \�������ֲ��Ժ�����������2D����
 *******************************************************************/
#pragma once
#include "Stdfx.h"
#include "Color.h"

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
	
};

