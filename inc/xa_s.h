/*
 * Project:		LPC1114_PwmConverter
 * File:		xa_s.h
 * Author:		Terukazu Sato
 * Version:		2.0.0
 *
 */
#ifndef __XA_S_H__
#define __XA_S_H__

#include "lpc11xx.h"
#include "uart.h"
#include "rs485.h"

#define XA_S_SYSVER		0x0200

#define RS485_TIMEOUT		1000		// =1ms
#define NO_RESPONSE			-1
typedef enum {
//
	Req_Response = 0x01,
	Req_DeviceID,
	Req_SystemVer,
	Req_Voltage,

	Cmd_RegisterWrite,
	Req_RegisterRead,

// Undefined device
	Cmd_SendData,
	Req_ReceiveData,

// MotorDriver
	Cmd_AllPWM,
	Cmd_PWM0,
	Cmd_PWM1,
	Cmd_PWM2,
	Cmd_PWM3,

// ADC
	Req_AllADC,
	Req_ADC0,
	Req_ADC1,
	Req_ADC2,
	Req_ADC3,

// UltrasonicDistanceSensor
	Req_AllUDS,
	Req_UDS0,
	Req_UDS1,
	Req_UDS2,
	Req_UDS3,
	Cmd_UDSSettings,

// IO Port
	Cmd_PortWrite,
	Cmd_PortWriteOR,
	Cmd_PortWriteAND,
	Cmd_PortWriteXOR,
	Req_PortRead,

// ServoMotor
	Cmd_AllServo,
	Cmd_Servo0,
	Cmd_Servo1,
	Cmd_Servo2,
	Cmd_Servo3,

// 9 axis sensor
	Req_Accel_XYZ,
	Req_Gyro_XYZ,
	Req_Accel_Gyro_XYZ,
	Req_Gyro_XYZ_Integral
} XA_list;

#define ACK					0x01
#define NACK				0xff
#define ERR_CHECKSUM		0xfe
#define ERR_INTERNAL		0xfd
#define ERR_LOWVOLTAGE		0xfc
/* MotorDriver */
#define BASEADDR_MTRDRV		0xd0
/* ADC */
#define BASEADDR_ADC		0xa0
/* I/O Port */
#define BASEADDR_IO			0x10
/* Ultrasonic Distance Sensor */
#define BASEADDR_UDS		0x20
/* ServoMotorDriver */
#define BASEADDR_SERVO		0x30
/* Other device */
#define BASEADDR_OTHER		0xe0

int XAS_In(uint8_t qty_byte, uint8_t rBuf[]);
int XAS_Out(uint8_t qty_byte, uint8_t tBuf[]);
void XAS_OutERR(uint8_t qty_byte, uint8_t errData);

#endif /* __XA_S_H__ */
