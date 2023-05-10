/*
*********************************************************************************************************
*	                                  
*	ģ������ : GUI����������
*	�ļ����� : MainTask.c
*	��    �� : V1.0
*	˵    �� : LCD����
*              
*	�޸ļ�¼ :
*		�汾��   ����         ����          ˵��
*		V1.0    2020-07-01      yujun  	    �װ�    
*                                     
*	Copyright (C), 2018-2030, ************yujun*************
*
*********************************************************************************************************
*/
#include "MainTask.h"



/*
*********************************************************************************************************
*                                               ����
*********************************************************************************************************
*/
GX_WINDOW 		*pScreen;
GX_WINDOW_ROOT  *root;

EisCv_Flag eiscvFlag;

// EIS MethodSCRIPT
char const * EIS_ON_WE_C =   "e\n"
                             // declare variables for frequency, real and imaginary parts of complex result
                             "var f\n"
                             "var r\n"
                             "var j\n"
                             // set to channel 0 (Lemo)
                             "set_pgstat_chan 0\n"
                             // set mode to High Speed
                             "set_pgstat_mode 3\n"
                             // set to 50 uA current range
                             "set_range ba 50u\n"
                             "set_autoranging ba 50u 500u\n"
                             "cell_on\n"
                             // call the EIS loop with 15 mV amplitude, f_start = 200 kHz, f_end = 100 Hz, nrOfPoints = 51, 0 mV DC
                             "meas_loop_eis f r j 15m 200k 100 51 0m\n"
                             // add the returned variables to the data package
                             "pck_start\n"
                             "pck_add f\n"
                             "pck_add r\n"
                             "pck_add j\n"
                             "pck_end\n"
                             "endloop\n"
                             "on_finished:\n"
                             "cell_off\n"
                             "\n";

                            
                             
char const *EIS_SCRIPT_10_100K = " e \n"
                                 "var h"
                                 "var r"
                                 "var j"
                                 "var c"
                                 "var p"
                                 "set_pgstat_chan 1"
                                 "set_pgstat_mode 0"
                                 "set_pgstat_chan 0"
                                 "set_pgstat_mode 3"
                                 "set_max_bandwidth 100k"
                                 
"set_range_minmax da 200m 200m"
                                 "set_range ba 2950u"
                                 "set_autoranging ba 59n 2950u"
                                 "set_e 200m"
                                 "cell_on"
                                 "meas_loop_ca p c 200m 500m 5"
                                 
"pck_start"
                                 "pck_add p"
                                 "pck_add c"
                                 "pck_end"
                                 "endloop"
                                 "set_range ba 2950u"
                                 
"set_autoranging ba 59n 2950u"
                                 "set_range ab 4200m"
                                 "set_autoranging ab 4200m 4200m"
                                 "meas_loop_eis h r j 5m 100k 10 49 200m"
                                 "pck_start"
                                 
"pck_add h"
                                 "pck_add r"
                                 "pck_add j"
                                 "pck_end"
                                 "endloop"
                                 "on_finished:"
                                 
"cell_off";


//const char *CV_SCIRPT = "e"
//                        "var c"
//                        "var p"
//                        "set_pgstat_chan 1"
//                        "set_pgstat_mode 0"
//                        "set_pgstat_chan 0"
//                        "set_pgstat_mode 2"
//                        "set_max_bandwidth 200"
//                        "set_range_minmax da -200m 600m"
//                        "set_range ba 590u"
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//                        ""
//    
//      set_autoranging ba 59n 590u
//    set_e -200m
//    cell_on
//    meas_loop_ca p c -200m 500m 5
//    pck_start
//    pck_add p
//    pck_add c
//    pck_end
//    endloop
//    meas_loop_cv p c -200m -200m 600m 1m 50m nscans(2)
//    pck_start
//    pck_add p
//    pck_add c
//    pck_end
//    endloop
//    on_finished:
//    cell_off
//    
    
    
    
    
    
    
    
  
    
                                 
                             
