/* =========================================================
 *
 * @ファイル名	:	mpu9150.h
 * @概要		:	9軸センサー(MPU9150)とI2Cで通信して、ごにょごにょする奴
 * @作成日	:	2014-10-27
 * @製作者	:	KINECT
 *
 ======================================================== */

#include "project.h"
#ifdef __MAIN_101__

#include <lpc11xx.h>
#include "i2c.h"
#include "mpu9150_101.h"

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

uint32_t MPU9150_Gyro_XYZ_Read(_Axis3_t* gyro)
{
	uint32_t state;
	I2CWriteLength = 2;
	I2CReadLength = 6;
	I2CMasterBuffer[0] = MPU9150_SLAVE_ADDR << 1;
	I2CMasterBuffer[1] = MPU9150_RA_GYRO_XH;
	I2CMasterBuffer[2] = (MPU9150_SLAVE_ADDR << 1) | RD_BIT;
	state = I2CEngine();

	gyro->XH = I2CSlaveBuffer[0];
	gyro->XL = I2CSlaveBuffer[1];
	gyro->YH = I2CSlaveBuffer[2];
	gyro->YL = I2CSlaveBuffer[3];
	gyro->ZH = I2CSlaveBuffer[4];
	gyro->ZL = I2CSlaveBuffer[5];

	return state;
}

uint32_t MPU9150_Accel_XYZ_Read(_Axis3_t *accel)
{
	uint32_t state;
	I2CWriteLength = 2;
	I2CReadLength = 6;
	I2CMasterBuffer[0] = MPU9150_SLAVE_ADDR << 1;
	I2CMasterBuffer[1] = MPU9150_RA_ACCEL_XH;
	I2CMasterBuffer[2] = (MPU9150_SLAVE_ADDR << 1) | RD_BIT;
	state = I2CEngine();

	accel->XH = I2CSlaveBuffer[0];
	accel->XL = I2CSlaveBuffer[1];
	accel->YH = I2CSlaveBuffer[2];
	accel->YL = I2CSlaveBuffer[3];
	accel->ZH = I2CSlaveBuffer[4];
	accel->ZL = I2CSlaveBuffer[5];

	return state;
}

#endif // __MAIN_101__
