/**************************************************************/
/*          qshKeyCommander1 - qshKeyCommanderHooks           */
/*------------------------------------------------------------*/
/*     Плагин для qshKeyCommander, позволяющий отслеживать    */
/*     сообщения от клавиатуры (пароль и комбинации клавиш)   */
/*****2004***************************Copyright by <Quash!>*****/


#include "windows.h"


// *Выключение Runtime Library
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:DllMain")
#pragma comment(linker,"/NODEFAULTLIB")



#define WM_MAINMENU         WM_USER+1           // Сообщение для показа/скрытия главного меню
#define serverclassname     "qshKeyCommander_1" // Имя класса окна-сервера (д.б. согласовано с главной программой в файле qkc_main.h)
#define MAX_PASSWORDSYMBOLS 25                  // Максимальное количество символов в пароле (д.б. согласовано с главной программой в файле qkc_main.h)



// Глобальные переменные для всех процессов
#pragma comment(linker, "/SECTION:shared,RWS")  
#pragma data_seg("shared")

// Идентификатор ловушки
HHOOK hook             = NULL;
// Разрешение на отслеживание
bool  pswrd_start      = false;
bool  hotkey_start     = false;
// Переданные данные
HWND  hwndServerWindow = NULL;
char  pswrd_input[MAX_PATH]           = "";
char  pswrd_data[MAX_PASSWORDSYMBOLS] = "passw0rd";
 
#pragma data_seg()



// Вход в dll
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
 return true;
}
 
// Функция обработки сообщений от клавиатуры
LRESULT CALLBACK KeyboardProc(int code,WPARAM wParam, LPARAM lParam)
{
 if ((code == HC_ACTION)&&(!(lParam&0x80000000)))//(!(HIWORD(lParam) & KF_UP)))
 {
  // *Отслеживание пароля (если разрешено)
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
       // Проверка на првильность уже введённых символов
       for (int i=0; i<lstrlen(pswrd_input); i++)
        if (pswrd_input[i]!=pswrd_data[i]) pswrd_input[0]='\0';
       // Показывается главное меню программы, если пароль набран правильно
       if (lstrcmp(pswrd_input,pswrd_data)==0) 
        {
         SendMessage(FindWindow(serverclassname,NULL),WM_MAINMENU,0,0);
         pswrd_input[0]='\0';
        }
      break;
   	 }
   }
  // *Отслеживание комбинации клавиш (если разрешено)
  if (hotkey_start)
   {
    switch ((char)wParam)
     {
      // Пропуск модификаторов
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

// Функции на экспорт
#ifdef __cplusplus
extern "C"
{
#endif



// Функция установки ловушек
__declspec (dllexport) void InitHooks(HMODULE hdllinst, bool pswrdstart, char password[MAX_PASSWORDSYMBOLS], bool hotkeystart, HWND hwndSrvWnd)
{
 // Копирование разрешающих флагов
 pswrd_start  = pswrdstart;
 hotkey_start = hotkeystart;
 // Передача данных
 hwndServerWindow = hwndSrvWnd;
 lstrcpy(pswrd_data,password);
 lstrcpy(pswrd_input,"");
 // Установка ловушки
 hook = SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)KeyboardProc,hdllinst,0);
}

// Функция удаления ловушек
__declspec (dllexport) void QuitHooks(void)
{
 UnhookWindowsHookEx(hook);
}



#ifdef __cplusplus
}
#endif
