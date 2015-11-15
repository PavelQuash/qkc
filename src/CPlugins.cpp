/**************************************************************/
/*                qshKeyCommander1 - CPlugins.cpp             */
/*------------------------------------------------------------*/
/*     Методы класса работы с подключаемыми библиотеками.     */
/*      Сохранение/загрузка, добавление, удаление и т.д.      */
/*****03.2005************************Copyright by <Quash!>*****/

#include "CPlugins.h"

#define PL_SYMB      13                  // Максимальное количество символов в имени файла для хранения его в PlugList ini файла (формат 8.3 + символ конца строки)
#define PL_SECTION   "qshKeyCommander_1" // Секция чтения данных
#define PL_KEYSIZE   "ActivePluginsSize" // Ключ размера массива
#define PL_KEYDATA   "ActivePluginsData" // Ключ данных массива

// Конструктор
CPlugins::CPlugins()
{
 // Обнуление значения
 PluginsData    = NULL;
 PluginsKeys    = NULL;
 PluginsCount   = 0;
 inifilename[0] = '\0';
 pluginsdir[0]  = '\0';
}

// Деструктор
CPlugins::~CPlugins()
{
 // Все подключённые библиотеки выгружаются
 for (int i=0; i<PluginsCount; i++)
  FreeLibrary(PluginsData[i].hDll);
 // Сохранение списка подключенных модулей и их данных
 this->SavePlugList(PL_SECTION,PL_KEYSIZE,PL_KEYDATA);
/* DEBUG
 // Деинициализация списка PluginsKeys
 for (i=0; i<PluginsCount; i++)
  {
   PluginsKeys[i].KbrdKeys.DeInit();
   PluginsKeys[i].WircKeys.DeInit();
  }
 // Освобождение главных массивов
 if (PluginsData!=NULL) memfree(PluginsData);
 if (PluginsKeys!=NULL) memfree(PluginsKeys);
*/
 // Освобождение главных массивов
 if (PluginsData!=NULL) delete [] PluginsData;
 if (PluginsKeys!=NULL) delete [] PluginsKeys;
}

// Установка ini-файла и каталога плагинов
// true - успешно; false - один из файлов не был найден
bool CPlugins::SetPaths(LPTSTR setini, LPTSTR setplugdir)
{
 // Установка ini-файла
 if (FileExists(false,setini)) lstrcpy(inifilename,setini);
 else return false; // Файл не доступен
 // Установка каталога плагинов
 if (FileExists(true,setplugdir)) lstrcpy(pluginsdir,setplugdir);
 else return false; // Файл не доступен
 // Загрузка данных, необходимых для работы
 this->LoadPlugList(PL_SECTION,PL_KEYSIZE,PL_KEYDATA);
 return true; // Успешно
}

