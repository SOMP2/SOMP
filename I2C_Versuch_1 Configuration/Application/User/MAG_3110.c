/*
 * MAG_3110.c
 *
 *  Created on: 25.10.2015
 *      Author: Michael Flach
 */
#include "MAG_3110.h"
#include "defines.h"
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/*@brief
 *This function initiates the magnetometer MAG 3110 and defines the operation Mode,
 *as described within the DATASHEET for the Freescale MAG 3110.
 *@date: 22.10.2015
 *@return: void
 *@param: I2C_HandleTypeDef interface, uint8_t Adresse
 */
void init_magnetometer(I2C_HandleTypeDef interface, uint8_t Adresse) {
	////////////////////////////////////////////////////////
	///Function Declarations
	// Initial Register Configuration
	// Read Data via INT1
	// For this the following steps were performed
	// Set Auto_MRST__EN Mode in CTRL_REG 2(0x11)
	uint16_t CTRL_REG_2_ADRESS=CTRL_REG2;
	uint8_t  CTRL_REG_2_CONFIG=AUTO;
	uint16_t CTRL_REG_2_DATA_S=1;
	// Set Read Data via INT1 in CTRL_REG 1(0x10)
	uint16_t CTRL_REG_1_ADRESS=CTRL_REG1;
	uint8_t  CTRL_REG_1_CONFIG=INT1_Read;
	uint16_t CTRL_REG_1_DATA_S=1;
	//IsDeviceReady/HAL_I2C_Mem_Write Configuration
	uint32_t Trials = 10;   //no. of Trials
    uint32_t Timeout= 10;   //ms
    ////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////
    ///Start Function
    //get Status until device is ready
	HAL_StatusTypeDef Status_sensor;
	Status_sensor = HAL_I2C_IsDeviceReady(&interface, Adresse, Trials, Timeout);
	// Transmit the defined data
	HAL_I2C_Mem_Write(&interface, Adresse, CTRL_REG_2_ADRESS, CTRL_REG_2_DATA_S, &CTRL_REG_2_CONFIG, CTRL_REG_2_DATA_S, Timeout);
	HAL_I2C_Mem_Write(&interface, Adresse, CTRL_REG_1_ADRESS, CTRL_REG_1_DATA_S, &CTRL_REG_1_CONFIG, CTRL_REG_1_DATA_S, Timeout);
	////////////////////////////////////////////////////////

}


/*@brief
 *This function gets the magnetic field vector within the carthesian coordinate frame.
 *@date: 22.10.2015
 *@return: int16_t *x_value, int16_t *y_value, int16_t *z_value
 *@param: I2C_HandleTypeDef interface, uint8_t Adresse
 */
