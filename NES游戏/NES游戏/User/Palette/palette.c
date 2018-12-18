#include "palette.h"
#include "bsp_lcd.h"



 /**
  * @brief  初始化画板界面
  * @param  无
  * @retval 无
  */	
void Palette_Init ( void )
{
	while( ! XPT2046_Touch_Calibrate () );      //等待触摸屏校正完毕
	
	
	ILI9341_GramScan ( 2 );

  /* 整屏清为白色 */
  ILI9341_Clear ( 0, 0, 320, 240, macWHITE );
  
  /* 画两条直线 */
  ILI9341_Clear ( 39,  0,  1, 30, macBLACK );
  ILI9341_Clear (  0, 29, 40,  1, macBLACK );

  ILI9341_Clear ( 0,  30, 40, 30, macGREEN );
  ILI9341_Clear ( 0,  60, 40, 30, macBLUE  );
  ILI9341_Clear ( 0,  90, 40, 30, macBRED  );
  ILI9341_Clear ( 0, 120, 40, 30, macGRED  );
  ILI9341_Clear ( 0, 150, 40, 30, macGBLUE );
  ILI9341_Clear ( 0, 180, 40, 30, macBLACK );
  ILI9341_Clear ( 0, 210, 40, 30, macRED   );  
	
	ILI9341_DispString_EN ( 7, 10, "CLR", macBACKGROUND, macRED );
	
  
}


 /**
  * @brief  在LCD指定位置描点
  * @param  x：X坐标
  * @param  y：Y坐标
  * @retval 无
  */	
void Palette_draw_point ( uint16_t x, uint16_t y )
{
  uint16_t y_pos = y;
	static uint16_t Pen_color = macBLACK;   //画笔默认为黑色 

	
  if ( x < 40 )    //在画板内取色 
  {
    if ( y > 30 )
			Pen_color = ( y_pos < 60  ) ? macGREEN :
									( y_pos < 90  ) ? macBLUE  :
								  ( y_pos < 120 ) ? macBRED  :
									( y_pos < 150 ) ? macGRED  :
									( y_pos < 180 ) ? macGBLUE :
									( y_pos < 210 ) ? macBLACK :
									( y_pos < 240 ) ? macRED   : macBLUE;
		
    else          //清屏
    {      
      #if 1
        ILI9341_Clear ( 40, 0, 280, 240, macBLACK );
      #else
        ILI9341_Clear ( 40, 0, 280, 240, macWHITE );
      #endif
			
      return;
			
    }
		
  }
	
  else
  {
    #if 0
    ILI9341_SetPointPixel (   x,   y, Pen_color );
    ILI9341_SetPointPixel ( x+1,   y, Pen_color );
    ILI9341_SetPointPixel (   x, y+1, Pen_color );
    ILI9341_SetPointPixel ( x+1, y+1, Pen_color );
		
    #else
    ILI9341_SetPointPixel (   x,   y, Pen_color );
    ILI9341_SetPointPixel ( x-1,   y, Pen_color );
    ILI9341_SetPointPixel (   x, y-1, Pen_color );
    ILI9341_SetPointPixel ( x+1,   y, Pen_color );
    ILI9341_SetPointPixel (   x, y+1, Pen_color );
		
    #endif
		
  }	
	
}

