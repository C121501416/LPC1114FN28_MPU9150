/* =========================================================
 *
 * @�t�@�C����	:	mpu9150.h
 * @�T�v		:	9���Z���T�[(MPU9150)��I2C�ŒʐM���āA���ɂ傲�ɂ傷��z
 * @�쐬��	:	2014-10-27
 * @�����	:	KINECT
 *
 ======================================================== */

#include "project.h"
#ifdef __MAIN_102__

#ifndef __MPU9150_102_H__
#define __MPU9150_102_H__

/* �C���N���[�h�t�@�C��   ======================================== */
#include "i2c.h"
#include "lpc_types.h"

/* �萔��`     ============================================= */
/* I2C�A�h���X  --------------------------------------------- */
#define MPU9150_SLAVE_ADDR		0x68

#define MPU9150_RA_CONFIG		0x1A
#define MPU9150_RA_GYRO_CONFIG	0x1B
#define MPU9150_RA_ACCEL_CONFIG	0x1C
#define MPU9150_RA_INT_PIN_CFG	0x37
#define MPU9150_RA_ACCEL_XH		0x3B
#define MPU9150_RA_GYRO_XH		0x43
#define MPU9150_RA_PWR_MGMT_1	0x6B
#define	MPU9150_RA_WHOAMI		0x75

/* �^��`  ================================================ */
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


/* �O���[�o���ϐ�    ========================================== */
/* I2C���W�X�^  --------------------------------------------- */
extern volatile uint32_t I2CCount;
extern volatile uint8_t  I2CMasterBuffer[BUFSIZE];
extern volatile uint8_t  I2CSlaveBuffer[BUFSIZE];
extern volatile uint32_t I2CMasterState;
extern volatile uint32_t I2CReadLength, I2CWriteLength;
extern _Axis3_t accel, gyro;

/* �֐��v���g�^�C�v�錾    ====================================== */
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
