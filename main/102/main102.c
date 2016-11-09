#include "project.h"

#ifdef __MAIN_102__

/* �C���N���[�h        ============================================= */
#include <lpc11xx.h>
#include "system_LPC11xx.h"
#include "lpc11xx_libcfg.h"
#include "i2c.h"
#include "uart.h"
#include "rs485.h"
#include "xa_s.h"
#include "systick.h"
#include "mpu9150_102.h"
#include "main102.h"

/* �֐��v���g�^�C�v�錾       ======================================= */

/* ���C���֐�     =============================================== */
int main(void)
{
	int status;
	uint8_t myaddr = SLAVE_ADDR, qty;
	uint8_t rBuf[16] = {0}, tBuf[16] = {0};

	/* �V�X�e��������           ========================================= */
	SysTickInit(INTERVAL);

	// I2C������
	I2CInit((uint32_t)I2CMASTER);
	// I2C��������ɂ͎��ԑ҂����K�v
	_delay_ms(300);

	/* MPU9150������     ========================================== */
	// �Z���V���O���X�^�[�g������A������Z���T���L���ɂȂ�
	MPU9150_Sensing_Start();

	// DLPF(�f�B�W�^�����[�p�X�t�B���^)�A�������g���͐؂�̂Č��
	MPU9150_DLPF_Config(0x06);

	// �W���C���Z���T�͈̔͐ݒ�
	MPU9150_Gyro_Config(0x00);

	// �����x�Z���T�͈̔́EHPF(�n�C�p�X�t�B���^)�̐ݒ�A�Ⴂ���g���͐؂�̂Č��
	MPU9150_Accel_Config(0x00, 0x00);

	MPU9150_Config();

	/* RS485������     ============================================ */
	// RS485�ʐM���������A�X���[�u�A�h���X�̏d���ɒ��ӂ��邱��
	RS485_Init(myaddr);

	while(1)
	{
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
				tBuf[0] = accel.X.BYTE.H;
				tBuf[1] = accel.X.BYTE.L;
				tBuf[2] = accel.Y.BYTE.H;
				tBuf[3] = accel.Y.BYTE.L;
				tBuf[4] = accel.Z.BYTE.H;
				tBuf[5] = accel.Z.BYTE.L;
				tBuf[6] = I2CMasterState;
				break;
			case Req_Gyro_XYZ:
				tBuf[0] = gyro.X.BYTE.H;
				tBuf[1] = gyro.X.BYTE.L;
				tBuf[2] = gyro.Y.BYTE.H;
				tBuf[3] = gyro.Y.BYTE.L;
				tBuf[4] = gyro.Z.BYTE.H;
				tBuf[5] = gyro.Z.BYTE.L;
				tBuf[6] = I2CMasterState;
				break;
			case Req_Accel_Gyro_XYZ:
				tBuf[0]  = accel.X.BYTE.H;
				tBuf[1]  = accel.X.BYTE.L;
				tBuf[2]  = accel.Y.BYTE.H;
				tBuf[3]  = accel.Y.BYTE.L;
				tBuf[4]  = accel.Z.BYTE.H;
				tBuf[5]  = accel.Z.BYTE.L;
				tBuf[6]  =  gyro.X.BYTE.H;
				tBuf[7]  =  gyro.X.BYTE.L;
				tBuf[8]  =  gyro.Y.BYTE.H;
				tBuf[9]  =  gyro.Y.BYTE.L;
				tBuf[10] =  gyro.Z.BYTE.H;
				tBuf[11] =  gyro.Z.BYTE.L;
				tBuf[12] = I2CMasterState;
				break;
		}
		XAS_Out(qty, tBuf);
	}
}

#endif

/******************************************************************************
**                            End Of File
******************************************************************************/
