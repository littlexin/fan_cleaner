/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup. 
 */

#include <board.h>
#include <rtthread.h>

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32f4xx_eth.h"
#endif

#ifdef RT_USING_GDB
#include <gdb_stub.h>
#endif


#ifdef USE_LOG
#define LOG(format,arg...) rt_kprintf(format,##arg);
#else 
#define LOG(format,arg...)
#endif

extern void rt_platform_init(void * para);


void toggle_led(void)
{
	GPIO_ToggleBits(GPIOE,GPIO_Pin_3);
}

void rt_init_thread_entry(void* parameter)
{
		rt_thread_t tid;
		
		/*start led thread*/
//		tid = rt_thread_create("led",led_thread,RT_NULL,1048,10,10);
//		if(tid != RT_NULL)
//				rt_thread_startup(tid);
		
		/*start platform init thread*/
		tid = rt_thread_create("platform_init", rt_platform_init, RT_NULL,1048,10,10);
		if(tid != RT_NULL)
				rt_thread_startup(tid);
		
    /* GDB STUB */
#ifdef RT_USING_GDB
    gdb_set_device("uart6");
    gdb_start();
#endif
    /* LwIP Initialization */
#ifdef RT_USING_LWIP
    {
        extern void lwip_sys_init(void);

        /* register ethernetif device */
        eth_system_device_init();

        rt_hw_stm32_eth_init();

        /* init lwip system */
        lwip_sys_init();
        rt_kprintf("TCP/IP initialized!\n");
    }
#endif
		
		
}

int rt_application_init()
{
    rt_thread_t tid;

    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

/*@}*/


//void led_thread(void)
//{
//		GPIO_InitTypeDef GPIO_InitTypeStructure;
//		rt_timer_t timer;
//		rt_tick_t timeout;
//		
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
//		GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_OUT;
//		GPIO_InitTypeStructure.GPIO_OType=GPIO_OType_PP;
//		GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_3;
//		GPIO_InitTypeStructure.GPIO_PuPd=GPIO_PuPd_UP;
//		GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_100MHz;
//		GPIO_Init(GPIOE,&GPIO_InitTypeStructure);
//		
//		/*creat timer and get timer handle*/
//		timer = rt_timer_create("led_timer",
//														(void *)toggle_led,
//														RT_NULL, 100,
//														RT_TIMER_FLAG_PERIODIC);
//		/*start led timer*/
//		rt_timer_start(timer);
//														
//		/*get timeout*/
//		rt_timer_control(timer, RT_TIMER_CTRL_GET_TIME,&timeout);
//		LOG("timeout:%d", timeout);												
//														
////	while(1)
////	{
////		/*toggle PE3 bits*/
////		GPIO_ToggleBits(GPIOE,GPIO_Pin_3);
////		/*delay 1 sec*/
////		rt_thread_delay(100);
////	}
//}
