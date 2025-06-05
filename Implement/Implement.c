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

// ȫ������������
static task_handle_t g_led1_handle           = INVALID_TASK_HANDLE;
static task_handle_t g_led2_handle           = INVALID_TASK_HANDLE;
static task_handle_t g_led3_handle           = INVALID_TASK_HANDLE;
static task_handle_t g_led4_handle           = INVALID_TASK_HANDLE;
static task_handle_t g_display_task_handle   = INVALID_TASK_HANDLE;
static task_handle_t g_heartbeat_task_handle = INVALID_TASK_HANDLE;
static task_handle_t g_oled_test_handle      = INVALID_TASK_HANDLE;
static task_handle_t g_key_scan_handle       = INVALID_TASK_HANDLE;
static task_handle_t g_rtc_test_handle       = INVALID_TASK_HANDLE;
static task_handle_t g_spi_flash_test_handle = INVALID_TASK_HANDLE;

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

    OLED_Init();        // OLED��ʾ����ʼ��
    KEY_Init();         // ������ʼ��
    spi_flash_init();   // SPI Flash��ʼ��

    delay_1ms(10);   // ��ʼ����ʱ
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
 * ����:       task_oled_test(void)
 * ˵��:       OLED��������
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       GitHub Copilot
 * ����:       ������������
 ************************************************************/
void task_oled_test(void)
{
    static uint8_t oled_test_step = 0;
    switch (oled_test_step)
    {
    case 0:
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"OLED Test", 16);
        OLED_ShowString(0, 2, (uint8_t*)"Hello, World!", 16);
        break;
    case 1: OLED_DrawLine(0, 20, 127, 20); break;
    case 2: OLED_DrawCircle(64, 40, 10); break;
    case 3:
        OLED_ShowChinese(0, 50, 0, 16);    // ��
        OLED_ShowChinese(18, 50, 1, 16);   // ʾ
        OLED_ShowChinese(36, 50, 2, 16);   // ��
        OLED_ShowChinese(54, 50, 3, 16);   // ��
        break;
    default:
        oled_test_step = 0;   // Reset test
        OLED_Clear();
        return;
    }
    oled_test_step++;
    printf("OLED Test Step: %d\r\n", oled_test_step);
}

/************************************************************
 * ����:       task_key_scan(void)
 * ˵��:       ����ɨ������ - ��ӷ�������
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       GitHub Copilot
 * ����:       ������������
 ************************************************************/
void task_key_scan(void)
{
    static uint8_t key1_last_state  = 1;   // �ϴΰ���״̬��1��ʾδ����
    static uint8_t key2_last_state  = 1;
    static uint8_t key1_press_count = 0;   // �������¼����������ڷ���
    static uint8_t key2_press_count = 0;

    uint8_t key1_current = KEY_Stat(KEY_PORT, KEY1_PIN);
    uint8_t key2_current = KEY_Stat(KEY_PORT, KEY2_PIN);

    // KEY1 ��������
    if (key1_current == 0)   // ����������
    {
        key1_press_count++;
        if (key1_press_count >= 3 && key1_last_state == 1)   // ����3�μ�⵽���£����ϴ����ͷ�״̬
        {
            printf("KEY1 Pressed\r\n");
            OLED_Clear();
            OLED_ShowString(0, 0, (uint8_t*)"KEY1 Pressed", 16);
            key1_last_state = 0;   // ����״̬Ϊ����
        }
    }
    else   // �����ͷ�
    {
        key1_press_count = 0;
        key1_last_state  = 1;   // ����״̬Ϊ�ͷ�
    }

    // KEY2 ��������
    if (key2_current == 0)   // ����������
    {
        key2_press_count++;
        if (key2_press_count >= 3 && key2_last_state == 1)   // ����3�μ�⵽���£����ϴ����ͷ�״̬
        {
            printf("KEY2 Pressed\r\n");
            OLED_Clear();
            OLED_ShowString(0, 0, (uint8_t*)"KEY2 Pressed", 16);
            key2_last_state = 0;   // ����״̬Ϊ����
        }
    }
    else   // �����ͷ�
    {
        key2_press_count = 0;
        key2_last_state  = 1;   // ����״̬Ϊ�ͷ�
    }
}

/************************************************************
 * ����:       task_rtc_test(void)
 * ˵��:       RTC��������
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       GitHub Copilot
 * ����:       ������������
 ************************************************************/
