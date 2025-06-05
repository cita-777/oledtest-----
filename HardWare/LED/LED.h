/************************************************************
 * ��Ȩ��2025CIMC Copyright��
 * �ļ���led.h
 * ����: Lingyu Meng
 * ƽ̨: 2025CIMC IHD-V04
 * �汾: Lingyu Meng     2025/6/4     V0.02    �Ż��������汾
 ************************************************************/
#ifndef __LED_H
#    define __LED_H

/************************* ͷ�ļ� *************************/

#    include "HeaderFiles.h"

/************************* �궨�� *************************/

// LED���ƺ궨�� - �ߵ�ƽ�������͵�ƽϨ��
#    define LED1_ON() gpio_bit_set(GPIOA, GPIO_PIN_4)
#    define LED1_OFF() gpio_bit_reset(GPIOA, GPIO_PIN_4)
#    define LED2_ON() gpio_bit_set(GPIOA, GPIO_PIN_5)
#    define LED2_OFF() gpio_bit_reset(GPIOA, GPIO_PIN_5)
#    define LED3_ON() gpio_bit_set(GPIOA, GPIO_PIN_6)
#    define LED3_OFF() gpio_bit_reset(GPIOA, GPIO_PIN_6)
#    define LED4_ON() gpio_bit_set(GPIOA, GPIO_PIN_7)
#    define LED4_OFF() gpio_bit_reset(GPIOA, GPIO_PIN_7)

/************************ �������� ************************/

/************************ �������� ************************/

void LED_Init(void);                 // LED ��ʼ��
void turn_on_led(uint8_t led_num);   // LED���ƺ���: 1-����LED1, 0-Ϩ��LED1

// ��LED���ƺ���
void led_control(uint8_t led_id, uint8_t state);   // ����ָ��LED: led_id(1-4), state(0-�ر�, 1-����)
void led_toggle(uint8_t led_id);                   // �л�ָ��LED״̬: led_id(1-4)
void all_leds_off(void);                           // �ر�����LED
void all_leds_on(void);                            // ��������LED

// LED������ - ���ڵ�����
void led1_task(void);   // LED1��˸���� - 500ms����
void led2_task(void);   // LED2��˸���� - 800ms����
void led3_task(void);   // LED3��˸���� - 1200ms����
void led4_task(void);   // LED4��˸���� - 2000ms����

#endif


/****************************End*****************************/
