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

// 全局任务句柄变量
static task_handle_t g_led1_handle           = INVALID_TASK_HANDLE;
static task_handle_t g_led2_handle           = INVALID_TASK_HANDLE;
static task_handle_t g_led3_handle           = INVALID_TASK_HANDLE;
static task_handle_t g_led4_handle           = INVALID_TASK_HANDLE;
static task_handle_t g_display_task_handle   = INVALID_TASK_HANDLE;
static task_handle_t g_heartbeat_task_handle = INVALID_TASK_HANDLE;
static task_handle_t g_oled_test_handle      = INVALID_TASK_HANDLE;
static task_handle_t g_key_scan_handle       = INVALID_TASK_HANDLE;
static task_handle_t g_rtc_test_handle       = INVALID_TASK_HANDLE;
static task_handle_t g_spi_flash_test_handle = INVALID_TASK_HANDLE;

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

    OLED_Init();        // OLED显示屏初始化
    KEY_Init();         // 按键初始化
    spi_flash_init();   // SPI Flash初始化
    OLED_DisPlay_On();

    delay_1ms(10);   // 初始化延时

    // 添加基础OLED测试 - 仿照你的正常代码
    printf("开始OLED基础测试...\r\n");
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"**Hello CIMC**.", 16);
    OLED_ShowString(0, 16, (uint8_t*)"OLED Init OK!", 16);
    OLED_Refresh();   // 关键！必须调用刷新
    printf("OLED基础测试完成\r\n");
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
 * 函数:       task_oled_test(void)
 * 说明:       OLED测试任务
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       GitHub Copilot
 * 其他:       调度器任务函数
 ************************************************************/
void task_oled_test(void)
{
    static uint8_t oled_test_step = 0;
    switch (oled_test_step)
    {
    case 0:
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"OLED Test", 16);
        OLED_ShowString(0, 16, (uint8_t*)"Hello, World!", 16);   // 修改Y坐标避免重叠
        OLED_Refresh();                                          // 添加刷新
        break;
    case 1:
        OLED_Clear();
        OLED_DrawLine(0, 20, 127, 20);
        OLED_Refresh();   // 添加刷新
        break;
    case 2:
        OLED_Clear();
        OLED_DrawCircle(64, 40, 10);
        OLED_Refresh();   // 添加刷新
        break;
    case 3:
        OLED_Clear();
        OLED_ShowChinese(0, 50, 0, 16);    // 显
        OLED_ShowChinese(18, 50, 1, 16);   // 示
        OLED_ShowChinese(36, 50, 2, 16);   // 测
        OLED_ShowChinese(54, 50, 3, 16);   // 试
        OLED_Refresh();                    // 添加刷新
        break;
    default:
        oled_test_step = 0;   // Reset test
        OLED_Clear();
        OLED_Refresh();   // 清屏后也要刷新
        return;
    }
    oled_test_step++;
    printf("OLED Test Step: %d\r\n", oled_test_step);
}

/************************************************************
 * 函数:       task_key_scan(void)
 * 说明:       按键扫描任务 - 简化版本，使用KEY.c的防抖功能
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       GitHub Copilot
 * 其他:       调度器任务函数
 ************************************************************/
void task_key_scan(void)
{
    // 执行按键扫描
    KEY_Scan();

    // 检查按键按下事件
    if (KEY_GetPressed(1))   // KEY1被按下
    {
        printf("KEY1 Pressed\r\n");
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"KEY1 Pressed", 16);
        OLED_Refresh();   // 添加刷新
    }

    if (KEY_GetPressed(2))   // KEY2被按下
    {
        printf("KEY2 Pressed\r\n");
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"KEY2 Pressed", 16);
        OLED_Refresh();   // 添加刷新
    }

    if (KEY_GetPressed(3))   // KEY3被按下
    {
        printf("KEY3 Pressed\r\n");
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"KEY3 Pressed", 16);
        OLED_Refresh();   // 添加刷新
    }

    if (KEY_GetPressed(4))   // KEY4被按下
    {
        printf("KEY4 Pressed\r\n");
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"KEY4 Pressed", 16);
        OLED_Refresh();   // 添加刷新
    }
}
/************************************************************
 * 函数:       task_rtc_test(void)
 * 说明:       RTC测试任务
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       GitHub Copilot
 * 其他:       调度器任务函数
 ************************************************************/