/*
*********************************************************************************************************
*	�� �� ��: _cbEventWindow
*	����˵��: ����window���¼��ص�����
*	��    ��: widget     ���ھ�� 
*             event_ptr  �¼�ָ��
*	�� �� ֵ: ����0��ʾ�ɹ�
*********************************************************************************************************
*/
UINT _cbEventWindow(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
//	GX_CHAR *buffer_address;
//	UINT buffer_size;
//	UINT content_size;

//    switch (event_ptr->gx_event_type)
//    {
//        /* �ؼ���ʾ�¼� */
//        case GX_EVENT_SHOW:
//     
//            /* Ĭ���¼����� */
//            gx_window_event_process(widget, event_ptr);
//            break;
//			
//  
//  
//        /* ��ť1 */
//        case GX_SIGNAL(GUI_ID_BUTTON0, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"1", 1);
//            break;
//		
//        /* ��ť2 */
//        case GX_SIGNAL(GUI_ID_BUTTON1, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"2", 1);
//            break;
//		
//        /* ��ť3 */
//        case GX_SIGNAL(GUI_ID_BUTTON2, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"3", 1);
//            break;
//
//        /* �������� */
//        case GX_SIGNAL(GUI_ID_BUTTON3, GX_EVENT_CLICKED):
//            gx_single_line_text_input_buffer_clear(&(window_1.window_1_text_input));
//            break;	
//
//        /* ��ť4 */
//        case GX_SIGNAL(GUI_ID_BUTTON4, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"4", 1);
//            break;	
//
//        /* ��ť5 */
//        case GX_SIGNAL(GUI_ID_BUTTON5, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"5", 1);
//            break;
//
//        /* ��ť6 */
//        case GX_SIGNAL(GUI_ID_BUTTON6, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"6", 1);
//            break;	
//
//        /* ��ťC */
//        case GX_SIGNAL(GUI_ID_BUTTON7, GX_EVENT_CLICKED):
//			gx_single_line_text_input_left_arrow(&(window_1.window_1_text_input));
//			gx_single_line_text_input_character_delete(&(window_1.window_1_text_input));
//            break;			
//		
//        /* ��ť7 */
//        case GX_SIGNAL(GUI_ID_BUTTON8, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"7", 1);
//            break;
//
//        /* ��ť8 */
//        case GX_SIGNAL(GUI_ID_BUTTON9, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"8", 1);
//            break;	
//
//        /* ��ť9 */
//        case GX_SIGNAL(GUI_ID_BUTTON10, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"9", 1);
//            break;	
//
//        /* δʹ��*/
//        case GX_SIGNAL(GUI_ID_BUTTON11, GX_EVENT_CLICKED):
//            break;
//
//        /* δʹ��*/
//        case GX_SIGNAL(GUI_ID_BUTTON12, GX_EVENT_CLICKED):
//            break;	
//
//        /* ��ť0 */
//        case GX_SIGNAL(GUI_ID_BUTTON13, GX_EVENT_CLICKED):
//			gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"0", 1);
//            break;	
//
//        /* δʹ�� */
//        case GX_SIGNAL(GUI_ID_BUTTON14, GX_EVENT_CLICKED):
//            break;
//
//        /* ��ťOK */
//        case GX_SIGNAL(GUI_ID_BUTTON15, GX_EVENT_CLICKED):
//			/* ��ȡ�ı� */
//			gx_single_line_text_input_buffer_get(&(window_1.window_1_text_input), &buffer_address, &content_size, &buffer_size);
//			gx_prompt_text_set(&(window.window_prompt_1), buffer_address);
//		
//			/* Ĭ���¼����� */
//            gx_window_event_process(widget, event_ptr);
//            break;			
//
//        default:
//            return gx_window_event_process(widget, event_ptr);
//    }

    return 0;
}
UINT _cbEventWindowNumpad(GX_WINDOW *widget, GX_EVENT *event_ptr)
{

    
    GX_CHAR *buffer_address;
    UINT buffer_size;
    UINT content_size;

    switch (event_ptr->gx_event_type)
    {
        /* �ؼ���ʾ�¼� */
        case GX_EVENT_SHOW:
     
            /* Ĭ���¼����� */
            gx_window_event_process(widget, event_ptr);
            break;
          


        /* ��ť1 */
        case GX_SIGNAL(NUM_BUTTON1, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"1", 1);
            break;
      
        /* ��ť2 */
        case GX_SIGNAL(NUM_BUTTON2, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"2", 1);
            break;
      
        /* ��ť3 */
        case GX_SIGNAL(NUM_BUTTON3, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"3", 1);
            break;

        /* �������� */
        case GX_SIGNAL(NUM_BUTTON_CL, GX_EVENT_CLICKED):
            gx_single_line_text_input_buffer_clear(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input));
            break;    

        /* ��ť4 */
        case GX_SIGNAL(NUM_BUTTON4, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"4", 1);
            break;    

        /* ��ť5 */
        case GX_SIGNAL(NUM_BUTTON5, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"5", 1);
            break;

        /* ��ť6 */
        case GX_SIGNAL(NUM_BUTTON6, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"6", 1);
            break;    

        /* ��ťC */
        case GX_SIGNAL(NUM_BUTTON_L, GX_EVENT_CLICKED):
          gx_single_line_text_input_left_arrow(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input));
          gx_single_line_text_input_character_delete(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input));
            break;            
      
        /* ��ť7 */
        case GX_SIGNAL(NUM_BUTTON7, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"7", 1);
            break;

        /* ��ť8 */
        case GX_SIGNAL(NUM_BUTTON8, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"8", 1);
            break;    

        /* ��ť9 */
        case GX_SIGNAL(NUM_BUTTON9, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"9", 1);
            break;    

        /* ��ť��*/
        case GX_SIGNAL(NUM_BUTTON_DOT, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)".", 1);
            break;    

        /* δʹ��*/
        case GX_SIGNAL(NUM_BUTTONXX, GX_EVENT_CLICKED):
            break;    

        /* ��ť0 */
        case GX_SIGNAL(NUM_BUTTON0, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"0", 1);
            break;    

