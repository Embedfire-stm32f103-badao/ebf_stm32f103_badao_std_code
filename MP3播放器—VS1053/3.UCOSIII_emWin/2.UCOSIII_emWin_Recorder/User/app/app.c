#include "includes.h"
#include  "app.h"

#define BK_COLOR       GUI_BLACK
#define MIDWIN_COLOR   0X303030
#define ICON_COLOR     GUI_LIGHTCYAN
#define TEXT_COLOR		 GUI_WHITE
/*
*********************************************************************************************************
*                                         宏定义 
*********************************************************************************************************
*/
#define ICONVIEW_Width     48   /* 控件ICONVIEW的宽 */  
#define ICONVIEW_Height    48   /* 控件ICONVIEW的高，包括图标的Y方向间距 */  
#define ICONVIEW_XSpace    10   /* 控件ICONVIEW的X方向间距 */ 

/*
*********************************************************************************************************
*                                      变量
*********************************************************************************************************
*/
GUI_XBF_DATA XBF_Data;
GUI_FONT     XBF_Font;

uint8_t UserApp_Flag=0;//任务界面标志

uint8_t Flag_ICON000  = 0;   /* ICONVIEW控件按下的标志，0表示未按下，1表示按下 */
uint8_t Flag_ICON001  = 0;



/* 用于桌面ICONVIEW图标的创建 */
typedef struct {
  const GUI_BITMAP * pBitmap;
  const char       * pText;
} BITMAP_ITEM;


/* 用于桌面ICONVIEW0图标的创建 */
static const BITMAP_ITEM _aBitmapItem0[] = {
  {&bmmusic,    "Music" 		},
	{&bmrecorder,	"Recorder"	},
};

//void FUN_ICON000Clicked(void)  {printf("FUN_ICON000Clicked\n");}
//void FUN_ICON001Clicked(void)  {printf("FUN_ICON001Clicked\n");}


/*
*********************************************************************************************************
*	函 数 名: _cbBkWindow
*	功能说明: 桌面窗口的回调函数 
*	形    参：WM_MESSAGE * pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
void _cbBkWindow(WM_MESSAGE * pMsg) 
{
	int NCode, Id;
	switch (pMsg->MsgId) 
	{
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
			NCode = pMsg->Data.v;                 /* Notification code */
			switch (Id) 
			{
				case GUI_ID_ICONVIEW0:
					switch (NCode) 
					{
						/* ICON控件点击消息 */
						case WM_NOTIFICATION_CLICKED:
							UserApp_Flag = 1;
							break;
						
						/* ICON控件释放消息 */
						case WM_NOTIFICATION_RELEASED: 
							
							/* 打开相应选项 */
							switch(ICONVIEW_GetSel(pMsg->hWinSrc))
							{
								/* Phone *******************************************************************/
								case 0:	
									Flag_ICON000 = 1;
									FUN_ICON000Clicked();			
									break;	
								
								/* Message ***********************************************************************/
								case 1:
									Flag_ICON001 = 1;
									FUN_ICON001Clicked();
									break;
								}	
						 break;
					}
				break;
			}			
			break;
		/* 重绘消息*/
		case WM_PAINT:		
				GUI_SetBkColor(BK_COLOR);
				GUI_SetColor(MIDWIN_COLOR);
				GUI_Clear();	
				GUI_FillRect(2,22,237,259);
				GUI_SetColor(TEXT_COLOR);
				GUI_DispStringHCenterAt("Wildfire emWin",120,2);
		break;
			
	 default:
		WM_DefaultProc(pMsg);
		break;
	}
}

/**
  * @brief  CreateBotWin，创建底部的窗口
  * @param  none
  * @retval none
  */
