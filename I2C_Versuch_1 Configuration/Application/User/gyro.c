/*
 * gyro.c
 *
 *  Created on: 25.10.2015
 *      Author: Michael Flach
 */
#include "gyro.h"
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/*@brief
 *This function returns the I2C address of the MAX21002 device
 *@date: 	05.01.2016
 *@return: 	uint8_t *sent_adress
 *@param: 	I2C_HandleTypeDef interface, uint8_t Adresse
 */
uint8_t gyro_who_I_am (I2C_HandleTypeDef *interface, uint8_t Adresse)
{
	uint8_t *sent_adress;
	//IsDeviceReady/HAL_I2C_Mem_Write Configuration
	uint32_t Trials  = 4;     //no. of Trials
	uint32_t Timeout = 100;   //ms
    ////////////////////////////////////////////////////////
	HAL_I2C_Mem_Read(interface, Adresse, 32, 1, &sent_adress,Trials, Timeout);
	return *sent_adress;
}

/*@brief
 *This function initiates the gyroscope MAX21002 and defines the operation Mode,
 *as described within the DATASHEET for the MAX21002.
 * return communikation ok, when Sensor was ready to receive Data
 *@date: 	05.01.2016
 *@return: 	uint8_t communication_ok
 *@param: 	I2C_HandleTypeDef interface, uint8_t Adresse, uint8_t current_range, uint8_t *changed_range, int16_t last_x_value,
			int16_t last_y_value
 */
uint8_t gyro_init(I2C_HandleTypeDef *interface, uint8_t Adresse,
		uint8_t current_range, uint8_t *changed_range, int16_t last_x_value,
		int16_t last_y_value, int8_t bandwidth, int8_t POWER_MODE,
		int8_t ODR_RATE) {
	////////////////////////////////////////////////////////
	///Function Declarations
	// Initial Register Configuration
	// Select Register Bank (0x21) to read
	// For this the following steps were performed
	////////////////////////////////////////////////////////
	uint8_t communication_ok = 0;
	int32_t POWER_CFG_DATA   = 0;
	int32_t SENSE_CFG1_DATA  = 0;
	int32_t SENSE_CFG2_DATA  = 0;
	uint8_t changed_range_t  = 0;
	// Eco Mode!!!
	gyro_set_configuration(last_x_value, last_y_value, &POWER_CFG_DATA,
			&SENSE_CFG1_DATA, &SENSE_CFG2_DATA, current_range, &changed_range_t,
			POWER_MODE, bandwidth, ODR_RATE);
	////////////////////////////////////////////////////////
	*changed_range = changed_range_t;

	// Select Bank 0(0x00) (User Bank)

	uint16_t DATA_1_ADRESS = BANK_SELECT
	;
	uint8_t DATA_1_CONFIG  = USER_BANK
	;
	uint16_t DATA_1_DATA_S = 1;
	// Set the Power Mode to POWER_DOWN_MODE in order to save energy during configuration
	uint16_t DATA_2_ADRESS = POWER_CFG
	;
	uint8_t DATA_2_CONFIG  = POWER_DOWN_MODE;
	uint16_t DATA_2_DATA_S = 1;
	// Set the the Bandwidth of the Low pass filter
	uint16_t DATA_3_ADRESS = SENSE_CFG1
	;
	uint8_t DATA_3_CONFIG  = SENSE_CFG1_DATA;
	uint16_t DATA_3_DATA_S = 1;
	// Select the Datarate of the ODR to 5 kHz
	uint16_t DATA_4_ADRESS = SENSE_CFG2
	;
	uint8_t DATA_4_CONFIG  = SENSE_CFG2_DATA;
	uint16_t DATA_4_DATA_S = 1;
	// Get back to Normal operation Mode in order to receive data
	uint16_t DATA_5_ADRESS = POWER_CFG
	;
	uint8_t DATA_5_CONFIG  = POWER_CFG_DATA;
	uint16_t DATA_5_DATA_S = 1;
	////////////////////////////////////////////////////////
	//IsDeviceReady/HAL_I2C_Mem_Write Configuration
	uint32_t Trials 	   = 4;   //no. of Trials
	uint32_t Timeout 	   = 20;   //ms
	////////////////////////////////////////////////////////

	///Start Function
	//get Status until device is ready
	HAL_StatusTypeDef Status_sensor;
	Status_sensor = HAL_I2C_IsDeviceReady(interface, Adresse, Trials, Timeout);
	// Transmit the defined data
	if (Status_sensor == HAL_OK) {
		HAL_I2C_Mem_Write(interface, Adresse, DATA_1_ADRESS, DATA_1_DATA_S,
				&DATA_1_CONFIG, DATA_1_DATA_S, Timeout);
		HAL_I2C_Mem_Write(interface, Adresse, DATA_2_ADRESS, DATA_2_DATA_S,
				&DATA_2_CONFIG, DATA_2_DATA_S, Timeout);
		HAL_I2C_Mem_Write(interface, Adresse, DATA_3_ADRESS, DATA_3_DATA_S,
				&DATA_3_CONFIG, DATA_3_DATA_S, Timeout);
		HAL_I2C_Mem_Write(interface, Adresse, DATA_4_ADRESS, DATA_4_DATA_S,
				&DATA_4_CONFIG, DATA_4_DATA_S, Timeout);
		HAL_I2C_Mem_Write(interface, Adresse, DATA_5_ADRESS, DATA_5_DATA_S,
				&DATA_5_CONFIG, DATA_5_DATA_S, Timeout);
		HAL_Delay(20);
		communication_ok = 1;
	} else {
		communication_ok = 0;
	}
	return communication_ok;
	////////////////////////////////////////////////////////
}
/*@brief
 *This function initiates the FIFO of the MAX21002 and defines the operation Mode,
 *as described within the DATASHEET for the MAX21002.
 *Furthermore it defines the FIFOs threshold.
 *@date: 	05.01.2016
 *@return: 	void
 *@param: 	I2C_HandleTypeDef interface, uint8_t Adresse,
			uint8_t FIFO_threshold
 */
