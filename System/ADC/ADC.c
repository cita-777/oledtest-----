/************************************************************
 * ��Ȩ��2025CIMC Copyright�� 
 * �ļ���adc.c
 * ����: Lingyu Meng
 * ƽ̨: 2025CIMC IHD-V04
 * �汾: Lingyu Meng     2025/03/07      V0.01    original
************************************************************/

/************************* ͷ�ļ� *************************/

#include "ADC.h"

/************************* �궨�� *************************/

/************************ �������� ************************/

/************************ �������� ************************/

/************************************************************ 
 * Function :       ADC_Init
 * Comment  :       ���ڳ�ʼ��ADC����ʹ��dma��
 * Parameter:       null
 * Return   :       null
 * Author   :       Lingyu Meng
 * Date     :       2025-02-30 V0.1 original
************************************************************/

void ADC_port_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);   // GPIOCʱ��ʹ��
	rcu_periph_clock_enable(RCU_ADC0);    // ʹ��ADCʱ��
	
	gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0);   // ����PC0Ϊģ������
	
	adc_clock_config(ADC_ADCCK_PCLK2_DIV8);   // adcʱ������
	
	ADC_Init();  // ADC����
	
	adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL); //  ��������������
	

}

/************************************************************ 
 * Function :       ADC_Init
 * Comment  :       ���ڳ�ʼ��ADC����ʹ��dma��
 * Parameter:       null
 * Return   :       null
 * Author   :       Lingyu Meng
 * Date     :       2025-02-30 V0.1 original
************************************************************/

void ADC_Init(void)
{
    adc_deinit();    // ��λADC
	
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);    	// ʹ������ת��ģʽ
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);   			// �����Ҷ��� 
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 1);  			// ͨ�����ã�������

    adc_routine_channel_config(ADC0, 0, ADC_CHANNEL_10, ADC_SAMPLETIME_56);   // �Թ������������

    adc_external_trigger_source_config(ADC0, ADC_ROUTINE_CHANNEL, ADC_EXTTRIG_INSERTED_T0_CH3);   // ADC �������ã����ö�ʱ������
    adc_external_trigger_config(ADC0, ADC_ROUTINE_CHANNEL, ENABLE);   							  // ���ô���
	
    adc_enable(ADC0);   		// ʹ��ADC�ӿ�
	
    delay_1ms(1);  				// �ȴ�1ms

    adc_calibration_enable(ADC0);    // ADCУ׼�͸�λADCУ׼
}



/****************************End*****************************/

