#include "mainheader.h"

GLint iScrX[] = {320, 400, 640, 768, 800, 854, 1024, 1152, 1280, 1400, 1440, 1600, 1680, 1920, 2048, 2560};
GLint iScrY[] = {200, 240, 480, 576, 600, 720, 768, 854, 960, 1024, 1050, 1080, 1200, 1536, 1600, 2048};
GLint iStartRes = 0;
CApplication::DispMode dm;

//====================

GLvoid getModes(HWND hDialog, GLint iResID)
{
	DEVMODE dmSet; EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmSet);
	SendDlgItemMessage(hDialog, iResID, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hDialog, iResID, CB_INSERTSTRING, 0, (LPARAM)"Choose one");
	GLint iCur = 1;
	FOR(i, 16)FOR(j, 16)
	{
		dmSet.dmPelsWidth = iScrX[i];
		dmSet.dmPelsHeight = iScrY[j];
		dmSet.dmBitsPerPel = 16;
		if(ChangeDisplaySettings(&dmSet, CDS_TEST | CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
		{
			char strText[50]; sprintf(strText, "%dx%d", iScrX[i], iScrY[j]);
			SendDlgItemMessage(hDialog, iResID, CB_INSERTSTRING, iCur, (LPARAM)strText);
			iCur++;
		}
	}
	SendDlgItemMessage(hDialog, iResID, CB_SETCURSEL, 0, 0);
}

//======================

LRESULT CALLBACK dlgStartup(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch(uMessage)
	{
		case WM_INITDIALOG:
			InitCommonControls();
			getModes(hWnd, ID_VID_RES);
			SendDlgItemMessage(hWnd, ID_VID_BPP, CB_INSERTSTRING, 0, (LPARAM)"16");
			SendDlgItemMessage(hWnd, ID_VID_BPP, CB_INSERTSTRING, 1, (LPARAM)"24");
			SendDlgItemMessage(hWnd, ID_VID_BPP, CB_INSERTSTRING, 2, (LPARAM)"32");
			SendDlgItemMessage(hWnd, ID_VID_BPP, CB_SETCURSEL, 2, 0);
			CheckDlgButton(hWnd, ID_VID_FUL, 1);
			break;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			break;

		case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case ID_OK:
			{
				char strText[50]; GLint iCurSel = (int)SendDlgItemMessage(hWnd, ID_VID_RES, CB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hWnd, ID_VID_RES, CB_GETLBTEXT, iCurSel, (LPARAM)strText);
				if(!strcmp(strText, "Choose one"))
				{
					MessageBox(hWnd, "Please choose a resolution!", "Resolution", MB_ICONINFORMATION | MB_OK);
					break;
				}
				sscanf(strText, "%dx%d", &dm.iScrX, &dm.iScrY);
				iCurSel = (int)SendDlgItemMessage(hWnd, ID_VID_BPP, CB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hWnd, ID_VID_BPP, CB_GETLBTEXT, iCurSel, (LPARAM)strText);
				sscanf(strText, "%d", &dm.iBpp);
				dm.iFS = IsDlgButtonChecked(hWnd, ID_VID_FUL);
				iStartRes = 1;
				EndDialog(hWnd, 0);
				break;
			}

			case ID_CANCEL:
				EndDialog(hWnd, 0);
				break;
		}
		break;
	}
	return 0;
}