void gyro_init_fifo(I2C_HandleTypeDef *interface, uint8_t Adresse,
		uint8_t FIFO_threshold) {
	////////////////////////////////////////////////////////
	///Function Declarations
	// Initial Register Configuration
	// For this the following steps were performed
	////////////////////////////////////////////////////////

	// Set the FIFO threshold (no. of stored datasets
	uint16_t DATA_1_ADRESS =  FIFO_TH;
	uint8_t  DATA_1_CONFIG =  FIFO_threshold;
	uint16_t DATA_1_DATA_S =  1;
	// Switch to FIFO Normal Mode
	uint16_t DATA_2_ADRESS =  FIFO_CFG;
	uint8_t  DATA_2_CONFIG =  FIFO_NORMAL_MODE;
	uint16_t DATA_2_DATA_S =  1;
	////////////////////////////////////////////////////////
	//IsDeviceReady/HAL_I2C_Mem_Write Configuration
	uint32_t Trials        =  4;  //no. of Trials
	uint32_t Timeout       =  10; //ms
	////////////////////////////////////////////////////////
	// Start Function
	// get Status until device is ready
	HAL_StatusTypeDef Status_sensor;
    Status_sensor 	       = HAL_I2C_IsDeviceReady(interface, Adresse, Trials, Timeout);
	// Transmit the defined data
	HAL_I2C_Mem_Write(interface, Adresse, DATA_1_ADRESS, DATA_1_DATA_S,
			&DATA_1_CONFIG, DATA_1_DATA_S, Timeout);
	HAL_I2C_Mem_Write(interface, Adresse, DATA_2_ADRESS, DATA_2_DATA_S,
			&DATA_2_CONFIG, DATA_2_DATA_S, Timeout);
	HAL_Delay(2);
	////////////////////////////////////////////////////////
}

/*@brief
 *This function gets the angular velocity vector within the carthesian coordinate frame.
 *The x- and y-value is given without any unit as value from -30000 to 30000, please use the get angular_velocity function in order to get a proper value
 *@date: 	22.10.2015
 *@return: 	int16_t *x_value, int16_t *y_value, int16_t *z_value
 *@param: 	I2C_HandleTypeDef interface, uint8_t Adresse, uint8_t no_of_tries
 */
