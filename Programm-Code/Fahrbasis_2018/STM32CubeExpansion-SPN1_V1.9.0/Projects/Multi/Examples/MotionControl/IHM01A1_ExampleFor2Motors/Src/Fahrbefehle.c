
#include "main.h"
#include "Fahrbefehl.h"
#include "math.h"

/*todo:kann raus
/// Timer handler for PWM1
extern TIM_HandleTypeDef hTimPwm1;
/// imer handler for PWM2
extern TIM_HandleTypeDef hTimPwm2;
*/

//variables
cmd_t currentCmd = {CMD_IDLE,0,0,0,0,STEP_MODE_FULL,0,0,0};
uint32_t c_steps0 = 0;
uint32_t c_steps1 = 0;




/*comment for functions:
 * The struct need to be loaded with mm parameters, the functions will work with step parameters.
 *
 * */




/*###################################################################################################################################*/
//private functions
/*------------------------------------------------------Main Functions---------------------------------------------------------------*/
uint8_t set_new_cmd(cmd_t newCMD)
{
	if(currentCmd.cmd_id==CMD_IDLE)
	{
		currentCmd = newCMD;
		initCMD();
		setParam(0, MIN_SPEED, convert_speed_to_stepsPerSecond(currentCmd.cmd_average_speed, currentCmd.cmd_s_mode), convert_acc_to_stepsPerSecond2(currentCmd.cmd_average_acc_dec, currentCmd.cmd_s_mode), currentCmd.cmd_s_mode);
		setParam(1, MIN_SPEED, convert_speed_to_stepsPerSecond(currentCmd.cmd_average_speed, currentCmd.cmd_s_mode), convert_acc_to_stepsPerSecond2(currentCmd.cmd_average_acc_dec, currentCmd.cmd_s_mode), currentCmd.cmd_s_mode);
		return 1;
	}else if(newCMD.cmd_id==CMD_STOP)
	{
		currentCmd = newCMD;
		initCMD();
		return 1;
	}else
	{
		return 0;
	}
}
void cmd_execute()
{
	switch(currentCmd.cmd_id)
	{
		case CMD_STOP:	cmd_emergency_stop(currentCmd); break;
		case CMD_IDLE:	break;
		case CMD_FW:	cmd_drive_fw(currentCmd);break;
		case CMD_BW:	cmd_drive_bw(currentCmd);break;
		case CMD_TL:	cmd_drive_tl(currentCmd); break;
		case CMD_TR:	cmd_drive_tr(currentCmd); break;
	}
}
/*------------------------------------------------------Setter/Init Functions--------------------------------------------------------*/
void initCMD(){
		currentCmd.cmd_dist					= currentCmd.cmd_dist + COR_DIST;
		currentCmd.cmd_degree				= currentCmd.cmd_degree + COR_DEGREE;
		c_steps0							= 0;
		c_steps1							= 0;
		currentCmd.prc_softStop_flag0		= 0;
		currentCmd.prc_softStop_flag1		= 0;
		currentCmd.cmd_state				= 0;
		/*todo: muss für Bogenfahrten angepasst werden*/
		return;
}
void setParam(uint8_t device, uint16_t minSpeed, uint16_t maxSpeed, uint16_t acc_dec, motorStepMode_t s_mode )
{
	BSP_MotorControl_SetAcceleration(device, acc_dec);
	BSP_MotorControl_SetDeceleration(device, acc_dec);
	BSP_MotorControl_SetMinSpeed(device, minSpeed);
	BSP_MotorControl_SetMaxSpeed(device, maxSpeed);
	BSP_MotorControl_SelectStepMode(device, s_mode);
}
void setParamWhileRunning(uint8_t device, uint16_t minSpeed, uint16_t maxSpeed, uint16_t acc_dec)
{
	BSP_MotorControl_SetAcceleration(device, acc_dec);
	BSP_MotorControl_SetDeceleration(device, acc_dec);
	BSP_MotorControl_SetMinSpeed(device, minSpeed);
	BSP_MotorControl_SetMaxSpeed(device, maxSpeed);
}

