/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                           (c) Copyright 2009-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                         Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : JJL
*                 EHS
*                 DC
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_CFG_SERIAL_EN                          DEF_ENABLED

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_TASK_START_PRIO                    2u
#define  APP_TASK_UPDATE_PRIO                   12u
//#define  APP_TASK_SHOT_PRIO                     4u
#define  APP_TASK_USERAPP_PRIO                  13u

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE                256u
#define  APP_TASK_UPDATE_STK_SIZE               256u   
//#define  APP_TASK_SHOT_STK_SIZE                 768u     
#define  APP_TASK_USERAPP_STK_SIZE              512*2u                         




/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#define  APP_TRACE_LEVEL                            TRACE_LEVEL_INFO
#define  APP_TRACE                                  printf

#define  APP_TRACE_INFO(x)            ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DEBUG(x)           ((APP_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(APP_TRACE x) : (void)0)

//***************************PART1:ON/OFF define*******************************

#define OS_DEBUG_ON         		0
#define OS_DEBUG_ARRAY_ON  			0
#define OS_DEBUG_FUNC_ON   			0
// Log define
#define OS_INFO(fmt,arg...)           printf("<<-OS-INFO->> "fmt"\n",##arg)
#define OS_ERROR(fmt,arg...)          printf("<<-OS-ERROR->> "fmt"\n",##arg)

#define OS_DEBUG(fmt,arg...)          do{\
                                         if(OS_DEBUG_ON)\
                                         printf("<<-OS-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)
#define OS_DEBUG_ARRAY(array, num)    do{\
                                         int32_t i;\
                                         uint8_t* a = array;\
                                         if(OS_DEBUG_ARRAY_ON)\
                                         {\
                                            printf("<<-OS-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printf("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printf("\n");\
                                                }\
                                            }\
                                            printf("\n");\
                                        }\
                                       }while(0)
#define OS_DEBUG_FUNC()               do{\
                                         if(OS_DEBUG_FUNC_ON)\
                                         printf("<<-OS-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)

#endif
