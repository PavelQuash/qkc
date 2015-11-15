/**************************************************************/
/*             qshKeyCommander1 - CGetKbrdKey.h               */
/*------------------------------------------------------------*/
/*     «аголовочный файл дл€ работы с диалогом определени€    */
/*                         гор€чих клавиш                     */
/*****2004***************************Copyright by <Quash!>*****/

#ifndef __GETKBRDKEY
#define __GETKBRDKEY

#include "windows.h"
#include "Addiction.h"

#define CNS_CAPSLOCK   1
#define CNS_NUMLOCK    2
#define CNS_SCROLLLOCK 4

class CGetKbrdKey
{
 public:
 void GetHotKey(BOOL withcns, WORD *hotkey, WORD *cns); // ‘ункци€, возвращающа€ "гор€чую" клавишу и флаги cns
};

#endif
