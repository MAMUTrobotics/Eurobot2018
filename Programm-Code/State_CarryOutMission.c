#ifndef STATE_CARRY_OUT_MISSION_H
#define STATE_CARRY_OUT_MISSION_H
/* 
 * ****************************************************************************
 * State_CarryOutMission
 * ****************************************************************************
 * Description:
 * 
 * 
 */

/*****************************************************************************/
// Includes:
#include "globalIncludes.h"


/**************************************************************************/
// CarryOutMission Behaviour:

#define CARRY_OUT_MISSION_CYCLE_TIME_MS DEFAULT_EVALUATION_CYCLE_TIME_MS	// Time interval in which the line follower array will be read

// Some speed values for different movements:
#define CARRY_OUT_MISSION_SPEED  ROBOT_DEFAULT_SPEED

// Mögliche Aufgaben:
#define CARRY_OUT_MISSION_FEED_THE_CITY_WITH_DRINKING_WATER		1
#define CARRY_OUT_MISSION_BUILDING_HEQ_BUILDINGS				2
#define CARRY_OUT_MISSION_FEEDING_HOME_AUTOMATION_PANEL			3
#define CARRY_OUT_MISSION_FORAGING_A_FLOWER						4
#define CARRY_OUT_MISSION_EVALUATE_PERFORMANCE					5

// Mögliche Kreuzungsarten:
#define SACKGASSE												1
#define VIER_WEGE_KREUZUNG										2
#define DREI_WEGE_KREUZUNG_LINKS_RECHTS							3
#define DREI_WEGE_KREUZUNG_LINKS_GERADEAUS						4
#define DREI_WEGE_KREUZUNG_RECHTS_GERADEAUS						5
#define FAHRE_EIN_STUECK_GERADEAUS								6

// Mögliche Fahrkommandos
#define RIGHT_ARC_90											1
#define LEFT_ARC_90												2
#define FRONT_40												3
#define RIGHT_ARC_180											4

uint8_t kreuzungDetektiert = IDLE;
uint8_t RechtsUndLinksAbbiegungErkannt = false;
uint8_t KreuzungsMitteErkannt = false;

struct behaviour_command_t carryOutMission = {0, 0, FWD, false, false, 0, IDLE};

void resetVariables(void)
{
	kreuzungDetektiert = IDLE;
	RechtsUndLinksAbbiegungErkannt = false;
	KreuzungsMitteErkannt = false;
	carryOutMission.state = IDLE;
}

/**
 * ........
 */
void behaviour_carryOutMission(void)
{
	
}

/**
 * ........
 */
void entscheideWohinGefahrenWerdenSoll(void)
{
	// Im folgenden werden zu Testzwecken sinnlose default-Werte verwendet, dies muss noch angepasst werden
	switch(kreuzungDetektiert)
	{
	case SACKGASSE:
		carryOutMission.move_value = 180;
		carryOutMission.dir = RIGHT;
		carryOutMission.rotate = true;
		carryOutMission.state = RIGHT_ARC_180;
		break;
		
	case VIER_WEGE_KREUZUNG:
		carryOutMission.move_value = 90;
		carryOutMission.dir = RIGHT;
		carryOutMission.rotate = true;
		carryOutMission.state = RIGHT_ARC_90;
		break;
		
	case DREI_WEGE_KREUZUNG_LINKS_RECHTS:
		carryOutMission.move_value = 90;
		carryOutMission.dir = RIGHT;
		carryOutMission.rotate = true;
		carryOutMission.state = RIGHT_ARC_90;
		break;
		
	case DREI_WEGE_KREUZUNG_LINKS_GERADEAUS:
		carryOutMission.move_value = 40;
		carryOutMission.dir = FWD;
		carryOutMission.move = true;
		carryOutMission.state = FRONT_40;
		break;
		
	case DREI_WEGE_KREUZUNG_RECHTS_GERADEAUS:
		carryOutMission.move_value = 90;
		carryOutMission.dir = RIGHT;
		carryOutMission.rotate = true;
		carryOutMission.state = RIGHT_ARC_90;
		break;
		
	default:
		carryOutMission.move_value = 0;
		carryOutMission.dir = RIGHT;
		carryOutMission.rotate = false;
		carryOutMission.move = false;
		carryOutMission.state = RIGHT_ARC_90;
		break;
	}
}

/**
 * The following function is used to get the line sensor value and check if the value has
 * has changed.
 */
