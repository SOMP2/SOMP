/*
 * MAG_3110.h
 *
 *  Created on: 25.10.2015
 *      Author: Michael Flach
 */

#ifndef APPLICATION_USER_MAG_3110_H_
#define APPLICATION_USER_MAG_3110_H_
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

////////////////////////////////////////////////////////
// Calibration Values
////////////////////////////////////////////////////////
const double CALIBRATION_MAG_HARD_SOFT_IRON[8][16] =
{		{-51.0495,110.4660,67.6775,0.6058,0.2858,-0.7424,-0.7924,0.1343,-0.5950,-0.0704,0.9488,0.3079,0.0235,0.0241,0.0252,40.00},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};



/*@brief
 *This function initiates the magnetometer MAG 3110 and defines the operation Mode,
 *as described within the DATASHEET for the Freescale MAG 3110.
 *@date: 22.10.2015
 *@return: void
 *@param: I2C_HandleTypeDef interface
 *@param: uint8_t Adresse
 */
void init_magnetometer(I2C_HandleTypeDef interface, uint8_t Adresse);
/*@brief
 *This function returns the current temperature measured at the temperature reference point of the magnetometer.
 *Hint: The offset correction has not been implemented yet.
 *@date: 22.10.2015
 *@return: int16_t *temp_value
 *@param: I2C_HandleTypeDef interface
 *@param: uint8_t Adresse
 */
void get_magnetometer_temp(I2C_HandleTypeDef interface, uint8_t Adresse, int16_t *temp_value);
/*@brief
 *This function gets the magnetic field vector within the carthesian coordinate frame.
 *@date: 22.10.2015
 *@return: int16_t *x_value
 *@return: int16_t *y_value
 *@return: int16_t *z_value
 *@param: I2C_HandleTypeDef interface
 *@param: uint8_t Adresse
 */
void get_magnetometer_magnetic_field_vector(I2C_HandleTypeDef interface, uint8_t Adresse,
		int16_t *x_value, int16_t *y_value, int16_t *z_value);

void get_mag_calibration_data_hard_soft_iron(uint8_t sensor_id, double *r_v_1,
		double *r_v_2, double *r_v_3, double *a_11_b, double *a_12_b,
		double *a_13_b, double *a_21_b, double *a_22_b, double *a_23_b,
		double *a_31_b, double *a_32_b, double *a_33_b, double *sqrt_u1,
		double *sqrt_u2, double *sqrt_u3, double *r_abs);

void get_calibrate_hard_soft_iron(uint8_t sensor_id, double x_non_calibrated,
		double y_non_calibrated, double z_non_calibrated,
		double *x_calibrated_normalize_soft_iron,
		double *y_calibrated_normalize_soft_iron,
		double *z_calibrated_normalize_soft_iron);
void get_calibrate_misalignment(uint8_t sensor_id);

#endif /* APPLICATION_USER_MAG_3110_H_ */
