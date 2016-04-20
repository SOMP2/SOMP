/*
 * defines.h
 *
 *  Created on: 27.10.2015
 *      Author: Michael Flach
 */

////////////////////////////////////////////////////////
//Registers
// See MAG 3110 Datasheet for explanation
////////////////////////////////////////////////////////
#define		MAG_3110_ADRESS			0x0E;
////////////////////////////////////////////////////////
#define 	DR_STATUS   			0x00; //Data ready status per axis
////////////////////////////////////////////////////////
//Output Registers for the magnetic field vector within the
//carthesian coordinate frame
////////////////////////////////////////////////////////
#define 	OUT_X_MSB   			0x01; //Bits [15:8] of X measurement
#define 	OUT_X_LSB   			0x02; //Bits [7:0]  of X measurement
#define 	OUT_Y_MSB   			0x03; //Bits [15:8] of Y measurement
#define 	OUT_Y_LSB   			0x04; //Bits [7:0]  of Y measurement
#define 	OUT_Z_MSB  			 	0x05; //Bits [15:8] of Z measurement
#define 	OUT_Z_LSB   			0x06; //Bits [7:0]  of Z measurement
////////////////////////////////////////////////////////
//System registers
////////////////////////////////////////////////////////
#define 	WHO_AM_I    			0x07; //Device ID Number
#define 	SYSMOD      			0x08; //Current System Mode
#define 	CTRL_REG1   			0x10; //Operation Modes
#define 	CTRL_REG2   			0x11; //Operation Modes
////////////////////////////////////////////////////////
//Hard iron Offset correction
////////////////////////////////////////////////////////
#define 	OFF_X_MSB   			0x09; //Bits [14:7] of user X offset
#define 	OFF_X_LSB   			0x0A; //Bits [6:0]  of user X offset
#define 	OFF_Y_MSB   			0x0B; //Bits [14:7] of user Y offset
#define 	OFF_Y_LSB   			0x0C; //Bits [6:0]  of user Y offset
#define 	OFF_Z_MSB   			0x0D; //Bits [14:7] of user Z offset
#define 	OFF_Z_LSB   			0x0E; //Bits [6:0]  of user Z offset
////////////////////////////////////////////////////////
//Current temperature at temperature reference point of
//the magnetometer
////////////////////////////////////////////////////////
#define 	DIE_TEMP    			0x0F; //Temperature, signed 8 bits in C
////////////////////////////////////////////////////////
//Operation Modes
////////////////////////////////////////////////////////
#define 	AUTO				  	0x80;
#define 	INT1_Read				0b00011001;
