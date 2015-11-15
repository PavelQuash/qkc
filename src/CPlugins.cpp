/**************************************************************/
/*                qshKeyCommander1 - CPlugins.cpp             */
/*------------------------------------------------------------*/
/*     ������ ������ ������ � ������������� ������������.     */
/*      ����������/��������, ����������, �������� � �.�.      */
/*****03.2005************************Copyright by <Quash!>*****/

#include "CPlugins.h"

#define PL_SYMB      13                  // ������������ ���������� �������� � ����� ����� ��� �������� ��� � PlugList ini ����� (������ 8.3 + ������ ����� ������)
#define PL_SECTION   "qshKeyCommander_1" // ������ ������ ������
#define PL_KEYSIZE   "ActivePluginsSize" // ���� ������� �������
#define PL_KEYDATA   "ActivePluginsData" // ���� ������ �������

// �����������
CPlugins::CPlugins()
{
 // ��������� ��������
 PluginsData    = NULL;
 PluginsKeys    = NULL;
 PluginsCount   = 0;
 inifilename[0] = '\0';
 pluginsdir[0]  = '\0';
}

// ����������
CPlugins::~CPlugins()
{
 // ��� ������������ ���������� �����������
 for (int i=0; i<PluginsCount; i++)
  FreeLibrary(PluginsData[i].hDll);
 // ���������� ������ ������������ ������� � �� ������
 this->SavePlugList(PL_SECTION,PL_KEYSIZE,PL_KEYDATA);
/* DEBUG
 // ��������������� ������ PluginsKeys
 for (i=0; i<PluginsCount; i++)
  {
   PluginsKeys[i].KbrdKeys.DeInit();
   PluginsKeys[i].WircKeys.DeInit();
  }
 // ������������ ������� ��������
 if (PluginsData!=NULL) memfree(PluginsData);
 if (PluginsKeys!=NULL) memfree(PluginsKeys);
*/
 // ������������ ������� ��������
 if (PluginsData!=NULL) delete [] PluginsData;
 if (PluginsKeys!=NULL) delete [] PluginsKeys;
}

// ��������� ini-����� � �������� ��������
// true - �������; false - ���� �� ������ �� ��� ������
bool CPlugins::SetPaths(LPTSTR setini, LPTSTR setplugdir)
{
 // ��������� ini-�����
 if (FileExists(false,setini)) lstrcpy(inifilename,setini);
 else return false; // ���� �� ��������
 // ��������� �������� ��������
 if (FileExists(true,setplugdir)) lstrcpy(pluginsdir,setplugdir);
 else return false; // ���� �� ��������
 // �������� ������, ����������� ��� ������
 this->LoadPlugList(PL_SECTION,PL_KEYSIZE,PL_KEYDATA);
 return true; // �������
}

