/**************************************************************/
/*              qshKeyCommander1 - CWircKeys.h                */
/*------------------------------------------------------------*/
/* Методы класса хранения кнопок WinLIRC модуля. Добавление,  */
/*   удаление, изменение, поиск, загрузка/сохранение в файл   */
/*                           элементов                        */
/*****03.2005************************Copyright by <Quash!>*****/

#include "CWircKeys.h"

// Конструктор
CWircKeys::CWircKeys()
{
 // Задаём начальные значения
 KeysData=NULL;
 KeysCount=0;
}

// Деструктор
CWircKeys::~CWircKeys()
{
 // Освобождаем память
 if (KeysData!=NULL) memfree(KeysData);
}

// Добавляется новый элемент
void CWircKeys::AddKey(stWircKey *newwirckey)
{
 int i; // Переменная для цикла
 // Создание списка с новым количеством элементов (+1)
 stWircKey *KeysDataTemp = (stWircKey*)memalloc((KeysCount+1)*sizeof(stWircKey));
 // Копирование всех элементов старого списка в новый
 for (i=0; i<KeysCount; i++)
  {
   lstrcpy(KeysDataTemp[i].WircKey,KeysData[i].WircKey);
   KeysDataTemp[i].Repeats = KeysData[i].Repeats;
   KeysDataTemp[i].CmdID   = KeysData[i].CmdID;
  }
 // Освобождение памяти старого списка
 memfree(KeysData);
 // Присваиваивание указателя нового списка
 KeysData=KeysDataTemp;
 // Определение значений нового элемента
 lstrcpy(KeysData[KeysCount].WircKey,newwirckey->WircKey);
 KeysData[KeysCount].Repeats = newwirckey->Repeats;
 KeysData[KeysCount].CmdID   = newwirckey->CmdID;
 // Инкремент на единицу счётчика
 KeysCount++;
}

// Удаляется элемент с индексом num
// true - успешно; false - элемент не найден
bool CWircKeys::DelKey(int num)
{
 // Проверка на допутимость num
 if ((num<0)||(num>=KeysCount)) return false; // Элемент не найден
 int i,k=0; // Переменные для цикла
 // Создание списка с новым количеством элементов (-1)
 stWircKey *KeysDataTemp = (stWircKey*)memalloc((KeysCount-1)*sizeof(stWircKey));
 // Копирование всех элементов старого списка в новый,
 // при этом num элемент отбрасывается
 for (i=0; i<KeysCount; i++)
  {
   // Продолжение цикла, если i=num
   if (i==num) {k++; continue;}
   lstrcpy(KeysDataTemp[i-k].WircKey,KeysData[i].WircKey);
   KeysDataTemp[i-k].Repeats = KeysData[i].Repeats;
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
bool CWircKeys::ChangeKey(int num, stWircKey *newwirckey)
{
 // Проверка на допустимость num
 if ((num<0)||(num>=KeysCount)) return false; // Элемент не найден
 // Изменение свойств элемента
 lstrcpy(KeysData[num].WircKey,newwirckey->WircKey);
 KeysData[num].Repeats = newwirckey->Repeats;
 KeysData[num].CmdID   = newwirckey->CmdID;
 return true; // Успешно
}

// Возвращяет элемент c индексом num
// true - успешно; false - элемент не найден
bool CWircKeys::GetKey(int num, stWircKey *getwirckey)
{
 // Проверка на допустимость num
 if ((num<0)||(num>=KeysCount)) return false; // Элемент не найден
 lstrcpy(getwirckey->WircKey,KeysData[num].WircKey);
 getwirckey->Repeats = KeysData[num].Repeats;
 getwirckey->CmdID   = KeysData[num].CmdID;
 return true; // Успешно
}

// Возвращает количество элементов в списке
int CWircKeys::GetKeysCount(void)
{ return KeysCount; }

// Возвращает номер первого элемента по коду клавиши
int CWircKeys::FindByKey(char wirckey[80])
{
 // Поиск в списке клавиши
 for (int i=0; i<KeysCount; i++)
  if (lstrcmp(KeysData[i].WircKey,wirckey)==0) return i;
 return (-1);
}

// Возвращает номер первого элемента по id команды
int CWircKeys::FindByCmdID(int cmdid)
{
 // Поиск в списке id команды
 for (int i=0; i<KeysCount; i++)
  if (KeysData[i].CmdID == cmdid) return i;
 return (-1);
}

// Загрузка списка клавиш из ini файла
void CWircKeys::LoadKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata) 
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
     KeysData = (stWircKey*)memalloc(inttemp*sizeof(stWircKey));
    }   
   // Чтение данных
   GetPrivateProfileStruct(section,keydata,KeysData,sizeof(stWircKey)*inttemp,inifilename);
   // Новое количество элементов
   KeysCount=inttemp;
  }
}

// Сохранение списка клавиш в ini файл
void CWircKeys::SaveKeys(LPTSTR inifilename, LPTSTR section, LPTSTR keysize, LPTSTR keydata)
{
 // Преобразование числа в строковой тип
 char strsize[10];
 wsprintf(strsize,"%i",KeysCount);
 // Сохранение количества элементов списка клавиш
 WritePrivateProfileString(section,keysize,strsize,inifilename);
 // Сохранение данных списка клавиш
 WritePrivateProfileStruct(section,keydata,KeysData,sizeof(stWircKey)*KeysCount,inifilename);
}