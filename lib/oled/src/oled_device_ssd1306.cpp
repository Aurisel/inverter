#include "oled_device_ssd1306.hpp"



namespace oled
{
	


	result device_ssd1306::initDevice()
	{
		this -> selectDevice();
		this -> resetDevice();
		uint32_t result = result::SUCCESS;
		result &= this -> _interface -> sendCommand(0xAE);	//关闭显示
		result &= this -> _interface -> sendCommand(0x8D);	//设置充电泵
		result &= this -> _interface -> sendCommand(0x14);
		result &= this -> _interface -> sendCommand(0x20);    //设置寻址模式为水平寻址
		result &= this -> _interface -> sendCommand(0x00);
		result &= this -> _interface -> sendCommand(0xD5);	//设置显示时钟分频比/振荡器频率
		result &= this -> _interface -> sendCommand(0x80);
		result &= this -> _interface -> sendCommand(0xA8);	//设置多路复用率
		result &= this -> _interface -> sendCommand(0x3F);
		result &= this -> _interface -> sendCommand(0xD3);	//设置显示偏移
		result &= this -> _interface -> sendCommand(0x00);
		result &= this -> _interface -> sendCommand(0x40);	//设置显示开始行
		result &= this -> _interface -> sendCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
		result &= this -> _interface -> sendCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置
		result &= this -> _interface -> sendCommand(0xDA);	//设置COM引脚硬件配置
		result &= this -> _interface -> sendCommand(0x12);
		result &= this -> _interface -> sendCommand(0x81);	//设置对比度控制
		result &= this -> _interface -> sendCommand(0xCF);
		result &= this -> _interface -> sendCommand(0xD9);	//设置预充电周期
		result &= this -> _interface -> sendCommand(0xF1);
		result &= this -> _interface -> sendCommand(0xDB);	//设置VCOMH取消选择级别
		result &= this -> _interface -> sendCommand(0x30);
		result &= this -> _interface -> sendCommand(0xA4);	//设置整个显示打开/关闭
		result &= this -> _interface -> sendCommand(0xA6);	//设置正常/倒转显示
		result &= this -> _interface -> sendCommand(0xAF);
		this -> releaseDevice();
		return (oled::result)result;
	}

	

}