//        /* δʹ�� */
//        case GX_SIGNAL(NUM_BUTTONXX, GX_EVENT_CLICKED):
//            break;

        /* ��ťOK */
        case GX_SIGNAL(NUM_BUTTON_OK, GX_EVENT_CLICKED):
            /* ��ȡ�ı� */
            gx_single_line_text_input_buffer_get(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), &buffer_address, &content_size, &buffer_size);
//            gx_prompt_text_set(&(window_eis.window_eis_prompt_eis_time), buffer_address);
            if (eiscvFlag.eisTime){
                gx_prompt_text_set(&(window_eis.window_eis_prompt_eis_time), buffer_address);
            }else if (eiscvFlag.eisDc){
                gx_prompt_text_set(&(window_eis.window_eis_prompt_eis_dc), buffer_address);
            }else if (eiscvFlag.eisAc){
                gx_prompt_text_set(&(window_eis.window_eis_prompt_eis_ac), buffer_address);
            }else if (eiscvFlag.eisFnum){
                gx_prompt_text_set(&(window_eis.window_eis_prompt_eis_num), buffer_address);
            }else if (eiscvFlag.eisFmax){
                gx_prompt_text_set(&(window_eis.window_eis_prompt_eis_fmax), buffer_address);
            }else if (eiscvFlag.eisFmin){
                gx_prompt_text_set(&(window_eis.window_eis_prompt_eis_fmin), buffer_address);
            }else if (eiscvFlag.cvTime){
                gx_prompt_text_set(&(window_cv.window_cv_prompt_cv_time), buffer_address);
            }else if (eiscvFlag.cvOv){
                gx_prompt_text_set(&(window_cv.window_cv_prompt_cv_ov), buffer_address);
            }else if (eiscvFlag.cvSv){
                gx_prompt_text_set(&(window_cv.window_cv_prompt_cv_sv), buffer_address);
            }else if (eiscvFlag.cvFv){
                gx_prompt_text_set(&(window_cv.window_cv_prompt_cv_fv), buffer_address);
            }else if (eiscvFlag.cvStepv){
                gx_prompt_text_set(&(window_cv.window_cv_prompt_cv_stepv), buffer_address);
            }else if (eiscvFlag.cvScanv){
                gx_prompt_text_set(&(window_cv.window_cv_prompt_cv_scanv), buffer_address);
            }else{

            }

            memset(&eiscvFlag, 0, sizeof(eiscvFlag));

            /* Ĭ���¼����� */
            gx_window_event_process(widget, event_ptr);
            break;            

        default:
            return gx_window_event_process(widget, event_ptr);
    }
    return 0;
}
UINT _cbEventWindowCv(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
        /* �ؼ���ʾ�¼� */
        case GX_EVENT_SHOW:
     
            /* Ĭ���¼����� */
            gx_window_event_process(widget, event_ptr);
            break;

        /* ��Ϣʱ�� */
        case GX_SIGNAL(prompt_cv_time, GX_EVENT_CLICKED):
            eiscvFlag.cvTime = 1;
            break;
      
        /* ��ʼ��ѹ */
        case GX_SIGNAL(prompt_cv_ov, GX_EVENT_CLICKED):
            eiscvFlag.cvOv = 1;
            break;
        
         /* ��ʼ��ѹ */
        case GX_SIGNAL(prompt_cv_sv, GX_EVENT_CLICKED):
            eiscvFlag.cvSv = 1;
            break;
      
        /* ��ֹ��ѹ */
        case GX_SIGNAL(prompt_cv_fv, GX_EVENT_CLICKED):
            eiscvFlag.cvFv = 1;
            break;
        
         /* ���� */
        case GX_SIGNAL(prompt_cv_stepv, GX_EVENT_CLICKED):
            eiscvFlag.cvStepv = 1;
            break;
      
        /* ɨ�� */
        case GX_SIGNAL(prompt_cv_scanv, GX_EVENT_CLICKED):
            eiscvFlag.cvScanv = 1;
            break;

        /* ��ʼcv���� */
        case GX_SIGNAL(CV_ID_OK, GX_EVENT_CLICKED):
            SendScriptToDevice(EIS_ON_WE_C);
            break;
        
        default:
            return gx_window_event_process(widget, event_ptr);
   }
    return 0;
}
UINT _cbEventWindowEis(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
   {
        /* �ؼ���ʾ�¼� */
        case GX_EVENT_SHOW:
     
            /* Ĭ���¼����� */
            gx_window_event_process(widget, event_ptr);
            break;

        /* ��Ϣʱ�� */
        case GX_SIGNAL(prompt_eis_time, GX_EVENT_CLICKED):
            eiscvFlag.eisTime = 1;
            break;
      
        /* ��ʼ��ѹ */
        case GX_SIGNAL(prompt_eis_dc, GX_EVENT_CLICKED):
            eiscvFlag.eisDc = 1;
            break;
        
         /* ������ѹ */
        case GX_SIGNAL(prompt_eis_ac, GX_EVENT_CLICKED):
            eiscvFlag.eisAc = 1;
            break;
      
        /* Ƶ�ʸ��� */
        case GX_SIGNAL(prompt_eis_num, GX_EVENT_CLICKED):
            eiscvFlag.eisFnum = 1;
            break;
        
         /* ���� */
        case GX_SIGNAL(prompt_eis_fmax, GX_EVENT_CLICKED):
            eiscvFlag.eisFmax = 1;
            break;
      
        /* ɨ�� */
        case GX_SIGNAL(prompt_eis_fmin, GX_EVENT_CLICKED):
            eiscvFlag.eisFmin = 1;
            break;
        
        /* ��ʼ�迹���� */
        case GX_SIGNAL(EIS_ID_OK, GX_EVENT_CLICKED):
            SendScriptToDevice(EIS_SCRIPT_10_100K);
            break;

        default:
        return gx_window_event_process(widget, event_ptr);
   }

    return 0;
}