/*------------------------------------------------------Support Functions------------------------------------------------------------*/
void cmd_p_softstop(uint8_t device)
{
	if(device)
			currentCmd.prc_softStop_flag1 = 1;
		else
			currentCmd.prc_softStop_flag0 = 1;
	BSP_MotorControl_SoftStop(device);
}
/*todo: must be tested*/
void cmd_drive_dist(motorDir_t dir0,motorDir_t dir1,uint32_t dist,uint32_t dist_for_acc_dec)
{
	if(currentCmd.cmd_state == 0)
	{
		BSP_MotorControl_Run(0,dir0);
		BSP_MotorControl_Run(1,dir1);
		if(dist/2<=dist_for_acc_dec)
		{
			currentCmd.cmd_state = 10;
		}else
		{
			currentCmd.cmd_state = 1;
		}
	}
	else if(currentCmd.cmd_state == 1 && c_steps0 >= dist - dist_for_acc_dec)
	{
		cmd_p_softstop(0);
		cmd_p_softstop(1);
		currentCmd.cmd_state = 2;
	}
	else if(currentCmd.cmd_state == 2 && c_steps0 >= dist-STEP_BUFFER)
	{
		BSP_MotorControl_HardStop(0);
		BSP_MotorControl_HardStop(1);
		currentCmd.cmd_id = CMD_IDLE;
		currentCmd.prc_softStop_flag0 = 0;
		currentCmd.prc_softStop_flag1 = 0;
		c_steps0 = 0;
		c_steps1 = 0;
		currentCmd.cmd_state = 0;
	}
	else if(currentCmd.cmd_state == 10 && c_steps0 >= dist/2)
	{
		cmd_p_softstop(0);
		cmd_p_softstop(1);
		currentCmd.cmd_state = 2;
	}
}
void emergency_stop(uint32_t dist_for_acc_dec)
{
	if(currentCmd.cmd_state == 0)
	{
		if(!currentCmd.prc_softStop_flag0 && !currentCmd.prc_softStop_flag1)
		{
			setParam(0, MIN_SPEED, convert_speed_to_stepsPerSecond(currentCmd.cmd_average_speed, currentCmd.cmd_s_mode), convert_acc_to_stepsPerSecond2(currentCmd.cmd_average_acc_dec, currentCmd.cmd_s_mode), currentCmd.cmd_s_mode);
			setParam(1, MIN_SPEED, convert_speed_to_stepsPerSecond(currentCmd.cmd_average_speed, currentCmd.cmd_s_mode), convert_acc_to_stepsPerSecond2(currentCmd.cmd_average_acc_dec, currentCmd.cmd_s_mode), currentCmd.cmd_s_mode);
			cmd_p_softstop(0);
			cmd_p_softstop(1);
			currentCmd.cmd_state = 1;
			currentCmd.cmd_state = 1;
		}else
		{
			setParamWhileRunning(0, MIN_SPEED, convert_speed_to_stepsPerSecond(currentCmd.cmd_average_speed, currentCmd.cmd_s_mode), convert_acc_to_stepsPerSecond2(currentCmd.cmd_average_acc_dec, currentCmd.cmd_s_mode));
			setParamWhileRunning(1, MIN_SPEED, convert_speed_to_stepsPerSecond(currentCmd.cmd_average_speed, currentCmd.cmd_s_mode), convert_acc_to_stepsPerSecond2(currentCmd.cmd_average_acc_dec, currentCmd.cmd_s_mode));
			currentCmd.cmd_state = 1;
		}
	}
	if(currentCmd.cmd_state == 1 &&(c_steps0 >=  dist_for_acc_dec - STEP_BUFFER ||c_steps1 >=  dist_for_acc_dec - STEP_BUFFER ))
	{
		BSP_MotorControl_HardStop(0);
		BSP_MotorControl_HardStop(1);
		currentCmd.cmd_id = CMD_IDLE;
		currentCmd.prc_softStop_flag0 = 0;
		currentCmd.prc_softStop_flag1 = 0;
		c_steps0 = 0;
		c_steps1 = 0;
		currentCmd.cmd_state = 0;
	}

}
/*------------------------------------------------------Executiv Commands------------------------------------------------------------*/

