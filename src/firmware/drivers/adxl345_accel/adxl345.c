// adxl345.c
// accel.c
#include "adxl345.h"
#include "i2c_driver.h"
#include <util/delay.h>
#include <stdbool.h>

double _a_x;
double _a_y;
double _a_z;

double _lowpass_x;
double _lowpass_y;
double _lowpass_z;

void ADXL345_ReadAccel(double *a_x, double *a_y, double *a_z);
void ADXL345_LowPass_Filter(double x, double y, double z);
void ADXL345_Calibrate(void);

void InitializeADXL345(void)
{
    uint8_t zero = 0;
    i2c_init();
    _delay_ms(2);
    // AN-1077: Minimum init
    i2c_start(ADXL345_ADDR);
    i2c_write(0x31);
    i2c_write(0x0B);    // +/- 16g, 13-bit Mode
    i2c_stop();

    i2c_start(ADXL345_ADDR);
    i2c_write(0x2D);
    i2c_write(0x08);    // Start measurement
    i2c_stop();

    i2c_start(ADXL345_ADDR);
    i2c_write(0x2E);
    i2c_write(0x80);    // Enable DATA_READY interrupt
    i2c_stop();
    _delay_ms(2);

    // Set offset reg to zero
    i2c_writeReg(ADXL345_ADDR, 0x1E, &zero, 1);
    i2c_writeReg(ADXL345_ADDR, 0x1F, &zero, 1);
    i2c_writeReg(ADXL345_ADDR, 0x20, &zero, 1);

    _a_x = 0;
    _a_y = 0;
    _a_z = 0;

    _lowpass_x = 0;
    _lowpass_x = 0;
    _lowpass_x = 0;

    _delay_ms(2); // Arbitrary delay amount
    // ADXL345_Calibrate();
}

void ADXL345_Calibrate(void)
{
    double tmpx = 0;
    double tmpy = 0;
    double tmpz = 0;

    uint8_t a_offx = 0;
    uint8_t a_offy = 0;
    uint8_t a_offz = 0;


    // take the mean from 10 gyro probes and divide
    // it from the current probe
    for (uint8_t i = 0; i < 10; i++)
    {
        ADXL345_UpdateData();
        tmpx += _a_x;
        tmpy += _a_y;
        tmpz += _a_z;
        _delay_ms(20); // Arbitrary delay amount
    }

    // Each LSB of output in full-resolution is one-quarter of an
    // LSB of the offset register. (Datasheet: Offset Calibration)
    a_offx = -round(tmpx / 10 / 4);
    a_offy = -round(tmpy / 10 / 4) ;
    a_offz = -round(((tmpz / 10) - 256) / 4) ;

    // Set calibrated offset values
    i2c_writeReg(ADXL345_ADDR, 0x1E, &a_offx, 1);
    i2c_writeReg(ADXL345_ADDR, 0x1F, &a_offy, 1);
    i2c_writeReg(ADXL345_ADDR, 0x20, &a_offz, 1);

}

void ADXL345_ReadAccel(double *a_x, double *a_y, double *a_z)
{
    uint8_t buff[6];

    i2c_start(ADXL345_ADDR);
    i2c_write(0x32);
    i2c_stop();

    i2c_receive(ADXL345_ADDR, buff, 6);

    *a_x = ((buff[1] << 8) | buff[0]);
    *a_y = ((buff[3] << 8) | buff[2]);
    *a_z = ((buff[5] << 8) | buff[4]);

}

void ADXL345_UpdateData(void)
{
    double x, y, z;
    ADXL345_ReadAccel(&x, &y, &z);
    ADXL345_LowPass_Filter(x, y, z);

    _a_x = _lowpass_x;
    _a_y = _lowpass_y;
    _a_z = _lowpass_z;
}

void ADXL345_LowPass_Filter(double x, double y, double z)
{
  double alpha = 0.2;

  _lowpass_x = x * alpha + (_lowpass_x * (1.0 - alpha));
  _lowpass_y = y * alpha + (_lowpass_y * (1.0 - alpha));
  _lowpass_z = z * alpha + (_lowpass_z * (1.0 - alpha));

}

double ADXL345_GetX(void)
{
    return _a_x / 256;
}

double ADXL345_GetY(void)
{
    return _a_y / 256;
}

double ADXL345_GetZ(void)
{
    return _a_z / 256;
}
