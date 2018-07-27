/* HX711.h - Library to interface the HX711 weight scale ADC*/

#ifndef HX711_H
#define HX711_H
	
	#include <inttypes.h>
	#include <stdbool.h>
	#include <util/delay.h>
	#include <avr/io.h>
	
	/* Macros for 'modus' parameter in 'interface_hx711'.
	Sets the channel and gain configuration*/

	#define CHANNEL_A_128	1
	#define CHANNEL_A_64	3
	#define CHANNEL_B_32	2


	/* Main function for interfacing the HX711 ADC.
	
	The port parameters accept addresses to the avr/io.h definitons 
	,while the pin parameters are needing the definitions themselves.
	
	Example: interface_hx711(&PINB,PINB7,&PORTB,PORTB6,...)

	Note: If you change the configuration, it will be applied with the
	next readout.

	Note: Don't forget to set the data direction registers*/

	uint32_t interface_hx711(volatile uint8_t *port_data,uint8_t pin_data,
				volatile uint8_t *port_clk,uint8_t pin_clk,
				bool powerdown, uint8_t modus);
	
	/* Main function to get an average of 'avgs' sampels from the HX711 ADC.

	It executes multiple calls of interface_hx711 and calculates
	the average. This functions supports at least 256 averages,with more
	avrages you can run into danger overflowing the 32bit storing variable.

	Note: Most of the HX711 modules are hardwired to give 10 sampels per
	seconds, so don't be suprised about long runtimes.*/	

	uint32_t average_hx711(volatile uint8_t *port_data,uint8_t pin_data,
         	               volatile uint8_t *port_clk,uint8_t pin_clk,
				uint8_t avgs, uint8_t modus);

	/*Function to interface multiple HX711's with an combined DOUT_pin.

	This function has the same functionality as the function
	'interface_hx711' with the addition you can interface
	multiple chips with one function call.

	The function first applies a powerdown to all chips and then performs
	a readout in the same order as the pins given in the arrays 'port_clk[]'
	and 'pin_clk[]'. The results are saved in the array "result[]", which
	has to be given as a function parameter and has to be defined
	and initialised beforehand.

	The variable 'chip_amount' defines the number of chips used.

	Other parameters are the same as in 'interface_hx711' with the
	addition they have to be given as an array.
	
	Note: The array with the 'port_clk' has to be defined like
	this: volatile uint8_t *port={&PORTB,&....,};
	*/
	void interface_multiple_hx711(volatile uint8_t *port_data,
			uint8_t pin_data,volatile uint8_t *port_clk[],
			uint8_t pin_clk[],bool powerdown,uint8_t *modus,
			uint8_t chip_amount,uint32_t result[]);

	/*Function to get an average of 'avgs' samples from the HX711 ADC
	with combined DOUT_pin.

	Note: Because the chips are read out in sequence, don't be
	surpised about a long runtime.
	*/
	void average_multiple_hx711(volatile uint8_t *port_data,uint8_t pin_data,
                        volatile uint8_t *port_clk[],uint8_t pin_clk[],
                        uint8_t avgs[],uint8_t modus[],
                        uint8_t chip_amount, uint32_t result[]);

#endif