// ����������� ������
// true - �������; false - ������ ��� ��� ��������� ��� �� ����� ���� ������ (� ������ �� �����)
bool CPlugins::AddPlugin(LPTSTR filename)
{
 if ((inifilename[0]=='\0')||(pluginsdir[0]=='\0')) return false; // ������ �� ����� ���� ������
 char filepath[MAX_PATH]; // ������ ���� � �����
 int i,k;                 // ���������� ��� �����
 HMODULE hdll;            // ����� ����������� dll
 // �������� �������� ������ �� ������� ������������ ��������
 for (i=0; i<PluginsCount; i++)
  if (lstrcmp(PluginsData[i].FileName,filename)==0) return false; // ��� ���� � ������
 // �������� ����� �� �������������
 wsprintf(filepath,"%s\\%s",pluginsdir,filename);
 if (!FileExists(0,filepath)) return false; // ���� �� ����������
 // �������� �� ������� ����������� ��������
 hdll = LoadLibrary(filepath);
 if (hdll==NULL) return false; // dll �� ����� ���� ���������
 // ����������� ������� ������� GetModuleData
 GMD_Type GetModuleData;
 GetModuleData = (GMD_Type)GetProcAddress(hdll,"GetModuleData");
 if (GetModuleData==NULL)
  {
   FreeLibrary(hdll);
   return false; // dll �� ����� ���� ���������
  }
 // ����� ��� � ������, �� ����������, �����������, ���� ������� => ���������� � ������
 stPluginData *PluginsDataTemp;
 stPluginKeys *PluginsKeysTemp;
 // �������� ������� ������� � ����� ����������� ��������� (+1)
 PluginsDataTemp = new stPluginData[PluginsCount+1];
 PluginsKeysTemp = new stPluginKeys[PluginsCount+1];
/* DEBUG
 PluginsDataTemp = (stPluginData*)memalloc((PluginsCount+1)*sizeof(stPluginData));
 PluginsKeysTemp = (stPluginKeys*)memalloc((PluginsCount+1)*sizeof(stPluginKeys));
 // ������������� ������ PluginsKeysTemp
 for (i=0; i<(PluginsCount+1); i++)
 {
  PluginsKeysTemp[i].KbrdKeys.Init();
  PluginsKeysTemp[i].WircKeys.Init();
 }
*/
 // ����������� ���� ��������� ������� ������ � �����
 for (i=0; i<PluginsCount; i++)
  {
   // ��� ����� ������
   lstrcpy(PluginsDataTemp[i].FileName,PluginsData[i].FileName);
   // ����� ������
   PluginsDataTemp[i].hDll=PluginsData[i].hDll;
   // ��������� ������
   PluginsDataTemp[i].Caption=PluginsData[i].Caption;
   // ����������� ������� ���������� �������
   PluginsDataTemp[i].Init          = PluginsData[i].Init;
   PluginsDataTemp[i].Quit          = PluginsData[i].Quit;
   PluginsDataTemp[i].Config        = PluginsData[i].Config;
   PluginsDataTemp[i].Command       = PluginsData[i].Command;
   PluginsDataTemp[i].CommandsCount = PluginsData[i].CommandsCount;
   PluginsDataTemp[i].CommandsList  = PluginsData[i].CommandsList;
   // ����������� ������ Keyboard
   stKbrdKey KbrdKey;
   for (k=0; k<PluginsKeys[i].KbrdKeys.GetKeysCount(); k++)
    {
     PluginsKeys[i].KbrdKeys.GetKey(k,&KbrdKey);   // ���������� k ��������
     PluginsKeysTemp[i].KbrdKeys.AddKey(&KbrdKey); // ���������� ���������� ��������
    }
   // ����������� ������ WinLIRC
   stWircKey WircKey;
   for (k=0; k<PluginsKeys[i].WircKeys.GetKeysCount(); k++)
    {
     PluginsKeys[i].WircKeys.GetKey(k,&WircKey);   // ���������� k ��������
     PluginsKeysTemp[i].WircKeys.AddKey(&WircKey); // ���������� ���������� ��������
    }
  }
/* DEBUG
 // ��������������� ������� ������ PluginsKeys
 for (i=0; i<PluginsCount; i++)
  {
   PluginsKeys[i].KbrdKeys.DeInit();
   PluginsKeys[i].WircKeys.DeInit();
  }
 // ������������ ������ ������ �������
 if (PluginsData!=NULL) memfree(PluginsData);
 if (PluginsKeys!=NULL) memfree(PluginsKeys);
*/
 // ������������ ������ ������ �������
 if (PluginsData!=NULL) delete [] PluginsData;
 if (PluginsKeys!=NULL) delete [] PluginsKeys;
 // ��������������� ���������� ����� �������
 PluginsData=PluginsDataTemp;
 PluginsKeys=PluginsKeysTemp;
 // ����������� �������� ������ ��������
 lstrcpy(PluginsData[PluginsCount].FileName,filename); // ���������� ����� �����
 PluginsData[PluginsCount].hDll = hdll;                // ����������� ������ ������
 // ��������� ������� ��� ������ �� ini �����
 PluginsKeys[PluginsCount].KbrdKeys.LoadKeys(inifilename,PluginsData[PluginsCount].FileName,KBRD_KEYSIZE,KBRD_KEYDATA);
 PluginsKeys[PluginsCount].WircKeys.LoadKeys(inifilename,PluginsData[PluginsCount].FileName,WIRC_KEYSIZE,WIRC_KEYDATA);
 // �������� �� ���������
 PluginsData[PluginsCount].Init          = NULL;
 PluginsData[PluginsCount].Quit          = NULL;
 PluginsData[PluginsCount].Config        = NULL;
 PluginsData[PluginsCount].Command       = 0;
 PluginsData[PluginsCount].CommandsCount = 0;
 // ����� ����������� ������� � ���������� ���������� �������� �������
 GetModuleData(&PluginsData[PluginsCount]);
 // ��������� �� ������� ��������
 PluginsCount++;
 return true; // �������
}

