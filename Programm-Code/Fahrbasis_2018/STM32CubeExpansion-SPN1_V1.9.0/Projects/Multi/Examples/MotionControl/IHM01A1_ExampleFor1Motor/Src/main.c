/**
  ******************************************************************************
  * @file    Multi/Examples/MotionControl/IHM01A1_ExampleFor1Motor/Src/main.c 
  * @author  IPC Rennes
  * @version V1.9.0
  * @date    September 11th, 2017
  * @brief   This example shows how to use 1 IHM01A1 expansion board
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @defgroup IHM01A1_Example_for_1_motor_device
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 static volatile uint16_t gLastError;

L6474_Init_t gL6474InitParams =
{
    160,                               /// Acceleration rate in step/s2. Range: (0..+inf).
    160,                               /// Deceleration rate in step/s2. Range: (0..+inf). 
    1600,                              /// Maximum speed in step/s. Range: (30..10000].
    800,                               ///Minimum speed in step/s. Range: [30..10000).
    250,                               ///Torque regulation current in mA. (TVAL register) Range: 31.25mA to 4000mA.
    750,                               ///Overcurrent threshold (OCD_TH register). Range: 375mA to 6000mA.
    L6474_CONFIG_OC_SD_ENABLE,         ///Overcurrent shutwdown (OC_SD field of CONFIG register). 
    L6474_CONFIG_EN_TQREG_TVAL_USED,   /// Torque regulation method (EN_TQREG field of CONFIG register).
    L6474_STEP_SEL_1_16,               /// Step selection (STEP_SEL field of STEP_MODE register).
    L6474_SYNC_SEL_1_2,                /// Sync selection (SYNC_SEL field of STEP_MODE register).
    L6474_FAST_STEP_12us,              /// Fall time value (T_FAST field of T_FAST register). Range: 2us to 32us.
    L6474_TOFF_FAST_8us,               /// Maximum fast decay time (T_OFF field of T_FAST register). Range: 2us to 32us.
    3,                                 /// Minimum ON time in us (TON_MIN register). Range: 0.5us to 64us.
    21,                                /// Minimum OFF time in us (TOFF_MIN register). Range: 0.5us to 64us.
    L6474_CONFIG_TOFF_044us,           /// Target Swicthing Period (field TOFF of CONFIG register).
    L6474_CONFIG_SR_320V_us,           /// Slew rate (POW_SR field of CONFIG register).
    L6474_CONFIG_INT_16MHZ,            /// Clock setting (OSC_CLK_SEL field of CONFIG register).
    (L6474_ALARM_EN_OVERCURRENT      |
     L6474_ALARM_EN_THERMAL_SHUTDOWN |
     L6474_ALARM_EN_THERMAL_WARNING  |
     L6474_ALARM_EN_UNDERVOLTAGE     |
     L6474_ALARM_EN_SW_TURN_ON       |
     L6474_ALARM_EN_WRONG_NPERF_CMD)    /// Alarm (ALARM_EN register).
};


/* Private function prototypes -----------------------------------------------*/
static void MyFlagInterruptHandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  int32_t pos;
  uint16_t mySpeed;

  /* STM32xx HAL library initialization */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();
    
//----- Init of the Motor control library 
/* Set the L6474 library to use 1 device */
  BSP_MotorControl_SetNbDevices(BSP_MOTOR_CONTROL_BOARD_ID_L6474, 1);
  /* When BSP_MotorControl_Init is called with NULL pointer,                  */
  /* the L6474 registers and parameters are set with the predefined values from file   */
  /* l6474_target_config.h, otherwise the registers are set using the   */
  /* L6474_Init_t pointer structure                */
  /* The first call to BSP_MotorControl_Init initializes the first device     */
  /* whose Id is 0.                                                           */
  /* The nth call to BSP_MotorControl_Init initializes the nth device         */
  /* whose Id is n-1.                                                         */
  /* Uncomment the call to BSP_MotorControl_Init below to initialize the      */
  /* device with the structure gL6474InitParams declared in the the main.c file */
  /* and comment the subsequent call having the NULL pointer                   */
  //BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_L6474, &gL6474InitParams);
  BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_L6474, NULL);
  
  /* Attach the function MyFlagInterruptHandler (defined below) to the flag interrupt */
  BSP_MotorControl_AttachFlagInterrupt(MyFlagInterruptHandler);

  /* Attach the function Error_Handler (defined below) to the error Handler*/
  BSP_MotorControl_AttachErrorHandler(Error_Handler);
  
  //----- Move of 16000 steps in the FW direction
  /* Move device 0 of 16000 steps in the FORWARD direction*/
  BSP_MotorControl_Move(0, FORWARD, 16000);

  /* Wait for the motor of device 0 ends moving */
  BSP_MotorControl_WaitWhileActive(0);

  /* Wait for 2 seconds */
  HAL_Delay(2000);  
  
