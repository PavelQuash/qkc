/**************************************************************/
/*                 qshKeyCommander1 - main.cpp                */
/*------------------------------------------------------------*/
/*    ������ ���������� Light Alloy ��� qshKeyCommander1      */
/*****2004***************************Copyright by <Quash!>*****/

#include "windows.h"
#include "..\qkc_plugin.h"
#include "LightAlloy_ids.h"

#define LightAlloy_ClassName "LightAlloyFront"

// *���������� Runtime Library
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:DllMain")
#pragma comment(linker,"/NODEFAULTLIB")



#define CMDCOUNT 69
int  CmdCount = CMDCOUNT;    // ���������� ������, ������������ � �������
char CmdList[CMDCOUNT][200]; // ������ ������

// �����������/���������� ������
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
 switch (ul_reason_for_call)
  {
   case DLL_PROCESS_ATTACH:
    // *���������� ������
    // ����
    lstrcpy(CmdList[FILE_OPEN]               ,"������� ����");
    lstrcpy(CmdList[FILE_INFO]               ,"���������� � �����");
    // ���������������
    lstrcpy(CmdList[PLAYBACK_STOP]           ,"����");
    lstrcpy(CmdList[PLAYBACK_PLAY]           ,"���������������");
    lstrcpy(CmdList[PLAYBACK_STOP_PLAY]      ,"���������������/����");
    lstrcpy(CmdList[PLAYBACK_SPEED_PLAY]     ,"���������� ���������������");
    lstrcpy(CmdList[PLAYBACK_FILTERS]        ,"������� DirectShow");
    // ���������
    lstrcpy(CmdList[SEEK_FRAME_STEP]         ,"���� �����");
    lstrcpy(CmdList[SEEK_FRAME_BACK]         ,"���� �����");
    lstrcpy(CmdList[SEEK_FORWARD]            ,"���������� �����");
    lstrcpy(CmdList[SEEK_BACKWARD]           ,"���������� �����");
    lstrcpy(CmdList[SEEK_JUMP_FORWARD]       ,"������ �����");
    lstrcpy(CmdList[SEEK_JUMP_BACKWARD]      ,"������ �����");
    lstrcpy(CmdList[SEEK_REWIND]             ,"���������� � ������");
    lstrcpy(CmdList[SEEK_SET_BOOKMARK]       ,"���������� ��������");
    // ����
    lstrcpy(CmdList[WINDOW_CONTROL_PANEL]    ,"��������/������ ������ ����������");
    lstrcpy(CmdList[WINDOW_PLAYLIST]         ,"��������/������ ������");
    lstrcpy(CmdList[WINDOW_FULLSCREEN]       ,"������������ ����/���� �����");
    lstrcpy(CmdList[WINDOW_ORIGINAL]         ,"������������");
    lstrcpy(CmdList[WINDOW_STAY_ON_TOP]      ,"������ ���� ����");
    lstrcpy(CmdList[WINDOW_MINIMIZE]         ,"��������������/������������");
    lstrcpy(CmdList[WINDOW_MAXIMIZE]         ,"���������������/������������");

    // ������
    lstrcpy(CmdList[PLAYLIST_NEXT]           ,"��������� ������� �� ������");
    lstrcpy(CmdList[PLAYLIST_PREV]           ,"���������� ������� �� ������");
    lstrcpy(CmdList[PLAYLIST_PLAY]           ,"��������������� ����������");
    lstrcpy(CmdList[PLAYLIST_ADD_FILES]      ,"�������� ����(�) � ������");
    lstrcpy(CmdList[PLAYLIST_ADD_FOLDER]     ,"�������� ����� � ������");
    lstrcpy(CmdList[PLAYLIST_DELETE]         ,"������� ������� �� ������");
    lstrcpy(CmdList[PLAYLIST_CLEAR]          ,"�������� ������");
    lstrcpy(CmdList[PLAYLIST_SAVE]           ,"��������� ������");
    lstrcpy(CmdList[PLAYLIST_MOVE_UP]        ,"����������� ������� ����");
    lstrcpy(CmdList[PLAYLIST_MOVE_DOWN]      ,"����������� ������� ����");
    lstrcpy(CmdList[PLAYLIST_SHUFFLE]        ,"���������� ������");
    lstrcpy(CmdList[PLAYLIST_SORT]           ,"����������� ������");
    lstrcpy(CmdList[PLAYLIST_REPORT]         ,"������� �� ������");
    lstrcpy(CmdList[PLAYLIST_REPEAT]         ,"������������ ������");
    lstrcpy(CmdList[PLAYLIST_BOOKMARKS]      ,"��������/������ �������� � ������");
    // �����
    lstrcpy(CmdList[VIDEO_PROPERTIES]        ,"��������� �����");
    lstrcpy(CmdList[VIDEO_SCREENSHOT]        ,"��������� ����");
    lstrcpy(CmdList[VIDEO_SCALE_50]          ,"�������  50%");
    lstrcpy(CmdList[VIDEO_SCALE_100]         ,"������� 100%");
    lstrcpy(CmdList[VIDEO_SCALE_200]         ,"������� 200%");
    lstrcpy(CmdList[VIDEO_RATIO_ASIS]        ,"��������� \"��� ����\"");
    lstrcpy(CmdList[VIDEO_RATIO_16_9]        ,"��������� 16:9");
    lstrcpy(CmdList[VIDEO_RATIO_4_3]         ,"���������  4:3");
    lstrcpy(CmdList[VIDEO_RATIO_WIDTH]       ,"��������� �� ������");
    lstrcpy(CmdList[VIDEO_RATIO_HEIGHT]      ,"��������� �� ������");
    lstrcpy(CmdList[VIDEO_RATIO_CUSTOM]      ,"������ ���������");
    lstrcpy(CmdList[VIDEO_RATIO_FREE]        ,"��� ���������� ���������");
    lstrcpy(CmdList[VIDEO_ZOOM_IN]           ,"��������� �����������");
    lstrcpy(CmdList[VIDEO_ZOOM_OUT]          ,"��������� �����������");
    lstrcpy(CmdList[VIDEO_BRIGHTNESS_INC]    ,"��������� �������");
    lstrcpy(CmdList[VIDEO_BRIGHTNESS_DEC]    ,"������� �������");
    lstrcpy(CmdList[VIDEO_CONTRAST_INC]      ,"��������� ��������");
    lstrcpy(CmdList[VIDEO_CONTRAST_DEC]      ,"������� ��������");
    lstrcpy(CmdList[VIDEO_SATURATION_INC]    ,"��������� ������������");
    lstrcpy(CmdList[VIDEO_SATURATION_DEC]    ,"������� ������������");
    // ��������
    lstrcpy(CmdList[SUBTITLES_LOAD]          ,"��������� ��������");
    lstrcpy(CmdList[SUBTITLES_SHOW]          ,"�������� ��������");
    // ����
    lstrcpy(CmdList[SOUND_PROPERTIES]        ,"��������� �����");
    lstrcpy(CmdList[SOUND_VOLUME_INC]        ,"��������� ���������");
    lstrcpy(CmdList[SOUND_VOLUME_DEC]        ,"��������� ���������");
    lstrcpy(CmdList[SOUND_MUTE]              ,"��������� ����");
    lstrcpy(CmdList[SOUND_ADD]               ,"��������� �������������� ����");
    // ���������
    lstrcpy(CmdList[APPLICATION_PREFERENCES] ,"���������");
    lstrcpy(CmdList[APPLICATION_HELP]        ,"������");
    lstrcpy(CmdList[APPLICATION_ABOUT]       ,"� ���������");
    lstrcpy(CmdList[APPLICATION_EXIT]        ,"�����");
    lstrcpy(CmdList[APPLICATION_POWER_OFF]   ,"��������� ���������");
    break;

   case DLL_PROCESS_DETACH:
    break;
  }
 return true;
}