void get_magnetometer_magnetic_field_vector(I2C_HandleTypeDef interface, uint8_t Adresse,
		int16_t *x_value, int16_t *y_value, int16_t *z_value) {
	////////////////////////////////////////////////////////
	///Function Declarations
	//Declaration of the maximum measurement Value

	// Hint: -20000 if offset correction is required by the sensor
	//       -30000 if no offset correction is required by the sensor
	//Declarations for the IsDeviceReady function
	uint32_t Trials = 10;                    //no. of Trials
	uint32_t Timeout = 10;                   //ms
	//Register Declarations
	uint16_t start_Register_to_read = OUT_X_MSB;  //Startregister
	uint16_t OUT_X_MSB_t= OUT_X_MSB;
	uint16_t OUT_X_LSB_t= OUT_X_LSB;
	uint16_t OUT_Y_MSB_t= OUT_Y_MSB;
	uint16_t OUT_Y_LSB_t= OUT_Y_LSB;
	uint16_t OUT_Z_MSB_t= OUT_Z_MSB;
	uint16_t OUT_Z_LSB_t= OUT_Z_LSB;

	uint16_t size_of_data = 1;              	  //Size of Data
	uint8_t x_LSB[6]= {0,0,0,0,0,0};
	uint8_t xl, xh, yl, yh, zl, zh;
	uint16_t x_value_t, y_value_t, z_value_t;
	////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////
	///Start Function
	//Wait until Device is ready to transmit data
	while (HAL_I2C_IsDeviceReady(&interface, Adresse, 3, 1))
		;
	//Request Data from Register 0x01 to 0x06
	// 0x01 MSB, 0x02 LSB of X_Value
	// 0x03 MSB, 0x04 LSB of y_Value
	// 0x05 MSB, 0x06 LSB of Z_Value
	HAL_I2C_Mem_Read(&interface, Adresse, OUT_X_MSB_t, 1,
			&x_LSB[0], 1, Timeout);
	HAL_I2C_Mem_Read(&interface, Adresse, OUT_X_LSB_t, 1,
			&x_LSB[1], 1, Timeout);
	HAL_I2C_Mem_Read(&interface, Adresse, OUT_Y_MSB_t, 1,
			&x_LSB[2], 1, Timeout);
	HAL_I2C_Mem_Read(&interface, Adresse, OUT_Y_LSB_t, 1,
			&x_LSB[3], 1, Timeout);
	HAL_I2C_Mem_Read(&interface, Adresse, OUT_Z_MSB_t, 1,
			&x_LSB[4], 1, Timeout);
	HAL_I2C_Mem_Read(&interface, Adresse, OUT_Z_LSB_t, 1,
			&x_LSB[5], 1, Timeout);
	//Define MSB and LSB of the x,y,z values
	xh = x_LSB[0];
	xl = x_LSB[1];
	yh = x_LSB[2];
	yl = x_LSB[3];
	zh = x_LSB[4];
	zl = x_LSB[5];
	//Merge MSB and LSB to unsigned Integer value
	// Transform the Bit value into B Value with micro teslas as unit
	x_value_t = ((uint16_t) (xl | (xh << 8)));
	y_value_t = ((uint16_t) (yl | (yh << 8)));
	z_value_t = ((uint16_t) (zl | (zh << 8)));
	////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////
	/// Return x,y,z values of the magnetic field vector
	*x_value = (int16_t) x_value_t ;
	*y_value = (int16_t) y_value_t ;
	*z_value = (int16_t) z_value_t ;
	////////////////////////////////////////////////////////
}

/*@brief
 *This function returns the current temperature measured at the temperature reference point of the magnetometer.
 *Hint: The offset correction has not been implemented yet.
 *@date: 22.10.2015
 *@return: int16_t *temp_value
 *@param: I2C_HandleTypeDef interface, uint8_t Adresse
 */
void get_magnetometer_temp(I2C_HandleTypeDef interface, uint8_t Adresse, int16_t *temp_value) {
	////////////////////////////////////////////////////////
	///Function Declarations
	//Declarations for the IsDeviceReady function
	uint32_t Trials = 10;   //no. of Trials
	uint32_t Timeout = 10;   //ms
	//Register Declarations
	uint16_t start_Register_to_read = DIE_TEMP; //Startregister
	uint16_t size_of_data = 1;              //Size of Data
	uint8_t  x_LSB[1];
	uint16_t temp_value_t;
	////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////
	///Start Function
	//Wait until Device is ready to transmit data
	while (HAL_I2C_IsDeviceReady(&interface, Adresse, Trials, Timeout))
		;
	//Request Data from Register
	// 0x0F is the unsigned value of the temperature
	HAL_I2C_Mem_Read(&interface, Adresse, start_Register_to_read, size_of_data,
			&x_LSB, Trials, Timeout);
	//x value
	temp_value_t = (uint16_t) x_LSB[0];
	////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////
	/// Return value of the temperature
	*temp_value = (int16_t) temp_value_t;
	////////////////////////////////////////////////////////
}


