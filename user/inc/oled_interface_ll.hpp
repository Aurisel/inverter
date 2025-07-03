#ifndef OLED_LL_G431_HPP
#define OLED_LL_G431_HPP

#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_spi.h"
#include "stm32g4xx_ll_dma.h"

#include "oled.hpp"

struct spi_config
{
    SPI_TypeDef *spi;
    GPIO_TypeDef *sck_group;
    uint32_t sck_pin;
    GPIO_TypeDef *sda_group;
    uint32_t sda_pin;
    spi_config(SPI_TypeDef *spi,GPIO_TypeDef *sck_group,uint32_t sck_pin,GPIO_TypeDef *sda_group,uint32_t sda_pin)
    :spi(spi),sck_group(sck_group),sck_pin(sck_pin),sda_group(sda_group),sda_pin(sda_pin){}
};

struct dma_config
{
    DMA_TypeDef *dma;
    uint32_t channel;
    dma_config(DMA_TypeDef *dma,uint32_t channel)
    :dma(dma),channel(channel){}
};



class oled_interface_spi_dma_ll :public oled_base_interface
{
    private:
        uint32_t _buffer[256];
        DMA_TypeDef *dma;
        uint32_t dma_channel;
        SPI_TypeDef *spi;
        GPIO_TypeDef *dc_group;
        uint32_t dc_pin;
    public:
        void transferComplete();
        oled_interface_spi_dma_ll(const spi_config &spi,const dma_config &dma, GPIO_TypeDef *dc_group, uint32_t dc_pin);
        oled_result sendCommand(uint8_t command);
        oled_result sendData(uint8_t *buffer, uint32_t size);
};

#endif