uint8_t gyro_get_gyro_vector(I2C_HandleTypeDef *interface, uint8_t Adresse,
		int16_t *x_value, int16_t *y_value, int16_t *z_value,
		uint8_t no_of_tries) {
	////////////////////////////////////////////////////////
	///Function Declarations
	//Declarations for the IsDeviceReady function
	uint32_t Timeout = 2; //ms
	//Register Declarations
	uint16_t size_of_data = 1; //Size of Data
	// define values, which are needed for further calculations
	uint8_t xl, xh, yl, yh;
	uint16_t x_value_t, y_value_t;
	uint8_t data_ready = 0;
	////////////////////////////////////////////////////////
	// Register Adress of the MSB and LSB of the gyro axis velocity and Data ready Bit
	////////////////////////////////////////////////////////
	uint16_t DATA_READY = REG_DATA_READY_BIT
	;
	uint16_t X_MSB_REGISTER = GYRO_X_H
	;
	uint16_t X_LSB_REGISTER = GYRO_X_L
	;
	uint16_t Y_MSB_REGISTER = GYRO_Y_H
	;
	uint16_t Y_LSB_REGISTER = GYRO_Y_L
	;
	////////////////////////////////////////////////////////
	uint8_t try = 0;
	////////////////////////////////////////////////////////
	// Start Function
	////////////////////////////////////////////////////////
	// check whether data is available
	while ((data_ready == 0) && (no_of_tries > try)) {
		HAL_I2C_Mem_Read(interface, Adresse, DATA_READY, 1, &data_ready, 1,
				Timeout);
		try++;
	}
	////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////
	// read out the MSB and LSB of every axis

	if (data_ready != 0) {
		HAL_I2C_Mem_Read(interface, Adresse, X_LSB_REGISTER, size_of_data, &xl,
				size_of_data, Timeout);
		HAL_I2C_Mem_Read(interface, Adresse, X_MSB_REGISTER, size_of_data, &xh,
				size_of_data, Timeout);
		HAL_I2C_Mem_Read(interface, Adresse, Y_LSB_REGISTER, size_of_data, &yl,
				size_of_data, Timeout);
		HAL_I2C_Mem_Read(interface, Adresse, Y_MSB_REGISTER, size_of_data, &yh,
				size_of_data, Timeout);
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		// Merge MSB and LSB to unsigned Integer value
		x_value_t = ((uint16_t) (xl | (xh << 8)));
		y_value_t = ((uint16_t) (yl | (yh << 8)));
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		/// Return x,y,z values of the velocity vector
		*x_value = (int16_t) x_value_t;
		*y_value = (int16_t) y_value_t;
		////////////////////////////////////////////////////////
		return 1;
	} else {
		*x_value = 0;
		*y_value = 0;
		return 0;
	}
}

/*@brief
 *This function resumes the gyro back to normal mode.
 *@date: 	22.10.2015
 *@return: 	void
 *@param: 	I2C_HandleTypeDef interface, uint8_t Adresse
 */
void gyro_resume_to_normal(I2C_HandleTypeDef *interface, uint8_t Adresse) {
	////////////////////////////////////////////////////////
	///Function Declarations
	// Initial Register Configuration
	// Read Data via INT1
	// For this the following steps were performed
	// Set Auto_MRST__EN Mode in CTRL_REG 2(0x11)
	uint16_t DATA_1_ADRESS = 0x00;
	uint8_t  DATA_1_CONFIG = 0x17;
	uint16_t DATA_1_DATA_S = 1;
	// Set Auto_MRST__EN Mode in CTRL_REG 2(0x11)
	uint16_t DATA_2_ADRESS = 0x00;
	uint8_t  DATA_2_CONFIG = 0x0F;
	uint16_t DATA_2_DATA_S = 1;
	// Set Auto_MRST__EN Mode in CTRL_REG 2(0x11)
	uint16_t DATA_3_ADRESS = 0x00;
	uint8_t  DATA_3_CONFIG = 0x00;
	uint16_t DATA_3_DATA_S = 1;
	////////////////////////////////////////////////////////
	//IsDeviceReady/HAL_I2C_Mem_Write Configuration
	uint32_t Timeout       = 10;  //ms
	////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////
	///Start Function
	//get Status until device is ready
	//HAL_StatusTypeDef Status_sensor;
	//Status_sensor = HAL_I2C_IsDeviceReady(&interface, Adresse, Trials, Timeout);
	////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////
	// Transmit the defined data
	HAL_I2C_Mem_Write(interface, Adresse, DATA_1_ADRESS, DATA_1_DATA_S,
			&DATA_1_CONFIG, DATA_1_DATA_S, Timeout);
	HAL_I2C_Mem_Write(interface, Adresse, DATA_2_ADRESS, DATA_2_DATA_S,
			&DATA_2_CONFIG, DATA_2_DATA_S, Timeout);
	HAL_I2C_Mem_Write(interface, Adresse, DATA_3_ADRESS, DATA_3_DATA_S,
			&DATA_3_CONFIG, DATA_3_DATA_S, Timeout);
	////////////////////////////////////////////////////////
}