//----- Move of 16000 steps in the BW direction

  /* Move device 0 of 16000 steps in the BACKWARD direction*/
  BSP_MotorControl_Move(0, BACKWARD, 16000);

  /* Wait for the motor of device 0 ends moving */
  BSP_MotorControl_WaitWhileActive(0);

   /* Set the current position of device 0 to be the Home position */
  BSP_MotorControl_SetHome(0);
  
  /* Wait for 2 seconds */
  HAL_Delay(2000);

//----- Go to position -6400

  /* Request device 0 to go to position -6400 */
  BSP_MotorControl_GoTo(0,-6400);  
  
  /* Wait for the motor ends moving */
  BSP_MotorControl_WaitWhileActive(0);

  /* Get current position of device 0*/
  pos = BSP_MotorControl_GetPosition(0);

  if (pos != -6400) 
  {
    Error_Handler(11);
  }
  
  /* Set the current position of device 0 to be the Mark position */
  BSP_MotorControl_SetMark(0);

  /* Wait for 2 seconds */
  HAL_Delay(2000);
  
//----- Go Home

  /* Request device 0 to go to Home */
  BSP_MotorControl_GoHome(0);  
  BSP_MotorControl_WaitWhileActive(0);

  /* Get current position of device 0 */
  pos = BSP_MotorControl_GetPosition(0);
  
  /* Wait for 2 seconds */
  HAL_Delay(2000);

//----- Go to position 6400

  /* Request device 0 to go to position 6400 */
  BSP_MotorControl_GoTo(0,6400);  
  
  /* Wait for the motor of device 0 ends moving */
  BSP_MotorControl_WaitWhileActive(0);

  /* Get current position of device 0*/
  pos = BSP_MotorControl_GetPosition(0);

  /* Wait for 2 seconds */
  HAL_Delay(2000);
  
//----- Go Mark which was set previously after go to -6400

  /* Request device 0 to go to Mark position */
  BSP_MotorControl_GoMark(0);  
  
  /* Wait for the motor of device 0 ends moving */
  BSP_MotorControl_WaitWhileActive(0);

  /* Get current position of device 0 */
  pos = BSP_MotorControl_GetPosition(0);

  /* Wait for 2 seconds */
  HAL_Delay(2000);

//----- Run the motor BACKWARD

  /* Request device 0 to run BACKWARD */
   BSP_MotorControl_Run(0,BACKWARD);       
   HAL_Delay(5000);

   /* Get current speed of device 0 */
   mySpeed = BSP_MotorControl_GetCurrentSpeed(0);

//----- Increase the speed while running

  /* Increase speed of device 0 to 2400 step/s */
  BSP_MotorControl_SetMaxSpeed(0,2400);
  HAL_Delay(5000);

   /* Get current speed of device 0 */
   mySpeed = BSP_MotorControl_GetCurrentSpeed(0);

//----- Decrease the speed while running

  /* Decrease speed of device 0 to 1200 step/s */
  BSP_MotorControl_SetMaxSpeed(0,1200);
  HAL_Delay(5000);

  /* Get current speed */
  mySpeed = BSP_MotorControl_GetCurrentSpeed(0);

//----- Increase acceleration while running

  /* Increase acceleration of device 0 to 480 step/s^2 */
  BSP_MotorControl_SetAcceleration(0,480);
  HAL_Delay(5000);

  /* Increase speed of device 0 to 2400 step/s */
  BSP_MotorControl_SetMaxSpeed(0,2400);
  HAL_Delay(5000);

  /* Get current speed of device 0 */
  mySpeed = BSP_MotorControl_GetCurrentSpeed(0);

  if (mySpeed != 2400)
  {
    Error_Handler(10);
  }
//----- Increase deceleration while running

  /* Increase deceleration of device 0 to 480 step/s^2 */
  BSP_MotorControl_SetDeceleration(0,480);
  HAL_Delay(5000);

  /* Decrease speed of device 0 to 1200 step/s */
  BSP_MotorControl_SetMaxSpeed(0,1200);
  HAL_Delay(5000);

  /* Get current speed */
  mySpeed = BSP_MotorControl_GetCurrentSpeed(0);

//----- Soft stopped required while running

  /* Request soft stop of device 0 */
  BSP_MotorControl_SoftStop(0);

  /* Wait for the motor of device 0 ends moving */  
  BSP_MotorControl_WaitWhileActive(0);

  /* Wait for 2 seconds */
  HAL_Delay(2000);

//----- Run stopped by hardstop

  /* Request device 0 to run in FORWARD direction */
  BSP_MotorControl_Run(0,FORWARD);       
  HAL_Delay(5000);
  
  /* Request device 0 to immediatly stop */
  BSP_MotorControl_HardStop(0);
  BSP_MotorControl_WaitWhileActive(0);

  /* Request device 0 to disable bridge */
  BSP_MotorControl_CmdDisable(0);
  
  /* Wait for 2 seconds */
  HAL_Delay(2000);
  
