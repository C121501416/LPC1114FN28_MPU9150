#include "project.h"

#ifdef __MAIN_101__

#include <lpc11xx.h>
#include "system_LPC11xx.h"
#include "lpc11xx_libcfg.h"
#include "i2c.h"
#include "uart.h"
#include "rs485.h"
#include "xa_s.h"
#include "systick.h"
#include "mpu9150_101.h"

int main(void)
{
	int status;
	uint32_t i2c_gyro_status, i2c_accel_status;
	uint8_t myaddr = 0xE0, qty;
	uint8_t rBuf[16] = {0}, tBuf[16] = {0};
	_Axis3_t accel, gyro;

	SystemCoreClockUpdate();
	SysTickInit(INTERVAL);

	// I2C�������A���s������H��
	I2CInit((uint32_t)I2CMASTER);

	// I2C��������ɂ͎��ԑ҂����K�v
	_delay_ms(300);

	// �Z���V���O���X�^�[�g������A������Z���T���L���ɂȂ�
	MPU9150_Sensing_Start();

	// DLPF(�f�B�W�^�����[�p�X�t�B���^)�A�������g���͐؂�̂Č��
	MPU9150_DLPF_Config(0x06);

	// �W���C���Z���T�͈̔͐ݒ�
	MPU9150_Gyro_Config(0x00);

	// �����x�Z���T�͈̔́EHPF(�n�C�p�X�t�B���^)�̐ݒ�A�Ⴂ���g���͐؂�̂Č��
	MPU9150_Accel_Config(0x00, 0x00);

	// RS485�ʐM���������A�X���[�u�A�h���X�̏d���ɒ��ӂ��邱��
	RS485_Init(myaddr);

	while(1)
	{
		/* ����s���K�v���Ȃ����A�v�����Ȃ��̂ł��傤���Ȃ�      ---------- */
		// 9���Z���T��������x�̒l�𒊏o
		i2c_accel_status = MPU9150_Accel_XYZ_Read(&accel);
		// 9���Z���T����p���x�̒l�𒊏o
		i2c_gyro_status = MPU9150_Gyro_XYZ_Read(&gyro);
		/* ---------------------------------------------------- */

		// �ʐM�v��������܂ŗH��
		while(!(LPC_UART->LSR & UART_LSR_RDR));
		if(RS485_RecvData() != myaddr)	continue;	// ����1�s����Ȃ�����

		// �}�X�^�����猩���A���M�f�[�^�������8�r�b�g�A��M�f�[�^��������8�r�b�g
		qty = RS485_RecvData();
		// �ւ񂶂��Ȃ������̂����΂˂̂悤�� �� �X�^�[�g�֖߂�
		if(qty == NO_RESPONSE)	continue;

		// �}�X�^����f�[�^����M����
		status = XAS_In(qty, rBuf);

		// �ւ񂶂���(ry
		if(status == NO_RESPONSE)	continue;
		else if(status == ERR_CHECKSUM)
		{
			XAS_OutERR(qty, ERR_CHECKSUM);
			continue;
		}

		// ���W�X�^�I��
		switch(rBuf[0])
		{
			case Req_Response:
				tBuf[0] = myaddr;
				break;
			case Req_Accel_XYZ:
				tBuf[0] = accel.XH;
				tBuf[1] = accel.XL;
				tBuf[2] = accel.YH;
				tBuf[3] = accel.YL;
				tBuf[4] = accel.ZH;
				tBuf[5] = accel.ZL;
				tBuf[6] = i2c_accel_status;
				break;
			case Req_Gyro_XYZ:
				tBuf[0] = gyro.XH;
				tBuf[1] = gyro.XL;
				tBuf[2] = gyro.YH;
				tBuf[3] = gyro.YL;
				tBuf[4] = gyro.ZH;
				tBuf[5] = gyro.ZL;
				tBuf[6] = i2c_gyro_status;
				break;
			case Req_Accel_Gyro_XYZ:
				tBuf[0]  = accel.XH;
				tBuf[1]  = accel.XL;
				tBuf[2]  = accel.YH;
				tBuf[3]  = accel.YL;
				tBuf[4]  = accel.ZH;
				tBuf[5]  = accel.ZL;
				tBuf[6]  = gyro.XH;
				tBuf[7]  = gyro.XL;
				tBuf[8]  = gyro.YH;
				tBuf[9]  = gyro.YL;
				tBuf[10] = gyro.ZH;
				tBuf[11] = gyro.ZL;
				tBuf[12] = i2c_accel_status;
				tBuf[13] = i2c_gyro_status;
				break;
		}
		XAS_Out(qty, tBuf);
	}
}

#endif

/******************************************************************************
**                            End Of File
******************************************************************************/
