// Intro3D.cpp : Defines the entry point for the application.
#include "stdafx.h"
#include "Software3D.h"
#include <crtdbg.h>

#include "SoftwareRenderer.h"
#include "RandomInt.h"

#define MAX_LOADSTRING 100

// Set initial window height and width
#define INITIAL_WINDOW_WIDTH 1024
#define INITIAL_WINDOW_HEIGHT 768

// Define object boundaries
#define OBJECT_BOUNDARY_WIDTH 1750
#define OBJECT_BOUNDARY_HEIGHT 1750
#define OBJECT_BOUNDARY_DEPTH 2000
#define TOTAL_OBJECTS 500

// Global Variables:
HINSTANCE hInst; // current instance
TCHAR szTitle[MAX_LOADSTRING]; // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name

HWND _renderWindow; // HWND for the main window to render to

RenderLibrary::SoftwareRenderer* _renderer; // Software Renderer object

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
std::string OpenFileDialog();

HMENU _mainMenu; // Reference for main menu

static MathLibrary::RandomInt _random; // Random number generator

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// Set a couple of flags to dump memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SOFTWARE3D, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Initialise GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup (&gdiplusToken, &gdiplusStartupInput, NULL);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOFTWARE3D));

	_mainMenu = GetMenu(_renderWindow); // Get a global reference for the main menu (useful later)

	// Initialise the Software Renderer
	_renderer = new RenderLibrary::SoftwareRenderer();
	_renderer->Initialise(&_renderWindow, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);

	// Set up the default render mode (GDI+, flat shading)
	_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_GDIPLUS, true);
	_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_BACKFACE, true);
	_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_FLAT_SHADING, true);

	// Set up a camera position that's not the origin
	_renderer->ManipulateCameraDirect(RenderLibrary::SoftwareRenderer::M_TRANSLATE, RenderLibrary::SoftwareRenderer::AXIS_Z, -4000);

	// Load some cubes
	int index;
	for(index = 0; index < TOTAL_OBJECTS; index++)
		_renderer->AddModel("Models\\cube.md2");

	// EVEN MORE DEBUG:
	for(index = 0; index < _renderer->GetNumberOfObjects(); index++)
	{
		/*_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_TRANSLATE, 0, RenderLibrary::SoftwareRenderer::AXIS_X, 0);
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_TRANSLATE, 0, RenderLibrary::SoftwareRenderer::AXIS_Y, 0);
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_TRANSLATE, 0, RenderLibrary::SoftwareRenderer::AXIS_Z, 0);
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_ROTATE, 0, RenderLibrary::SoftwareRenderer::AXIS_X, 0);
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_ROTATE, 0, RenderLibrary::SoftwareRenderer::AXIS_Y, 0);
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_ROTATE, 0, RenderLibrary::SoftwareRenderer::AXIS_Z, 0);*/

		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_TRANSLATE, index, RenderLibrary::SoftwareRenderer::AXIS_X, (float)_random.GetRangedRandomNumber(0, OBJECT_BOUNDARY_WIDTH));
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_TRANSLATE, index, RenderLibrary::SoftwareRenderer::AXIS_Y, (float)_random.GetRangedRandomNumber(0, OBJECT_BOUNDARY_HEIGHT));
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_TRANSLATE, index, RenderLibrary::SoftwareRenderer::AXIS_Z, (float)_random.GetRangedRandomNumber(0, OBJECT_BOUNDARY_DEPTH));
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_ROTATE, index, RenderLibrary::SoftwareRenderer::AXIS_X, (float)_random.GetRangedRandomNumber(0, 1000));
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_ROTATE, index, RenderLibrary::SoftwareRenderer::AXIS_Y, (float)_random.GetRangedRandomNumber(0, 1000));
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_ROTATE, index, RenderLibrary::SoftwareRenderer::AXIS_Z, (float)_random.GetRangedRandomNumber(0, 1000));
	}

	// Main message loop:
	while (true)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			// test if this is a quit
			if (msg.message == WM_QUIT)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		_renderer->Update();
		_renderer->Render();
	}

	_renderer->Shutdown();

	Gdiplus::GdiplusShutdown(gdiplusToken);

	// Dump memory leaks to the Output window
	_CrtDumpMemoryLeaks();

	return (int) msg.wParam;
}

