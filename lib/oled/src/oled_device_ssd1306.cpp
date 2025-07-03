#include "oled_device_ssd1306.hpp"

void gpio_clk_enable(uint32_t gpio_group);

oled_device_ssd1306::oled_device_ssd1306(oled_base_interface *interface,
										 GPIO_TypeDef *cs_group,
										 uint32_t cs_pin,
										 GPIO_TypeDef *rst_group,
										 uint32_t rst_pin)
{
	this -> interface = interface;
	this -> cs_group = cs_group;
	this -> cs_pin = cs_pin;
	this -> rst_group = rst_group;
	this -> rst_pin = rst_pin;
	gpio_clk_enable((uint32_t)cs_group);
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


oled_result oled_device_ssd1306::initDevice()
{
    this -> selectDevice();
	this -> resetDevice();
    uint32_t result = oled_result::OLED_SUCCESS;
    result &= this -> interface -> sendCommand(0xAE);	//关闭显示
	result &= this -> interface -> sendCommand(0x8D);	//设置充电泵
	result &= this -> interface -> sendCommand(0x14);
    result &= this -> interface -> sendCommand(0x20);    //设置寻址模式为水平寻址
	result &= this -> interface -> sendCommand(0x00);
	result &= this -> interface -> sendCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	result &= this -> interface -> sendCommand(0x80);
	result &= this -> interface -> sendCommand(0xA8);	//设置多路复用率
	result &= this -> interface -> sendCommand(0x3F);
	result &= this -> interface -> sendCommand(0xD3);	//设置显示偏移
	result &= this -> interface -> sendCommand(0x00);
	result &= this -> interface -> sendCommand(0x40);	//设置显示开始行
	result &= this -> interface -> sendCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	result &= this -> interface -> sendCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置
	result &= this -> interface -> sendCommand(0xDA);	//设置COM引脚硬件配置
	result &= this -> interface -> sendCommand(0x12);
	result &= this -> interface -> sendCommand(0x81);	//设置对比度控制
	result &= this -> interface -> sendCommand(0xCF);
	result &= this -> interface -> sendCommand(0xD9);	//设置预充电周期
	result &= this -> interface -> sendCommand(0xF1);
	result &= this -> interface -> sendCommand(0xDB);	//设置VCOMH取消选择级别
	result &= this -> interface -> sendCommand(0x30);
	result &= this -> interface -> sendCommand(0xA4);	//设置整个显示打开/关闭
	result &= this -> interface -> sendCommand(0xA6);	//设置正常/倒转显示
	result &= this -> interface -> sendCommand(0xAF);
	this -> releaseDevice();
    return (oled_result)result;
}

void oled_device_ssd1306::selectDevice()
{
	LL_GPIO_ResetOutputPin(this -> cs_group,this -> cs_pin);
}

void oled_device_ssd1306::releaseDevice()
{
	LL_GPIO_SetOutputPin(this -> cs_group,this -> cs_pin);
}

void oled_device_ssd1306::resetDevice()
{
	volatile uint32_t i;
	LL_GPIO_SetOutputPin(this -> rst_group,this -> rst_pin);
	for(i = 0;i < 50000;++i);
	LL_GPIO_ResetOutputPin(this -> rst_group,this -> rst_pin);
	for(i = 0;i < 50000;++i);
	LL_GPIO_SetOutputPin(this -> rst_group,this -> rst_pin);
}

