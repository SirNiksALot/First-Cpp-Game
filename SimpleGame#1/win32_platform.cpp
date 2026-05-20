#include "utils.cpp"
#include <iostream>
#include <Windows.h>

bool running = true;

struct Render_state {
    int height, width;
    void* memory;
    BITMAPINFO bitmapinfo;

};

Render_state render_state;

#include "renderer.cpp" // including the renderer.cpp file here means it has access to the global render_state struct 


LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
    case WM_CLOSE:
    case WM_DESTROY: {
        running = false;
    }break; 
    case WM_SIZE: {
        RECT rect; //created rect struct
		GetClientRect(hwnd, &rect); // populates rect struct with the dimensions of the client window rectangle 
		render_state.width = rect.right - rect.left;
        render_state.height = rect.bottom - rect.top;

        int size = render_state.width * render_state.height * sizeof(unsigned int); // buffer required to draw/render pixels onto window 
        if (render_state.memory) {
            VirtualFree(render_state.memory, 0, MEM_RELEASE);// if this pointer / memory block is already in use , clear it 
        }
        render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // reserve and commit memory for the buffer

        render_state.bitmapinfo.bmiHeader.biSize = sizeof(render_state.bitmapinfo.bmiHeader);
        render_state.bitmapinfo.bmiHeader.biWidth = render_state.width;
        render_state.bitmapinfo.bmiHeader.biHeight = render_state.height;
        render_state.bitmapinfo.bmiHeader.biPlanes = 1;
        render_state.bitmapinfo.bmiHeader.biBitCount = 32;
        render_state.bitmapinfo.bmiHeader.biCompression = BI_RGB;

    }break;
    default: {
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    }
    return result;
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR     lpCmdLine, int       nShowCmd) {
	// create a window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Game window class";
	window_class.lpfnWndProc = window_callback;



	// register the window 
	 RegisterClass(&window_class);
	// create window 


     HWND hwnd = CreateWindowEx(
         0,                              // Optional window styles.
         L"Game window class",           // Window class
         L"Learn to Program Windows",    // Window text
         WS_OVERLAPPEDWINDOW  | WS_VISIBLE ,            // Window style

         // Size and position
         CW_USEDEFAULT, CW_USEDEFAULT, 1280 , 720 ,0 , 0 ,

         hInstance,  // Instance handle
         0        // Additional application data
     );

     if (hwnd == NULL)
     {
         return 0;
     }
     HDC hdc = GetDC(hwnd);




     while (running) {
         //input
         MSG msg;
         while (PeekMessage(&msg,hwnd,0,0,PM_REMOVE)) {
            TranslateMessage(&msg);
             DispatchMessage(&msg);
         }
         //simulate
         clear_screen(0xff5500);
         draw_rect(0,0, 20 , 20, 0x00ff22);
         
         //render
         StretchDIBits(
             hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height,
             render_state.memory, &render_state.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
     }
}