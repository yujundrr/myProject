/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI界面主函数
*	文件名称 : MainTask.c
*	版    本 : V1.0
*	说    明 : LCD界面
*              
*	修改记录 :
*		版本号   日期         作者          说明
*		V1.0    2020-07-01      yujun  	    首版    
*                                     
*	Copyright (C), 2018-2030, ************yujun*************
*
*********************************************************************************************************
*/
#include "MainTask.h"



/*
*********************************************************************************************************
*                                               变量
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
*	函 数 名: _cbEventWindow
*	功能说明: 窗口window的事件回调函数
*	形    参: widget     窗口句柄 
*             event_ptr  事件指针
*	返 回 值: 返回0表示成功
*********************************************************************************************************
*/
UINT _cbEventWindow(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
//	GX_CHAR *buffer_address;
//	UINT buffer_size;
//	UINT content_size;

//    switch (event_ptr->gx_event_type)
//    {
//        /* 控件显示事件 */
//        case GX_EVENT_SHOW:
//     
//            /* 默认事件处理 */
//            gx_window_event_process(widget, event_ptr);
//            break;
//			
//  
//  
//        /* 按钮1 */
//        case GX_SIGNAL(GUI_ID_BUTTON0, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"1", 1);
//            break;
//		
//        /* 按钮2 */
//        case GX_SIGNAL(GUI_ID_BUTTON1, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"2", 1);
//            break;
//		
//        /* 按钮3 */
//        case GX_SIGNAL(GUI_ID_BUTTON2, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"3", 1);
//            break;
//
//        /* 清除输入框 */
//        case GX_SIGNAL(GUI_ID_BUTTON3, GX_EVENT_CLICKED):
//            gx_single_line_text_input_buffer_clear(&(window_1.window_1_text_input));
//            break;	
//
//        /* 按钮4 */
//        case GX_SIGNAL(GUI_ID_BUTTON4, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"4", 1);
//            break;	
//
//        /* 按钮5 */
//        case GX_SIGNAL(GUI_ID_BUTTON5, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"5", 1);
//            break;
//
//        /* 按钮6 */
//        case GX_SIGNAL(GUI_ID_BUTTON6, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"6", 1);
//            break;	
//
//        /* 按钮C */
//        case GX_SIGNAL(GUI_ID_BUTTON7, GX_EVENT_CLICKED):
//			gx_single_line_text_input_left_arrow(&(window_1.window_1_text_input));
//			gx_single_line_text_input_character_delete(&(window_1.window_1_text_input));
//            break;			
//		
//        /* 按钮7 */
//        case GX_SIGNAL(GUI_ID_BUTTON8, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"7", 1);
//            break;
//
//        /* 按钮8 */
//        case GX_SIGNAL(GUI_ID_BUTTON9, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"8", 1);
//            break;	
//
//        /* 按钮9 */
//        case GX_SIGNAL(GUI_ID_BUTTON10, GX_EVENT_CLICKED):
//            gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"9", 1);
//            break;	
//
//        /* 未使用*/
//        case GX_SIGNAL(GUI_ID_BUTTON11, GX_EVENT_CLICKED):
//            break;
//
//        /* 未使用*/
//        case GX_SIGNAL(GUI_ID_BUTTON12, GX_EVENT_CLICKED):
//            break;	
//
//        /* 按钮0 */
//        case GX_SIGNAL(GUI_ID_BUTTON13, GX_EVENT_CLICKED):
//			gx_single_line_text_input_character_insert(&(window_1.window_1_text_input), (GX_UBYTE *)"0", 1);
//            break;	
//
//        /* 未使用 */
//        case GX_SIGNAL(GUI_ID_BUTTON14, GX_EVENT_CLICKED):
//            break;
//
//        /* 按钮OK */
//        case GX_SIGNAL(GUI_ID_BUTTON15, GX_EVENT_CLICKED):
//			/* 获取文本 */
//			gx_single_line_text_input_buffer_get(&(window_1.window_1_text_input), &buffer_address, &content_size, &buffer_size);
//			gx_prompt_text_set(&(window.window_prompt_1), buffer_address);
//		
//			/* 默认事件处理 */
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
        /* 控件显示事件 */
        case GX_EVENT_SHOW:
     
            /* 默认事件处理 */
            gx_window_event_process(widget, event_ptr);
            break;
          


        /* 按钮1 */
        case GX_SIGNAL(NUM_BUTTON1, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"1", 1);
            break;
      
        /* 按钮2 */
        case GX_SIGNAL(NUM_BUTTON2, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"2", 1);
            break;
      
        /* 按钮3 */
        case GX_SIGNAL(NUM_BUTTON3, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"3", 1);
            break;

        /* 清除输入框 */
        case GX_SIGNAL(NUM_BUTTON_CL, GX_EVENT_CLICKED):
            gx_single_line_text_input_buffer_clear(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input));
            break;    

        /* 按钮4 */
        case GX_SIGNAL(NUM_BUTTON4, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"4", 1);
            break;    

        /* 按钮5 */
        case GX_SIGNAL(NUM_BUTTON5, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"5", 1);
            break;

        /* 按钮6 */
        case GX_SIGNAL(NUM_BUTTON6, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"6", 1);
            break;    

        /* 按钮C */
        case GX_SIGNAL(NUM_BUTTON_L, GX_EVENT_CLICKED):
          gx_single_line_text_input_left_arrow(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input));
          gx_single_line_text_input_character_delete(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input));
            break;            
      
        /* 按钮7 */
        case GX_SIGNAL(NUM_BUTTON7, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"7", 1);
            break;

        /* 按钮8 */
        case GX_SIGNAL(NUM_BUTTON8, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"8", 1);
            break;    

        /* 按钮9 */
        case GX_SIGNAL(NUM_BUTTON9, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"9", 1);
            break;    

        /* 按钮点*/
        case GX_SIGNAL(NUM_BUTTON_DOT, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)".", 1);
            break;    

        /* 未使用*/
        case GX_SIGNAL(NUM_BUTTONXX, GX_EVENT_CLICKED):
            break;    

        /* 按钮0 */
        case GX_SIGNAL(NUM_BUTTON0, GX_EVENT_CLICKED):
            gx_single_line_text_input_character_insert(&(window_eiscv_numpad.window_eiscv_numpad_numpad_text_input), (GX_UBYTE *)"0", 1);
            break;    

