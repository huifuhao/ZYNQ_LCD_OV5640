#include <Sleep.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xaxivdma_i.h"
#include "xgpiops.h"
#include "xstatus.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "gui_windows.h"
#include "psintc.h"
#include "gt9147ip.h"


#include "xil_io.h"
#include "sleep.h"
//宏定义
#define VDMA_ID           XPAR_AXIVDMA_0_DEVICE_ID    //VDMA器件ID
#define VDMA_BASEaddr     XPAR_AXI_VDMA_0_BASEADDR
#define height1           480
#define width1            800
#define KEY               11


void led_faiure(int state);
void  led_right(int state);
void  color();
void guidata_to_stream(u8 *lcd_chuli,u8 *lcd_yuanshi);
//frame buffer的起始地址
unsigned int const frame_buffer_addr = (XPAR_PS7_DDR_0_S_AXI_BASEADDR+ 0x1000000);
//拷贝缓存区地址
unsigned int const frame_buffer_addr2 = (XPAR_PS7_DDR_0_S_AXI_BASEADDR+ 0x134BC00);
#define  MIO_PIN  7//输出正确引脚
#define  MIO_PIN1  8//输出错误引脚


XGpioPs_Config  *ConfigPtr; //定义了个指针,也就是存储了设备的ID和基地址，前面是变量名
XGpioPs Gpio;  //也是结构体变量，操作对象
int Status;

#define GPIO_DEVICE_ID  	XPAR_XGPIOPS_0_DEVICE_ID  //设备id，有几个设备

int main(void)
{
	xil_printf("OV7725  successful!\r\n");
	int i,j;
	int  total;
	Xil_DCacheDisable();
	u8 *lcd_yuanshi;  //char指针一次增加1个字节，int指针一次增加4个字节
    u8 *lcd_chuli;
    lcd_yuanshi=frame_buffer_addr;
    lcd_chuli=frame_buffer_addr2;
    u32 data;
	//初始化GPIO
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);  //根据器件ID查找器件的配置信息
	Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);  //进行初始化操作，初始化函数
	//设置GPIO方向为输出
	XGpioPs_SetDirectionPin(&Gpio, KEY, 0);//0作为输入，1作为输出,设置输出
	//output_pin是一个117位的。gpio是一个实例
			//设置输出使能1打开

	u32 status;
    emio_init();               //初始化EMIO
    status = ov5640_init(640,480,1800,1000);    //初始化ov7725

    if(status == 0)
    		xil_printf("OV7725 detected successful!\r\n");
    	else
    		xil_printf("OV7725 detected failed!\r\n");
    instance_init();
	 memset(frame_buffer_addr,0,800*480*4*3);
	 memset(frame_buffer_addr2,0,800*480*4*3);
    //设置VideoMode
	//配置VDMA写通道
     Xil_Out32(VDMA_BASEaddr+0x30, 0x1);
	 Xil_Out32(VDMA_BASEaddr+0xAc, frame_buffer_addr2);
	 Xil_Out32(VDMA_BASEaddr+0xA8, 800*3);
	 Xil_Out32(VDMA_BASEaddr+0xA4, 800*3);
	 Xil_Out32(VDMA_BASEaddr+0xA0, 480);
	//配置vdma读通道
	 Xil_Out32(VDMA_BASEaddr, 0x1);
	 Xil_Out32(VDMA_BASEaddr+0x5c, frame_buffer_addr2);
	 Xil_Out32(VDMA_BASEaddr+0x58, 800*3);
	 Xil_Out32(VDMA_BASEaddr+0x54, 800*3);
	 Xil_Out32(VDMA_BASEaddr+0x50, 480);

	 //配置PL AXI GPIO
	     XGpio_SetDataDirection(&axi_gpio_inst, KEY_CHANNEL, 1); //设置PL AXI GPIO通道1为输入
	     XGpio_InterruptEnable(&axi_gpio_inst, KEY_MASK);		//使能通道1中断
	     XGpio_InterruptGlobalEnable(&axi_gpio_inst);			//使能AXI GPIO全局中断

	     //设置中断优先级和触发类型(高电平触发)上升沿
	     XScuGic_SetPriorityTriggerType(&scugic_inst, GPIO_INT_ID, 0xA0, 0x1);
	     //关联中断ID和中断处理函数
	     XScuGic_Connect(&scugic_inst, GPIO_INT_ID, axi_gpio_handler, &axi_gpio_inst);
	     //使能AXI GPIO中断
	     XScuGic_Enable(&scugic_inst, GPIO_INT_ID);

	     //设置并打开中断异常处理功能
	     Xil_ExceptionInit();
	     Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
	     		(Xil_ExceptionHandler)XScuGic_InterruptHandler, &scugic_inst);
	     Xil_ExceptionEnable();


