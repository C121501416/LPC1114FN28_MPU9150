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

#ifndef __MPU9150_102_H__
#define __MPU9150_102_H__

/* インクルードファイル   ======================================== */
#include "i2c.h"
#include "lpc_types.h"

/* 定数定義     ============================================= */
/* I2Cアドレス  --------------------------------------------- */
#define MPU9150_SLAVE_ADDR		0x68

#define MPU9150_RA_CONFIG		0x1A
#define MPU9150_RA_GYRO_CONFIG	0x1B
#define MPU9150_RA_ACCEL_CONFIG	0x1C
#define MPU9150_RA_INT_PIN_CFG	0x37
#define MPU9150_RA_ACCEL_XH		0x3B
#define MPU9150_RA_GYRO_XH		0x43
#define MPU9150_RA_PWR_MGMT_1	0x6B
#define	MPU9150_RA_WHOAMI		0x75

/* 型定義  ================================================ */
typedef struct
{
	union
	{
		int16_t WORD;
		struct
		{
			uint8_t L;
			uint8_t H;
		} BYTE;
	} X;
	union
	{
		int16_t WORD;
		struct
		{
			uint8_t L;
			uint8_t H;
		} BYTE;
	} Y;
	union
	{
		int16_t WORD;
		struct
		{
			uint8_t L;
			uint8_t H;
		} BYTE;
	} Z;
} _Axis3_t;


/* グローバル変数    ========================================== */
/* I2Cレジスタ  --------------------------------------------- */
extern volatile uint32_t I2CCount;
extern volatile uint8_t  I2CMasterBuffer[BUFSIZE];
extern volatile uint8_t  I2CSlaveBuffer[BUFSIZE];
extern volatile uint32_t I2CMasterState;
extern volatile uint32_t I2CReadLength, I2CWriteLength;
extern _Axis3_t accel, gyro;

/* 関数プロトタイプ宣言    ====================================== */
uint8_t		MPU9150_MyAddr_Read(void);
void		MPU9150_Sensing_Start(void);
void		MPU9150_DLPF_Config (uint8_t DLPF_CFG);
void		MPU9150_Gyro_Config (uint8_t FS_SEL);
void 		MPU9150_Accel_Config(uint8_t AFS_SEL, uint8_t HPF);
uint32_t	MPU9150_Gyro_XYZ_Read (_Axis3_t* gyro);
uint32_t	MPU9150_Accel_XYZ_Read(_Axis3_t *accel);
void		MPU9150_Config(void);

#endif // __MPU9150_102_H__
#endif // __MAIN_102__
