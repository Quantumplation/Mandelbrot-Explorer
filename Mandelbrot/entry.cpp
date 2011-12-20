#include <Windows.h>

#include "mandelbrot.h"
#include "shader.h"

LRESULT CALLBACK WindowProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

int WINAPI WinMain( 
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) 
{
	HWND hWnd;
	WNDCLASSEX window_class;

	ZeroMemory(&window_class, sizeof(WNDCLASSEX));

    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = WindowProc;
    window_class.hInstance = hInstance;
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)COLOR_WINDOW;
    window_class.lpszClassName = "MandelbrotWindow";

    RegisterClassEx(&window_class);

	RECT window_rect = {0, 0, 800, 600};
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL,
                          "MandelbrotWindow",
                          "Mandelbrot Visualizer",
                          WS_OVERLAPPEDWINDOW,
						  300,
						  300,
						  window_rect.right - window_rect.left,
						  window_rect.bottom - window_rect.top,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);

    ShowWindow(hWnd, nCmdShow);

	MSG message = {0};

	Mandelbrot renderer(hWnd);
	renderer.CreateShader("mandelbrot", "mandelbrot.hlsl", "VShader",
		                    "PShader", "gradient.png", true);
	renderer.CreateSprite();
    while(TRUE)
	{
		if(PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
			if(message.message == WM_QUIT)
				break;
		}
		else
		{
			renderer.Draw();
		}
    }

    return message.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, 
						    LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}