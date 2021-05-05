#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "qextserialport.h"



class CSerialPort // : public SerialPort
{
public:
    enum UART_TYPE // 串口号
    {
        UART_0_SCAN_GUN,// scanning gun
        UART_1_SCAN_MODEULE,// Barcode scanning module
        UART_2_PC_PRINTER, // printer and PC
        UART_3_430
    };
public:
    CSerialPort();

    static QextSerialPort * GetSerialPort(UART_TYPE eType);
    static bool OpenSerialPort(UART_TYPE eType);

private:
    // 串口0扫描枪 串口1条形码扫描模块 串口2打印机和PC 串口3单片机430采集模块
    static QextSerialPort *s_pUart0; // 串口指针
    static QextSerialPort *s_pUart1; // 串口指针
    static QextSerialPort *s_pUart2; // 串口指针
    static QextSerialPort *s_pUart3; // 串口指针
};

#endif // SERIALPORT_H
