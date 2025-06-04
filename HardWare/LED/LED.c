/************************************************************
 * ��Ȩ��2025CIMC Copyright��
 * �ļ���led.c
 * ����: Lingyu Meng
 * ƽ̨: 2025CIMC IHD-V04
 * �汾: Lingyu Meng     2023/2/16     V0.01    original
 ************************************************************/

/************************* ͷ�ļ� *************************/

#include "LED.h"

/************************* �궨�� *************************/

// LED GPIO���� - ��������Ӳ���޸�
#define LED_GPIO_PORT GPIOE
#define LED_GPIO_PIN GPIO_PIN_2
#define LED_RCU RCU_GPIOE

/************************ �������� ************************/

/************************ �������� ************************/

/************************************************************
 * ����:       LED_Init(void)
 * ˵��:       LED��ʼ��
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       ��ʼ��LED GPIO
 ************************************************************/
void LED_Init(void)
{
    // ʹ��GPIOʱ��
    rcu_periph_clock_enable(LED_RCU);

    // ����GPIOΪ���ģʽ
    gpio_mode_set(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GPIO_PIN);
    gpio_output_options_set(LED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_GPIO_PIN);

    // ��ʼ��LEDΪϨ��״̬
    gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
}

/************************************************************
 * ����:       turn_on_led(uint8_t led_num)
 * ˵��:       ����LED����
 * ����:       led_num: 1-����LED, 0-Ϩ��LED
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       LED���ƺ���
 ************************************************************/
void turn_on_led(uint8_t led_num)
{
    if (led_num)
    {
        // ����LED (�͵�ƽ��������������Ӳ������)
        gpio_bit_reset(LED_GPIO_PORT, LED_GPIO_PIN);
    }
    else
    {
        // Ϩ��LED (�ߵ�ƽϨ�𣬸�������Ӳ������)
        gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
    }
}



/****************************End*****************************/
