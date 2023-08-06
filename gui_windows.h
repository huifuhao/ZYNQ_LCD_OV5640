/*
 * gui_windows.h
 *
 *  Created on: 2022Äê1ÔÂ29ÈÕ
 *      Author: Administrator
 */
#include "ugui.h"
#include "xil_types.h"

UG_GUI gui ; // Global GUI structure

#define GUI_HEIGHT    480
#define GUI_WIDTH     800
#define MAX_OBJECTS   15


/* Window 1 */
UG_WINDOW window_1;
UG_OBJECT obj_buff_wnd_1[MAX_OBJECTS];
UG_BUTTON button1_1;
UG_TEXTBOX textbox1_1;
UG_TEXTBOX textbox1_2;
UG_TEXTBOX textbox1_3;

void PixelSet(UG_S16 x, UG_S16 y, UG_COLOR c);
void window_1_callback( UG_MESSAGE* msg );
void create_window1(void);

void gui_create(UG_GUI *GuiPtr);
void create_button();
