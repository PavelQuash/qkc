/**************************************************************/
/*                 qshKeyCommander1 - main.cpp                */
/*------------------------------------------------------------*/
/*    Плагин управления Light Alloy для qshKeyCommander1      */
/*****2004***************************Copyright by <Quash!>*****/

#include "windows.h"
#include "..\qkc_plugin.h"
#include "LightAlloy_ids.h"

#define LightAlloy_ClassName "LightAlloyFront"

// *Отключение Runtime Library
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:DllMain")
#pragma comment(linker,"/NODEFAULTLIB")



#define CMDCOUNT 69
int  CmdCount = CMDCOUNT;    // Количество команд, используемое в плагине
char CmdList[CMDCOUNT][200]; // Список команд

// Подключение/отключение модуля
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
 switch (ul_reason_for_call)
  {
   case DLL_PROCESS_ATTACH:
    // *Заполнение списка
    // Файл
    lstrcpy(CmdList[FILE_OPEN]               ,"Открыть файл");
    lstrcpy(CmdList[FILE_INFO]               ,"Информация о файле");
    // Воспроизведение
    lstrcpy(CmdList[PLAYBACK_STOP]           ,"Стоп");
    lstrcpy(CmdList[PLAYBACK_PLAY]           ,"Воспроизведение");
    lstrcpy(CmdList[PLAYBACK_STOP_PLAY]      ,"Воспроизведение/Стоп");
    lstrcpy(CmdList[PLAYBACK_SPEED_PLAY]     ,"Ускоренное воспроизведение");
    lstrcpy(CmdList[PLAYBACK_FILTERS]        ,"Фильтры DirectShow");
    // Перемотка
    lstrcpy(CmdList[SEEK_FRAME_STEP]         ,"Кадр вперёд");
    lstrcpy(CmdList[SEEK_FRAME_BACK]         ,"Кадр назад");
    lstrcpy(CmdList[SEEK_FORWARD]            ,"Перемотать вперёд");
    lstrcpy(CmdList[SEEK_BACKWARD]           ,"Перемотать назад");
    lstrcpy(CmdList[SEEK_JUMP_FORWARD]       ,"Прыжок вперёд");
    lstrcpy(CmdList[SEEK_JUMP_BACKWARD]      ,"Прыжок назад");
    lstrcpy(CmdList[SEEK_REWIND]             ,"Перемотать в начало");
    lstrcpy(CmdList[SEEK_SET_BOOKMARK]       ,"Установить закладку");
    // Окно
    lstrcpy(CmdList[WINDOW_CONTROL_PANEL]    ,"Показать/скрыть панель управления");
    lstrcpy(CmdList[WINDOW_PLAYLIST]         ,"Показать/скрыть список");
    lstrcpy(CmdList[WINDOW_FULLSCREEN]       ,"Переключение окно/весь экран");
    lstrcpy(CmdList[WINDOW_ORIGINAL]         ,"Оригинальный");
    lstrcpy(CmdList[WINDOW_STAY_ON_TOP]      ,"Поверх всех окон");
    lstrcpy(CmdList[WINDOW_MINIMIZE]         ,"Минимизировать/восстановить");
    lstrcpy(CmdList[WINDOW_MAXIMIZE]         ,"Максимизировать/восстановить");

    // Список
    lstrcpy(CmdList[PLAYLIST_NEXT]           ,"Следующий элемент по списку");
    lstrcpy(CmdList[PLAYLIST_PREV]           ,"Предыдущий элемент по списку");
    lstrcpy(CmdList[PLAYLIST_PLAY]           ,"Воспроизведение выбранного");
    lstrcpy(CmdList[PLAYLIST_ADD_FILES]      ,"Добавить файл(ы) в список");
    lstrcpy(CmdList[PLAYLIST_ADD_FOLDER]     ,"Добавить папку в список");
    lstrcpy(CmdList[PLAYLIST_DELETE]         ,"Удалить эдемент из списка");
    lstrcpy(CmdList[PLAYLIST_CLEAR]          ,"Очистить список");
    lstrcpy(CmdList[PLAYLIST_SAVE]           ,"Сохранить список");
    lstrcpy(CmdList[PLAYLIST_MOVE_UP]        ,"Переместить элемент выше");
    lstrcpy(CmdList[PLAYLIST_MOVE_DOWN]      ,"Переместить элемент ниже");
    lstrcpy(CmdList[PLAYLIST_SHUFFLE]        ,"Перемешать список");
    lstrcpy(CmdList[PLAYLIST_SORT]           ,"Сортировать список");
    lstrcpy(CmdList[PLAYLIST_REPORT]         ,"Таблица по списку");
    lstrcpy(CmdList[PLAYLIST_REPEAT]         ,"Зацикливание списка");
    lstrcpy(CmdList[PLAYLIST_BOOKMARKS]      ,"Показать/скрыть закладки в списке");
    // Видео
    lstrcpy(CmdList[VIDEO_PROPERTIES]        ,"Настройки видео");
    lstrcpy(CmdList[VIDEO_SCREENSHOT]        ,"Сохранить кадр");
    lstrcpy(CmdList[VIDEO_SCALE_50]          ,"Масштаб  50%");
    lstrcpy(CmdList[VIDEO_SCALE_100]         ,"Масштаб 100%");
    lstrcpy(CmdList[VIDEO_SCALE_200]         ,"Масштаб 200%");
    lstrcpy(CmdList[VIDEO_RATIO_ASIS]        ,"Пропорции \"как есть\"");
    lstrcpy(CmdList[VIDEO_RATIO_16_9]        ,"Пропорции 16:9");
    lstrcpy(CmdList[VIDEO_RATIO_4_3]         ,"Пропорции  4:3");
    lstrcpy(CmdList[VIDEO_RATIO_WIDTH]       ,"Пропорции по ширине");
    lstrcpy(CmdList[VIDEO_RATIO_HEIGHT]      ,"Пропорции по высоте");
    lstrcpy(CmdList[VIDEO_RATIO_CUSTOM]      ,"Особые пропорции");
    lstrcpy(CmdList[VIDEO_RATIO_FREE]        ,"Без сохранения пропорций");
    lstrcpy(CmdList[VIDEO_ZOOM_IN]           ,"Увеличить изображение");
    lstrcpy(CmdList[VIDEO_ZOOM_OUT]          ,"Уменьшить изображение");
    lstrcpy(CmdList[VIDEO_BRIGHTNESS_INC]    ,"Прибавить яркость");
    lstrcpy(CmdList[VIDEO_BRIGHTNESS_DEC]    ,"Убавить яркость");
    lstrcpy(CmdList[VIDEO_CONTRAST_INC]      ,"Прибавить контраст");
    lstrcpy(CmdList[VIDEO_CONTRAST_DEC]      ,"Убавить контраст");
    lstrcpy(CmdList[VIDEO_SATURATION_INC]    ,"Прибавить насыщенность");
    lstrcpy(CmdList[VIDEO_SATURATION_DEC]    ,"Убавить насыщенность");
    // Субтитры
    lstrcpy(CmdList[SUBTITLES_LOAD]          ,"Загрузить субтитры");
    lstrcpy(CmdList[SUBTITLES_SHOW]          ,"Показать субтитры");
    // Звук
    lstrcpy(CmdList[SOUND_PROPERTIES]        ,"Настройки звука");
    lstrcpy(CmdList[SOUND_VOLUME_INC]        ,"Увеличить громкость");
    lstrcpy(CmdList[SOUND_VOLUME_DEC]        ,"Уменьшить громкость");
    lstrcpy(CmdList[SOUND_MUTE]              ,"Отключить звук");
    lstrcpy(CmdList[SOUND_ADD]               ,"Загрузить дополнительный звук");
    // Программа
    lstrcpy(CmdList[APPLICATION_PREFERENCES] ,"Настройки");
    lstrcpy(CmdList[APPLICATION_HELP]        ,"Помощь");
    lstrcpy(CmdList[APPLICATION_ABOUT]       ,"О программе");
    lstrcpy(CmdList[APPLICATION_EXIT]        ,"Выход");
    lstrcpy(CmdList[APPLICATION_POWER_OFF]   ,"Выключить компьютер");
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

// Отправление сообщения окну LightAlloy
BOOL LightAlloyCommandPost(int command)
{
 HWND hwndLA;
 // Ищем окно программы
 hwndLA = FindWindow(LightAlloy_ClassName,NULL);
 if (hwndLA)
  { 
   // Посылаем окну сообщение
   PostMessage(hwndLA,WM_LACMD,command,0);
   return true;
  }
 return false;
}

// Запрос на выполнение команды под номером cmdid
void Command(int cmdid)
{
 switch (cmdid)
  {
   case FILE_OPEN: // ?
    LightAlloyCommandPost(LAC_FILE_OPEN);
    break;
   case FILE_INFO: // ?
    LightAlloyCommandPost(LAC_FILE_INFO);
    break;

   case PLAYBACK_STOP:
    LightAlloyCommandPost(LAC_PLAYBACK_STOP);
    break;
   case PLAYBACK_PLAY:
    LightAlloyCommandPost(LAC_PLAYBACK_PLAY);
    break;
   case PLAYBACK_STOP_PLAY:
    LightAlloyCommandPost(LAC_PLAYBACK_STOP_PLAY);
    break;
   case PLAYBACK_SPEED_PLAY:
    LightAlloyCommandPost(LAC_PLAYBACK_SPEED_PLAY);
    break;
   case PLAYBACK_FILTERS:
    LightAlloyCommandPost(LAC_PLAYBACK_FILTERS);
    break;

   case SEEK_FRAME_STEP:
    LightAlloyCommandPost(LAC_SEEK_FRAME_STEP);
    break;
   case SEEK_FRAME_BACK:
    LightAlloyCommandPost(LAC_SEEK_FRAME_BACK);
    break;
   case SEEK_FORWARD:
    LightAlloyCommandPost(LAC_SEEK_FORWARD);
    break;
   case SEEK_BACKWARD:
    LightAlloyCommandPost(LAC_SEEK_BACKWARD);
    break;
   case SEEK_JUMP_FORWARD:
    LightAlloyCommandPost(LAC_SEEK_JUMP_FORWARD);
    break;
   case SEEK_JUMP_BACKWARD:
    LightAlloyCommandPost(LAC_SEEK_JUMP_BACKWARD);
    break;
   case SEEK_REWIND:
    LightAlloyCommandPost(LAC_SEEK_REWIND);
    break;
   case SEEK_SET_BOOKMARK:
    LightAlloyCommandPost(LAC_SEEK_SET_BOOKMARK);
    break;

   case WINDOW_CONTROL_PANEL:
    LightAlloyCommandPost(LAC_WINDOW_CONTROL_PANEL);
    break;
   case WINDOW_PLAYLIST:
    LightAlloyCommandPost(LAC_WINDOW_PLAYLIST);
    break;
   case WINDOW_FULLSCREEN:
    LightAlloyCommandPost(LAC_WINDOW_FULLSCREEN);
    break;
   case WINDOW_ORIGINAL:
    LightAlloyCommandPost(LAC_WINDOW_ORIGINAL);
    break;
   case WINDOW_STAY_ON_TOP:
    LightAlloyCommandPost(LAC_WINDOW_STAY_ON_TOP);
    break;
   case WINDOW_MINIMIZE:
    LightAlloyCommandPost(LAC_WINDOW_MINIMIZE);
    break;
   case WINDOW_MAXIMIZE:
    LightAlloyCommandPost(LAC_WINDOW_MAXIMIZE);
    break;

   case PLAYLIST_NEXT:
    LightAlloyCommandPost(LAC_PLAYLIST_NEXT);
    break;
   case PLAYLIST_PREV:
    LightAlloyCommandPost(LAC_PLAYLIST_PREV);
    break;
   case PLAYLIST_PLAY:
    LightAlloyCommandPost(LAC_PLAYLIST_PLAY);
    break;
   case PLAYLIST_ADD_FILES:
    LightAlloyCommandPost(LAC_PLAYLIST_ADD_FILES);
    break;
   case PLAYLIST_ADD_FOLDER:
    LightAlloyCommandPost(LAC_PLAYLIST_ADD_FOLDER);
    break;
   case PLAYLIST_DELETE:
    LightAlloyCommandPost(LAC_PLAYLIST_DELETE);
    break;
   case PLAYLIST_CLEAR:
    LightAlloyCommandPost(LAC_PLAYLIST_CLEAR);
    break;
   case PLAYLIST_SAVE:
    LightAlloyCommandPost(LAC_PLAYLIST_SAVE);
    break;
   case PLAYLIST_MOVE_UP:
    LightAlloyCommandPost(LAC_PLAYLIST_MOVE_UP);
    break;
   case PLAYLIST_MOVE_DOWN:
    LightAlloyCommandPost(LAC_PLAYLIST_MOVE_DOWN);
    break;
   case PLAYLIST_SHUFFLE:
    LightAlloyCommandPost(LAC_PLAYLIST_SHUFFLE);
    break;
   case PLAYLIST_SORT:
    LightAlloyCommandPost(LAC_PLAYLIST_SORT);
    break;
   case PLAYLIST_REPORT:
    LightAlloyCommandPost(LAC_PLAYLIST_REPORT);
    break;
   case PLAYLIST_REPEAT:
    LightAlloyCommandPost(LAC_PLAYLIST_REPEAT);
    break;
   case PLAYLIST_BOOKMARKS:
    LightAlloyCommandPost(LAC_PLAYLIST_BOOKMARKS);
    break;

   case VIDEO_PROPERTIES:
    LightAlloyCommandPost(LAC_VIDEO_PROPERTIES);
    break;
   case VIDEO_SCREENSHOT:
    LightAlloyCommandPost(LAC_VIDEO_SCREENSHOT);
    break;
   case VIDEO_SCALE_50:
    LightAlloyCommandPost(LAC_VIDEO_SCALE_50);
    break;
   case VIDEO_SCALE_100:
    LightAlloyCommandPost(LAC_VIDEO_SCALE_100);
    break;
   case VIDEO_SCALE_200:
    LightAlloyCommandPost(LAC_VIDEO_SCALE_200);
    break;
   case VIDEO_RATIO_ASIS:
    LightAlloyCommandPost(LAC_VIDEO_RATIO_ASIS);
    break;
   case VIDEO_RATIO_16_9:
    LightAlloyCommandPost(LAC_VIDEO_RATIO_16_9);
    break;
   case VIDEO_RATIO_4_3:
    LightAlloyCommandPost(LAC_VIDEO_RATIO_4_3);
    break;
   case VIDEO_RATIO_WIDTH:
    LightAlloyCommandPost(LAC_VIDEO_RATIO_WIDTH);
    break;
   case VIDEO_RATIO_HEIGHT:
    LightAlloyCommandPost(LAC_VIDEO_RATIO_HEIGHT);
    break;
   case VIDEO_RATIO_CUSTOM:
    LightAlloyCommandPost(LAC_VIDEO_RATIO_CUSTOM);
    break;
   case VIDEO_RATIO_FREE:
    LightAlloyCommandPost(LAC_VIDEO_RATIO_FREE);
    break;
   case VIDEO_ZOOM_IN:
    LightAlloyCommandPost(LAC_VIDEO_ZOOM_IN);
    break;
   case VIDEO_ZOOM_OUT:
    LightAlloyCommandPost(LAC_VIDEO_ZOOM_OUT);
    break;
   case VIDEO_BRIGHTNESS_INC:
    LightAlloyCommandPost(LAC_VIDEO_BRIGHTNESS_INC);
    break;
   case VIDEO_BRIGHTNESS_DEC:
    LightAlloyCommandPost(LAC_VIDEO_BRIGHTNESS_DEC);
    break;
   case VIDEO_CONTRAST_INC:
    LightAlloyCommandPost(LAC_VIDEO_CONTRAST_INC);
    break;
   case VIDEO_CONTRAST_DEC:
    LightAlloyCommandPost(LAC_VIDEO_CONTRAST_DEC);
    break;
   case VIDEO_SATURATION_INC:
    LightAlloyCommandPost(LAC_VIDEO_SATURATION_INC);
    break;
   case VIDEO_SATURATION_DEC:
    LightAlloyCommandPost(LAC_VIDEO_SATURATION_DEC);
    break;

   case SUBTITLES_LOAD:
    LightAlloyCommandPost(LAC_SUBTITLES_LOAD);
    break;
   case SUBTITLES_SHOW:
    LightAlloyCommandPost(LAC_SUBTITLES_SHOW);
    break;

   case SOUND_PROPERTIES: // ?
    LightAlloyCommandPost(LAC_SOUND_PROPERTIES);
    break;
   case SOUND_VOLUME_INC:
    LightAlloyCommandPost(LAC_SOUND_VOLUME_INC);
    break;
   case SOUND_VOLUME_DEC:
    LightAlloyCommandPost(LAC_SOUND_VOLUME_DEC);
    break;
   case SOUND_MUTE:
    LightAlloyCommandPost(LAC_SOUND_MUTE);
    break;
   case SOUND_ADD:
    LightAlloyCommandPost(LAC_SOUND_ADD);
    break;

   case APPLICATION_PREFERENCES:
    LightAlloyCommandPost(LAC_APPLICATION_PREFERENCES);
    break;
   case APPLICATION_HELP:
    LightAlloyCommandPost(LAC_APPLICATION_HELP);
    break;
   case APPLICATION_ABOUT:
    LightAlloyCommandPost(LAC_APPLICATION_ABOUT);
    break;
   case APPLICATION_EXIT:
    LightAlloyCommandPost(LAC_APPLICATION_EXIT);
    break;
   case APPLICATION_POWER_OFF:
    LightAlloyCommandPost(LAC_APPLICATION_POWER_OFF);
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
 PluginData->Caption = "LightAlloy v1.00 (02.06.2004г.)\n\n© <Quash!>, г.Екатеринбург)";

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