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
//�궨��
#define VDMA_ID           XPAR_AXIVDMA_0_DEVICE_ID    //VDMA����ID
#define VDMA_BASEaddr     XPAR_AXI_VDMA_0_BASEADDR
#define height1           480
#define width1            800
#define KEY               11


void led_faiure(int state);
void  led_right(int state);
void  color();
void guidata_to_stream(u8 *lcd_chuli,u8 *lcd_yuanshi);
//frame buffer����ʼ��ַ
unsigned int const frame_buffer_addr = (XPAR_PS7_DDR_0_S_AXI_BASEADDR+ 0x1000000);
//������������ַ
unsigned int const frame_buffer_addr2 = (XPAR_PS7_DDR_0_S_AXI_BASEADDR+ 0x134BC00);
#define  MIO_PIN  7//�����ȷ����
#define  MIO_PIN1  8//�����������


XGpioPs_Config  *ConfigPtr; //�����˸�ָ��,Ҳ���Ǵ洢���豸��ID�ͻ���ַ��ǰ���Ǳ�����
XGpioPs Gpio;  //Ҳ�ǽṹ���������������
int Status;

#define GPIO_DEVICE_ID  	XPAR_XGPIOPS_0_DEVICE_ID  //�豸id���м����豸

int main(void)
{
	xil_printf("OV7725  successful!\r\n");
	int i,j;
	int  total;
	Xil_DCacheDisable();
	u8 *lcd_yuanshi;  //charָ��һ������1���ֽڣ�intָ��һ������4���ֽ�
    u8 *lcd_chuli;
    lcd_yuanshi=frame_buffer_addr;
    lcd_chuli=frame_buffer_addr2;
    u32 data;
	//��ʼ��GPIO
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);  //��������ID����������������Ϣ
	Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);  //���г�ʼ����������ʼ������
	//����GPIO����Ϊ���
	XGpioPs_SetDirectionPin(&Gpio, KEY, 0);//0��Ϊ���룬1��Ϊ���,�������
	//output_pin��һ��117λ�ġ�gpio��һ��ʵ��
			//�������ʹ��1��

	u32 status;
    emio_init();               //��ʼ��EMIO
    status = ov5640_init(640,480,1800,1000);    //��ʼ��ov7725

    if(status == 0)
    		xil_printf("OV7725 detected successful!\r\n");
    	else
    		xil_printf("OV7725 detected failed!\r\n");
    instance_init();
	 memset(frame_buffer_addr,0,800*480*4*3);
	 memset(frame_buffer_addr2,0,800*480*4*3);
    //����VideoMode
	//����VDMAдͨ��
     Xil_Out32(VDMA_BASEaddr+0x30, 0x1);
	 Xil_Out32(VDMA_BASEaddr+0xAc, frame_buffer_addr2);
	 Xil_Out32(VDMA_BASEaddr+0xA8, 800*3);
	 Xil_Out32(VDMA_BASEaddr+0xA4, 800*3);
	 Xil_Out32(VDMA_BASEaddr+0xA0, 480);
	//����vdma��ͨ��
	 Xil_Out32(VDMA_BASEaddr, 0x1);
	 Xil_Out32(VDMA_BASEaddr+0x5c, frame_buffer_addr2);
	 Xil_Out32(VDMA_BASEaddr+0x58, 800*3);
	 Xil_Out32(VDMA_BASEaddr+0x54, 800*3);
	 Xil_Out32(VDMA_BASEaddr+0x50, 480);

	 //����PL AXI GPIO
	     XGpio_SetDataDirection(&axi_gpio_inst, KEY_CHANNEL, 1); //����PL AXI GPIOͨ��1Ϊ����
	     XGpio_InterruptEnable(&axi_gpio_inst, KEY_MASK);		//ʹ��ͨ��1�ж�
	     XGpio_InterruptGlobalEnable(&axi_gpio_inst);			//ʹ��AXI GPIOȫ���ж�

	     //�����ж����ȼ��ʹ�������(�ߵ�ƽ����)������
	     XScuGic_SetPriorityTriggerType(&scugic_inst, GPIO_INT_ID, 0xA0, 0x1);
	     //�����ж�ID���жϴ�����
	     XScuGic_Connect(&scugic_inst, GPIO_INT_ID, axi_gpio_handler, &axi_gpio_inst);
	     //ʹ��AXI GPIO�ж�
	     XScuGic_Enable(&scugic_inst, GPIO_INT_ID);

	     //���ò����ж��쳣������
	     Xil_ExceptionInit();
	     Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
	     		(Xil_ExceptionHandler)XScuGic_InterruptHandler, &scugic_inst);
	     Xil_ExceptionEnable();


