#include "adc.h"

static uint8_t adc_initialized = 0;

/**
 * @brief Initialize ADC
 * 
 * This function initializes the Analog Digital Converter. The reference voltage is set to AVCC.
 * And the highest possible frequency is used
 */
void init_ADC(void)
{
	adc_initialized = 1;
	
	//AVCC als Referenzspannung
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);

	//Höchst mögliche Frequenz
	ADCSRA |= (1 << ADPS0);
	ADCSRA |= (1 << ADPS1);
	ADCSRA |= (1 << ADPS2);

	//Enable ADC
	ADCSRA |= (1 << ADEN);
}

/**
 * @brief Gets the value at the given ADC Pin
 * 
 * @param channel ADC pin to read
 * @return uint16_t Value at pin
 */
uint16_t read_ADC(uint8_t channel)
{
    ASSERT_LED(ERR_INIT, adc_initialized);

	uint16_t result = 0;

	//reset
	ADMUX = (ADMUX & 0xE0);
	ADCSRB &= ~(1 << MUX5);


	//select chanel	
	if(channel > 7) {
		ADCSRB |= (1 << MUX5);
		ADMUX |= (channel - 8);
	} else {
		ADMUX |= channel;
	}

	//Single Conversion starten
	ADCSRA |= (1 << ADSC);

	//Blockieren bis Konvertierung fertig
	uint16_t counter = 0;
	while (ADCSRA & (1 << ADSC) && (counter < UINT16_MAX)) {
		++counter;
	};

	ASSERT_LED(ERR_LOOP_EXIT, (counter != UINT16_MAX));

	//Auslesen
	result = ADC;

	return result;
}

