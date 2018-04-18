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
// Konstanten:

#define ROBOT_DEFAULT_SPEED		35	// Dieser Wert beeinflusst die Fahrtgeschwindigkeit bei allen Verhaltensweisen des Roboters
#define DEFAULT_EVALUATION_CYCLE_TIME_MS 60 	// Wird in task_LineFollower() verwendet
#define MAX_TIME_INTERVAL_FOR_CROSSROAD_EVALUATION	1100	// Innerhalb dieser Zeit in ms muss eine Kreuzung evaluiert werden können

/*****************************************************************************/
// Variablen:
uint8_t lastFollowLineArrayValue = 0;		// Letzter Wert, der vom Linien-Sensor-Array geliefert wurde


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