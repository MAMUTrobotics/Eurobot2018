/* 
 * ****************************************************************************
 * State_FollowLine
 * ****************************************************************************
 * Description:
 * The followLine behavior is used to follow lines. 
 *
 */

/*****************************************************************************/
// Includes:
#include "globalIncludes.h"


/**************************************************************************/
// FollowLine Behaviour:


#define LINE_FOLLOWER_ARRAY_ADR  124 	// I2C-Adress of the line follower array
#define REG_DATA_B	0x10				//	RegDataB Data register _ I/O[15_8] (Bank B) 1111 1111*
#define LINE_FOLLOWER_READ_CYCLE_TIME_MS 100	// Time interval in which the line follower array will be read

// Some speed values for different movements:
#define FOLLOW_LINE_SPEED  40
#define FOLLOW_LINE_SPEED_DIFFERENCE_DURING_ARC FOLLOW_LINE_SPEED / 2

// States for the Avoid FSM:
#define FOLLOW_LINE_DETECTED 		1

struct behaviour_command_t followLine = {0, 0, FWD, false, false, 0, IDLE};

uint8_t lastFollowLineArrayValue = 0;
uint8_t currentFollowLineArrayValue = 0;

/**
 * If the line follower array detects a line the following code has to evaluate
 * what to do. The robot can drive straigt if the line is in the middle of it or
 * the robot could drive an arc if the line is off center.
 */
void behaviour_followLine(void)
{
	if(currentFollowLineArrayValue != 0) // Does a line have been found?
	{
		if( followLine.state != FOLLOW_LINE_DETECTED)
			writeString_P("Line found\n"); 
		followLine.state = FOLLOW_LINE_DETECTED;
	}
	else
	{
		if( followLine.state != FOLLOW_LINE_DETECTED)
			writeString_P("Line lost\n"); 
		followLine.state = IDLE;
	}
		
	switch(followLine.state)
	{
		case IDLE: 
		// Do nothing
			;
		break;
		
		case FOLLOW_LINE_DETECTED:
		// Here the code for the follwing of the lines has to be inserted:
			followLine.dir = FWD;
			followLine.speed_left = FOLLOW_LINE_SPEED;
			followLine.speed_right = FOLLOW_LINE_SPEED;
			
			
			if(   currentFollowLineArrayValue & 0b00000001    // Most left line sensor
			   || currentFollowLineArrayValue & 0b00000010	  // Second most left line sensor
			   || currentFollowLineArrayValue & 0b00000100 )  // Third most left line sensor
			{
				if( currentFollowLineArrayValue & 0b00000001) // Hat der äußerste angesprochen?
				{
					writeString_P("Drive left sharp arc\n");
					followLine.dir = LEFT;
				}
				else
				{
					writeString_P("Drive left arc\n");
					followLine.speed_left  -= FOLLOW_LINE_SPEED_DIFFERENCE_DURING_ARC;
					followLine.speed_right += FOLLOW_LINE_SPEED_DIFFERENCE_DURING_ARC;
				}
				
			}
			else if(   currentFollowLineArrayValue & 0b10000000		// Most right line sensor
			        || currentFollowLineArrayValue & 0b01000000     // Second most right line sensor
					|| currentFollowLineArrayValue & 0b00100000)	// Third most right line sensor
			{
				if( currentFollowLineArrayValue & 0b10000000) // Hat der äußerste angesprochen?
				{
					writeString_P("Drive right sharp arc\n");
					followLine.dir = RIGHT;
				}
				else
				{
					writeString_P("Drive right arc\n"); 
					followLine.speed_left  += FOLLOW_LINE_SPEED_DIFFERENCE_DURING_ARC;
					followLine.speed_right -= FOLLOW_LINE_SPEED_DIFFERENCE_DURING_ARC;
				}
			}
			else
			{
				writeString_P("Drive straight forward\n"); 
			}
		break;
	}
}

/**
 * The following function is used to get the line sensor value and check if the value has
 * has changed.
 */
void task_LineFollower(void)
{
	if(getStopwatch2() > LINE_FOLLOWER_READ_CYCLE_TIME_MS) 
	{
		// Der Empfang der Liniensensorwerte:
		uint8_t results[2];
		I2CTWI_readRegisters(LINE_FOLLOWER_ARRAY_ADR, REG_DATA_B, results, 2);
		currentFollowLineArrayValue = results[1];	// Byte 0 contains last conversion result
		
		if(currentFollowLineArrayValue != lastFollowLineArrayValue)
		{
			writeString_P("LinienWert: "); writeInteger(lastFollowLineArrayValue, DEC);
			writeString_P(" -> "); writeInteger(currentFollowLineArrayValue, DEC);
			writeChar('\n');
		}
		
		lastFollowLineArrayValue = currentFollowLineArrayValue;
		
		setStopwatch2(0);
	}
}
