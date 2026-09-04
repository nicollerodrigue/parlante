/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint32_t pasos = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN 0 */

/*==============================================================*/
/* INICIALIZAR RETARDO EN MICROSEGUNDOS USANDO DWT               */
/*==============================================================*/
void delay_us_dwt_init(void)
{
    /* Habilitar el sistema de depuracion */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* Reiniciar contador */
    DWT->CYCCNT = 0;

    /* Habilitar contador de ciclos */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    /* Calcular cuantos ciclos hay en 1 microsegundo */
    pasos = HAL_RCC_GetSysClockFreq() / 1000000;

    /* Seguridad */
    if (pasos == 0)
    {
        pasos = 1;
    }
}


/*==============================================================*/
/* RETARDO EN MICROSEGUNDOS                                      */
/*==============================================================*/
void delay_us_dwt(uint32_t reta)
{
    uint32_t inicio = DWT->CYCCNT;
    uint32_t ciclos = pasos * reta;

    while ((DWT->CYCCNT - inicio) < ciclos)
    {
        /* Esperar */
    }
}


/*==============================================================*/
/* GENERAR UNA NOTA MUSICAL                                      */
/* frec = frecuencia en Hz                                       */
/* dura = duracion en milisegundos                               */
/*==============================================================*/
void Sound_play(uint32_t frec, uint32_t dura)
{
    uint32_t dela;
    uint32_t repe;

    /* Evitar division entre cero */
    if (frec == 0)
    {
        HAL_Delay(dura);
        return;
    }

    /* Media onda en microsegundos */
    dela = 500000 / frec;

    /* Cantidad de ciclos completos */
    repe = (frec * dura) / 1000;

    while (repe--)
    {
        /* Nivel alto */
        HAL_GPIO_WritePin(parlante_GPIO_Port,
                          parlante_Pin,
                          GPIO_PIN_SET);

        delay_us_dwt(dela);

        /* Nivel bajo */
        HAL_GPIO_WritePin(parlante_GPIO_Port,
                          parlante_Pin,
                          GPIO_PIN_RESET);

        delay_us_dwt(dela);
    }

    /* Asegurar que termine apagado */
    HAL_GPIO_WritePin(parlante_GPIO_Port,
                      parlante_Pin,
                      GPIO_PIN_RESET);
}


/*==============================================================*/
/* MELODIA MARIO BROS - INICIO                                   */
/*==============================================================*/
void melodia1(void)
{
    /* E5 */
    Sound_play(659, 150);
    HAL_Delay(40);

    /* E5 */
    Sound_play(659, 150);
    HAL_Delay(40);

    /* E5 */
    Sound_play(659, 150);
    HAL_Delay(100);

    /* C5 */
    Sound_play(523, 150);
    HAL_Delay(40);

    /* E5 */
    Sound_play(659, 150);
    HAL_Delay(40);

    /* G5 */
    Sound_play(784, 350);
    HAL_Delay(150);

    /* G4 */
    Sound_play(392, 350);
    HAL_Delay(200);
}


/*==============================================================*/
/* FUNCION CHAO                                                  */
/*==============================================================*/
void chao(void)
{
    GPIOA->ODR = 0b00111001;
    HAL_Delay(1000);

    GPIOA->ODR = 0b01110110;
    HAL_Delay(1000);

    GPIOA->ODR = 0b01110111;
    HAL_Delay(1000);

    GPIOA->ODR = 0b00111111;
    HAL_Delay(1000);
}


/*==============================================================*/
/* FUNCION HOLA                                                  */
/*==============================================================*/
void hola(uint16_t retardo)
{
    GPIOA->ODR = 0b01110110;
    HAL_Delay(retardo);

    GPIOA->ODR = 0b00111111;
    HAL_Delay(retardo);

    GPIOA->ODR = 0b00111000;
    HAL_Delay(retardo);

    GPIOA->ODR = 0b01110111;
    HAL_Delay(retardo);
}

/* USER CODE END 0 */


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();

    /* USER CODE BEGIN 2 */

    /* IMPORTANTE:
       Inicializar el contador DWT antes de reproducir sonido */
    delay_us_dwt_init();

    /* USER CODE END 2 */


    /* Infinite loop */
    while (1)
    {
        melodia1();

        
        /* Pausa antes de repetir la melodia */
        HAL_Delay(500);
    }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Configure the main internal regulator output voltage */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);


    /* Initializes the RCC Oscillators */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;

    RCC_OscInitStruct.HSIState = RCC_HSI_ON;

    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;


    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }


    /* Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType =
                    RCC_CLOCKTYPE_HCLK |
                    RCC_CLOCKTYPE_SYSCLK |
                    RCC_CLOCKTYPE_PCLK1 |
                    RCC_CLOCKTYPE_PCLK2;


    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;

    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;

    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;


    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct,
                            FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
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


    /* Apagar parlante inicialmente */
    HAL_GPIO_WritePin(parlante_GPIO_Port,
                      parlante_Pin,
                      GPIO_PIN_RESET);


    /* Apagar pines del display inicialmente */
    HAL_GPIO_WritePin(GPIOA,
                      DPA_Pin |
                      DPB_Pin |
                      DPC_Pin |
                      DPD_Pin |
                      DPE_Pin |
                      DPF_Pin |
                      DPG_Pin |
                      GPIO_PIN_7,
                      GPIO_PIN_RESET);


    /* Configurar pin del parlante */
    GPIO_InitStruct.Pin = parlante_Pin;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull = GPIO_NOPULL;

    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(parlante_GPIO_Port, &GPIO_InitStruct);


    /* Configurar pines del display */
    GPIO_InitStruct.Pin =
                    DPA_Pin |
                    DPB_Pin |
                    DPC_Pin |
                    DPD_Pin |
                    DPE_Pin |
                    DPF_Pin |
                    DPG_Pin |
                    GPIO_PIN_7;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull = GPIO_NOPULL;

    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  * @param  file: pointer to the source file name
  * @param  line: pointer to the line number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* Puedes colocar codigo de depuracion aqui */
}

#endif /* USE_FULL_ASSERT */