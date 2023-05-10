/*
*********************************************************************************************************
*	                                  
*	ģ������ : GUI����������
*	�ļ����� : MainTask.c
*	��    �� : V1.0
*	˵    �� : GUI����������
*
*		�汾��   ����         ����            ˵��
*		v1.0    2020-07-06  Eric2013  	      �װ�
*
*	Copyright (C), 2020-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __MainTask_H
#define __MainTask_H


#include "bsp.h"
#include "gx_user.h"
#include "guiapp_resources.h"
#include "guiapp_specifications.h"
#include "em.h"



/*
************************************************************************
*						�궨��
************************************************************************
*/
typedef struct EisCv_Flag
{
    SHORT eisTime;
    SHORT eisDc;
    SHORT eisAc;
    SHORT eisFnum;
    SHORT eisFmax;
    SHORT eisFmin;

    SHORT cvTime;
    SHORT cvOv;
    SHORT cvSv;
    SHORT cvFv;
    SHORT cvStepv;
    SHORT cvScanv;
}EisCv_Flag;


/*
************************************************************************
*						���ⲿ�ļ�����
************************************************************************
*/
extern void MainTask(void);
extern void TOUCH_Calibration(uint8_t _PointCount);
extern void gx_initconfig(void);
extern UINT stm32h7_graphics_driver_setup_565rgb(GX_DISPLAY *display);
extern void SendScriptToDevice(char const * scriptText);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
