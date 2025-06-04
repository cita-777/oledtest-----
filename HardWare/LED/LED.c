/************************************************************
 * 版权：2025CIMC Copyright。
 * 文件：led.c
 * 作者: Lingyu Meng
 * 平台: 2025CIMC IHD-V04
 * 版本: Lingyu Meng     2023/2/16     V0.01    original
 ************************************************************/

/************************* 头文件 *************************/

#include "LED.h"

/************************* 宏定义 *************************/

// LED GPIO定义 - 根据您的硬件修改
#define LED_GPIO_PORT GPIOE
#define LED_GPIO_PIN GPIO_PIN_2
#define LED_RCU RCU_GPIOE

/************************ 变量定义 ************************/

/************************ 函数定义 ************************/

/************************************************************
 * 函数:       LED_Init(void)
 * 说明:       LED初始化
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       初始化LED GPIO
 ************************************************************/
void LED_Init(void)
{
    // 使能GPIO时钟
    rcu_periph_clock_enable(LED_RCU);

    // 配置GPIO为输出模式
    gpio_mode_set(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GPIO_PIN);
    gpio_output_options_set(LED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_GPIO_PIN);

    // 初始化LED为熄灭状态
    gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
}

/************************************************************
 * 函数:       turn_on_led(uint8_t led_num)
 * 说明:       控制LED开关
 * 输入:       led_num: 1-点亮LED, 0-熄灭LED
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       LED控制函数
 ************************************************************/
void turn_on_led(uint8_t led_num)
{
    if (led_num)
    {
        // 点亮LED (低电平点亮，根据您的硬件调整)
        gpio_bit_reset(LED_GPIO_PORT, LED_GPIO_PIN);
    }
    else
    {
        // 熄灭LED (高电平熄灭，根据您的硬件调整)
        gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
    }
}



/****************************End*****************************/