//0x34BC00=800*480*3*3����      0x119400Ϊһ֡800*480��ͼ��ռ���ڴ��С
 gui_create(&gui);

	// memcpy(lcd_chuli,lcd_yuanshi,0xBB800);
 while(1){

	 usleep(20);
	 guidata_to_stream(lcd_chuli,lcd_yuanshi);
 }



		 return 0;
}




void guidata_to_stream(u8 *lcd_chuli,u8 *lcd_yuanshi)//lcd_yuanshiΪgui���Դ��ַ
{
	int i,j;  //frame addr2Ϊʵ�ʻ�������ַ
	for(j=0;j<height1;j++)
				 {
					for(i=0;i<width1;i++)//hang
					{
					 if(i>640)
					   {
						 *(lcd_chuli+j*width1*3+i*3+0)=*(lcd_yuanshi+j*width1*4+i*4+0);   //j��ʵ��������j+1�У�
						 *(lcd_chuli+j*width1*3+i*3+1)=*(lcd_yuanshi+j*width1*4+i*4+1);
						 *(lcd_chuli+j*width1*3+i*3+2)=*(lcd_yuanshi+j*width1*4+i*4+2);  //��ɫ
					   }

					  //ˢ��Cache�����ݸ�����DDR3��
					}
				 }

}




void led_faiure(int state)
{
	//��ʼ��GPIO
			ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);  //��������ID����������������Ϣ
			Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);  //���г�ʼ����������ʼ������
			//����GPIO����Ϊ���
			XGpioPs_SetDirectionPin(&Gpio, MIO_PIN1, 1);//0��Ϊ���룬1��Ϊ���,�������
			//output_pin��һ��117λ�ġ�gpio��һ��ʵ��
			//�������ʹ��1��
			XGpioPs_SetOutputEnablePin(&Gpio, MIO_PIN1, 1);//�����ʹ��
			XGpioPs_WritePin(&Gpio, MIO_PIN1,state);

}

void  led_right(int state)
{
	            //��ʼ��GPIO
				ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);  //��������ID����������������Ϣ
				Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
						ConfigPtr->BaseAddr);  //���г�ʼ����������ʼ������
				//����GPIO����Ϊ���
				XGpioPs_SetDirectionPin(&Gpio, MIO_PIN, 1);//0��Ϊ���룬1��Ϊ���,�������
				//output_pin��һ��117λ�ġ�gpio��һ��ʵ��
				//�������ʹ��1��
				XGpioPs_SetOutputEnablePin(&Gpio, MIO_PIN,1);//�����ʹ��
				XGpioPs_WritePin(&Gpio, MIO_PIN, state);
}

void instance_init()
{
    //��ʼ���жϿ���������
	scugic_cfg_ptr = XScuGic_LookupConfig(SCUGIC_ID);
    XScuGic_CfgInitialize(&scugic_inst, scugic_cfg_ptr, scugic_cfg_ptr->CpuBaseAddress);



    //��ʼ��PL��  AXI GPIO����
    XGpio_Initialize(&axi_gpio_inst, AXI_GPIO_ID);
}

/**/


//PL�� AXI GPIO �жϷ���(����)����
void axi_gpio_handler(void *CallbackRef)
{
    int tpx,tpy;
	XGpio *GpioPtr = (XGpio *)CallbackRef;

	print("Interrupt Detected!\n");
	XGpio_InterruptDisable(GpioPtr, KEY_MASK);				//�ر�AXI GPIO�ж�ʹ��
	usleep(500);
	//tpx=GT9147IP_mReadReg(gt9147baseaddr, GT9147IP_S00_AXI_SLV_REG0_OFFSET);
	//tpy=GT9147IP_mReadReg(gt9147baseaddr, GT9147IP_S00_AXI_SLV_REG1_OFFSET);
	//printf("%d ,%d \n\r",tpx,tpy);
	//sleep(1);
	tpx=GT9147IP_mReadReg(gt9147baseaddr, GT9147IP_S00_AXI_SLV_REG0_OFFSET);
	tpy=GT9147IP_mReadReg(gt9147baseaddr, GT9147IP_S00_AXI_SLV_REG1_OFFSET);
	printf("%d ,%d \n\r",tpx,tpy);
	sleep(1);												//��ʱ1s,��������
	XGpio_InterruptClear(GpioPtr, KEY_MASK);				//����ж�
	XGpio_InterruptEnable(GpioPtr, KEY_MASK);				//ʹ��AXI GPIO�ж�
}

