#ifndef STATE_CRUISE_H
#define STATE_CRUISE_H
/* 
 * ****************************************************************************
 * State_Cruise
 * ****************************************************************************
 * Description:
 * The cruise behaviour is the lowest priority behaviour. It is only used to
 * ensure the robot moves if it has not anything more important to do.
 */

/*****************************************************************************/
// Includes:
#include "globalIncludes.h"


/*****************************************************************************/
// Cruise Behaviour:

#define CRUISE_SPEED_FWD    ROBOT_DEFAULT_SPEED  // Default speed when no obstacles are detected!
uint8_t speed = CRUISE_SPEED_FWD;

#define MOVE_FORWARDS 1
struct behaviour_command_t cruise = {CRUISE_SPEED_FWD, CRUISE_SPEED_FWD, FWD, false, false, 0, CRUISE_SPEED_FWD};

/**
 * We don't have anything to do here - this behaviour has only
 * a constant value for moving forwards - s. above!
 * Of course you can change this and add some random or timed movements 
 * in here...
 */
void behaviour_cruise(void)
{
	if(cruise.state == IDLE)
		return;
		
	if( !isStopwatch5Running())
	{
		writeString_P("Start Cruise\n"); 
		speed = CRUISE_SPEED_FWD;
		setStopwatch5(0);
		startStopwatch5();
	}
	
	// The following code was used to make the robot increase speed if no
	// obstacle has been detected. It isn't functioning so well 
	
	//if(getStopwatch5() > 1000)
	//{
	//	setStopwatch5(500);
	//	startStopwatch5();
	//	
	//	
	//	if(speed < 150)
	//	{
	//		speed += 1;
	//	}
	//	
	//	if(speed < 100)
	//	{
	//		speed += 1;
	//	}
	//	
	//	
	//	if(speed < 80)
	//	{
	//		speed += 1;
	//	}
	//	
	//	cruise.speed_left = speed;
	//	cruise.speed_right = speed;
	//	writeString_P("Speed: "); 
	//	writeInteger(speed, DEC);
	//	writeString_P("\n"); 
	//}
}

#endif