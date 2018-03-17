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

static UART_HandleTypeDef UartHandle;

void Error_Handler()
{
}

void  BSP_LED_Off (unsigned char  led)
{
    switch (led) {
        case 1u:HAL_GPIO_WritePin(GPIOI, GPIO_PIN_11, GPIO_PIN_RESET);break;
        default:
             break;
    }
}

void  BSP_LED_On (unsigned char  led)
{
    switch (led) {
        case 1u:HAL_GPIO_WritePin(GPIOI, GPIO_PIN_11, GPIO_PIN_SET);break;
        default:
             break;
    }
}

void  BSP_LED_Tog (unsigned char  led)
{
    switch (led)
    {
        case 1u:HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_11);break;
        default:
             break;
    }
}


void  BSP_LED_Init (void)
{
    GPIO_InitTypeDef  gpio_init;
    /* PF10-LED1 */
    __HAL_RCC_GPIOI_CLK_ENABLE();
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    gpio_init.Pin   = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOI, &gpio_init);
    BSP_LED_Off(1u);
}

void SysTick_Handler(void)
{
    HAL_IncTick ();
}

void main(void)
{
    HAL_Init();
    //SystemClock_Config();
    UartHandle.Instance        = USART1;

    UartHandle.Init.BaudRate   = 115200;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;
      UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    BSP_LED_Init();
    if (HAL_UART_Init(&UartHandle) != HAL_OK)
        Error_Handler();    
    while(1)
    {
        HAL_Delay(1000);    
        BSP_LED_Tog(1);
        printf("yyfish nihao zhonguo\r\n");
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
  HAL_UART_Transmit(&UartHandle, (uint8_t *)buf, count, 0xffffff);
  return count;
}
