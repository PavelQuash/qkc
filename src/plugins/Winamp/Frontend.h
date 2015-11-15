#ifndef _WAFE_H_
#define _WAFE_H_
/*
** Winamp frontend/plug-in control API documentation v1.1.
** By Justin Frankel. Updates by Christophe Thibault.
** Copyright (C) 1997-2000, Nullsoft Inc.
** Last updated: JUL.12.2000.
*/

// IPC_USER
#define IPC_CLOSEWINAMP 40001
#define IPC_RESTARTWINAMP 135  // (requires Winamp 2.2+)
#define IPC_SETPLAYLISTPOS 121 // (requires Winamp 2.0+)
#define IPC_GETLISTLENGTH 124  // (requires Winamp 2.0+)
#define IPC_SETVOLUME 122      // (requires Winamp 2.0+)
#define IPC_ISPLAYING 104      // (none)
#define IPC_GETLISTPOS 125     // (requires Winamp 2.05+)
#define IPC_GETOUTPUTTIME 105  // (none)
#define IPC_GETINFO 126        // (requires Winamp 2.05+)
#define IPC_GET_SHUFFLE 250    // (requires Winamp 2.4+)
#define IPC_GET_REPEAT 251     // (requires Winamp 2.4+)

// Управление текущей песней
#define WINAMP_REW5S 40144 // Rewinds 5 seconds
#define WINAMP_FFWD5S 40148 // Fast forwards 5 seconds
#define WINAMP_PLAY 40045 // Play button
#define WINAMP_PAUSE 40046 // Pause/Unpause button
#define WINAMP_STOP 40047 // Stop button
#define WINAMP_STOPAFTERTRACK 40157 // Stop after current track
#define WINAMP_FADEOUT 40147 // Fadeout and stop
#define WINAMP_VOLUP 40058 // Raise volume by 1% 
#define WINAMP_VOLDOWN 40059 // Lower volume by 1% 

// Перемещение по песням
#define WINAMP_PREVTRACK 40044 // Previous track button
#define WINAMP_NEXTTRACK 40048 // Next track button
#define WINAMP_BACK10TRACKS 40197 // Moves back 10 tracks in playlist
#define WINAMP_FORW10TRACKS 40195 // Moves forward 10 tracks in playlist
#define WINAMP_JUMPTOTIME 40193 // Open jump to time dialog
#define WINAMP_JUMPTOFILE 40194 // Open jump to file dialog

// Информация
#define WINAMP_FILEINFO 40188 // Open file info box

// Визуализация
#define WINAMP_EXECVISUAL 40192 // Execute current visualization plug-in
#define WINAMP_CONFVISUAL 40221 // Configure current visualization plug-in

// Открытие
#define WINAMP_OPENURL 40155 // Open URL dialog
#define WINAMP_OPENFILE 40029 // Open file dialog
#define WINAMP_OPENDIR 40187 // Open directory dialog
#define WINAMP_PLAYCD 40323 //  Play audio CD 
#define WINAMP_ADDTOBOOKMARK 40321 // Adds current track as a bookmark

// Косметические
#define WINAMP_TIMEELAPSED 40037 // Set time display mode to elapsed
#define WINAMP_TIMEREMAINING 40038 // Set time display mode to remaining
#define WINAMP_SHUFFLE  40023 // Toggle shuffle
#define WINAMP_REPEAT  40022 // Toggle repeat
#define WINAMP_CLEARPL 40324 // Clearing playlist

// Служебные
#define WINAMP_ABOUT 40041 // Toggle about box
#define WINAMP_MAINMENU 40043 // Toggle main menu
#define WINAMP_DOUBLESIZE 40165 // Toggle doublesize mode
#define WINAMP_ONTOP 40019 // Toggle always on top

// Управление окнами Winamp
#define WINAMP_HSWINAMP 40258 // Toggle main window visible
#define WINAMP_HSEQUALIZER 40036 // Toggle EQ
#define WINAMP_HSPLAYLIST 40040 // Toggle playlist editor
#define WINAMP_HSMINIBROWSER 40298 // Toggle minibrowser
#define WINAMP_WSWINAMP 40064 // Toggle Windowshade
#define WINAMP_WSEQUALIZER 40310
#define WINAMP_WSPLAYLIST 40266 // Toggle Playlist Windowshade