static void CreateIconWin(void)
{
	uint8_t i;
	WM_HWIN BOTWIN;
	/*在指定位置创建指定尺寸的ICONVIEW 小工具*/
	BOTWIN=ICONVIEW_CreateEx(
							 62,                					      /* 小工具的最左像素（在父坐标中）*/
							 27, 								                /* 小工具的最上像素（在父坐标中）*/
							 116,                               /* 小工具的水平尺寸（单位：像素）*/
							 62,				        /* 小工具的垂直尺寸（单位：像素）*/
							 WM_HBKWIN, 				          /* 父窗口的句柄。如果为0 ，则新小工具将成为桌面（顶级窗口）的子窗口 */
							 WM_CF_SHOW | WM_CF_HASTRANS,       /* 窗口创建标记。为使小工具立即可见，通常使用 WM_CF_SHOW */ 
							 0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* 默认是0，如果不够现实可设置增减垂直滚动条 */
							 GUI_ID_ICONVIEW0, 			            /* 小工具的窗口ID */
							 ICONVIEW_Width, 				            /* 图标的水平尺寸 */
							 ICONVIEW_Height+10);					        /* 图标的垂直尺寸 */
	WM_BringToTop(BOTWIN);
	/* 向ICONVIEW 小工具添加新图标 */
	for (i = 0; i < GUI_COUNTOF(_aBitmapItem0); i++) 
	{
		ICONVIEW_AddBitmapItem(BOTWIN, _aBitmapItem0[i].pBitmap,_aBitmapItem0[i].pText);
		ICONVIEW_SetTextColor(BOTWIN,i,TEXT_COLOR);
	}	
	/* 设置小工具的背景色 32 位颜色值的前8 位可用于alpha混合处理效果*/
	ICONVIEW_SetBkColor(BOTWIN, ICONVIEW_CI_SEL, ICON_COLOR | 0x80000000);
	/* 设置字体 */
	ICONVIEW_SetFont(BOTWIN, &GUI_Font10_ASCII);
	/* 设置初始选择的图标为 -1 (表示尚未选择)*/
	ICONVIEW_SetSel(BOTWIN,-1);
	/* 设置图标在x 或y 方向上的间距。*/
	ICONVIEW_SetSpace(BOTWIN, GUI_COORD_X, ICONVIEW_XSpace);
//	ICONVIEW_SetSpace(hWin, GUI_COORD_Y, ICONVIEW_YSpace);
	/* 设置对齐方式 在5.22版本中最新加入的 */
	ICONVIEW_SetIconAlign(BOTWIN, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
}
/*
*********************************************************************************************************
*
*       _cbGetData
*
* Function description
*   Callback function for getting font data
*
* Parameters:
*   Off      - Position of XBF file to be read
*   NumBytes - Number of requested bytes
*   pVoid    - Application defined pointer
*   pBuffer  - Pointer to buffer to be filled by the function
*
* Return value:
*   0 on success, 1 on error
*********************************************************************************************************
*/
static int _cbGetData(U32 Offset, U16 NumBytes, void * pVoid, void * pBuffer)
{
	SPI_FLASH_BufferRead(pBuffer,60*4096+Offset,NumBytes);
	return 0;
}

static void _CreateXBF(void) 
{
	//
	// Create XBF font
	//
	GUI_XBF_CreateFont(&XBF_Font,       // Pointer to GUI_FONT structure in RAM
					   &XBF_Data,         // Pointer to GUI_XBF_DATA structure in RAM
					   GUI_XBF_TYPE_PROP,//GUI_XBF_TYPE_PROP_EXT, 		// Font type to be created
					   _cbGetData,        // Pointer to callback function
					   0);        // Pointer to be passed to GetData function
}

/*
*********************************************************************************************************
*	函 数 名: UserAPP
*	功能说明: GUI主函数 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void UserAPP(void)
{
	UserApp_Flag=0;	
	/***************************设置皮肤色*****************************/
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	/***************************设置默认字体**********************************/
	_CreateXBF();
	GUI_UC_SetEncodeUTF8();
  GUI_SetDefaultFont(&XBF_Font);
	/*************************************************************************/
	WM_SetCallback(WM_HBKWIN, _cbBkWindow);
	
	/* 创建三个窗口 状态栏、控制栏、主窗口*/	
	CreateIconWin();
	GUI_Delay(50);
	WM_InvalidateWindow(WM_GetDialogItem(WM_HBKWIN, GUI_ID_ICONVIEW0));
	LCD_BK_EN;
	while(1)
	{
		GUI_Delay(5);
	}
}
