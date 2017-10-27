/* 
 * ****************************************************************************
 * EuroBot 
 * ****************************************************************************
 * Description:
 * This file contains the main() and the behaviour controller, which determines 
 * what the robot has to do, considering the current enviromental situation. 
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h"
#include "RP6I2CmasterTWI.c"     // Include the I2C-Bus Slave Library

#include "State_Escape.c"
#include "State_Avoid.c"
#include "State_FollowLine.c"
#include "State_Cruise.c"


/*****************************************************************************/
// Move command:

/**
 * This function processes the movement commands that the behaviours generate. 
 * Depending on the values in the behaviour_command_t struct, it sets motor
 * speed, moves a given distance or rotates.
 */
void moveCommand(struct behaviour_command_t * cmd)
{
	if(cmd->move_value > 0)  // move or rotate?
	{
		if(cmd->rotate)
			rotate(cmd->speed_left, cmd->dir, cmd->move_value, false); 
		else if(cmd->move)
			move(cmd->speed_left, cmd->dir, DIST_MM(cmd->move_value), false); 
		cmd->move_value = 0; // clear move value - the move commands are only
		                     // given once and then runs in background.
	}
	else if(!(cmd->move || cmd->rotate)) // just move at speed? 
	{
		changeDirection(cmd->dir);
		moveAtSpeed(cmd->speed_left,cmd->speed_right);
	}
	else if(isMovementComplete()) // movement complete? --> clear flags!
	{
		cmd->rotate = false;
		cmd->move = false;
	}
}

/*****************************************************************************/
// Behaviour control:

/**
 * The behaviourController task controls the subsumption architechture. 
 * It implements the priority levels of the different behaviours. 
 */
void behaviourController(void)
{
    // Call all the behaviour tasks:
	behaviour_escape();
	behaviour_avoid();
	behaviour_followLine();
	behaviour_cruise();


    // Execute the commands depending on priority levels:
	if(escape.state != IDLE) 				// Highest priority - 4
		moveCommand(&escape);
	else if(avoid.state != IDLE) 			// Priority - 3
		moveCommand(&avoid);
	else if(followLine.state != IDLE) 		// Priority - 2
		moveCommand(&followLine);
	else if(cruise.state != IDLE)			// Priority - 1
		moveCommand(&cruise); 
	else                     				// Lowest priority - 0
		moveCommand(&STOP);  				// Default command - do nothing! 
											// In the current implementation this never 
											// happens.
}




/**
 * The main-function has some initialization-functions followed by a endless loop.
 * In the loop the interface- and behaviour-functions are called.
 */
int main(void)
{
	initRobotBase(); 
	setLEDs(0b111111);
	mSleep(2500);
	setLEDs(0b100100);
	
	writeString_P("Initialisierungen beginnen\n");
	
	I2CTWI_initMaster(100); // Initialize the TWI Module for Master operation
							// with 100kHz SCL Frequency
	
	// Set Bumpers state changed event handler:
	BUMPERS_setStateChangedHandler(bumpersStateChangedNew);
	
	// Set ACS state changed event handler:
	ACS_setStateChangedHandler(acsStateChangedNew);
	
	powerON(); // Turn on Encoders, Current sensing, ACS and Power LED.
	
	// Activates the Anti-Collision-System. The range can vary by using setACSPwrLow(), setACSPwrMed() or setACSPwrHigh()
	setACSPwrMed();
	
	startStopwatch2(); 	// Wird in task_LineFollower() verwendet
	
	writeString_P("EuroBotBot wird gestartet\n");
	
	// Main loop
	while(true) 
	{
		// Hier wird die zyklische Auswertung des ACS, Bumper und ADC durchgeführt. Zudem werden hier die Motoren angesteuert
		task_RP6System();   // Calls task_ADC(), task_ACS(), task_Bumpers() und	task_motionControl(). Implemented in RP6RobotBaseLib.c
		task_I2CTWI();			// Call I2C Management routine. Implemented in RP6RobotBaseLib.c
		
		task_LineFollower();	// Implemented in State_FollowLine.c
		
		behaviourController();  // Hier wird das aktuelle Verhalten des Roboters evaluiert
	}
	return 0;
}
