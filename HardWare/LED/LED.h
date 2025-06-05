/************************************************************
 * 版权：2025CIMC Copyright。
 * 文件：led.h
 * 作者: Lingyu Meng
 * 平台: 2025CIMC IHD-V04
 * 版本: Lingyu Meng     2025/6/4     V0.02    优化调度器版本
 ************************************************************/
#ifndef __LED_H
#    define __LED_H

/************************* 头文件 *************************/

#    include "HeaderFiles.h"

/************************* 宏定义 *************************/

// LED控制宏定义 - 高电平点亮，低电平熄灭
#    define LED1_ON() gpio_bit_set(GPIOA, GPIO_PIN_4)
#    define LED1_OFF() gpio_bit_reset(GPIOA, GPIO_PIN_4)
#    define LED2_ON() gpio_bit_set(GPIOA, GPIO_PIN_5)
#    define LED2_OFF() gpio_bit_reset(GPIOA, GPIO_PIN_5)
#    define LED3_ON() gpio_bit_set(GPIOA, GPIO_PIN_6)
#    define LED3_OFF() gpio_bit_reset(GPIOA, GPIO_PIN_6)
#    define LED4_ON() gpio_bit_set(GPIOA, GPIO_PIN_7)
#    define LED4_OFF() gpio_bit_reset(GPIOA, GPIO_PIN_7)

/************************ 变量定义 ************************/

/************************ 函数定义 ************************/

void LED_Init(void);                 // LED 初始化
void turn_on_led(uint8_t led_num);   // LED控制函数: 1-点亮LED1, 0-熄灭LED1

// 多LED控制函数
void led_control(uint8_t led_id, uint8_t state);   // 控制指定LED: led_id(1-4), state(0-关闭, 1-点亮)
void led_toggle(uint8_t led_id);                   // 切换指定LED状态: led_id(1-4)
void all_leds_off(void);                           // 关闭所有LED
void all_leds_on(void);                            // 点亮所有LED

// LED任务函数 - 用于调度器
void led1_task(void);   // LED1闪烁任务 - 500ms周期
void led2_task(void);   // LED2闪烁任务 - 800ms周期
void led3_task(void);   // LED3闪烁任务 - 1200ms周期
void led4_task(void);   // LED4闪烁任务 - 2000ms周期

#endif


/****************************End*****************************/
