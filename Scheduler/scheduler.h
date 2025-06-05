#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "gd32f4xx.h"
#include <stddef.h>
#include <stdint.h>

#define MAX_TASKS 8   // �����������

// ����״̬
typedef enum
{
    TASK_DISABLED = 0,
    TASK_ENABLED  = 1
} task_state_t;

// �򻯵�����ṹ
typedef struct
{
    void (*func)(void);       // ������
    uint32_t     interval;    // ִ�м��(ms)
    uint32_t     countdown;   // ����ʱ(ms)
    task_state_t state;       // ����״̬
    uint8_t      ready;       // ���������־��1=��Ҫִ�У�0=����Ҫִ��
} task_t;

// ����������
typedef uint8_t task_handle_t;
#define INVALID_TASK_HANDLE 0xFF

// �������ӿ� - �������
void          scheduler_init(void);
task_handle_t scheduler_add_task(void (*task_func)(void), uint32_t interval_ms);
void          scheduler_enable_task(task_handle_t handle);
void          scheduler_disable_task(task_handle_t handle);
void          scheduler_tick(void);   // 1ms�жϵ��� - ������ʱ��
void          scheduler_run(void);    // ��ѭ������ - ִ�о�������
uint8_t       scheduler_get_task_count(void);

#endif   // __SCHEDULER_H
