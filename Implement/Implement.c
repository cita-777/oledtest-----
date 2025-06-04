/************************************************************
 * ��Ȩ��2025CIMC Copyright��
 * �ļ���implement.c
 * ����: Lingyu Meng
 * ƽ̨: 2025CIMC IHD-V04
 * �汾: Lingyu Meng     2025/6/4     V0.02    �Ż��������汾
 ************************************************************/

/************************* ͷ�ļ� *************************/
#include "Implement.h"
#include "scheduler.h"
#include "system_gd32f4xx.h"   // ȷ�� SystemCoreClock ������

/************************ �������� ************************/
int   adc_value = 0;     // ADC����ֵ
float Vol_Value = 0.0;   // ADC����ֵת���ɵ�ѹֵ


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
    LED_Init();         // LED ��ʼ��
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
    update_count++;
    printf("[��ʾ����] ���¼���: %d\r\n", update_count);
}

/************************************************************
 * ����:       task_led_blink(void)
 * ˵��:       LED��˸���� - ÿ500ms�л�һ��LED״̬
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       ������������
 ************************************************************/
void task_led_blink(void)
{
    static uint8_t led_state = 0;

    if (led_state)
    {
        turn_on_led(1);   // ����LED
    }
    else
    {
        turn_on_led(0);   // Ϩ��LED
    }
    led_state = !led_state;
}

/************************************************************
 * ����:       task_heartbeat(void)
 * ˵��:       �������� - ÿ10�����һ��ϵͳ״̬
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       ������������
 ************************************************************/
void task_heartbeat(void)
{
    static uint32_t heartbeat_count = 0;
    heartbeat_count++;
    printf("=== System Heartbeat #%lu ===\r\n", heartbeat_count);
    printf("Active Tasks: %d\r\n", scheduler_get_task_count());
    printf("SystemCoreClock: %dHz\r\n", SystemCoreClock);
}

/************************************************************
 * ����:       App_Task_UpdateDisplay(void)
 * ˵��:       ���������ݵ���ʾ���º���
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����        Lingyu Meng
 * ����:       �˺����ɵ����������Ե���
 ************************************************************/
void App_Task_UpdateDisplay(void)
{
    task_display_update();   // �����µ�������
}

/************************************************************
 * ����:       scheduler_test_demo(void)
 * ˵��:       ������������ʾ - չʾ�Ż���ļ��API
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       һ�ۿ����ĵ�����ʹ�÷�ʽ
 ************************************************************/
void scheduler_test_demo(void)
{
    printf("\n=== �Ż������������ʾ ===\n");
    printf("�ص�: ����������API��������\n\n");

    // 1. ������� - ����API��һ�и㶨
    task_handle_t led_task       = scheduler_add_task(task_led_blink, 500);         // LEDÿ500ms��˸
    task_handle_t display_task   = scheduler_add_task(task_display_update, 1000);   // ��ʾÿ1�����
    task_handle_t heartbeat_task = scheduler_add_task(task_heartbeat, 10000);       // ����ÿ10��

    printf("? �����3������\n");
    printf("  - LED��˸���� (���=%d, ���=500ms)\n", led_task);
    printf("  - ��ʾ�������� (���=%d, ���=1000ms)\n", display_task);
    printf("  - �������� (���=%d, ���=10000ms)\n", heartbeat_task);   // 2. ��֤�����Ƿ���ӳɹ�������
    if (led_task != INVALID_TASK_HANDLE)
    {
        printf("? LED������ӳɹ�\n");
        scheduler_enable_task(led_task);   // ����LED����
    }
    if (display_task != INVALID_TASK_HANDLE)
    {
        printf("? ��ʾ������ӳɹ�\n");
        scheduler_enable_task(display_task);   // ������ʾ����
    }
    if (heartbeat_task != INVALID_TASK_HANDLE)
    {
        printf("? ����������ӳɹ�\n");
        scheduler_enable_task(heartbeat_task);   // ������������
    }
    printf("\n��ǰ��Ծ������: %d\n", scheduler_get_task_count());
    printf("? �������������ã���������ʼ����...\n");
    printf("�����߿���һ�ۿ�����API��ƣ�\n");
    printf("ע��۲죺LED��˸������������������Զ�����\n");
}

/************************************************************
 * ����:       UsrFunction(void)
 * ˵��:       �û������� - ���ú͹������������
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       Lingyu Meng
 * ����:       �Ż������������ʹ��ʾ��
 ************************************************************/
void UsrFunction(void)
{
    printf("\n\r###############################################################################\n\r");
    printf("�Ż������������ʾ����������...\n\r");
    printf("GD32470 SystemCoreClock: %dHz\n\r", SystemCoreClock);
    printf("###############################################################################\n\r");

    // ���е�����������ʾ
    scheduler_test_demo();   // ��ѭ�� - �õ�������������
    printf("\n������ѭ��������������������...\n");
    while (1)
    {
        // ִ�е����� - ��鲢���о���������
        scheduler_run();

        // ��ѭ���п��Դ�����������
        // ����ʱ����֤���ʵʱ��
    }
}


/****************************End*****************************/
