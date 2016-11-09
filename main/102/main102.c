#include "project.h"

#ifdef __MAIN_102__

/* インクルード        ============================================= */
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

/* 関数プロトタイプ宣言       ======================================= */

/* メイン関数     =============================================== */
int main(void)
{
	int status;
	uint8_t myaddr = SLAVE_ADDR, qty;
	uint8_t rBuf[16] = {0}, tBuf[16] = {0};

	/* システム初期化           ========================================= */
	SysTickInit(INTERVAL);

	// I2C初期化
	I2CInit((uint32_t)I2CMASTER);
	// I2C初期化後には時間待ちが必要
	_delay_ms(300);

	/* MPU9150初期化     ========================================== */
	// センシングをスタートさせる、これよりセンサが有効になる
	MPU9150_Sensing_Start();

	// DLPF(ディジタルローパスフィルタ)、高い周波数は切り捨て御免
	MPU9150_DLPF_Config(0x06);

	// ジャイロセンサの範囲設定
	MPU9150_Gyro_Config(0x00);

	// 加速度センサの範囲・HPF(ハイパスフィルタ)の設定、低い周波数は切り捨て御免
	MPU9150_Accel_Config(0x00, 0x00);

	MPU9150_Config();

	/* RS485初期化     ============================================ */
	// RS485通信を初期化、スレーブアドレスの重複に注意すること
	RS485_Init(myaddr);

	while(1)
	{
		// 通信要求があるまで幽閉
		while(!(LPC_UART->LSR & UART_LSR_RDR));
		if(RS485_RecvData() != myaddr)	continue;	// この1行いらないかも

		// マスタ側から見た、送信データ数が上位8ビット、受信データ数が下位8ビット
		qty = RS485_RecvData();
		// へんじがないただのしかばねのようだ → スタートへ戻る
		if(qty == NO_RESPONSE)	continue;

		// マスタからデータを受信する
		status = XAS_In(qty, rBuf);

		// へんじがな(ry
		if(status == NO_RESPONSE)	continue;
		else if(status == ERR_CHECKSUM)
		{
			XAS_OutERR(qty, ERR_CHECKSUM);
			continue;
		}

		// レジスタ選択
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
