/************************************************************
 * 版权：2025CIMC Copyright。
 * 文件：key.c
 * 作者: Lingyu Meng
 * 平台: 2025CIMC IHD-V04
 * 版本: Lingyu Meng     2025/2/16     V0.01    original
 ************************************************************/

/************************* 头文件 *************************/
#include "KEY.h"

/************************ 全局变量定义 ************************/
// 按键状态结构体，用于防抖处理
typedef struct
{
    uint8_t last_state;     // 上次按键状态，1表示未按下
    uint8_t press_count;    // 按键按下计数器，用于防抖
    uint8_t pressed_flag;   // 按键按下标志，1表示有新的按键按下事件
} key_state_t;

static key_state_t key1_state = {1, 0, 0};
static key_state_t key2_state = {1, 0, 0};
static key_state_t key3_state = {1, 0, 0};
static key_state_t key4_state = {1, 0, 0};

/************************************************************
 * Function :       KEY_Init
 * Comment  :       用于初始化KEY端口
 * Parameter:       null
 * Return   :       null
 * Author   :       Lingyu Meng
 * Date     :       2025-02-30 V0.1 original
 ************************************************************/
void KEY_Init(void)
{
    rcu_periph_clock_enable(KEY_CLK);   // 初始化KEY总线时钟

    gpio_mode_set(KEY_PORT,
                  GPIO_MODE_INPUT,
                  GPIO_PUPD_PULLUP,
                  KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN);   // 配置GPIO模式为输入
}

/************************************************************
 * Function :       KEY_Stat
 * Comment  :       用于读取按键状态 - 原始版本（保留）
 * Parameter:       按键端口和引脚
 * Return   :       按键状态：1为按键按下，0为按键未按下
 * Author   :       Lingyu Meng
 * Date     :       2025-02-30 V0.1 original
 ************************************************************/
uint8_t KEY_Stat(uint32_t port, uint16_t pin)
{
    if (gpio_input_bit_get(port, pin) == RESET)   // 读取GPIO状态，如果按键被按下了
    {
        delay_1ms(20);                                // 延时消抖
        if (gpio_input_bit_get(port, pin) == RESET)   // 再读一次GPIO，按键真的被按下了吗？
        {
            // while(gpio_input_bit_get(port, pin) == RESET);		//等待按键释放
            return 1;   // 返回按键状态1
        }
    }
    return 0;
}

/************************************************************
 * Function :       key_debounce_process
 * Comment  :       按键防抖处理内部函数
 * Parameter:       key_state: 按键状态结构体指针
 *                  current: 当前按键物理状态 (0=按下, 1=释放)
 * Return   :       1: 有新的按键按下事件, 0: 无事件
 * Author   :       GitHub Copilot
 ************************************************************/
static uint8_t key_debounce_process(key_state_t* key_state, uint8_t current)
{
    if (current == 0)   // 按键被按下
    {
        key_state->press_count++;
        if (key_state->press_count >= 3 && key_state->last_state == 1)   // 连续3次检测到按下，且上次是释放状态
        {
            key_state->last_state   = 0;   // 更新状态为按下
            key_state->pressed_flag = 1;   // 设置按下标志
            return 1;                      // 返回按键按下事件
        }
    }
    else   // 按键释放
    {
        key_state->press_count = 0;
        key_state->last_state  = 1;   // 更新状态为释放
    }
    return 0;   // 无按键事件
}

/************************************************************
 * Function :       KEY_Scan
 * Comment  :       按键扫描函数 - 带防抖处理
 * Parameter:       无
 * Return   :       无
 * Author   :       GitHub Copilot
 * Date     :       2025-06-05 V0.2 添加防抖处理
 ************************************************************/
void KEY_Scan(void)
{
    // 读取所有按键的当前状态
    uint8_t key1_current = (gpio_input_bit_get(KEY_PORT, KEY1_PIN) == RESET) ? 0 : 1;
    uint8_t key2_current = (gpio_input_bit_get(KEY_PORT, KEY2_PIN) == RESET) ? 0 : 1;
    uint8_t key3_current = (gpio_input_bit_get(KEY_PORT, KEY3_PIN) == RESET) ? 0 : 1;
    uint8_t key4_current = (gpio_input_bit_get(KEY_PORT, KEY4_PIN) == RESET) ? 0 : 1;

    // 处理按键防抖
    key_debounce_process(&key1_state, key1_current);
    key_debounce_process(&key2_state, key2_current);
    key_debounce_process(&key3_state, key3_current);
    key_debounce_process(&key4_state, key4_current);
}

/************************************************************
 * Function :       KEY_GetPressed
 * Comment  :       获取按键按下事件 - 一次性消费
 * Parameter:       key_num: 按键号 (1-4)
 * Return   :       1: 该按键有新的按下事件, 0: 无事件
 * Author   :       GitHub Copilot
 * Date     :       2025-06-05 V0.2
 ************************************************************/
uint8_t KEY_GetPressed(uint8_t key_num)
{
    uint8_t result = 0;

    switch (key_num)
    {
    case 1:
        result                  = key1_state.pressed_flag;
        key1_state.pressed_flag = 0;   // 清除标志
        break;
    case 2:
        result                  = key2_state.pressed_flag;
        key2_state.pressed_flag = 0;   // 清除标志
        break;
    case 3:
        result                  = key3_state.pressed_flag;
        key3_state.pressed_flag = 0;   // 清除标志
        break;
    case 4:
        result                  = key4_state.pressed_flag;
        key4_state.pressed_flag = 0;   // 清除标志
        break;
    default: result = 0; break;
    }

    return result;
}

/************************************************************
 * Function :       KEY_IsPressed
 * Comment  :       查询按键当前是否处于按下状态
 * Parameter:       key_num: 按键号 (1-4)
 * Return   :       1: 按键当前被按下, 0: 按键当前未按下
 * Author   :       GitHub Copilot
 * Date     :       2025-06-05 V0.2
 ************************************************************/
uint8_t KEY_IsPressed(uint8_t key_num)
{
    switch (key_num)
    {
    case 1: return (key1_state.last_state == 0) ? 1 : 0;
    case 2: return (key2_state.last_state == 0) ? 1 : 0;
    case 3: return (key3_state.last_state == 0) ? 1 : 0;
    case 4: return (key4_state.last_state == 0) ? 1 : 0;
    default: return 0;
    }
}
/****************************End*****************************/