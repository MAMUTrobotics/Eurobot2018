/**
  ******************************************************************************
  * @file    Multi/Examples/MotionControl/IHM01A1_ExampleFor2Motors/Src/main.c 
  * @author  IPC Rennes
  * @version V1.9.0
  * @date    September 11th, 2017
  * @brief   This example shows how to drive 2 motors with 2 IHM01A1 expansion boards
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
#define HAL_I2C_MODULE_ENABLED
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal_i2c.h"

/** @defgroup IHM01A1_Example_for_2_motor_devices
  * @{
  */ 

#define D 0
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

 uint16_t speed = 3000;
 static volatile uint16_t gLastError;
 I2C_HandleTypeDef hi2c1;

/* Private function prototypes -----------------------------------------------*/
static void MyFlagInterruptHandler(void);
static void MX_I2C1_Init(void);
static void MX_GPIO_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */


int main(void)
{

	 HAL_Init(); //STM32xx HAL library initialization



	 SystemClock_Config(); //Configure the system clock;
	 MX_I2C1_Init();
	 MX_GPIO_Init();

	 BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);  // Initialisierung des Push Buttons

	 BSP_MotorControl_SetNbDevices(BSP_MOTOR_CONTROL_BOARD_ID_L6474, 2);	//lädt boardparameter

	 BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_L6474, NULL); //initialisierung vom ersten Motor treiber

	 BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_L6474, NULL); //initialisierung vom zweiten Motor treiber



	 BSP_MotorControl_AttachFlagInterrupt(MyFlagInterruptHandler);

	 BSP_MotorControl_AttachErrorHandler(Error_Handler);

	 BSP_MotorControl_ResetAllDevices();// DIESER BEFEHL löst mein PROBLEM mit der Spannungsversorgung :):)
	 HAL_Delay(1000);

	 /*						test area					todo: delete*/

	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
	 /*original stepping mode 1_8*/
	 /*todo: anderer stepping mode im moment nicht möglich*/

	 cmd_t idle = {CMD_IDLE,0,0,0,0,STEP_MODE_1_8};
	 cmd_t fw = {CMD_FW,500,0,1000,70,STEP_MODE_1_8};
	 cmd_t fw1 ={CMD_FW,1000,0,1000,70,STEP_MODE_1_8};
	 cmd_t fw2 = {CMD_FW,1500,0,1000,70,STEP_MODE_1_8};/*fehler*/
	 cmd_t fw3 = {CMD_FW,2000,0,1000,70,STEP_MODE_1_8};/*fehler*/

	 //cmd_t bw = {CMD_BW,500,0,500,50,STEP_MODE_1_8};

	 //cmd_t tl = {CMD_TL,0,90,50,50,STEP_MODE_1_16};

	 cmd_t tr = {CMD_TL,0,90,1000,60,STEP_MODE_1_16};
	 cmd_t tr1 = {CMD_TL,0,180,1000,60,STEP_MODE_1_16};
	 cmd_t tr2 = {CMD_TL,0,270,1000,60,STEP_MODE_1_16};



	 /*	----------------------------------------------------------------*/
		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);


		 //simple test:
//		 while(1)
//		 {
//			 set_new_cmd(fw);
//			 cmd_execute();
//		 }


		 //simple test enable:
//		 while(1)
//		 {
//			 if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0))
//				 set_new_cmd(fw);
//			 cmd_execute();
//		 }

