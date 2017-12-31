 /*
  ******************************************************************************
  * @file    yyfish.c
  * @author  <iysheng@163.com>
  * @version V0.9
  * @date    30-December-2017
  * @brief   yyfish Project main c source
  */
#include <stm32f7xx_hal.h>
#include <stdio.h>

#if 1
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#endif
static UART_HandleTypeDef UartHandle;

#define  BSP_LED0_GPIOB_PIN                       GPIO_PIN_0
#define  BSP_LED1_GPIOB_PIN                       GPIO_PIN_1

void Error_Handler()
{
}

void  BSP_LED_Off (unsigned char  led)
{
    switch (led) {
        case 0u:HAL_GPIO_WritePin(GPIOB, BSP_LED0_GPIOB_PIN, GPIO_PIN_SET);break;
        case 1u:HAL_GPIO_WritePin(GPIOB, BSP_LED1_GPIOB_PIN, GPIO_PIN_SET);break;
        case 2u:HAL_GPIO_WritePin(GPIOI, GPIO_PIN_11, GPIO_PIN_RESET);break;
        default:
             HAL_GPIO_WritePin(GPIOI, GPIO_PIN_11, GPIO_PIN_RESET);
             HAL_GPIO_WritePin(GPIOB, BSP_LED0_GPIOB_PIN, GPIO_PIN_SET);
             HAL_GPIO_WritePin(GPIOB, BSP_LED1_GPIOB_PIN, GPIO_PIN_SET);
             break;
    }
}

void  BSP_LED_On (unsigned char  led)
{
    switch (led) {
        case 0u:HAL_GPIO_WritePin(GPIOB, BSP_LED0_GPIOB_PIN, GPIO_PIN_RESET);break;
        case 1u:HAL_GPIO_WritePin(GPIOB, BSP_LED1_GPIOB_PIN, GPIO_PIN_RESET);break;
        case 2u:HAL_GPIO_WritePin(GPIOI, GPIO_PIN_11, GPIO_PIN_SET);break;
        default:
             HAL_GPIO_WritePin(GPIOI, GPIO_PIN_11, GPIO_PIN_SET);
             HAL_GPIO_WritePin(GPIOB, BSP_LED0_GPIOB_PIN, GPIO_PIN_RESET);
             HAL_GPIO_WritePin(GPIOB, BSP_LED1_GPIOB_PIN, GPIO_PIN_RESET);
             break;
    }
}

void  BSP_LED_Tog (unsigned char  led)
{
	switch (led)
	{
		case 0u:HAL_GPIO_TogglePin(GPIOB, BSP_LED0_GPIOB_PIN);break;
		case 1u:HAL_GPIO_TogglePin(GPIOB, BSP_LED1_GPIOB_PIN);break;
		case 2u:HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_11);break;
		default:
		 	 HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_11);
			 HAL_GPIO_TogglePin(GPIOB, BSP_LED0_GPIOB_PIN);
			 HAL_GPIO_TogglePin(GPIOB, BSP_LED1_GPIOB_PIN);
			 break;
	}
}


void  BSP_LED_Init (void)
{
    GPIO_InitTypeDef  gpio_init;

    __HAL_RCC_GPIOB_CLK_ENABLE();                               /* Enable GPIO clock for LED1(PF10)                     */
    __HAL_RCC_GPIOI_CLK_ENABLE();
                                                                /* Configure the GPIOB for LED1(PF10)                   */
    gpio_init.Pin   = BSP_LED0_GPIOB_PIN|BSP_LED1_GPIOB_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio_init);
    gpio_init.Pin   = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOI, &gpio_init);
    BSP_LED_Off(0u);
    BSP_LED_Off(1u);
    BSP_LED_On(2u);
}

void SysTick_Handler(void)
{
	HAL_IncTick ();
}

void main(void)
{
	char ch='Y';
	HAL_Init();
	//SystemClock_Config();
	UartHandle.Instance        = USART1;

	UartHandle.Init.BaudRate   = 9600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX_RX;
  	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	BSP_LED_Init();
	while(1)
	{
		BSP_LED_Tog(2);
		HAL_Delay(1000);	
		if (HAL_UART_Init(&UartHandle) != HAL_OK)
	  	{
	    	/* Initialization Error */
	    	Error_Handler();	
	  	}
		else
		{
			//printf("yyfish nihao zhonguo\r\n");
	  		HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);
		}
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_Initure;
  if(huart->Instance == USART2)
  {
    __HAL_RCC_USART2_CLK_ENABLE();//USART2时钟使能
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_Initure.Pin=GPIO_PIN_2;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;
    GPIO_Initure.Pull=GPIO_NOPULL;
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW;
    GPIO_Initure.Alternate=GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
    GPIO_Initure.Pin=GPIO_PIN_3;
    GPIO_Initure.Alternate=GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  }
  else if(huart->Instance == USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();//USART1时钟使能
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_Initure.Pin=GPIO_PIN_10;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;
    GPIO_Initure.Pull=GPIO_NOPULL;
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW;
    GPIO_Initure.Alternate=GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    GPIO_Initure.Pin=GPIO_PIN_9; 
    GPIO_Initure.Alternate=GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}


int _write (int fd, const void *buf, size_t count)
{
  (void)fd;
  //HAL_UART_Transmit(&UART_DEBUG, (uint8_t *)buf, count, 0xFFFF);
  HAL_UART_Transmit_IT(&UartHandle, (uint8_t *)buf, count);
  return count;
}

#if 1
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART3 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
#endif
/*
void _exit(int status)
{

}

*/
