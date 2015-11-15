/**************************************************************/
/*             qshKeyCommander1 - CGetKbrdKey.cpp             */
/*------------------------------------------------------------*/
/*      ������, ������������ ���������� (������� �������)     */
/*****2004***************************Copyright by <Quash!>*****/

#include "CGetKbrdKey.h"

HWND hwndChHK,    // ������� ����
     hwndStatus;  // ������ (���������� �� ��������������� �������)
BOOL GKK_WithCNS; // ���� ���������� ������������ CNS
WORD GKK_HotKey;  // �������
WORD GKK_CNS;     // ����� CNS


LRESULT CALLBACK DlgGetKbrdKeyProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam); // ������� ��������� ��������� �������� ����
void DlgGetKbrdKey_Update(); // ���������� ����������� �������� ����

// �������, ������������ "�������" ������� � ����� cns
void CGetKbrdKey::GetHotKey(BOOL withcns, WORD *hotkey, WORD *cns)
{
 // ����������� � ����� ���������� ������
 GKK_HotKey  = *hotkey;
 if (cns!=NULL) GKK_CNS = *cns;
 GKK_WithCNS = withcns;

 HINSTANCE hinst=GetModuleHandle(NULL);
 // �������� ������ ����
 WNDCLASS  wc;
 wc.style         = CS_HREDRAW|CS_VREDRAW;         
 wc.lpfnWndProc   = (WNDPROC)DlgGetKbrdKeyProc;
 wc.hInstance     = hinst;
 wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
 wc.lpszClassName = "IDD_SETTINGS_KBRD_ENTERDATA";                    
 wc.hCursor       = LoadCursor(NULL,IDC_ARROW);     
 wc.hIcon         = LoadIcon(NULL,IDI_HAND); 
 wc.lpszMenuName  = NULL;
 wc.cbClsExtra    = 0;                                   
 wc.cbWndExtra    = 0;                                   
 RegisterClass(&wc);

 // ��������� ����
 int Width  = 300; // ������
 int Height = 125; // ������
 RECT rct;
 GetWindowRect(GetDesktopWindow(),&rct);

 // �������� �������� ����
 hwndChHK   = CreateWindow(wc.lpszClassName,"���������� ������� ������",WS_POPUP|WS_DLGFRAME,(rct.right/2)-(Width/2),(rct.bottom/2)-(Height/2),Width,Height,NULL,NULL,hinst,NULL);
 hwndStatus = CreateWindow("STATIC","",WS_VISIBLE|WS_CHILD,10,10,275,100,hwndChHK,NULL,hinst,NULL);

 // ������������ ������� ����, ������������ � �����������
 SetWindowPos(hwndChHK,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
 DlgGetKbrdKey_Update();
 ShowWindow(hwndChHK, SW_SHOW);
 UpdateWindow(hwndChHK);    

 // ���� ��������� ��������� �� ����
 MSG message;
 while (GetMessage(&message,NULL,0,0)) 
  TranslateMessage(&message),DispatchMessage(&message);
 // ����������� � ����� ���������� ������
 *hotkey = GKK_HotKey;
 if (cns!=NULL) *cns = GKK_CNS;
}

// ������� ��������� ��������� �������� ����
LRESULT CALLBACK DlgGetKbrdKeyProc(HWND hwndWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
 LRESULT ret = 0; // �� ��������� 
 switch (msg)
  {
   case WM_SYSKEYUP:
    SendMessage(hwndWnd,WM_KEYDOWN,(WPARAM)wParam,(LPARAM)lParam);
    break;
   case WM_KEYDOWN:
    {
     BYTE lobytekey = LOBYTE(GKK_HotKey);
     BYTE hibytekey = HIBYTE(GKK_HotKey);
     BYTE lobytecns = LOBYTE(GKK_CNS);
     BYTE hibytecns = HIBYTE(GKK_CNS);
     switch ((int) wParam)
      {
       case VK_LWIN: // Win
       case VK_RWIN:
        lobytekey ^= MOD_WIN;
        break;
       case VK_CONTROL: // Ctrl
        lobytekey ^= MOD_CONTROL;
        break;
       case VK_SHIFT: // Shift
        lobytekey ^= MOD_SHIFT;
        break;
       case VK_MENU: // Alt
        lobytekey ^= MOD_ALT;
        break;
       // ������ ������������ ������� Caps Lock
       // 0 0 or 0 1 => 1 0
       //        1 0 => 1 1
       //        1 1 => 0 0
       case VK_CAPITAL: // Caps Lock
        if (!GKK_WithCNS) break; // �����, ���� CNS �� �����
             if (!(lobytecns&CNS_CAPSLOCK))                             { lobytecns |= CNS_CAPSLOCK; hibytecns &= CNS_CAPSLOCK; }
        else if ((lobytecns&CNS_CAPSLOCK)&&(!(hibytecns&CNS_CAPSLOCK))) { hibytecns ^= CNS_CAPSLOCK; }
        else if ((lobytecns&CNS_CAPSLOCK)&&(hibytecns&&CNS_CAPSLOCK))   { lobytecns ^= CNS_CAPSLOCK; hibytecns ^= CNS_CAPSLOCK; }
        GKK_CNS = MAKEWORD(lobytecns,hibytecns);
        break;
       // ������ ������������ ������� Num Lock
       // 0 0 or 0 1 => 1 1
       //        1 1 => 1 0
       //        1 0 => 0 0
       case VK_NUMLOCK: // Num Lock
        if (!GKK_WithCNS) break; // �����, ���� CNS �� �����
             if (!(lobytecns&CNS_NUMLOCK))                             { lobytecns |= CNS_NUMLOCK; hibytecns |= CNS_NUMLOCK; }
        else if ((lobytecns&CNS_NUMLOCK)&&(hibytecns&CNS_NUMLOCK))     { hibytecns ^= CNS_NUMLOCK; }
        else if ((lobytecns&CNS_NUMLOCK)&&(!(hibytecns&&CNS_NUMLOCK))) { lobytecns ^= CNS_NUMLOCK; }
        GKK_CNS = MAKEWORD(lobytecns,hibytecns);
        break;
       // ������ ������������ ������� Scroll Lock
       // 0 0 or 0 1 => 1 0
       //        1 0 => 1 1
       //        1 1 => 0 0
       case VK_SCROLL: // Scroll Lock
        if (!GKK_WithCNS) break; // �����, ���� CNS �� �����
             if (!(lobytecns&CNS_SCROLLLOCK))                               { lobytecns |= CNS_SCROLLLOCK; hibytecns &= CNS_SCROLLLOCK; }
        else if ((lobytecns&CNS_SCROLLLOCK)&&(!(hibytecns&CNS_SCROLLLOCK))) { hibytecns ^= CNS_SCROLLLOCK; }
        else if ((lobytecns&CNS_SCROLLLOCK)&&(hibytecns&&CNS_SCROLLLOCK))   { lobytecns ^= CNS_SCROLLLOCK; hibytecns ^= CNS_SCROLLLOCK; }
        GKK_CNS = MAKEWORD(lobytecns,hibytecns);
        break;
       default:
        hibytekey = (int) wParam;
        DestroyWindow(hwndWnd);
        break;
      }
     GKK_HotKey    = MAKEWORD(lobytekey,hibytekey);
     DlgGetKbrdKey_Update();
    }
    break;
   case WM_CLOSE: // �������� �����
    DestroyWindow(hwndWnd); // ���������� ����
    break;
   case WM_DESTROY: // ����������� �����
    PostQuitMessage(0); 
    break;
   default: // ��������� �� ���������
    ret = DefWindowProc(hwndWnd,msg,wParam,lParam); 
    break;
  }
 return (ret);
}

// ���������� ����������� �������� ����
void DlgGetKbrdKey_Update()
{
 // �������'\n+Num Lock ��� ��������� '��������'\n-Scroll Lock",
 char buffer [300],
      buffer2[300];
 KeyToStr(GKK_HotKey,buffer2);
 wsprintf(buffer,"������� ���������� ������:\n%s\n",buffer2);
 if (GKK_WithCNS)
  {
   // ��� Caps Lock
   if (LOBYTE(GKK_CNS)&CNS_CAPSLOCK)
    {
     lstrcat(buffer,"\n+");
     lstrcat(buffer,"Caps Lock ��� ��������� '");
     if (HIBYTE(GKK_CNS)&CNS_CAPSLOCK) lstrcat(buffer,"�������");
     else                              lstrcat(buffer,"��������");
     lstrcat(buffer,"'");
    }
   else 
    {
     lstrcat(buffer,"\n-");
     lstrcat(buffer,"Caps Lock");
    }
   // ��� Num Lock
   if (LOBYTE(GKK_CNS)&CNS_NUMLOCK)
    {
     lstrcat(buffer,"\n+");
     lstrcat(buffer,"Num Lock ��� ��������� '");
     if (HIBYTE(GKK_CNS)&CNS_NUMLOCK) lstrcat(buffer,"�������");
     else                             lstrcat(buffer,"��������");
     lstrcat(buffer,"'");
    }
   else 
    {
     lstrcat(buffer,"\n-");
     lstrcat(buffer,"Num Lock");
    }
   // ��� Scroll Lock
   if (LOBYTE(GKK_CNS)&CNS_SCROLLLOCK)
    {
     lstrcat(buffer,"\n+");
     lstrcat(buffer,"Scroll Lock ��� ��������� '");
     if (HIBYTE(GKK_CNS)&CNS_SCROLLLOCK) lstrcat(buffer,"�������");
     else                                lstrcat(buffer,"��������");
     lstrcat(buffer,"'");
    }
   else 
    {
     lstrcat(buffer,"\n-");
     lstrcat(buffer,"Scroll Lock");
    }
  }
 SendMessage(hwndStatus,WM_SETTEXT,0,(LPARAM)(LPCTSTR)buffer);
}