void task_rtc_test(void)
{
    static uint8_t rtc_initialized = 0;
    static char    time_str[32];

    if (rtc_initialized == 0)
    {
        RTC_Init();   // ��ʼ��RTC��������Ҫ���û�����ʱ��
        printf("RTC��ʼ�����\r\n");
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"RTC��ʾʱ��", 16);
        rtc_initialized = 1;
    }

    // ֱ�ӻ�ȡ����ʾ��ǰʱ��
    rtc_show_time();

    // ��OLED����ʾʱ��
    rtc_parameter_struct rtc_time;
    rtc_current_time_get(&rtc_time);

    // ��ʽ��ʱ���ַ���
    sprintf(time_str, "20%02x-%02x-%02x", rtc_time.year, rtc_time.month, rtc_time.date);
    OLED_ShowString(0, 2, (uint8_t*)time_str, 16);

    sprintf(time_str, "%02x:%02x:%02x", rtc_time.hour, rtc_time.minute, rtc_time.second);
    OLED_ShowString(0, 4, (uint8_t*)time_str, 16);
}

/************************************************************
 * ����:       task_spi_flash_test(void)
 * ˵��:       SPI Flash�������� (һ����)
 * ����:       ��
 * ���:       ��
 * ����ֵ:     ��
 * ����:       GitHub Copilot
 * ����:       ������������, ִ��һ�κ��Զ�����
 ************************************************************/
