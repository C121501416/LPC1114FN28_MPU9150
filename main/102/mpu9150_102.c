/* =========================================================
 *
 * @ファイル名	:	mpu9150.h
 * @概要		:	9軸センサー(MPU9150)とI2Cで通信して、ごにょごにょする奴
 * @作成日	:	2014-10-27
 * @製作者	:	KINECT
 *
 ======================================================== */

#include "project.h"
#ifdef __MAIN_102__

#include "LPC11xx.h"
#include "system_LPC11xx.h"
#include "lpc11xx_libcfg.h"
#include "i2c.h"
#include "mpu9150_102.h"
#include "lpc_types.h"

_Axis3_t accel, gyro;

void TIMER32_0_IRQHandler(void)
{
	static uint32_t I2CMasterState_last = 13;
	static uint32_t i2c_timeout;
	LPC_TMR32B0->IR = TIM_IR_CLR(TIM_MR0_INT);

	/* 初期条件でMPU9150の値を受け取る設定 */
	if(I2CMasterState_last == 13)
	{
		I2CWriteLength = 2;
		I2CReadLength = 14;
		I2CMasterBuffer[0] = MPU9150_SLAVE_ADDR << 1;
		I2CMasterBuffer[1] = MPU9150_RA_ACCEL_XH;
		I2CMasterBuffer[2] = (MPU9150_SLAVE_ADDR << 1) | RD_BIT;
		LPC_I2C->CONSET = I2CONSET_STA;
		I2CMasterState = I2C_BUSY;
	}
	/* MPU9150の値が更新された */
	if(I2CMasterState != I2C_BUSY && I2CMasterState_last == I2C_BUSY)
	{
		LPC_I2C->CONCLR = I2CONCLR_STAC;
		accel.X.BYTE.H = I2CSlaveBuffer[0];
		accel.X.BYTE.L = I2CSlaveBuffer[1];
		accel.Y.BYTE.H = I2CSlaveBuffer[2];
		accel.Y.BYTE.L = I2CSlaveBuffer[3];
		accel.Z.BYTE.H = I2CSlaveBuffer[4];
		accel.Z.BYTE.L = I2CSlaveBuffer[5];
//		temp.BYTE.H	   = I2CSlaveBuffer[6];
//		temp.BYTE.L	   = I2CSlaveBuffer[7];
		 gyro.X.BYTE.H = I2CSlaveBuffer[8];
		 gyro.X.BYTE.L = I2CSlaveBuffer[9];
		 gyro.Y.BYTE.H = I2CSlaveBuffer[10];
		 gyro.Y.BYTE.L = I2CSlaveBuffer[11];
		 gyro.Z.BYTE.H = I2CSlaveBuffer[12];
		 gyro.Z.BYTE.L = I2CSlaveBuffer[13];
		LPC_I2C->CONSET = I2CONSET_STA;
		I2CMasterState = I2C_BUSY;
	}
	I2CMasterState_last = I2CMasterState;
}

void MPU9150_Config(void)
{
	/* Enable the I2C Interrupt */
	NVIC_SetPriority(I2C_IRQn, 0);

	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_CT32B0, ENABLE);

	LPC_TMR32B0->PR = SystemCoreClock / 1000000 - 1;	// 1us
	LPC_TMR32B0->MR0 = 100;								// 100us
	LPC_TMR32B0->MCR = TIM_INT_ON_MATCH(0)|TIM_RESET_ON_MATCH(0);
	LPC_TMR32B0->TCR = TIM_ENABLE;
	NVIC_SetPriority(TIMER_32_0_IRQn, 3);
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
}

uint8_t	MPU9150_MyAddr_Read(void)
{
	I2CWriteLength = 2;
	I2CReadLength  = 1;
	I2CMasterBuffer[0] = MPU9150_SLAVE_ADDR << 1;
	I2CMasterBuffer[1] = MPU9150_RA_WHOAMI;
	I2CMasterBuffer[2] = (MPU9150_SLAVE_ADDR << 1) | RD_BIT;
	I2CEngine();
	return I2CSlaveBuffer[0];
}

void MPU9150_Sensing_Start(void)
{
	I2CWriteLength = 3;
	I2CReadLength = 0;
	I2CMasterBuffer[0] = MPU9150_SLAVE_ADDR << 1;
	I2CMasterBuffer[1] = MPU9150_RA_PWR_MGMT_1;
	I2CMasterBuffer[2] = 0x00;
	I2CEngine();

	I2CWriteLength = 3;
	I2CReadLength = 0;
	I2CMasterBuffer[0] = MPU9150_SLAVE_ADDR << 1;
	I2CMasterBuffer[1] = MPU9150_RA_INT_PIN_CFG;
	I2CMasterBuffer[2] = 0x02;
	I2CEngine();
}

void MPU9150_DLPF_Config(uint8_t DLPF_CFG)
{
	I2CWriteLength = 3;
	I2CReadLength = 0;
	I2CMasterBuffer[0] = MPU9150_SLAVE_ADDR << 1;
	I2CMasterBuffer[1] = MPU9150_RA_CONFIG;
	I2CMasterBuffer[2] = DLPF_CFG & 0x07;
	I2CEngine();
}

void MPU9150_Gyro_Config(uint8_t FS_SEL)
{
	I2CWriteLength = 3;
	I2CReadLength = 0;
	I2CMasterBuffer[0] = MPU9150_SLAVE_ADDR << 1;
	I2CMasterBuffer[1] = MPU9150_RA_GYRO_CONFIG;
	I2CMasterBuffer[2] = (FS_SEL & 0x03) << 3;
	I2CEngine();
}

void MPU9150_Accel_Config(uint8_t AFS_SEL, uint8_t HPF)
{
	I2CWriteLength = 3;
	I2CReadLength = 0;
	I2CMasterBuffer[0] = MPU9150_SLAVE_ADDR << 1;
	I2CMasterBuffer[1] = MPU9150_RA_ACCEL_CONFIG;
	I2CMasterBuffer[2] = ((AFS_SEL & 0x03) << 3) | (HPF & 0x07);
	I2CEngine();
}

#endif // __MAIN_102__
