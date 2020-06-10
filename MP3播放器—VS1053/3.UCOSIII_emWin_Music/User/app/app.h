#ifndef	_APP_H_
#define	_APP_H_

#include "includes.h"
#include "app.h"
#include "GUI.h"
#include "DIALOG.h"

#define APPBKCOLOR  		GUI_LIGHTGRAY
#define APPTEXTCOLOR    GUI_MAGENTA

extern uint8_t   tpad_flag;
extern uint8_t   UserApp_Flag;

extern FIL       file;
extern FRESULT   result;
extern UINT      bw;

extern uint8_t   UserApp_Flag;
extern GUI_FONT  XBF_Font;

extern GUI_CONST_STORAGE GUI_BITMAP bmmusic;
extern GUI_CONST_STORAGE GUI_BITMAP bmrecorder;



extern uint8_t Flag_ICON000;
extern void FUN_ICON000Clicked(void);

//extern uint8_t Flag_ICON001;
//extern void FUN_ICON001Clicked(void);

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void UserAPP(void);
#endif	//_APP_H_