void task_spi_flash_test(void)
{
    static uint8_t test_executed = 0;   // ��ǲ����Ƿ���ִ��

    if (test_executed == 1)
    {
        return;   // ����Ѿ�ִ�й���ֱ�ӷ���
    }

    test_executed = 1;   // �����ִ��

    uint32_t flash_id = 0;
    uint8_t  write_buffer[SPI_FLASH_PAGE_SIZE];
    uint8_t  read_buffer[SPI_FLASH_PAGE_SIZE];
    uint16_t i;

    printf("=== SPI Flash Test (One Time Only) ===\r\n");
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"SPI Flash Test", 16);

    flash_id = spi_flash_read_id();
    printf("Flash ID: 0x%X\r\n", flash_id);
    char id_str[20];
    sprintf(id_str, "Flash ID:0x%X", flash_id);
    OLED_ShowString(0, 2, (uint8_t*)id_str, 16);

    if (flash_id == SFLASH_ID)   // SFLASH_ID �� Implement.h �ж���
    {
        printf("SPI Flash ID OK.\r\n");
        OLED_ShowString(0, 4, (uint8_t*)"ID OK", 16);

        // ׼��д������
        for (i = 0; i < SPI_FLASH_PAGE_SIZE; i++)
        {
            write_buffer[i] = i % 256;
        }

        printf("Erasing sector...\r\n");
        OLED_ShowString(0, 6, (uint8_t*)"Erasing...", 16);
        spi_flash_sector_erase(FLASH_WRITE_ADDRESS);   // FLASH_WRITE_ADDRESS �� Implement.h �ж���
        spi_flash_wait_for_write_end();
        printf("Erase complete.\r\n");

        printf("Writing data to 0x%08X...\r\n", FLASH_WRITE_ADDRESS);
        OLED_ShowString(0, 6, (uint8_t*)"Writing...", 16);
        spi_flash_buffer_write(write_buffer, FLASH_WRITE_ADDRESS, SPI_FLASH_PAGE_SIZE);
        spi_flash_wait_for_write_end();
        printf("Write complete.\r\n");

        printf("Reading data from 0x%08X...\r\n", FLASH_READ_ADDRESS);   // FLASH_READ_ADDRESS �� Implement.h �ж���
        OLED_ShowString(0, 6, (uint8_t*)"Reading...", 16);
        spi_flash_buffer_read(read_buffer, FLASH_READ_ADDRESS, SPI_FLASH_PAGE_SIZE);
        printf("Read complete.\r\n");

        // У������
        if (memory_compare(write_buffer, read_buffer, SPI_FLASH_PAGE_SIZE) == SUCCESS)
        {
            printf("SPI Flash Test Passed! Data verified.\r\n");
            OLED_ShowString(0, 6, (uint8_t*)"Test Passed!", 16);
        }
        else
        {
            printf("SPI Flash Test Failed! Data mismatch.\r\n");
            OLED_ShowString(0, 6, (uint8_t*)"Test Failed!", 16);
        }
    }
    else
    {
        printf("SPI Flash ID Error. Expected 0x%X, Got 0x%X\r\n", SFLASH_ID, flash_id);
        OLED_ShowString(0, 4, (uint8_t*)"ID Error", 16);
    }

    printf("SPI Flash test completed. Disabling task...\r\n");
    // ʹ��ȫ�־����������
    if (g_spi_flash_test_handle != INVALID_TASK_HANDLE)
    {
        scheduler_disable_task(g_spi_flash_test_handle);
        printf("SPI Flash task disabled successfully.\r\n");
    }
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
    printf("=== ��LED��������ʾ ===\n");

    // 1. ����ĸ���ͬƵ�ʵ�LED����ʹ��ȫ�־����
    g_led1_handle = scheduler_add_task(led1_task, 500);   // LED1ÿ500ms��˸
    // g_led2_handle = scheduler_add_task(led2_task, 1000);   // LED2ÿ800ms��˸ (ע�͵��Լ�����������)
    // g_led3_handle = scheduler_add_task(led3_task, 2000);   // LED3ÿ1200ms��˸ (ע�͵��Լ�����������)
    // g_led4_handle = scheduler_add_task(led4_task, 4000);   // LED4ÿ2000ms��˸ (ע�͵��Լ�����������)

    // �����ʾ���º���������
    g_display_task_handle   = scheduler_add_task(task_display_update, 1000);   // ��ʾÿ1�����
    g_heartbeat_task_handle = scheduler_add_task(task_heartbeat, 10000);       // ����ÿ10��

    // ����µĲ�������
    g_oled_test_handle      = scheduler_add_task(task_oled_test, 2000);        // OLEDÿ2����²�������
    g_key_scan_handle       = scheduler_add_task(task_key_scan, 50);           // ����ÿ50msɨ��һ�Σ�����Ƶ�ʣ�
    g_rtc_test_handle       = scheduler_add_task(task_rtc_test, 1000);         // RTCÿ1�����һ��ʱ����ʾ
    g_spi_flash_test_handle = scheduler_add_task(task_spi_flash_test, 5000);   // SPI Flash���ԣ�5���ִ��һ��

    // 2. ��������LED����
    if (g_led1_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_led1_handle);
        printf("LED1���������� - 500ms����\n");
    }
    // if (g_led2_handle != INVALID_TASK_HANDLE)
    // {
    //     scheduler_enable_task(g_led2_handle);
    //     printf("LED2���������� - 800ms����\n");
    // }
    // if (g_led3_handle != INVALID_TASK_HANDLE)
    // {
    //     scheduler_enable_task(g_led3_handle);
    //     printf("LED3���������� - 1200ms����\n");
    // }
    // if (g_led4_handle != INVALID_TASK_HANDLE)
    // {
    //     scheduler_enable_task(g_led4_handle);
    //     printf("LED4���������� - 2000ms����\n");
    // }

    // ������������
    if (g_display_task_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_display_task_handle);   // ������ʾ����
    }
    if (g_heartbeat_task_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_heartbeat_task_handle);   // ������������
    }
    if (g_oled_test_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_oled_test_handle);
        printf("OLED�������������� - 2000ms����\n");
    }
    if (g_key_scan_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_key_scan_handle);
        printf("����ɨ������������ - 50ms����\n");
    }
    if (g_rtc_test_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_rtc_test_handle);
        printf("RTC�������������� - 1000ms����\n");
    }
    if (g_spi_flash_test_handle != INVALID_TASK_HANDLE)
    {
        scheduler_enable_task(g_spi_flash_test_handle);
        printf("SPI Flash�������������� - 5000ms��ִ��һ��\n");
    }

    printf("���������: %d | LED��Ƶ�ʵ�����������\n", scheduler_get_task_count());
    printf("�۲�LED1-LED4�Բ�ͬƵ����˸...\n");
    printf("OLED����ʾ������Ϣ����������ʱ������ʾ��RTCʱ�佫��ʾ��SPI Flash�����ж�д���ԡ�\n");
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

/************************************************************
 * ����:       memory_compare
 * ˵��:       �Ƚ������ڴ������Ƿ���ͬ
 * ����:       src: Դ�ڴ�ָ��
 *             dst: Ŀ���ڴ�ָ��
 *             length: �Ƚϵ��ֽ���
 * ����ֵ:     SUCCESS(0) - ��ͬ��ERROR(1) - ��ͬ
 * ����:       Lingyu Meng
 ************************************************************/
ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint16_t length)
{
    uint16_t i;

    for (i = 0; i < length; i++)
    {
        if (src[i] != dst[i])
        {
            return ERROR;   // ������κ�һ���ֽڲ�ͬ������ERROR(1)
        }
    }

    return SUCCESS;   // �����ֽڶ���ͬ������SUCCESS(0)
}
/****************************End*****************************/