// ���������� ������
// true - �������� �������; false - ������� �� ������
bool CPlugins::DelPlugin(int num)
{
 // �������� �� ����������� num
 if ((num<0)||(num>=PluginsCount)) return false; // ������� �� ������
 
 // ���������� ������ ���������� ������
 PluginsKeys[num].KbrdKeys.SaveKeys(inifilename,PluginsData[num].FileName,KBRD_KEYSIZE,KBRD_KEYDATA);
 PluginsKeys[num].WircKeys.SaveKeys(inifilename,PluginsData[num].FileName,WIRC_KEYSIZE,WIRC_KEYDATA);

 // ���������� ���������� ������ �� ������
 FreeLibrary(PluginsData[num].hDll);

 int i,k,l=0; // ���������� ��� �����
 stPluginData *PluginsDataTemp;
 stPluginKeys *PluginsKeysTemp;
 // �������� ������ � ����� ����������� ��������� (-1)
 PluginsDataTemp = new stPluginData[PluginsCount-1];
 PluginsKeysTemp = new stPluginKeys[PluginsCount-1];
/* DEBUG
 PluginsDataTemp = (stPluginData*)memalloc((PluginsCount-1)*sizeof(stPluginData));
 PluginsKeysTemp = (stPluginKeys*)memalloc((PluginsCount-1)*sizeof(stPluginKeys));
 // ������������� ������ PluginsKeysTemp
 for (i=0; i<(PluginsCount-1); i++)
  {
   PluginsKeysTemp[i].KbrdKeys.Init();
   PluginsKeysTemp[i].WircKeys.Init();
  }
*/
 // ����������� ���� ��������� ������� ������ � �����
 for (i=0; i<PluginsCount; i++)
  {
   if (i==num) {l++; continue;}
   lstrcpy(PluginsDataTemp[i-l].FileName,PluginsData[i].FileName); // ��� ����� ������
   PluginsDataTemp[i-l].hDll          = PluginsData[i].hDll;       // ����� ������
   PluginsDataTemp[i-l].Caption       = PluginsData[i].Caption;    // ��������� ������
   // ����������� ������� ���������� �������
   PluginsDataTemp[i-l].Init          = PluginsData[i].Init;
   PluginsDataTemp[i-l].Quit          = PluginsData[i].Quit;
   PluginsDataTemp[i-l].Config        = PluginsData[i].Config;
   PluginsDataTemp[i-l].Command       = PluginsData[i].Command;
   PluginsDataTemp[i-l].CommandsCount = PluginsData[i].CommandsCount;
   PluginsDataTemp[i-l].CommandsList  = PluginsData[i].CommandsList;
   // ����������� ������ Keyboard
   stKbrdKey KbrdKey;
   for (k=0; k<PluginsKeys[i].KbrdKeys.GetKeysCount(); k++)
    {
     PluginsKeys[i].KbrdKeys.GetKey(k,&KbrdKey);     // ���������� k ��������
     PluginsKeysTemp[i-l].KbrdKeys.AddKey(&KbrdKey); // ���������� ���������� ��������
    }
   // ����������� ������ WinLIRC
   stWircKey WircKey;
   for (k=0; k<PluginsKeys[i].WircKeys.GetKeysCount(); k++)
    {
     PluginsKeys[i].WircKeys.GetKey(k,&WircKey);     // ���������� k ��������
     PluginsKeysTemp[i-l].WircKeys.AddKey(&WircKey); // ���������� ���������� ��������
    }
  }
/* DEBUG
 // ��������������� ������� ������ PluginsKeys
 for (i=0; i<PluginsCount; i++)
  {
   PluginsKeys[i].KbrdKeys.DeInit();
   PluginsKeys[i].WircKeys.DeInit();
  }
 // ������������ ������ ������ �������
 if (PluginsData!=NULL) memfree(PluginsData);
 if (PluginsKeys!=NULL) memfree(PluginsKeys);
*/
 // ������������ ������ ������ �������
 if (PluginsData!=NULL) delete [] PluginsData;
 if (PluginsKeys!=NULL) delete [] PluginsKeys;
 // ��������������� ���������� ����� �������
 PluginsData=PluginsDataTemp;
 PluginsKeys=PluginsKeysTemp;
 // ��������� �� ������� �������� 
 PluginsCount--;
 return true; // �������
}

// ������ num �������
bool CPlugins::GetPluginData(int num, stPluginData *getplugindata)
{
 // �������� �� ������������ num
 if ((num<0)||(num>=PluginsCount)) return false; // ������� �� ������
 lstrcpy(getplugindata->FileName,PluginsData[num].FileName);
 getplugindata->hDll          = PluginsData[num].hDll;
 getplugindata->Caption       = PluginsData[num].Caption;
 getplugindata->Init          = PluginsData[num].Init;
 getplugindata->Quit          = PluginsData[num].Quit;
 getplugindata->Config        = PluginsData[num].Config;
 getplugindata->Command       = PluginsData[num].Command;
 getplugindata->CommandsCount = PluginsData[num].CommandsCount;
 getplugindata->CommandsList  = PluginsData[num].CommandsList;
 return true; // �������
}

