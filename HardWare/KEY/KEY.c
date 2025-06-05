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


/************************************************************
 * Function :       KEY_Init
 * Comment  :       ���ڳ�ʼ��LED�˿�
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
 * Comment  :       ���ڶ�ȡ����״̬
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
/****************************End*****************************/
