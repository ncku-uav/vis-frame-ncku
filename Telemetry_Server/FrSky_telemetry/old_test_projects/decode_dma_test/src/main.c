int main(void)
{
 /* USER CODE BEGIN 1 */
 /* USER CODE END 1 */
 /* MCU Configuration----------------------------------------------------------*/
 /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
 HAL_Init();
 /* Configure the system clock */
 SystemClock_Config();
 /* Initialize all configured peripherals */
 MX_GPIO_Init();
 MX_DMA_Init();
 MX_DAC_Init();
 MX_TIM6_Init();
 /* USER CODE BEGIN 2 */
 HAL_TIM_Base_Start(&htim6);
 HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
 HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)sine_wave_array, 32, DAC_ALIGN_12B_R);
 /* USER CODE END 2 */
 /* USER CODE BEGIN 3 */
 /* Infinite loop */
 while (1)
 {
 }
 /* USER CODE END 3 */
}  
