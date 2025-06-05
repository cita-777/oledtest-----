/************************************************************
 * ��Ȩ��2025CIMC Copyright��
 * �ļ���key.c
 * ����: Lingyu Meng
 * ƽ̨: 2025CIMC IHD-V04
 * �汾: Lingyu Meng     2025/2/16     V0.01    original
 ************************************************************/

/************************* ͷ�ļ� *************************/
#include "KEY.h"

/************************ ȫ�ֱ������� ************************/
// ����״̬�ṹ�壬���ڷ�������
typedef struct
{
    uint8_t last_state;     // �ϴΰ���״̬��1��ʾδ����
    uint8_t press_count;    // �������¼����������ڷ���
    uint8_t pressed_flag;   // �������±�־��1��ʾ���µİ��������¼�
} key_state_t;

static key_state_t key1_state = {1, 0, 0};
static key_state_t key2_state = {1, 0, 0};
static key_state_t key3_state = {1, 0, 0};
static key_state_t key4_state = {1, 0, 0};

/************************************************************
 * Function :       KEY_Init
 * Comment  :       ���ڳ�ʼ��KEY�˿�
 * Parameter:       null
 * Return   :       null
 * Author   :       Lingyu Meng
 * Date     :       2025-02-30 V0.1 original
 ************************************************************/
void KEY_Init(void)
{
    rcu_periph_clock_enable(KEY_CLK);   // ��ʼ��KEY����ʱ��

    gpio_mode_set(KEY_PORT,
                  GPIO_MODE_INPUT,
                  GPIO_PUPD_PULLUP,
                  KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN);   // ����GPIOģʽΪ����
}

/************************************************************
 * Function :       KEY_Stat
 * Comment  :       ���ڶ�ȡ����״̬ - ԭʼ�汾��������
 * Parameter:       �����˿ں�����
 * Return   :       ����״̬��1Ϊ�������£�0Ϊ����δ����
 * Author   :       Lingyu Meng
 * Date     :       2025-02-30 V0.1 original
 ************************************************************/
uint8_t KEY_Stat(uint32_t port, uint16_t pin)
{
    if (gpio_input_bit_get(port, pin) == RESET)   // ��ȡGPIO״̬�����������������
    {
        delay_1ms(20);                                // ��ʱ����
        if (gpio_input_bit_get(port, pin) == RESET)   // �ٶ�һ��GPIO��������ı���������
        {
            // while(gpio_input_bit_get(port, pin) == RESET);		//�ȴ������ͷ�
            return 1;   // ���ذ���״̬1
        }
    }
    return 0;
}

/************************************************************
 * Function :       key_debounce_process
 * Comment  :       �������������ڲ�����
 * Parameter:       key_state: ����״̬�ṹ��ָ��
 *                  current: ��ǰ��������״̬ (0=����, 1=�ͷ�)
 * Return   :       1: ���µİ��������¼�, 0: ���¼�
 * Author   :       GitHub Copilot
 ************************************************************/
static uint8_t key_debounce_process(key_state_t* key_state, uint8_t current)
{
    if (current == 0)   // ����������
    {
        key_state->press_count++;
        if (key_state->press_count >= 3 && key_state->last_state == 1)   // ����3�μ�⵽���£����ϴ����ͷ�״̬
        {
            key_state->last_state   = 0;   // ����״̬Ϊ����
            key_state->pressed_flag = 1;   // ���ð��±�־
            return 1;                      // ���ذ��������¼�
        }
    }
    else   // �����ͷ�
    {
        key_state->press_count = 0;
        key_state->last_state  = 1;   // ����״̬Ϊ�ͷ�
    }
    return 0;   // �ް����¼�
}

/************************************************************
 * Function :       KEY_Scan
 * Comment  :       ����ɨ�躯�� - ����������
 * Parameter:       ��
 * Return   :       ��
 * Author   :       GitHub Copilot
 * Date     :       2025-06-05 V0.2 ��ӷ�������
 ************************************************************/
void KEY_Scan(void)
{
    // ��ȡ���а����ĵ�ǰ״̬
    uint8_t key1_current = (gpio_input_bit_get(KEY_PORT, KEY1_PIN) == RESET) ? 0 : 1;
    uint8_t key2_current = (gpio_input_bit_get(KEY_PORT, KEY2_PIN) == RESET) ? 0 : 1;
    uint8_t key3_current = (gpio_input_bit_get(KEY_PORT, KEY3_PIN) == RESET) ? 0 : 1;
    uint8_t key4_current = (gpio_input_bit_get(KEY_PORT, KEY4_PIN) == RESET) ? 0 : 1;

    // ����������
    key_debounce_process(&key1_state, key1_current);
    key_debounce_process(&key2_state, key2_current);
    key_debounce_process(&key3_state, key3_current);
    key_debounce_process(&key4_state, key4_current);
}

/************************************************************
 * Function :       KEY_GetPressed
 * Comment  :       ��ȡ���������¼� - һ��������
 * Parameter:       key_num: ������ (1-4)
 * Return   :       1: �ð������µİ����¼�, 0: ���¼�
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
        key1_state.pressed_flag = 0;   // �����־
        break;
    case 2:
        result                  = key2_state.pressed_flag;
        key2_state.pressed_flag = 0;   // �����־
        break;
    case 3:
        result                  = key3_state.pressed_flag;
        key3_state.pressed_flag = 0;   // �����־
        break;
    case 4:
        result                  = key4_state.pressed_flag;
        key4_state.pressed_flag = 0;   // �����־
        break;
    default: result = 0; break;
    }

    return result;
}

/************************************************************
 * Function :       KEY_IsPressed
 * Comment  :       ��ѯ������ǰ�Ƿ��ڰ���״̬
 * Parameter:       key_num: ������ (1-4)
 * Return   :       1: ������ǰ������, 0: ������ǰδ����
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