//
//		 while(1)
//		 {
//			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
//			if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0))
//				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
//			if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1))
//				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
//			if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0))
//				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
//		 }

	 //todo: die Folgende scheiße muss laufen, und zwar so genau wie möglich

	 while(1){
		 if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)&&HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)&&HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)){
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
			 for(uint8_t count = 0; count<=1;count++)
			 {

				 while(!set_new_cmd(fw))
				 {
					 cmd_execute();
				 }
				 while(!set_new_cmd(tr))
				 {
					 cmd_execute();
				 }
				 while(!set_new_cmd(fw1))
				 {
					 cmd_execute();
				 }
				 while(!set_new_cmd(tr))
				 {
					 cmd_execute();
				 }
				 while(!set_new_cmd(idle))
				 {
					 cmd_execute();
				 }
			 }
		 }else if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)&&!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)&&HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)){
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
			 for(uint8_t count = 0; count<=0;count++)
			 {

				 while(!set_new_cmd(fw))
				 {
					 cmd_execute();
				 }
				 while(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0))
				 {
					cmd_execute();
				}

			 }
		 }else if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)&&HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)&&!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)){
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
			 for(uint8_t count = 0; count<=0;count++)
			 {
				 while(!set_new_cmd(fw1))
				 {
					 cmd_execute();
				 }
				 while(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0))
				 {
					 cmd_execute();
				 }
			 }
		 }else if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)&&!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)&&!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)){
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
			 for(uint8_t count = 0; count<=0;count++)
			 {
				 while(!set_new_cmd(fw2))
				 {
					 cmd_execute();
				 }
				 while(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0))
				 {
					 cmd_execute();
				 }
			}
		 }
	 }
}




/**/


void MX_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);


	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);


	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);


	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);

	/*todo: müssen für i2c wieder raus*/

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);
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

}

  static void MX_I2C1_Init(void)
  {

    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x00000708;
    hi2c1.Init.OwnAddress1 = 0x01;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
      Error_Handler(1);
    }

      /**Configure Analogue filter
      */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
      Error_Handler(1);
    }

      /**Configure Digital filter
      */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
    {
      Error_Handler(1);
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
  BSP_LED_Init(LED2);
  /* Infinite loop */
  while(1)
  {
	  //BSP_MotorControl_WaitWhileActive(0);
	  	BSP_LED_On(LED2);
	  	//HAL_GPIO_WritePin(LED2_GPIO_PORT,LED2_PIN,GPIO_PIN_RESET); //Hiermit wird die LED zurück gesetzt
	  	HAL_Delay(1000); //diese HAL_* dinger werden aus einer anderen Bibliothek geholt, das muss geg.fals initialisiert werden. Siehe oben:  BSP_LED_Init(LED2);
	  	BSP_LED_Off(LED2);
	  	//HAL_GPIO_WritePin(LED2_GPIO_PORT,LED2_PIN,GPIO_PIN_SET);	//Hiermit wird die LED gesetzt
	  	HAL_Delay(1000);											//leuchtet 1 sec
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





//--------------------------- old code parts -----------------------------------------------------------


//--------------------------- Testfunktion changes by max speed ----------------------------------------
/*
{
		 if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0))									//rechter Pin
		 {
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);

			 //--------------------------------Code-------------------------------------------------------
			 speed += 500;
			 if(speed > 5000)
			 {
				 speed = 5000;
			 }
			 BSP_MotorControl_SetMaxSpeed(0,speed);
			 BSP_MotorControl_SetMaxSpeed(1,speed);

			 //-------------------------------------------------------------------------------------------

		 	 HAL_Delay(500);

		 }else if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0))								//linker Pin
		 {
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);

			 //--------------------------------Code-------------------------------------------------------
			 speed -= 500;
			 BSP_MotorControl_SetMaxSpeed(0,speed);
			 BSP_MotorControl_SetMaxSpeed(1,speed);
			 if(speed <= 200)
			 {
				 BSP_MotorControl_HardStop(0);
				 BSP_MotorControl_HardStop(1);
				 speed = 200;
			 }
			 //-------------------------------------------------------------------------------------------

			 HAL_Delay(500);
		 }
		 else if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1))
		 {
			 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);						//mittlerer Pin

			 //--------------------------------Code-------------------------------------------------------
			 BSP_MotorControl_SoftStop(0);
 			 BSP_MotorControl_SoftStop(1);
		     //-------------------------------------------------------------------------------------------
			 HAL_Delay(500);

		 }

		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		 if(speed<3000) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
	 }
*/
