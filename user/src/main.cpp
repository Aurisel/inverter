#include "init.h"
#include "pid.h"
#include "oled_interface_ll.hpp"
#include "oled_canvas_char.hpp"
#include "oled_device_ssd1306.hpp"
#include "arm_math.h"
#include "printf.h"
#include "stm32g4xx_ll_system.h"

#define CYCLE_COUNT 200
#define CYCLES 10
#define Uout 16.0f

int phase_count = 0;

uint8_t pll_flag = 0;
pid vol_pid(0.008f,0.0f,0.0f,Uout,0.0f);
char buffer[16];
float K = 1.0f;
const float cos_table[CYCLE_COUNT] = {1.0f,0.9995015891261738f,0.9980068533314934f,0.995517282601106f,0.9920353585932578f,0.9875645521655237f,0.9821093199149804f,0.9756750997357736f,0.9682683053985072f,0.959896320156857f,0.9505674893877829f,0.9402911122726756f,0.9290774325277306f,0.9169376281927888f,0.9038838004888236f,0.8899289617551803f,0.8750870224785937f,0.859372777426912f,0.8428018909013506f,0.8253908811219762f,0.8071571037619854f,0.7881187346471924f,0.7682947516379708f,0.7477049157117092f,0.7263697512646394f,0.7043105256526722f,0.6815492279916338f,0.6581085472380377f,0.6340118495722387f,0.6092831551065166f,0.5839471139413063f,0.5580289815934402f,0.5315545938209014f,0.5045503408691775f,0.47704314116488955f,0.44906041448292017f,0.4206300546137844f,0.39178040155849314f,0.36254021327862457f,0.3329386370297613f,0.30300518030687273f,0.2727696814306032f,0.24226227980378318f,0.21151338586781906f,0.18055365078890231f,0.14941393590426094f,0.11812528195890805f,0.08671887816355073f,0.05522603110450817f,0.0236781335366241f,-0.00789336690971331f,-0.0394569990762527f,-0.070981299648016f,-0.10243484451661329f,-0.13378628010447902f,-0.1650043546187993f,-0.196057949203978f,-0.22691610896159004f,-0.2575480738068967f,-0.28792330913116637f,-0.3180115362392381f,-0.34778276253198237f,-0.37720731140357594f,-0.40625585182378904f,-0.434899427575793f,-0.4631094861203477f,-0.4908579070575935f,-0.5181170301580772f,-0.5448596829350705f,-0.5710592077306947f,-0.5966894882888559f,-0.6217249757884951f,-0.6461407143112096f,-0.669912365717855f,-0.6930162339093318f,-0.7154292884473712f,-0.7371291875117788f,-0.7580943001712453f,-0.7783037279455297f,-0.7977373256375193f,-0.8163757214143991f,-0.8342003361179174f,-0.8511934017844945f,-0.8673379793567146f,-0.8826179755685469f,-0.8970181589874635f,-0.9105241751974622f,-0.9231225611078606f,-0.9348007583735982f,-0.9455471259136671f,-0.9553509515151948f,-0.9642024625116117f,-0.972092835524257f,-0.9790142052577144f,-0.9849596723401105f,-0.9899233102005571f,-0.9939001709768878f,-0.9968862904477932f,-0.9988786919844436f,-0.9998753895176573f,-0.9998753895176573f,-0.9988786919844436f,-0.9968862904477932f,-0.9939001709768879f,-0.9899233102005571f,-0.9849596723401105f,-0.9790142052577145f,-0.972092835524257f,-0.9642024625116118f,-0.9553509515151949f,-0.9455471259136671f,-0.9348007583735983f,-0.9231225611078607f,-0.9105241751974622f,-0.8970181589874636f,-0.8826179755685468f,-0.8673379793567146f,-0.8511934017844947f,-0.8342003361179176f,-0.8163757214143992f,-0.7977373256375195f,-0.7783037279455299f,-0.7580943001712456f,-0.737129187511779f,-0.7154292884473714f,-0.693016233909332f,-0.6699123657178551f,-0.6461407143112101f,-0.6217249757884953f,-0.5966894882888565f,-0.571059207730695f,-0.5448596829350704f,-0.5181170301580778f,-0.49085790705759375f,-0.4631094861203483f,-0.4348994275757932f,-0.40625585182378887f,-0.37720731140357633f,-0.3477827625319824f,-0.3180115362392388f,-0.28792330913116665f,-0.25754807380689737f,-0.22691610896159048f,-0.19605794920397804f,-0.16500435461879978f,-0.13378628010447927f,-0.10243484451661397f,-0.07098129964801624f,-0.03945699907625272f,-0.007893366909713777f,0.023678133536623857f,0.055226031104507474f,0.08671887816355048f,0.11812528195890803f,0.14941393590426047f,0.1805536507889021f,0.2115133858678184f,0.24226227980378293f,0.27276968143060315f,0.3030051803068723f,0.33293863702976106f,0.3625402132786239f,0.3917804015584929f,0.42063005461378355f,0.4490604144829198f,0.4770431411648888f,0.5045503408691777f,0.5315545938209012f,0.5580289815934403f,0.5839471139413059f,0.6092831551065166f,0.6340118495722382f,0.6581085472380375f,0.6815492279916333f,0.7043105256526719f,0.7263697512646388f,0.7477049157117087f,0.7682947516379706f,0.7881187346471925f,0.8071571037619851f,0.8253908811219762f,0.8428018909013504f,0.8593727774269119f,0.8750870224785934f,0.8899289617551801f,0.9038838004888231f,0.9169376281927886f,0.9290774325277305f,0.9402911122726756f,0.9505674893877828f,0.959896320156857f,0.9682683053985071f,0.9756750997357736f,0.9821093199149803f,0.9875645521655236f,0.9920353585932578f,0.9955172826011058f,0.9980068533314933f,0.9995015891261738f,1.0f};
oled_interface_spi_dma_ll oled_interface(spi_config(SPI1,GPIOA,LL_GPIO_PIN_5,GPIOA,LL_GPIO_PIN_7),dma_config(DMA1,LL_DMA_CHANNEL_1),GPIOB,LL_GPIO_PIN_10);
oled_canvas_char_small canvas;
oled_device_ssd1306 oled_device(&oled_interface,GPIOB,LL_GPIO_PIN_11,GPIOB,LL_GPIO_PIN_2);
uint16_t adc_buffer[2];

