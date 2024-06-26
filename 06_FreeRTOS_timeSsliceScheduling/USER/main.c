#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "sdram.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define TASK1_TASK_PRIO		2
//任务堆栈大小	
#define TASK1_STK_SIZE 		128  
//任务句柄
TaskHandle_t Task1Task_Handler;
//任务函数
void task1_task(void *pvParameters);

//任务优先级
#define TASK2_TASK_PRIO		2
//任务堆栈大小	
#define TASK2_STK_SIZE 		128  
//任务句柄
TaskHandle_t Task2Task_Handler;
//任务函数
void task2_task(void *pvParameters);

int main(void)
{
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
	delay_init(180);                //初始化延时函数
	uart_init(115200);              //初始化串口
    LED_Init();                     //初始化LED 
	KEY_Init();						//初始化按键
	SDRAM_Init();					//初始化SDRAM
	LCD_Init();						//初始化LCD
	
    POINT_COLOR = RED;
	LCD_ShowString(30,10,200,16,16,"Apollo STM32F4/F7");	
	LCD_ShowString(30,30,200,16,16,"FreeRTOS Examp 9-1");
	LCD_ShowString(30,50,200,16,16,"FreeRTOS Round Robin");
	LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,90,200,16,16,"2016/10/20");
	
    //创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄                
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建TASK1任务
    xTaskCreate((TaskFunction_t )task1_task,             
                (const char*    )"task1_task",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler);   
    //创建TASK2任务
    xTaskCreate((TaskFunction_t )task2_task,     
                (const char*    )"task2_task",   
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_TASK_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler); 
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//task1任务函数
void task1_task(void *pvParameters)
{
	u8 task1_num=0;
	while(1)
	{
		task1_num++;					//任务1执行次数加1 注意task1_num1加到255的时候会清零！！
		LED0=!LED0;
		taskENTER_CRITICAL();           //进入临界区
		printf("任务1已经执行：%d次\r\n",task1_num);
		taskEXIT_CRITICAL();            //退出临界区
		delay_xms(10);					//延时10ms，模拟任务运行10ms，此函数不会引起任务调度
	}
}

//task2任务函数
void task2_task(void *pvParameters)
{
	u8 task2_num=0;
	while(1)
	{
		task2_num++;					//任务2执行次数加1 注意task2_num1加到255的时候会清零！！
        LED1=!LED1;
		taskENTER_CRITICAL();           //进入临界区
		printf("任务2已经执行：%d次\r\n",task2_num);
		taskEXIT_CRITICAL();            //退出临界区
		delay_xms(10);					//延时10ms，模拟任务运行10ms，此函数不会引起任务调度
	}
}