void task_rtc_test(void)
{
    static uint8_t rtc_initialized = 0;
    static char    time_str[32];

    if (rtc_initialized == 0)
    {
        RTC_Init();   // 初始化RTC，但不会要求用户输入时间
        printf("RTC初始化完成\r\n");
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"RTC显示时间", 16);
        OLED_Refresh();   // 添加刷新
        rtc_initialized = 1;
    }

    // 直接获取并显示当前时间
    rtc_show_time();

    // 在OLED上显示时间
    rtc_parameter_struct rtc_time;
    rtc_current_time_get(&rtc_time);

    // 格式化时间字符串
    sprintf(time_str, "20%02x-%02x-%02x", rtc_time.year, rtc_time.month, rtc_time.date);
    OLED_ShowString(0, 2, (uint8_t*)time_str, 16);

    sprintf(time_str, "%02x:%02x:%02x", rtc_time.hour, rtc_time.minute, rtc_time.second);
    OLED_ShowString(0, 4, (uint8_t*)time_str, 16);

    OLED_Refresh();   // 添加刷新
}

/************************************************************
 * 函数:       task_spi_flash_test(void)
 * 说明:       SPI Flash测试任务 (一次性)
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       GitHub Copilot
 * 其他:       调度器任务函数, 执行一次后自动禁用
 ************************************************************/
