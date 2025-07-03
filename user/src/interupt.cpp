#include "ll.h"


extern "C"
{
    void TIM7_IRQHandler()
    {
        LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_6);
        LL_TIM_ClearFlag_UPDATE(TIM7);
    }
}