// Открытие окон настроек
#define WINAMP_HSPREFERENCES 40012 // Toggle preferences screen
#define WINAMP_PREFINDICATOR 40317
#define WINAMP_PREFVISUALIZATION 40191 // Open visualization plug-in options
#define WINAMP_SKINSELECTOR 40219 // Open skin selector
#define WINAMP_EDITBOOKMARKS 40320 // Show the edit bookmarks

/* 
Неиспользованные команды WM_COMMAND
  
  Start of playlist 40154 
  Go to end of playlist  40158 
  Open visualization options 40190 
  Toggle title Autoscrolling 40189 
  Toggle easymove 40186 
  Reload the current skin 40291 
  Load a preset from EQ 40253 
  Save a preset to EQF 40254 
  Opens load presets dialog 40172 
  Opens auto-load presets dialog 40173 
  Load default preset 40174 
  Opens save preset dialog 40175 
  Opens auto-load save preset 40176 
  Opens delete preset dialog 40178 
  Opens delete an auto load preset dialog 40180 
*/

//-!NO!-
#define IPC_DELETE 101

/*
** SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_DELETE);
**
** You can use IPC_DELETE to clear Winamp's internal playlist.
*/

//-!NO!-
#define IPC_JUMPTOTIME 106

/* (requires Winamp 1.60+)
** SendMessage(hwnd_winamp,WM_WA_IPC,ms,IPC_JUMPTOTIME);
** IPC_JUMPTOTIME sets the position in milliseconds of the 
** current song (approximately).
** Returns -1 if not playing, 1 on eof, or 0 if successful
*/

//-!NO!-
#define IPC_WRITEPLAYLIST 120

/* (requires Winamp 1.666+)
** SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_WRITEPLAYLIST);
**
** IPC_WRITEPLAYLIST writes the current playlist to <winampdir>\\Winamp.m3u,
** and returns the current playlist position.
** Kinda obsoleted by some of the 2.x new stuff, but still good for when
** using a front-end (instead of a plug-in)
*/

//-!NO!-
#define IPC_SETPANNING 123

/* (requires Winamp 2.0+)
** SendMessage(hwnd_winamp,WM_WA_IPC,panning,IPC_SETPANNING);
**
** IPC_SETPANNING sets the panning of Winamp (from 0 (left) to 255 (right)).
*/

//-!NO!-
#define IPC_GETEQDATA 127 

/* (requires Winamp 2.05+)
** int data=SendMessage(hwnd_winamp,WM_WA_IPC,pos,IPC_GETEQDATA);
**
** IPC_GETEQDATA queries the status of the EQ. 
** The value returned depends on what 'pos' is set to:
** Value      Meaning
** ------------------
** 0-9        The 10 bands of EQ data. 0-63 (+20db - -20db)
** 10         The preamp value. 0-63 (+20db - -20db)
** 11         Enabled. zero if disabled, nonzero if enabled.
** 12         Autoload. zero if disabled, nonzero if enabled.
*/

//-!NO!-
#define IPC_SETEQDATA 128

/* (requires Winamp 2.05+)
** SendMessage(hwnd_winamp,WM_WA_IPC,pos,IPC_GETEQDATA);
** SendMessage(hwnd_winamp,WM_WA_IPC,value,IPC_SETEQDATA);
**
** IPC_SETEQDATA sets the value of the last position retrieved
** by IPC_GETEQDATA.
*/

//-!NO!-
#define IPC_ADDBOOKMARK 129
/* (requires Winamp 2.4+)
** SendMessage(hwnd_winamp,WM_WA_IPC,(WPARAM)file,IPC_ADDBOOKMARK);
**
** IPC_ADDBOOKMARK will add the specified file to the Winamp bookmark list.
*/

