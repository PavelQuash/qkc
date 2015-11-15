/**************************************************************/
/*              qshKeyCommander1 - CWircKeys.h                */
/*------------------------------------------------------------*/
/* ������ ������ �������� ������ WinLIRC ������. ����������,  */
/*   ��������, ���������, �����, ��������/���������� � ����   */
/*                           ���������                        */
/*****03.2005************************Copyright by <Quash!>*****/

#include "CWircKeys.h"

// �����������
CWircKeys::CWircKeys()
{
 // ����� ��������� ��������
 KeysData=NULL;
 KeysCount=0;
}

// ����������
CWircKeys::~CWircKeys()
{
 // ����������� ������
 if (KeysData!=NULL) memfree(KeysData);
}

// ����������� ����� �������
void CWircKeys::AddKey(stWircKey *newwirckey)
{
 int i; // ���������� ��� �����
 // �������� ������ � ����� ����������� ��������� (+1)
 stWircKey *KeysDataTemp = (stWircKey*)memalloc((KeysCount+1)*sizeof(stWircKey));
 // ����������� ���� ��������� ������� ������ � �����
 for (i=0; i<KeysCount; i++)
  {
   lstrcpy(KeysDataTemp[i].WircKey,KeysData[i].WircKey);
   KeysDataTemp[i].Repeats = KeysData[i].Repeats;
   KeysDataTemp[i].CmdID   = KeysData[i].CmdID;
  }
 // ������������ ������ ������� ������
 memfree(KeysData);
 // ��������������� ��������� ������ ������
 KeysData=KeysDataTemp;
 // ����������� �������� ������ ��������
 lstrcpy(KeysData[KeysCount].WircKey,newwirckey->WircKey);
 KeysData[KeysCount].Repeats = newwirckey->Repeats;
 KeysData[KeysCount].CmdID   = newwirckey->CmdID;
 // ��������� �� ������� ��������
 KeysCount++;
}

// ��������� ������� � �������� num
// true - �������; false - ������� �� ������
bool CWircKeys::DelKey(int num)
{
 // �������� �� ����������� num
 if ((num<0)||(num>=KeysCount)) return false; // ������� �� ������
 int i,k=0; // ���������� ��� �����
 // �������� ������ � ����� ����������� ��������� (-1)
 stWircKey *KeysDataTemp = (stWircKey*)memalloc((KeysCount-1)*sizeof(stWircKey));
 // ����������� ���� ��������� ������� ������ � �����,
 // ��� ���� num ������� �������������
 for (i=0; i<KeysCount; i++)
  {
   // ����������� �����, ���� i=num
   if (i==num) {k++; continue;}
   lstrcpy(KeysDataTemp[i-k].WircKey,KeysData[i].WircKey);
   KeysDataTemp[i-k].Repeats = KeysData[i].Repeats;
   KeysDataTemp[i-k].CmdID   = KeysData[i].CmdID;
  }
 // ������������ ������ ������� ������
 memfree(KeysData);
 // ��������������� ��������� ������ ������
 KeysData=KeysDataTemp;
 // ��������� �� ������� ��������
 KeysCount--;
 return true; // �������
}

// ��������� ������ �������� ��� ������� num 
// true - �������; false - ������� �� ������
bool CWircKeys::ChangeKey(int num, stWircKey *newwirckey)
{
 // �������� �� ������������ num
 if ((num<0)||(num>=KeysCount)) return false; // ������� �� ������
 // ��������� ������� ��������
 lstrcpy(KeysData[num].WircKey,newwirckey->WircKey);
 KeysData[num].Repeats = newwirckey->Repeats;
 KeysData[num].CmdID   = newwirckey->CmdID;
 return true; // �������
}

// ���������� ������� c �������� num
// true - �������; false - ������� �� ������
bool CWircKeys::GetKey(int num, stWircKey *getwirckey)
{
 // �������� �� ������������ num
 if ((num<0)||(num>=KeysCount)) return false; // ������� �� ������
 lstrcpy(getwirckey->WircKey,KeysData[num].WircKey);
 getwirckey->Repeats = KeysData[num].Repeats;
 getwirckey->CmdID   = KeysData[num].CmdID;
 return true; // �������
}

// ���������� ���������� ��������� � ������
int CWircKeys::GetKeysCount(void)
{ return KeysCount; }

// ���������� ����� ������� �������� �� ���� �������
int CWircKeys::FindByKey(char wirckey[80])
{
 // ����� � ������ �������
 for (int i=0; i<KeysCount; i++)
  if (lstrcmp(KeysData[i].WircKey,wirckey)==0) return i;
 return (-1);
}

// ���������� ����� ������� �������� �� id �������
int CWircKeys::FindByCmdID(int cmdid)
{
 // ����� � ������ id �������
 for (int i=0; i<KeysCount; i++)
  if (KeysData[i].CmdID == cmdid) return i;
 return (-1);
}

// �������� ������ ������ �� ini �����
void CWircKeys::LoadKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata) 
{
 int inttemp;
 // ������ ���������� ��������� � ������
 inttemp = GetPrivateProfileInt(section,keysize,-1,inifilename);
 // ���� <=0, �� �������� ������
 if (inttemp<=0)
  {
   // ����������� ������ � ��������
   memfree(KeysData);
   KeysData=NULL;
   KeysCount=0;
  }
 // ����� �������� ������ � ��������� ������
 else
  {
   // ���� ���������� ��������� �� ��������� � �������, 
   // �� �������� ������ ������ (������ ������ ��������)
   if (KeysCount!=inttemp)
    {
	 memfree(KeysData);
     KeysData = (stWircKey*)memalloc(inttemp*sizeof(stWircKey));
    }   
   // ������ ������
   GetPrivateProfileStruct(section,keydata,KeysData,sizeof(stWircKey)*inttemp,inifilename);
   // ����� ���������� ���������
   KeysCount=inttemp;
  }
}

// ���������� ������ ������ � ini ����
void CWircKeys::SaveKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata)
{
 // �������������� ����� � ��������� ���
 char strsize[10];
 wsprintf(strsize,"%i",KeysCount);
 // ���������� ���������� ��������� ������ ������
 WritePrivateProfileString(section,keysize,strsize,inifilename);
 // ���������� ������ ������ ������
 WritePrivateProfileStruct(section,keydata,KeysData,sizeof(stWircKey)*KeysCount,inifilename);
}