/*
 * gyro.h
 *
 *  Created on: 06.01.2016
 *      Author: Michael Flach
 */

#ifndef APPLICATION_USER_GYRO_H_
#define APPLICATION_USER_GYRO_H_
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
////////////////////////////////////////////////////////
//Registers
// See MAX21002 Datasheet for explanation
////////////////////////////////////////////////////////
#define		MAX_21002_ADRESS		0xB1;
#define     WHO				        0x20;
////////////////////////////////////////////////////////
//Output Registers for the magnetic field vector within the
//carthesian coordinate frame
////////////////////////////////////////////////////////
// Output Registers for Gyro measurements
////////////////////////////////////////////////////////
#define 	GYRO_X_H  			    0x23; //Bits [15:8] of X measurement
#define 	GYRO_X_L    			0x24; //Bits [7:0]  of X measurement
#define 	GYRO_Y_H   			    0x25; //Bits [15:8] of Y measurement
#define 	GYRO_Y_L     			0x26; //Bits [7:0]  of Y measurement
////////////////////////////////////////////////////////
// System registers
////////////////////////////////////////////////////////
#define     REG_DATA_READY_BIT      0x22;
////////////////////////////////////////////////////////
// User Bank selection
////////////////////////////////////////////////////////
#define     BANK_SELECT           	0x21;
#define     USER_BANK               0x00;
////////////////////////////////////////////////////////
// Define Powermode
////////////////////////////////////////////////////////
#define     POWER_CFG               0x00;
////////////////////////////////////////////////////////
// Options
////////////////////////////////////////////////////////
#define     NORMAL_MODE             0x0F
#define     POWER_DOWN_MODE         0x07
////////////////////////////////////////////////////////
// Define Sensingmode
////////////////////////////////////////////////////////
// Sensing Mode Register 1
// Defines the Low pass bandfilter configuration
////////////////////////////////////////////////////////
#define     SENSE_CFG1              0x01; //
////////////////////////////////////////////////////////
// Options for selftestmode off and Low pass bandfilter configuration
////////////////////////////////////////////////////////
#define     BW_LP_2_HZ             	0;
#define     BW_LP_4_HZ             	4;
#define     BW_LP_6_HZ             	8;
#define     BW_LP_8_HZ             	12;
#define     BW_LP_10_HZ             16;
#define     BW_LP_14_HZ             20;
#define     BW_LP_22_HZ             24;
#define     BW_LP_32_HZ             28;
#define     BW_LP_50_HZ             32;
#define     BW_LP_75_HZ             36;
#define     BW_LP_100_HZ            40;
#define     BW_LP_150_HZ            44;
#define     BW_LP_200_HZ            48;
#define     BW_LP_250_HZ            52;
#define     BW_LP_300_HZ            56;
#define     BW_LP_400_HZ            60;
////////////////////////////////////////////////////////
// Sensing Mode Register 2
////////////////////////////////////////////////////////
#define     SENSE_CFG2				0x02;
////////////////////////////////////////////////////////
// Options for Data output rate in Normal Mode
////////////////////////////////////////////////////////
#define     ODR_5_KHZ               0x01; // 5   kHz Output Datarate
#define		ODR_100_HZ				99;   // 100 Hz
#define     ODR_25_HZ				0x9F; // 25  Hz
#define     ODR_10_HZ               0xCC; // 10  Hz
////////////////////////////////////////////////////////
// Options for Data output rate in Eco mode
////////////////////////////////////////////////////////
#define 	ECO_ODR_200_HZ			0;
#define 	ECO_ODR_100_HZ			1;
#define 	ECO_ODR_50_HZ			2;
#define 	ECO_ODR_25_HZ			3;
// Sensing Mode Register 2
////////////////////////////////////////////////////////
#define     SENSE_CFG3              0x00; // defines Highpass band filter
////////////////////////////////////////////////////////
// Options for high pass bandfilter configuration
////////////////////////////////////////////////////////
#define     BW_HP_0_1_HZ            0x00; // 0,1 HZ Highpassfilter as default
////////////////////////////////////////////////////////
// Temperature
////////////////////////////////////////////////////////
#define     TEMP_H					0x29;
#define     TEMP_L					0x2A;
////////////////////////////////////////////////////////
// FIFO Configuration Registers
////////////////////////////////////////////////////////
#define 	FIFO_TH					0x17; // FIFO Thereshold
#define  	FIFO_CFG				0x18; // FIFO Configuration
#define   	FIFO_STATUS				0x3D; // FIFO Status Flags
////////////////////////////////////////////////////////
// FIFO Options
////////////////////////////////////////////////////////
#define     FIFO_NORMAL_MODE		0x57; // FIFO Mode = Normal
////////////////////////////////////////////////////////
// FIFO Data Registers
////////////////////////////////////////////////////////
#define     FIFO_DATA               0x3E; // FIFO DATA Register
#define     FIFO_COUNT				0x3C; // Available number of FIFO samples
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// Calibration Values
////////////////////////////////////////////////////////
// First Offset Calibration
// Calibration Data for every single sensor due to the following matrix for every measurement range
// {sensor_id,s_x,s_y,wx_offset,wy_offset}
// sensor id: Identification of the sensor
// s_x, s_y	: Scale factors of the gyro data
// w_x, w_y : offset values of the gyro
// dimention is the range
// for return just use the following routine
// CALIBRATION_GYRO_offset_scale[range-1][sensor_id-1][sx:0,sy:1,w_x=2,w_y=3]
////////////////////////////////////////////////////////
const double CALIBRATION_GYRO_offset_scale[5][8][4] =
{ { {  -1.0087, 1.011, -0.3660, -0.2781 }, 	// Sensor 1 Range 1
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 2 Range 1
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 3 Range 1
	{  1.048, 1.048, 1.67, 1.67 },	// Sensor 4 Range 1
	{  1.048, 1.048, 1.67, 1.67 },	// Sensor 5 Range 1
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 6 Range 1
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 7 Range 1
	{  1.048, 1.048, 1.67, 1.67 } },// Sensor 8 Range 1
  { {  1.04073, 2.30352, -0.54194, 1.76039 }, 	// Sensor 1 Range 2
    {  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 2 Range 2
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 3 Range 2
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 4 Range 2
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 5 Range 2
	{  1.048, 1.048, 1.67, 1.67 },	// Sensor 6 Range 2
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 7 Range 2
	{  1.048, 1.048, 1.67, 1.67 } },// Sensor 8 Range 2
  { {  1.08440, 2.39167, -0.66274, 1.67606 }, 	// Sensor 1 Range 3
    {  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 2 Range 3
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 3 Range 3
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 4 Range 3
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 5 Range 3
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 6 Range 3
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 7 Range 3
	{  1.048, 1.048, 1.67, 1.67 } },// Sensor 8 Range 3
  { {  1.08441, 2.36623, -0.62397, 1.62852 }, 	// Sensor 1 Range 4
    {  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 2 Range 4
	{  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 3 Range 4
	{  1.048, 1.048, 1.67, 1.67 },	// Sensor 4 Range 4
	{  1.048, 1.048, 1.67, 1.67 },	// Sensor 5 Range 4
	{  1.048, 1.048, 1.67, 1.67 },	// Sensor 6 Range 4
	{  1.048, 1.048, 1.67, 1.67 },	// Sensor 7 Range 4
	{  1.048, 1.048, 1.67, 1.67 } },// Sensor 8 Range 4
  { {  1.04128, 2.26058, -0.61785, 1.69774 }, 	// Sensor 1 Range 5
	{  1.048, 1.048, 1.67, 1.67 },	// Sensor 2 Range 5
	{  1.048, 1.048, 1.67, 1.67 },	// Sensor 3 Range 5
	{  1.048, 1.048, 1.67, 1.67 },	// Sensor 4 Range 5
    {  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 5 Range 5
    {  1.048, 1.048, 1.67, 1.67 },	// Sensor 6 Range 5
    {  1.048, 1.048, 1.67, 1.67 }, 	// Sensor 7 Range 5
	{  1.048, 1.048, 1.67, 1.67 }}};// Sensor 8 Range 5

// a11..a33 : Values of the rotation matrix using the algebraic methode
//			  (from measurement coordinate frame to satellite fixed coordinate frame)
//			  {a_11,a_12,a_13,a_21,a_22,a_23,a_31,a_32,a_33}
const double CALIBRATION_GYRO_misalignment			[8][9]   = {   { -0.7296,-0.6839,0,-0.6839,0.7296,0,0,0,-1.00},
																   { 1,0,0,0,1,0,0,0,1},
																   { 1,0,0,0,1,0,0,0,1},
																   { 1,0,0,0,1,0,0,0,1},
																   { 1,0,0,0,1,0,0,0,1},
																   { 1,0,0,0,1,0,0,0,1},
																   { 1,0,0,0,1,0,0,0,1},
																   { 1,0,0,0,1,0,0,0,1}};

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////

/*@brief
 *This function returns the I2C address of the MAX21002 device
 *@date: 	05.01.2016
 *@return: 	uint8_t *sent_adress
 *@param: 	I2C_HandleTypeDef interface, uint8_t Adresse
 */
uint8_t gyro_who_I_am(I2C_HandleTypeDef *interface, uint8_t Adresse);

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
		int8_t ODR_RATE);
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
		uint8_t FIFO_threshold);
/*@brief
 *This function gets the angular velocity vector within the carthesian coordinate frame.
 *The x- and y-value is given without any unit as value from -30000 to 30000
 *@date: 	22.10.2015
 *@return: 	int16_t *x_value, int16_t *y_value, int16_t *z_value
 *@param: 	I2C_HandleTypeDef interface, uint8_t Adresse, uint8_t no_of_tries
 */
uint8_t gyro_get_gyro_vector(I2C_HandleTypeDef *interface, uint8_t Adresse,
		int16_t *x_value, int16_t *y_value, int16_t *z_value, uint8_t no_of_tries);
/*@brief
 *This function resumes the gyro back to normal mode.
 *@date: 	22.10.2015
 *@return: 	void
 *@param: 	I2C_HandleTypeDef interface, uint8_t Adresse
 */
void gyro_resume_to_normal(I2C_HandleTypeDef *interface, uint8_t Adresse);
/*@brief
 *Returns the angular velocity of the gyro in [deg/s], based on the earlier set measurement range.
 *@date: 	22.10.2015
 *@return: 	float *x_velocity, float *y_velocity
 *@param: 	int16_t x_output, int16_t y_output, int8_t range
 */
void gyro_get_angular_velocity(int16_t x_output, int16_t y_output, int8_t range,
		double *x_velocity, double *y_velocity);

/*@brief
 *Decides whether the measurement range has to be changed or no.
 * change_range_yes_no = 0 -->no
 * change_range_yes_no = 1 -->yes
 *@date: 	22.10.2015
 *@return: 	uint8_t change_range_yes_no
 *@param: 	int16_t last_x_value,int16_t last_y_value, uint8_t current_range
 */
uint8_t gyro_change_measurement_range(int16_t last_x_value,
		int16_t last_y_value, uint8_t current_range);
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
		uint8_t current_range, uint8_t *changed_range, int8_t POWER_MODE, int8_t bandwidth,int8_t ODR_RATE);
/*@brief
 *Returns the uncalibrated temperature of the gyroscope
 *@date: 	22.10.2015
 *@return: 	int16_t *tempout
 *@param: 	I2C_HandleTypeDef *interface, uint8_t Adresse
 */
void gyro_get_temperature(I2C_HandleTypeDef *interface, uint8_t Adresse,
		int16_t *tempout);
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
		double *w_x, double *w_y);
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
		double *y_value_calibrated,double *z_value_calibrated);

void gyro_get_FIFO_Data_vector(I2C_HandleTypeDef *interface, uint8_t Adresse,
		uint8_t threshold, uint8_t *datavector[]);
void gyro_get_FIFO_Data(I2C_HandleTypeDef *interface, uint8_t Adresse,
		uint8_t no_trials_fifo, uint8_t threshold);
void gyro_get_fifo_count(I2C_HandleTypeDef *interface, uint8_t Adresse,
		uint16_t *fifo_c);
void gyro_get_fifo_x_y_value(uint8_t data_buffer[], uint8_t threshold,
		int16_t x_value, int16_t y_value);
#endif /* APPLICATION_USER_MAG_3110_H_ */
