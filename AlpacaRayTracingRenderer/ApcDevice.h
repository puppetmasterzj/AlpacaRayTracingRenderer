/*******************************************************************
 * \file ApcDevice.h
 *
 * \author puppet_master
 * \date 2018/05/20添加注释
 * \https://blog.csdn.net/puppet_master
 * \羊驼软渲染器AlpacaSoftRenderer的主要实现
 * \包含MVP矩阵生成，视口映射，简单裁剪，深度测试，光栅化，数据插值
 * \包含部分测试函数，基本的2D绘制
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

