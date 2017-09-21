/*******************************************************************************
File    : Gpio.c
Purpose : 
********************************** Includes ***********************************/
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h" 
#include "misc.h" 
#include "stm32f4xx_exti.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"


#include "gpio.h"
#include "board.h"



void gpio_init(void)
{
    
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;
    
    //  GPIO_DeInit(GPIOA);
    //  GPIO_DeInit(GPIOC);
    
    /* Enable GPIOB clock */
    RCC_AHB1PeriphClockCmd(INVEN_INT_GPIO_CLK, ENABLE);
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    /* Configure invensense sensor interrupt pin as input floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Pin = INVEN_INT_PIN;
    GPIO_Init(INVEN_INT_GPIO_PORT, &GPIO_InitStructure); //GPIOA
    
    /* Connect EXTI Line to inv sensor interrupt pin */
    SYSCFG_EXTILineConfig(INVEN_INT_EXTI_PORT, INVEN_INT_EXTI_PIN);
    
    /* Configure EXTI Line1 */
    EXTI_InitStructure.EXTI_Line = INVEN_INT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    /* Enable and set EXTI Line Interrupt to the highest priority */
    NVIC_InitStructure.NVIC_IRQChannel = INVEN_INT_EXTI_IRQ;
    // http://www.freertos.org/RTOS-Cortex-M3-M4.html
    // configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY in FreeRTOSConfig.h
    // Should be lower (higher value) than configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10; // prob dont need to be changed
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void EnableInvInterrupt(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    /* Configure EXTI Line1 */
    EXTI_InitStructure.EXTI_Line = INVEN_INT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
void DisableInvInterrupt(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    /* Configure EXTI Line1 */
    EXTI_InitStructure.EXTI_Line = INVEN_INT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(INVEN_INT_EXTI_LINE);
}

void InvIntHandler(void)
{
    /* Clear the EXTI line 1 pending bit */
    EXTI_ClearITPendingBit(INVEN_INT_EXTI_LINE);
}

void debug_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(DEBUG_GPIO_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DEBUG_GPIO_PORT, &GPIO_InitStructure);
}
