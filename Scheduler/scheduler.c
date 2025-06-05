#include "scheduler.h"

// 静态任务数组和计数器
static task_t  tasks[MAX_TASKS];
static uint8_t task_count = 0;

/**
 * @brief 初始化调度器
 */
void scheduler_init(void)
{
    task_count = 0;   // 清空所有任务
    for (uint8_t i = 0; i < MAX_TASKS; i++)
    {
        tasks[i].func      = NULL;
        tasks[i].interval  = 0;
        tasks[i].countdown = 0;
        tasks[i].state     = TASK_DISABLED;
        tasks[i].ready     = 0;   // 初始化就绪标志
    }
}

/**
 * @brief 添加任务到调度器
 * @param task_func: 任务函数指针
 * @param interval_ms: 执行间隔(毫秒)
 * @return 任务句柄，失败返回INVALID_TASK_HANDLE
 */
task_handle_t scheduler_add_task(void (*task_func)(void), uint32_t interval_ms)
{
    // 参数检查
    if (task_func == NULL || interval_ms == 0 || task_count >= MAX_TASKS)
    {
        return INVALID_TASK_HANDLE;
    }   // 添加任务
    tasks[task_count].func      = task_func;
    tasks[task_count].interval  = interval_ms;
    tasks[task_count].countdown = interval_ms;
    tasks[task_count].state     = TASK_DISABLED;   // 默认禁用，需手动启用
    tasks[task_count].ready     = 0;               // 初始化就绪标志

    return task_count++;
}

/**
 * @brief 启用任务
 * @param handle: 任务句柄
 */
void scheduler_enable_task(task_handle_t handle)
{
    if (handle < task_count)
    {
        tasks[handle].state     = TASK_ENABLED;
        tasks[handle].countdown = tasks[handle].interval;   // 重置倒计时
    }
}

/**
 * @brief 禁用任务
 * @param handle: 任务句柄
 */
void scheduler_disable_task(task_handle_t handle)
{
    if (handle < task_count)
    {
        tasks[handle].state = TASK_DISABLED;
    }
}

/**
 * @brief 调度器心跳 - 在1ms定时器中断中调用
 * @note 中断中只更新时钟，不执行任务！确保中断快速执行
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

            // 时间到了，设置就绪标志，但不在中断中执行任务
            if (tasks[i].countdown == 0)
            {
                tasks[i].ready     = 1;                   // 标记任务就绪
                tasks[i].countdown = tasks[i].interval;   // 重置倒计时
            }
        }
    }
}

/**
 * @brief 调度器运行 - 在主循环中调用
 * @note 检查并执行所有就绪的任务，安全可靠
 */
void scheduler_run(void)
{
    for (uint8_t i = 0; i < task_count; i++)
    {
        if (tasks[i].ready && tasks[i].func != NULL)
        {
            tasks[i].ready = 0;   // 清除就绪标志
            tasks[i].func();      // 在主循环中安全执行任务
        }
    }
}

/**
 * @brief 获取当前任务数量
 * @return 任务数量
 */
uint8_t scheduler_get_task_count(void)
{
    return task_count;
}
