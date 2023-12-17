#include <windows.h>
#include "resource.h"
#include "creatingButtons.h"

const char g_szClassName[] = "myWindowClass"; // step 1.1 Registering the Window Class - the name of our window class
HWND g_hToolbar = NULL;

// Adding a dialog box with DialogBox()
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

// Adding a dialog box with CreateDialog()
BOOL CALLBACK ToolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_PRESS:
            MessageBox(hwnd, L"Hi!", L"This is a message",
                MB_OK | MB_ICONEXCLAMATION);
            //EndDialog(hwnd, IDC_PRESS);
            break;
        case IDC_OTHER:
            MessageBox(hwnd, L"Bye!", L"This is also a message",
                MB_OK | MB_ICONEXCLAMATION);
            //EndDialog(hwnd, IDC_OTHER);
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
    switch(msg)
    {
        case WM_CREATE:
        g_hToolbar = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR),
            hwnd, ToolDlgProc);
        if (g_hToolbar != NULL)
        {
            ShowWindow(g_hToolbar, SW_SHOW);
        }
        else
        {
            MessageBox(hwnd, L"CreateDialog returned NULL", L"Warning!",
                MB_OK | MB_ICONINFORMATION);
        }
        break;
        case WM_COMMAND:
        switch (LOWORD(wParam))
        {
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
                if (ret == IDOK) {
                    MessageBox(hwnd, L"Dialog exited with IDOK.", L"Notice",
                        MB_OK | MB_ICONINFORMATION);
                }
                else if (ret == IDCANCEL) {
                    MessageBox(hwnd, L"Dialog exited with IDCANCEL.", L"Notice",
                        MB_OK | MB_ICONINFORMATION);
                }
                else if (ret == -1) {
                    MessageBox(hwnd, L"Dialog failed!", L"Error",
                        MB_OK | MB_ICONINFORMATION);
                }
            }
            break;
        // Other menu commands...
        }
        break;
        
        case WM_LBUTTONDOWN:
        {
            char szFileName[MAX_PATH];
            HINSTANCE hInstance = GetModuleHandle(NULL);

            GetModuleFileName(hInstance, szFileName, MAX_PATH);
            MessageBox(hwnd, szFileName, L"This program is:", MB_OK | MB_ICONINFORMATION);
        }
        break;
        case WM_RBUTTONDOWN:
            MessageBox(hwnd, L"Right Mouse Clicked", L"Message", MB_OK | MB_ICONINFORMATION);
            break;
        
        case WM_CLOSE: // WM_CLOSE is sent when the user presses the Close Button
            DestroyWindow(hwnd); // the system sends the WM_DESTROY message to the window getting destroyed
        break;
        case WM_DESTROY: // destroys any remaining child windows before finally removing our window from the system
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
        L"The title of my window", //window name (Read more about it)
        WS_OVERLAPPEDWINDOW, // the Window Style parameter
        CW_USEDEFAULT, CW_USEDEFAULT, 360, 160, //x,y,width,height (in pixels)
        NULL, NULL, hInstance, NULL); //the Parent Window handle, the menu handle, the application instance handle, and a pointer to window creation data.
   
    HWND hwndEdit = CreateWindowEx( // Text
        WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT("test"),
        WS_CHILD | WS_VISIBLE, 
        100, 20, 140, 20, 
        hwnd, NULL, NULL, NULL);
    
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