#include <windows.h>
#include "creatingButtons.h"
#include <stdbool.h>

int ROW_Y = 10;
int ROW_X = 10;


void createButtons(HWND hwnd) {

	char labels[] = "789+456-123xC0=/";
	HWND buttons[16];
	wchar_t buttonText[2]; // Wide character array to store a single character and null terminator
	int indent;

	for (int i = 0; i < 16; i++) { // TODO: Change from 16 to the string's length

		buttonText[0] = labels[i]; // Copy the character
		buttonText[1] = '\0'; // Null-terminate the string

		indent = i % 4;
		if (!indent) ROW_Y = ROW_Y + 40;

		buttons[i] = CreateWindow( // Button
			L"Button", // Predefined class; Unicode assumed
			buttonText, // Button text
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			ROW_X + indent * 40, ROW_Y, 30, 30, //x, y, width, height
			hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL); // Parent, menu, application instance, pointer
	}

}