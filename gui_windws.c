/*
 * gui_windws.c
 *
 *  Created on: 2022年1月29日
 *      Author: Administrator
 */

#include "ugui.h"
#include <Sleep.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "gui_windows.h"

unsigned int const frame_buffer_addr3 = (XPAR_PS7_DDR_0_S_AXI_BASEADDR+ 0x1000000);


void PixelSet(UG_S16 x, UG_S16 y, UG_COLOR c)
{

	u32*pixwrite;
	pixwrite=frame_buffer_addr3;
	u32 iPixelAddr;
	iPixelAddr = y * 800  + x;
	*(pixwrite+iPixelAddr) = c;
}





void gui_create(UG_GUI *GuiPtr)
{

	UG_Init(GuiPtr, PixelSet, GUI_WIDTH, GUI_HEIGHT);
	UG_FillScreen ( C_BLACK );  //填充对应的颜色
    //create_window1();
    create_button();


 //   UG_WaitForUpdate();

}


void create_window1(void)
{
    /* Create Window 1 */
    UG_WindowCreate( &window_1, obj_buff_wnd_1, MAX_OBJECTS,window_1_callback );
    xil_printf("ok1");
    UG_WindowSetTitleText( &window_1, "huifuhao" );
    UG_WindowSetTitleTextFont( &window_1, &FONT_12X20 );


  /* Create Button 1 */
  //UG_ButtonCreate( &window_1, &button1_1, BTN_ID_0, 0,0,50,50);
 // UG_ButtonSetStyle( &window_1 , BTN_ID_0 , BTN_STYLE_3D  | BTN_STYLE_USE_ALTERNATE_COLORS);
      //UG_ButtonSetAlternateForeColor(&window_1 , BTN_ID_0 , C_RED);
      //UG_ButtonSetAlternateBackColor(&window_1 , BTN_ID_0 , C_BLUE);
      //UG_ButtonSetFont( &window_1, BTN_ID_0, &FONT_22X36 );
      //UG_ButtonSetText( &window_1, BTN_ID_0, " Now" );
     // UG_FillCircle(700,400,50,C_CHART_REUSE);
     //

      xil_printf("ok");




}

void window_1_callback( UG_MESSAGE* msg )
{
	}

void create_button()
{

	UG_FillFrame(645,0,660,479,C_MIDNIGHT_BLUE);


	UG_FillFrame(680,0,799,79,C_WHITE);
	UG_FillFrame(685,5,794,75,C_DEEP_SKY_BLUE);
	UG_FontSelect ( &FONT_24X40 );
	UG_SetBackcolor(C_DEEP_SKY_BLUE);
	UG_SetForecolor ( C_WHITE ) ;
	UG_PutString(720,25,"up");

	UG_FillFrame(680,81,799,159,C_WHITE);
	UG_FillFrame(685,85,794,154,C_DEEP_SKY_BLUE);
	UG_FontSelect ( &FONT_24X40 );
	UG_SetBackcolor(C_DEEP_SKY_BLUE);
	UG_SetForecolor ( C_WHITE ) ;
	UG_PutString(690,95,"down");


	UG_FillFrame(680,161,799,239,C_WHITE);
	UG_FillFrame(685,165,794,234,C_DEEP_SKY_BLUE);
	UG_FontSelect ( &FONT_24X40 );
	UG_SetBackcolor(C_DEEP_SKY_BLUE);
	UG_SetForecolor ( C_WHITE ) ;
	UG_PutString(690,175,"left");

	UG_FillFrame(680,241,799,319,C_WHITE);
	UG_FillFrame(685,245,794,315,C_DEEP_SKY_BLUE);
	UG_FontSelect ( &FONT_24X40 );
	UG_SetBackcolor(C_DEEP_SKY_BLUE);
	UG_SetForecolor ( C_WHITE ) ;
	UG_PutString(690,255,"rght");

	UG_FillCircle(740,400,60,C_WHITE);
	UG_FillCircle(740,400,50,C_DEEP_SKY_BLUE);
	UG_FontSelect ( &FONT_16X26 );
	UG_SetBackcolor(C_DEEP_SKY_BLUE);
	UG_SetForecolor ( C_WHITE ) ;
	UG_PutString(700,385,"begin");


}

