#include "project.h"

#ifdef __MAIN_100__

#include "lpc11xx.h"
#include "system_LPC11xx.h"
#include "lpc11xx_libcfg.h"
#include "systick.h"
#include "xprintf.h"
#include "soft_pwm.h"
#include "xa_s.h"
#include "uart.h"
#include "rs485.h"
#include "i2c.h"

#define MPU9150_ADDRESS			0x68
#define MPU9150_RA_WHOAMI		0x75
#define MPU9150_RA_ACCEL_XH		0x3B
#define MPU9150_RA_GYRO_XH		0x43
#define MPU9150_RA_PWR_MGMT_1	0x6B
#define MPU9150_RA_INT_PIN_CFG	0x37
#define MPU9150_RA_GYRO_CONFIG	0x1B
#define MPU9150_RA_ACCEL_CONFIG	0x1C

typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
} _Axis3_t;

extern volatile uint32_t I2CCount;
extern volatile uint8_t I2CMasterBuffer[BUFSIZE];
extern volatile uint8_t I2CSlaveBuffer[BUFSIZE];
extern volatile uint32_t I2CMasterState;
extern volatile uint32_t I2CReadLength, I2CWriteLength;

uint8_t MPU9150_whoami(void)
{
	I2CWriteLength = 2;
	I2CReadLength  = 1;
	I2CMasterBuffer[0] = MPU9150_ADDRESS << 1;
	I2CMasterBuffer[1] = MPU9150_RA_WHOAMI;
	I2CMasterBuffer[2] = (MPU9150_ADDRESS << 1) | RD_BIT;

	I2CEngine();

	return I2CSlaveBuffer[0];
}

void MPU9150_StartSensing(void)
{
	I2CWriteLength = 3;
	I2CReadLength = 0;
	I2CMasterBuffer[0] = MPU9150_ADDRESS << 1;
	I2CMasterBuffer[1] = MPU9150_RA_PWR_MGMT_1;
	I2CMasterBuffer[2] = 0x00;
	I2CEngine();

	I2CWriteLength = 3;
	I2CReadLength = 0;
	I2CMasterBuffer[0] = MPU9150_ADDRESS << 1;
	I2CMasterBuffer[1] = MPU9150_RA_INT_PIN_CFG;
	I2CMasterBuffer[2] = 0x02;
	I2CEngine();
}

void MPU9150_ConfigGyro(void)
{
	I2CWriteLength = 3;
	I2CReadLength = 0;
	I2CMasterBuffer[0] = MPU9150_ADDRESS << 1;
	I2CMasterBuffer[1] = MPU9150_RA_GYRO_CONFIG;
	I2CMasterBuffer[2] = 0x18;
	I2CEngine();
}

void MPU9150_ReadAcceleXYZ(_Axis3_t *acc)
{
	I2CWriteLength = 2;
	I2CReadLength = 6;
	I2CMasterBuffer[0] = MPU9150_ADDRESS << 1;
	I2CMasterBuffer[1] = MPU9150_RA_ACCEL_XH;
	I2CMasterBuffer[2] = (MPU9150_ADDRESS << 1) | RD_BIT;

	I2CEngine();

	acc->x = (I2CSlaveBuffer[0] << 8) | I2CSlaveBuffer[1];
	acc->y = (I2CSlaveBuffer[2] << 8) | I2CSlaveBuffer[3];
	acc->z = (I2CSlaveBuffer[4] << 8) | I2CSlaveBuffer[5];
}

void MPU9150_ReadGyroXYZ(_Axis3_t *gyro)
{
	int i;
	for(i = 0; i < BUFSIZE; i++)	I2CSlaveBuffer[i] = 0;

	I2CWriteLength = 2;
	I2CReadLength = 6;
	I2CMasterBuffer[0] = MPU9150_ADDRESS << 1;
	I2CMasterBuffer[1] = MPU9150_RA_GYRO_XH;
	I2CMasterBuffer[2] = (MPU9150_ADDRESS << 1) | RD_BIT;

	I2CEngine();

	gyro->x = (I2CSlaveBuffer[0] << 8) | I2CSlaveBuffer[1];
	gyro->y = (I2CSlaveBuffer[2] << 8) | I2CSlaveBuffer[3];
	gyro->z = (I2CSlaveBuffer[4] << 8) | I2CSlaveBuffer[5];
}

/*******************************************************************************
**   Main Function  main()
*******************************************************************************/
int main (void)
{
	int i, j;
	int status;
	uint8_t myaddr = 0xE0, qty;
	uint8_t rBuf[16] = {0}, tBuf[16] = {0};
	_Axis3_t acc, gyro;

	if(I2CInit((uint32_t)I2CMASTER) == FALSE)
	{
		while(1);
	}
	for(i = 0; i < 3000; i++)
		for(j = 0; j < 3000; j++);

	MPU9150_StartSensing();

	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_GPIO, ENABLE);
	RS485_Init(myaddr);

	while(1)
	{
		while(!(LPC_UART->LSR & UART_LSR_RDR));
		if(RS485_RecvData() != myaddr)	continue;

		if((qty = RS485_RecvData()) == NO_RESPONSE)	continue;

		status = XAS_In(qty, rBuf);

		if(status == NO_RESPONSE)	continue;
		else if(status == ERR_CHECKSUM)
		{
			XAS_OutERR(qty, ERR_CHECKSUM);
			continue;
		}

		MPU9150_ReadAcceleXYZ(&acc);

		switch(rBuf[0])
		{
			case Req_Response:
				tBuf[0] = myaddr;
				break;
			case Req_AcceleXYZ:
				tBuf[0] = acc.x & 0x00FF;
				tBuf[1] = (acc.x & 0xFF00) >> 8;
				tBuf[2] = acc.y & 0x00FF;
				tBuf[3] = (acc.y & 0xFF00) >> 8;
				tBuf[4] = acc.z & 0x00FF;
				tBuf[5] = (acc.z & 0xFF00) >> 8;
				break;
			case Req_GyroXYZ:
				for(i = 7; i >= 0; i--)
				{
					tBuf[i] = i;
				}
				break;
		}
		XAS_Out(qty, tBuf);
	}
}

#endif

/******************************************************************************
**                            End Of File
******************************************************************************/