/*@brief
 *Returns the angular velocity of the gyro in [deg/s], based on the earlier set measurement range.
 *@date: 	22.10.2015
 *@return: 	float *x_velocity, float *y_velocity
 *@param: 	int16_t x_output, int16_t y_output, int8_t range
 */
void gyro_get_angular_velocity(int16_t x_output, int16_t y_output, int8_t range,
		double *x_velocity, double *y_velocity) {
	// get transformation value out of the measurement range
	// for x-values
	double transformation=1;
	switch (range) {
	case 1:
		transformation = 0.0010416;
		break;
	case 2:
		transformation = 0.0020833;
		break;
	case 3:
		transformation = 0.0040000;
		break;
	case 4:
		transformation = 0.0080000;
		break;
	case 5:
		transformation = 0.0166666;
		break;
	case 6:
		transformation = 0.0333333;
	}
	double x_output_float=(double)x_output;
	double y_output_float=(double)y_output;
	// calculate the velocity out of the transformation and the raw data from the gyro
	*x_velocity = x_output_float * transformation;
	*y_velocity = y_output_float * transformation;
}


/*@brief
 *Decides whether the measurement range has to be changed or no.
 * change_range_yes_no = 0 -->no
 * change_range_yes_no = 1 -->yes
 *@date: 	22.10.2015
 *@return: 	uint8_t change_range_yes_no
 *@param: 	int16_t last_x_value,int16_t last_y_value, uint8_t current_range
 */
uint8_t gyro_change_measurement_range(int16_t last_x_value,
		int16_t last_y_value, uint8_t current_range) {
	// default maximum value
	int16_t maximum_value = 0;
	// default return value
	uint8_t change_range_yes_no = 0;
	// get the abs value
	last_x_value = abs(last_x_value);
	last_y_value = abs(last_y_value);
// determine the maximum value of the x and y range
	if (last_x_value >= last_y_value) {
		maximum_value = last_x_value;
	} else {
		maximum_value = last_y_value;
	}
// rise the range, when the maximum value reaches the last 20 percent of the total measurement range
	if ((maximum_value > 24000) && (current_range < 6)) {
		change_range_yes_no = 1;
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	}
	// lower the range, when the maximum value is lower than the first 20 percent of the total measurement range
	else {
		if ((maximum_value < 6000) && (current_range > 1)) {
			change_range_yes_no = 1;
		} else {
			change_range_yes_no = 0;
		}
	}
	return change_range_yes_no;
}

/*@brief
 *This function sets the measurement range of the gyro MAX21002 and does some crazy configurations.
 *The x_value and y_value is defined as the last raw data input from the gyro.
 *The function determined the measurement range automatically, by setting the POWER_CFG and SENSE_CFG1 Register.
 *There are the following options for the POWER_MODE(pwr_mode):
 *1:"Power down mode" (no measurements are taken, minimum power consumption)
 *2:"Normal mode"     (nominal power consumption, low noise measurement)
 *3:"Standby mode"    (reduce power consumption, lower turn on time than the Power down mode, no data generation)
 *4:"Eco mode"        (low power consumption, higher noise level than normal mode)
 *The following could be set for the bandwidth value
 *1 : 2  Hz
 *2 : 4  Hz
 *3 : 6  Hz
 *4 : 8  Hz
 *5 : 10 Hz
 *6 : 14 Hz
 *7 : 22 Hz
 *8 : 32 Hz
 *9 : 50 Hz
 *10 :75 Hz
 *11: 100Hz
 *12: 150Hz
 *13: 200Hz
 *14: 250Hz
 *15: 300Hz
 *16: 400Hz
 *For the Operational Data (ODR_RATE value) rate value in normal Mode
 *1: 5  KHz
 *2:100  Hz
 *3:25	 Hz
 *4:10   Hz
 * And in ECO Mode
 *5: 200 Hz
 *6: 100 Hz
 *7:  50 Hz
 *8:  25 Hz
 *see MAX21002 Datasheet page 45 for further explanation!
 *@date: 22.10.2015
 *@return: int16_t *x_value, int16_t *y_value, int16_t *z_value
 *@param: int16_t x_value, int16_t y_value,
		uint8_t current_range, int8_t POWER_MODE, int8_t bandwidth,int8_t ODR_RATE
 */
