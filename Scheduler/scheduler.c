#include "scheduler.h"

// ��̬��������ͼ�����
static task_t  tasks[MAX_TASKS];
static uint8_t task_count = 0;

/**
 * @brief ��ʼ��������
 */
void scheduler_init(void)
{
    task_count = 0;   // �����������
    for (uint8_t i = 0; i < MAX_TASKS; i++)
    {
        tasks[i].func      = NULL;
        tasks[i].interval  = 0;
        tasks[i].countdown = 0;
        tasks[i].state     = TASK_DISABLED;
        tasks[i].ready     = 0;   // ��ʼ��������־
    }
}

/**
 * @brief ������񵽵�����
 * @param task_func: ������ָ��
 * @param interval_ms: ִ�м��(����)
 * @return ��������ʧ�ܷ���INVALID_TASK_HANDLE
 */
task_handle_t scheduler_add_task(void (*task_func)(void), uint32_t interval_ms)
{
    // �������
    if (task_func == NULL || interval_ms == 0 || task_count >= MAX_TASKS)
    {
        return INVALID_TASK_HANDLE;
    }   // �������
    tasks[task_count].func      = task_func;
    tasks[task_count].interval  = interval_ms;
    tasks[task_count].countdown = interval_ms;
    tasks[task_count].state     = TASK_DISABLED;   // Ĭ�Ͻ��ã����ֶ�����
    tasks[task_count].ready     = 0;               // ��ʼ��������־

    return task_count++;
}

/**
 * @brief ��������
 * @param handle: ������
 */
void scheduler_enable_task(task_handle_t handle)
{
    if (handle < task_count)
    {
        tasks[handle].state     = TASK_ENABLED;
        tasks[handle].countdown = tasks[handle].interval;   // ���õ���ʱ
    }
}

/**
 * @brief ��������
 * @param handle: ������
 */
void scheduler_disable_task(task_handle_t handle)
{
    if (handle < task_count)
    {
        tasks[handle].state = TASK_DISABLED;
    }
}

/**
 * @brief ���������� - ��1ms��ʱ���ж��е���
 * @note �ж���ֻ����ʱ�ӣ���ִ������ȷ���жϿ���ִ��
 */
void scheduler_tick(void)
{
    for (uint8_t i = 0; i < task_count; i++)
    {
        if (tasks[i].state == TASK_ENABLED)
        {
            if (tasks[i].countdown > 0)
            {
                tasks[i].countdown--;
            }

            // ʱ�䵽�ˣ����þ�����־���������ж���ִ������
            if (tasks[i].countdown == 0)
            {
                tasks[i].ready     = 1;                   // ����������
                tasks[i].countdown = tasks[i].interval;   // ���õ���ʱ
            }
        }
    }
}

/**
 * @brief ���������� - ����ѭ���е���
 * @note ��鲢ִ�����о��������񣬰�ȫ�ɿ�
 */
void scheduler_run(void)
{
    for (uint8_t i = 0; i < task_count; i++)
    {
        if (tasks[i].ready && tasks[i].func != NULL)
        {
            tasks[i].ready = 0;   // ���������־
            tasks[i].func();      // ����ѭ���а�ȫִ������
        }
    }
}

/**
 * @brief ��ȡ��ǰ��������
 * @return ��������
 */
uint8_t scheduler_get_task_count(void)
{
    return task_count;
}