//-!NO!-
#define IPC_MBOPEN 241
/* (requires Winamp 2.05+)
** SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_MBOPEN);
** SendMessage(hwnd_winamp,WM_WA_IPC,(WPARAM)url,IPC_MBOPEN);
**
** IPC_MBOPEN will open a new URL in the minibrowser. if url is NULL, it will open the Minibrowser window.
*/

//-!NO!-
#define IPC_INETAVAILABLE 242
/* (requires Winamp 2.05+)
** val=SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_INETAVAILABLE);
**
** IPC_INETAVAILABLE will return 1 if the Internet connection is available for Winamp.
*/

//-!NO!-
#define IPC_UPDTITLE 243
/* (requires Winamp 2.2+)
** SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_UPDTITLE);
**
** IPC_UPDTITLE will ask Winamp to update the informations about the current title.
*/

//-!NO!-
#define IPC_CHANGECURRENTFILE 245
/* (requires Winamp 2.05+)
** SendMessage(hwnd_winamp,WM_WA_IPC,(WPARAM)file,IPC_CHANGECURRENTFILE);
**
** IPC_CHANGECURRENTFILE will set the current playlist item.
*/

//-!NO!-
#define IPC_GETMBURL 246
/* (requires Winamp 2.2+)
** char buffer[4096]; // Urls can be VERY long
** SendMessage(hwnd_winamp,WM_WA_IPC,(WPARAM)buffer,IPC_GETMBURL);
**
** IPC_GETMBURL will retrieve the current Minibrowser URL into buffer.
*/

//-!NO!-
#define IPC_REFRESHPLCACHE 247
/* (requires Winamp 2.2+)
** SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_REFRESHPLCACHE);
**
** IPC_REFRESHPLCACHE will flush the playlist cache buffer.
*/

//-!NO!-
#define IPC_MBBLOCK 248
/* (requires Winamp 2.4+)
** SendMessage(hwnd_winamp,WM_WA_IPC,value,IPC_MBBLOCK);
**
** IPC_MBBLOCK will block the Minibrowser from updates if value is set to 1
*/

//-!NO!-
#define IPC_MBOPENREAL 249
/* (requires Winamp 2.4+)
** SendMessage(hwnd_winamp,WM_WA_IPC,(WPARAM)url,IPC_MBOPENREAL);
**
** IPC_MBOPENREAL works the same as IPC_MBOPEN except that it will works even if 
** IPC_MBBLOCK has been set to 1
*/

//-!NO!-
#define IPC_SET_SHUFFLE 252
/* (requires Winamp 2.4+)
** SendMessage(hwnd_winamp,WM_WA_IPC,value,IPC_SET_SHUFFLE);
**
** IPC_SET_SHUFFLE sets the status of the Shuffle option (1 to turn it on)
*/

//-!NO!-
#define IPC_SET_REPEAT 253
/* (requires Winamp 2.4+)
** SendMessage(hwnd_winamp,WM_WA_IPC,value,IPC_SET_REPEAT);
**
** IPC_SET_REPEAT sets the status of the Repeat option (1 to turn it on)
*/

/**************************************************************************/

/*
** Some API calls tend to require that you send data via WM_COPYDATA
** instead of WM_USER. Such as IPC_PLAYFILE:
*/

//-!NO!-
#define IPC_PLAYFILE 100

/*
** COPYDATASTRUCT cds;
** cds.dwData = IPC_PLAYFILE;
** cds.lpData = (void *) "file.mp3";
** cds.cbData = strlen((char *) cds.lpData)+1; // include space for null char
** SendMessage(hwnd_winamp,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
**
** This will play the file "file.mp3".
**
*/


//-!NO!-
#define IPC_CHDIR 103

/*
** COPYDATASTRUCT cds;
** cds.dwData = IPC_CHDIR;
** cds.lpData = (void *) "c:\\download";
** cds.cbData = strlen((char *) cds.lpData)+1; // include space for null char
** SendMessage(hwnd_winamp,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
**
** This will make Winamp change to the directory C:\\download
**
*/

/*
** EOF.. Enjoy.
*/

#endif
