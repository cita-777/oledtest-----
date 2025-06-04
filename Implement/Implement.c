/************************************************************
 * 版权：2025CIMC Copyright。
 * 文件：implement.c
 * 作者: Lingyu Meng
 * 平台: 2025CIMC IHD-V04
 * 版本: Lingyu Meng     2025/6/4     V0.02    优化调度器版本
 ************************************************************/

/************************* 头文件 *************************/
#include "Implement.h"
#include "scheduler.h"
#include "system_gd32f4xx.h"   // 确保 SystemCoreClock 被声明

/************************ 变量定义 ************************/
int   adc_value = 0;     // ADC采样值
float Vol_Value = 0.0;   // ADC采样值转换成电压值


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
    LED_Init();         // LED 初始化
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
    update_count++;
    printf("[显示任务] 更新计数: %d\r\n", update_count);
}

/************************************************************
 * 函数:       task_led_blink(void)
 * 说明:       LED闪烁任务 - 每500ms切换一次LED状态
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       调度器任务函数
 ************************************************************/
void task_led_blink(void)
{
    static uint8_t led_state = 0;

    if (led_state)
    {
        turn_on_led(1);   // 点亮LED
    }
    else
    {
        turn_on_led(0);   // 熄灭LED
    }
    led_state = !led_state;
}

/************************************************************
 * 函数:       task_heartbeat(void)
 * 说明:       心跳任务 - 每10秒输出一次系统状态
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       调度器任务函数
 ************************************************************/
void task_heartbeat(void)
{
    static uint32_t heartbeat_count = 0;
    heartbeat_count++;
    printf("=== System Heartbeat #%lu ===\r\n", heartbeat_count);
    printf("Active Tasks: %d\r\n", scheduler_get_task_count());
    printf("SystemCoreClock: %dHz\r\n", SystemCoreClock);
}

/************************************************************
 * 函数:       App_Task_UpdateDisplay(void)
 * 说明:       保持向后兼容的显示更新函数
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者        Lingyu Meng
 * 其他:       此函数由调度器周期性调用
 ************************************************************/
void App_Task_UpdateDisplay(void)
{
    task_display_update();   // 调用新的任务函数
}

/************************************************************
 * 函数:       scheduler_test_demo(void)
 * 说明:       调度器测试演示 - 展示优化后的简洁API
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       一眼看懂的调度器使用方式
 ************************************************************/
void scheduler_test_demo(void)
{
    printf("\n=== 优化任务调度器演示 ===\n");
    printf("特点: 句柄管理、简洁API、高性能\n\n");

    // 1. 添加任务 - 简洁的API，一行搞定
    task_handle_t led_task       = scheduler_add_task(task_led_blink, 500);         // LED每500ms闪烁
    task_handle_t display_task   = scheduler_add_task(task_display_update, 1000);   // 显示每1秒更新
    task_handle_t heartbeat_task = scheduler_add_task(task_heartbeat, 10000);       // 心跳每10秒

    printf("? 添加了3个任务：\n");
    printf("  - LED闪烁任务 (句柄=%d, 间隔=500ms)\n", led_task);
    printf("  - 显示更新任务 (句柄=%d, 间隔=1000ms)\n", display_task);
    printf("  - 心跳任务 (句柄=%d, 间隔=10000ms)\n", heartbeat_task);   // 2. 验证任务是否添加成功并启用
    if (led_task != INVALID_TASK_HANDLE)
    {
        printf("? LED任务添加成功\n");
        scheduler_enable_task(led_task);   // 启用LED任务
    }
    if (display_task != INVALID_TASK_HANDLE)
    {
        printf("? 显示任务添加成功\n");
        scheduler_enable_task(display_task);   // 启用显示任务
    }
    if (heartbeat_task != INVALID_TASK_HANDLE)
    {
        printf("? 心跳任务添加成功\n");
        scheduler_enable_task(heartbeat_task);   // 启用心跳任务
    }
    printf("\n当前活跃任务数: %d\n", scheduler_get_task_count());
    printf("? 所有任务已启用，调度器开始工作...\n");
    printf("开发者可以一眼看懂的API设计！\n");
    printf("注意观察：LED闪烁、串口输出、调度器自动运行\n");
}

/************************************************************
 * 函数:       UsrFunction(void)
 * 说明:       用户主函数 - 设置和管理任务调度器
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       优化的任务调度器使用示例
 ************************************************************/
void UsrFunction(void)
{
    printf("\n\r###############################################################################\n\r");
    printf("优化任务调度器演示程序启动中...\n\r");
    printf("GD32470 SystemCoreClock: %dHz\n\r", SystemCoreClock);
    printf("###############################################################################\n\r");

    // 运行调度器测试演示
    scheduler_test_demo();   // 主循环 - 让调度器持续运行
    printf("\n进入主循环，调度器将持续工作...\n");
    while (1)
    {
        // 执行调度器 - 检查并运行就绪的任务
        scheduler_run();

        // 主循环中可以处理其他事务
        // 无延时，保证最佳实时性
    }
}


/****************************End*****************************/
