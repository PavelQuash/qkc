/**************************************************************/
/*                 qshKeyCommander1 - main.cpp                */
/*------------------------------------------------------------*/
/*         Плагин управления ОС для qshKeyCommander1          */
/*****2004***************************Copyright by <Quash!>*****/

// Добавлено: Winmm.lib - работа с cdaudio


#include "windows.h"
#include "system_ids.h"
#include "shlobj.h" // SHAddToRecentDocs
#include "..\qkc_plugin.h"


// *Отключение Runtime Library
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:DllMain")
#pragma comment(linker,"/NODEFAULTLIB")


#define CMDCOUNT 49
int  CmdCount = CMDCOUNT;    // Количество команд, используемое в плагине
char CmdList[CMDCOUNT][200]; // Список команд

// Подключение/отключение модуля
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
 switch (ul_reason_for_call)
  {
   case DLL_PROCESS_ATTACH:
    // Заполнение списка
    lstrcpy(CmdList[RUN_SCREENSAVER]                  ,"Запуск скринсейвера");
    lstrcpy(CmdList[SH_ICONS_DESKTOP]                 ,"Скрыть/показать иконки рабочего стола");
    lstrcpy(CmdList[SH_TASK_PANEL]                    ,"Скрыть/показать панель задач");
    lstrcpy(CmdList[CD_AUDIO_OPEN]                    ,"Открыть лоток аудио CD");
    lstrcpy(CmdList[CD_AUDIO_CLOSE]                   ,"Закрыть лоток аудио CD");
    lstrcpy(CmdList[CD_AUDIO_OPENCLOSE]               ,"*Открыть/закрыть лоток CD");
    lstrcpy(CmdList[CLEAR_RECYCLEDBIN]                ,"Очистить корзину");
    lstrcpy(CmdList[CLEAR_DOCUMENTS]                  ,"Очистить документы");
    lstrcpy(CmdList[VOLUME_SYSTEM_UP]                 ,"+Увеличить громкость");
    lstrcpy(CmdList[VOLUME_SYSTEM_DOWN]               ,"-Уменьшить громкость");
    lstrcpy(CmdList[PC_SHUTDOWN]                      ,"Выключить компьютер");
    lstrcpy(CmdList[PC_RESTART]                       ,"Перезагрузка компьютера");
    lstrcpy(CmdList[PC_QUICKRESTART]                  ,"Быстрая перезагрузка компьютера");
    lstrcpy(CmdList[PC_RESTARTOS]                     ,"*Перезагрузка операционной системы");
    lstrcpy(CmdList[PC_LOGOUT]                        ,"Завершение сеанса пользователя");
    lstrcpy(CmdList[CHANGE_WALLPAPER]                 ,"*Сменить обои");
    lstrcpy(CmdList[MONITOR_OFF]                      ,"Выключить монитор");
    lstrcpy(CmdList[MONITOR_ON]                       ,"Включить монитор");
    lstrcpy(CmdList[KEYBOARD_ON_OFF]                  ,"*Выключить/включить клавиатуру");
    lstrcpy(CmdList[MOUSE_ON_OFF]                     ,"*Выключить/включить мышь");
    lstrcpy(CmdList[CONTROL_PANEL]                    ,"Панель управления");
    lstrcpy(CmdList[DIALOG_EXECUTE]                   ,"*Диалог выполнить");
    lstrcpy(CmdList[MINIMIZE_ALL_WINDOWS]             ,">Свернуть все окна");
    lstrcpy(CmdList[SLEEP]                            ,"*Спящий режим (ждущий)");
    lstrcpy(CmdList[SHOW_TASK_LIST]                   ,"Стартовое меню");

    lstrcpy(CmdList[CONTROL_INSTALLPROGRAMS]          ,"Установка/удаление программ");
    lstrcpy(CmdList[CONTROL_SCREENSETTINGS]           ,"Экран");
    lstrcpy(CmdList[CONTROL_FINDFAST]                 ,"Поиск файлов");
    lstrcpy(CmdList[CONTROL_IESETTINGS]               ,"Свойства обозревателя");
    lstrcpy(CmdList[CONTROL_LANGUAGE]                 ,"Язык и стандарты");
    lstrcpy(CmdList[CONTROL_GAMEDEVICES]              ,"Игровые устройства");
    lstrcpy(CmdList[CONTROL_MOUSE]                    ,"Мышь");
    lstrcpy(CmdList[CONTROL_MULTIMEDIA]               ,"Мультимедиа");
    lstrcpy(CmdList[CONTROL_MODEM]                    ,"Модем");
    lstrcpy(CmdList[CONTROL_NETWORK]                  ,"Сеть");
    lstrcpy(CmdList[CONTROL_PASSWORDS]                ,"Пароли");
    lstrcpy(CmdList[CONTROL_POWERSETTINGS]            ,"Управление электропитанием");
    lstrcpy(CmdList[CONTROL_SCANERS_AND_CAMERAS]      ,"Сканеры и камеры");
    lstrcpy(CmdList[CONTROL_SYSTEM]                   ,"Система");
    lstrcpy(CmdList[CONTROL_TELEPHONE]                ,"Телефонные соединения");
    lstrcpy(CmdList[CONTROL_THEMES]                   ,"Темы рабочего стола");
    lstrcpy(CmdList[CONTROL_DATATIME]                 ,"Дата и время");
    lstrcpy(CmdList[CONTROL_KEYBOARD]                 ,"Клавиатура");
    lstrcpy(CmdList[CONTROL_PRINTERS]                 ,"Принтеры");
    lstrcpy(CmdList[CONTROL_FONTS]                    ,"Шрифты");
    lstrcpy(CmdList[CONTROL_SOUND]                    ,"*Звук");
    lstrcpy(CmdList[CONTROL_USERS]                    ,"*Пользователи");
    lstrcpy(CmdList[CONTROL_MAIL]                     ,"*Почта");
    lstrcpy(CmdList[CONTROL_INSTALLHARDWARE]          ,"*Установка оборудования");
    break;

   case DLL_PROCESS_DETACH:
    break;
  }
 return true;
}