//0x34BC00=800*480*3*3缓存      0x119400为一帧800*480的图像占用内存大小
 gui_create(&gui);

	// memcpy(lcd_chuli,lcd_yuanshi,0xBB800);
 while(1){

	 usleep(20);
	 guidata_to_stream(lcd_chuli,lcd_yuanshi);
 }



		 return 0;
}




void guidata_to_stream(u8 *lcd_chuli,u8 *lcd_yuanshi)//lcd_yuanshi为gui的显存地址
{
	int i,j;  //frame addr2为实际缓存区地址
	for(j=0;j<height1;j++)
				 {
					for(i=0;i<width1;i++)//hang
					{
					 if(i>640)
					   {
						 *(lcd_chuli+j*width1*3+i*3+0)=*(lcd_yuanshi+j*width1*4+i*4+0);   //j行实际上走了j+1行，
						 *(lcd_chuli+j*width1*3+i*3+1)=*(lcd_yuanshi+j*width1*4+i*4+1);
						 *(lcd_chuli+j*width1*3+i*3+2)=*(lcd_yuanshi+j*width1*4+i*4+2);  //红色
					   }

					  //刷新Cache，数据更新至DDR3中
					}
				 }

}




void led_faiure(int state)
{
	//初始化GPIO
			ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);  //根据器件ID查找器件的配置信息
			Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);  //进行初始化操作，初始化函数
			//设置GPIO方向为输出
			XGpioPs_SetDirectionPin(&Gpio, MIO_PIN1, 1);//0作为输入，1作为输出,设置输出
			//output_pin是一个117位的。gpio是一个实例
			//设置输出使能1打开
			XGpioPs_SetOutputEnablePin(&Gpio, MIO_PIN1, 1);//打开输出使能
			XGpioPs_WritePin(&Gpio, MIO_PIN1,state);

}

void  led_right(int state)
{
	            //初始化GPIO
				ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);  //根据器件ID查找器件的配置信息
				Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
						ConfigPtr->BaseAddr);  //进行初始化操作，初始化函数
				//设置GPIO方向为输出
				XGpioPs_SetDirectionPin(&Gpio, MIO_PIN, 1);//0作为输入，1作为输出,设置输出
				//output_pin是一个117位的。gpio是一个实例
				//设置输出使能1打开
				XGpioPs_SetOutputEnablePin(&Gpio, MIO_PIN,1);//打开输出使能
				XGpioPs_WritePin(&Gpio, MIO_PIN, state);
}

void instance_init()
{
    //初始化中断控制器驱动
	scugic_cfg_ptr = XScuGic_LookupConfig(SCUGIC_ID);
    XScuGic_CfgInitialize(&scugic_inst, scugic_cfg_ptr, scugic_cfg_ptr->CpuBaseAddress);



    //初始化PL端  AXI GPIO驱动
    XGpio_Initialize(&axi_gpio_inst, AXI_GPIO_ID);
}

/**/


//PL端 AXI GPIO 中断服务(处理)函数
void axi_gpio_handler(void *CallbackRef)
{
    int tpx,tpy;
	XGpio *GpioPtr = (XGpio *)CallbackRef;

	print("Interrupt Detected!\n");
	XGpio_InterruptDisable(GpioPtr, KEY_MASK);				//关闭AXI GPIO中断使能
	usleep(500);
	//tpx=GT9147IP_mReadReg(gt9147baseaddr, GT9147IP_S00_AXI_SLV_REG0_OFFSET);
	//tpy=GT9147IP_mReadReg(gt9147baseaddr, GT9147IP_S00_AXI_SLV_REG1_OFFSET);
	//printf("%d ,%d \n\r",tpx,tpy);
	//sleep(1);
	tpx=GT9147IP_mReadReg(gt9147baseaddr, GT9147IP_S00_AXI_SLV_REG0_OFFSET);
	tpy=GT9147IP_mReadReg(gt9147baseaddr, GT9147IP_S00_AXI_SLV_REG1_OFFSET);
	printf("%d ,%d \n\r",tpx,tpy);
	sleep(1);												//延时1s,按键消抖
	XGpio_InterruptClear(GpioPtr, KEY_MASK);				//清除中断
	XGpio_InterruptEnable(GpioPtr, KEY_MASK);				//使能AXI GPIO中断
}

