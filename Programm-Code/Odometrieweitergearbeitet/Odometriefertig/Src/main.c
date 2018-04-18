/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
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
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
/*if(GeschwindigkeitLinksmms < 100){ GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_0<<6);}      //Test für Ausgabe der Geschwindigkeit des linken Rads übers Display
else if(GeschwindigkeitLinksmms < 200){GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_1<<6);}
else if(GeschwindigkeitLinksmms < 300){GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_2<<6);}
else if(GeschwindigkeitLinksmms < 400){GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_3<<6);}
else if(GeschwindigkeitLinksmms < 500){GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_4<<6);}
else if(GeschwindigkeitLinksmms < 600){GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_5<<6);}
else if(GeschwindigkeitLinksmms < 700){GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_6<<6);}
else if(GeschwindigkeitLinksmms < 800){GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_7<<6);}
else if(GeschwindigkeitLinksmms < 900){GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_8<<6);}
else if(GeschwindigkeitLinksmms < 1000){GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_9<<6);}
else {GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_h<<6);}*/

#include  <errno.h>
#include  <sys/unistd.h> 				// STDOUT_FILENO, STDERR_FILENO
#include <math.h>

#define STANDBY				0			// Führt kein Programm aus
#define RESET 				1			// Setzt die Ticksgesamt auf 0
#define GERADEAUSFAHRT		2			// Gibt an was die Odometrie überprüft
#define PUNKTDREHUNG		3			// Überwacht die Drehung auf einem Punkt
#define BOGENFAHRT			4			// Überwacht die Drehung um einen Bezugspunkt
#define KOORDINATENRESET	5			// Setzt das Koordinatensystem auf (0|0|0)
#define ABSTANDSMESSUNGR	11			// Testprogramme
#define ZIELANFAHRT			12
#define WINKELMESSUNG		13
										// Drehrichtung Definitions
										// physische Abmessungen
//#define RADDURCHMESSERMM 	74.37		// Angegebener Durchmesser 72 mm aber gespannt groesser
//#define RADABSTANDMM 		329			// Abstand der Odometrieraeder
#define TOLERANZ			1			// Fehlertoleranz des Odometrie Systems in Promille
//#define PI 					3.141592653589793
//#define TICKSPROUMDREHUNG	4000		// Encoderaufloesung pro Umdrehung
//#define TIMERINTERVALL      0.02		// Gibt den zeitlichen Abstand zweier Timerinterrupts in Sekunden an.
										// 7 Segment Definitions
#define KOORDINATENVORGABE		1
#define PROGRAMMAUSWAHL			2
#define KOORDINATENABFRAGE		3
#define GESCHWINDIGKEITSABFRAGE 4
#define FEHLERABFRAGE			5
#define FORTSCHRITT				6



#define DISP_CLEARE 		0b0000000
#define DISP_1 				0b0000110
#define DISP_2 				0b1011011
#define DISP_3 				0b1001111
#define DISP_4 				0b1100110
#define DISP_5 				0b1101101
#define DISP_6 				0b1111101
#define DISP_7 				0b0000111
#define DISP_8 				0b1111111
#define DISP_9 				0b1101111
#define DISP_0 				0b0111111
#define DISP_r 				0b1010000
#define DISP_l 				0b0111000
#define DISP_h 				0b1110100
#define DISP_korrekt 		0b0011110

#define I2C_ADDRESS    0x01
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
// Kommunikation

uint8_t FunctionCode;
uint8_t aTxBuffer[TXBUFFERSIZE] ;
/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];


//Variablen für Streckenberechnung
double 	dFahrtstreckeLinksmm;
double	dFahrtstreckeRechtsmm;
double 	dFahrtstreckeMittelmm;

double 	dwinkellinks;						// Grad
double 	dwinkelrechts;						// Grad
double 	droboterwinkel;						// Grad

double 	ZielstreckeLinksmm;
double 	ZielstreckeRechtsmm;
double 	Zielroboterwinkel;					// Grad


double 	quadratgroessemm;					// Test für das Koordinatensystem
double  testwinkel;							// Test für den Roboterwinkel
int16_t	testbluetooth_ink;
int32_t schleifenzaehler;

double 	abstandzumbezugspunktmm;			// Für Bogenfahrt
double 	bogenfahrtkonstante;				// Für Bogenfahrt

float 	GeschwindigkeitLinksmms;			// Für Programm velocity In mm pro Sekunde
float 	GeschwindigkeitRechtsmms;			// Für Programm velocity
float 	Lineargeschwindigkeitmms;			// Für Programm velocity
float 	Winkelgeschwindigkeitgrads;			// Für Programm velocity In Grad pro Sekunde

double 	x_koordinatemm;						// Variablen für das Koordinatensystem
double 	y_koordinatemm;
double 	alpha_koordinategrad;
double 	alpha_koordinateradiant;

double 	winkelvproTick;						//Variablen in denen aus physikalischen Abmessungen des Systems Konstanten berechnet werden.
double 	winkelvproTickpunktdrehung;
double 	xneufall2;
double 	yneufall2;
double 	xneufall3;
double 	yneufall3;
double 	xneufall6;
double 	yneufall6;
double 	xneufall7;
double 	yneufall7;
double 	streckeprotick;