//        /* 未使用 */
//        case GX_SIGNAL(NUM_BUTTONXX, GX_EVENT_CLICKED):
//            break;

        /* 按钮OK */
        case GX_SIGNAL(NUM_BUTTON_OK, GX_EVENT_CLICKED):
            /* 获取文本 */
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

            /* 默认事件处理 */
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
        /* 控件显示事件 */
        case GX_EVENT_SHOW:
     
            /* 默认事件处理 */
            gx_window_event_process(widget, event_ptr);
            break;

        /* 静息时间 */
        case GX_SIGNAL(prompt_cv_time, GX_EVENT_CLICKED):
            eiscvFlag.cvTime = 1;
            break;
      
        /* 初始电压 */
        case GX_SIGNAL(prompt_cv_ov, GX_EVENT_CLICKED):
            eiscvFlag.cvOv = 1;
            break;
        
         /* 起始电压 */
        case GX_SIGNAL(prompt_cv_sv, GX_EVENT_CLICKED):
            eiscvFlag.cvSv = 1;
            break;
      
        /* 终止电压 */
        case GX_SIGNAL(prompt_cv_fv, GX_EVENT_CLICKED):
            eiscvFlag.cvFv = 1;
            break;
        
         /* 步进 */
        case GX_SIGNAL(prompt_cv_stepv, GX_EVENT_CLICKED):
            eiscvFlag.cvStepv = 1;
            break;
      
        /* 扫速 */
        case GX_SIGNAL(prompt_cv_scanv, GX_EVENT_CLICKED):
            eiscvFlag.cvScanv = 1;
            break;

        /* 开始cv测量 */
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
        /* 控件显示事件 */
        case GX_EVENT_SHOW:
     
            /* 默认事件处理 */
            gx_window_event_process(widget, event_ptr);
            break;

        /* 静息时间 */
        case GX_SIGNAL(prompt_eis_time, GX_EVENT_CLICKED):
            eiscvFlag.eisTime = 1;
            break;
      
        /* 初始电压 */
        case GX_SIGNAL(prompt_eis_dc, GX_EVENT_CLICKED):
            eiscvFlag.eisDc = 1;
            break;
        
         /* 交流电压 */
        case GX_SIGNAL(prompt_eis_ac, GX_EVENT_CLICKED):
            eiscvFlag.eisAc = 1;
            break;
      
        /* 频率个数 */
        case GX_SIGNAL(prompt_eis_num, GX_EVENT_CLICKED):
            eiscvFlag.eisFnum = 1;
            break;
        
         /* 步进 */
        case GX_SIGNAL(prompt_eis_fmax, GX_EVENT_CLICKED):
            eiscvFlag.eisFmax = 1;
            break;
      
        /* 扫速 */
        case GX_SIGNAL(prompt_eis_fmin, GX_EVENT_CLICKED):
            eiscvFlag.eisFmin = 1;
            break;
        
        /* 开始阻抗测量 */
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
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{
	/* 避免上电后瞬间的撕裂感 */
	LCD_SetBackLight(0);
	
	/*
       触摸校准函数默认是注释掉的，电阻屏需要校准，电容屏无需校准。如果用户需要校准电阻屏的话，执行
	   此函数即可，会将触摸校准参数保存到EEPROM里面，以后系统上电会自动从EEPROM里面加载。
	*/
#if 0
	LCD_SetBackLight(255);
	LCD_InitHard();
    TOUCH_Calibration(2);
#endif
	
	/*初始化配置 */
	gx_initconfig();

	/* 配置显示屏 */
    gx_studio_display_configure(DISPLAY_1, stm32h7_graphics_driver_setup_565rgb,
        LANGUAGE_CHINESE, DISPLAY_1_THEME_1, &root);


    
    /* 创建窗口 */
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
		
	/* 显示根窗口 */
    gx_widget_show(root);

    /* 启动GUIX */
    gx_system_start();
	
	tx_thread_sleep(300);
	LCD_SetBackLight(255);
	
	while(1)
	{
		tx_thread_sleep(20);
	}
}

/***************************** ************yujun************* (END OF FILE) *********************************/