void gyro_set_configuration(int16_t x_value, int16_t y_value,
		int32_t *POWER_CFG_DATA, int32_t *SENSE_CFG1_DATA,int32_t *SENSE_CFG2_DATA,
		uint8_t current_range, uint8_t *changed_range, int8_t POWER_MODE, int8_t bandwidth,int8_t ODR_RATE) {
	int16_t maximum_value = 1;

	x_value=abs(x_value);
	y_value=abs(y_value);
	// determine the maximum value of the x and y range
	if (x_value >= y_value) {
		maximum_value = x_value;
	}
	else
	{
		maximum_value = y_value;
	}

	uint8_t changed_range_t=current_range;
	// rise the range, when the maximum value reaches the last 20 percent of the total measurement range
	if ((maximum_value > 24000) && (changed_range_t < 6)) {
		changed_range_t = changed_range_t + 1;
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
	}
	// lower the range, when the maximum value is lower than the first 20 percent of the total measurement range
	if ((maximum_value < 6000) && (changed_range_t > 1)) {
		changed_range_t = changed_range_t - 1;
	}
	else
	{
		changed_range_t = changed_range_t+0;
	}
	// changing the power mode
	// first clear the POWER_CFG data.
	int32_t POWER_CFG_DATA_t = 0;
	// Enable the measurement of the x and y axis.
	POWER_CFG_DATA_t = POWER_CFG_DATA_t + 7;
	// write the POWER_MODE to the register.
	// b000000 for Power down mode
	// b001000 for Normal mode
	// b011000 for Eco mode
	// b010000 for Standby mode
	switch (POWER_MODE) {
	case 1:
		POWER_CFG_DATA_t = POWER_CFG_DATA_t + 0;
		break;
	case 2:
		POWER_CFG_DATA_t = POWER_CFG_DATA_t + 8;
		break;
	case 3:
		POWER_CFG_DATA_t = POWER_CFG_DATA_t + 16;
		break;
	case 4:
		POWER_CFG_DATA_t = POWER_CFG_DATA_t + 24;
	}
	// Configure the SENSE_CFG2 mode in order to change the operational Data rate
	int32_t SENSE_CFG_2_t =0;
	switch (ODR_RATE) {
	case 1:
		SENSE_CFG_2_t = ODR_5_KHZ;
		break;
	case 2:
		SENSE_CFG_2_t = ODR_100_HZ;
		break;
	case 3:
		SENSE_CFG_2_t = ODR_25_HZ;
		break;
	case 4:
		SENSE_CFG_2_t = ODR_10_HZ;
		break;
	case 5:
		SENSE_CFG_2_t = ECO_ODR_200_HZ;
		break;
	case 6:
		SENSE_CFG_2_t = ECO_ODR_100_HZ;
		break;
	case 7:
		SENSE_CFG_2_t = ECO_ODR_50_HZ;
		break;
	case 8:
		SENSE_CFG_2_t = ECO_ODR_25_HZ;
	}
	// Set the Bandwidth of the gyro and define the low pass filter
	uint32_t Bandwidth_LP=1;
	switch (bandwidth) {
	case 1:
		Bandwidth_LP = BW_LP_2_HZ;
		break;
	case 2:
		Bandwidth_LP = BW_LP_4_HZ;
		break;
	case 3:
		Bandwidth_LP = BW_LP_6_HZ;
		break;
	case 4:
		Bandwidth_LP = BW_LP_8_HZ;
		break;
	case 5:
		Bandwidth_LP = BW_LP_10_HZ;
		break;
	case 6:
		Bandwidth_LP = BW_LP_14_HZ;
		break;
	case 7:
		Bandwidth_LP = BW_LP_22_HZ;
		break;
	case 8:
		Bandwidth_LP = BW_LP_32_HZ;
		break;
	case 9:
		Bandwidth_LP = BW_LP_50_HZ;
		break;
	case 10:
		Bandwidth_LP = BW_LP_75_HZ;
		break;
	case 11:
		Bandwidth_LP = BW_LP_100_HZ;
		break;
	case 12:
		Bandwidth_LP = BW_LP_150_HZ;
		break;
	case 13:
		Bandwidth_LP = BW_LP_200_HZ;
		break;
	case 14:
		Bandwidth_LP = BW_LP_250_HZ;
		break;
	case 15:
		Bandwidth_LP = BW_LP_300_HZ;
		break;
	case 16:
		Bandwidth_LP = BW_LP_400_HZ;
	}
	// define the ODR
	// write the measurement range to the register
	// first Power CFG register
	// b0000000 for 1000   dps
	// b0100000 for  500   dps
	// b1000000 for  250   dps
	// b1100000 for  125   dps
	// b0100000 for  62,5  dps
	// b1000000 for  31,25 dps
	uint32_t SENSE_CFG1_DATA_t = 0;
	// second write to SENCE_CFG1 Register
	// d17 for 31,25 dps to 62,5 dps, no test data, and Low pass bandwidth value of Bandwidth_LP.
	// d16 for 125   dps to 1000 dps, no test data, and Low pass bandwidth value of Bandwidth_LP.
	switch (changed_range_t) {
	case 6:
		POWER_CFG_DATA_t = POWER_CFG_DATA_t + 0;
		SENSE_CFG1_DATA_t = Bandwidth_LP;
		break;
	case 5:
		POWER_CFG_DATA_t = POWER_CFG_DATA_t+ 64;
		SENSE_CFG1_DATA_t = Bandwidth_LP;
		break;
	case 4:
		POWER_CFG_DATA_t = POWER_CFG_DATA_t + 128;
		SENSE_CFG1_DATA_t = Bandwidth_LP;
		break;
	case 3:
		POWER_CFG_DATA_t = POWER_CFG_DATA_t + 192;
		SENSE_CFG1_DATA_t = Bandwidth_LP;
		break;
	case 2:
		POWER_CFG_DATA_t = POWER_CFG_DATA_t + 64;
		SENSE_CFG1_DATA_t = Bandwidth_LP+1;
		break;
	case 1:
		POWER_CFG_DATA_t = POWER_CFG_DATA_t + 128;
		SENSE_CFG1_DATA_t = Bandwidth_LP+1;
	}

	*changed_range=changed_range_t;
	*POWER_CFG_DATA =POWER_CFG_DATA_t;
	*SENSE_CFG1_DATA=SENSE_CFG1_DATA_t;
	*SENSE_CFG2_DATA=SENSE_CFG_2_t;

}
/*@brief
 *Returns the uncalibrated temperature of the gyroscope
 *@date: 	22.10.2015
 *@return: 	int16_t *tempout
 *@param: 	I2C_HandleTypeDef *interface, uint8_t Adresse
 */