// Подключение модуля
// true - успешно; false - модуль уже был подключен или не может быть найден (в список не попал)
bool CPlugins::AddPlugin(LPTSTR filename)
{
 if ((inifilename[0]=='\0')||(pluginsdir[0]=='\0')) return false; // Модуль не может быть найден
 char filepath[MAX_PATH]; // Полный путь к файлу
 int i,k;                 // Переменная для цикла
 HMODULE hdll;            // Хэндл загружаемой dll
 // Просмотр готового списка на наличие добавляемого элемента
 for (i=0; i<PluginsCount; i++)
  if (lstrcmp(PluginsData[i].FileName,filename)==0) return false; // Уже есть в списке
 // Проверка файла на существование
 wsprintf(filepath,"%s\\%s",pluginsdir,filename);
 if (!FileExists(0,filepath)) return false; // Файл не существует
 // Проверка на предмет возможности загрузки
 hdll = LoadLibrary(filepath);
 if (hdll==NULL) return false; // dll не может быть загружена
 // Определение наличия функции GetModuleData
 GMD_Type GetModuleData;
 GetModuleData = (GMD_Type)GetProcAddress(hdll,"GetModuleData");
 if (GetModuleData==NULL)
  {
   FreeLibrary(hdll);
   return false; // dll не может быть загружена
  }
 // Файла нет в списке, он существует, загружается, есть функция => добавление в список
 stPluginData *PluginsDataTemp;
 stPluginKeys *PluginsKeysTemp;
 // Создание главных списков с новым количеством элементов (+1)
 PluginsDataTemp = new stPluginData[PluginsCount+1];
 PluginsKeysTemp = new stPluginKeys[PluginsCount+1];
/* DEBUG
 PluginsDataTemp = (stPluginData*)memalloc((PluginsCount+1)*sizeof(stPluginData));
 PluginsKeysTemp = (stPluginKeys*)memalloc((PluginsCount+1)*sizeof(stPluginKeys));
 // Инициализация списка PluginsKeysTemp
 for (i=0; i<(PluginsCount+1); i++)
 {
  PluginsKeysTemp[i].KbrdKeys.Init();
  PluginsKeysTemp[i].WircKeys.Init();
 }
*/
 // Копирование всех элементов старого списка в новый
 for (i=0; i<PluginsCount; i++)
  {
   // Имя файла модуля
   lstrcpy(PluginsDataTemp[i].FileName,PluginsData[i].FileName);
   // Хэндл модуля
   PluginsDataTemp[i].hDll=PluginsData[i].hDll;
   // Заголовок модуля
   PluginsDataTemp[i].Caption=PluginsData[i].Caption;
   // Копирование адресов вызываемых функций
   PluginsDataTemp[i].Init          = PluginsData[i].Init;
   PluginsDataTemp[i].Quit          = PluginsData[i].Quit;
   PluginsDataTemp[i].Config        = PluginsData[i].Config;
   PluginsDataTemp[i].Command       = PluginsData[i].Command;
   PluginsDataTemp[i].CommandsCount = PluginsData[i].CommandsCount;
   PluginsDataTemp[i].CommandsList  = PluginsData[i].CommandsList;
   // Копирование данных Keyboard
   stKbrdKey KbrdKey;
   for (k=0; k<PluginsKeys[i].KbrdKeys.GetKeysCount(); k++)
    {
     PluginsKeys[i].KbrdKeys.GetKey(k,&KbrdKey);   // Считывание k элемента
     PluginsKeysTemp[i].KbrdKeys.AddKey(&KbrdKey); // Сохранение считанного элемента
    }
   // Копирование данных WinLIRC
   stWircKey WircKey;
   for (k=0; k<PluginsKeys[i].WircKeys.GetKeysCount(); k++)
    {
     PluginsKeys[i].WircKeys.GetKey(k,&WircKey);   // Считывание k элемента
     PluginsKeysTemp[i].WircKeys.AddKey(&WircKey); // Сохранение считанного элемента
    }
  }
/* DEBUG
 // Деинициализация старого списка PluginsKeys
 for (i=0; i<PluginsCount; i++)
  {
   PluginsKeys[i].KbrdKeys.DeInit();
   PluginsKeys[i].WircKeys.DeInit();
  }
 // Освобождение памяти старых списков
 if (PluginsData!=NULL) memfree(PluginsData);
 if (PluginsKeys!=NULL) memfree(PluginsKeys);
*/
 // Освобождение памяти старых списков
 if (PluginsData!=NULL) delete [] PluginsData;
 if (PluginsKeys!=NULL) delete [] PluginsKeys;
 // Присваиваивание указателей новых списков
 PluginsData=PluginsDataTemp;
 PluginsKeys=PluginsKeysTemp;
 // Определение значений нового элемента
 lstrcpy(PluginsData[PluginsCount].FileName,filename); // Сохранение имени файла
 PluginsData[PluginsCount].hDll = hdll;                // Определение хэндла модуля
 // Загружаем клавиши для модуля из ini файла
 PluginsKeys[PluginsCount].KbrdKeys.LoadKeys(inifilename,PluginsData[PluginsCount].FileName,KBRD_KEYSIZE,KBRD_KEYDATA);
 PluginsKeys[PluginsCount].WircKeys.LoadKeys(inifilename,PluginsData[PluginsCount].FileName,WIRC_KEYSIZE,WIRC_KEYDATA);
 // Значения по умолчанию
 PluginsData[PluginsCount].Init          = NULL;
 PluginsData[PluginsCount].Quit          = NULL;
 PluginsData[PluginsCount].Config        = NULL;
 PluginsData[PluginsCount].Command       = 0;
 PluginsData[PluginsCount].CommandsCount = 0;
 // Вызов стандартной функции и заполнение указателей адресами функций
 GetModuleData(&PluginsData[PluginsCount]);
 // Инкремент на единицу счётчика
 PluginsCount++;
 return true; // Успешно
}

