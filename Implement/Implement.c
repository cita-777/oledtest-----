/************************************************************
 * Copyright: 2025CIMC Copyright.
 * File: implement.c
 * Author: Lingyu Meng
 * Platform: 2025CIMC IHD-V04
 * Version: Lingyu Meng 2025/6/4 V0.02 Optimized Scheduler
 ************************************************************/

/************************* ͷ�ļ� *************************/
#include "Implement.h"
#include "scheduler.h"
#include "system_gd32f4xx.h"   // ȷ��SystemCoreClock������
#include <stdint.h>            // uint32_t���Ͷ���

/************************ �������� ************************/
int   adc_value = 0;     // ADC����ֵ
float Vol_Value = 0.0;   // ADCֵת��Ϊ��ѹ


/************************ �������� ************************/

/************************************************************
 * ����:       System_Init(void)
 * ˵��:       ϵͳ��ʼ��
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       �򻯰汾��ʼ��
 ************************************************************/
void System_Init(void)
{
    systick_config();   // ʱ�����ú͵�������ʼ��
    USART0_Config();    // ���ڳ�ʼ��
    LED_Init();         // LED��ʼ��
    delay_1ms(10);      // ��ʼ����ʱ
}

/************************************************************
 * ����:       task_display_update(void)
 * ˵��:       ��ʾ�������� - �򻯰汾
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       ������������
 ************************************************************/
void task_display_update(void)
{
    static uint16_t update_count = 0;
    update_count++;   // �������ֻ���ض�������ʾ
    if (update_count % 10 == 0)
    {
        printf("��ʾ����: %d\r\n", update_count);
    }
}

/************************************************************
 * ����:       task_heartbeat(void)
 * ˵��:       �������� - ÿ10�����ϵͳ״̬
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       ������������
 ************************************************************/
void task_heartbeat(void)
{
    static int heartbeat_count = 0;
    heartbeat_count++;

    // ʹ�õ���printf���������ַ����ض�
    printf("=== ���� #%d === ��Ծ����: %d\r\n", heartbeat_count, scheduler_get_task_count());
}



/************************************************************
 * ����:       scheduler_test_demo(void)
 * ˵��:       ������������ʾ - չʾ�Ż���ļ��API
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       һ�ۿ����ĵ�����ʹ�÷�ʽ - �ĸ�LED��ͬƵ����˸
 ************************************************************/
void scheduler_test_demo(void)
{
    +printf("\n=== ��LED��������ʾ ===\n");

    // 1. ����ĸ���ͬƵ�ʵ�LED����
    task_handle_t led1_handle = scheduler_add_task(led1_task, 500);    // LED1ÿ500ms��˸
    task_handle_t led2_handle = scheduler_add_task(led2_task, 1000);   // LED2ÿ800ms��˸
    task_handle_t led3_handle = scheduler_add_task(led3_task, 2000);   // LED3ÿ1200ms��˸
    task_handle_t led4_handle = scheduler_add_task(led4_task, 4000);   // LED4ÿ2000ms��˸

    // �����ʾ���º���������
    task_handle_t display_task   = scheduler_add_task(task_display_update, 1000);   // ��ʾÿ1�����
    task_handle_t heartbeat_task = scheduler_add_task(task_heartbeat, 10000);       // ����ÿ10��

    // 2. ��������LED����
    if (led1_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(led1_handle);
        printf("LED1���������� - 500ms����\n");
    }
    if (led2_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(led2_handle);
        printf("LED2���������� - 800ms����\n");
    }
    if (led3_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(led3_handle);
        printf("LED3���������� - 1200ms����\n");
    }
    if (led4_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(led4_handle);
        printf("LED4���������� - 2000ms����\n");
    }

    // ������������
    if (display_task != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(display_task);   // ������ʾ����
    }
    if (heartbeat_task != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(heartbeat_task);   // ������������
    }

    printf("���������: %d | LED��Ƶ�ʵ�����������\n", scheduler_get_task_count());
    printf("�۲�LED1-LED4�Բ�ͬƵ����˸...\n");
}

/************************************************************
 * ����:       UsrFunction(void)
 * ˵��:       ���û����� - ���ú͹������������
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       �Ż��������������ʾ
 ************************************************************/
void UsrFunction(void)
{
    printf("\n\r=== ��LEDƵ�ʵ�������ʾ������ ===\n\r");

    // ���е�����������ʾ
    scheduler_test_demo();   // ��ѭ�� - �õ�������������
    printf("\n������ѭ��...\n");
    while (1)
    {
        // ִ�е����� - ��鲢���о���������
        scheduler_run();
        // ����ʱ����֤���ʵʱ��
    }
}


/****************************End*****************************/
