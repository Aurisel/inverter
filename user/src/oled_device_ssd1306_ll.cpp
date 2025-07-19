#include "oled_device_ssd1306_ll.hpp"

void gpio_clk_enable(uint32_t gpio_group);

namespace oled
{

    device_ssd1306_ll::device_ssd1306_ll(base::interface *interface,GPIO_TypeDef *cs_group,uint32_t cs_pin,GPIO_TypeDef *rst_group,uint32_t rst_pin)
    {
        this -> _interface = interface;
		this -> cs_group = cs_group;
		this -> cs_pin = cs_pin;
		this -> rst_group = rst_group;
		this -> rst_pin = rst_pin;
		gpio_clk_enable((uint32_t)cs_group);
		gpio_clk_enable((uint32_t)rst_group);
		LL_GPIO_InitTypeDef gpio_init;
		gpio_init.Mode = LL_GPIO_MODE_OUTPUT;
		gpio_init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		gpio_init.Speed = LL_GPIO_SPEED_FREQ_HIGH;
		gpio_init.Pull = LL_GPIO_PULL_NO;
		gpio_init.Pin = cs_pin;
		LL_GPIO_Init(cs_group,&gpio_init);
		gpio_init.Pin = rst_pin;
		LL_GPIO_Init(rst_group,&gpio_init);
		this -> initDevice();
    }

	void device_ssd1306_ll::selectDevice()
	{
		LL_GPIO_ResetOutputPin(this -> cs_group,this -> cs_pin);
	}

	void device_ssd1306_ll::releaseDevice()
	{
		LL_GPIO_SetOutputPin(this -> cs_group,this -> cs_pin);
	}


	void device_ssd1306_ll::resetDevice()
	{
		volatile uint32_t i;
		LL_GPIO_SetOutputPin(this -> rst_group,this -> rst_pin);
		for(i = 0;i < 50000;++i);
		LL_GPIO_ResetOutputPin(this -> rst_group,this -> rst_pin);
		for(i = 0;i < 50000;++i);
		LL_GPIO_SetOutputPin(this -> rst_group,this -> rst_pin);
	}

}