/************************************************************
 * 版权：2025CIMC Copyright。
 * 文件：implement.h
 * 作者: Lingyu Meng
 * 平台: 2025CIMC IHD-V04
 * 版本: Lingyu Meng     2023/2/16     V0.01    original
 ************************************************************/

#ifndef __IMPLEMENT_H
#    define __IMPLEMENT_H

/************************* 头文件 *************************/

#    include "HeaderFiles.h"

/************************* 宏定义 *************************/


/************************ 变量定义 ************************/


/************************ 函数定义 ************************/

void System_Init(void);   // 系统初始化
void UsrFunction(void);   // 用户函数

// 任务函数声明
void task_display_update(void);      // 显示更新任务
void task_led_blink(void);           // LED闪烁任务
void task_heartbeat(void);           // 系统心跳任务
void App_Task_UpdateDisplay(void);   // 兼容性函数

// 测试函数声明
void scheduler_test_demo(void);   // 调度器测试演示

#    define BUFFER_SIZE 256
#    define TX_BUFFER_SIZE (countof(tx_buffer) - 1)
#    define RX_BUFFER_SIZE 0xFF

#    define countof(a) (sizeof(a) / sizeof(*(a)))

#    define SFLASH_ID 0xC84013
#    define FLASH_WRITE_ADDRESS 0x000000
#    define FLASH_READ_ADDRESS FLASH_WRITE_ADDRESS


void      turn_on_led(uint8_t led_num);
void      get_chip_serial_num(void);
ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint16_t length);
void      test_status_led_init(void);

#endif


/****************************End*****************************/
