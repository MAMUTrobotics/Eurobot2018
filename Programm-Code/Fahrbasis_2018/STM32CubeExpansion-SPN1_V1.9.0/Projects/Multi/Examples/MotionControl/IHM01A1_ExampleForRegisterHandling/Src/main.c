/**
  ******************************************************************************
  * @file    Multi/Examples/MotionControl/IHM01A1_ExampleForRegisterHandling/Src/main.c 
  * @author  IPC Rennes
  * @version V1.9.0
  * @date    September 11th, 2017
  * @brief   This example shows how handle L6474 registers
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
    
/** @defgroup IHM01A1_Example_for_RegisterHandling
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
    1075,                               ///Overcurrent threshold (OCD_TH register). Range: 375mA to 6000mA.
    L6474_CONFIG_OC_SD_DISABLE,         ///Overcurrent shutwdown (OC_SD field of CONFIG register). 
    L6474_CONFIG_EN_TQREG_TVAL_USED,   /// Torque regulation method (EN_TQREG field of CONFIG register).
    L6474_STEP_SEL_1_8,               /// Step selection (STEP_SEL field of STEP_MODE register).
    L6474_SYNC_SEL_1_2,                /// Sync selection (SYNC_SEL field of STEP_MODE register).
    L6474_FAST_STEP_10us,              /// Fall time value (T_FAST field of T_FAST register). Range: 2us to 32us.
    L6474_TOFF_FAST_10us,               /// Maximum fast decay time (T_OFF field of T_FAST register). Range: 2us to 32us.
    3,                                 /// Minimum ON time in us (TON_MIN register). Range: 0.5us to 64us.
    21,                                /// Minimum OFF time in us (TOFF_MIN register). Range: 0.5us to 64us.
    L6474_CONFIG_TOFF_044us,           /// Target Swicthing Period (field TOFF of CONFIG register).
    L6474_CONFIG_SR_320V_us,           /// Slew rate (POW_SR field of CONFIG register).
    L6474_CONFIG_INT_16MHZ,            /// Clock setting (OSC_CLK_SEL field of CONFIG register).
    (L6474_ALARM_EN_OVERCURRENT      |
     L6474_ALARM_EN_THERMAL_SHUTDOWN |
     L6474_ALARM_EN_THERMAL_WARNING  |
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
  uint32_t registerValue;

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
  
  /* initialisation with parameters from gL6474InitParam */
  BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_L6474, &gL6474InitParams);
  /* initialisation with parameters from l6474_target_config.h */
  //BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_L6474, NULL);
  
  /* Attach the function MyFlagInterruptHandler (defined below) to the flag interrupt */
  BSP_MotorControl_AttachFlagInterrupt(MyFlagInterruptHandler);

  /* Attach the function Error_Handler (defined below) to the error Handler*/
  BSP_MotorControl_AttachErrorHandler(Error_Handler);
  
  /* Read ABS_POS register of device 0*/  
  registerValue = BSP_MotorControl_CmdGetParam(0, L6474_ABS_POS);

  if (registerValue != 1)
  {
    /* Set  ABS_POS register of device 0 to 1 */
    BSP_MotorControl_CmdSetParam(0, L6474_ABS_POS, 1);
  }

  /* Read ABS_POS register of device 0*/  
  registerValue = BSP_MotorControl_CmdGetParam(0, L6474_ABS_POS);
  
  /* Read EL_POS register of device 0*/  
  registerValue = BSP_MotorControl_CmdGetParam(0, L6474_EL_POS);

  if (registerValue != 64)
  {
    /* Set  EL_POS register of device 0 to 64 */
    BSP_MotorControl_CmdSetParam(0, L6474_EL_POS, 64);
  }

  /* Read MARK register of device 0*/  
  registerValue = BSP_MotorControl_CmdGetParam(0, L6474_MARK);

  if (registerValue != 1)
  {
    /* Set  MARK register of device 0 to 0 */
    BSP_MotorControl_CmdSetParam(0, L6474_MARK, 1);
  }

  /* Read T_FAST register of device 0*/  
  registerValue = BSP_MotorControl_CmdGetParam(0,L6474_T_FAST);  

  if (registerValue != ((uint8_t)L6474_CONF_PARAM_TOFF_FAST_DEVICE_0 |
                        (uint8_t)L6474_CONF_PARAM_FAST_STEP_DEVICE_0))
  {
    /* Set  T_FAST register of device 0 to predefined value of device 1 */
    BSP_MotorControl_CmdSetParam(0, L6474_T_FAST, (uint8_t)L6474_CONF_PARAM_TOFF_FAST_DEVICE_0 |(uint8_t)L6474_CONF_PARAM_FAST_STEP_DEVICE_0);
  }


  /* Read OCD_TH register of device 0*/  
  registerValue = BSP_MotorControl_CmdGetParam(0, L6474_OCD_TH);  

  if (registerValue != L6474_CONF_PARAM_OCD_TH_DEVICE_0)
  {
   /* Set  OCD_TH register of device 0 to predefined value of device 1 */
    BSP_MotorControl_CmdSetParam(0, L6474_OCD_TH, L6474_CONF_PARAM_OCD_TH_DEVICE_0);
  }

  /* Read STEP_MODE register of device 0*/  
  registerValue = BSP_MotorControl_CmdGetParam(0,L6474_STEP_MODE);    

  if (registerValue != ((uint8_t)L6474_CONF_PARAM_STEP_SEL_DEVICE_0 |
                        (uint8_t)L6474_CONF_PARAM_SYNC_SEL_DEVICE_0))
  {
   /* Set  STEP_MODE register of device 0 to predefined value of device 1*/
    BSP_MotorControl_CmdSetParam(0, L6474_STEP_MODE, (uint8_t)L6474_CONF_PARAM_STEP_SEL_DEVICE_0 |
                                                          (uint8_t)L6474_CONF_PARAM_SYNC_SEL_DEVICE_0);
  }

  /* Read ALARM_EN register of device 0*/  
  registerValue = BSP_MotorControl_CmdGetParam(0, L6474_ALARM_EN);    

  if (registerValue != L6474_CONF_PARAM_ALARM_EN_DEVICE_0)
  {
   /* Set ALARM_EN register of device 0 to predefined value of device 1 */
    BSP_MotorControl_CmdSetParam(0, L6474_ALARM_EN, L6474_CONF_PARAM_ALARM_EN_DEVICE_0);
  }

  /* Read CONFIG register of device 0*/  
  registerValue = BSP_MotorControl_CmdGetParam(0, L6474_CONFIG);    

  if (registerValue != ((uint16_t)L6474_CONF_PARAM_CLOCK_SETTING_DEVICE_0 |
                        (uint16_t)L6474_CONF_PARAM_TQ_REG_DEVICE_0 |
                        (uint16_t)L6474_CONF_PARAM_OC_SD_DEVICE_0 |
                        (uint16_t)L6474_CONF_PARAM_SR_DEVICE_0 |
                        (uint16_t)L6474_CONF_PARAM_TOFF_DEVICE_0))
  {
     /* Set CONFIG register of device 0 to predefined value of device 1 */
     BSP_MotorControl_CmdSetParam(0, L6474_CONFIG, (uint16_t)L6474_CONF_PARAM_CLOCK_SETTING_DEVICE_0 |
                                              (uint16_t)L6474_CONF_PARAM_TQ_REG_DEVICE_0 |
                                              (uint16_t)L6474_CONF_PARAM_OC_SD_DEVICE_0 |
                                              (uint16_t)L6474_CONF_PARAM_SR_DEVICE_0 |
                                              (uint16_t)L6474_CONF_PARAM_TOFF_DEVICE_0);
  
  }
  
   /* Set TVAL register by using analog value */
   if (L6474_GetAnalogValue(0, L6474_TVAL) == 250)
  {
    L6474_SetAnalogValue(0,  L6474_TVAL, 281.25);
    if (L6474_GetAnalogValue(0, L6474_TVAL) != 281.25f)
    {
      Error_Handler(1); 
    }
  }
  
  /* Set L6474_TON_MIN register by using analog value */
  if (L6474_GetAnalogValue(0, L6474_TON_MIN) == 3)
  {
    L6474_SetAnalogValue(0,  L6474_TON_MIN, 4);
    if (L6474_GetAnalogValue(0, L6474_TON_MIN) != 4)
    {
      Error_Handler(2); 
    }
  }
   
   /* Set L6474_TOFF_MIN register by using analog value */
  if (L6474_GetAnalogValue(0, L6474_TOFF_MIN) == 21)
  {
    L6474_SetAnalogValue(0,  L6474_TOFF_MIN, 22);
    if (L6474_GetAnalogValue(0, L6474_TOFF_MIN) != 22)
    {
      Error_Handler(3); 
    }
}

  /* Set L6474_OCD_TH register by using analog value */
  if (L6474_GetAnalogValue(0, L6474_OCD_TH) == 750)
  {
    L6474_SetAnalogValue(0,  L6474_OCD_TH, 1125);
    if (L6474_GetAnalogValue(0, L6474_OCD_TH) != 1125)
     {
      Error_Handler(4); 
     }
  }
  
  
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
