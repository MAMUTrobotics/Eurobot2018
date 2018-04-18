/**
  ******************************************************************************
  * @file    Multi/Examples/MotionControl/IHM01A1_ExampleFor3Motors/Src/main.c 
  * @author  IPC Rennes
  * @version V1.9.0
  * @date    September 11th, 2017
  * @brief   This example shows how to drive 3 motors with 3 IHM01A1 expansion boards
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
    
/** @defgroup IHM01A1_Example_for_3_motor_devices
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 static volatile uint16_t gLastError;
 
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
  uint32_t myMaxSpeed;
  uint32_t myMinSpeed;
  uint16_t myAcceleration;
  uint16_t myDeceleration;

  /* STM32xx HAL library initialization */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();
    
/* Set the L6474 library to use 3 device */
  BSP_MotorControl_SetNbDevices(BSP_MOTOR_CONTROL_BOARD_ID_L6474, 3);
  
  /* When BSP_MotorControl_Init is called with NULL pointer,                  */
  /* the L6474 registers and parameters are set with the predefined values from file   */
  /* l6474_target_config.h, otherwise the registers are set using the   */
  /* L6474_Init_t pointer structure                */
  /* The first call to BSP_MotorControl_Init initializes the first device     */
  /* whose Id is 0.                                                           */
  /* The nth call to BSP_MotorControl_Init initializes the nth device         */
  /* whose Id is n-1.                                                         */
  
  /* Initialisation of first device */
  BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_L6474, NULL);
  /* Initialisation of second device */
  BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_L6474, NULL);
  /* Initialisation of third device */
  BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_L6474, NULL);

  /* Attach the function MyFlagInterruptHandler (defined below) to the flag interrupt */
  BSP_MotorControl_AttachFlagInterrupt(MyFlagInterruptHandler);

  /* Attach the function Error_Handler (defined below) to the error Handler*/
  BSP_MotorControl_AttachErrorHandler(Error_Handler);
  
  /* Set Current position to be the home position for device 0 */
  BSP_MotorControl_SetHome(0);

  /* Set Current position to be the home position for device 1 */
  BSP_MotorControl_SetHome(1);

  /* Set Current position to be the home position for device 2 */
  BSP_MotorControl_SetHome(2);

  /* Request device 0 to Goto position 3200 */ 
  BSP_MotorControl_GoTo(0,3200);  

  /* Wait for  device 0 ends moving */  
  BSP_MotorControl_WaitWhileActive(0);

  /* Get current position of device 0*/
  pos = BSP_MotorControl_GetPosition(0);
    
  /* If the read position of device 0 is 3200 */
  /* Request device 1 to go to the same position */
  if (pos == 3200)
  {
    /* Set current position of device 0 to be its mark position*/
    BSP_MotorControl_SetMark(0); 
    
    /* Request device 1 to Go to the same position  */ 
    BSP_MotorControl_GoTo(1,pos); 
    
    /* Wait for  device 1 ends moving */  
    BSP_MotorControl_WaitWhileActive(1);
  }
  
  /* Get current position of device 0*/
  pos = BSP_MotorControl_GetPosition(1);
  
  /* If the read position of device 1 is 3200 */
  /* Request device 2 to go to the same position */
  if (pos == 3200)
  {
    /* Request device 2 to Go to the same position  */ 
    BSP_MotorControl_GoTo(2,pos); 

    /* Wait for device 2 ends moving */  
    BSP_MotorControl_WaitWhileActive(2);
  }
  
  /* Get current position of device 0*/
  pos = BSP_MotorControl_GetPosition(2);

  /* Wait for 1s */
  HAL_Delay(1000);
  
  if (pos == 3200)
  {
    /* Request all devices to go home */
    BSP_MotorControl_GoHome(0); 
    BSP_MotorControl_GoHome(1); 
    BSP_MotorControl_GoHome(2); 

    /* Wait for all devices end moving */ 
    BSP_MotorControl_WaitWhileActive(0);
    BSP_MotorControl_WaitWhileActive(1);
    BSP_MotorControl_WaitWhileActive(2);
  }
  
  /* Wait for 1s */
  HAL_Delay(1000);
  
  /* Request device 0 to Goto position -3200 */ 
  BSP_MotorControl_GoTo(0,-3200);  

  /* Wait for device 0 ends moving */  
  BSP_MotorControl_WaitWhileActive(0);

  /* Get current position of device 0*/
  pos = BSP_MotorControl_GetPosition(0);
    
  /* If the read position of device 0 is -3200 */
  /* Request device 1 to go to the same position */
  if (pos == -3200)
  {
    /* Request device 1 to go to the same position  */ 
    BSP_MotorControl_GoTo(1,pos); 
    
    /* Wait for device 1 ends moving */  
    BSP_MotorControl_WaitWhileActive(1);
  }
  
  /* Get current position of device 0*/
  pos = BSP_MotorControl_GetPosition(1);
  
  /* If the read position of device 1 is -3200 */
  /* Request device 2 to go to the same position */
  if (pos == -3200)
  {
    /* Request device 2 to Go to the same position  */ 
    BSP_MotorControl_GoTo(2,pos); 

    /* Wait for device 2 ends moving */  
    BSP_MotorControl_WaitWhileActive(2);
  }
  
  /* Get current position of device 0*/
  pos = BSP_MotorControl_GetPosition(2);

  /* Wait for 1s */
  HAL_Delay(1000);

  if (pos == -3200)
  {
    /* Set current position of device 2 to be its mark position*/
    BSP_MotorControl_SetMark(2); 

    /* Request all devices to go home */
    BSP_MotorControl_GoHome(0); 
    BSP_MotorControl_GoHome(1); 
    BSP_MotorControl_GoHome(2); 

    /* Wait for all devices end moving */ 
    BSP_MotorControl_WaitWhileActive(0);
    BSP_MotorControl_WaitWhileActive(1);
    BSP_MotorControl_WaitWhileActive(2);
  }

  /* Wait for 1s */
  HAL_Delay(1000);
  
  /* Request device 0 and device 2 to go their mark position */
  BSP_MotorControl_GoMark(0); 
  BSP_MotorControl_GoMark(2); 
  
  /* Wait for device 0 and 2 ends moving */ 
  BSP_MotorControl_WaitWhileActive(0);
  BSP_MotorControl_WaitWhileActive(2);
  
  /* Wait for 1s */
  HAL_Delay(1000);
  
   /* Request device 0 to run in FORWARD direction */
  BSP_MotorControl_Run(0,FORWARD); 
 
  /* Get device 0 max speed */
  myMaxSpeed = BSP_MotorControl_GetMaxSpeed(0);
  
  /* Wait for device 0 reaches its max speed */
  while (BSP_MotorControl_GetCurrentSpeed(0) != myMaxSpeed);

  /* Set max speed of device 1 to be the one of device 0 -320 step/s */
  myMaxSpeed -= 320;
  BSP_MotorControl_SetMaxSpeed(1, myMaxSpeed);

  /* Request device 0 to run in FORWARD direction */
  BSP_MotorControl_Run(1,FORWARD); 
 
  /* Wait for device 1 reaches its max speed */
  while (BSP_MotorControl_GetCurrentSpeed(1) != myMaxSpeed);

   /* Set max speed of device 2 to be the one of device 1 -320 step/s */
  myMaxSpeed -= 320;
  BSP_MotorControl_SetMaxSpeed(2, myMaxSpeed);

  /* Request device 2 to run in FORWARD direction */
  BSP_MotorControl_Run(2,FORWARD); 

  /* Wait for device 1 reaches its max speed */
  while (BSP_MotorControl_GetCurrentSpeed(2) != myMaxSpeed);

  /* Wait for 3s */
  HAL_Delay(3000);
  
  /* Request device 1 to make a soft stop */
  BSP_MotorControl_SoftStop(1);
  
  /* Wait for both devices end moving */
  BSP_MotorControl_WaitWhileActive(1);  
  
  /* Request device 0 and 2 to make a hard stop */
  BSP_MotorControl_HardStop(0);
  BSP_MotorControl_HardStop(2);
  
  /* Wait for both devices end moving */
  BSP_MotorControl_WaitWhileActive(0);  
  BSP_MotorControl_WaitWhileActive(2);    
  
  /* Request all devices to go home */
  BSP_MotorControl_GoHome(0);  
  BSP_MotorControl_GoHome(1);  
  BSP_MotorControl_GoHome(2);  
  
  /* Wait for all devices end moving */  
  BSP_MotorControl_WaitWhileActive(0);
  BSP_MotorControl_WaitWhileActive(1);
  BSP_MotorControl_WaitWhileActive(2);

  /* Get acceleration, deceleration, Maxspeed and MinSpeed of device 0*/
  myAcceleration = BSP_MotorControl_GetAcceleration(0);
  myDeceleration = BSP_MotorControl_GetDeceleration(0);
  myMaxSpeed = BSP_MotorControl_GetMaxSpeed(0);
  myMinSpeed = BSP_MotorControl_GetMinSpeed(0);
  
  /* Select 1/8 microstepping mode for device 1 */
  BSP_MotorControl_SelectStepMode(1, STEP_MODE_1_8);

  /* Set speed and acceleration of device 1 to be consistent with 1/8 microstepping mode */
  BSP_MotorControl_SetMaxSpeed(1, myMaxSpeed / 2 );
  BSP_MotorControl_SetMinSpeed(1, myMinSpeed / 2 );
  BSP_MotorControl_SetAcceleration(1, myAcceleration / 2 );
  BSP_MotorControl_SetDeceleration(1, myDeceleration / 2 );
  
  /* Select ful step mode  for device 2 */
  BSP_MotorControl_SelectStepMode(2, STEP_MODE_FULL);

  /* Set speed and acceleration of device 1 to be consistent with full step mode */
  BSP_MotorControl_SetMaxSpeed(2, myMaxSpeed / 16);
  BSP_MotorControl_SetMinSpeed(2, myMinSpeed / 16);
  BSP_MotorControl_SetAcceleration(2, myAcceleration / 16);
  BSP_MotorControl_SetDeceleration(2, myDeceleration / 16);  
  
  /* Infinite loop */
  while(1)
  {
    /* Device 0 is using 1/16 microstepping mode */ 
    /* Device 1 is using 1/8 microstepping mode */
    /* Device 2 is using full step mode */
    BSP_MotorControl_GoTo(0, -3200); 
    BSP_MotorControl_GoTo(1, 1600); 
    BSP_MotorControl_GoTo(2, -200);   
    BSP_MotorControl_WaitWhileActive(0);
    BSP_MotorControl_WaitWhileActive(1);
    BSP_MotorControl_WaitWhileActive(2);

    BSP_MotorControl_GoTo(0, 3200); 
    BSP_MotorControl_GoTo(1, -1600); 
    BSP_MotorControl_GoTo(2, 200); 
    BSP_MotorControl_WaitWhileActive(0);
    BSP_MotorControl_WaitWhileActive(1);
    BSP_MotorControl_WaitWhileActive(2);
  }
}

/**
  * @brief  This function is the User handler for the flag interrupt
  * @param  None
  * @retval None
  */
void MyFlagInterruptHandler(void)
{
  /* Get status of device 0 */
  /* this will clear the flags */
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

  /* Get status of device 1 */
  /* this will clear the flags */
  statusRegister = BSP_MotorControl_CmdGetStatus(1);  
  
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

  /* Get status of device 2 */
  /* this will clear the flags */
  statusRegister = BSP_MotorControl_CmdGetStatus(2);  

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
