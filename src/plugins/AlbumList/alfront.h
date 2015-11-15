#ifndef __ALFRONT_H__
#define __ALFRONT_H__
/*
** Album List for Winamp frontend control API documentation v1.2.
** By Safai Ma.
** Copyright (C) 1999-2002
** Last updated: FEB.03.2002.
**
** Most of the stuff here are the same as using Winamp's frontend
** so that the learning curve is kept to the minimum.
**
** Introduction
** -----------------------
** This file describes a means to easily communicate to Album List
** via the classic Win32 Message API.
**
** These definitions/code assume C/C++. Porting to VB/Delphi shouldn't
** be too hard.
**
** First, you find the HWND of the Album List main window.
**
** HWND hwnd_albumlist = FindWindow("Winamp AL",NULL);
**
** Once you have the hwnd_albumlist, it's a good idea to check the version
** number. To do this, you send a WM_AL_IPC message to hwnd_albumlist.
** Note that WM_AL_IPC is defined as Win32's WM_USER.
**
** Note that sometimes you might want to use PostMessage instead of
** SendMessage.
**
** Requirements
** -----------------------
** You will need to have at least Album List 1.35 to use these functions
*/

#define WM_AL_IPC WM_USER

/**************************************************************************/

// Управление воспроизведением
#define IPC_PLAYALBUM       100 // (requires AL 1.35+)
#define IPC_PLAYRANDOMALBUM 102 // (requires AL 1.36+)
#define IPC_PLAYALLALBUMS	  116 // (requires AL 1.41+)

// Перемещение по playlist'у
#define IPC_PLAYPREVALBUM	  114 // (requires AL 1.41+)
#define IPC_PLAYNEXTALBUM	  115 // (requires AL 1.41+)

//#define IPC_GETVERSION 0        // (requires AL 1.36+)
#define IPC_GETALBUMINDEX   104 // (requires AL 1.36+)
//#define IPC_GETALBUMSIZE 101    // (requires AL 1.36+)
//#define IPC_GETALBUMYEAR 105    // (requires AL 1.36+)

#endif /* __ALFRONT_H__ */
