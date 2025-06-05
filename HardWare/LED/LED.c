/************************************************************
 * ��Ȩ��2025CIMC Copyright��
 * �ļ���led.c
 * ����: Lingyu Meng
 * ƽ̨: 2025CIMC IHD-V04
 * �汾: Lingyu Meng     2025/6/4     V0.02    �Ż��������汾
 ************************************************************/

/************************* ͷ�ļ� *************************/

#include "LED.h"
#include "USART0.h"

/************************ ȫ�ֱ������� ************************/

/************************************************************
 * Function :       LED_Init
 * Comment  :       ���ڳ�ʼ��LED�˿�
 * Parameter:       null
 * Return   :       null
 * Author   :       Lingyu Meng
 * Date     :       2025-02-30 V0.1 original
 ************************************************************/

void LED_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);   // ��ʼ��GPIO_A����ʱ��

    // ��ʼ��LED1�˿�
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);             // GPIOģʽ����Ϊ���
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);   // �����������
    gpio_bit_reset(GPIOA, GPIO_PIN_4);                                              // ���ų�ʼ��ƽΪ�͵�ƽ

    // ��ʼ��LED2~4�˿ڣ�������ʼ���˿ڣ�
    gpio_mode_set(
        GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);   // GPIOģʽ����Ϊ���
    gpio_output_options_set(
        GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);   // �����������
    gpio_bit_reset(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);                          // ���ų�ʼ��ƽΪ�͵�ƽ
}

/************************************************************
 * ����:       turn_on_led(uint8_t led_num)
 * ˵��:       ����LED���� - ���ݵ���������
 * ����:       led_num: 1-����LED1, 0-Ϩ��LED1
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       Ϊ�������ṩ��LED���ƽӿ�
 ************************************************************/
void turn_on_led(uint8_t led_num)
{
    if (led_num)
    {
        LED1_ON();   // ����LED1
    }
    else
    {
        LED1_OFF();   // Ϩ��LED1
    }
}

/************************************************************
 * ����:       led_control(uint8_t led_id, uint8_t state)
 * ˵��:       ����ָ��LED�Ŀ���״̬
 * ����:       led_id: LED���(1-4), state: ״̬(0-�ر�, 1-����)
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       ֧�ֿ���LED1-LED4
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
    default: break;   // ��ЧLED��ţ������κβ���
    }
}

/************************************************************
 * ����:       led_toggle(uint8_t led_id)
 * ˵��:       �л�ָ��LED��״̬
 * ����:       led_id: LED���(1-4)
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       ֧���л�LED1-LED4״̬
 ************************************************************/
void led_toggle(uint8_t led_id)
{
    static uint8_t led_states[4] = {0, 0, 0, 0};   // ��¼LED״̬

    if (led_id >= 1 && led_id <= 4)
    {
        led_states[led_id - 1] = !led_states[led_id - 1];   // �л�״̬
        led_control(led_id, led_states[led_id - 1]);        // Ӧ����״̬
    }
}

/************************************************************
 * ����:       all_leds_off(void)
 * ˵��:       �ر�����LED
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       һ���Թر�LED1-LED4
 ************************************************************/
void all_leds_off(void)
{
    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
    LED4_OFF();
}

/************************************************************
 * ����:       all_leds_on(void)
 * ˵��:       ��������LED
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       һ���Ե���LED1-LED4
 ************************************************************/
void all_leds_on(void)
{
    LED1_ON();
    LED2_ON();
    LED3_ON();
    LED4_ON();
}

// LED������ - ���ڵ�����
/************************************************************
 * ����:       led1_task(void)
 * ˵��:       LED1��˸���� - 500msƵ��
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       ������������ - 500ms����
 ************************************************************/
void led1_task(void)
{
    led_toggle(1);   // �л�LED1״̬
                     // printf("LED1\r\n");
}

/************************************************************
 * ����:       led2_task(void)
 * ˵��:       LED2��˸���� - 800msƵ��
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       ������������ - 800ms����
 ************************************************************/
void led2_task(void)
{
    led_toggle(2);   // �л�LED2״̬
    printf("LED2\n");
}

/************************************************************
 * ����:       led3_task(void)
 * ˵��:       LED3��˸���� - 1200msƵ��
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       ������������ - 1200ms����
 ************************************************************/
void led3_task(void)
{
    led_toggle(3);   // �л�LED3״̬
    printf("LED3\n");
}

/************************************************************
 * ����:       led4_task(void)
 * ˵��:       LED4��˸���� - 2000msƵ��
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       ������������ - 2000ms����
 ************************************************************/
void led4_task(void)
{
    led_toggle(4);   // �л�LED4״̬
    printf("LED4\n");
}



/****************************End*****************************/
