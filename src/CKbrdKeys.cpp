/**************************************************************/
/*             qshKeyCommander1 - CKbrdKeys.cpp               */
/*------------------------------------------------------------*/
/*      ������ ������ �������� ������������ ������ ������.    */
/*           ����������, ��������, ���������, �����,          */
/*            ��������/���������� � ���� ���������            */
/*****03.2005************************Copyright by <Quash!>*****/

#include "CKbrdKeys.h"

// �����������
CKbrdKeys::CKbrdKeys()
{
 // ������� ��������� ��������
 KeysData=NULL;
 KeysCount=0;
}

// ����������
CKbrdKeys::~CKbrdKeys()
{
 // ������������ ������
 if (KeysData!=NULL) memfree(KeysData);
}

// ����������� ����� �������
void CKbrdKeys::AddKey(stKbrdKey *newkbrdkey)
{
 int i; // ���������� ��� �����
 // �������� ������ � ����� ����������� ��������� (+1)
 stKbrdKey *KeysDataTemp = (stKbrdKey*)memalloc((KeysCount+1)*sizeof(stKbrdKey)); 
 // ����������� ���� ��������� ������� ������ � �����
 for (i=0; i<KeysCount; i++)
  {
   KeysDataTemp[i].KbrdKey = KeysData[i].KbrdKey;
   KeysDataTemp[i].CNS     = KeysData[i].CNS;
   KeysDataTemp[i].CmdID   = KeysData[i].CmdID;
  }
 // ������������ ������ ������� ������
 memfree(KeysData);
 // ��������������� ��������� ������ ������
 KeysData=KeysDataTemp;
 // ����������� �������� ������ ��������
 KeysData[KeysCount].KbrdKey = newkbrdkey->KbrdKey;
 KeysData[KeysCount].CNS     = newkbrdkey->CNS;
 KeysData[KeysCount].CmdID   = newkbrdkey->CmdID;
 // ��������� �� ������� ��������
 KeysCount++;
}

// ��������� ������� � �������� num
// true - �������; false - ������� �� ������
bool CKbrdKeys::DelKey(int num)
{
 // �������� �� ����������� num
 if ((num<0)||(num>=KeysCount)) return false; // ������� �� ������
 int i,k=0; // ���������� ��� �����
 // �������� ������ � ����� ����������� ��������� (-1)
 stKbrdKey *KeysDataTemp = (stKbrdKey*)memalloc((KeysCount-1)*sizeof(stKbrdKey));   
 // ����������� ���� ��������� ������� ������ � �����,
 // ��� ���� num ������� �������������
 for (i=0; i<KeysCount; i++)
  {
   // ����������� �����, ���� i=num
   if (i==num) {k++; continue;}
   KeysDataTemp[i-k].KbrdKey = KeysData[i].KbrdKey;
   KeysDataTemp[i-k].CNS     = KeysData[i].CNS;
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
bool CKbrdKeys::ChangeKey(int num, stKbrdKey *newkbrdkey)
{
 // �������� �� ������������ num
 if ((num<0)||(num>=KeysCount)) return false; // ������� �� ������
 // ��������� ������� ��������
 KeysData[num].KbrdKey = newkbrdkey->KbrdKey;
 KeysData[num].CNS     = newkbrdkey->CNS;
 KeysData[num].CmdID   = newkbrdkey->CmdID;
 return true; // �������
}

// ���������� ������� c �������� num
// true - �������; false - ������� �� ������
bool CKbrdKeys::GetKey(int num, stKbrdKey *getkbrdkey)
{
 // �������� �� ������������ num
 if ((num<0)||(num>=KeysCount)) return false; // ������� �� ������
 getkbrdkey->KbrdKey = KeysData[num].KbrdKey;
 getkbrdkey->CNS     = KeysData[num].CNS;
 getkbrdkey->CmdID   = KeysData[num].CmdID;
 return true; // �������
}

// ���������� ���������� ��������� � ������
int CKbrdKeys::GetKeysCount(void)
{ return KeysCount; }

// ���������� ����� ������� �������� �� ���� �������
int CKbrdKeys::FindByKey(WORD kbrdkey)
{
 // ����� � ������ �������
 for (int i=0; i<KeysCount; i++)
  if (KeysData[i].KbrdKey == kbrdkey) return i;
 return (-1);
}

// ���������� ����� ������� �������� �� id �������
int CKbrdKeys::FindByCmdID(int cmdid)
{
 // ����� � ������ id �������
 for (int i=0; i<KeysCount; i++)
  if (KeysData[i].CmdID == cmdid) return i;
 return (-1);
}

// �������� ������ ������ �� ini �����
void CKbrdKeys::LoadKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata) 
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
     KeysData = (stKbrdKey*)memalloc(inttemp*sizeof(stKbrdKey)); 
    }   
   // ������ ������
   GetPrivateProfileStruct(section,keydata,KeysData,sizeof(stKbrdKey)*inttemp,inifilename);
   // ����� ���������� ���������
   KeysCount=inttemp;
  }
}

// ���������� ������ ������ � ini ����
void CKbrdKeys::SaveKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata)
{
 // �������������� ����� � ��������� ���
 char strsize[10];
 wsprintf(strsize,"%i",KeysCount);
 // ���������� ���������� ��������� ������ ������
 WritePrivateProfileString(section,keysize,strsize,inifilename);
 // ���������� ������ ������ ������
 WritePrivateProfileStruct(section,keydata,KeysData,sizeof(stKbrdKey)*KeysCount,inifilename);
}