int8_t 	Programmauswahl;						// Programmauswahl
int8_t 	Fehler;								// Variable um einen Fehler zu speichern, der sich auf das Ziel bezieht
int8_t 	FehlerTyp2;							// Variable um einen Fehler zu speichern, der sich auf die aktuelle Fahrtstrecke bezieht


uint8_t 	EncoderwertRechts;					// Variablen der Encoder
int32_t 	TicksgesamtRechts;
int32_t 	TicksgesamtRechtsalt;
int32_t 	TicksgesamtRechtsneu;
int8_t 		ChangeRechts;						// Für das Koordinatensystem

uint8_t 	EncoderwertLinks;
int32_t 	TicksgesamtLinks;
int32_t 	TicksgesamtLinksalt;
int32_t 	TicksgesamtLinksneu;
int8_t 		ChangeLinks;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void initialisierung();
void konstantenberechnung();
void read_encoder();
void calc_encoder();
void koordinatenberechnung();
void reset();
void geradeausfahrt();
void punktdrehung();
void bogenfahrt();
void koordinatenreset();
void abstandsmessungrechts();
void zielanfahrtrechts();
void winkelmessunglinks();

HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    uint8_t i;
    int32_t x_Koord = 0;
    int32_t y_Koord = 0;
    int32_t winkel =  0;

    switch(FunctionCode){
    case KOORDINATENVORGABE:            //Koordinatenvorgabe
        for(i=1;i<5;i++){
            x_Koord = x_Koord | aRxBuffer[i];
            if(i<4)
            x_Koord = x_Koord << 8;
        }
        for(i=5;i<9;i++){
            y_Koord &= aRxBuffer[i];
            y_Koord = y_Koord << 8;
        }
        for(i=9;i<17;i++){
            winkel &= aRxBuffer[i];
            winkel = winkel<< 8;
        }
        x_koordinatemm = (double)x_Koord/1000000;
        y_koordinatemm = (double)y_Koord/1000000;
        alpha_koordinategrad =(double)winkel/100;
        break;

    case PROGRAMMAUSWAHL:			break;
    case KOORDINATENABFRAGE:		break;
    case GESCHWINDIGKEITSABFRAGE: 	break;
    case FEHLERABFRAGE:				break;
    case FORTSCHRITT:				break;
    default:						break;
        ;
    }
    if(HAL_I2C_Slave_Transmit_IT(&hi2c1, (uint8_t*)aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
            {
              /* Transfer error in transmission process */
              Error_Handler();
            }
}

//for debug:
void display_out(uint8_t in);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
int _write(int file, char *data, int len)						// Für das Überladen der printf
{
   if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
   {
      errno = EBADF;
      return -1;
   }

   // arbitrary timeout 1000
   HAL_StatusTypeDef status =
      HAL_UART_Transmit(&huart2, (uint8_t*)data, len, 1000);

   // return # of bytes written - as best we can tell
   return (status == HAL_OK ? len : 0);
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	  initialisierung();							//Enthält HAL GPIO Timer und andere Initialisierung
	  konstantenberechnung();						// Berechnet Konstanten, die sich aus den Abmessungen des Roboters ergeben

	  Programmauswahl = STANDBY;					//!!!!PROGRAMMAUSWAHL!!!

	  ZielstreckeLinksmm = 0;						//Für Geradeausfahrt. Roboterziel
	  ZielstreckeRechtsmm = 0;
	  Zielroboterwinkel = 0;						//Für Punktdrehung und Bogenfahrt

	  abstandzumbezugspunktmm = 500;				//Für Bogenfahrt
	  testwinkel = 360;								// Um Winkelberechnung des Koordinatensystems zu testen
	  quadratgroessemm = 600;						//Zum Koordinatensystemtest
	  testbluetooth_ink = 0;						//Regelt in welchen Zeitabständen der HC 06 sendet
	 schleifenzaehler = 0;							//Zählt die Schleifendurchläufe zwischen zwei Timer Interrupts


  /* USER CODE END 1 */



  while (1)
  {
  /* USER CODE END WHILE */
	  /*##-2- Put I2C peripheral in reception process ###########################*/
		        if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY){
		      if(HAL_I2C_Slave_Receive_IT(&hi2c1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
		      {
		        /* Transfer error in reception process */
		        Error_Handler();
		      }
		    }
  /* USER CODE BEGIN 3 */



	  read_encoder();
	  calc_encoder();
	  koordinatenberechnung();
	  ++schleifenzaehler;
	  if(testbluetooth_ink>199)
	  	  {
		  testbluetooth_ink = 0;
		  printf("X-Koordinate: %f Y-Koordinate: %f Roboterwinkel: %f Schleife: %i \n\r",(float)x_koordinatemm, (float)y_koordinatemm, (float)alpha_koordinategrad, schleifenzaehler);
	  	  schleifenzaehler = 0;
	  	  }

	  switch (Programmauswahl)
	  {

	  	  case STANDBY:											break;
	  	  case RESET:  				reset();			 		break;
	  	  case GERADEAUSFAHRT: 		geradeausfahrt();	 		break;
	  	  case PUNKTDREHUNG: 		punktdrehung();		 		break;
	  	  case BOGENFAHRT: 			bogenfahrt();		 		break;
	  	  case KOORDINATENRESET:	koordinatenreset();			break;
	  	  case ABSTANDSMESSUNGR: 	abstandsmessungrechts(); 	break;
	  	  case ZIELANFAHRT: 		zielanfahrtrechts();		break;
	  	  case WINKELMESSUNG:		winkelmessunglinks();		break;
	  	  default: 												break;
	  }
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 4;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 8401;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 201;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA6 PA7 PA8 
                           PA9 PA10 PA11 PA12 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void initialisierung()
{

	  HAL_Init();

	  SystemClock_Config();

	  MX_GPIO_Init();
	  MX_I2C1_Init();
	  MX_TIM2_Init();
	  MX_USART2_UART_Init();

	  HAL_TIM_Base_Start_IT(&htim2);

	  GeschwindigkeitLinksmms = 0;					// Variablen der Funktion zur Geschwindigkeitsberechnung
	  GeschwindigkeitRechtsmms = 0;
	  Lineargeschwindigkeitmms = 0;
	  Winkelgeschwindigkeitgrads = 0;

	  dFahrtstreckeLinksmm = 0;
	  dFahrtstreckeRechtsmm = 0;
	  dFahrtstreckeMittelmm = 0;

	  dwinkellinks = 0;
	  dwinkelrechts = 0;
	  droboterwinkel = 0;

	  ZielstreckeLinksmm = 0;
	  ZielstreckeRechtsmm = 0;
	  Zielroboterwinkel = 0;
	  quadratgroessemm = 0;
	  abstandzumbezugspunktmm = 0;
	  bogenfahrtkonstante = 0;

	  x_koordinatemm = 0;
	  y_koordinatemm = 0;
	  alpha_koordinategrad = 0;
	  alpha_koordinateradiant = 0;

	  Programmauswahl = STANDBY;
	  Fehler = 0;										//Bezieht sich auf die Endposition
	  FehlerTyp2 = 0;									//Bezieht sich auf die aktuelle Stellung

	  EncoderwertRechts = 0;
	  TicksgesamtRechts = 0;
	  TicksgesamtRechtsalt = 0;
	  TicksgesamtRechtsneu = 0;
	  ChangeRechts = 0;

	  EncoderwertLinks = 0;
	  TicksgesamtLinks = 0;
	  TicksgesamtLinksalt = 0;
	  TicksgesamtLinksneu = 0;
	  ChangeLinks = 0;
}
void konstantenberechnung()
{
winkelvproTick = (180.0 * RADDURCHMESSERMM)/(TICKSPROUMDREHUNG * RADABSTANDMM);
winkelvproTickpunktdrehung = (360 *RADDURCHMESSERMM)/(RADABSTANDMM * TICKSPROUMDREHUNG);
xneufall2 = (RADABSTANDMM/2) * (1 - cos(winkelvproTick * PI / 180.0));
yneufall2 = ((-1) * RADABSTANDMM/2) * sin(winkelvproTick * PI / 180.0);
xneufall3 = (RADABSTANDMM/2) * (cos(winkelvproTick * PI / 180.0) - 1);
yneufall3 = ((-1) * RADABSTANDMM/2) * sin(winkelvproTick * PI / 180.0);
xneufall6 = (RADABSTANDMM/2) * (cos(winkelvproTick * PI / 180.0) - 1);
yneufall6 = (RADABSTANDMM/2) * sin(winkelvproTick * PI / 180.0);
xneufall7 = (RADABSTANDMM/2) * (1 - cos(winkelvproTick * PI / 180.0));
yneufall7 = (RADABSTANDMM/2) * sin(winkelvproTick * PI / 180.0);
streckeprotick = (PI * RADDURCHMESSERMM)/TICKSPROUMDREHUNG;

}

void read_encoder()													// schiebt Encoderwerte weiter und fügt neue Werte ein
{
	EncoderwertLinks = (EncoderwertLinks<<2) & 0x0C;
	EncoderwertLinks |= (uint8_t)(((GPIOB->IDR)>>12) & 0x03);
	EncoderwertRechts = (EncoderwertRechts<<2) & 0x0C;
	EncoderwertRechts |=(uint8_t)(((GPIOB->IDR)>>14) & 0x03);
}
void calc_encoder()
{
	if(EncoderwertLinks == 0x07 ||EncoderwertLinks == 0x0E||EncoderwertLinks == 0x08||EncoderwertLinks == 0x01)
	{++TicksgesamtLinks ; ChangeLinks = 1;}
	else if (EncoderwertLinks == 0x02||EncoderwertLinks == 0x0B||EncoderwertLinks == 0x0D||EncoderwertLinks == 0x04)
	{--TicksgesamtLinks; ChangeLinks =-1;}
	else if((EncoderwertLinks&0x03)==(EncoderwertLinks>>2))
	{ ChangeLinks = 0;}
	else
	{ChangeLinks = 0;}

	if(EncoderwertRechts == 0x07 ||EncoderwertRechts == 0x0E||EncoderwertRechts == 0x08||EncoderwertRechts == 0x01)
	{--TicksgesamtRechts ; ChangeRechts = -1;}
	else if (EncoderwertRechts == 0x02||EncoderwertRechts == 0x0B||EncoderwertRechts == 0x0D||EncoderwertRechts == 0x04)
	{++TicksgesamtRechts; ChangeRechts = 1;}
	else if((EncoderwertRechts&0x03)==(EncoderwertRechts>>2))
	{ ChangeRechts = 0;}
	else
	{ChangeRechts = 0;}
}
void koordinatenberechnung()
{
alpha_koordinateradiant = alpha_koordinategrad * PI / 180.0;

if (ChangeLinks == -1 && ChangeRechts == -1)				//Fährt zurück
{
	x_koordinatemm = x_koordinatemm + streckeprotick * sin(alpha_koordinateradiant);
	y_koordinatemm = y_koordinatemm - streckeprotick * cos(alpha_koordinateradiant);
}
else if(ChangeLinks == -1 && ChangeRechts == 0)				//Linkes Rad fährt zurück
{
	x_koordinatemm = x_koordinatemm - xneufall2 * cos((-1)* alpha_koordinateradiant) - yneufall2 * sin((-1)* alpha_koordinateradiant);
	y_koordinatemm = y_koordinatemm - xneufall2 * sin((-1)* alpha_koordinateradiant) + yneufall2 * cos((-1)* alpha_koordinateradiant);
	alpha_koordinategrad = alpha_koordinategrad - winkelvproTick;
}
else if(ChangeLinks == 0 && ChangeRechts == -1)				//Rechtes Rad fährt zurück
{
	x_koordinatemm = x_koordinatemm - xneufall3 * cos((-1)* alpha_koordinateradiant) - yneufall3 * sin((-1)* alpha_koordinateradiant);
	y_koordinatemm = y_koordinatemm - xneufall3 * sin((-1)* alpha_koordinateradiant) + yneufall3 * cos((-1)* alpha_koordinateradiant);
	alpha_koordinategrad = alpha_koordinategrad + winkelvproTick;
}
else if(ChangeLinks == -1 && ChangeRechts == 1)				//dreht sich gegen den Uhrzeigersinn
{
	alpha_koordinategrad = alpha_koordinategrad - winkelvproTickpunktdrehung;
}
else if(ChangeLinks == 1 && ChangeRechts == -1)				//dreht sich im Uhrzeigersinn
{
	alpha_koordinategrad = alpha_koordinategrad + winkelvproTickpunktdrehung;
}
else if(ChangeLinks == 0 && ChangeRechts == 1)				//Rechtes Rad fährt vorwärts
{
	x_koordinatemm = x_koordinatemm - xneufall6 * cos((-1)* alpha_koordinateradiant) - yneufall6 * sin((-1)* alpha_koordinateradiant);
	y_koordinatemm = y_koordinatemm - xneufall6 * sin((-1)* alpha_koordinateradiant) + yneufall6 * cos((-1)* alpha_koordinateradiant);
	alpha_koordinategrad = alpha_koordinategrad - winkelvproTick;
}
else if(ChangeLinks == 1 && ChangeRechts == 0)				//Linkes Rad fährt vorwärts
{
	x_koordinatemm = x_koordinatemm - xneufall7 * cos((-1)* alpha_koordinateradiant) - yneufall7 * sin((-1)* alpha_koordinateradiant);
	y_koordinatemm = y_koordinatemm - xneufall7 * sin((-1)* alpha_koordinateradiant) + yneufall7 * cos((-1)* alpha_koordinateradiant);
	alpha_koordinategrad = alpha_koordinategrad + winkelvproTick;
}
else if(ChangeLinks == 1 && ChangeRechts == 1)				//Fährt vorwärts
{
	x_koordinatemm = x_koordinatemm - streckeprotick * sin(alpha_koordinateradiant);
	y_koordinatemm = y_koordinatemm + streckeprotick * cos(alpha_koordinateradiant);
}
else {}

/*if (alpha_koordinategrad > testwinkel * (1000 - 0.1 * TOLERANZ)/1000 && alpha_koordinategrad < testwinkel * (1000 + 0.1 * TOLERANZ)/1000  )		// Test des Koordinatensystems. In welchem Teil des Spielfelds befindet sich der Robo
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_0<<6);}
else if(alpha_koordinategrad > testwinkel * (1000 - TOLERANZ)/1000 && alpha_koordinategrad < testwinkel * (1000 + TOLERANZ)/1000 )
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_1<<6);}
else if(alpha_koordinategrad > testwinkel * (1000 - 2 * TOLERANZ)/1000 && alpha_koordinategrad < testwinkel * (1000 + 2 * TOLERANZ)/1000  )
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_2<<6);}
else if(alpha_koordinategrad > testwinkel * (1000 - 3 * TOLERANZ)/1000 && alpha_koordinategrad < testwinkel * (1000 + 3 * TOLERANZ)/1000 )
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_3<<6);}
else if(alpha_koordinategrad > testwinkel * (1000 - 4 * TOLERANZ)/1000 && alpha_koordinategrad < testwinkel * (1000 + 4 * TOLERANZ)/1000 )
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_4<<6);}
else if(alpha_koordinategrad > testwinkel * (1000 - 5 * TOLERANZ)/1000 && alpha_koordinategrad < testwinkel * (1000 + 5 * TOLERANZ)/1000 )
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_5<<6);}
else if(alpha_koordinategrad > testwinkel * (1000 - 6 * TOLERANZ)/1000 && alpha_koordinategrad < testwinkel * (1000 + 6 * TOLERANZ)/1000 )
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_6<<6);}
else if(alpha_koordinategrad > testwinkel * (1000 - 7 * TOLERANZ)/1000 && alpha_koordinategrad < testwinkel * (1000 + 7 * TOLERANZ)/1000 )
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_7<<6);}
else if(alpha_koordinategrad > testwinkel * (1000 - 8 * TOLERANZ)/1000 && alpha_koordinategrad < testwinkel * (1000 + 8 * TOLERANZ)/1000 )
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_8<<6);}
else if(alpha_koordinategrad > testwinkel * (1000 - 9 * TOLERANZ)/1000 && alpha_koordinategrad < testwinkel * (1000 + 9 * TOLERANZ)/1000 )
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_9<<6);}
else
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_l<<6);}*/

/*if (x_koordinatemm < ((-1) * quadratgroessemm/6) && y_koordinatemm >(quadratgroessemm/6) )		// Test des Koordinatensystems. In welchem Teil des Spielfelds befindet sich der Robo
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_1<<6);}
else if((x_koordinatemm > ((-1) * quadratgroessemm/6) && x_koordinatemm < ( quadratgroessemm/6)) && y_koordinatemm >(quadratgroessemm/6))
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_2<<6);}
else if(x_koordinatemm < ((-1) * quadratgroessemm/6) && (y_koordinatemm >( (-1) * quadratgroessemm/6) && y_koordinatemm < (quadratgroessemm/6)) )
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_3<<6);}
else if(x_koordinatemm > (quadratgroessemm/6) && y_koordinatemm >(quadratgroessemm/6))
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_4<<6);}
else if(x_koordinatemm < ((-1) * quadratgroessemm/6) && y_koordinatemm <((-1) * quadratgroessemm/6))
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_5<<6);}
else if(x_koordinatemm > (quadratgroessemm/6) && (y_koordinatemm >( (-1) * quadratgroessemm/6) && y_koordinatemm < (quadratgroessemm/6)))
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_6<<6);}
else if((x_koordinatemm > ((-1) * quadratgroessemm/6) && x_koordinatemm < ( quadratgroessemm/6)) && y_koordinatemm < ((-1) * quadratgroessemm/6))
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_7<<6);}
else if(x_koordinatemm > (quadratgroessemm/6) && y_koordinatemm <((-1) * quadratgroessemm/6))
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_8<<6);}
else
{GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_0<<6);}*/

}
void reset()
{
	dFahrtstreckeLinksmm = 		0;
	dFahrtstreckeRechtsmm = 	0;
	dFahrtstreckeMittelmm = 	0;

	dwinkellinks = 				0;
	dwinkelrechts = 			0;
	droboterwinkel = 			0;

	ZielstreckeLinksmm = 		0;
	ZielstreckeRechtsmm = 		0;
	Zielroboterwinkel = 		0;

	abstandzumbezugspunktmm = 	0;
	bogenfahrtkonstante = 		0;

	Programmauswahl = 			STANDBY;
 	Fehler = 					0;
	FehlerTyp2 = 				0;

	EncoderwertRechts = 		0;
	TicksgesamtRechts = 		0;
	TicksgesamtRechtsalt = 		0;
	TicksgesamtRechtsneu = 		0;
	ChangeRechts = 				0;

	EncoderwertLinks = 			0;
	TicksgesamtLinks = 			0;
	TicksgesamtLinksalt = 		0;
	TicksgesamtLinksneu = 		0;
	ChangeLinks = 				0;
}
void geradeausfahrt()
{
	dFahrtstreckeRechtsmm 	= ((double)TicksgesamtRechts/TICKSPROUMDREHUNG) * PI *RADDURCHMESSERMM;
	dFahrtstreckeLinksmm 	= ((double)TicksgesamtLinks/TICKSPROUMDREHUNG) * PI *RADDURCHMESSERMM;


    if (dFahrtstreckeLinksmm <  dFahrtstreckeRechtsmm * (1000 - TOLERANZ)/1000)					//Gibt den Laufzeitfehler aus
	{
	FehlerTyp2 = 1;
	}
    else if (dFahrtstreckeLinksmm >  dFahrtstreckeRechtsmm * (1000 + TOLERANZ)/1000)
    {
    FehlerTyp2= 2;
    }
    else
    {
    FehlerTyp2 = 0;
    }

    if (dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 - TOLERANZ)/1000 &&  dFahrtstreckeRechtsmm < ZielstreckeRechtsmm * (1000 - TOLERANZ)/1000)		//Fehler 1. Gibt die Typ1 Fehler aus
    {
    	Fehler= 1;
    	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_1<<6);
    }
    else if(dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 - TOLERANZ)/1000 && (( dFahrtstreckeRechtsmm > ZielstreckeRechtsmm * (1000 - TOLERANZ)/1000) && ( dFahrtstreckeRechtsmm < ZielstreckeRechtsmm * (1000 + TOLERANZ)/1000))) // Fehler 2
    {
    	Fehler= 2;
    	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_2<<6);
    }
    else if(((dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 - TOLERANZ)/1000) && (dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 + TOLERANZ)/1000)) &&  dFahrtstreckeRechtsmm < ZielstreckeRechtsmm * (1000 - TOLERANZ)/1000) // Fehler 3
       {
       	Fehler= 3;
       	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_3<<6);
       }
    else if(dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 - TOLERANZ)/1000 &&  dFahrtstreckeRechtsmm > ZielstreckeRechtsmm * (1000 + TOLERANZ)/1000) // Fehler 4
       {
       	Fehler= 4;
       	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_4<<6);
       }
    else if(dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 + TOLERANZ)/1000 &&  dFahrtstreckeRechtsmm < ZielstreckeRechtsmm * (1000 - TOLERANZ)/1000) // Fehler 5
     {
     	Fehler= 5;
     	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_5<<6);
     }
     else if(((dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 - TOLERANZ)/1000) && (dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 + TOLERANZ)/1000)) && dFahrtstreckeRechtsmm > ZielstreckeRechtsmm * (1000 + TOLERANZ)/1000) // Fehler 6
        {
        	Fehler= 6;
        	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_6<<6);
        }
     else if(dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 + TOLERANZ)/1000 && (( dFahrtstreckeRechtsmm > ZielstreckeRechtsmm * (1000 - TOLERANZ)/1000) && ( dFahrtstreckeRechtsmm < ZielstreckeRechtsmm * (1000 + TOLERANZ)/1000)) ) // Fehler 7
        {
        	Fehler= 7;
        	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_7<<6);
        }
     else if(dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 + TOLERANZ)/1000 &&  dFahrtstreckeRechtsmm > ZielstreckeRechtsmm * (1000 + TOLERANZ)/1000) // Fehler 8
      {
      	Fehler= 8;
      	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_8<<6);
      }
      else																																											// Fehler 0
      {
    	  Fehler= 0;
    	  GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_0<<6);
      }
}
void punktdrehung()
{
	dFahrtstreckeRechtsmm 	= ((double)TicksgesamtRechts/TICKSPROUMDREHUNG) * PI *RADDURCHMESSERMM;
	dFahrtstreckeLinksmm 	= ((double)TicksgesamtLinks/TICKSPROUMDREHUNG) * PI *RADDURCHMESSERMM;
	dFahrtstreckeMittelmm 	= (dFahrtstreckeLinksmm - dFahrtstreckeRechtsmm)/2; 							// Winkel im Uhrzeigersinn positiv
	droboterwinkel 			= (dFahrtstreckeMittelmm * 360)/(PI * RADABSTANDMM);
	ZielstreckeLinksmm 		= (Zielroboterwinkel * PI * RADABSTANDMM)/ 360;
	ZielstreckeRechtsmm 	= (Zielroboterwinkel * PI * RADABSTANDMM * (-1))/ 360;

    if (dFahrtstreckeLinksmm < (-1) * dFahrtstreckeRechtsmm * (1000 - TOLERANZ)/1000)					//Gibt den Laufzeitfehler aus
	{
	FehlerTyp2 = 1;
	}
    else if (dFahrtstreckeLinksmm > (-1) * dFahrtstreckeRechtsmm * (1000 + TOLERANZ)/1000)
    {
    FehlerTyp2= 2;
    }
    else
    {
    FehlerTyp2 = 0;
    }

    if (dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 - TOLERANZ)/1000 && (-1) * dFahrtstreckeRechtsmm < ZielstreckeRechtsmm *(-1)* (1000 - TOLERANZ)/1000)		//Fehler 1. Gibt die Typ1 Fehler aus
    {
    	Fehler= 1;
    	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_1<<6);
    }
    else if(dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 - TOLERANZ)/1000 && (((-1) * dFahrtstreckeRechtsmm > ZielstreckeRechtsmm *(-1)* (1000 - TOLERANZ)/1000) && ((-1) * dFahrtstreckeRechtsmm < ZielstreckeRechtsmm *(-1)* (1000 + TOLERANZ)/1000))) // Fehler 2
    {
    	Fehler= 2;
    	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_2<<6);
    }
    else if(((dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 - TOLERANZ)/1000) && (dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 + TOLERANZ)/1000)) && (-1) * dFahrtstreckeRechtsmm < ZielstreckeRechtsmm *(-1)* (1000 - TOLERANZ)/1000) // Fehler 3
       {
       	Fehler= 3;
       	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_3<<6);
       }
    else if(dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 - TOLERANZ)/1000 && (-1) * dFahrtstreckeRechtsmm > ZielstreckeRechtsmm *(-1)* (1000 + TOLERANZ)/1000) // Fehler 4
       {
       	Fehler= 4;
       	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_4<<6);
       }
    else if(dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 + TOLERANZ)/1000 && (-1) * dFahrtstreckeRechtsmm < ZielstreckeRechtsmm *(-1)* (1000 - TOLERANZ)/1000) // Fehler 5
     {
     	Fehler= 5;
     	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_5<<6);
     }
     else if(((dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 - TOLERANZ)/1000) && (dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 + TOLERANZ)/1000)) && (-1) * dFahrtstreckeRechtsmm > ZielstreckeRechtsmm *(-1)* (1000 + TOLERANZ)/1000) // Fehler 6
        {
        	Fehler= 6;
        	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_6<<6);
        }
     else if(dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 + TOLERANZ)/1000 && (((-1) * dFahrtstreckeRechtsmm > ZielstreckeRechtsmm *(-1)* (1000 - TOLERANZ)/1000) && ((-1) * dFahrtstreckeRechtsmm < ZielstreckeRechtsmm *(-1)* (1000 + TOLERANZ)/1000)) ) // Fehler 7
        {
        	Fehler= 7;
        	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_7<<6);
        }
     else if(dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 + TOLERANZ)/1000 && (-1) * dFahrtstreckeRechtsmm > ZielstreckeRechtsmm *(-1)* (1000 + TOLERANZ)/1000) // Fehler 8
      {
      	Fehler= 8;
      	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_8<<6);
      }
      else																																											// Fehler 0
      {
    	  Fehler= 0;
    	  GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_0<<6);
      }

}
void bogenfahrt()
{

ZielstreckeLinksmm = (Zielroboterwinkel * 2 * PI * (abstandzumbezugspunktmm + RADABSTANDMM/2))/360.0;
ZielstreckeRechtsmm = (Zielroboterwinkel * 2 * PI * (abstandzumbezugspunktmm - RADABSTANDMM/2))/360.0;
dFahrtstreckeLinksmm = ((double)TicksgesamtLinks/TICKSPROUMDREHUNG) * PI *RADDURCHMESSERMM;
dFahrtstreckeRechtsmm = ((double)TicksgesamtRechts/TICKSPROUMDREHUNG) * PI *RADDURCHMESSERMM;
// Berechnung des Fehlertyps 2
if (RADABSTANDMM/2 > abstandzumbezugspunktmm * (1000 - TOLERANZ)/1000 && RADABSTANDMM/2 < abstandzumbezugspunktmm * (1000 + TOLERANZ)/1000)
{
	FehlerTyp2 = 0; 		// Fehler des Typs 2 können nicht erkannt werden
}
else if (RADABSTANDMM/2 > (-1) * abstandzumbezugspunktmm * (1000 - TOLERANZ)/1000 && RADABSTANDMM/2 <  (-1) * abstandzumbezugspunktmm * (1000 + TOLERANZ)/1000)
{
	FehlerTyp2 = 0; 	//Fehler des Typs 2 können nicht erkannt werden
}
else
{
	bogenfahrtkonstante = (abstandzumbezugspunktmm - RADABSTANDMM/2)/(abstandzumbezugspunktmm + RADABSTANDMM/2);

	if(dFahrtstreckeRechtsmm < dFahrtstreckeLinksmm * bogenfahrtkonstante * (1000 - TOLERANZ)/1000)
	{
		FehlerTyp2 = 2;
	}
	else if(dFahrtstreckeRechtsmm > dFahrtstreckeLinksmm * bogenfahrtkonstante * (1000 + TOLERANZ)/1000)
	{
		FehlerTyp2 = 1;
	}
	else { FehlerTyp2 = 0;}

}
// Überprüfung des Fehlertyps 1

if (dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 - TOLERANZ)/1000 &&  dFahrtstreckeRechtsmm < ZielstreckeRechtsmm * (1000 - TOLERANZ)/1000)		//Fehler 1. Gibt die Typ1 Fehler aus
{
	Fehler= 1;
	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_1<<6);
}
else if(dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 - TOLERANZ)/1000 && (( dFahrtstreckeRechtsmm > ZielstreckeRechtsmm * (1000 - TOLERANZ)/1000) && ( dFahrtstreckeRechtsmm < ZielstreckeRechtsmm * (1000 + TOLERANZ)/1000))) // Fehler 2
{
	Fehler= 2;
	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_2<<6);
}
else if(((dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 - TOLERANZ)/1000) && (dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 + TOLERANZ)/1000)) &&  dFahrtstreckeRechtsmm < ZielstreckeRechtsmm * (1000 - TOLERANZ)/1000) // Fehler 3
   {
   	Fehler= 3;
   	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_3<<6);
   }
