#ifndef FAHRBEFEHLHEADER
#define FAHRBEFEHLHEADER


#define RADDURCHMESSER 71    // in mm
#define RADABSTAND		177	// in mm
#define FULLSTEPS		200	// in mm
#define STEP_BUFFER 	0
#define COR_DEGREE		5
#define COR_DIST		1
#define MIN_SPEED 		30



typedef enum {
	CMD_IDLE 	= 0,
	CMD_FW		= 1,
	CMD_BW		= 2,
	CMD_TL		= 3,
	CMD_TR		= 4,
	CMD_STOP 	= 5							//todo:
} cmd_id_t;

typedef struct
{
	cmd_id_t			cmd_id;
	uint32_t 			cmd_dist;
	uint32_t 			cmd_degree;
	uint32_t 			cmd_average_speed;
	uint32_t 			cmd_average_acc_dec;
	motorStepMode_t 	cmd_s_mode;
	uint8_t				prc_softStop_flag0;
	uint8_t				prc_softStop_flag1;
	uint8_t				cmd_state;
} cmd_t;



//fuctionsprototyps
/*------------------------------------------------------Main Functions---------------------------------------------------------------*/
uint8_t set_new_cmd(cmd_t newCMD);
void cmd_execute();
/*------------------------------------------------------Support Functions------------------------------------------------------------*/
void cmd_p_softstop(uint8_t device);
void cmd_drive_dist(motorDir_t dir0,motorDir_t dir1,uint32_t dist,uint32_t dist_for_acc_dec);
void emergency_stop(uint32_t dist_for_acc_dec);
/*------------------------------------------------------Executiv Commands------------------------------------------------------------*/
void cmd_drive_fw(cmd_t cmd);
void cmd_drive_bw(cmd_t cmd);
void cmd_drive_tl(cmd_t cmd);
void cmd_drive_tr(cmd_t cmd);
void cmd_emergency_stop(cmd_t cmd);
/*------------------------------------------------------Setter/Getter/Inits----------------------------------------------------------*/
void initCMD();
void setParam(uint8_t device, uint16_t minSpeed, uint16_t maxSpeed, uint16_t acc_dec, motorStepMode_t s_mode );
void setParamWhileRunning(uint8_t device, uint16_t minSpeed, uint16_t maxSpeed, uint16_t acc_dec);
/*------------------------------------------------------calculation functions--------------------------------------------------------*/
uint32_t calc_steps_of_dist(uint16_t dist_mm, motorStepMode_t stepping_mode);
uint32_t calc_steps_of_decc(uint16_t dec, uint16_t speed, motorStepMode_t stepping_mode);
uint32_t calc_steps_of_degree(uint16_t degree, motorStepMode_t stepping_mode);

uint32_t calc_steps_of_decc_from_steps(uint32_t dec_steps, uint32_t speed_steps);
/*------------------------------------------------------conversion functions---------------------------------------------------------*/
uint32_t convert_speed_to_mmPerSecond(uint32_t speed_steps, motorStepMode_t stepping_mode);
uint32_t convert_speed_to_stepsPerSecond(uint16_t speed, motorStepMode_t stepping_mode);
uint32_t convert_acc_to_stepsPerSecond2(uint16_t acc, motorStepMode_t stepping_mode);
uint32_t inverse_of_stepping_mode_to_int(motorStepMode_t s_mode);
/*------------------------------------------------------Getter-----------------------------------------------------------------------*/
motorDir_t get_dir(uint8_t device, cmd_id_t);




#endif