void task_spi_flash_test(void)
{
    static uint8_t test_executed = 0;   // 标记测试是否已执行

    if (test_executed == 1)
    {
        return;   // 如果已经执行过，直接返回
    }

    test_executed = 1;   // 标记已执行

    uint32_t flash_id = 0;
    uint8_t  write_buffer[SPI_FLASH_PAGE_SIZE];
    uint8_t  read_buffer[SPI_FLASH_PAGE_SIZE];
    uint16_t i;

    printf("=== SPI Flash Test (One Time Only) ===\r\n");
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"SPI Flash Test", 16);
    OLED_Refresh();   // 添加刷新

    flash_id = spi_flash_read_id();
    printf("Flash ID: 0x%X\r\n", flash_id);
    char id_str[20];
    sprintf(id_str, "Flash ID:0x%X", flash_id);
    OLED_ShowString(0, 2, (uint8_t*)id_str, 16);
    OLED_Refresh();   // 添加刷新

    if (flash_id == SFLASH_ID)   // SFLASH_ID 在 Implement.h 中定义
    {
        printf("SPI Flash ID OK.\r\n");
        OLED_ShowString(0, 4, (uint8_t*)"ID OK", 16);
        OLED_Refresh();   // 添加刷新

        // 准备写入数据
        for (i = 0; i < SPI_FLASH_PAGE_SIZE; i++)
        {
            write_buffer[i] = i % 256;
        }

        printf("Erasing sector...\r\n");
        OLED_ShowString(0, 6, (uint8_t*)"Erasing...", 16);
        OLED_Refresh();                                // 添加刷新
        spi_flash_sector_erase(FLASH_WRITE_ADDRESS);   // FLASH_WRITE_ADDRESS 在 Implement.h 中定义
        spi_flash_wait_for_write_end();
        printf("Erase complete.\r\n");

        printf("Writing data to 0x%08X...\r\n", FLASH_WRITE_ADDRESS);
        OLED_ShowString(0, 6, (uint8_t*)"Writing...", 16);
        OLED_Refresh();   // 添加刷新
        spi_flash_buffer_write(write_buffer, FLASH_WRITE_ADDRESS, SPI_FLASH_PAGE_SIZE);
        spi_flash_wait_for_write_end();
        printf("Write complete.\r\n");

        printf("Reading data from 0x%08X...\r\n", FLASH_READ_ADDRESS);   // FLASH_READ_ADDRESS 在 Implement.h 中定义
        OLED_ShowString(0, 6, (uint8_t*)"Reading...", 16);
        OLED_Refresh();   // 添加刷新
        spi_flash_buffer_read(read_buffer, FLASH_READ_ADDRESS, SPI_FLASH_PAGE_SIZE);
        printf("Read complete.\r\n");

        // 校验数据
        if (memory_compare(write_buffer, read_buffer, SPI_FLASH_PAGE_SIZE) == SUCCESS)
        {
            printf("SPI Flash Test Passed! Data verified.\r\n");
            OLED_ShowString(0, 6, (uint8_t*)"Test Passed!", 16);
        }
        else
        {
            printf("SPI Flash Test Failed! Data mismatch.\r\n");
            OLED_ShowString(0, 6, (uint8_t*)"Test Failed!", 16);
        }
        OLED_Refresh();   // 添加刷新
    }
    else
    {
        printf("SPI Flash ID Error. Expected 0x%X, Got 0x%X\r\n", SFLASH_ID, flash_id);
        OLED_ShowString(0, 4, (uint8_t*)"ID Error", 16);
        OLED_Refresh();   // 添加刷新
    }

    printf("SPI Flash test completed. Disabling task...\r\n");
    // 使用全局句柄禁用任务
    if (g_spi_flash_test_handle != INVALID_TASK_HANDLE)
    {
        scheduler_disable_task(g_spi_flash_test_handle);
        printf("SPI Flash task disabled successfully.\r\n");
    }
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
    printf("=== 多LED调度器演示 ===\n");

    // 1. 添加四个不同频率的LED任务（使用全局句柄）
    g_led1_handle = scheduler_add_task(led1_task, 500);   // LED1每500ms闪烁
    // g_led2_handle = scheduler_add_task(led2_task, 1000);   // LED2每800ms闪烁 (注释掉以减少任务数量)
    // g_led3_handle = scheduler_add_task(led3_task, 2000);   // LED3每1200ms闪烁 (注释掉以减少任务数量)
    // g_led4_handle = scheduler_add_task(led4_task, 4000);   // LED4每2000ms闪烁 (注释掉以减少任务数量)

    // 添加显示更新和心跳任务
    g_display_task_handle   = scheduler_add_task(task_display_update, 1000);   // 显示每1秒更新
    g_heartbeat_task_handle = scheduler_add_task(task_heartbeat, 10000);       // 心跳每10秒

    // 添加新的测试任务
    g_oled_test_handle      = scheduler_add_task(task_oled_test, 2000);        // OLED每2秒更新测试内容
    g_key_scan_handle       = scheduler_add_task(task_key_scan, 50);           // 按键每50ms扫描一次（降低频率）
    g_rtc_test_handle       = scheduler_add_task(task_rtc_test, 1000);         // RTC每1秒更新一次时间显示
    g_spi_flash_test_handle = scheduler_add_task(task_spi_flash_test, 5000);   // SPI Flash测试，5秒后执行一次

    // 2. 启用所有LED任务
    if (g_led1_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_led1_handle);
        printf("LED1任务已启用 - 500ms周期\n");
    }
    // if (g_led2_handle != INVALID_TASK_HANDLE)
    // {
    //     scheduler_enable_task(g_led2_handle);
    //     printf("LED2任务已启用 - 800ms周期\n");
    // }
    // if (g_led3_handle != INVALID_TASK_HANDLE)
    // {
    //     scheduler_enable_task(g_led3_handle);
    //     printf("LED3任务已启用 - 1200ms周期\n");
    // }
    // if (g_led4_handle != INVALID_TASK_HANDLE)
    // {
    //     scheduler_enable_task(g_led4_handle);
    //     printf("LED4任务已启用 - 2000ms周期\n");
    // }

    // 启用其他任务
    if (g_display_task_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_display_task_handle);   // 启用显示任务
    }
    if (g_heartbeat_task_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_heartbeat_task_handle);   // 启用心跳任务
    }
    if (g_oled_test_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_oled_test_handle);
        printf("OLED测试任务已启用 - 2000ms周期\n");
    }
    if (g_key_scan_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_key_scan_handle);
        printf("按键扫描任务已启用 - 50ms周期\n");
    }
    if (g_rtc_test_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_rtc_test_handle);
        printf("RTC测试任务已启用 - 1000ms周期\n");
    }
    if (g_spi_flash_test_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_spi_flash_test_handle);
        printf("SPI Flash测试任务已启用 - 5000ms后执行一次\n");
    }

    printf("已添加任务: %d | LED多频率调度器已启动\n", scheduler_get_task_count());
    printf("观察LED1-LED4以不同频率闪烁...\n");
    printf("OLED将显示测试信息，按键按下时会有提示，RTC时间将显示，SPI Flash将进行读写测试。\n");
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

/************************************************************
 * 函数:       memory_compare
 * 说明:       比较两段内存内容是否相同
 * 输入:       src: 源内存指针
 *             dst: 目标内存指针
 *             length: 比较的字节数
 * 返回值:     SUCCESS(0) - 相同，ERROR(1) - 不同
 * 作者:       Lingyu Meng
 ************************************************************/
ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint16_t length)
{
    uint16_t i;

    for (i = 0; i < length; i++)
    {
        if (src[i] != dst[i])
        {
            return ERROR;   // 如果有任何一个字节不同，返回ERROR(1)
        }
    }

    return SUCCESS;   // 所有字节都相同，返回SUCCESS(0)
}
/****************************End*****************************/