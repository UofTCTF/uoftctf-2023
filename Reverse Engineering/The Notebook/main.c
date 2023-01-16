//#define UNICODE // For the sake of the CTF, encode strings with ASCII

#define OEMRESOURCE // For OEM cursors
#include <windows.h>
#include <CommCtrl.h> // For listbox window class macro

#include "resource.h" // Resource ID macros

#define ID_TIMER_DEBUG 1

#define WIN_WIDTH 600
#define WIN_HEIGHT 800

struct NoteEntry {
	CHAR *name;
	CHAR *secret; // Encoded content (flag)
	int secLen; // Length of secret string
};

int listItemsTop = 0;
struct NoteEntry *listItems;

HFONT hFont; // Font for all windows
HWND hPassEdit; // Password edit field

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDialog(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	MSG msg;
	HWND hWnd;
	WNDCLASSA wc = {0};
	
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//wc.cbClsExtra = 0; // No need for extra bytes
    //wc.cbWndExtra = 0;
	wc.lpszClassName = "NoteLock";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName = MAKEINTRESOURCEA(ID_MENU);
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadImageA(NULL, MAKEINTRESOURCEA(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE);
	wc.hIcon = LoadImageA(hInstance, MAKEINTRESOURCEA(ID_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	
	RegisterClassA(&wc);
	
	hWnd = CreateWindowA(
		wc.lpszClassName,
		"NoteLock",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE,  // WS_VISIBLE not needed (Since we call ShowWindow)
		CW_USEDEFAULT, CW_USEDEFAULT, WIN_WIDTH, WIN_HEIGHT, // Default size and position
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	// Error checking
	if (hWnd == NULL) {
		return GetLastError();
	}
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	while(GetMessage(&msg, NULL, 0, 0) > 0) { // According to windows - ALSO check http://www.winprog.org/tutorial/message_loop.html
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_CREATE: ;
			// Register any hotkeys, load resources (bitmaps, etc.)
			// Create all child windows, controls, etc.
			
			// Create font struct to pass to all child windows with text (ListBox, Static)
			hFont = CreateFontA(
				20,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				ANSI_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				FF_DONTCARE,
				"MS Shell Dlg");
			
			HWND hListTitle = CreateWindowA(
				WC_STATIC,
				"ListBoxTitle",
				SS_CENTER | WS_VISIBLE | WS_CHILD,
				20, 0, WIN_WIDTH/2 - 40, 20,
				hWnd,
				NULL,
				GetModuleHandle(NULL),
				NULL
			);
			
			SetWindowTextA(hListTitle, "Note selection");
			//SendMessage(hListTitle, WM_SETFONT, (WPARAM) hFont, (LPARAM) 1);
			
			HWND hLB = CreateWindowExA(
				WS_EX_LEFT, // Default
				WC_LISTBOX,
				"NoteSelect",
				LBS_NOTIFY | LBS_STANDARD | WS_CHILD | WS_VISIBLE,
				20, 20, WIN_WIDTH/2 - 40, WIN_HEIGHT - 100, // 100 > caption + menu height
				hWnd,
				NULL,
				GetModuleHandle(NULL),
				NULL
			);
			//SendMessage(hLB, WM_SETFONT, (WPARAM) hFont, (LPARAM) 1);
			
			// Populate listbox with init items:
			listItems = (struct NoteEntry *)malloc(3*sizeof(struct NoteEntry));
			
			
			
			listItems[0].name = "My deepest darkest secret";
			listItems[0].secret = "\x2e\x3c\x14\x76\x33\xee\xf4\x31\xb9\x3f\x6\x78\x20\x19\x39\xa2\x26\x1a\xe0\x25\xe2\x35\x8\x30\x50";
			listItems[0].secLen = 25;
			listItems[1].name = "My darkest secret";
			listItems[1].secret = "\x2e\x3c\x14\x25\x30\xa2\xf4\x24\xfc\x76\x18\x21\x74\x2\x39\xe1\x24\x1e\xe7\x76\xfb\x35\xe\x31\x2\x78\x7b\xac";
			listItems[1].secLen = 28;
			listItems[2].name = "My secret";
			listItems[2].secret = "\x2e\x3c\x14\x25\x30\xa2\xe6\x33\xfa\x24\x10\x2c\x27\x51\x2b\xe7\x76\x13\xf2\x24\xf1\x39\x15\x30\x14\x32\x75\xb8\xe6\x3d\xec\x3a\x19\x62";
			listItems[2].secLen = 34;
			listItemsTop = 3;
			
			for (int i = 0; i < listItemsTop; i++) {
				int p = (int)SendMessage(hLB, LB_ADDSTRING, 0, (LPARAM)listItems[i].name);
				SendMessage(hLB, LB_SETITEMDATA, p, (LPARAM)i); // Ensure item data matches the index in listItems
			}
			
			// Create vertical divider line
			CreateWindowA(
				WC_STATIC,
				"DividerLine",
				SS_ETCHEDVERT | WS_VISIBLE | WS_CHILD,
				WIN_WIDTH/2, 20, 1, WIN_HEIGHT - 100,
				hWnd,
				NULL,
				GetModuleHandle(NULL),
				NULL
			);
			
			// Create title for password input:
			HWND hPassTitle = CreateWindowA(
				WC_STATIC,
				"PasswordTitle",
				SS_CENTER | WS_VISIBLE | WS_CHILD,
				WIN_WIDTH/2 + 20, 0, WIN_WIDTH/2 - 40, 20,
				hWnd,
				NULL,
				GetModuleHandle(NULL),
				NULL
			);
			SetWindowTextA(hPassTitle, "Please enter the password here:");
			//SendMessage(hPassTitle, WM_SETFONT, (WPARAM) hFont, (LPARAM) 1);
			
			// Create password edit control:
			hPassEdit = CreateWindowA(
				WC_EDIT,
				"PasswordEdit",
				WS_BORDER | WS_VISIBLE | WS_CHILD,
				WIN_WIDTH/2 + 20, 20, WIN_WIDTH/2 - 40, 20,
				hWnd,
				NULL,
				GetModuleHandle(NULL),
				NULL
			);
			SetWindowTextA(hPassEdit, "Password");
			//SendMessage(hPassEdit, WM_SETFONT, (WPARAM) hFont, (LPARAM) 1);
			
			// Register debugger check timer:
			SetTimer(hWnd, ID_TIMER_DEBUG, 2000, NULL); // Every 2 seconds post timer message
			
			break;
		case WM_COMMAND:
			switch (HIWORD(wParam)) {
				case 0: // => a menu command
					switch(LOWORD(wParam)) {
						case ID_EXIT:
							PostMessage(hWnd, WM_CLOSE, 0, 0);
							break;
						case ID_ABOUT:
							DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_DIALOG_ABOUT), hWnd, (DLGPROC)AboutDialog);
							break;
					}
					break;
				//case 1: // => an accelerator command - breaks with LBN_SELCHANGE, same identifier :(
					// blank for now, no accelerators
					//break;
				case LBN_SELCHANGE:
					// TODO: Enable note selection button
					
					break;
				case LBN_DBLCLK: ;
					// For testing:
					int selectedIndex = (int)SendMessage((HWND)lParam, LB_GETCURSEL, 0, 0); // lParam = handle to listbox
					if (selectedIndex != -1) { // Message may be processed after user clicks off selection
						int listIndex = (int)SendMessage((HWND)lParam, LB_GETITEMDATA, selectedIndex, 0);
						// Grab input from password field
						int len = GetWindowTextLength(hPassEdit);
						char* input = NULL;
						if (len > 0) {
							input = (char*)GlobalAlloc(GPTR, len + 1); // Init to zero
							
							// Error checks
							if (input == NULL) {
								break;
							}
							if (GetWindowTextA(hPassEdit, input, len + 1) == 0) {
								GlobalFree(input);
								break;
							}
						}
						
						// Use caesar and XOR to verify the password by decoding a string
						unsigned char *key = (unsigned char *)input; // Do arithmetic on unsigned
						for (int i = 0; i < len; i++) {
							key[i] = key[i] + 0x23;
						}
						
						// Decode the following check-string to get "THIS IS THE CORRECT PASSPHRASE!"
						
						unsigned int i = 0;
						unsigned char verifyString[] = {0x2e, 0x1c, 0x38, 0x5, 0x75, 0xcb, 0xc6, 0x76, 0xcd, 0x1e, 0x30, 0x78, 0x17, 0x3e, 0xe, 0xd0, 0x13, 0x38, 0xc7, 0x76, 0xc5, 0x1b, 0x29, 0x7, 0x21, 0x1e, 0x7, 0xc3, 0xc6, 0x13, 0xb8, 0x00};
						unsigned char checkPhrase[] = "THIS IS THE CORRECT PASSPHRASE!";
						
						int equal = 1;
						while (i < sizeof(verifyString) - 1) { // Don't XOR the null term
							verifyString[i] ^= key[i % len];
							if (verifyString[i] != checkPhrase[i]) {
								equal = 0;
							}
							i += 1;
						}
						
						if (equal) {
							// Decode the flag stored in *list item*.secret using shifting and XOR
							char *decodedSecret = (char*)GlobalAlloc(GPTR, listItems[listIndex].secLen + 1);
							if (decodedSecret != NULL) {
								for (int j = 0; j < listItems[listIndex].secLen; j++) {
									decodedSecret[j] = key[j % len] ^ listItems[listIndex].secret[j];
								}
								
								MessageBoxA(hWnd, decodedSecret, "List Item Flag", MB_OK);
							}
							GlobalFree((HANDLE)decodedSecret);
						}
						else {
							MessageBoxA(hWnd, "That ain't the right password!", "Nope!", MB_OK);
						}
						
						GlobalFree((HANDLE)input);
					}
					break;
			}
			break;
		case WM_TIMER:
			switch (wParam) {
				case ID_TIMER_DEBUG: // The only timer really needed
					if (IsDebuggerPresent()) {
						MessageBoxA(NULL, "NO DEBUGGING!", "Nuh-uh", MB_OK);
						PostQuitMessage(1);
					}
				break;
			}
			break;
		case WM_CLOSE:
			// Free any handles, etc.
			free(listItems);
			DeleteObject(hFont);
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL CALLBACK AboutDialog(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_INITDIALOG:
			return TRUE;
		case WM_COMMAND: // Close dialog when OK is pressed
			switch(LOWORD(wParam)) {
				case IDOK:
					EndDialog(hWnd, IDOK);
					break;
			}
			break;
		case WM_CLOSE: // Handle dialog X button
			EndDialog(hWnd, 0);
			break;
		default:
			return FALSE;
	}
	return TRUE;
}