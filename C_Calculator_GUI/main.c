#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

#include <windows.h>
#include <stdio.h>
#include "tinyexpr.h"
#include "resource.h"

int ROW_Y = 50;
int ROW_X = 20;

wchar_t result[64];

HWND hwndEdit; // Global variable for text

HWND buttons[16];

wchar_t* expr;
char* nexpr;

void CenterWindow(HWND hwnd) {
	RECT rcClient, rcWindow;

	GetClientRect(GetDesktopWindow(), &rcClient);
	GetWindowRect(hwnd, &rcWindow);

	int xPos = (rcClient.right - (rcWindow.right - rcWindow.left)) / 2;
	int yPos = (rcClient.bottom - (rcWindow.bottom - rcWindow.top)) / 2;

	SetWindowPos(hwnd, NULL, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void AppendTextToEdit(HWND hwndEdit, const wchar_t* newText) {
	// Get the current length of text in the edit control
	int len = GetWindowTextLength(hwndEdit);

	// Allocate buffer to hold the current text plus the new text plus null terminator
	wchar_t* buffer = (wchar_t*)malloc((len + wcslen(newText) + 1) * sizeof(wchar_t));

	// Get the current text
	GetWindowText(hwndEdit, buffer, len + 1);

	// Append the new text
	wcscat_s(buffer, len + wcslen(newText) + 1, newText);

	// Set the updated text back to the edit control
	SetWindowText(hwndEdit, buffer);

	// Free the buffer
	free(buffer);
}


void createButtons(HWND hwnd) {

	char labels[] = "789+456-123xC0=/";
	wchar_t buttonText[2]; // Wide character array to store a single character and null terminator
	int indent;

	for (int i = 0; i < 16; i++) {

		buttonText[0] = labels[i]; // Copy the character
		buttonText[1] = '\0'; // Null-terminate the string

		indent = i % 4;
		if (!indent) ROW_Y = ROW_Y + 75;

		buttons[i] = CreateWindow( // Button
			L"Button", // Predefined class; Unicode assumed
			buttonText, // Button text
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON,  // Styles 
			ROW_X + indent * 75, ROW_Y, 60, 60, //x, y, width, height
			hwnd, (HMENU)(BTN_ID_7 + i), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL); // Parent, menu, application instance, pointer

		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_7ICON+i));
		SendMessage(buttons[i], BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

	}

}

const char g_szClassName[] = "myWindowClass"; // step 1.1 Registering the Window Class - the name of our window class
HWND g_hToolbar = NULL;

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_INITDIALOG:

		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hbrBackground = NULL;
	switch (msg)
	{

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case BTN_ID_7:
			AppendTextToEdit(hwndEdit, L"7");
			break;
		case BTN_ID_8:
			AppendTextToEdit(hwndEdit, L"8");
			break;
		case BTN_ID_9:
			AppendTextToEdit(hwndEdit, L"9");
			break;
		case BTN_ID_P:
			AppendTextToEdit(hwndEdit, L"+");
			break;
		case BTN_ID_4:
			AppendTextToEdit(hwndEdit, L"4");
			break;
		case BTN_ID_5:
			AppendTextToEdit(hwndEdit, L"5");
			break;
		case BTN_ID_6:
			AppendTextToEdit(hwndEdit, L"6");
			break;
		case BTN_ID_M:
			AppendTextToEdit(hwndEdit, L"-");
			break;
		case BTN_ID_1:
			AppendTextToEdit(hwndEdit, L"1");
			break;
		case BTN_ID_2:
			AppendTextToEdit(hwndEdit, L"2");
			break;
		case BTN_ID_3:
			AppendTextToEdit(hwndEdit, L"3");
			break;
		case BTN_ID_x:
			AppendTextToEdit(hwndEdit, L"*");
			break;
		case BTN_ID_C:
			SetWindowText(hwndEdit, "");
			break;
		case BTN_ID_0:
			AppendTextToEdit(hwndEdit, L"0");
			break;
		case BTN_ID_E:
			expr = (wchar_t*)malloc((GetWindowTextLength(hwndEdit) + 1) * sizeof(wchar_t));
			GetWindowText(hwndEdit, expr, GetWindowTextLength(hwndEdit) + 1);
			nexpr = (char*)malloc((wcstombs(NULL, expr, 0) + 1) * sizeof(char));
			wcstombs(nexpr, expr, (wcstombs(NULL, expr, 0) + 1));

			// Evaluate the expression and get the result
			double evalResult = te_interp(nexpr, 0);

			if (evalResult == (int)evalResult) {
				// Format the result with to int
				swprintf(result, sizeof(result) / sizeof(wchar_t), L"%d", (int)evalResult);
			}
			else {
				// Format the result with exactly 10 digits after the decimal point
				swprintf(result, sizeof(result) / sizeof(wchar_t), L"%.10f", evalResult);
				// Remove trailing zeros
				int len = wcslen(result);
				while (len > 0 && (result[len - 1] == L'0' || result[len - 1] == L'.')) {
					if (result[len - 1] == L'.') {
						result[len - 1] = L'\0'; // Remove the decimal point and stop
						break;
					}
					result[--len] = L'\0'; // Remove trailing zero
				}	
			}

			// Set the formatted result in the edit control
			SetWindowText(hwndEdit, result);

			// Free allocated memory
			free(expr);
			free(nexpr);
			break;
		case BTN_ID_D:
			AppendTextToEdit(hwndEdit, L"/");
			break;
		case ID_FILE_EXIT:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case ID_DIALOG_SHOW:
			ShowWindow(g_hToolbar, SW_SHOW);
			break;
		case ID_DIALOG_HIDE:
			ShowWindow(g_hToolbar, SW_HIDE);
			break;
			//... 
		case ID_HELP_ABOUT:
		{
			int ret = DialogBox(GetModuleHandle(NULL),
				MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
			if (ret == -1) {
				MessageBox(hwnd, L"Dialog failed!", L"Error",
					MB_OK | MB_ICONINFORMATION);
			}
		}
		break;
		}
		break;

	case WM_CREATE:
		CenterWindow(hwnd);
		hbrBackground = CreateSolidBrush(RGB(44, 44, 44));
		break;

	case WM_ERASEBKGND:
	{
		HDC hdc = (HDC)wParam;
		RECT rect;
		GetClientRect(hwnd, &rect);
		FillRect(hdc, &rect, hbrBackground);
		return 1; // Indicate that background is erased
	}

	case WM_CLOSE: // WM_CLOSE is sent when the user presses the Close Button
		DestroyWindow(hwnd); // the system sends the WM_DESTROY message to the window getting destroyed
		break;
	case WM_DESTROY: // destroys any remaining child windows before finally removing our window from the system
		if (hbrBackground) {
			DeleteObject(hbrBackground); // Clean up the brush
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}


/*
HINSTANCE hInstance - Handle to the programs executable module (the .exe file in memory)
HINSTANCE hPrevInstance - Always NULL for Win32 programs.
LPSTR lpCmdLine - The command line arguments as a single string. NOT including the program name.
int nCmdShow - An integer value which may be passed to ShowWindow().
*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) // equivalent of main()
{
	WNDCLASSEX wc; //step 1.2 Registering the Window Class
	HWND hwnd;
	MSG Msg;

	//step 1.3 Registering the Window Class - to register our window class

	wc.cbSize = sizeof(WNDCLASSEX); //The size of the structure.
	wc.style = 0; //Class Styles (CS_*), not to be confused with Window Styles (WS_*) This can usually be set to 0.
	wc.lpfnWndProc = WndProc; //Pointer to the window procedure for this window class.
	wc.cbClsExtra = 0; //Amount of extra data allocated for this class in memory. Usually 0.
	wc.cbWndExtra = 0; //Amount of extra data allocated in memory per window of this type. Usually 0.
	wc.hInstance = hInstance; //Handle to application instance 
	//wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON)); //Large (usually 32x32) icon shown when the user presses Alt+Tab.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Cursor that will be displayed over our window.
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //Background Brush to set the color of our window.
	//wc.lpszMenuName  = NULL;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU); //Name of a menu resource to use for the windows with this class.
	wc.lpszClassName = g_szClassName; //Name to identify the class with.
	//wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0); //Small (usually 16x16) icon to show in the taskbar and in the top left corner of the window.

	// check if window registration failed
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	// step 2 - creating the window

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE, //the extended windows style, later try 0
		g_szClassName, //what kind of window to create
		L"Calculator", //window name (Read more about it)
		WS_OVERLAPPEDWINDOW, // the Window Style parameter
		CW_USEDEFAULT, CW_USEDEFAULT, 350, 1.618*320, //x,y,width,height (in pixels)
		NULL, NULL, hInstance, NULL); //the Parent Window handle, the menu handle, the application instance handle, and a pointer to window creation data.
	
	hwndEdit = CreateWindowEx( // Text
		WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL, // Add ES_BOTTOM style for bottom alignment
		25, 20, 280, 50,
		hwnd, NULL, NULL, NULL);

	// Set a new font for the edit control
	HFONT hFont = CreateFont(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Calibri");

	SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

	createButtons(hwnd);

	// check if CreateWindow() failed
	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	ShowWindow(hwnd, nCmdShow); //nCmdShow - choice for window minimized, maximized, etc.
	UpdateWindow(hwnd);

	// step 3 - the message loop

	while (GetMessage(&Msg, NULL, 0, 0) > 0) // messages from the system (clicks, keyboard type etc.) to the program's message queue
	{
		if (!IsDialogMessage(g_hToolbar, &Msg))
		{
			TranslateMessage(&Msg);// some additional processing on keyboard events like generating WM_CHAR messages to go along with WM_KEYDOWN messages
			DispatchMessage(&Msg);// sends the message out to the window that the message was sent to.
		}
	}
	return Msg.wParam;
}