//----- GOTO stopped by softstop

 /* Request device 0 to go to position 20000  */
  BSP_MotorControl_GoTo(0,20000);  
  HAL_Delay(5000);

  /* Request device 0 to perform a soft stop */
  BSP_MotorControl_SoftStop(0);
  BSP_MotorControl_WaitWhileActive(0);

  /* Wait for 2 seconds */
  HAL_Delay(2000);  

  //----- Read inexistent register to test MyFlagInterruptHandler

  /* Try to read an inexistent register */
  /* the flag interrupt should be raised */
  /* and the MyFlagInterruptHandler function called */
  BSP_MotorControl_CmdGetParam(0,0x1F);
  HAL_Delay(500);

//----- Change step mode to full step mode

  /* Select full step mode for device 0 */
   BSP_MotorControl_SelectStepMode(0,STEP_MODE_FULL);

  /* Set speed and acceleration to be consistent with full step mode */
   BSP_MotorControl_SetMaxSpeed(0,100);
   BSP_MotorControl_SetMinSpeed(0,50);
   BSP_MotorControl_SetAcceleration(0,10);
   BSP_MotorControl_SetDeceleration(0,10);

  /* Request device 0 to go position 200 */
   BSP_MotorControl_GoTo(0,200);  

  /* Wait for the motor of device 0 ends moving */
   BSP_MotorControl_WaitWhileActive(0);

  /* Get current position */
  pos =  BSP_MotorControl_GetPosition(0);

  /* Wait for 2 seconds */
  HAL_Delay(2000);
  
//----- Restore 1/16 microstepping mode

  /* Reset device 0 to 1/16 microstepping mode */
  BSP_MotorControl_SelectStepMode(0,STEP_MODE_1_16);    

  /* Update speed, acceleration, deceleration for 1/16 microstepping mode*/
  BSP_MotorControl_SetMaxSpeed(0,1600);
  BSP_MotorControl_SetMinSpeed(0,800);
  BSP_MotorControl_SetAcceleration(0,160);
  BSP_MotorControl_SetDeceleration(0,160);  
  
  /* Infinite loop */
  while(1)
  {
    /* Request device 0 to go position -6400 */
    BSP_MotorControl_GoTo(0,-6400);

    /* Wait for the motor of device 0 ends moving */
    BSP_MotorControl_WaitWhileActive(0);

    /* Request device 0 to go position 6400 */
    BSP_MotorControl_GoTo(0,6400);

    /* Wait for the motor of device 0 ends moving */
    BSP_MotorControl_WaitWhileActive(0);  
  }
}

/**
  * @brief  This function is the User handler for the flag interrupt
  * @param  None
  * @retval None
  */
void MyFlagInterruptHandler(void)
{
  /* Get the value of the status register via the L6474 command GET_STATUS */
  uint16_t statusRegister = BSP_MotorControl_CmdGetStatus(0);
  
  /* Check HIZ flag: if set, power brigdes are disabled */
  if ((statusRegister & L6474_STATUS_HIZ) == L6474_STATUS_HIZ)
  {
    // HIZ state
    // Action to be customized            
  }

  /* Check direction bit */
  if ((statusRegister & L6474_STATUS_DIR) == L6474_STATUS_DIR)
  {
    // Forward direction is set
    // Action to be customized            
  }  
  else
  {
    // Backward direction is set
    // Action to be customized            
  }  

  /* Check NOTPERF_CMD flag: if set, the command received by SPI can't be performed */
  /* This often occures when a command is sent to the L6474 */
  /* while it is in HIZ state */
  if ((statusRegister & L6474_STATUS_NOTPERF_CMD) == L6474_STATUS_NOTPERF_CMD)
  {
      // Command received by SPI can't be performed
     // Action to be customized            
  }  

  /* Check WRONG_CMD flag: if set, the command does not exist */
  if ((statusRegister & L6474_STATUS_WRONG_CMD) == L6474_STATUS_WRONG_CMD)
  {
     //command received by SPI does not exist 
     // Action to be customized          
  }  

  /* Check UVLO flag: if not set, there is an undervoltage lock-out */
  if ((statusRegister & L6474_STATUS_UVLO) == 0)
  {
     //undervoltage lock-out 
     // Action to be customized          
  }  

  /* Check TH_WRN flag: if not set, the thermal warning threshold is reached */
  if ((statusRegister & L6474_STATUS_TH_WRN) == 0)
  {
    //thermal warning threshold is reached
    // Action to be customized          
  }    

  /* Check TH_SHD flag: if not set, the thermal shut down threshold is reached */
  if ((statusRegister & L6474_STATUS_TH_SD) == 0)
  {
    //thermal shut down threshold is reached 
    // Action to be customized          
  }    

  /* Check OCD  flag: if not set, there is an overcurrent detection */
  if ((statusRegister & L6474_STATUS_OCD) == 0)
  {
    //overcurrent detection 
    // Action to be customized          
  }      
 
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  error number of the error
  * @retval None
  */
void Error_Handler(uint16_t error)
{
  /* Backup error number */
  gLastError = error;
  
  /* Infinite loop */
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