void cmd_drive_fw(cmd_t cmd)
{
	cmd_drive_dist( get_dir(0,CMD_FW) , get_dir(1,CMD_FW) , calc_steps_of_dist(cmd.cmd_dist, cmd.cmd_s_mode) , calc_steps_of_decc(cmd.cmd_average_acc_dec, cmd.cmd_average_speed,cmd.cmd_s_mode));
}
void cmd_drive_bw(cmd_t cmd)
{
	cmd_drive_dist( get_dir(0,CMD_BW) , get_dir(1,CMD_BW) , calc_steps_of_dist(cmd.cmd_dist, cmd.cmd_s_mode) , calc_steps_of_decc(cmd.cmd_average_acc_dec, cmd.cmd_average_speed,cmd.cmd_s_mode));
}
void cmd_drive_tl(cmd_t cmd)
{
	cmd_drive_dist( get_dir(0,CMD_TL) , get_dir(1,CMD_TL) , calc_steps_of_degree(cmd.cmd_degree , cmd.cmd_s_mode), calc_steps_of_decc(cmd.cmd_average_acc_dec, cmd.cmd_average_speed,cmd.cmd_s_mode));
}
void cmd_drive_tr(cmd_t cmd)
{
	cmd_drive_dist( get_dir(0,CMD_TR) , get_dir(1,CMD_TR) , calc_steps_of_degree(cmd.cmd_degree , cmd.cmd_s_mode), calc_steps_of_decc(cmd.cmd_average_acc_dec, cmd.cmd_average_speed,cmd.cmd_s_mode));
}
void cmd_inc_speed(cmd_t cmd)
{
}
void cmd_dec_speed(cmd_t cmd)
{
}
cmd_emergency_stop(cmd_t cmd){
	emergency_stop(calc_steps_of_decc(cmd.cmd_average_acc_dec,cmd.cmd_average_speed,cmd.cmd_s_mode));
}
/*------------------------------------------------------calculation functions--------------------------------------------------------*/
uint32_t calc_steps_of_dist(uint16_t dist_mm, motorStepMode_t stepping_mode)
{
	return ((double)1000*FULLSTEPS*inverse_of_stepping_mode_to_int(stepping_mode)*dist_mm)/(3140*RADDURCHMESSER);
}
uint32_t calc_steps_of_decc(uint16_t dec, uint16_t speed, motorStepMode_t stepping_mode)
{
	return (calc_steps_of_dist((((double)(speed*speed)-(convert_speed_to_mmPerSecond(MIN_SPEED,stepping_mode)*convert_speed_to_mmPerSecond(MIN_SPEED,stepping_mode)))/(dec*2)),stepping_mode));
}
uint32_t calc_steps_of_decc_from_steps(uint32_t dec_steps, uint32_t speed_steps)
{
	return ((double)speed_steps*speed_steps)/(dec_steps*2);
}
uint32_t calc_steps_of_degree(uint16_t degree, motorStepMode_t stepping_mode)
{
	return ((double)RADABSTAND*FULLSTEPS*inverse_of_stepping_mode_to_int(stepping_mode)*degree)/(360*RADDURCHMESSER);;

}
/*------------------------------------------------------conversion functions---------------------------------------------------------*/
uint32_t convert_speed_to_mmPerSecond(uint32_t speed_steps, motorStepMode_t stepping_mode)
{
	return (double)3140*RADDURCHMESSER*speed_steps/(1000*FULLSTEPS*inverse_of_stepping_mode_to_int(stepping_mode));
}
uint32_t convert_speed_to_stepsPerSecond(uint16_t speed, motorStepMode_t stepping_mode)
{
	return calc_steps_of_dist(speed,stepping_mode);
}
uint32_t convert_acc_to_stepsPerSecond2(uint16_t acc, motorStepMode_t stepping_mode)
{
	return calc_steps_of_dist(acc,stepping_mode);
}
uint32_t inverse_of_stepping_mode_to_int(motorStepMode_t s_mode)
{
	switch(s_mode)
	{
		case	STEP_MODE_FULL: 	return 1;				// not implemented at the moment
		case	STEP_MODE_HALF: 	return 2;				// not implemented at the moment
		case	STEP_MODE_1_4:		return 4;				// not implemented at the moment
		case   	STEP_MODE_1_8:		return 8;
		case   	STEP_MODE_1_16: 	return 16;
		case   	STEP_MODE_1_32: 	return 32;
		case   	STEP_MODE_1_64: 	return 64;				// not implemented at the moment
		case   	STEP_MODE_1_128:	return 128;				// not implemented at the moment
		case   	STEP_MODE_1_256:	return 256;				// not implemented at the moment
		case 	STEP_MODE_UNKNOW:	return 0;				// not implemented at the moment
		case  	STEP_MODE_WAVE:		return 0;				// not implemented at the moment
	}
	return 1;
}
/*------------------------------------------------------Getter-----------------------------------------------------------------------*/
motorDir_t get_dir(uint8_t device, cmd_id_t type){
	switch (type)
	{
		case CMD_FW:
		{
			if(device)
				return BACKWARD;
			else
				return FORWARD;
		}
		case CMD_BW:
		{
			if(device)
				return BACKWARD;
			else
				return FORWARD;
		}
		case CMD_TL:
		{
			return BACKWARD;
		}
		case CMD_TR:
		{
			return FORWARD;
		}
		case CMD_STOP:
			return UNKNOW_DIR;
		case CMD_IDLE:
			return UNKNOW_DIR;
	}
	return UNKNOW_DIR;
}
/*todo:
 *			+stepps to steps verbessern
 *			+Richtungen und Drehrichtungen der Fahrkommandos testen
 * */

