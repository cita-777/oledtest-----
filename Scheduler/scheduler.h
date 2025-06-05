#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "gd32f4xx.h"
#include <stddef.h>
#include <stdint.h>

#define MAX_TASKS 8   // 最大任务数量

// 任务状态
typedef enum
{
    TASK_DISABLED = 0,
    TASK_ENABLED  = 1
} task_state_t;

// 简化的任务结构
typedef struct
{
    void (*func)(void);       // 任务函数
    uint32_t     interval;    // 执行间隔(ms)
    uint32_t     countdown;   // 倒计时(ms)
    task_state_t state;       // 任务状态
    uint8_t      ready;       // 任务就绪标志：1=需要执行，0=不需要执行
} task_t;

// 任务句柄类型
typedef uint8_t task_handle_t;
#define INVALID_TASK_HANDLE 0xFF

// 调度器接口 - 简洁明了
void          scheduler_init(void);
task_handle_t scheduler_add_task(void (*task_func)(void), uint32_t interval_ms);
void          scheduler_enable_task(task_handle_t handle);
void          scheduler_disable_task(task_handle_t handle);
void          scheduler_tick(void);   // 1ms中断调用 - 仅更新时钟
void          scheduler_run(void);    // 主循环调用 - 执行就绪任务
uint8_t       scheduler_get_task_count(void);

#endif   // __SCHEDULER_H