inline void Inverter_Start()
{
    LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_14);
}

int main()
{
    /* LL_VREFBUF_Enable() */
    Sys_Config();
    GPIO_Config();
    PWM_Config();
    ADC_Config(adc_buffer);
    
    TIM7_Config();
    oled_device.flush(canvas);
    LL_TIM_EnableCounter(TIM1);
    LL_TIM_EnableCounter(TIM6);
    Inverter_Start();
    GPIO_Int_Config();
    for(volatile int i = 0;i < 500000;++i);
    LL_TIM_EnableCounter(TIM7);
    while(true)
    {
        volatile uint32_t i;
        for(i = 0;i < 100000;++i);
        oled_device.flush(canvas);
    }
}

int calc_compare(int count)
{
	int temp = (int)(1000.0f * (1.0f - K * cos_table[count]));
	if (temp > 2000)
	{
		return 2000;
	}
	else if(temp < 0)
	{
		return 0;
	}
	return temp;
}

void pwm_update(float Uk)
{
	static uint8_t first_flag = 0;
	static float target = 0.0f;
	static float temp_target = 0.0f;
	
	temp_target += Uk;
	sprintf(buffer,"Uk : %.4f",Uk);
	canvas.showString(3,1,buffer);
	K += Uk; 
	if(K > 1.0f)
	{
		K = 1.0f;
	}
	else if(K < 0.0f)
	{
		K = 0.0f;
	}
	sprintf(buffer,"K  : %.4f",K);
	canvas.showString(4,1,buffer);
}

