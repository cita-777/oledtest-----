/************************************************************
 * ��Ȩ��2025CIMC Copyright��
 * �ļ���implement.h
 * ����: Lingyu Meng
 * ƽ̨: 2025CIMC IHD-V04
 * �汾: Lingyu Meng     2023/2/16     V0.01    original
 ************************************************************/

#ifndef __IMPLEMENT_H
#    define __IMPLEMENT_H

/************************* ͷ�ļ� *************************/

#    include "HeaderFiles.h"

/************************* �궨�� *************************/


/************************ �������� ************************/


/************************ �������� ************************/

void System_Init(void);   // ϵͳ��ʼ��
void UsrFunction(void);   // �û�����

// ����������
void task_display_update(void);      // ��ʾ��������
void task_led_blink(void);           // LED��˸����
void task_heartbeat(void);           // ϵͳ��������
void App_Task_UpdateDisplay(void);   // �����Ժ���

// ���Ժ�������
void scheduler_test_demo(void);   // ������������ʾ

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