// ����������� ���������� ������������ �������
int CPlugins::GetPluginsCount(void)
{ return PluginsCount; }

// ����� ������ �� ����� �����
int  CPlugins::FindByFileName(LPTSTR filename)
{
 for (int i=0; i<PluginsCount; i++)
  if (lstrcmp(PluginsData[i].FileName,filename)==0) return i;
 return (-1);
}

// ����� ������ �� ������ ������
int  CPlugins::FindByHandle(HINSTANCE hdll)
{
 for (int i=0; i<PluginsCount; i++)
  if (PluginsData[i].hDll==hdll) return i;
 return (-1);
}

// ��������� ������ ������������ �������
bool CPlugins::LoadPlugList(LPTSTR section, LPTSTR keysize, LPTSTR keydata)
{
 int inttemp;
 // ������ ���������� ��������� � ������
 inttemp = GetPrivateProfileInt(section,keysize,-1,inifilename);
 // ���� � ini-����� ���������� ������� >0, �� - ��������� ������ � ���������� ������
 if (inttemp>0)
  {
   char (*pluglist) [PL_SYMB]; // �������� ���������
   char filename[MAX_PATH];
   //DEBUG: pluglist = (char (*)[13])memalloc(inttemp*PL_SYMB);
   pluglist = new char [inttemp][PL_SYMB]; // ��������� ������ ��� ������ ������������ �������
   // ������ ������ ������
   GetPrivateProfileStruct(section,keydata,pluglist,sizeof(char [PL_SYMB])*inttemp,inifilename);
   // ������������ ���������� ��� ������ �� ������� 8.3
   for (int i=0; i<inttemp; i++)
    {
     GetLongFileName(pluginsdir,pluglist[i],filename);
     // ���� ���� �� ����������, �� � AddPlugin �� ������������� � ���� ������������
     this->AddPlugin(filename);
    }
   //DEBUG: memfree(pluglist);
   delete [] pluglist; // ������������ ������ ������
  }
/* DEBUG [???]
 else // debug ������������ ������ (�� ������ ������, ���� ����� �� ������)
  {
   // ��������������� ������ PluginsKeysTemp
   for (int i=0; i<PluginsCount; i++)
	{
     PluginsKeys[i].KbrdKeys.DeInit();
     PluginsKeys[i].WircKeys.DeInit();
	}
   // ����������� ������ � ��������
   memfree(PluginsData);
   memfree(PluginsKeys);
   PluginsData=NULL;
   PluginsKeys=NULL;
   PluginsCount=0;
  }
*/
 return true; // �������
}

// ��������� ������ ������������ �������
bool CPlugins::SavePlugList(LPTSTR section, LPTSTR keysize, LPTSTR keydata)
{
 if ((inifilename[0]=='\0')||(pluginsdir[0]=='\0')) return false; // ������ �� ����� ���� ������
 char (*pluglist)[PL_SYMB]; // �������� ���������
 int i;                     // ��� �����
 // ��������� ������ ��� ������ ������������ �������
 // DEBUG: pluglist = (char (*)[13])memalloc(PluginsCount*PL_SYMB);
 pluglist = new char [PluginsCount][PL_SYMB];
 // ��������� ������ ������������ �������
 char fpath[MAX_PATH],
      spath[MAX_PATH],
      filename[13];
 for (i=0; i<PluginsCount; i++)
  {
   // ������������ ��������� ����� ����� � ������ 8.3
   wsprintf(fpath,"%s\\%s",pluginsdir,PluginsData[i].FileName); // ������������ ������� ���� � ����� � fpath
   GetShortPathName(fpath,spath,sizeof(spath));                 // ������������ ��������� ���� � ������� 8.3 � spath
   GetFileTitle(spath,filename,sizeof(filename));               // ��������� ������ ����� ����� � filename
   wsprintf(pluglist[i],"%s",filename);                         // ���������� filename � ������
   // ���������� ������ �������� ������ i
   PluginsKeys[i].KbrdKeys.SaveKeys(inifilename,PluginsData[i].FileName,KBRD_KEYSIZE,KBRD_KEYDATA);
   PluginsKeys[i].WircKeys.SaveKeys(inifilename,PluginsData[i].FileName,WIRC_KEYSIZE,WIRC_KEYDATA);
  }

 // ���������� � ini ����
 char str[10];
 wsprintf(str,"%i",PluginsCount);
 WritePrivateProfileString(section,keysize,str,inifilename);
 WritePrivateProfileStruct(section,keydata,pluglist,sizeof(char [PL_SYMB])*PluginsCount,inifilename);
 // ����������� ������
 // DEBUG: memfree(pluglist);
 delete [] pluglist;
 return true; // �������
}