void task_EvaluateMission(void)
{
	if( getStopwatch1() > MAX_TIME_INTERVAL_FOR_CROSSROAD_EVALUATION)	// Wurde schon zu lange keiner Linie mehr gefolgt?
	{
		resetVariables();
		return;
	}
	
	if( kreuzungDetektiert != IDLE)	// Wurde bereits eine Kreuzung entdeckt?
	{
		if(!(carryOutMission.move || carryOutMission.rotate)) // Wait for movement/rotation to be completed
			carryOutMission.state = IDLE;
		return;
	}
	
	if( RechtsUndLinksAbbiegungErkannt == true)
	{
		if( KreuzungsMitteErkannt == false)
		{
			// Es wurde zuvor erkannt, dass eine Vierwegekreuzung oder eine Dreiwegekreuzung vorliegt.
			// Nun wird geradeausgefahren, bis die Mitte der Kreuzung passiert wird
			if(  ( lastFollowLineArrayValue & 0b00100000
			    || lastFollowLineArrayValue & 0b00010000
			    || lastFollowLineArrayValue & 0b00001000
		        || lastFollowLineArrayValue & 0b00000100) == false ) 	// Wurde die Mitte einer T- bzw. Vierer-Kreuzung erkannt?
			{
				KreuzungsMitteErkannt = true;
			}
		}
		else
		{
			// Es wurde zuvor erkannt, dass eine Vierwegekreuzung oder eine Dreiwegekreuzung vorliegt.
			// Es wurde zuvor die Mitte der Kreuzung erkannt.
			// Nun wird geradeausgefahren, bis das hintere Ende der Wegmarkierungen erscheint.
			
			if( lastFollowLineArrayValue & 0b00100000
			 && lastFollowLineArrayValue & 0b00010000
			 && lastFollowLineArrayValue & 0b00001000
		     && lastFollowLineArrayValue & 0b00000100) 	// Wurde das hintere Ende der T-Kreuzung erkannt?
			{
				kreuzungDetektiert = DREI_WEGE_KREUZUNG_LINKS_RECHTS;
				writeString_P("3-Wege-Kreuzung erkannt (Rechts + Geradeaus)\n"); 
			}
			else if( (lastFollowLineArrayValue & 0b00100000 && lastFollowLineArrayValue & 0b00000100)
				  && (lastFollowLineArrayValue & 0b00010000 || lastFollowLineArrayValue & 0b00001000) == false) 	// Wurde das hintere Ende der Vierwege-Kreuzung erkannt?
		    {
				kreuzungDetektiert = VIER_WEGE_KREUZUNG;
				writeString_P("4-Wege-Kreuzung erkannt\n"); 
		    }
		}
		
		// Alle Variablen müssen noch initialisiert und zurückgesetzt werden
	}
	else if(    lastFollowLineArrayValue & 0b00100000
	         && lastFollowLineArrayValue & 0b00010000
		     && lastFollowLineArrayValue & 0b00001000
		     && lastFollowLineArrayValue & 0b00000100)	// Wurde eine Sackgasse detektiert?
	{
		kreuzungDetektiert = SACKGASSE;
		writeString_P("Sackgasse erkannt\n"); 
	}
	else if(    lastFollowLineArrayValue & 0b01000000
		     && lastFollowLineArrayValue & 0b00100000
		     && lastFollowLineArrayValue & 0b00000100
		     && lastFollowLineArrayValue & 0b00000010)	// Wurde eine Rechts- und eine Linkskurve entdeckt?
	{
		// Es kann sich sowohl um eine T-Kreuzung, als auch um eine Vier-Wege-Kreuzung handeln. Daher muss noch
		// ein Stück gefahren werden, um eine dieser beiden Optionen auszuschließen
		writeString_P("Rechts- und Linkskurve gefunden\n");
		carryOutMission.state = FAHRE_EIN_STUECK_GERADEAUS;
		carryOutMission.dir = FWD;
		carryOutMission.speed_left  = CARRY_OUT_MISSION_SPEED;
		carryOutMission.speed_right = CARRY_OUT_MISSION_SPEED;
		RechtsUndLinksAbbiegungErkannt = true;
	}
	else if(    lastFollowLineArrayValue & 0b10000000
			 && lastFollowLineArrayValue & 0b01000000
			 && lastFollowLineArrayValue & 0b00100000 )	// Haben die äußeren drei rechten Liniensensoren angesprochen? 
	{
		kreuzungDetektiert = DREI_WEGE_KREUZUNG_RECHTS_GERADEAUS;
		writeString_P("3-Wege-Kreuzung erkannt (Rechts + Geradeaus)\n"); 
	}
	else if(    lastFollowLineArrayValue & 0b00000001
	         && lastFollowLineArrayValue & 0b00000010
		     && lastFollowLineArrayValue & 0b00000100 )	// Haben die äußeren drei linken Liniensensoren angesprochen? 
	{
		kreuzungDetektiert = DREI_WEGE_KREUZUNG_LINKS_GERADEAUS;
		writeString_P("3-Wege-Kreuzung erkannt (Links + Geradeaus)\n"); 
	}
	else
	{
		resetVariables();	// Falls nichts sinnvolles erkannt wurde, soll nichts getan werden
	}
	
	if( kreuzungDetektiert != IDLE)	// Wurde eine Kreuzung entdeckt?
	{
		entscheideWohinGefahrenWerdenSoll();
	}
}

#endif