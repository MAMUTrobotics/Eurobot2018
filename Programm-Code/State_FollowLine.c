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
#define FOLLOW_LINE_SPEED_DIFFERENCE_DURING_ARC FOLLOW_LINE_SPEED / 3

// States for the Avoid FSM:
#define FOLLOW_LINE_DETECTED 		1

struct behaviour_command_t followLine = {0, 0, FWD, false, false, 0, IDLE};

uint8_t lastFollowLineArrayValue = 0;
uint8_t currentFollowLineArrayValue = 0;

// The behaviour command data type:
enum
{
	nichtsGefunden = 0,
	Linie_1_gefunden = 1,
	Linie_1_und_Luecke_gefunden = 2,
	Linie_2_gefunden = 3
};

int searchForTwoLines( int *GefundenePosition )
{
	int state = nichtsGefunden;
	int modulo = 0;
	int i, currentValue = currentFollowLineArrayValue;
	
	for(i = 0; i < 8; i++)
	{
		modulo = currentValue % 2;
		currentValue /= 2;
		//writeString_P("i: ");
		//writeInteger(*GefundenePosition, DEC);
		//writeString_P(" val: ");
		//writeInteger(currentValue, DEC);
		//writeString_P("\n");
		
		switch(state)
		{
		default:
		case nichtsGefunden:
			if( modulo == true )
				state = Linie_1_gefunden;
			break;
			
		case Linie_1_gefunden:
			if( modulo == false )
			{
				state = Linie_1_und_Luecke_gefunden;
				*GefundenePosition = i;
			}
			break;
			
		case Linie_1_und_Luecke_gefunden:
			if( modulo == true )
				state = Linie_2_gefunden;
			break;
			
		case Linie_2_gefunden:
			return Linie_2_gefunden;
			break;
		}
	}
	
	return state;
}

/**
 * If the line follower array detects a line the following code has to evaluate
 * what to do. The robot can drive straigt if the line is in the middle of it or
 * the robot could drive an arc if the line is off center.
 */
void behaviour_followLine(void)
{
	if(currentFollowLineArrayValue != 0) // Does a line have been found?
	{
		//if( followLine.state != FOLLOW_LINE_DETECTED)
		//	writeString_P("Line found\n"); 
		followLine.state = FOLLOW_LINE_DETECTED;
	}
	else
	{
		//if( followLine.state != FOLLOW_LINE_DETECTED)
		//	writeString_P("Line lost\n"); 
		followLine.state = IDLE;
	}
		
	switch(followLine.state)
	{
		int stateLines = nichtsGefunden;
		int mitteZwischenDenLinien = -1;
		case IDLE: 
		// Do nothing
			;
		break;
		
		case FOLLOW_LINE_DETECTED:
		// Here the code for the follwing of the lines has to be inserted:
			followLine.dir = FWD;
			followLine.speed_left = FOLLOW_LINE_SPEED;
			followLine.speed_right = FOLLOW_LINE_SPEED;
			
			stateLines = searchForTwoLines( &mitteZwischenDenLinien);
			
			if( stateLines == Linie_2_gefunden )
			{
				writeString_P("Doppellinie gefunden: pos:");
				writeInteger(mitteZwischenDenLinien, DEC);
				writeString_P(" : ");
				writeInteger(currentFollowLineArrayValue, DEC);
				writeString_P("\n");
			}
			else
			{
				writeString_P("Doppellinie NICHT gefunden: ");
				writeInteger(stateLines, DEC);
				writeString_P(" : ");
				writeInteger(currentFollowLineArrayValue, DEC);
				writeString_P("\n");
				return;
			}
			
			switch( mitteZwischenDenLinien )
			{
			case 1:
				followLine.dir = LEFT;
				break;
			
			case 2:
				followLine.speed_left  = 0;
				break;
			
			case 3:
				followLine.speed_left  -= FOLLOW_LINE_SPEED_DIFFERENCE_DURING_ARC;
				followLine.speed_right += FOLLOW_LINE_SPEED_DIFFERENCE_DURING_ARC;
				break;
			
			case 4:
				followLine.speed_left  += FOLLOW_LINE_SPEED_DIFFERENCE_DURING_ARC;
				followLine.speed_right -= FOLLOW_LINE_SPEED_DIFFERENCE_DURING_ARC;
				break;
			
			case 5:
				followLine.speed_right = 0;
				break;
			
			case 6:
				followLine.dir = RIGHT;
				break;
			
			case 0:
			case 7:
			default:
				break;
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
