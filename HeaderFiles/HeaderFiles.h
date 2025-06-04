/************************************************************
 * 版权：2025CIMC Copyright。
 * 文件：headerfiles.h
 * 作者: Lingyu Meng
 * 平台: 2025CIMC IHD-V04
 * 版本: Lingyu Meng     2023/2/16     V0.01    original
 ************************************************************/

#ifndef __HEADERFILES_H
#    define __HEADERFILES_H

/************************* 头文件 *************************/
#    include "gd32f4xx.h"
#    include "gd32f4xx_libopt.h"
#    include "oled.h"
#    include "string.h"
#    include "systick.h"
#    include <stdint.h>
#    include <stdio.h>
#    include "ADC.h"         // ADC
#    include "Implement.h"   // 执行函数
#    include "LED.h"         // LED
#    include "USART0.h"      // 串口
#    include "scheduler.h"



void test_flash_read_write(void);
#endif

/****************************End*****************************/
