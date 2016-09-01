/*
File Name:	WIN32.H

Copyright 1995 Coda Music Technology (all rights reserved)

Author:  	Randy Stokes
Created:	11/22/95

Purpose:

First, see WIN16.H for an explanation of its existence.

WIN16.H also includes some macros that I created for messages that Microsoft
left out (presumably because packing did not change in Win32). However, it still
seems more elegant and safe to extract values via a macro, so I'm adding the
ones we need. This file adds them for the 32 bit side.

Modification History:

**  11/01/96 ===== shipped Finale Windows 3.7.0 =====
**  11/21/96 ===== shipped Finale Mac 3.7.0 =====
**  12/16/96 ===== shipped Finale Windows 3.7.1 =====
**  03/31/97     WIN32.H        RES       UpDown_SetRange(), UpDown_SetPos():
**						changed from macros to inline code; added overloads
**  03/31/97     WIN32.H        RES       moved UpDown_SetRange(), UpDown_SetPos()
**						from win32.h to spincntl.h
**  07/21/97     WIN32.H        RES       comments
**  07/22/97     WIN32.H        RES       use _TXCHAR instead of TCHAR
**  10/07/97 ===== shipped Finale Win 3.8.0 =====
**  11/13/97 ===== shipped Finale Mac 3.8.0 =====
**  01/09/98 ===== shipped Finale Win 3.8.2 =====
**  06/03/98 ===== shipped Finale Win 4.0.0r2 =====
**  08/12/98     WIN32.H        RES       added support for WM_IME_CHAR
**  12/11/98 ===== shipped Finale Win 98c.r1 =====
*//*~*/
/*
$Log: WIN32.H,v $
Revision 1.1.1.1  2002/12/19 19:13:13  dfober
first import

** 
** 8     8/16/99 12:03p Rstokes
** ===== shipped Finale Win 2000 =====
** 
** 7     7/20/99 4:34p Rstokes
** ===== shipped Finale Mac 2000 =====
** 
** 6     7/12/99 3:16p Rstokes
** added tchar.h
** 
** 5     7/12/99 11:32a Rstokes
** more on _TXCHAR(), _TUCHAR()
** 
** 4     7/09/99 3:02p Beth
** #917954 and 918133, the alt characters were not working because
** GET_WM_CHAR_CH used a signed char rather then unsigned.
** 
** 3     2/08/99 12:34p Beth
** Removed _TXCHAR definition for USE_MFC configuration.   MFC already
** defines it.
 * 
 * 2     1/28/99 2:42p Rstokes
 * $Log keyword, plus possibly MSPEC/INSTUSED work although I'm trying to
 * keep that stuff in a separate check-in
*/

#ifndef _WIN32_H_
#define _WIN32_H_

#ifdef WIN32

#ifndef _INC_TCHAR
#include "tchar.h"
#endif

// WM_ERASEBKGND:

#define GET_WM_ERASEBKGND_DC(wp, lp)				(HDC)(wp)

// WM_CREATE:

#define GET_WM_CREATE_PTR(wp, lp)                 	(CREATESTRUCT *)(lp)

// generic mouse message unpacking; takes advantage of fact that all mouse messages
// are packed the same way (see flotledg.c for example of usage)

#define GET_WM_MOUSE_X(wp, lp)						(int)(short)LOWORD(lp)
#define GET_WM_MOUSE_Y(wp, lp)						(int)(short)HIWORD(lp)
#define GET_WM_MOUSE_PNT(wp, lp, pt)			\
	{ (pt)->x = GET_WM_MOUSE_X(wp, lp); (pt)->y = GET_WM_MOUSE_Y(wp, lp);}
#define GET_WM_MOUSE_KEYFLAGS(wp, lp)			 	(UINT)(wp)

// WM_LBUTTONDOWN:

#define GET_WM_LBUTTONDOWN_X			GET_WM_MOUSE_X
#define GET_WM_LBUTTONDOWN_Y			GET_WM_MOUSE_Y
#define GET_WM_LBUTTONDOWN_PNT			GET_WM_MOUSE_PNT
#define GET_WM_LBUTTONDOWN_KEYFLAGS		GET_WM_MOUSE_KEYFLAGS

