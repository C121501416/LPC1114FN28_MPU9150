/*
 * �t�@�C�����F		soft_pwm.h
 * ���e�F			���[�^�[4���ƃT�[�{���[�^4����PWM�M���𐶐�����D
 * �쐬�����F		2014/7/10
 * �o�[�W�����F		1.1.0
 * �쐬�ҁF		�����P��
 * �g�p���F		LPC1114, CooCox CoIDE
 */

#ifndef __SOFT_PWM_H__
#define __SOFT_PWM_H__

#include <lpc11xx.h>
#include "lpc11xx_gpio.h"
#include "lpc11xx_tmr.h"
#include "lpc11xx_syscon.h"

#define BRAKEVALUE		32099

/* �����ŏo�̓s����ݒ肷��D
 * ���[�^�ЂƂɂ��CA��B�̏o�͂�����D
 *  ���]		A=1,B=0
 *  �t�]		A=0,B=1
 *  �u���[�L	A=1,B=1
 *  �t���[		A=0,B=0
 * �ƂȂ�D
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


/* �T�v�F		���[�^�pPWM�̏��������s���D
 * �����F		�Ȃ�
 * �߂�l�F	�Ȃ�
 */
void SoftPWM_Init(void);

/* �T�v�F		���[�^PWM�M�����o�͂���D
 * ����(1)�F	���[�^�ԍ�(0~3)
 * ����(2):	�f���[�e�B��(-100~100, BRAKEVALUE)
 * �߂�l�F	�Ȃ�
 */
void SoftPWM_Drive(int ch, int duty);


/* �T�v�F		�T�[�{���[�^�pPWM�̏��������s���D
 * �����F		�Ȃ�
 * �߂�l�F	�Ȃ�
 */
void Servo_Init(void);

/* �T�v�F		�T�[�{���[�^���w��p�x�ɋ쓮����D
 * ����(1)�F	�T�[�{���[�^�ԍ�(0~3)
 * ����(2):	�T�[�{���[�^�ɗ^����p���X�̒���(10�}�C�N���b�P��)
 * 			��ʓI��150(1.5ms)�ŃT�[�{�z�[���������ɂȂ�A75(0.75ms)����225(2.25ms)�Ł}90�x���
 * �߂�l�F	�Ȃ�
 */
void Servo_Drive(int ch, int pulsewidth);


#endif /* __SOFT_PWM_H__ */
