#include "system.h"

#include <inttypes.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

//#define MOTOR_TEST
//#define IMU_DEBUG
//#define GPS_DEBUG
//#define LED_DEBUG
//#define PACKET_DEBUG
#define NIRAJ_TESTING

#ifdef IMU_DEBUG
	char gbuffer[15];
	char abuffer[15];
#endif

#ifdef GPS_DEBUG
	char gpsbuffer[50];
	int i = 0;
#endif

#ifdef MOTOR_TEST
	uint8_t byte = 0;
	uint16_t motor_vals[4] = {0};
	uint8_t motor = 0;
#endif

int main (void) {

	system_initialize();

	for(;;) {

		#ifdef MOTOR_TEST
			if (!UART_IsEmpty()) {
				byte = UART_GetByte();
				toggle_led(SYSTEM_LED);

				switch (byte) {

					case '1':
						motor = 1;
						break;
					case '2':
						motor = 2;
						break;
					case '3':
						motor = 3;
						break;
					case '4':
						motor = 4;
						break;

					case '5':
						motor = 5;
						break;

					case 'w':

						if (motor == 5) {
							motor_vals[0] += 10;
							if (motor_vals[0] > 255) {
								motor_vals[0] = 255;
							}
							motor_vals[1] += 10;
							if (motor_vals[1] > 255) {
								motor_vals[1] = 255;
							}
							motor_vals[2] += 10;
							if (motor_vals[2] > 255) {
								motor_vals[2] = 255;
							}
							motor_vals[3] += 10;
							if (motor_vals[3] > 255) {
								motor_vals[3] = 255;
							}

							motor_set(1, motor_vals[0]);
							motor_set(2, motor_vals[1]);
							motor_set(3, motor_vals[2]);
							motor_set(4, motor_vals[3]);


						} else {
							motor_vals[motor - 1] += 10;
							if (motor_vals[motor - 1] > 255) {
								motor_vals[motor - 1] = 255;
							}
							motor_set(motor, (uint8_t)motor_vals[motor - 1]);
						}

						break;
					case 's':
						if (motor == 5) {
							if (motor_vals[0] > 10) {
								motor_vals[0] -= 10;
							} else {
								motor_vals[0] = 0;
							}

							if (motor_vals[1] > 10) {
								motor_vals[1] -= 10;
							} else {
								motor_vals[1] = 0;
							}

							if (motor_vals[2] > 10) {
								motor_vals[2] -= 10;
							} else {
								motor_vals[2] = 0;
							}

							if (motor_vals[2] > 10) {
								motor_vals[2] -= 10;
							} else {
								motor_vals[2] = 0;
							}

							motor_set(1, motor_vals[0]);
							motor_set(2, motor_vals[1]);
							motor_set(3, motor_vals[2]);
							motor_set(4, motor_vals[3]);
						} else {
							if (motor_vals[motor - 1] > 10) {
								motor_vals[motor - 1] -= 10;
							} else {
								motor_vals[motor - 1] = 0;
							}

							motor_set(motor, (uint8_t)motor_vals[motor - 1]);
						}

						break;

					case 'k':

						motor_set(1, 0);
						motor_set(2, 0);
						motor_set(3, 0);
						motor_set(4, 0);

						motor_vals[0] = 0;
						motor_vals[1] = 0;
						motor_vals[2] = 0;
						motor_vals[3] = 0;


						break;

					default:
						break;

				}
			}
		#endif // MOTOR_TEST

		#ifdef LED_DEBUG
			#ifdef LEDS
	  			toggle_led(GP_LED2);
	  			toggle_led(GP_LED1);
		  	#endif
  		#endif

		#ifdef PACKET_DEBUG
		  	#ifdef COM
		  		#ifdef UART
					receive_packet();
				#endif
		  		packet_send();
		  	#endif
		#endif // PACKET_DEBUG

		#ifdef IMU_DEBUG
			#ifdef GYRO
	  			Gyro_Update();
	  		#endif

	  		#ifdef ACCEL
	  			Accel_Update();
	  		#endif

		 	#ifdef GYRO
	  			memset(gbuffer, '\0', 15);
				UART_SendByte('\n');

				sprintf(gbuffer, "  G_X = %f\n", Gyro_GetX());
				UART_SendString(gbuffer);
				sprintf(gbuffer, " G_Y = %f\n", Gyro_GetY());
				UART_SendString(gbuffer);
				sprintf(gbuffer, " G_Z = %f\n", Gyro_GetZ());
				UART_SendString(gbuffer);

				UART_SendByte('\n');

		 	#endif

		 	#ifdef ACCEL
				memset(abuffer, '\0', 15);
				UART_SendByte('\n');

				sprintf(abuffer, "  A_X = %f\n", Accel_GetX());
				UART_SendString(abuffer);
				sprintf(abuffer, " A_Y = %f\n", Accel_GetY());
				UART_SendString(abuffer);
				sprintf(abuffer, " A_Z = %f\n", Accel_GetZ());
				UART_SendString(abuffer);

				UART_SendString("--------------------------");
		 	#endif // ACCEL

        	#endif // IMU_DEBUG

		#ifdef GPS_DEBUG
		        #ifdef GPS
		            NEO6M_GetChar();

		            if(GPS_NewDataReady()) {

					UART_SendString("-------***********-------");

		                memset(gpsbuffer, '\0', 50);
		                GPS_UpdateData();
		                struct tm time = GPS_GetTime();
		                sprintf(gpsbuffer, "T: %02d:%02d:%02d\nL: %.0f\nL: %.0f\nS: %.2f\nA: %.2f\n", time.tm_hour, time.tm_min, time.tm_sec, GPS_GetLatitude(), GPS_GetLongitude(), GPS_GetSpeed(), GPS_GetAltitude());

		                while (gpsbuffer[i] != '\0') {
		                    UART_SendByte(gpsbuffer[i]);
		                    i++;
		                }
		            }

		        #endif
	    	#endif

	 	_delay_ms(500);

		#ifdef NIRAJ_TESTING
			/*
			switch(command):
			case MANUAL {
				AttituteAdjustSetDesired(yawDesired, pitchDesired, rollDesires);
				break;
			}
			case GO_TO_LOCATION {
				calculatePath();
				// might want to stabalize before changing
				AttituteAdjustSetDesired(yawDesired, pitchDesired, rollDesires);
				break;
			}
			case GO_HOME {
				break;
			}
			default {
				break;
			}
			*/

			if (system_ticked() == TRUE) {
				#ifdef CAM
					Cam_Update();
				#endif
				Gyro_Update();
				Accel_Update();

				/*
				if (GPS_NewDataReady()) {
					memset(gpsbuffer, '\0', 50);
					GPS_UpdateData();
					struct tm time = GPS_GetTime();
					sprintf(gpsbuffer, "T: %02d:%02d:%02d\nL: %.0f\nL: %.0f\nS: %.2f\nA: %.2f\n", time.tm_hour, time.tm_min, time.tm_sec, GPS_GetLatitude(), GPS_GetLongitude(), GPS_GetSpeed(), GPS_GetAltitude());
				}
				*/

				/*
				// Not too sure how packets will be parsed
				if (xbee ready) {
					getPacketXbee();
					processPacket();
				}
				*/


				// Update PID
				//double ypr[3];
				//imu2euler(ypr, Accel_GetX(), Accel_GetY(), Accel_GetZ(), Mag_GetX(), Mag_GetY());
				//sprintf(testing, "Y: %f, P:%f, R:%f\n", ypr[0], ypr[1], ypr[2]);
				UART_SendString("hello\n");
				//AttituteAdjustUpdatePID(ypr[0], ypr[1], ypr[2]);

				/*
				// Update Motors
				int16_t* motor_delta = AttitudeAdjustGetActuation();
				motor_set(MOTOR_ONE, motor_get_speed(MOTOR_ONE) + motor_delta[0]);
				motor_set(MOTOR_TWO, motor_get_speed(MOTOR_TW0) + motor_delta[1]);
				motor_set(MOTOR_THREE, motor_get_speed(MOTOR_ONE) + motor_delta[2]);
				motor_set(MOTOR_FOUR, motor_get_speed(MOTOR_ONE) + motor_delta[3]);

				sendPacket();
				*/

	  			toggle_led(SYSTEM_LED);
				system_untick();
			}
			#endif		// NIRAJ STUFF

		}

	}
