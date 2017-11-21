#ifndef STATE_ESCAPE_H
#define STATE_ESCAPE_H
/* 
 * ****************************************************************************
 * State_Escape
 * ****************************************************************************
 * Description:
 * The escape behaviour ensures that if a collision happend, the robot drives a
 * short distance backwards, then turns itself and starts moving again.
 */

/*****************************************************************************/
// Includes:
#include "globalIncludes.h"


/****************************************************************************/
// Escape Behaviour:

#define ESCAPE_SPEED_BWD    ROBOT_DEFAULT_SPEED
#define ESCAPE_SPEED_ROTATE ROBOT_DEFAULT_SPEED / 2


#define ESCAPE_FRONT		1
#define ESCAPE_FRONT_WAIT 	2
#define ESCAPE_LEFT  		3
#define ESCAPE_LEFT_WAIT	4
#define ESCAPE_RIGHT	    5
#define ESCAPE_RIGHT_WAIT 	6
#define ESCAPE_WAIT_END		7
struct behaviour_command_t escape = {0, 0, FWD, false, false, 0, IDLE}; 

struct behaviour_command_t STOP = {0, 0, FWD, false, false, 0, IDLE};

/**
 * This is the Escape behaviour for the Bumpers.
 */
void behaviour_escape(void)
{
	static uint8_t bump_count = 0;
	
	switch(escape.state)
	{
		case IDLE: 
		break;
		case ESCAPE_FRONT:
			escape.speed_left = ESCAPE_SPEED_BWD;
			escape.dir = BWD;
			escape.move = true;
			if(bump_count > 3)
				escape.move_value = 220;
			else
				escape.move_value = 160;
			escape.state = ESCAPE_FRONT_WAIT;
			bump_count+=2;
		break;
		case ESCAPE_FRONT_WAIT:			
			if(!escape.move) // Wait for movement to be completed
			{	
				escape.speed_left = ESCAPE_SPEED_ROTATE;
				if(bump_count > 3)
				{
					escape.move_value = 100;
					escape.dir = RIGHT;
					bump_count = 0;
				}
				else 
				{
					escape.dir = LEFT;
					escape.move_value = 70;
				}
				escape.rotate = true;
				escape.state = ESCAPE_WAIT_END;
			}
		break;
		case ESCAPE_LEFT:
			escape.speed_left = ESCAPE_SPEED_BWD;
			escape.dir 	= BWD;
			escape.move = true;
			if(bump_count > 3)
				escape.move_value = 190;
			else
				escape.move_value = 150;
			escape.state = ESCAPE_LEFT_WAIT;
			bump_count++;
		break;
		case ESCAPE_LEFT_WAIT:
			if(!escape.move) // Wait for movement to be completed
			{
				escape.speed_left = ESCAPE_SPEED_ROTATE;
				escape.dir = RIGHT;
				escape.rotate = true;
				if(bump_count > 3)
				{
					escape.move_value = 110;
					bump_count = 0;
				}
				else
					escape.move_value = 80;
				escape.state = ESCAPE_WAIT_END;
			}
		break;
		case ESCAPE_RIGHT:	
			escape.speed_left = ESCAPE_SPEED_BWD;
			escape.dir 	= BWD;
			escape.move = true;
			if(bump_count > 3)
				escape.move_value = 190;
			else
				escape.move_value = 150;
			escape.state = ESCAPE_RIGHT_WAIT;
			bump_count++;
		break;
		case ESCAPE_RIGHT_WAIT:
			if(!escape.move) // Wait for movement to be completed
			{ 
				escape.speed_left = ESCAPE_SPEED_ROTATE;		
				escape.dir = LEFT;
				escape.rotate = true;
				if(bump_count > 3)
				{
					escape.move_value = 110;
					bump_count = 0;
				}
				else
					escape.move_value = 80;
				escape.state = ESCAPE_WAIT_END;
			}
		break;
		case ESCAPE_WAIT_END:
			if(!(escape.move || escape.rotate)) // Wait for movement/rotation to be completed
				escape.state = IDLE;
		break;
	}
}

/**
 * Bumpers Event handler
 */
void bumpersStateChangedNew(void)
{
	if(bumper_left && bumper_right) // Both Bumpers were hit
	{
		escape.state = ESCAPE_FRONT;
	}
	else if(bumper_left)  			// Left Bumper was hit
	{
		if(escape.state != ESCAPE_FRONT_WAIT) 
			escape.state = ESCAPE_LEFT;
	}
	else if(bumper_right) 			// Right Bumper was hit
	{
		if(escape.state != ESCAPE_FRONT_WAIT)
			escape.state = ESCAPE_RIGHT;
	}
	stopStopwatch5(); 	// Reinitialisierung der Anfahrt-GeschwindigkeitsRampe
}

#endif