else if(dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 - TOLERANZ)/1000 &&  dFahrtstreckeRechtsmm > ZielstreckeRechtsmm * (1000 + TOLERANZ)/1000) // Fehler 4
   {
   	Fehler= 4;
   	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_4<<6);
   }
else if(dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 + TOLERANZ)/1000 &&  dFahrtstreckeRechtsmm < ZielstreckeRechtsmm * (1000 - TOLERANZ)/1000) // Fehler 5
 {
 	Fehler= 5;
 	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_5<<6);
 }
 else if(((dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 - TOLERANZ)/1000) && (dFahrtstreckeLinksmm < ZielstreckeLinksmm * (1000 + TOLERANZ)/1000)) && dFahrtstreckeRechtsmm > ZielstreckeRechtsmm * (1000 + TOLERANZ)/1000) // Fehler 6
    {
    	Fehler= 6;
    	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_6<<6);
    }
 else if(dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 + TOLERANZ)/1000 && (( dFahrtstreckeRechtsmm > ZielstreckeRechtsmm * (1000 - TOLERANZ)/1000) && ( dFahrtstreckeRechtsmm < ZielstreckeRechtsmm * (1000 + TOLERANZ)/1000)) ) // Fehler 7
    {
    	Fehler= 7;
    	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_7<<6);
    }
 else if(dFahrtstreckeLinksmm > ZielstreckeLinksmm * (1000 + TOLERANZ)/1000 &&  dFahrtstreckeRechtsmm > ZielstreckeRechtsmm * (1000 + TOLERANZ)/1000) // Fehler 8
  {
  	Fehler= 8;
  	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_8<<6);
  }
  else																																											// Fehler 0
  {
	  Fehler= 0;
	  GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_0<<6);
  }
}
void koordinatenreset()
{
	 x_koordinatemm = 0;
	 y_koordinatemm = 0;
	 alpha_koordinategrad = 0;
}
void abstandsmessungrechts()
{
	dFahrtstreckeRechtsmm = ((double)TicksgesamtRechts/TICKSPROUMDREHUNG) * PI *RADDURCHMESSERMM;

	if (dFahrtstreckeRechtsmm < 100)
	{

			GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_0<<6);
	}
	else if (dFahrtstreckeRechtsmm < 200)
		{

			GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_1<<6);
		}
	else if (dFahrtstreckeRechtsmm < 300)
		{

			GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_2<<6);
		}
	else if (dFahrtstreckeRechtsmm < 400)
		{

			GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_3<<6);
		}
	else if (dFahrtstreckeRechtsmm < 500)
		{

			GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_4<<6);
		}
	else if (dFahrtstreckeRechtsmm < 600)
		{

			GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_5<<6);
		}
	else if (dFahrtstreckeRechtsmm < 700)
		{

			GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_6<<6);
		}
	else if (dFahrtstreckeRechtsmm < 800)
		{

			GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_7<<6);
		}
	else if (dFahrtstreckeRechtsmm < 900)
		{

			GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_8<<6);
		}
	else if (dFahrtstreckeRechtsmm < 1000)
		{

			GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_9<<6);
		}
	else
		{

			GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_h<<6);
		}
}
void zielanfahrtrechts()
{
dFahrtstreckeRechtsmm = ((double)TicksgesamtRechts/TICKSPROUMDREHUNG) * PI *RADDURCHMESSERMM;


if (dFahrtstreckeRechtsmm < (ZielstreckeRechtsmm * (1000 - TOLERANZ))/1000)
{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_l<<6);
}
else if(dFahrtstreckeRechtsmm > (ZielstreckeRechtsmm * (1000 + TOLERANZ))/1000)
{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_h<<6);
}
else
{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_korrekt<<6);
}
}
void winkelmessunglinks()
{
	dwinkellinks = ((double)TicksgesamtLinks/TICKSPROUMDREHUNG) * 360;

	if(dwinkellinks < 0)
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_l<<6);
	}
	else if(dwinkellinks <10)
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_0<<6);
	}
	else if(dwinkellinks <20)
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_1<<6);
	}
	else if(dwinkellinks <30)
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_2<<6);
	}
	else if(dwinkellinks <40)
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_3<<6);
	}
	else if(dwinkellinks <50)
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_4<<6);
	}
	else if(dwinkellinks <60)
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_5<<6);
	}
	else if(dwinkellinks <70)
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_6<<6);
	}
	else if(dwinkellinks <80)
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_7<<6);
	}
	else if(dwinkellinks <90)
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_8<<6);
	}
	else if(dwinkellinks <100)
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_9<<6);
	}

	else if(dwinkellinks < (180 *(1000 - TOLERANZ)/1000))
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_r<<6);
	}
	else if(dwinkellinks > (180 *(1000 + TOLERANZ)/1000))
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_h<<6);
	}
	else
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_korrekt<<6);
	}
}																						//Debugfunctions:
void display_out(uint8_t in)
{
	if(in>=0 && in <=9)
	{
		 switch (in)
			  {
				  case 0:	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_0<<6);break;
				  case 1:	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_1<<6);break;
				  case 2:	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_2<<6);break;
				  case 3:	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_3<<6);break;
				  case 4:	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_4<<6);break;
				  case 5:	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_5<<6);break;
				  case 6:	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_6<<6);break;
				  case 7:	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_7<<6);break;
				  case 8:	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_8<<6);break;
				  case 9:	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_9<<6);break;
				  default:	break;
			  }
	}
	else
	{
		GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(DISP_CLEARE<<6);
	}
}

void display_out_bin(uint8_t bin)
{
	GPIOA->ODR = (GPIOA->IDR & 0xFFFFE03F)|(bin<<6);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
