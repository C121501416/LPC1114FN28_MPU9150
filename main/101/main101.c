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

	// I2C初期化、失敗したら幽閉
	I2CInit((uint32_t)I2CMASTER);

	// I2C初期化後には時間待ちが必要
	_delay_ms(300);

	// センシングをスタートさせる、これよりセンサが有効になる
	MPU9150_Sensing_Start();

	// DLPF(ディジタルローパスフィルタ)、高い周波数は切り捨て御免
	MPU9150_DLPF_Config(0x06);

	// ジャイロセンサの範囲設定
	MPU9150_Gyro_Config(0x00);

	// 加速度センサの範囲・HPF(ハイパスフィルタ)の設定、低い周波数は切り捨て御免
	MPU9150_Accel_Config(0x00, 0x00);

	// RS485通信を初期化、スレーブアドレスの重複に注意すること
	RS485_Init(myaddr);

	while(1)
	{
		/* 毎回行う必要がないが、致し方ないのでしょうがない      ---------- */
		// 9軸センサから加速度の値を抽出
		i2c_accel_status = MPU9150_Accel_XYZ_Read(&accel);
		// 9軸センサから角速度の値を抽出
		i2c_gyro_status = MPU9150_Gyro_XYZ_Read(&gyro);
		/* ---------------------------------------------------- */

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
