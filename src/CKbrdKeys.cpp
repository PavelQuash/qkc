/**************************************************************/
/*             qshKeyCommander1 - CKbrdKeys.cpp               */
/*------------------------------------------------------------*/
/*      Методы класса хранения клавиатурных клавиш модуля.    */
/*           Добавление, удаление, изменение, поиск,          */
/*            загрузка/сохранение в файл элементов            */
/*****03.2005************************Copyright by <Quash!>*****/

#include "CKbrdKeys.h"

// Конструктор
CKbrdKeys::CKbrdKeys()
{
 // Задание начальных значений
 KeysData=NULL;
 KeysCount=0;
}

// Деструктор
CKbrdKeys::~CKbrdKeys()
{
 // Освобождение памяти
 if (KeysData!=NULL) memfree(KeysData);
}

// Добавляется новый элемент
void CKbrdKeys::AddKey(stKbrdKey *newkbrdkey)
{
 int i; // Переменная для цикла
 // Создание списка с новым количеством элементов (+1)
 stKbrdKey *KeysDataTemp = (stKbrdKey*)memalloc((KeysCount+1)*sizeof(stKbrdKey)); 
 // Копирование всех элементов старого списка в новый
 for (i=0; i<KeysCount; i++)
  {
   KeysDataTemp[i].KbrdKey = KeysData[i].KbrdKey;
   KeysDataTemp[i].CNS     = KeysData[i].CNS;
   KeysDataTemp[i].CmdID   = KeysData[i].CmdID;
  }
 // Освобождение памяти старого списка
 memfree(KeysData);
 // Присваиваивание указателя нового списка
 KeysData=KeysDataTemp;
 // Определение значений нового элемента
 KeysData[KeysCount].KbrdKey = newkbrdkey->KbrdKey;
 KeysData[KeysCount].CNS     = newkbrdkey->CNS;
 KeysData[KeysCount].CmdID   = newkbrdkey->CmdID;
 // Инкремент на единицу счётчика
 KeysCount++;
}

// Удаляется элемент с индексом num
// true - успешно; false - элемент не найден
bool CKbrdKeys::DelKey(int num)
{
 // Проверка на допутимость num
 if ((num<0)||(num>=KeysCount)) return false; // Элемент не найден
 int i,k=0; // Переменные для цикла
 // Создание списка с новым количеством элементов (-1)
 stKbrdKey *KeysDataTemp = (stKbrdKey*)memalloc((KeysCount-1)*sizeof(stKbrdKey));   
 // Копирование всех элементов старого списка в новый,
 // при этом num элемент отбрасывается
 for (i=0; i<KeysCount; i++)
  {
   // Продолжение цикла, если i=num
   if (i==num) {k++; continue;}
   KeysDataTemp[i-k].KbrdKey = KeysData[i].KbrdKey;
   KeysDataTemp[i-k].CNS     = KeysData[i].CNS;
   KeysDataTemp[i-k].CmdID   = KeysData[i].CmdID;
  }
 // Освобождение памяти старого списка
 memfree(KeysData);
 // Присваиваивание указателя нового списка
 KeysData=KeysDataTemp;
 // Дикремент на единицу счётчика
 KeysCount--;
 return true; // Успешно
}

// Изменение данных элемента под номером num 
// true - успешно; false - элемент не найден
bool CKbrdKeys::ChangeKey(int num, stKbrdKey *newkbrdkey)
{
 // Проверка на допустимость num
 if ((num<0)||(num>=KeysCount)) return false; // Элемент не найден
 // Изменение свойств элемента
 KeysData[num].KbrdKey = newkbrdkey->KbrdKey;
 KeysData[num].CNS     = newkbrdkey->CNS;
 KeysData[num].CmdID   = newkbrdkey->CmdID;
 return true; // Успешно
}

// Возвращает элемент c индексом num
// true - успешно; false - элемент не найден
bool CKbrdKeys::GetKey(int num, stKbrdKey *getkbrdkey)
{
 // Проверка на допустимость num
 if ((num<0)||(num>=KeysCount)) return false; // Элемент не найден
 getkbrdkey->KbrdKey = KeysData[num].KbrdKey;
 getkbrdkey->CNS     = KeysData[num].CNS;
 getkbrdkey->CmdID   = KeysData[num].CmdID;
 return true; // Успешно
}

// Возвращает количество элементов в списке
int CKbrdKeys::GetKeysCount(void)
{ return KeysCount; }

// Возвращает номер первого элемента по коду клавиши
int CKbrdKeys::FindByKey(WORD kbrdkey)
{
 // Поиск в списке клавиши
 for (int i=0; i<KeysCount; i++)
  if (KeysData[i].KbrdKey == kbrdkey) return i;
 return (-1);
}

// Возвращает номер первого элемента по id команды
int CKbrdKeys::FindByCmdID(int cmdid)
{
 // Поиск в списке id команды
 for (int i=0; i<KeysCount; i++)
  if (KeysData[i].CmdID == cmdid) return i;
 return (-1);
}

// Загрузка списка клавиш из ini файла
void CKbrdKeys::LoadKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata) 
{
 int inttemp;
 // Чтение количества элементов в списке
 inttemp = GetPrivateProfileInt(section,keysize,-1,inifilename);
 // Если <=0, то обнуляем список
 if (inttemp<=0)
  {
   // Освобождаем память и обнуляем
   memfree(KeysData);
   KeysData=NULL;
   KeysCount=0;
  }
 // Иначе выделяем память и считываем данные
 else
  {
   // Если количество элементов не совпадает с текущим, 
   // то выделяем память заново (старые данные теряются)
   if (KeysCount!=inttemp)
    {
	 memfree(KeysData);
     KeysData = (stKbrdKey*)memalloc(inttemp*sizeof(stKbrdKey)); 
    }   
   // Чтение данных
   GetPrivateProfileStruct(section,keydata,KeysData,sizeof(stKbrdKey)*inttemp,inifilename);
   // Новое количество элементов
   KeysCount=inttemp;
  }
}

// Сохранение списка клавиш в ini файл
void CKbrdKeys::SaveKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata)
{
 // Преобразование числа в строковой тип
 char strsize[10];
 wsprintf(strsize,"%i",KeysCount);
 // Сохранение количества элементов списка клавиш
 WritePrivateProfileString(section,keysize,strsize,inifilename);
 // Сохранение данных списка клавиш
 WritePrivateProfileStruct(section,keydata,KeysData,sizeof(stKbrdKey)*KeysCount,inifilename);
}
