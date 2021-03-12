/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "vl53l0x_class.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

uint8_t conv_cplt=0;
uint8_t i,scaner;
VL53L0X_Dev_t sensor1, sensor2;

HAL_StatusTypeDef result;

// Uncomment ONE of these three lines to get
//#define LONG_RANGE
//#define HIGH_SPEED
#define HIGH_ACCURACY

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

void VL53L0X_Start(VL53L0X_Dev_t *lidar, uint8_t newAddr);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

	return ch;
}

/* round robin lowband fir filter */
#define NBSAMPLES 16

uint16_t rr_fir_filter0(uint16_t newsample) {
	static uint16_t samples[NBSAMPLES];
	static uint16_t numsample=0;
	static uint16_t slidingsum=0;
	slidingsum-= samples[numsample];
	samples[numsample]= newsample;
	slidingsum+= samples[numsample];
	numsample++;
	numsample%=NBSAMPLES;
	return slidingsum/NBSAMPLES;
}

uint16_t rr_fir_filter(uint8_t ch, uint16_t newsample) {
	if (ch>2) return 0;
	static uint16_t samples[2][NBSAMPLES]={0};
	static uint16_t numsample=0;
	static uint16_t slidingsum[2]={0};
	slidingsum[ch] -= samples[ch][numsample];
	samples[ch][numsample]= newsample;
	slidingsum[ch]+= samples[ch][numsample];
	numsample++;
	numsample%=NBSAMPLES;
	return slidingsum[ch]/NBSAMPLES;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */

	printf("Start ...\r\n");


	uint8_t sensor1_newAddr = 0x36;
	uint8_t sensor2_newAddr = 0x30;
	sensor1.I2cDevAddr = 0x29;
	sensor2.I2cDevAddr = 0x29;



	HAL_GPIO_WritePin(VL1_XSHUT_GPIO_Port, VL1_XSHUT_Pin, GPIO_PIN_RESET); //shut down the VL53L0X sensor.
	HAL_Delay(100); //100

	HAL_GPIO_WritePin(VL2_XSHUT_GPIO_Port, VL2_XSHUT_Pin, GPIO_PIN_RESET); //shut down the VL53L0X sensor.
	HAL_Delay(100); //100

	HAL_GPIO_WritePin(VL1_XSHUT_GPIO_Port, VL1_XSHUT_Pin, GPIO_PIN_SET); //start up the sensor.
	HAL_Delay(100);  //24

	VL53L0X_Start(&sensor1, sensor1_newAddr);

	SetDeviceAddress(&sensor1, sensor1_newAddr);
	sensor1.I2cDevAddr = sensor1.I2cDevAddr /2;

	HAL_GPIO_WritePin(VL2_XSHUT_GPIO_Port, VL2_XSHUT_Pin, GPIO_PIN_SET); //start up the sensor.
	HAL_Delay(100);  //24
//
	VL53L0X_Start(&sensor2, sensor2_newAddr);




	//	VL53L0X_Start(&sensor1, sensor1_newAddr);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		uint32_t distance = 0;
		uint32_t avg_dist = 0;
		float alpha = 0.8;
		float beta = (1-alpha);


		int status;
		while(1){

			status=VL53L0X_GetDistance(&sensor1, &distance);
			avg_dist = avg_dist*(alpha) + distance*(beta);

			status=VL53L0X_GetDistance(&sensor2, &distance);
			avg_dist = avg_dist*(alpha) + distance*(beta);

//			status=VL53L0X_GetDistance(&sensor1, &distance);
//			avg_dist = avg_dist*(alpha) + distance*(beta);

//
//			SetDeviceAddress(&sensor1, sensor1_newAddr);
//			sensor1.I2cDevAddr = sensor1.I2cDevAddr /2;

		}

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(VL1_XSHUT_GPIO_Port, VL1_XSHUT_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LD2_Pin */
	GPIO_InitStruct.Pin = LD2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : VL1_XSHUT_Pin */
	GPIO_InitStruct.Pin = VL1_XSHUT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(VL1_XSHUT_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = VL2_XSHUT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(VL2_XSHUT_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void _Error_Handler(char *file, int line)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	char msg[256];
	sprintf(msg,"%s,%d", file,line);
	/* User can add his own implementation to report the HAL error return state */
	while(1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

void VL53L0X_Start(VL53L0X_Dev_t *lidar, uint8_t newAddr){
	lidar->I2cDevAddr=0x29; // ADDRESS_DEFAULT;
	lidar->comms_type=1;    // VL53L0X_COMMS_I2C
	lidar->comms_speed_khz=100;

//	HAL_GPIO_WritePin(VL1_XSHUT_GPIO_Port, VL1_XSHUT_Pin, GPIO_PIN_RESET); //shut down the VL53L0X sensor.
//	HAL_Delay(100); //100
//
//	HAL_GPIO_WritePin(VL1_XSHUT_GPIO_Port, VL1_XSHUT_Pin, GPIO_PIN_SET); //start up the sensor.
//	HAL_Delay(100);  //24

	if(!VL53L0X_InitSensor(lidar, 0x52)) //attempt to initialise it with the necessary settings for normal operation. Returns 0 if fail, 1 if success.
	{
		printf("Failed to initialize\r\n");
	}
	else
	{
		printf("Successfully initialized\r\n");
	}
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while(1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{ 
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
