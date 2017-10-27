#ifndef GLOBALINCLUDES_H
#define GLOBALINCLUDES_H
/* 
 * ****************************************************************************
 * globalIncludes 
 * ****************************************************************************
 * Description:
 * In this file will be all data structures stored, which are common used
 * in different files.
 */

/*****************************************************************************/
// Behaviour command type:

#define IDLE  				0

// The behaviour command data type:
struct behaviour_command_t
{
	uint8_t  speed_left;  // left speed (is used for rotation and 
						  // move distance commands - if these commands are 
						  // active, speed_right is ignored!)
	uint8_t  speed_right; // right speed
	unsigned dir:2;       // direction (FWD, BWD, LEFT, RIGHT)
	unsigned move:1;      // move flag
	unsigned rotate:1;    // rotate flag
	uint16_t move_value;  // move value is used for distance and angle values
	uint8_t  state;       // state of the behaviour
};


#endif