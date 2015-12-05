//************************************* Winmain.cpp *******************************************
// 
//				This source file contains important Win32 API functions.
//
//************************************* Winmain.cpp *******************************************


#include "Main.h"


GLuint g_Wnd;		  // Our global window variable used for lots of functions
GLvoid* g_Inst; // Global application instance variable
GLvoid* g_DC;
HGLRC g_RC;

GLboolean kp[256];   // Is key pressed?
GLboolean quitapp=false;

GLint FPS5=12; // Variable used for speed-optimize game

//////////////////////////////////////// KEYS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//							This function checks for key pressed.
//
//////////////////////////////////////// KEYS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLboolean keys(GLint keyid)
{
	if(GetKeyState(keyid) & 0x80) return true;
	else return false;
}

//////////////////////////////////////// ONEKEY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//					This function is used to check for a single key press.
//
//////////////////////////////////////// ONEKEY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLboolean onekey(GLint keyid)
{
	if(keys(keyid) && !kp[keyid]){kp[keyid]=true; return true;}
	if(!keys(keyid)){kp[keyid]=false; return false;}
	return false;
}

////////////////////////////// CHANGE TO FULLSCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function changes screen settings.
//
////////////////////////////// CHANGE TO FULLSCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLvoid ChangeToFullScreen()
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
	GLint result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	

	// Check if we didn't recieved a good return message From the function
	if(result != DISP_CHANGE_SUCCESSFUL)
	{
		// Display the error message and quit the program
		MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
		PostQuitMessage(0);
	}
}

// f- function that returns GLfloat depending on computer's speed

GLfloat f(GLfloat inc)
{
	return (float)(inc*12)/FPS5;
}

///////////////////////////////////// UPDATEFPS10 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//					This function updates frames per second each 100 ms.
//
///////////////////////////////////// UPDATEFPS10 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLvoid UpdateFPS5()
{
	static GLfloat last=GetTickCount();;
	GLfloat current=GetTickCount();
	static GLfloat fps;

	fps++;

	if(last+200<current)
	{
		last=GetTickCount();
		FPS5=fps;
		fps=0;
	}

}

////////////////////////////////// QUIT APPLICATION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Frees memory, all structures, variables...
//
////////////////////////////////// QUIT APPLICATION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLvoid QuitApplication()
{
	quitapp=true;
	if (g_RC)											
	{
		wglMakeCurrent(NULL, NULL);						// This frees our rendering memory and sets everything back to normal
		wglDeleteContext(g_RC);						// Delete our OpenGL Rendering Context	
	}
	
	if (g_DC) ReleaseDC(g_Wnd, g_DC);						// Release our GLvoid* from memory
		
	ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
	ShowCursor(TRUE);								// Show Mouse Pointer
	
	UnregisterClass(class_name, g_Inst);		// Free the window class
	PostQuitMessage (0);								// Post a QUIT message to the window
}

///////////////////////////////// REGISTER MY CLASS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Registers my own class with Windows OS.
//
///////////////////////////////// REGISTER MY CLASS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLvoid RegisterMyClass(const GLvoid* &hInst)
{
	WNDCLASSEX wcex; // This is our window class variable

	wcex.cbSize=sizeof(WNDCLASSEX); // Here we set its size
	wcex.cbClsExtra=0; // No extra class data
	wcex.cbWndExtra=0; // No extra window data

	wcex.hCursor=LoadCursor(NULL,IDC_ARROW); // Use default cursor                  
	wcex.hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1)); // Main application icon
	wcex.hIconSm=LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1)); // Icon that appears in top left corner of window
	wcex.lpszMenuName=NULL; // We're not using menu

	wcex.style=CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // This is drawing style- horizontal and vertical redraw
	wcex.hbrBackground=(HBRUSH) GetStockObject (WHITE_BRUSH); // This is background used for window using this class

	wcex.hInstance=hInst; // Class application instance
	wcex.lpfnWndProc=(WNDPROC)WndProc; // Function, that will handle messages for window using this class
	wcex.lpszClassName=class_name; // Name of the class

	RegisterClassEx(&wcex); // This function does all dirty work for us- registers initialized class
}

///////////////////////////////////// CREATE MY WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//					This function creates main application window.
//
///////////////////////////////////// CREATE MY WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLboolean CreateMyWindow(const GLvoid*& hInst, LPSTR title) 
{
	DWORD dwStyle= WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	

    g_Wnd= CreateWindow(class_name,title, dwStyle, 0, 0,
						800,600, 
						NULL, NULL, hInst, NULL);

	if (!g_Wnd)return FALSE; // If there was some error, return false
	
    ShowWindow(g_Wnd, SW_SHOWNORMAL); // This shows window created above
    UpdateWindow(g_Wnd); // And this updates it (???)
	ChangeToFullScreen();
	ShowCursor(false);
    g_Inst=hInst; // Set global instance variable to Windows registered instance

    return TRUE; // Everything happened succesful
}

//////////////////////////////////////// MAIN LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//			Main application loop, that runs from begin to end.
//
//////////////////////////////////////// MAIN LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*


GLuint MainLoop()
{
	MSG msg;

	while(1)											// Do our infinate loop
	{
	
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 	// Check if there was a message
        { 
			if(msg.message == WM_QUIT)					// If the message wasn't to quit
				break;
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else											// if there wasn't a message
		{
		if(quitapp)break;
		UpdateFPS5(); // Update FPS
		
		RenderScene(); // Render scene
        } 
	}
	QuitApplication();
	return msg.wParam;									// Return from the program
}

///////////////////////////////////////// WIN MAIN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//			Main application function. Gets called when you application runs.
//
///////////////////////////////////////// WIN MAIN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLint WINAPI WinMain(GLvoid* mainInst,GLvoid* prevInst,LPSTR command,GLint show)
{								
	RegisterMyClass(mainInst);						// Register class with Windows OS	
	if(!CreateMyWindow(mainInst,"Collisions in 3D world"))return false; // Create window and if something wasn't succesful, return false and quit app
	Init();	// Initialize whole application			
	

return MainLoop();	// Return what MainLoop returns

}