void gyro_get_temperature(I2C_HandleTypeDef *interface, uint8_t Adresse,int16_t *tempout)
{
	    ////////////////////////////////////////////////////////
		///Function Declarations
		//Declarations for the IsDeviceReady function
		uint32_t Timeout		= 10;  //ms
		//Register Declarations
		uint16_t size_of_data 	= 1;              	          //Size of Data
		// define values, which are needed for further calculations
		uint8_t tl,th;
		uint16_t temp_value_t;
		uint8_t data_ready 		= 0;
		////////////////////////////////////////////////////////
		// Register Adress of the MSB and LSB of the gyro axis velocity and Data ready Bit
		////////////////////////////////////////////////////////
		uint16_t DATA_READY 		= REG_DATA_READY_BIT
		;
		uint16_t TEMP_MSB_REGISTER 	= TEMP_H;
		;
		uint16_t TEMP_LSB_REGISTER 	= TEMP_L;
		;
		////////////////////////////////////////////////////////
		// Start Function
		////////////////////////////////////////////////////////
		// check whether data is available
		while (data_ready == 0) {
			HAL_I2C_Mem_Read(interface, Adresse, DATA_READY, 1, &data_ready, 1,
					10);
		}
		// read out the MSB and LASB of every axis

		HAL_I2C_Mem_Read(interface, Adresse, TEMP_LSB_REGISTER, size_of_data, &tl,
				size_of_data, Timeout);
		HAL_I2C_Mem_Read(interface, Adresse, TEMP_MSB_REGISTER, size_of_data, &th,
				size_of_data, Timeout);

		// Merge MSB and LSB to unsigned Integer value
		// Transform the Bit value into B Value
		temp_value_t = ((uint16_t) (tl | (th << 8)));
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		/// Return the Temperature..cool or hoooot
		*tempout 	 = (int16_t) temp_value_t;
		////////////////////////////////////////////////////////
}

