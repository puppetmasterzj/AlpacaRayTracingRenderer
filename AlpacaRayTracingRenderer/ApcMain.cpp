/*******************************************************************
 * \file ApcMain.cpp
 *
 * \author puppet_master
 * \date 2018/05/20
 * \https://blog.csdn.net/puppet_master
 * \萌萌哒羊驼渲染器(?ω?) 
 * \入口函数，包含窗口创建，FPS显示，设备创建，调用渲染，双缓冲
 *******************************************************************/
#include "Stdfx.h"
#include "ApcDevice.h"

static const int windowWidth = 600;
static const int windowHeight = 450;
const char* windowTitle = "Alpaca Ray-Tracing Renderer";
HDC hdc = NULL;
HDC screenHDC = NULL;
ApcDevice* device = NULL;
HWND handler = NULL;

static LRESULT OnEvent(HWND, UINT, WPARAM, LPARAM);
void CreateRenderDevice();
void CreateSystemWindow();
void Update();
void DoRender();
void ShowFPS();

int main()
{
	CreateSystemWindow();
	CreateRenderDevice();
	Update();
	return 0;
}

void CreateSystemWindow()
{
	WNDCLASS wndClass = { CS_BYTEALIGNCLIENT, (WNDPROC)OnEvent, 0, 0, 0, NULL, NULL, NULL, NULL, TEXT(windowTitle) };
	wndClass.hInstance = GetModuleHandle(NULL);

	if (!RegisterClass(&wndClass))
		return;

	handler = CreateWindow(TEXT(windowTitle), TEXT(windowTitle), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wndClass.hInstance, NULL);
	if (handler == NULL)
		return;

	hdc = GetDC((handler));
	screenHDC = CreateCompatibleDC(hdc);
	//ReleaseDC(handler, hdc);

	BITMAPINFO bitmapInfo = { { sizeof(BITMAPINFOHEADER),windowWidth, windowHeight, 1, 32, BI_RGB, windowWidth * windowHeight * 4, 0, 0, 0, 0 } };
	LPVOID ptr;
	//创建设备无关的位图
	HBITMAP bitmapHandler = CreateDIBSection(screenHDC, &bitmapInfo, DIB_RGB_COLORS, &ptr, 0, 0);
	if (bitmapHandler == NULL)
		return;

	HBITMAP screenObject = (HBITMAP)SelectObject(screenHDC, bitmapHandler);

	SetWindowPos(handler, NULL, 300, 200, windowWidth, windowHeight, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));

	ShowWindow(handler, SW_NORMAL);
	UpdateWindow(handler);
}

void CreateRenderDevice()
{
	device = new ApcDevice();
	device->InitDevice(screenHDC, windowWidth, windowHeight);
}

LRESULT OnEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Update()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DoRender();
			ShowFPS();
		}
	}
} 

void DoRender()
{
	device->Clear();
	device->DoRender();
	//双缓冲
	BitBlt(hdc, 0, 0, windowWidth, windowHeight, screenHDC, 0, 0, SRCCOPY);
}

//偷个懒，直接摘抄了一段浅墨大大的FPS计算代码
void ShowFPS()
{
	static float  fps = 0;
	static int    frameCount = 0;
	static float  currentTime = 0.0f;
	static float  lastTime = 0.0f;

	frameCount++;
	currentTime = timeGetTime()*0.001f;
							  
	if (currentTime - lastTime > 1.0f) 
	{
		fps = (float)frameCount / (currentTime - lastTime);  
		lastTime = currentTime; 
		frameCount = 0;
	}

	char strBuffer[20];
	sprintf_s(strBuffer, 20, "%0.3f", fps);
	TextOut(hdc, 0, 0, strBuffer, 6);
}
