/*
 *	Project:	LPC1114_RS485Slave_Servo
 *	File:		soft_pwm.c
 *	Author:		Terukazu Sato
 */


#include "soft_pwm.h"

#define DEADTIME_COUNT	2

#define FAST_IO_H(port_ptr, pin)	port_ptr->MASKED_ACCESS[pin] = pin
#define FAST_IO_L(port_ptr, pin)	port_ptr->MASKED_ACCESS[pin] = 0

/* Variables ------------------------------------------------------------ */
typedef enum {
	free = 		0x00,
	forward = 	0x01,
	backward = 	0x02,
	brake = 	0x04,
	deadtime = 	0x08,
} PWM_STATE;
typedef struct {
	int duty;
	int pulseCounter;
	PWM_STATE dir;
} MotorPWM;

static MotorPWM Motor[4];
static int resolution = 100;
static uint8_t stateMemory[4];
static struct {
	uint8_t count;
	uint8_t backup_dir;
	uint8_t backup_duty;
} dt[4];

static const struct {
	struct {
		GPIO_TypeDef *port;
		int pin;
	} A,B;
} PwmSignalPin[4] = {
	{{ MOTOR0_A }, { MOTOR0_B }},
	{{ MOTOR1_A }, { MOTOR1_B }},
	{{ MOTOR2_A }, { MOTOR2_B }},
	{{ MOTOR3_A }, { MOTOR3_B }}
};


static int Servo[4], ServoPulse[4], ServoCh, cycle = SERVO_CYCLE;
static const struct {
	GPIO_TypeDef *port;
	int pin;
} ServoPin[4] = { { SERVO0 }, { SERVO1 }, { SERVO2 }, { SERVO3 } };

/* Functions ------------------------------------------------------------ */
void SoftPWM_Init()
{
	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_CT16B0, ENABLE);

	LPC_TMR16B0->PR = SystemCoreClock / 1000000 - 1;// 1us
	LPC_TMR16B0->MR0 = 50;							// 50us = 20kHz  => PWM 200Hz
	LPC_TMR16B0->MCR = TIM_INT_ON_MATCH(0)|TIM_RESET_ON_MATCH(0);
	LPC_TMR16B0->TCR = TIM_ENABLE;
	NVIC_EnableIRQ(TIMER_16_0_IRQn);
}
void Servo_Init()
{
	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_CT32B0, ENABLE);

	LPC_TMR32B0->PR = SystemCoreClock / 1000000 - 1;// 1us
	LPC_TMR32B0->MR0 = 10;							// 10us
	LPC_TMR32B0->MCR = TIM_INT_ON_MATCH(0)|TIM_RESET_ON_MATCH(0);
	LPC_TMR32B0->TCR = TIM_ENABLE;
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
}

void EnterDeadtime(int ch)
{
	Motor[ch].dir = free;
	Motor[ch].duty = 0;
	dt[ch].count = DEADTIME_COUNT;
	FAST_IO_L(PwmSignalPin[ch].A.port, PwmSignalPin[ch].A.pin);
	FAST_IO_L(PwmSignalPin[ch].B.port, PwmSignalPin[ch].B.pin);
}

void SoftPWM_Drive(int ch, int duty)
{
	if (stateMemory[ch] == deadtime) return;

	if (duty == BRAKEVALUE) {
		if (stateMemory[ch] & (forward|backward)) {
			stateMemory[ch] = deadtime;
			dt[ch].backup_dir = brake;
			dt[ch].backup_duty = 100;
			EnterDeadtime(ch);
			return;
		}
		Motor[ch].duty = 100;
		Motor[ch].dir = brake;
		stateMemory[ch] = brake;
	}
	else if (duty > 0) {
		if (stateMemory[ch] & (backward|brake)) {
			stateMemory[ch] = deadtime;
			dt[ch].backup_dir = forward;
			dt[ch].backup_duty = duty;
			EnterDeadtime(ch);
			return;
		}
		Motor[ch].duty = duty;
		Motor[ch].dir = forward;
		stateMemory[ch] = forward;
	}
	else if (duty < 0) {
		if (stateMemory[ch] & (forward|brake)) {
			stateMemory[ch] = deadtime;
			dt[ch].backup_dir = backward;
			dt[ch].backup_duty = -duty;
			EnterDeadtime(ch);
			return;
		}
		Motor[ch].duty = -duty;
		Motor[ch].dir = backward;
		stateMemory[ch] = backward;
	}
	else {
		Motor[ch].duty = 0;
		Motor[ch].dir = free;
		stateMemory[ch] = free;
	}
}

void TIMER16_0_IRQHandler(void)
{
	LPC_TMR16B0->IR = TIM_IR_CLR(TIM_MR0_INT);
	int i;
	if (resolution--) {
		for (i=0; i<MOTOR_USE; i++) {
			if (Motor[i].pulseCounter-- == 0) {
				FAST_IO_L(PwmSignalPin[i].A.port, PwmSignalPin[i].A.pin);
				FAST_IO_L(PwmSignalPin[i].B.port, PwmSignalPin[i].B.pin);
			}
		}
	}
	else {
		resolution = 100;
		for (i=0; i<MOTOR_USE; i++) {
			if (dt[i].count) {
				/* In deadtime */
				dt[i].count--;
				if (dt[i].count == 0) {
					/* Finished deadtime */
					stateMemory[i] = dt[i].backup_dir;
					Motor[i].dir = dt[i].backup_dir;
					Motor[i].duty = dt[i].backup_duty;
				}
			}
			else {
				Motor[i].pulseCounter = Motor[i].duty;
				switch (Motor[i].dir) {
				case free:
					FAST_IO_L(PwmSignalPin[i].A.port, PwmSignalPin[i].A.pin);
					FAST_IO_L(PwmSignalPin[i].B.port, PwmSignalPin[i].B.pin);
					break;
				case forward:
					FAST_IO_H(PwmSignalPin[i].A.port, PwmSignalPin[i].A.pin);
					FAST_IO_L(PwmSignalPin[i].B.port, PwmSignalPin[i].B.pin);
					break;
				case backward:
					FAST_IO_L(PwmSignalPin[i].A.port, PwmSignalPin[i].A.pin);
					FAST_IO_H(PwmSignalPin[i].B.port, PwmSignalPin[i].B.pin);
					break;
				case brake:
					FAST_IO_H(PwmSignalPin[i].A.port, PwmSignalPin[i].A.pin);
					FAST_IO_H(PwmSignalPin[i].B.port, PwmSignalPin[i].B.pin);
					break;
				default:
					break;
			}
			}
		}
	}
}

void Servo_Drive(int ch, int pulsewidth)
{
	Servo[ch] = pulsewidth;
}

void TIMER32_0_IRQHandler(void)
{
	LPC_TMR32B0->IR = TIM_IR_CLR(TIM_MR0_INT);

	cycle--;
	if ((ServoCh == SERVO_USE) && (cycle != 0)) {
		return;
	} else if (cycle == 0) {
		ServoCh = 0;
		cycle = SERVO_CYCLE;
	}
	if (ServoPulse[ServoCh]--) {
		FAST_IO_H(ServoPin[ServoCh].port, ServoPin[ServoCh].pin);
	}
	else {
		FAST_IO_L(ServoPin[ServoCh].port, ServoPin[ServoCh].pin);
		ServoPulse[ServoCh] = Servo[ServoCh];
		ServoCh++;
	}
}