/*@brief
 *Returns the values, which are needed to get the right calibrated gyro output
 *You just need the right range and sensor identification
 *@date: 	22.10.2015
 *@return: 	double *a_11,
		double *a_12, double *a_13, double *a_21, double *a_22, double *a_23,
		double *a_31, double *a_32, double *a_33, double *s_x, double *s_y,
		double *w_x, double *w_y
 *@param: 	uint8_t sensor_id, int8_t range
 */
void gyro_get_calibration_data(uint8_t sensor_id, int8_t range, double *a_11,
		double *a_12, double *a_13, double *a_21, double *a_22, double *a_23,
		double *a_31, double *a_32, double *a_33, double *s_x, double *s_y,
		double *w_x, double *w_y) {
	// First get the data for the Offset and scale factor
	*s_x = CALIBRATION_GYRO_offset_scale[range-1][sensor_id-1][0];
	*s_y = CALIBRATION_GYRO_offset_scale[range-1][sensor_id-1][1];
	*w_x = CALIBRATION_GYRO_offset_scale[range-1][sensor_id-1][2];
	*w_y = CALIBRATION_GYRO_offset_scale[range-1][sensor_id-1][3];
	// Second get the data for the misalignment
	*a_11 = CALIBRATION_GYRO_misalignment[sensor_id-1][0];
	*a_12 = CALIBRATION_GYRO_misalignment[sensor_id-1][1];
	*a_13 = CALIBRATION_GYRO_misalignment[sensor_id-1][2];
	*a_21 = CALIBRATION_GYRO_misalignment[sensor_id-1][3];
	*a_22 = CALIBRATION_GYRO_misalignment[sensor_id-1][4];
	*a_23 = CALIBRATION_GYRO_misalignment[sensor_id-1][5];
	*a_31 = CALIBRATION_GYRO_misalignment[sensor_id-1][6];
	*a_32 = CALIBRATION_GYRO_misalignment[sensor_id-1][7];
	*a_33 = CALIBRATION_GYRO_misalignment[sensor_id-1][8];
}
/*@brief
 *Returns the values, which are needed to get the right calibrated gyro output
 *You just need the right range and sensor identification
 *@date: 	22.10.2015
 *@return: 	double *a_11,
		double *a_12, double *a_13, double *a_21, double *a_22, double *a_23,
		double *a_31, double *a_32, double *a_33, double *s_x, double *s_y,
		double *w_x, double *w_y
 *@param: 	uint8_t sensor_id, int8_t range
 */

void gyro_get_scale_offset_mis_cal(uint8_t sensor_id,int8_t range, double x_value_raw,
		double y_value_raw, double *x_value_calibrated,
		double *y_value_calibrated,double *z_value_calibrated) {
	// define values, which are needed for further calculations
	// misalignment factors
	double a_11, a_12, a_13, a_21, a_22, a_23, a_31, a_32, a_33;
	// Scale factors
	double s_x, s_y;
	// Zero Offset
	double w_x_o, w_y_o;
	// those values are just needed in between the calculations
	double w_x_t,w_y_t;
	// get the calibration values for a defines sensor id
	gyro_get_calibration_data(sensor_id,range, &a_11, &a_12, &a_13, &a_21, &a_22,
	&a_23, &a_31, &a_32, &a_33, &s_x, &s_y, &w_x_o, &w_y_o);
	// calibrate the scale factor and offset of every axis
	w_x_t = s_x*(x_value_raw-w_x_o);
	w_y_t = s_y*(y_value_raw-w_y_o);
	// algebraic methode and therefore compensation of misalignment
	*x_value_calibrated=a_11*w_x_t+a_12*w_y_t;
	*y_value_calibrated=a_21*w_x_t+a_22*w_y_t;
	*z_value_calibrated=a_31*w_x_t+a_32*w_y_t;
}


// Some FIFO Functions, which don't work properly......sad
void gyro_get_FIFO_Data (I2C_HandleTypeDef *interface, uint8_t Adresse,uint8_t no_trials_fifo, uint8_t threshold)
{
	///Function Declarations
	//Declarations for the IsDeviceReady function
	uint32_t Trials           = 10; //no. of Trials
	uint32_t Timeout          = 10; //ms
	//Register Declarations
	uint16_t size_of_data     = 1;  //Size of Data
	// define values, which are needed for further calculations
	uint8_t  FIFO_STATUS_DATA = 0;
	uint8_t  FIFO_OVTHOLD     = 0;
	uint8_t  n				  = 0;
	/// Start Function
	//  first read the FIFO_STATUS Register for FIFO Ovthold
	uint16_t DATA_1_ADRESS    = FIFO_STATUS;
	uint16_t DATA_1_DATA_S    = 1;
	uint16_t FIFO_count_counter=0;
	// Get Data and wait until FIFO is full
	while ((FIFO_OVTHOLD==0))
	{
		n=n+1;
		HAL_I2C_Mem_Read(&interface, Adresse, DATA_1_ADRESS, 1, &FIFO_STATUS_DATA,
							1, Timeout);
		FIFO_OVTHOLD=(FIFO_STATUS_DATA&(1 << 2));
	}
	HAL_Delay(1000);
	gyro_get_fifo_count(interface,Adresse,&FIFO_count_counter);
	uint8_t datavector[255];
	gyro_get_FIFO_Data_vector(interface,Adresse,threshold,datavector);

}

