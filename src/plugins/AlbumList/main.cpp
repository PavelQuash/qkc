/**************************************************************/
/*                 qshKeyCommander1 - main.cpp                */
/*------------------------------------------------------------*/
/*      Плагин управления AlbumList (для Winamp2)             */
/*****2004***************************Copyright by <Quash!>*****/

#include "../qkc_plugin.h" // Структура данных плагина для QshKeyCommander
#include "windows.h"       // Стандарт :)
#include "AlbumList_ids.h" // Идентификаторы команд
#include "alfront.h"       // Заголовочный файл AlbumList



// *Отключение Runtime Library
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:DllMain")
#pragma comment(linker,"/NODEFAULTLIB")



#define CMDCOUNT 6
int  CmdCount = CMDCOUNT;    // Количество команд, используемое в плагине
char CmdList[CMDCOUNT][200]; // Список команд
char AlbumListClassName[] = "Winamp AL"; // Класс окна AlbumList

// Подключение/отключение модуля
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
 switch (ul_reason_for_call)
  {
   case DLL_PROCESS_ATTACH:
    {
     // Заполнение списка
     lstrcpy(CmdList[PLAYALBUM],       "Воспроизведение альбома");
     lstrcpy(CmdList[PLAYRANDOMALBUM], "Воспроизвести случайный альбом");
     lstrcpy(CmdList[PLAYALLALBUMS],   "Воспроизвести все альбомы");

     lstrcpy(CmdList[PREVALBUM],       "Предыдущий альбом");
     lstrcpy(CmdList[NEXTALBUM],       "Следующий альбом");

     lstrcpy(CmdList[HSALBUMLIST],     "Показать/скрыть окно Album List");
    }
    break;

   case DLL_PROCESS_DETACH:
    break;
  }
 return true;
}

// (ждёт) Передача сообщения окну AlbumList для получения/установки параметров Winamp
int AlbumListUserSend(int id, int data)
{
 // Поиск окна Winamp
	HWND hwndAL = FindWindow(AlbumListClassName,NULL);
		// Если окно найдено, то сообщение посылается, иначе ничего не происходит
	if (hwndAL) return (SendMessage(hwndAL,WM_AL_IPC,id,data));
	else return (-1);
}

// (не ждёт) Передача сообщения окну AlbumList'а для получения/установки параметров
int AlbumListUserPost(int id, int data)
{
 // Поиск окна Winamp
	HWND hwndAL = FindWindow(AlbumListClassName,NULL);
	// Если окно найдено, то сообщение посылается, иначе ничего не происходит
	if (hwndAL) return (PostMessage(hwndAL,WM_AL_IPC,id,data));
	else return (-1);
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
   // Управление воспроизведением
   case PLAYALBUM:
    {
     int i = AlbumListUserSend(IPC_GETALBUMINDEX,0);
     if (i<0) AlbumListUserPost(IPC_PLAYALBUM,0);
     else     AlbumListUserPost(IPC_PLAYALBUM,i);
    }
    break;
   case PLAYRANDOMALBUM:
    AlbumListUserPost(IPC_PLAYRANDOMALBUM,0);
    break;
   case PLAYALLALBUMS:
    AlbumListUserPost(IPC_PLAYALLALBUMS,0);
    break;

   // Перемещение по playlist'у
   case PREVALBUM:
    AlbumListUserPost(IPC_PLAYPREVALBUM,0);
    break;
   case NEXTALBUM:
    AlbumListUserPost(IPC_PLAYNEXTALBUM,0);
    break;

   case HSALBUMLIST:
    {
     HWND hwndAL = FindWindow(AlbumListClassName,NULL);
     if (IsWindowVisible(hwndAL)) 
      ShowWindow(hwndAL,SW_HIDE);
     else                         
     {
      ShowWindow(hwndAL,SW_SHOW);
      SetForegroundWindow(hwndAL);
     }
    }
    break;

   // Сообщение по умолчанию, ничего не происходит
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
 PluginData->Caption = "Album List v1.00 (21.05.2004г.)\nAlbumList  плагин для Winamp\n© <Quash!>, г.Екатеринбург)";

 // Передача адресов функций qshKeyCommander'у
 PluginData->Init    = NULL;//Init;
 PluginData->Quit    = NULL;//Quit;
 PluginData->Config  = NULL;//Config;
 PluginData->Command = Command;

 // Передача адресов списка и количества его элементов в памяти
 PluginData->CommandsCount = &CmdCount;
 PluginData->CommandsList  = CmdList;
}

#ifdef __cplusplus
}
#endif