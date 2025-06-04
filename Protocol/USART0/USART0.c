/************************************************************
 * ��Ȩ��2025CIMC Copyright�� 
 * �ļ���usart0.c
 * ����: Lingyu Meng
 * ƽ̨: 2025CIMC IHD-V04
 * �汾: Lingyu Meng     2025/3/10     V0.01    original
************************************************************/


/************************* ͷ�ļ� *************************/

#include "USART0.h"

/************************* �궨�� *************************/

/************************ �������� ************************/

uint8_t data_recv = 0;  // ����
/************************ �������� ************************/

int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}

/************************************************************ 
 * Function :       USART0_Config
 * Comment  :       ���ڳ�ʼ��usart0 �������
 * Parameter:       null
 * Return   :       null
 * Author   :       Lingyu Meng
 * Date     :       2025-03-10 V0.1 original
************************************************************/
void USART0_Config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);    // ʹ��GPIOʱ��
    rcu_periph_clock_enable(RCU_USART0);   // ʹ�ܴ���ʱ��
	
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9 | GPIO_PIN_10);				//���ö˿ڸ���
	
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);  		//�˿���������Ϊ����
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);  

	gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_10);  	//�˿���������Ϊ����	
	gpio_output_options_set(GPIOA, GPIO_MODE_ANALOG, GPIO_OSPEED_50MHZ, GPIO_PIN_10);  


    usart_deinit(USART0);    						// ���ڸ�λ
    usart_word_length_set(USART0, USART_WL_8BIT);   // �ֳ�Ϊ8λ
    usart_stop_bit_set(USART0, USART_STB_1BIT);     // ֹͣλ1λ
    usart_parity_config(USART0, USART_PM_NONE);		// ��У��
    usart_baudrate_set(USART0, 115200U);     		// ������115200
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);     // ����ʹ��
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);   // ����ʹ��
	usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_enable(USART0);          							// ����ʹ��
}


/************************************************************ 
 * Function :       USART0_SendData
 * Comment  :       ��usart0��������
 * Parameter:       ����ָ�룬���ݳ���
 * Return   :       null
 * Author   :       Lingyu Meng
 * Date     :       2025-03-14 V0.2 original
************************************************************/
void USART0_SendData(uint16_t *buf,uint16_t len)
 {
     uint16_t t;
     for(t=0;t<len;t++)      
     {           
         while(usart_flag_get(USART0, USART_FLAG_TC) == RESET);  
         usart_data_transmit(USART0,buf[t]);
     }     
     while(usart_flag_get(USART0, USART_FLAG_TC) == RESET);          
}

/************************************************************ 
 * Function :       USART0_IRQHandler
 * Comment  :       �����жϷ��������������մ�������
 * Parameter:       null
 * Return   :       null
 * Author   :       Lingyu Meng
 * Date     :       2025-03-14 V0.2 original
************************************************************/
void USART0_IRQHandler(void)
{
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
	{			
		data_recv = usart_data_receive(USART0);    //  ���մ�������
		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE); 	//��������жϱ�־λ
        process_data(data_recv); 		//��������
				
    }    
}

/************************************************************ 
 * Function :       process_data
 * Comment  :       ����������ֱ�Ӵ��ڴ�ӡ����
 * Parameter:       char�͵����ݣ�ֻ֧�ֵ��ַ�
 * Return   :       null
 * Author   :       Lingyu Meng
 * Date     :       2025-03-14 V0.2 original
************************************************************/
void process_data(uint8_t data)
{
	
		usart_data_transmit(USART0, data_recv);    //  �������� 
	

}