extern "C"
{

    void DMA1_Channel1_IRQHandler()
    {
        if(LL_DMA_IsActiveFlag_TC1(DMA1))
        {
            LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_1);
            LL_DMA_ClearFlag_TC1(DMA1);
            oled_device.flushComplete();
        }
    }

    void DMA1_Channel2_IRQHandler()
    {
        LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_2);
        LL_DMA_ClearFlag_TC2(DMA1);
        LL_DMA_SetMemoryAddress(DMA1,LL_DMA_CHANNEL_2,(uint32_t)adc_buffer);
        LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_2,2);
        LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_2);
        LL_ADC_REG_StartConversion(ADC1);
        static uint16_t vol_max = 0;
	    static uint16_t vol_min = 65535;
        uint16_t temp[2] = {adc_buffer[0],adc_buffer[1]};
        static float vol_squ = 0.0f;
        static float cur_squ = 0.0f;
	    static uint16_t count = 0;
		
		static uint64_t _temp[2] = {1,1};

        /* ++count;
        if((count % 5) != 0)
		{
			_temp[0] *= temp[0];
			_temp[1] *= temp[1];
			if(temp[0] > vol_max)
			{
				vol_max = temp[0];
			}
			if(temp[0] < vol_min)
			{
				vol_min = temp[0];
			}
		}
		else
		{
            float vol;
            if(vol_max != 0 && vol_min != 0)
			    vol = powf((float)(_temp[0] * temp[0] / vol_max / vol_min ),0.333333333f) * 0.01289377289377f * 1.00542651793183f - 0.210419735674368f;
            else 
                vol = 0.0f;
			float cur = powf((float)(_temp[1] * temp[1]),0.2f) * 0.0013431013431013f - 0.025f;
            sprintf(buffer,"Vol : %.2f V",vol);
            canvas.showString(1,1,buffer);
            sprintf(buffer,"Cur : %.3f A",cur);
            canvas.showString(2,1,buffer);
            count = 0;
            vol_max = 0;
			vol_min = 65535;
            _temp[0] = 1;
			_temp[1] = 1;
		} */
        
		if((count % 5) != 4)
		{
			_temp[0] *= temp[0];
			_temp[1] *= temp[1];
			if(temp[0] > vol_max)
			{
				vol_max = temp[0];
			}
			if(temp[0] < vol_min)
			{
				vol_min = temp[0];
			}
		}
		else
		{
			//float vol = ((3.3f * ((float)ADC_Buffer[0] / 4095.0f)) - 2.5f) * 41.0f;
			//(NEW)
			//float vol = (0.033837890625f * powf((float)(_temp[0] * temp[0] / vol_max / vol_min ),0.333333333f) - 105.0f);
			float vol = (0.07326007326007f * powf((float)(_temp[0] * temp[0] / vol_max / vol_min ),0.333333333f) - 227.27272727272725f);
			float cur = (0.00335693359375f * powf((float)(_temp[1] * temp[1]),0.2f) - 10.416666666666668f);
            /* float vol = (0.073639637543f * powf((float)(_temp[0] * temp[0] / vol_max / vol_min ),0.333333333f) - 246.4017941522152f); */
			vol_max = 0;
			vol_min = 65535;
			_temp[0] = 1;
			_temp[1] = 1;
			vol_squ += (vol * vol);
			cur_squ += cur * cur;
		}
		if(++count == (1000 * CYCLES))
		{
			float vol_temp;
			static float _vol_temp = 0.0f;
			float cur_temp;
			arm_sqrt_f32(vol_squ / ((float)(1000 * CYCLES) / 5.0f),&vol_temp);
			arm_sqrt_f32(cur_squ / ((float)(1000 * CYCLES) / 5.0f),&cur_temp);
            
            float vol_value = vol_temp * 1.05296796050355f + 0.199481180084053f; //1
            // float vol_value = vol_temp * 1.1257751802404f - 2.3174657907226f; //2
            // float vol_value = vol_temp * 1.0941420821378f - 2.0645721377927f; //3
            float cur_value = cur_temp * 1.02858945376739f - 0.0618879285837834f; //2
            
            /* pwm_update(vol_pid.update(vol_value)); */


            // float vol_value = vol_temp;
            sprintf((char *)buffer,"VOL: %.2f",vol_value);
            canvas.showString(1,1,buffer);
			sprintf((char *)buffer,"CUR: %.3f",cur_value);
            canvas.showString(2,1,buffer);
			count = 0;
			vol_squ = 0.0f;
			cur_squ = 0.0f;
			_vol_temp = vol_temp;
		}
        
    }

    void TIM1_UP_TIM16_IRQHandler()
    {
        
        if(LL_TIM_IsActiveFlag_UPDATE(TIM1))
        {
            LL_TIM_OC_SetCompareCH1(TIM1,calc_compare(phase_count));
            if(++phase_count == CYCLE_COUNT)
            {
                phase_count = 0;
            }
            LL_TIM_ClearFlag_UPDATE(TIM1);
        }
    }
    
    void TIM7_IRQHandler()
    {
        static uint8_t status = 0;
        uint8_t flag = LL_GPIO_IsInputPinSet(GPIOB,LL_GPIO_PIN_9);
        switch(status)
        {
            case 0:
                if(!flag)
                    ++status;
                break;
            case 1:
                if(flag)
                    ++status;
                break;
            case 2:
                if(flag)
                    ++status;
                else
                    status = 0;
                break;
            case 3:
                if(flag)
                {
                    phase_count = 48;
                }
                status = 0;
                break;
        }
        LL_TIM_ClearFlag_UPDATE(TIM7);
    }
    
}




