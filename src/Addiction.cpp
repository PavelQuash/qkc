/**************************************************************/
/*              qshKeyCommander1 - Addiction.h                */
/*------------------------------------------------------------*/
/*             Реализация дополнительных процедур             */
/*****2004***************************Copyright by <Quash!>*****/

#include "Addiction.h"



// Проверка файла на существование
// true - найден файл; false - файл не найден
bool FileExists(bool dir, LPTSTR pathname)
{
 WIN32_FIND_DATA finddata;
 HANDLE          hfile;

 // Попытка искать
 hfile = FindFirstFile(pathname,&finddata);
 if (hfile==INVALID_HANDLE_VALUE) return false;
        if ((dir)&&(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))     return true;
   else if ((!dir)&&(!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))) return true;
 // Продолжение поиска
  while (FindNextFile(hfile,&finddata))
        if ((dir)&&(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))     return true;
   else if ((!dir)&&(!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))) return true;
 return false; // Файл не найден
}



// Преобразование имени файла из короткого в длинное (обычное)
// true - успешно; false - файл не найден
bool GetLongFileName(LPTSTR filepath, LPTSTR filename, LPTSTR longfilename)
{
 WIN32_FIND_DATA finddata;
 HANDLE          hfile;
 char fullpath[MAX_PATH];
 wsprintf(fullpath,"%s\\%s",filepath,filename);

 // Попытка искать
 hfile = FindFirstFile(fullpath,&finddata);
 if (hfile==INVALID_HANDLE_VALUE) return false;
  if (!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
   {
    wsprintf(longfilename,"%s",finddata.cFileName);
    return true;
   }
 // Продолжение поиска
  while (FindNextFile(hfile,&finddata))
   if (!(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) 
    {
     wsprintf(longfilename,"%s",finddata.cFileName);
     return true;
    }
 longfilename[0]='\0'; 
 return false; // Файл не найден
}



// Ключ переводится в вид строки
// L: модификаторы; H: виртуальный код
void KeyToStr(WORD key, LPTSTR strkey)
{
 strkey[0]='\0';
 // Клавиши-модификаторы
 if (LOBYTE(key)&MOD_WIN)     lstrcat(strkey,"Win + ");
 if (LOBYTE(key)&MOD_CONTROL) lstrcat(strkey,"Ctrl + ");
 if (LOBYTE(key)&MOD_SHIFT)   lstrcat(strkey,"Shift + ");
 if (LOBYTE(key)&MOD_ALT)     lstrcat(strkey,"Alt + ");
 switch (HIBYTE(key))
  {
   // Не определено
   case 0x00:      lstrcat(strkey,"<None>"); break;
   // Управляющие клавиши
   case VK_ESCAPE: lstrcat(strkey,"Escape"); break;
   case VK_TAB:    lstrcat(strkey,"Tab"); break;
   case VK_APPS:   lstrcat(strkey,"Menu"); break;
   case VK_RETURN: lstrcat(strkey,"Enter"); break;
   case VK_SPACE:  lstrcat(strkey,"Space"); break;
   case VK_BACK:   lstrcat(strkey,"Back Space"); break;
   // Клавиши на дополнительной клавиатуре
   case VK_UP:     lstrcat(strkey,"Up"); break;
   case VK_RIGHT:  lstrcat(strkey,"Right"); break;
   case VK_LEFT:   lstrcat(strkey,"Left"); break;
   case VK_DOWN:   lstrcat(strkey,"Down"); break;
   case VK_INSERT: lstrcat(strkey,"Insert"); break;
   case VK_DELETE: lstrcat(strkey,"Delete"); break;
   case VK_HOME:   lstrcat(strkey,"Home"); break;
   case VK_END:    lstrcat(strkey,"End"); break;
   case VK_PRIOR:  lstrcat(strkey,"Page Up"); break;
   case VK_NEXT:   lstrcat(strkey,"Page Down"); break;
   case VK_NUMPAD0:lstrcat(strkey,"Np 0");break;
   case VK_NUMPAD1:lstrcat(strkey,"Np 1");break;
   case VK_NUMPAD2:lstrcat(strkey,"Np 2");break;
   case VK_NUMPAD3:lstrcat(strkey,"Np 3");break;
   case VK_NUMPAD4:lstrcat(strkey,"Np 4");break;
   case VK_NUMPAD5:lstrcat(strkey,"Np 5");break;
   case VK_NUMPAD6:lstrcat(strkey,"Np 6");break;
   case VK_NUMPAD7:lstrcat(strkey,"Np 7");break;
   case VK_NUMPAD8:lstrcat(strkey,"Np 8");break;
   case VK_NUMPAD9:lstrcat(strkey,"Np 9");break;
   case VK_DIVIDE:   lstrcat(strkey,"Np /");break;
   case VK_MULTIPLY: lstrcat(strkey,"Np *");break;
   case VK_SUBTRACT: lstrcat(strkey,"Np -");break;
   case VK_ADD:      lstrcat(strkey,"Np +");break;
   case VK_DECIMAL:  lstrcat(strkey,"Np .");break;
   // Клавиши F##
   case VK_F1:     lstrcat(strkey,"F1"); break;
   case VK_F2:     lstrcat(strkey,"F2"); break;
   case VK_F3:     lstrcat(strkey,"F3"); break;
   case VK_F4:     lstrcat(strkey,"F4"); break;
   case VK_F5:     lstrcat(strkey,"F5"); break;
   case VK_F6:     lstrcat(strkey,"F6"); break;
   case VK_F7:     lstrcat(strkey,"F7"); break;
   case VK_F8:     lstrcat(strkey,"F8"); break;
   case VK_F9:     lstrcat(strkey,"F9"); break;
   case VK_F10:    lstrcat(strkey,"F10"); break;
   case VK_F11:    lstrcat(strkey,"F11"); break;
   case VK_F12:    lstrcat(strkey,"F12"); break;
   case VK_F13:    lstrcat(strkey,"F13"); break;
   case VK_F14:    lstrcat(strkey,"F14"); break;
   case VK_F15:    lstrcat(strkey,"F15"); break;
   case VK_F16:    lstrcat(strkey,"F16"); break;
   case VK_F17:    lstrcat(strkey,"F17"); break;
   case VK_F18:    lstrcat(strkey,"F18"); break;
   case VK_F19:    lstrcat(strkey,"F19"); break;
   case VK_F20:    lstrcat(strkey,"F20"); break;
   case VK_F21:    lstrcat(strkey,"F21"); break;
   case VK_F22:    lstrcat(strkey,"F22"); break;
   case VK_F23:    lstrcat(strkey,"F23"); break;
   case VK_F24:    lstrcat(strkey,"F24"); break;
   // Символьные клавиши
   case 0x30:      lstrcat(strkey,"0"); break;
   case 0x31:      lstrcat(strkey,"1"); break;
   case 0x32:      lstrcat(strkey,"2"); break;
   case 0x33:      lstrcat(strkey,"3"); break;
   case 0x34:      lstrcat(strkey,"4"); break;
   case 0x35:      lstrcat(strkey,"5"); break;
   case 0x36:      lstrcat(strkey,"6"); break;
   case 0x37:      lstrcat(strkey,"7"); break;
   case 0x38:      lstrcat(strkey,"8"); break;
   case 0x39:      lstrcat(strkey,"9"); break;
   case 0x41:      lstrcat(strkey,"A"); break;
   case 0x42:      lstrcat(strkey,"B"); break;
   case 0x43:      lstrcat(strkey,"C"); break;
   case 0x44:      lstrcat(strkey,"D"); break;
   case 0x45:      lstrcat(strkey,"E"); break;
   case 0x46:      lstrcat(strkey,"F"); break;
   case 0x47:      lstrcat(strkey,"G"); break;
   case 0x48:      lstrcat(strkey,"H"); break;
   case 0x49:      lstrcat(strkey,"I"); break;
   case 0x4A:      lstrcat(strkey,"J"); break;
   case 0x4B:      lstrcat(strkey,"K"); break;
   case 0x4C:      lstrcat(strkey,"L"); break;
   case 0x4D:      lstrcat(strkey,"M"); break;
   case 0x4E:      lstrcat(strkey,"N"); break;
   case 0x4F:      lstrcat(strkey,"O"); break;
   case 0x50:      lstrcat(strkey,"P"); break;
   case 0x51:      lstrcat(strkey,"Q"); break;
   case 0x52:      lstrcat(strkey,"R"); break;
   case 0x53:      lstrcat(strkey,"S"); break;
   case 0x54:      lstrcat(strkey,"T"); break;
   case 0x55:      lstrcat(strkey,"U"); break;
   case 0x56:      lstrcat(strkey,"V"); break;
   case 0x57:      lstrcat(strkey,"W"); break;
   case 0x58:      lstrcat(strkey,"X"); break;
   case 0x59:      lstrcat(strkey,"Y"); break;
   case 0x5A:      lstrcat(strkey,"Z"); break;
   // Другие клавиши (около цифр)
   case 0xC0:      lstrcat(strkey,"`"); break;
   case 0xBD:      lstrcat(strkey,"-"); break;
   case 0xBB:      lstrcat(strkey,"="); break;
   case 0xDC:      lstrcat(strkey,"\\"); break;
   case 0xDB:      lstrcat(strkey,"["); break;
   case 0xDD:      lstrcat(strkey,"]"); break;
   case 0xBA:      lstrcat(strkey,";"); break;
   case 0xDE:      lstrcat(strkey,"'"); break;
   case 0xBC:      lstrcat(strkey,","); break;
   case 0xBE:      lstrcat(strkey,"."); break;
   case 0xBF:      lstrcat(strkey,"/"); break;
   default:
    char str[30];
    wsprintf(str,"%i",HIBYTE(key));
    lstrcat(strkey,"<"); lstrcat(strkey,str); lstrcat(strkey,">");
    break;
  }
}

// Преобразование строки в целое число
int stoi(LPTSTR str)
{
 int c,     // Число текущей позиции
     k=1,   // Степень
     res=0; // Результат

 for (int i=lstrlen(str)-1; i>=0; i--)
  {
   switch (str[i])
    {
     case '0': c= 0; break;
     case '1': c= 1; break;
     case '2': c= 2; break;
     case '3': c= 3; break;
     case '4': c= 4; break;
     case '5': c= 5; break;
     case '6': c= 6; break;
     case '7': c= 7; break;
     case '8': c= 8; break;
     case '9': c= 9; break;
     case '-': // Если первый символ - минус
      if (i==0) 
       {
        c=-1;
        break;
       }
     default: // Если незнакомый символ, то возвращается нуль
      return 0;
      break;
    }
   if (c==-1) continue;
   res+=c*k;
   k*=10; 
  }	
 if (str[0]=='-') res*=-1;
 return res;
}



// Процедуры работы с памятью
// Выделение памяти для переменной размером size (если попытка неудачна, то возвращается NULL)
void* memalloc(int size)
{
 return HeapAlloc(GetProcessHeap(),0,size);
}
// Освобождение памяти переменной (если всё успешно, значение функции ненулевое)
int memfree(void* var)
{
 if (var==NULL) return 1; // Возвращается ненулевое значение (успешно, указатель не указывает ни на что)
 return HeapFree(GetProcessHeap(),0,var);
} 
// Перераспределение памяти (если попытка неудачна, то возвращается NULL)
void* qsh_realloc(void* var, int size)
{
 if (var == NULL) return memalloc(size);
 return HeapReAlloc(GetProcessHeap(),0,var,size);
}

