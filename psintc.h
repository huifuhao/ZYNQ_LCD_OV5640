/*
 * psintc.h
 *
 *  Created on: 2022��2��8��
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

#define SCUGIC_ID	XPAR_SCUGIC_0_DEVICE_ID		//�жϿ����� ID
#define GPIOPS_ID   XPAR_XGPIOPS_0_DEVICE_ID   	//PS��  GPIO���� ID
#define AXI_GPIO_ID XPAR_AXI_GPIO_0_DEVICE_ID	//PL��  AXI GPIO���� ID
#define GPIO_INT_ID XPAR_FABRIC_GPIO_0_VEC_ID	//PL��  AXI GPIO�ж� ID


#define KEY_CHANNEL    1							//PL����ʹ��AXI GPIOͨ��1
#define KEY_MASK    XGPIO_IR_CH1_MASK 			//ͨ��1��λ����
#define gt9147baseaddr XPAR_GT9147IP_0_S00_AXI_BASEADDR

//��������
void instance_init();							//��ʼ����������
void axi_gpio_handler(void *CallbackRef);		//�жϷ�����

//ȫ�ֱ���
XScuGic 		 scugic_inst;					//�жϿ�����  ����ʵ��
XScuGic_Config * scugic_cfg_ptr; 				//�жϿ�����  ������Ϣ
XGpioPs          gpiops_inst;     				//PS�� GPIO ����ʵ��
XGpioPs_Config * gpiops_cfg_ptr; 				//PS�� GPIO ������Ϣ
XGpio            axi_gpio_inst;					//PL�� AXI GPIO ����ʵ��
