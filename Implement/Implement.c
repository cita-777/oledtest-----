/************************************************************
 * Copyright: 2025CIMC Copyright.
 * File: implement.c
 * Author: Lingyu Meng
 * Platform: 2025CIMC IHD-V04
 * Version: Lingyu Meng 2025/6/4 V0.02 Optimized Scheduler
 ************************************************************/

/************************* 头文件 *************************/
#include "Implement.h"
#include "scheduler.h"
#include "system_gd32f4xx.h"   // 确保SystemCoreClock已声明
#include <stdint.h>            // uint32_t类型定义

/************************ 变量定义 ************************/
int   adc_value = 0;     // ADC采样值
float Vol_Value = 0.0;   // ADC值转换为电压


/************************ 函数定义 ************************/

/************************************************************
 * 函数:       System_Init(void)
 * 说明:       系统初始化
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       简化版本初始化
 ************************************************************/
void System_Init(void)
{
    systick_config();   // 时钟配置和调度器初始化
    USART0_Config();    // 串口初始化
    LED_Init();         // LED初始化
    delay_1ms(10);      // 初始化延时
}

/************************************************************
 * 函数:       task_display_update(void)
 * 说明:       显示更新任务 - 简化版本
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       调度器任务函数
 ************************************************************/
void task_display_update(void)
{
    static uint16_t update_count = 0;
    update_count++;   // 简化输出，只在特定次数显示
    if (update_count % 10 == 0)
    {
        printf("显示更新: %d\r\n", update_count);
    }
}

/************************************************************
 * 函数:       task_heartbeat(void)
 * 说明:       心跳任务 - 每10秒输出系统状态
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       调度器任务函数
 ************************************************************/
void task_heartbeat(void)
{
    static int heartbeat_count = 0;
    heartbeat_count++;

    // 使用单个printf避免中文字符被截断
    printf("=== 心跳 #%d === 活跃任务: %d\r\n", heartbeat_count, scheduler_get_task_count());
}



/************************************************************
 * 函数:       scheduler_test_demo(void)
 * 说明:       调度器测试演示 - 展示优化后的简洁API
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       一眼看懂的调度器使用方式 - 四个LED不同频率闪烁
 ************************************************************/
void scheduler_test_demo(void)
{
    +printf("\n=== 多LED调度器演示 ===\n");

    // 1. 添加四个不同频率的LED任务
    task_handle_t led1_handle = scheduler_add_task(led1_task, 500);    // LED1每500ms闪烁
    task_handle_t led2_handle = scheduler_add_task(led2_task, 1000);   // LED2每800ms闪烁
    task_handle_t led3_handle = scheduler_add_task(led3_task, 2000);   // LED3每1200ms闪烁
    task_handle_t led4_handle = scheduler_add_task(led4_task, 4000);   // LED4每2000ms闪烁

    // 添加显示更新和心跳任务
    task_handle_t display_task   = scheduler_add_task(task_display_update, 1000);   // 显示每1秒更新
    task_handle_t heartbeat_task = scheduler_add_task(task_heartbeat, 10000);       // 心跳每10秒

    // 2. 启用所有LED任务
    if (led1_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(led1_handle);
        printf("LED1任务已启用 - 500ms周期\n");
    }
    if (led2_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(led2_handle);
        printf("LED2任务已启用 - 800ms周期\n");
    }
    if (led3_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(led3_handle);
        printf("LED3任务已启用 - 1200ms周期\n");
    }
    if (led4_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(led4_handle);
        printf("LED4任务已启用 - 2000ms周期\n");
    }

    // 启用其他任务
    if (display_task != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(display_task);   // 启用显示任务
    }
    if (heartbeat_task != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(heartbeat_task);   // 启用心跳任务
    }

    printf("已添加任务: %d | LED多频率调度器已启动\n", scheduler_get_task_count());
    printf("观察LED1-LED4以不同频率闪烁...\n");
}

/************************************************************
 * 函数:       UsrFunction(void)
 * 说明:       主用户函数 - 设置和管理任务调度器
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       优化的任务调度器演示
 ************************************************************/
void UsrFunction(void)
{
    printf("\n\r=== 多LED频率调度器演示已启动 ===\n\r");

    // 运行调度器测试演示
    scheduler_test_demo();   // 主循环 - 让调度器持续运行
    printf("\n进入主循环...\n");
    while (1)
    {
        // 执行调度器 - 检查并运行就绪的任务
        scheduler_run();
        // 无延时，保证最佳实时性
    }
}


/****************************End*****************************/
