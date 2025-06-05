/************************************************************
 * 版权：2025CIMC Copyright。
 * 文件：led.c
 * 作者: Lingyu Meng
 * 平台: 2025CIMC IHD-V04
 * 版本: Lingyu Meng     2025/6/4     V0.02    优化调度器版本
 ************************************************************/

/************************* 头文件 *************************/

#include "LED.h"
#include "USART0.h"

/************************ 全局变量定义 ************************/

/************************************************************
 * Function :       LED_Init
 * Comment  :       用于初始化LED端口
 * Parameter:       null
 * Return   :       null
 * Author   :       Lingyu Meng
 * Date     :       2025-02-30 V0.1 original
 ************************************************************/

void LED_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);   // 初始化GPIO_A总线时钟

    // 初始化LED1端口
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);             // GPIO模式设置为输出
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);   // 输出参数设置
    gpio_bit_reset(GPIOA, GPIO_PIN_4);                                              // 引脚初始电平为低电平

    // 初始化LED2~4端口（批量初始化端口）
    gpio_mode_set(
        GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);   // GPIO模式设置为输出
    gpio_output_options_set(
        GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);   // 输出参数设置
    gpio_bit_reset(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);                          // 引脚初始电平为低电平
}

/************************************************************
 * 函数:       turn_on_led(uint8_t led_num)
 * 说明:       控制LED开关 - 兼容调度器调用
 * 输入:       led_num: 1-点亮LED1, 0-熄灭LED1
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       为调度器提供的LED控制接口
 ************************************************************/
void turn_on_led(uint8_t led_num)
{
    if (led_num)
    {
        LED1_ON();   // 点亮LED1
    }
    else
    {
        LED1_OFF();   // 熄灭LED1
    }
}

/************************************************************
 * 函数:       led_control(uint8_t led_id, uint8_t state)
 * 说明:       控制指定LED的开关状态
 * 输入:       led_id: LED编号(1-4), state: 状态(0-关闭, 1-点亮)
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       支持控制LED1-LED4
 ************************************************************/
void led_control(uint8_t led_id, uint8_t state)
{
    switch (led_id)
    {
    case 1:
        if (state)
            LED1_ON();
        else
            LED1_OFF();
        break;
    case 2:
        if (state)
            LED2_ON();
        else
            LED2_OFF();
        break;
    case 3:
        if (state)
            LED3_ON();
        else
            LED3_OFF();
        break;
    case 4:
        if (state)
            LED4_ON();
        else
            LED4_OFF();
        break;
    default: break;   // 无效LED编号，不做任何操作
    }
}

/************************************************************
 * 函数:       led_toggle(uint8_t led_id)
 * 说明:       切换指定LED的状态
 * 输入:       led_id: LED编号(1-4)
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       支持切换LED1-LED4状态
 ************************************************************/
void led_toggle(uint8_t led_id)
{
    static uint8_t led_states[4] = {0, 0, 0, 0};   // 记录LED状态

    if (led_id >= 1 && led_id <= 4)
    {
        led_states[led_id - 1] = !led_states[led_id - 1];   // 切换状态
        led_control(led_id, led_states[led_id - 1]);        // 应用新状态
    }
}

/************************************************************
 * 函数:       all_leds_off(void)
 * 说明:       关闭所有LED
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       一次性关闭LED1-LED4
 ************************************************************/
void all_leds_off(void)
{
    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
    LED4_OFF();
}

/************************************************************
 * 函数:       all_leds_on(void)
 * 说明:       点亮所有LED
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       一次性点亮LED1-LED4
 ************************************************************/
void all_leds_on(void)
{
    LED1_ON();
    LED2_ON();
    LED3_ON();
    LED4_ON();
}

// LED任务函数 - 用于调度器
/************************************************************
 * 函数:       led1_task(void)
 * 说明:       LED1闪烁任务 - 500ms频率
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       调度器任务函数 - 500ms周期
 ************************************************************/
void led1_task(void)
{
    led_toggle(1);   // 切换LED1状态
                     // printf("LED1\r\n");
}

/************************************************************
 * 函数:       led2_task(void)
 * 说明:       LED2闪烁任务 - 800ms频率
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       调度器任务函数 - 800ms周期
 ************************************************************/
void led2_task(void)
{
    led_toggle(2);   // 切换LED2状态
    printf("LED2\n");
}

/************************************************************
 * 函数:       led3_task(void)
 * 说明:       LED3闪烁任务 - 1200ms频率
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       调度器任务函数 - 1200ms周期
 ************************************************************/
void led3_task(void)
{
    led_toggle(3);   // 切换LED3状态
    printf("LED3\n");
}

/************************************************************
 * 函数:       led4_task(void)
 * 说明:       LED4闪烁任务 - 2000ms频率
 * 输入:       无
 * 输出:       无
 * 返回值:     无
 * 作者:       Lingyu Meng
 * 其他:       调度器任务函数 - 2000ms周期
 ************************************************************/
void led4_task(void)
{
    led_toggle(4);   // 切换LED4状态
    printf("LED4\n");
}



/****************************End*****************************/