void gyro_get_FIFO_Data_vector(I2C_HandleTypeDef *interface, uint8_t Adresse,
		uint8_t threshold, uint8_t *datavector[]) {
	///Function Declarations
	//Declarations for the IsDeviceReady function
	uint32_t Trials = 10; //no. of Trials
	uint32_t Timeout = 10; //ms
	// define values, which are needed for further calculations
	uint8_t i;
	uint16_t cnt = 0;
	uint8_t j = 0;
	__IO uint8_t k = 0;
	__IO uint16_t p = 0;
	__IO uint8_t data_buffer[threshold];
	uint16_t FIFO_count_counter;
	__IO int16_t X_Vector[threshold];
	__IO int16_t Y_Vector[threshold];
	/// Start Function
	//  first read the FIFO_STATUS Register for FIFO Ovthold
	uint16_t DATA_1_ADRESS = FIFO_DATA
	;
	uint16_t DATA_1_DATA_S = 1;

	//for (k=0;k<threshold;k++)
	//{
	//data_buffer[k]=k;
	//}
	// Get Data and wait until FIFO is full
//	for (i=0;i<threshold;i++)
//	{
//		uint16_t current_data=0;
//		HAL_I2C_Mem_Read(&interface, Adresse, DATA_1_ADRESS, 1, &data_buffer[i],250,4);
//		//data_buffer[i]=current_data;
//		//j=j+1;
//	}
	HAL_I2C_Mem_Read(&interface, Adresse, DATA_1_ADRESS, 0, data_buffer, 155,
			10);

	for (k = 59; k < threshold; k += 6) {
		X_Vector[p] = (int16_t)(data_buffer[k] << 8) | (data_buffer[k + 1]);
		Y_Vector[p] = (int16_t)(data_buffer[k + 2] << 8) | (data_buffer[k + 3]);
		p++;
	}
	p = 0;

	//X_Vector[p] =(int16_t)(data_buffer[cnt]<<8)||(data_buffer[cnt+1]);
	//	Y_Vector[p] =(int16_t)(data_buffer[cnt+2]<<8)||(data_buffer[cnt+3]);

	//int16_t x_value,y_value;
	//gyro_get_fifo_x_y_value(data_buffer,threshold,&x_value,&y_value);

}

void gyro_get_fifo_x_y_value(uint8_t data_buffer[], uint8_t threshold,
		int16_t x_value, int16_t y_value) {
	uint16_t p = 0;
	uint16_t k = 0;
	int16_t X_Vector[threshold];
	int16_t Y_Vector[threshold];
	for (k = 0; k < threshold; k++) {
		X_Vector[p] = (int16_t)(data_buffer[k] << 8) || (data_buffer[k + 1]);
		Y_Vector[p] = (int16_t)(data_buffer[k + 2] << 8)
				|| (data_buffer[k + 3]);
		p++;
	}
}

void gyro_get_fifo_count(I2C_HandleTypeDef *interface, uint8_t Adresse,uint16_t *fifo_c)
{
		///Function Declarations
		//Declarations for the IsDeviceReady function
		uint32_t Trials           = 10; //no. of Trials
		uint32_t Timeout          = 10; //ms
		// define values, which are needed for further calculations
		uint16_t FIFO_count_t	  = 0;
		/// Start Function
		//  first read the FIFO_STATUS Register for FIFO Ovthold
		uint16_t DATA_1_ADRESS    = FIFO_COUNT;
		uint16_t DATA_1_DATA_S    = 1;

		HAL_I2C_Mem_Read(&interface, Adresse, DATA_1_ADRESS, 1, &FIFO_count_t,1,Timeout);
		*fifo_c=FIFO_count_t;
}