// Отключение модуля
// true - удаление успешно; false - элемент не найден
bool CPlugins::DelPlugin(int num)
{
 // Проверка на допутимость num
 if ((num<0)||(num>=PluginsCount)) return false; // Элемент не найден
 
 // Сохранение данных удаляемого модуля
 PluginsKeys[num].KbrdKeys.SaveKeys(inifilename,PluginsData[num].FileName,KBRD_KEYSIZE,KBRD_KEYDATA);
 PluginsKeys[num].WircKeys.SaveKeys(inifilename,PluginsData[num].FileName,WIRC_KEYSIZE,WIRC_KEYDATA);

 // Выгружение удаляемого модуля из памяти
 FreeLibrary(PluginsData[num].hDll);

 int i,k,l=0; // Переменные для цикла
 stPluginData *PluginsDataTemp;
 stPluginKeys *PluginsKeysTemp;
 // Создание списка с новым количеством элементов (-1)
 PluginsDataTemp = new stPluginData[PluginsCount-1];
 PluginsKeysTemp = new stPluginKeys[PluginsCount-1];
/* DEBUG
 PluginsDataTemp = (stPluginData*)memalloc((PluginsCount-1)*sizeof(stPluginData));
 PluginsKeysTemp = (stPluginKeys*)memalloc((PluginsCount-1)*sizeof(stPluginKeys));
 // Инициализация списка PluginsKeysTemp
 for (i=0; i<(PluginsCount-1); i++)
  {
   PluginsKeysTemp[i].KbrdKeys.Init();
   PluginsKeysTemp[i].WircKeys.Init();
  }
*/
 // Копирование всех элементов старого списка в новый
 for (i=0; i<PluginsCount; i++)
  {
   if (i==num) {l++; continue;}
   lstrcpy(PluginsDataTemp[i-l].FileName,PluginsData[i].FileName); // Имя файла модуля
   PluginsDataTemp[i-l].hDll          = PluginsData[i].hDll;       // Хэндл модуля
   PluginsDataTemp[i-l].Caption       = PluginsData[i].Caption;    // Заголовок модуля
   // Копирование адресов вызываемых функций
   PluginsDataTemp[i-l].Init          = PluginsData[i].Init;
   PluginsDataTemp[i-l].Quit          = PluginsData[i].Quit;
   PluginsDataTemp[i-l].Config        = PluginsData[i].Config;
   PluginsDataTemp[i-l].Command       = PluginsData[i].Command;
   PluginsDataTemp[i-l].CommandsCount = PluginsData[i].CommandsCount;
   PluginsDataTemp[i-l].CommandsList  = PluginsData[i].CommandsList;
   // Копирование данных Keyboard
   stKbrdKey KbrdKey;
   for (k=0; k<PluginsKeys[i].KbrdKeys.GetKeysCount(); k++)
    {
     PluginsKeys[i].KbrdKeys.GetKey(k,&KbrdKey);     // Считывание k элемента
     PluginsKeysTemp[i-l].KbrdKeys.AddKey(&KbrdKey); // Сохранение считанного элемента
    }
   // Копирование данных WinLIRC
   stWircKey WircKey;
   for (k=0; k<PluginsKeys[i].WircKeys.GetKeysCount(); k++)
    {
     PluginsKeys[i].WircKeys.GetKey(k,&WircKey);     // Считывание k элемента
     PluginsKeysTemp[i-l].WircKeys.AddKey(&WircKey); // Сохранение считанного элемента
    }
  }
/* DEBUG
 // Деинициализация старого списка PluginsKeys
 for (i=0; i<PluginsCount; i++)
  {
   PluginsKeys[i].KbrdKeys.DeInit();
   PluginsKeys[i].WircKeys.DeInit();
  }
 // Освобождение памяти старых списков
 if (PluginsData!=NULL) memfree(PluginsData);
 if (PluginsKeys!=NULL) memfree(PluginsKeys);
*/
 // Освобождение памяти старых списков
 if (PluginsData!=NULL) delete [] PluginsData;
 if (PluginsKeys!=NULL) delete [] PluginsKeys;
 // Присваиваивание указателей новых списков
 PluginsData=PluginsDataTemp;
 PluginsKeys=PluginsKeysTemp;
 // Декремент на единицу счётчика 
 PluginsCount--;
 return true; // Успешно
}

// Данные num плагина
bool CPlugins::GetPluginData(int num, stPluginData *getplugindata)
{
 // Проверка на допустимость num
 if ((num<0)||(num>=PluginsCount)) return false; // Элемент не найден
 lstrcpy(getplugindata->FileName,PluginsData[num].FileName);
 getplugindata->hDll          = PluginsData[num].hDll;
 getplugindata->Caption       = PluginsData[num].Caption;
 getplugindata->Init          = PluginsData[num].Init;
 getplugindata->Quit          = PluginsData[num].Quit;
 getplugindata->Config        = PluginsData[num].Config;
 getplugindata->Command       = PluginsData[num].Command;
 getplugindata->CommandsCount = PluginsData[num].CommandsCount;
 getplugindata->CommandsList  = PluginsData[num].CommandsList;
 return true; // Успешно
}

