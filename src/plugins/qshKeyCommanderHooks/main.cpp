/**************************************************************/
/*          qshKeyCommander1 - qshKeyCommanderHooks           */
/*------------------------------------------------------------*/
/*     ������ ��� qshKeyCommander, ����������� �����������    */
/*     ��������� �� ���������� (������ � ���������� ������)   */
/*****2004***************************Copyright by <Quash!>*****/


#include "windows.h"


// *���������� Runtime Library
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:DllMain")
#pragma comment(linker,"/NODEFAULTLIB")



#define WM_MAINMENU         WM_USER+1           // ��������� ��� ������/������� �������� ����
#define serverclassname     "qshKeyCommander_1" // ��� ������ ����-������� (�.�. ����������� � ������� ���������� � ����� qkc_main.h)
#define MAX_PASSWORDSYMBOLS 25                  // ������������ ���������� �������� � ������ (�.�. ����������� � ������� ���������� � ����� qkc_main.h)



// ���������� ���������� ��� ���� ���������
#pragma comment(linker, "/SECTION:shared,RWS")  
#pragma data_seg("shared")

// ������������� �������
HHOOK hook             = NULL;
// ���������� �� ������������
bool  pswrd_start      = false;
bool  hotkey_start     = false;
// ���������� ������
HWND  hwndServerWindow = NULL;
char  pswrd_input[MAX_PATH]           = "";
char  pswrd_data[MAX_PASSWORDSYMBOLS] = "passw0rd";
 
#pragma data_seg()



// ���� � dll
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
 return true;
}
 
// ������� ��������� ��������� �� ����������
LRESULT CALLBACK KeyboardProc(int code,WPARAM wParam, LPARAM lParam)
{
 if ((code == HC_ACTION)&&(!(lParam&0x80000000)))//(!(HIWORD(lParam) & KF_UP)))
 {
  // *������������ ������ (���� ���������)
  if (pswrd_start)
   {
    char buffer[64];
	switch ((char)wParam)
     {
      case VK_CONTROL: // Ctrl
      case VK_MENU:    // Alt
      case VK_SHIFT:   // Shift
      case VK_LWIN:    // Win
      case VK_RWIN:
      case VK_CAPITAL: // Caps Lock
      case VK_NUMLOCK: // Num Lock
      case VK_SCROLL:  // Scroll Lock
	  break;
	  default:
       BYTE keyarray[256] ;
       if(GetKeyboardState(keyarray))
	    if(!ToAscii(wParam,(HIWORD(lParam)&(0x0000FFFF)),keyarray,(WORD*)&buffer[0],NULL)) buffer[0]='\0';				
       lstrcat(pswrd_input,buffer);
       // �������� �� ����������� ��� �������� ��������
       for (int i=0; i<lstrlen(pswrd_input); i++)
        if (pswrd_input[i]!=pswrd_data[i]) pswrd_input[0]='\0';
       // ������������ ������� ���� ���������, ���� ������ ������ ���������
       if (lstrcmp(pswrd_input,pswrd_data)==0) 
        {
         SendMessage(FindWindow(serverclassname,NULL),WM_MAINMENU,0,0);
         pswrd_input[0]='\0';
        }
      break;
   	 }
   }
  // *������������ ���������� ������ (���� ���������)
  if (hotkey_start)
   {
    switch ((char)wParam)
     {
      // ������� �������������
	  case VK_CONTROL: // Ctrl
	  case VK_MENU:    // Alt
	  case VK_SHIFT:   // Shift
	  case VK_LWIN:    // Win
	  case VK_RWIN:
	  case VK_CAPITAL: // Caps Lock
      case VK_NUMLOCK: // Num Lock
      case VK_SCROLL:  // Scroll Lock
	  break;
	  default:
       {
        WORD modifiers = 0;
        if ((GetKeyState(VK_LWIN)&0x8000)||(GetKeyState(VK_RWIN)&0x8000)) modifiers |= MOD_WIN;
        if (GetKeyState(VK_CONTROL)&0x8000) modifiers |= MOD_CONTROL;
        if (GetKeyState(VK_SHIFT)&0x8000)   modifiers |= MOD_SHIFT;
        if (GetKeyState(VK_MENU)&0x8000)    modifiers |= MOD_ALT;
        if (IsWindow(hwndServerWindow))
         SendMessage(hwndServerWindow,WM_HOTKEY,1,MAKELPARAM(modifiers,wParam));
       }
      break;
	 }
   }
 }
 return CallNextHookEx(hook, code, wParam, lParam);
}

// ������� �� �������
#ifdef __cplusplus
extern "C"
{
#endif



// ������� ��������� �������
__declspec (dllexport) void InitHooks(HMODULE hdllinst, bool pswrdstart, char password[MAX_PASSWORDSYMBOLS], bool hotkeystart, HWND hwndSrvWnd)
{
 // ����������� ����������� ������
 pswrd_start  = pswrdstart;
 hotkey_start = hotkeystart;
 // �������� ������
 hwndServerWindow = hwndSrvWnd;
 lstrcpy(pswrd_data,password);
 lstrcpy(pswrd_input,"");
 // ��������� �������
 hook = SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)KeyboardProc,hdllinst,0);
}

// ������� �������� �������
__declspec (dllexport) void QuitHooks(void)
{
 UnhookWindowsHookEx(hook);
}



#ifdef __cplusplus
}
#endif