/*
*********************************************************************************************************
*	�� �� ��: MainTask
*	����˵��: GUI������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MainTask(void) 
{
	/* �����ϵ��˲���˺�Ѹ� */
	LCD_SetBackLight(0);
	
	/*
       ����У׼����Ĭ����ע�͵��ģ���������ҪУ׼������������У׼������û���ҪУ׼�������Ļ���ִ��
	   �˺������ɣ��Ὣ����У׼�������浽EEPROM���棬�Ժ�ϵͳ�ϵ���Զ���EEPROM������ء�
	*/
#if 0
	LCD_SetBackLight(255);
	LCD_InitHard();
    TOUCH_Calibration(2);
#endif
	
	/*��ʼ������ */
	gx_initconfig();

	/* ������ʾ�� */
    gx_studio_display_configure(DISPLAY_1, stm32h7_graphics_driver_setup_565rgb,
        LANGUAGE_CHINESE, DISPLAY_1_THEME_1, &root);


    
    /* �������� */
		gx_studio_named_widget_create("window_eiscv_numpad", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);
    gx_studio_named_widget_create("window_home", (GX_WIDGET *)root, (GX_WIDGET **)&pScreen);
	gx_studio_named_widget_create("window_cv", (GX_WIDGET *)root, (GX_WIDGET **)&pScreen);
	gx_studio_named_widget_create("window_eis", (GX_WIDGET *)root, (GX_WIDGET **)&pScreen);
    

    gx_studio_named_widget_create("window_eis_orig", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);
    gx_studio_named_widget_create("window_eis_orig_bo", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);
    gx_studio_named_widget_create("window_eis_orig_ny", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);
    gx_studio_named_widget_create("window_eis_orig_zri", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);

    gx_studio_named_widget_create("window_eis_sm", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);
    gx_studio_named_widget_create("window_eis_sm_bo", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);
    gx_studio_named_widget_create("window_eis_sm_ny", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);
    gx_studio_named_widget_create("window_eis_sm_zri", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);

    gx_studio_named_widget_create("window_eis_fit", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);
    gx_studio_named_widget_create("window_eis_fit_bo", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);
    gx_studio_named_widget_create("window_eis_fit_ny", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);
    gx_studio_named_widget_create("window_eis_fit_zri", (GX_WIDGET *)GX_NULL, (GX_WIDGET **)&pScreen);

    
    gx_studio_named_widget_create("window_zzz_lock", (GX_WIDGET *)root, (GX_WIDGET **)&pScreen);
		
	/* ��ʾ������ */
    gx_widget_show(root);

    /* ����GUIX */
    gx_system_start();
	
	tx_thread_sleep(300);
	LCD_SetBackLight(255);
	
	while(1)
	{
		tx_thread_sleep(20);
	}
}

/***************************** ************yujun************* (END OF FILE) *********************************/