// WM_RBUTTONDOWN:

#define GET_WM_RBUTTONDOWN_X			GET_WM_MOUSE_X
#define GET_WM_RBUTTONDOWN_Y			GET_WM_MOUSE_Y
#define GET_WM_RBUTTONDOWN_PNT			GET_WM_MOUSE_PNT
#define GET_WM_RBUTTONDOWN_KEYFLAGS		GET_WM_MOUSE_KEYFLAGS

// WM_LBUTTONDBLCLK:

#define GET_WM_LBUTTONDBLCLK_X			GET_WM_MOUSE_X
#define GET_WM_LBUTTONDBLCLK_Y			GET_WM_MOUSE_Y
#define GET_WM_LBUTTONDBLCLK_PNT		GET_WM_MOUSE_PNT
#define GET_WM_LBUTTONDBLCLK_KEYFLAGS	GET_WM_MOUSE_KEYFLAGS

// WM_MOUSEMOVE:

#define GET_WM_MOUSEMOVE_X				GET_WM_MOUSE_X
#define GET_WM_MOUSEMOVE_Y				GET_WM_MOUSE_Y
#define GET_WM_MOUSEMOVE_PNT			GET_WM_MOUSE_PNT
#define GET_WM_MOUSEMOVE_KEYFLAGS		GET_WM_MOUSE_KEYFLAGS

// WM_KEYDOWN:

#define GET_WM_KEYDOWN_VKEY(wp, lp)					(UINT)(wp)
#define GET_WM_KEYDOWN_REPEAT(wp, lp)				(int)(short)LOWORD(lp)
#define GET_WM_KEYDOWN_FLAGS(wp, lp)				(UINT)HIWORD(lp)

// WM_KEYUP:

#define GET_WM_KEYUP_VKEY						GET_WM_KEYDOWN_VKEY
#define GET_WM_KEYUP_REPEAT						GET_WM_KEYDOWN_REPEAT
#define GET_WM_KEYUP_FLAGS						GET_WM_KEYDOWN_FLAGS

// WM_SYSKEYDOWN:

#define GET_WM_SYSKEYDOWN_VKEY					GET_WM_KEYDOWN_VKEY
#define GET_WM_SYSKEYDOWN_REPEAT				GET_WM_KEYDOWN_REPEAT
#define GET_WM_SYSKEYDOWN_FLAGS					GET_WM_KEYDOWN_FLAGS

// WM_ENABLE:

#define GET_WM_ENABLE_FLAG(wp, lp)					(BOOL)(wp)

// WM_DRAWITEM:

#define GET_WM_DRAWITEM_PTR(wp, lp)					(const DRAWITEMSTRUCT *)(lp)

// WM_MEASUREITEM:

#define GET_WM_MEASUREITEM_PTR(wp, lp)				(MEASUREITEMSTRUCT *)(lp)

// WM_SYSCOMMAND:

#define GET_WM_SYSCOMMAND_CMD(wp, lp)				(UINT)(wp)

// WM_SIZE:

#define GET_WM_SIZE_STATE(wp, lp)					(UINT)(wp)
#define GET_WM_SIZE_CX(wp, lp)						(int)(short)LOWORD(lp)
#define GET_WM_SIZE_CY(wp, lp)						(int)(short)HIWORD(lp)

// WM_GETMINMAXINFO:

#define GET_WM_GETMINMAXINFO_PTR(wp, lp)			(LPMINMAXINFO)(lp)

// WM_SHOWWINDOW:

#define GET_WM_SHOWWINDOW_SHOW(wp, lp)				(BOOL)(wp)

// WM_ACTIVATEAPP:

#define GET_WM_ACTIVATEAPP_FLAG(wp, lp)				(BOOL)(wp)

// WM_CHAR:

// 7/21/97 RES: There is a BUG in GET_WM_CHAR_CH() as well as the HANDLE_WM_CHAR()
// in WINDOWSX.H that it is based on. (I have not found confirmation of this bug
// online.) Because this bug occurs in WINDOWSX.H, I am not fixing it there, but
// instead am undefining HANDLE_WM_CHAR() and redefining it here using _TXCHAR,
// which is the unsigned version of TCHAR from the system include TCHAR.H.
// The problem is that casting to TCHAR causes sign extension, which did not
// happen in Win16. This means high-order characters like Alt-0169 (copyright
// symbol) come through as negative numbers!