void get_calibrate_mag(uint8_t sensor_id,double trans, double x_non_calibrated,
		double y_non_calibrated, double z_non_calibrated,
		double *x_calibrated_full, double *y_calibrated_full,
		double *z_calibrated_full) {
	    double x_calibrated_normalize_soft_iron, y_calibrated_normalize_soft_iron,
			z_calibrated_normalize_soft_iron;

	// Transformation due to measurement scale Factor of 10
	double x_non_calibrated_t=x_non_calibrated*trans;
	double y_non_calibrated_t=y_non_calibrated*trans;
	double z_non_calibrated_t=z_non_calibrated*trans;
	// Perform Soft and Hard Iron Calibration
	get_calibrate_hard_soft_iron(sensor_id, x_non_calibrated_t, y_non_calibrated_t,
			z_non_calibrated_t, &x_calibrated_normalize_soft_iron,
			&y_calibrated_normalize_soft_iron,
			&z_calibrated_normalize_soft_iron);
    // return fully calibrated Values
	*x_calibrated_full = x_calibrated_normalize_soft_iron;
	*y_calibrated_full = y_calibrated_normalize_soft_iron;
	*z_calibrated_full = z_calibrated_normalize_soft_iron;

}



void get_mag_calibration_data_hard_soft_iron(uint8_t sensor_id, double *r_v_1,
		double *r_v_2, double *r_v_3, double *a_11_b, double *a_12_b,
		double *a_13_b, double *a_21_b, double *a_22_b, double *a_23_b,
		double *a_31_b, double *a_32_b, double *a_33_b, double *sqrt_u1,
		double *sqrt_u2, double *sqrt_u3, double *r_abs) {
    // Offset Vector
	*r_v_1 		= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][0];
	*r_v_2 		= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][1];
	*r_v_3 		= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][2];
    // Get Rotation Matrix
	*a_11_b 	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][3];
	*a_12_b 	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][4];
	*a_13_b 	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][5];
	*a_21_b 	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][6];
	*a_22_b 	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][7];
	*a_23_b 	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][8];
	*a_31_b 	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][9];
	*a_32_b 	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][10];
	*a_33_b 	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][11];
    // Get Normalization
	*sqrt_u1 	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][12];
	*sqrt_u2 	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][13];
	*sqrt_u3 	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][14];
	*r_abs  	= CALIBRATION_MAG_HARD_SOFT_IRON[sensor_id][15];
}

void get_calibrate_hard_soft_iron(uint8_t sensor_id, double x_non_calibrated,
		double y_non_calibrated, double z_non_calibrated,
		double *x_calibrated_normalize_soft_iron,
		double *y_calibrated_normalize_soft_iron,
		double *z_calibrated_normalize_soft_iron) {
	// Offset vector
	double r_v_1, r_v_2, r_v_3;
	// Rotation Matrix Hard and Soft Iron Values
	double a_11_b, a_12_b, a_13_b, a_21_b, a_22_b, a_23_b, a_31_b, a_32_b,
			a_33_b;
	// Value Definition
	double x_calibrated_movepoint_hard_iron, y_calibrated_movepoint_hard_iron,
			z_calibrated_movepoint_hard_iron;
	// Normalization
	double sqrt_u1, sqrt_u2, sqrt_u3;
	double r_abs;
	// Returns


	get_mag_calibration_data_hard_soft_iron(sensor_id, &r_v_1, &r_v_2, &r_v_3,
			&a_11_b, &a_12_b, &a_13_b, &a_21_b, &a_22_b, &a_23_b, &a_31_b,
			&a_32_b, &a_33_b, &sqrt_u1, &sqrt_u2, &sqrt_u3, &r_abs);
	x_calibrated_movepoint_hard_iron = r_v_1 + a_11_b * x_non_calibrated
			+ a_21_b * y_non_calibrated + a_31_b * z_non_calibrated;
	y_calibrated_movepoint_hard_iron = r_v_2 + a_12_b * x_non_calibrated
			+ a_22_b * y_non_calibrated + a_32_b * z_non_calibrated;
	z_calibrated_movepoint_hard_iron = r_v_3 + a_13_b * x_non_calibrated
			+ a_23_b * y_non_calibrated + a_33_b * z_non_calibrated;

	*x_calibrated_normalize_soft_iron = x_calibrated_movepoint_hard_iron
			* sqrt_u1 * r_abs;
	*y_calibrated_normalize_soft_iron = y_calibrated_movepoint_hard_iron
			* sqrt_u2 * r_abs;
	*z_calibrated_normalize_soft_iron = z_calibrated_movepoint_hard_iron
			* sqrt_u3 * r_abs;

}

void get_calibrate_misalignment(uint8_t sensor_id) {

}

