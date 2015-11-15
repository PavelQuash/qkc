/**************************************************************/
/*               qshKeyCommander1 - Settings.h                */
/*------------------------------------------------------------*/
/*    Заголовочный файл для описания функций, работающих с    */
/*                    настройками программы                   */
/*****2004***************************Copyright by <Quash!>*****/

#include "windows.h"
#include "qkc_main.h"
#include "Addiction.h"

#ifndef __SETTINGS
#define __SETTINGS

bool SettingsLoad();
void SettingsSave();
bool SettingsLanguageLoad(LPTSTR newlng);
void SettingsUpdate();

#endif