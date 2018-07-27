#include "hx711.h"

void set_pin(volatile uint8_t **port_set,uint8_t pin_set,bool state)
{
	if(state==1)
	{
		**port_set|=(1<<pin_set);	
	}
	else
	{
		**port_set&=~(1<<pin_set);
	}	
}

bool read_pin(volatile uint8_t **port_read,uint8_t pin_read)
{
	return (**port_read & ( 1 << pin_read));
}

uint32_t interface_hx711(volatile uint8_t *port_data,uint8_t pin_data,
			volatile uint8_t *port_clk,uint8_t pin_clk,
			bool powerdown, uint8_t modus)
{
	uint32_t count;
	
	if(powerdown == 1)
	{
	
		set_pin(&port_clk,pin_clk,1);
		_delay_us(60);
		return 0;	
	}	
	else
	{
	uint8_t i;
	
	count=0;
	i=0;
	
	set_pin(&port_clk,pin_clk,0);
	
	while (!read_pin(&port_data,pin_data));

	_delay_us(1);
	set_pin(&port_clk,pin_clk,0);
	_delay_us(1);	
	while(read_pin(&port_data,pin_data));
	for(i=0;i<23;i++)
	{
		set_pin(&port_clk,pin_clk,1);
		_delay_us(1);
		count=count<<1;
		set_pin(&port_clk,pin_clk,0);
		_delay_us(1);
		if(read_pin(&port_data,pin_data))
		{
			count++;
		}
	}
	
	do
	{
		set_pin(&port_clk,pin_clk,0);
		_delay_us(1);
		set_pin(&port_clk,pin_clk,1);
		_delay_us(1);
		set_pin(&port_clk,pin_clk,0);
		modus--;
	}
	while(modus!=0);
	
	count = count>>6;
	set_pin(&port_clk,pin_clk,1);
	_delay_us(1);	
	set_pin(&port_clk,pin_clk,0);
	_delay_us(1);
	count ^= 0x800000;
			
	}
	
	return count;
}

uint32_t average_hx711(volatile uint8_t *port_data,uint8_t pin_data,
			volatile uint8_t *port_clk,uint8_t pin_clk,
			uint8_t avgs, uint8_t modus)
{
	uint8_t i;
	uint32_t result;
	result = 0;

	for(i=0;i<avgs;i++)
	{
		result+=interface_hx711(port_data,pin_data,
					port_clk,pin_clk,0,modus);
	
	}
	return result/avgs;
}

void interface_multiple_hx711(volatile uint8_t *port_data,uint8_t pin_data,
                        volatile uint8_t *port_clk[],uint8_t pin_clk[],
                        bool powerdown,uint8_t modus[],
			uint8_t chip_amount, uint32_t result[])
{

	int i;
	/*Power down all HX711's on the bus*/
	for(i=0;i<chip_amount;i++)
	{
		interface_hx711(port_data,pin_data,&(*port_clk[i]),
				pin_clk[i],1,modus[i]);
	}

	if(powerdown==1)
	{
		goto onlypowerdown;
	}
	/*Powerup and readout chips in order */

	for(i=0;i<chip_amount;i++){

		result[i]=interface_hx711(port_data,pin_data,&(*port_clk[i]),
                                	pin_clk[i],0,modus[i]);


		interface_hx711(port_data,pin_data,&(*port_clk[i]),
				pin_clk[i],1,modus[i]);



	}
	onlypowerdown:;
}

void average_multiple_hx711(volatile uint8_t *port_data,uint8_t pin_data,
                        volatile uint8_t *port_clk[],uint8_t pin_clk[],
                        uint8_t avgs[],uint8_t modus[],
                        uint8_t chip_amount, uint32_t result[])
{

        int i;
        /*Power down all HX711's on the bus*/
        for(i=0;i<chip_amount;i++)
        {
                interface_hx711(port_data,pin_data,&(*port_clk[i]),
                                pin_clk[i],1,modus[i]);
        }

        /*Powerup and readout chips in order */

        for(i=0;i<chip_amount;i++)
	{

                result[i]=average_hx711(port_data,pin_data,port_clk[i],
                                        pin_clk[i],avgs[i],modus[i]);


                interface_hx711(port_data,pin_data,&(*port_clk[i]),
                                pin_clk[i],1,modus[i]);



        }
}
