#include "oled_interface_ll.hpp"
#include "ll.h"
#include "string.h"



void gpio_clk_enable(uint32_t gpio_group);

void spi_clk_enable(uint32_t spi_per);

void dma_clk_enable(uint32_t dma_per,uint32_t dma_channel);

namespace oled
{
    interface_spi_dma_ll::interface_spi_dma_ll(const spi_config &spi,const dma_config &dma, GPIO_TypeDef *dc_group, uint32_t dc_pin)
    {
        this -> buffer = (void *)_buffer;
        this -> buffer_size = 1024;
        this -> spi = spi.spi;
        this -> dma = dma.dma;
        this -> dma_channel = dma.channel;
        this -> dc_group = dc_group;
        this -> dc_pin = dc_pin;
        gpio_clk_enable((uint32_t)spi.sck_group);
        gpio_clk_enable((uint32_t)spi.sda_group);
        LL_GPIO_InitTypeDef gpio_init = {0};
        gpio_init.Mode = LL_GPIO_MODE_ALTERNATE;
        gpio_init.Speed = LL_GPIO_SPEED_FREQ_HIGH;
        gpio_init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        gpio_init.Pull = LL_GPIO_PULL_NO;
        gpio_init.Alternate = LL_GPIO_AF_5;

        gpio_init.Pin = spi.sck_pin;
        LL_GPIO_Init(spi.sck_group,&gpio_init);

        gpio_init.Pin = spi.sda_pin;
        LL_GPIO_Init(spi.sda_group,&gpio_init);

        gpio_clk_enable((uint32_t)dc_group);
        LL_GPIO_SetPinMode(dc_group,dc_pin,LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinOutputType(dc_group,dc_pin,LL_GPIO_OUTPUT_PUSHPULL);
        LL_GPIO_SetPinSpeed(dc_group,dc_pin,LL_GPIO_SPEED_FREQ_HIGH);

        spi_clk_enable((uint32_t)spi.spi);
        LL_SPI_SetMode(spi.spi,LL_SPI_MODE_MASTER);
        LL_SPI_SetStandard(spi.spi,LL_SPI_PROTOCOL_MOTOROLA);
        LL_SPI_SetClockPhase(spi.spi,LL_SPI_PHASE_1EDGE);
        LL_SPI_SetClockPolarity(spi.spi,LL_SPI_POLARITY_LOW);
        LL_SPI_SetBaudRatePrescaler(spi.spi,LL_SPI_BAUDRATEPRESCALER_DIV32);
        LL_SPI_SetTransferBitOrder(spi.spi,LL_SPI_MSB_FIRST);
        LL_SPI_SetTransferDirection(spi.spi,LL_SPI_FULL_DUPLEX);
        LL_SPI_SetDataWidth(spi.spi,LL_SPI_DATAWIDTH_8BIT);
        LL_SPI_SetNSSMode(spi.spi,LL_SPI_NSS_SOFT);
        LL_SPI_Enable(spi.spi);
        
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMAMUX1);
        dma_clk_enable((uint32_t)dma.dma,dma.channel);
        LL_DMA_ConfigTransfer(dma.dma,dma.channel,LL_DMA_DIRECTION_MEMORY_TO_PERIPH | LL_DMA_MODE_NORMAL | LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_INCREMENT | LL_DMA_MDATAALIGN_BYTE | LL_DMA_PDATAALIGN_BYTE | LL_DMA_PRIORITY_LOW);
        LL_DMA_ConfigAddresses(dma.dma,dma.channel,(uint32_t)this -> buffer,LL_SPI_DMA_GetRegAddr(spi.spi),LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
        switch((uint32_t)spi.spi)
        {
            case SPI1_BASE: LL_DMA_SetPeriphRequest(dma.dma,dma.channel,LL_DMAMUX_REQ_SPI1_TX); break;
            case SPI2_BASE: LL_DMA_SetPeriphRequest(dma.dma,dma.channel,LL_DMAMUX_REQ_SPI2_TX); break;
            case SPI3_BASE: LL_DMA_SetPeriphRequest(dma.dma,dma.channel,LL_DMAMUX_REQ_SPI3_TX); break;
            default: break;
        }
        LL_DMA_EnableIT_TC(dma.dma, dma.channel);
        LL_SPI_EnableDMAReq_TX(spi.spi);
    }

    result interface_spi_dma_ll::sendCommand(uint8_t command)
    {
        if(this -> _status == status::BUSY)
            return result::FAIL;
        this -> _status = status::BUSY;
        LL_GPIO_ResetOutputPin(this -> dc_group,this -> dc_pin);
        LL_SPI_TransmitData8(this -> spi,command);
        while(!LL_SPI_IsActiveFlag_TXE(this -> spi));
        while(LL_SPI_IsActiveFlag_BSY(this -> spi));
        this -> _status = status::IDLE;
        return result::SUCCESS;
    }

    result interface_spi_dma_ll::sendData(uint8_t *buffer, uint32_t size)
    {
        if(this -> _status == status::BUSY)
            return result::FAIL;
        this -> _status = status::BUSY;
        uint32_t min_size = (size > this -> buffer_size) ? this -> buffer_size : size;
        memcpy(this -> buffer,buffer,min_size);
        LL_DMA_SetDataLength(this -> dma,this -> dma_channel,min_size);
        LL_GPIO_SetOutputPin(this -> dc_group,this -> dc_pin);
        LL_DMA_EnableChannel(this -> dma,this -> dma_channel);
        return result::SUCCESS;
    }

    void interface_spi_dma_ll::transferComplete()
    {
        while(LL_SPI_IsActiveFlag_BSY(this -> spi));
        this -> _status = status::IDLE;
    }

}



void gpio_clk_enable(uint32_t gpio_group)
{
    switch(gpio_group)
    {
        case GPIOA_BASE: if(!LL_AHB2_GRP1_IsEnabledClock(LL_AHB2_GRP1_PERIPH_GPIOA)) LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA); break;
        case GPIOB_BASE: if(!LL_AHB2_GRP1_IsEnabledClock(LL_AHB2_GRP1_PERIPH_GPIOB)) LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB); break;
        case GPIOC_BASE: if(!LL_AHB2_GRP1_IsEnabledClock(LL_AHB2_GRP1_PERIPH_GPIOC)) LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC); break;
        case GPIOD_BASE: if(!LL_AHB2_GRP1_IsEnabledClock(LL_AHB2_GRP1_PERIPH_GPIOD)) LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD); break;
        case GPIOE_BASE: if(!LL_AHB2_GRP1_IsEnabledClock(LL_AHB2_GRP1_PERIPH_GPIOE)) LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE); break;
        case GPIOF_BASE: if(!LL_AHB2_GRP1_IsEnabledClock(LL_AHB2_GRP1_PERIPH_GPIOF)) LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF); break;
        case GPIOG_BASE: if(!LL_AHB2_GRP1_IsEnabledClock(LL_AHB2_GRP1_PERIPH_GPIOG)) LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOG); break;
    }
    
}

void spi_clk_enable(uint32_t spi_per)
{
    switch(spi_per)
    {
        case (uint32_t)SPI1_BASE: LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1); break;
        case (uint32_t)SPI2_BASE: LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2); break;
        case (uint32_t)SPI3_BASE: LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3); break;
        default: break;
    }
}

void dma_clk_enable(uint32_t dma_per,uint32_t dma_channel)
{
    switch(dma_per)
    {
        case (uint32_t)DMA1_BASE: LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);NVIC_SetPriority(DMA1_Channel1_IRQn,1);NVIC_EnableIRQ((IRQn_Type)((uint8_t)DMA1_Channel1_IRQn + dma_channel)); break;
        case (uint32_t)DMA2_BASE: LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);NVIC_SetPriority(DMA2_Channel1_IRQn,1);NVIC_EnableIRQ((IRQn_Type)((uint8_t)DMA2_Channel1_IRQn + dma_channel)); break;
        default: break;
    }
}