// Возврашение количества подключенных модулей
int CPlugins::GetPluginsCount(void)
{ return PluginsCount; }

// Найти модуль по имени файла
int  CPlugins::FindByFileName(LPTSTR filename)
{
 for (int i=0; i<PluginsCount; i++)
  if (lstrcmp(PluginsData[i].FileName,filename)==0) return i;
 return (-1);
}

// Найти модуль по хэндлу модуля
int  CPlugins::FindByHandle(HINSTANCE hdll)
{
 for (int i=0; i<PluginsCount; i++)
  if (PluginsData[i].hDll==hdll) return i;
 return (-1);
}

// Загрузить список подключенных модулей
bool CPlugins::LoadPlugList(LPTSTR section, LPTSTR keysize, LPTSTR keydata)
{
 int inttemp;
 // Чтение количества элементов в списке
 inttemp = GetPrivateProfileInt(section,keysize,-1,inifilename);
 // Если в ini-файле количество модулей >0, то - выделение памяти и считывание данных
 if (inttemp>0)
  {
   char (*pluglist) [PL_SYMB]; // Описание указателя
   char filename[MAX_PATH];
   //DEBUG: pluglist = (char (*)[13])memalloc(inttemp*PL_SYMB);
   pluglist = new char [inttemp][PL_SYMB]; // Выделение памяти для списка подключённых модулей
   // Чтение данных списка
   GetPrivateProfileStruct(section,keydata,pluglist,sizeof(char [PL_SYMB])*inttemp,inifilename);
   // Формирование нормальных имён файлов из формата 8.3
   for (int i=0; i<inttemp; i++)
    {
     GetLongFileName(pluginsdir,pluglist[i],filename);
     // Если файл не существует, то в AddPlugin всё предусмотрено и файл игнорируется
     this->AddPlugin(filename);
    }
   //DEBUG: memfree(pluglist);
   delete [] pluglist; // Освобождение памяти списка
  }
/* DEBUG [???]
 else // debug Освобождение памяти (на всякий случай, хотя можно не писать)
  {
   // Деинициализация списка PluginsKeysTemp
   for (int i=0; i<PluginsCount; i++)
	{
     PluginsKeys[i].KbrdKeys.DeInit();
     PluginsKeys[i].WircKeys.DeInit();
	}
   // Освобождаем память и обнуляем
   memfree(PluginsData);
   memfree(PluginsKeys);
   PluginsData=NULL;
   PluginsKeys=NULL;
   PluginsCount=0;
  }
*/
 return true; // Успешно
}

// Сохранить список подключенных модулей
bool CPlugins::SavePlugList(LPTSTR section, LPTSTR keysize, LPTSTR keydata)
{
 if ((inifilename[0]=='\0')||(pluginsdir[0]=='\0')) return false; // Модуль не может быть найден
 char (*pluglist)[PL_SYMB]; // Описание указателя
 int i;                     // Для цикла
 // Выделение памяти для списка подключённых модулей
 // DEBUG: pluglist = (char (*)[13])memalloc(PluginsCount*PL_SYMB);
 pluglist = new char [PluginsCount][PL_SYMB];
 // Формируем список подключенных модулей
 char fpath[MAX_PATH],
      spath[MAX_PATH],
      filename[13];
 for (i=0; i<PluginsCount; i++)
  {
   // Формирование короткого имени файла в формат 8.3
   wsprintf(fpath,"%s\\%s",pluginsdir,PluginsData[i].FileName); // Формирование полного пути к файлу в fpath
   GetShortPathName(fpath,spath,sizeof(spath));                 // Формирование короткого пути в формате 8.3 в spath
   GetFileTitle(spath,filename,sizeof(filename));               // Выделение только имени файла в filename
   wsprintf(pluglist[i],"%s",filename);                         // Сохранение filename в массив
   // Сохранение данных текущего модуля i
   PluginsKeys[i].KbrdKeys.SaveKeys(inifilename,PluginsData[i].FileName,KBRD_KEYSIZE,KBRD_KEYDATA);
   PluginsKeys[i].WircKeys.SaveKeys(inifilename,PluginsData[i].FileName,WIRC_KEYSIZE,WIRC_KEYDATA);
  }

 // Сохранение в ini файл
 char str[10];
 wsprintf(str,"%i",PluginsCount);
 WritePrivateProfileString(section,keysize,str,inifilename);
 WritePrivateProfileStruct(section,keydata,pluglist,sizeof(char [PL_SYMB])*PluginsCount,inifilename);
 // Освобождаем память
 // DEBUG: memfree(pluglist);
 delete [] pluglist;
 return true; // Успешно
}
