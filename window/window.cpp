#include "stdafx.h"


bool running;

HWND hwnd;//Window handle
HDC hdc;//Device context handle
HGLRC hglrc;//OpenGL rendering context



//######################################################################################################
//##                                   START FUNCTION                                                 ##
//##This function does the initialization and calls message handling function when message is received##
//######################################################################################################
int WINAPI WinMain( HINSTANCE hInstance,    
                    HINSTANCE hPrevInstance,
                    LPSTR szCmdLine,
                    int iCmdShow )
{
	//############################################################
	//## Define a window type that we want to use. Register it. ##
	//############################################################

	WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hInstance = hInstance;
    wc.lpfnWndProc = MsgHandle;
    wc.lpszClassName = TEXT("GameWindow");
    wc.lpszMenuName = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass( &wc );

	hwnd = CreateWindow( // Set window to the center of the screen
        TEXT("GameWindow"),
        TEXT("JONgIne"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL,
        hInstance, NULL );
	if (!hwnd)
	{
		MessageBox(NULL,
			_T("CreateWindow failed!"),
			_T("Error!"),
			NULL);
		return 1;
	}

	// Remove unnecessary parts of window
	DWORD dwStyle = ::GetWindowLong(hwnd, GWL_STYLE);
	DWORD dwRemove = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	DWORD dwNewStyle = dwStyle & ~dwRemove;
	::SetWindowLong(hwnd, GWL_STYLE, dwNewStyle);
	::SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	//###################################
	//## Get device context for OpenGL ##
	//###################################
	hdc = GetDC(hwnd);

	if (hdc == NULL)
	{
		MessageBox(hwnd, _T("Failed to Get the Window Device Context"),
                         _T("Error!"), NULL);
		return 1;
	}
	//#####################################
	//## Create OpenGL rendering context ##
	//#####################################
	int indexPixelFormat = 0;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0,0,0,0,0,0,0,0,0,0,0,0,0, // useles parameters
		16,
		0,0,0,0,0,0,0
	};

	// Choose the closest pixel format available
	indexPixelFormat = ChoosePixelFormat(hdc, &pfd);

	// Set the pixel format for the provided window DC
	SetPixelFormat(hdc, indexPixelFormat, &pfd);

	if ((hglrc = wglCreateContext(hdc)) == NULL)
	{
		MessageBox(hwnd, _T("Failed to create rendering context!"),
			_T("Error!"), NULL);
		return 1;
	}
	//#########################################################
	//## Set the created rendering context as the active one ##
	//#########################################################
	if ((wglMakeCurrent(hdc, hglrc)) == false)
	{
		MessageBox(hwnd, _T("Failed to set current rendering context!"),
			_T("Error!"), NULL);
		return 1;
	}
	//###############
	//## Init Glew ##
	//###############
	bool err = glewInit();

	if(err != GLEW_OK)
	{
		std::cout << "Glew initialization failed!" << std::endl;
		return -1;
	}	
	//#####################################
	//## Init and renderer, Engine and UI.##
	//#####################################
	Renderer::get().init(hdc);
	Engine::get().init(&hwnd, ::GetDeviceCaps(hdc, HORZRES), ::GetDeviceCaps(hdc, VERTRES));
	UI::get().init(hdc);

	//###############################
	//## Open and draw the window ###
	//###############################
	ShowCursor(false);
	ShowWindow(hwnd, iCmdShow );

	//##################
	//## Start engine ##
	//##################
	Engine::get().start();

	// Release context
	wglMakeCurrent(hdc, NULL);	// release device context in use by rc
    wglDeleteContext(hglrc);	// delete rendering context

	return 0;
}
