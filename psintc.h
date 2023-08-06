/*
 * psintc.h
 *
 *  Created on: 2022年2月8日
 *      Author: Administrator
 */

#include "xparameters.h"
#include "xaxivdma.h"
#include "xaxivdma_i.h"
#include "xgpiops.h"
#include "xstatus.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xgpio.h"

#define SCUGIC_ID	XPAR_SCUGIC_0_DEVICE_ID		//中断控制器 ID
#define GPIOPS_ID   XPAR_XGPIOPS_0_DEVICE_ID   	//PS端  GPIO器件 ID
#define AXI_GPIO_ID XPAR_AXI_GPIO_0_DEVICE_ID	//PL端  AXI GPIO器件 ID
#define GPIO_INT_ID XPAR_FABRIC_GPIO_0_VEC_ID	//PL端  AXI GPIO中断 ID


#define KEY_CHANNEL    1							//PL按键使用AXI GPIO通道1
#define KEY_MASK    XGPIO_IR_CH1_MASK 			//通道1的位定义
#define gt9147baseaddr XPAR_GT9147IP_0_S00_AXI_BASEADDR

//函数声明
void instance_init();							//初始化器件驱动
void axi_gpio_handler(void *CallbackRef);		//中断服务函数

//全局变量
XScuGic 		 scugic_inst;					//中断控制器  驱动实例
XScuGic_Config * scugic_cfg_ptr; 				//中断控制器  配置信息
XGpioPs          gpiops_inst;     				//PS端 GPIO 驱动实例
XGpioPs_Config * gpiops_cfg_ptr; 				//PS端 GPIO 配置信息
XGpio            axi_gpio_inst;					//PL端 AXI GPIO 驱动实例
