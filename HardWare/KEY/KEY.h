/************************************************************
 * ��Ȩ��2025CIMC Copyright��
 * �ļ���key.h
 * ����: Lingyu Meng
 * ƽ̨: 2025CIMC IHD-V04
 * �汾: Lingyu Meng     2025/2/16     V0.01    original
 ************************************************************/
#ifndef __KEY_H
#    define __KEY_H

/************************* ͷ�ļ� *************************/

#    include "HeaderFiles.h"

/************************* �궨�� *************************/

// KEY���Ŷ���
#    define KEY1_PIN GPIO_PIN_2
#    define KEY2_PIN GPIO_PIN_3
#    define KEY3_PIN GPIO_PIN_4
#    define KEY4_PIN GPIO_PIN_5

// KEY�˿ڶ���
#    define KEY_PORT GPIOE
#    define KEY_CLK RCU_GPIOE

/************************ �������� ************************/


/************************ �������� ************************/

void    KEY_Init(void);                          // ������ʼ��
uint8_t KEY_Stat(uint32_t port, uint16_t pin);   // ����״̬ɨ��

#endif


/****************************End*****************************/
