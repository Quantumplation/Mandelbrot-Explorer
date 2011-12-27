#include <Windows.h>
#include "mandelbrot.h"
#include "shader.h"
#include "shader_parameters.h"
#include "animated_value.h"

struct MandelbrotParams
{
	float scale;
	float X, Y;
	float padding;
};

AnimatedValue<float> *scale;
AnimatedValue<float> *X, *Y;

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

	timeBeginPeriod(1);
	float scale_value, x_value, y_value;
	scale = new AnimatedValue<float>(&scale_value, 100, 2, 3000, CURVE_SMOOTH);
	X = new AnimatedValue<float>(&x_value, 0, 0, 500, CURVE_LINEAR);
	Y = new AnimatedValue<float>(&y_value, 0, 0, 500, CURVE_LINEAR);
	scale->Start();
	X->Start();
	Y->Start();

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
			if(scale->IsAnimating() || X->IsAnimating() || Y->IsAnimating())
			{
				scale->Update();
				X->Update();
				Y->Update();
				MandelbrotParams params = {scale->GetValue(), X->GetValue(), Y->GetValue(), 0.0f};
				renderer.SetShaderParameters<MandelbrotParams>("mandelbrot", params);
			}
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
		case WM_MOUSEWHEEL:
			{
				int clicks = GET_WHEEL_DELTA_WPARAM(wParam);
				float end_value = scale->GetEndValue();
				end_value *= pow(0.9, clicks/120);
				scale->Interrupt(end_value, 500);
			} break;
		case WM_KEYDOWN:
			{
				if(wParam == VK_LEFT)
				{
					float x_value = X->GetEndValue() - scale->GetValue() / 10;
					X->Interrupt(x_value, 500);
				}
				if(wParam == VK_RIGHT)
				{
					float x_value = X->GetEndValue() + scale->GetValue() / 10;
					X->Interrupt(x_value, 500);
				}
				if(wParam == VK_UP)
				{
					float y_value = Y->GetEndValue() + scale->GetValue() / 10;
					Y->Interrupt(y_value, 500);
				}
				if(wParam == VK_DOWN)
				{
					float y_value = Y->GetEndValue() - scale->GetValue() / 10;
					Y->Interrupt(y_value, 500);
				}
			} break;
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}