// ������� �������������
//void Init(void)
//{ MessageBox(0,"MyPlugin Init","Info:",0); }

// ������� ���������������
//void Quit(void)
//{ MessageBox(0,"MyPlugin Quit","Info:",0); }

// ������������ �������
//void Config(void)
//{ MessageBox(0,"MyPlugin Config","Info:",0); }

// ����������� ��������� ���� LightAlloy
BOOL LightAlloyCommandPost(int command)
{
 HWND hwndLA;
 // ���� ���� ���������
 hwndLA = FindWindow(LightAlloy_ClassName,NULL);
 if (hwndLA)
  { 
   // �������� ���� ���������
   PostMessage(hwndLA,WM_LACMD,command,0);
   return true;
  }
 return false;
}

// ������ �� ���������� ������� ��� ������� cmdid
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

   // ������� �� ����������� ��������� (�� ������ ������)
   default:
    break;
  }
}



#ifdef __cplusplus
extern "C"
{
#endif

// ������� �� �������, ���������� �������� �� ���������
__declspec (dllexport) void GetModuleData(struct stPluginData *PluginData)
{
 // ���������� ���������
 PluginData->Caption = "LightAlloy v1.00 (02.06.2004�.)\n\n� <Quash!>, �.������������)";

 // �������� ������� ������� qshKeyCommander'�
 PluginData->Init          = NULL;//Init;
 PluginData->Quit          = NULL;//Quit;
 PluginData->Config        = NULL;//Config;
 PluginData->Command       = Command;

 // �������� ������� ������ � ���������� ��� ��������� � ������
 PluginData->CommandsCount = &CmdCount;
 PluginData->CommandsList  = CmdList;
}

#ifdef __cplusplus
}
#endif