// Функция инициализации
//void Init(void)
//{ MessageBox(0,"MyPlugin Init","Info:",0); }

// Функция деинициализации
//void Quit(void)
//{ MessageBox(0,"MyPlugin Quit","Info:",0); }

// Конфигурация плагина
//void Config(void)
//{ MessageBox(0,"MyPlugin Config","Info:",0); }

// Запрос на выполнение команды под номером cmdid
void Command(int cmdid)
{
 switch (cmdid)
  {
   case RUN_SCREENSAVER: // debug
    PostMessage(GetDesktopWindow(),WM_SYSCOMMAND,SC_SCREENSAVE,0);
    break;
   case SH_ICONS_DESKTOP:
    if (IsWindowVisible(FindWindow("Progman",NULL))) ShowWindow(FindWindow("Progman",NULL),SW_HIDE);
    else                                             ShowWindow(FindWindow("Progman",NULL),SW_SHOW);
    break;
   case SH_TASK_PANEL:
    if (IsWindowVisible(FindWindow("Shell_TrayWnd",NULL))) ShowWindow(FindWindow("Shell_TrayWnd",NULL),SW_HIDE);
    else                                                   ShowWindow(FindWindow("Shell_TrayWnd",NULL),SW_SHOW);
    break;
   case CLEAR_DOCUMENTS:
    SHAddToRecentDocs(SHARD_PATH,NULL);
    break;


/*
Вопрос: 

Как програмно перезагрузить Windows? Ответ: Используйте функцию ExitWindows(). В качестве первого параметра ей передается она из трех констант: 
   EW_RESTARTWINDOWS 
   EW_REBOOTSYSTEM 
   EW_EXITANDEXECAPP 
Второй параметр используется для перезагрузки компьютера в режиме эмуляции MS DOS. 

Пример:
  ExitWindows(EW_RESTARTWINDOWS, 0 );  

    
завеpшение сеанса: RUNDLL32.EXE shell32.dll,SHExitWindowsEx
выключение:        RUNDLL32.EXE shell32.dll,SHExitWindowsEx 1
Reboot:            RUNDLL32.EXE shell32.dll,SHExitWindowsEx 2

Без запpоса
завеpшение сеанса: RUNDLL32.EXE shell32.dll,SHExitWindowsEx 4
выключение:        RUNDLL32.EXE shell32.dll,SHExitWindowsEx 5
Reboot:            RUNDLL32.EXE shell32.dll,SHExitWindowsEx 6
*/
   case PC_SHUTDOWN:
    ExitWindowsEx(EWX_SHUTDOWN,0);
    break;
   case PC_RESTART:
    ExitWindowsEx(EWX_REBOOT,0);
    break;
   case PC_QUICKRESTART:
    ExitWindowsEx(EWX_FORCE|EWX_REBOOT,0);
    break;
   case PC_RESTARTOS:
   // ExitWindowsEx(EW_RESTARTWINDOWS,0);
    break;
   case PC_LOGOUT:
    ExitWindowsEx(EWX_LOGOFF,0);
    break;
   case CD_AUDIO_OPEN:
    mciSendString("Set cdaudio door open wait",NULL,0,GetDesktopWindow());
    break;
   case CD_AUDIO_CLOSE:
    mciSendString("Set cdaudio door closed wait",NULL,0,GetDesktopWindow());
    break;
   case CLEAR_RECYCLEDBIN:
    SHEmptyRecycleBin(NULL,NULL,SHERB_NOPROGRESSUI);
    break;
   case MONITOR_OFF:
    PostMessage(GetDesktopWindow(),WM_SYSCOMMAND,SC_MONITORPOWER,1);
    break;
   case MONITOR_ON:
    PostMessage(GetDesktopWindow(),WM_SYSCOMMAND,SC_MONITORPOWER,-1); 
    break;
   case CONTROL_PANEL:
    WinExec("control.exe",SW_SHOWNORMAL);
    break;
   case MINIMIZE_ALL_WINDOWS:
    PostMessage(FindWindow("Shell_TrayWnd",NULL),WM_COMMAND,0x019F,0);
    break;
   case SHOW_TASK_LIST:
    PostMessage(GetDesktopWindow(),WM_SYSCOMMAND,SC_TASKLIST,0);
    break;

   case CONTROL_INSTALLPROGRAMS:
    WinExec("control.exe appwiz.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_SCREENSETTINGS:
    WinExec("control.exe desk.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_FINDFAST:
    WinExec("control.exe findfast.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_IESETTINGS:
    WinExec("control.exe inetcpl.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_LANGUAGE:
    WinExec("control.exe intl.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_GAMEDEVICES:
    WinExec("control.exe joy.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_MOUSE:
    WinExec("control.exe mouse",SW_SHOWNORMAL);
    break;
   case CONTROL_MULTIMEDIA:
    WinExec("control.exe mmsys.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_MODEM:
    WinExec("control.exe modem.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_NETWORK:
    WinExec("control.exe netcpl.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_PASSWORDS:
    WinExec("control.exe password.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_POWERSETTINGS:
    WinExec("control.exe powercfg.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_SCANERS_AND_CAMERAS:
    WinExec("control.exe sticpl.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_SYSTEM:
    WinExec("control.exe sysdm.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_TELEPHONE:
    WinExec("control.exe telephon.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_THEMES:
    WinExec("control.exe themes.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_DATATIME:
    WinExec("control.exe timedate.cpl",SW_SHOWNORMAL);
    break;
   case CONTROL_KEYBOARD:
    WinExec("control.exe keyboard",SW_SHOWNORMAL);
    break;
   case CONTROL_PRINTERS:
    WinExec("control.exe printers",SW_SHOWNORMAL);
    break;
   case CONTROL_FONTS:
    WinExec("control.exe fonts",SW_SHOWNORMAL);
    break;
   case CONTROL_SOUND:
    //WinExec("control.exe",SW_SHOWNORMAL);
    break;
   case CONTROL_USERS:
    //WinExec("control.exe",SW_SHOWNORMAL);
    break;
   case CONTROL_MAIL:
    //WinExec("control.exe",SW_SHOWNORMAL);
    break;
   case CONTROL_INSTALLHARDWARE:
    //WinExec("control.exe",SW_SHOWNORMAL);
    break;
    
    // Реакция на постороннее сообщение (на всякий случай)
   default:
    break;
  }
}



#ifdef __cplusplus
extern "C"
{
#endif

// Функция на экспорт, вызываемая плагином по умолчанию
__declspec (dllexport) void GetModuleData(struct stPluginData *PluginData)
{
 // Заполнение заголовка
 PluginData->Caption = "System v0.24 (04.06.2004г.)\n\n© <Quash!>, г.Екатеринбург)";

 // Передача адресов функций qshKeyCommander'у
 PluginData->Init          = NULL;//Init;
 PluginData->Quit          = NULL;//Quit;
 PluginData->Config        = NULL;//Config;
 PluginData->Command       = Command;

 // Передача адресов списка и количества его элементов в памяти
 PluginData->CommandsCount = &CmdCount;
 PluginData->CommandsList  = CmdList;
}

#ifdef __cplusplus
}
#endif