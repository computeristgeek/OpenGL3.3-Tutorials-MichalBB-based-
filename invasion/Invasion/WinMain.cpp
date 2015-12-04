//************************************* Winmain.cpp *******************************************
// 
//				This source file contains important Win32 API functions.
//
//************************************* Winmain.cpp *******************************************


#include "Main.h"
#include "resource.h"

HWND g_Wnd;		  // Our global window variable used for lots of functions
HINSTANCE g_Inst; // Global application instance variable
HDC g_DC;
HGLRC g_RC;

////////////////////////////// CHANGE TO FULLSCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function changes screen settings.
//
////////////////////////////// CHANGE TO FULLSCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void ChangeToFullScreen()
{
	DEVMODE dmSettings;									// Device Mode variable

	memset(&dmSettings,0,sizeof(dmSettings));			// Makes Sure Memory's Cleared

	// Get current settings -- This function fills our the settings
	// This makes sure NT and Win98 machines change correctly
	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
	{
		// Display error message if we couldn't get display settings
		MessageBox(NULL, "Could Not Enum Display Settings", "Error", MB_OK);
		return;
	}

	dmSettings.dmPelsWidth	= 800;				// Selected Screen Width
	dmSettings.dmPelsHeight	= 600;			// Selected Screen Height
	
	// This function actually changes the screen to full screen
	// CDS_FULLSCREEN Gets Rid Of Start Bar.
	// We always want to get a result from this function to check if we failed
	int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	

	// Check if we didn't recieved a good return message From the function
	if(result != DISP_CHANGE_SUCCESSFUL)
	{
		// Display the error message and quit the program
		MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
		PostQuitMessage(0);
	}
}

////////////////////////////////// QUIT APPLICATION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Frees memory, all structures, variables...
//
////////////////////////////////// QUIT APPLICATION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void QuitApplication()
{
		if (g_RC)											
	{
		wglMakeCurrent(NULL, NULL);						// This frees our rendering memory and sets everything back to normal
		wglDeleteContext(g_RC);						// Delete our OpenGL Rendering Context	
	}
	
	if (g_DC) ReleaseDC(g_Wnd, g_DC);						// Release our HDC from memory
		
	ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
	ShowCursor(TRUE);								// Show Mouse Pointer
	
	UnregisterClass("Megabyte Softworks", g_Inst);		// Free the window class
	PostQuitMessage (0);								// Post a QUIT message to the window
}

///////////////////////////////// REGISTER MY CLASS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Registers my own class with Windows OS.
//
///////////////////////////////// REGISTER MY CLASS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RegisterMyClass(const HINSTANCE &hInst)
{
	WNDCLASSEX wcex; // This is our window class variable

	wcex.cbSize=sizeof(wcex); // Here we set its size
	wcex.cbClsExtra=0; // No extra class data
	wcex.cbWndExtra=0; // No extra window data

	wcex.hCursor=LoadCursor(NULL,IDC_ARROW); // Use default cursor                  
	wcex.hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1)); // Main application icon
	wcex.hIconSm=LoadIcon(NULL,IDI_WINLOGO); // Icon that appears in top left corner of window
	wcex.lpszMenuName=NULL; // We're not using menu

	wcex.style=CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // This is drawing style- horizontal and vertical redraw
	wcex.hbrBackground=(HBRUSH) GetStockObject (WHITE_BRUSH); // This is background used for window using this class

	wcex.hInstance=hInst; // Class application instance
	wcex.lpfnWndProc=(WNDPROC)WndProc; // Function, that will handle messages for window using this class
	wcex.lpszClassName="Megabyte Softworks"; // Name of the class

	RegisterClassEx(&wcex); // This function does all dirty work for us- registers initialized class
}

///////////////////////////////////// CREATE MY WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//					This function creates main application window.
//
///////////////////////////////////// CREATE MY WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CreateMyWindow(const HINSTANCE& hInst) 
{

	DWORD dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	
	ShowCursor(FALSE);	

	

    g_Wnd= CreateWindow("Megabyte Softworks","Invasion", dwStyle, 0, 0,
						800,600, 
						NULL, NULL, hInst, NULL);

	if (!g_Wnd)return FALSE; // If there was some error, return false
	
    ShowWindow(g_Wnd, SW_SHOWNORMAL); // This shows window created above
    UpdateWindow(g_Wnd); // And this updates it (???)
	ChangeToFullScreen();
    g_Inst=hInst; // Set global instance variable to Windows registered instance

    return TRUE; // Everything happened succesful
}

//////////////////////////////////////// MAIN LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//			Main application loop, that runs from begin to end.
//
//////////////////////////////////////// MAIN LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

WPARAM MainLoop()
{
	MSG msg;
	while(1)											// Do our infinate loop
	{													// Check if there was a message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(msg.message == WM_QUIT)					// If the message wasn't to quit
				break;
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else											// if there wasn't a message
		{ 
		switch(game)
		{
		case 0:
			RenderMenu();
			break;
		case 1:
			RenderScene();
		break;
		case 2:
			Result();
			break;

		}
        } 
	}
	
	return msg.wParam;									// Return from the program
}

///////////////////////////////////////// WIN MAIN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//			Main application function. Gets called when you application runs.
//
///////////////////////////////////////// WIN MAIN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int WINAPI WinMain(HINSTANCE mainInst,HINSTANCE prevInst,LPSTR command,int show)
{								
	RegisterMyClass(mainInst);						// Register class with Windows OS	
	if(!CreateMyWindow(mainInst))return false; // Create window and if something wasn't succesful, return false and quit app
	Init();	// Initialize whole application			
	

return MainLoop();	// Return what MainLoop returns


}

