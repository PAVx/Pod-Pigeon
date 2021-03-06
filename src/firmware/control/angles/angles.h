/*
	PAVx -- Pod-Based Autonomous Vehicles
	Library Created By: Niraj Raniga
	March 2017
*/

// angle.h

// Reference: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

#ifndef _ANGLES_H_
#define _ANGLES_H_

#include <stdbool.h>

#define YAW_ANGLE (0)
#define PITCH_ANGLE (1)
#define ROLL_ANGLE (2)

void imu2euler(double* ypr, double accX, double accY, double accZ, double gyroX, double gyroY, double gyroZ, double magX, double magY);
void imu2euler_simple(double* ypr, double accX, double accY, double accZ, double magX, double magY);

#endif	// _ANGLES_H_
