#include <windows.h>
#include "creatingButtons.h"

int createButtons(HWND hwnd){
    HWND hwndButton1 = CreateWindow( // Button
        L"Button", // Predefined class; Unicode assumed
        L"1", // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        10, 40, 30, 30, //x, y, width, height
        hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL); // Parent, menu, application instance, pointer

}