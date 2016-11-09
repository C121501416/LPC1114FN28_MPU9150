/*
 * ファイル名：		soft_pwm.h
 * 内容：			モーター4つ分とサーボモータ4つ分のPWM信号を生成する．
 * 作成日時：		2014/7/10
 * バージョン：		1.1.0
 * 作成者：		佐藤輝一
 * 使用環境：		LPC1114, CooCox CoIDE
 */

#ifndef __SOFT_PWM_H__
#define __SOFT_PWM_H__

#include <lpc11xx.h>
#include "lpc11xx_gpio.h"
#include "lpc11xx_tmr.h"
#include "lpc11xx_syscon.h"

#define BRAKEVALUE		32099

/* ここで出力ピンを設定する．
 * モータひとつにつき，AとBの出力がある．
 *  正転		A=1,B=0
 *  逆転		A=0,B=1
 *  ブレーキ	A=1,B=1
 *  フリー		A=0,B=0
 * となる．
 */
#define MOTOR_USE		4
#define MOTOR0_A		LPC_GPIO1, GPIO_Pin_3
#define MOTOR0_B		LPC_GPIO0, GPIO_Pin_8
#define MOTOR1_A		LPC_GPIO1, GPIO_Pin_2
#define MOTOR1_B		LPC_GPIO0, GPIO_Pin_9
#define MOTOR2_A		LPC_GPIO1, GPIO_Pin_1
#define MOTOR2_B		LPC_GPIO0, GPIO_Pin_10
#define MOTOR3_A		LPC_GPIO1, GPIO_Pin_0
#define MOTOR3_B		LPC_GPIO0, GPIO_Pin_11

#define SERVO_USE		4
#define SERVO0			LPC_GPIO0, GPIO_Pin_7
#define SERVO1			LPC_GPIO0, GPIO_Pin_3
#define SERVO2			LPC_GPIO1, GPIO_Pin_9
#define SERVO3			LPC_GPIO1, GPIO_Pin_8

#define SERVO_CYCLE		100 * 20		// 20ms


/* 概要：		モータ用PWMの初期化を行う．
 * 引数：		なし
 * 戻り値：	なし
 */
void SoftPWM_Init(void);

/* 概要：		モータPWM信号を出力する．
 * 引数(1)：	モータ番号(0~3)
 * 引数(2):	デューティ比(-100~100, BRAKEVALUE)
 * 戻り値：	なし
 */
void SoftPWM_Drive(int ch, int duty);


/* 概要：		サーボモータ用PWMの初期化を行う．
 * 引数：		なし
 * 戻り値：	なし
 */
void Servo_Init(void);

/* 概要：		サーボモータを指定角度に駆動する．
 * 引数(1)：	サーボモータ番号(0~3)
 * 引数(2):	サーボモータに与えるパルスの長さ(10マイクロ秒単位)
 * 			一般的に150(1.5ms)でサーボホーンが中央になり、75(0.75ms)から225(2.25ms)で±90度回る
 * 戻り値：	なし
 */
void Servo_Drive(int ch, int pulsewidth);


#endif /* __SOFT_PWM_H__ */