//  FUNCTION: MyRegisterClass()
//  PURPOSE: Registers the window class.
//  COMMENTS: This function and its usage are only necessary if you want this code to be compatible with Win32 systems prior to the 'RegisterClassEx' function that was added to Windows 95. It is important to call this function so that the application will get 'well formed' small icons associated with it.

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOFTWARE3D));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SOFTWARE3D);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//   FUNCTION: InitInstance(HINSTANCE, int)
//   PURPOSE: Saves instance handle and creates main window
//   COMMENTS: In this function, we save the instance handle in a global variable and create and display the main program window.

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	// Create the renderer window
	_renderWindow = CreateWindow(TEXT("Software3D"), TEXT("Software 3D Renderer"), WS_OVERLAPPEDWINDOW, 20, 20, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	if (!_renderWindow)
	{
		return FALSE;
	}

	ShowWindow(_renderWindow, nCmdShow);
	UpdateWindow(_renderWindow);

	return TRUE;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//  PURPOSE:  Processes messages for the main window.
//		WM_COMMAND	- process the application menu
//		WM_PAINT	- Paint the main window
//		WM_DESTROY	- post a quit message and return

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static std::string filename;

	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// Parse the menu selections:
		switch (wmId)
		{
#pragma region Menu Items
	case IDM_ABOUT: // File -> About...
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT: // File -> Exit...
		DestroyWindow(hWnd);
		break;

	case ID_LIGHTS_ADDLIGHT: // Lights -> Add Directional Light (random direction / colour)...
		_renderer->AddDirectionalLight(MathLibrary::Vector((float)_random.GetRangedRandomNumber(-50, 50), (float)_random.GetRangedRandomNumber(-50, 50), (float)_random.GetRangedRandomNumber(-50, 50), (float)_random.GetRangedRandomNumber(-50, 50)), 
			RenderLibrary::Light::LightColour(_random.GetRangedRandomNumber(0, 75), _random.GetRangedRandomNumber(0, 75), _random.GetRangedRandomNumber(0, 75), _random.GetRangedRandomNumber(0, 75))); // Add light to light manager
		break;

	case ID_MODELS_ADDMODEL: // Models -> Add Model (random position / orientation / reflectivity)...
		// Show the file open dialog
		filename = "";
		filename = OpenFileDialog();

		// Handle errors gracefully
		if (filename.size() != 0) // Currently only handles 'cancel' from dialog
		{
			_renderer->AddModel(filename.c_str()); // Add / load model to object manager
		}

		// Set random position
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_TRANSLATE, _renderer->GetNumberOfObjects() - 1, RenderLibrary::SoftwareRenderer::AXIS_X, (float)_random.GetRangedRandomNumber(0, OBJECT_BOUNDARY_WIDTH));
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_TRANSLATE, _renderer->GetNumberOfObjects() - 1, RenderLibrary::SoftwareRenderer::AXIS_Y, (float)_random.GetRangedRandomNumber(0, OBJECT_BOUNDARY_HEIGHT));
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_TRANSLATE, _renderer->GetNumberOfObjects() - 1, RenderLibrary::SoftwareRenderer::AXIS_Z, (float)_random.GetRangedRandomNumber(0, OBJECT_BOUNDARY_DEPTH));

		// Set random orientation
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_ROTATE, _renderer->GetNumberOfObjects() - 1, RenderLibrary::SoftwareRenderer::AXIS_X, (float)_random.GetRangedRandomNumber(0, OBJECT_BOUNDARY_WIDTH));
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_ROTATE, _renderer->GetNumberOfObjects() - 1, RenderLibrary::SoftwareRenderer::AXIS_Y, (float)_random.GetRangedRandomNumber(0, OBJECT_BOUNDARY_HEIGHT));
		_renderer->ManipulateObjectDirect(RenderLibrary::SoftwareRenderer::M_ROTATE, _renderer->GetNumberOfObjects() - 1, RenderLibrary::SoftwareRenderer::AXIS_Z, (float)_random.GetRangedRandomNumber(0, OBJECT_BOUNDARY_DEPTH));

		// Set random reflectivity


		// Resume rendering
		break;

	case ID_GDI_ENABLED: // Mode -> GDI+ -> Enabled
		if (_renderer->F_GDIPLUS == false) // Off
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_GDIPLUS, true); // Set the render mode to on
			CheckMenuItem(_mainMenu, ID_DIRECT2D_ENABLED, MF_UNCHECKED); // Set the menu item to unchecked
			CheckMenuItem(_mainMenu, ID_GDI_ENABLED, MF_CHECKED); // Set the GDI+ menu item to checked
		}
		else if (_renderer->F_GDIPLUS == true) // On
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_GDIPLUS, false); // Set the render mode to off
			CheckMenuItem(_mainMenu, ID_DIRECT2D_ENABLED, MF_CHECKED); // Set the menu item to checked
			CheckMenuItem(_mainMenu, ID_GDI_ENABLED, MF_UNCHECKED); // Set the GDI+ menu item to unchecked
		}
		break; // Resume rendering

	case ID_DIRECT2D_ENABLED: // Mode -> Direct2D -> Enabled
		if (_renderer->F_GDIPLUS == false) // Off
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_GDIPLUS, true); // Set the render mode to on
			CheckMenuItem(_mainMenu, ID_DIRECT2D_ENABLED, MF_UNCHECKED); // Set the menu item to unchecked
			CheckMenuItem(_mainMenu, ID_GDI_ENABLED, MF_CHECKED); // Set the GDI+ menu item to checked
		}
		else if (_renderer->F_GDIPLUS == true) // On
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_GDIPLUS, false); // Set the render mode to off
			CheckMenuItem(_mainMenu, ID_DIRECT2D_ENABLED, MF_CHECKED); // Set the menu item to checked
			CheckMenuItem(_mainMenu, ID_GDI_ENABLED, MF_UNCHECKED); // Set the GDI+ menu item to unchecked
		}
		break; // Resume rendering

	case ID_WIREFRAME_ENABLED: // Mode -> GDI+ -> Wire frame -> Enabled						
		if (_renderer->F_WIREFRAME == false) // Off
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_WIRE_FRAME, true); // Set the render mode to on
			CheckMenuItem(_mainMenu, ID_WIREFRAME_ENABLED, MF_CHECKED); // Set the menu item to checked
		}
		else if (_renderer->F_WIREFRAME == true) // On
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_WIRE_FRAME, false); // Set the render mode to off
			CheckMenuItem(_mainMenu, ID_WIREFRAME_ENABLED, MF_UNCHECKED); // Set the menu item to unchecked
		}
		break; // Resume rendering
	case ID_WIREFRAME_TEXTPOINTS_ENABLED: // Mode -> GDI+ -> Wireframe -> Text Points
		if (_renderer->F_TEXT_POINTS == false) // Off
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_TEXT_POINTS, true); // Set the render mode to on
			CheckMenuItem(_mainMenu, ID_WIREFRAME_TEXTPOINTS_ENABLED, MF_CHECKED); // Set the menu item to checked
		}
		else if (_renderer->F_TEXT_POINTS == true) // On
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_TEXT_POINTS, false); // Set the render mode to off
			CheckMenuItem(_mainMenu, ID_WIREFRAME_TEXTPOINTS_ENABLED, MF_UNCHECKED); // Set the menu item to unchecked
		}
		break; // Resume rendering

	case ID_GDI_FLATSHADING_ENABLED: // Mode -> GDI+ -> Flat Shading -> Enabled
		if (_renderer->F_FLATSHADING == false) // Off
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_FLAT_SHADING, true); // Set the render mode to on
			CheckMenuItem(_mainMenu, ID_GDI_FLATSHADING_ENABLED, MF_CHECKED); // Set the menu item to checked
		}
		else if (_renderer->F_FLATSHADING == true) // On
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_FLAT_SHADING, false); // Set the render mode to off
			CheckMenuItem(_mainMenu, ID_GDI_FLATSHADING_ENABLED, MF_UNCHECKED); // Set the menu item to unchecked
		}
		break; // Resume rendering

	case ID_GDI_BACKFACECULLING_ENABLED: // Mode -> GDI+ -> Backface Culling -> Enabled
		if (_renderer->F_BACKFACE == false) // Off
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_BACKFACE, true); // Set the render mode to on
			CheckMenuItem(_mainMenu, ID_GDI_BACKFACECULLING_ENABLED, MF_CHECKED); // Set the menu item to checked
		}
		else if (_renderer->F_BACKFACE == true) // On
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_BACKFACE, false); // Set the render mode to off
			CheckMenuItem(_mainMenu, ID_GDI_BACKFACECULLING_ENABLED, MF_UNCHECKED); // Set the menu item to unchecked
		}
		break; // Resume rendering

	case ID_GDI_SCANLINE_ENABLED: // Mode -> GDI+ Scanline -> Enabled
		if (_renderer->F_SCANLINE == false) // Off
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_SCANLINE, true); // Set the render mode to on
			CheckMenuItem(_mainMenu, ID_GDI_SCANLINE_ENABLED, MF_CHECKED); // Set the menu item to checked
		}
		else if (_renderer->F_SCANLINE == true) // On
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_SCANLINE, false); // Set the render mode to off
			CheckMenuItem(_mainMenu, ID_GDI_SCANLINE_ENABLED, MF_UNCHECKED); // Set the menu item to unchecked
		}
		break; // Resume rendering

	case ID_GDI_GOURAUDSHADING_ENABLED: // Mode -> GDI+ -> Scanline -> Gouraud Shading -> Enabled
		if (_renderer->F_GOURAUD == false) // Off
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_GOURAUD, true); // Set the render mode to on
			CheckMenuItem(_mainMenu, ID_GDI_GOURAUDSHADING_ENABLED, MF_CHECKED); // Set the menu item to checked
		}
		else if (_renderer->F_GOURAUD == true) // On
		{
			_renderer->ChangeRenderMode(RenderLibrary::SoftwareRenderer::R_GOURAUD, false); // Set the render mode to off
			CheckMenuItem(_mainMenu, ID_GDI_GOURAUDSHADING_ENABLED, MF_UNCHECKED); // Set the menu item to unchecked
		}
		break; // Resume rendering

	case ID_LIGHTS_REMOVELAST:
		_renderer->RemoveLastLight();
		break;

	case ID_MODELS_REMOVELAST:
		_renderer->RemoveLastModel();
		break;

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
#pragma endregion
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// TODO: Add any drawing code here...

		EndPaint(hWnd, &ps);
		break;
	case WM_SIZING:
		if (hWnd == _renderWindow)
		{
			_renderer->WindowSizeChanged(&_renderWindow);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Returns an empty string if dialog is canceled
std::string OpenFileDialog() 
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "MD2 Models (*.md2)\0*.md2\0";
	ofn.lpstrFile = TEXT(fileName);
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	std::string fileNameStr;

	if ( GetOpenFileName(&ofn) )
		fileNameStr = fileName;

	return fileNameStr;
}