// RES 7/9/99: It looks like the definition of _TXCHAR in TCHAR.H has changed
// since I made the above comment in 1997. Now, _TXCHAR is only *sometime*
// unsigned, whereas _TUCHAR is *always* unsigned. So, until they change the
// headers again without telling us and break it all over, I'm going to use
// _TUCHAR unconditionally.

//// MFC already has this defined as an unsigned char
//#ifndef USE_MFC
//#ifndef _UNICODE
//typedef unsigned char _TXCHAR;
//#endif
//#endif  // USE_MFC
//
////#define GET_WM_CHAR_CH(wp, lp)						(TCHAR)(wp)
//// evs 7/9/99:  The bug that Randy mentions above, has shown up again.  With MFC the
//// better choice seems to be to use the already defined TUCHAR or T Unsigned CHAR.  That will
//// keep us from having to dig into the MFC headers and rearrange things.
//#ifdef USE_MFC
#define GET_WM_CHAR_CH(wp, lp)						(_TUCHAR)(wp)
//#else
//#define GET_WM_CHAR_CH(wp, lp)						(_TXCHAR)(wp)
//#endif

#undef HANDLE_WM_CHAR
#undef FORWARD_WM_CHAR

/* void Cls_OnChar(HWND hwnd, _TUCHAR ch, int cRepeat) */
#define HANDLE_WM_CHAR(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (_TUCHAR)(wParam), (int)(short)LOWORD(lParam)), 0L)
#define FORWARD_WM_CHAR(hwnd, ch, cRepeat, fn) \
    (void)(fn)((hwnd), WM_CHAR, (WPARAM)(_TUCHAR)(ch), MAKELPARAM((cRepeat),0))

// WM_IME_CHAR: (8/12/98 RES, patterned after WM_CHAR macros)

#if defined(UNICODE) || defined(_UNICODE)
#error this will have to be changed for Unicode
#endif

#define GET_WM_IME_CHAR_CH1(wp, lp)						(_TUCHAR)LOBYTE(LOWORD(wp))
#define GET_WM_IME_CHAR_CH2(wp, lp)						(_TUCHAR)HIBYTE(LOWORD(wp))

/* void Cls_OnImeChar(HWND hwnd, _TUCHAR ch1, _TUCHAR ch2, DWORD lKeyData) */
#define HANDLE_WM_IME_CHAR(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), GET_WM_IME_CHAR_CH1((wParam),(lParam)),\
     GET_WM_IME_CHAR_CH2((wParam),(lParam)), (DWORD)(lParam)), 0L)
#define FORWARD_WM_IME_CHAR(hwnd, ch1, ch2, lKeyData, fn) \
    (void)(fn)((hwnd), WM_IME_CHAR, MAKEWPARAM(MAKEWORD((ch1),(ch2)),0), MAKELPARAM((lKeyData),0))

// WM_SETTEXT:

#define GET_WM_SETTEXT_PTR(wp, lp)					(LPCTSTR)(lp)

// WM_SETREDRAW:

#define GET_WM_SETREDRAW_FLAG(wp, lp)				(BOOL)(wp)

// WM_SETFONT:

#define GET_WM_SETFONT_HFONT(wp, lp)				(HFONT)(wp)
#define GET_WM_SETFONT_REDRAW(wp, lp)				(BOOL)(lp)

// Macros for common controls. These are in the spirit of the macros for
// built-in controls in windowsx.h. For some reason, they were not extended
// to the common controls. This are Win32 only.

//#define	UpDown_SetRange(hwndCtl,lo,hi)	((void)SendMessage( hwndCtl, UDM_SETRANGE, \
//	0, (LPARAM) MAKELONG((short)hi, (short)lo  )))
//
//#define	UpDown_SetPos(hwndCtl,pos)	((short)SendMessage( hwndCtl, UDM_SETPOS, \
//	0, (LPARAM) MAKELONG((short)pos, 0  )))

#endif
#endif
