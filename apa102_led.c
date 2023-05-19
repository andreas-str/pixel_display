/* includes */
#include "apa102_led.h"
#include <stdio.h>
#include "pico/stdlib.h"


/* variables */
uint8_t SpiSendFrame[LED_START_FRAME_SIZE + 4 * LED_FRAME_SIZE + LED_END_FRAME_SIZE];
uint8_t _frameModified; 		// when frame is changed the stimuli is set high
spi_inst_t *_spiHandler;

/* functions */

void DigiLed_init(spi_inst_t *spi)
{
	_frameModified = TRUE; 		// Initial set to true to force update after initialization of frame buffer

	_spiHandler = spi;			// SPI handler is given to library

	// TODO Auto-generated constructor stub

	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		_digitalLedframe[led].FieldsIn.INIT = 0x07; // Set MSB first 3 bits to identify start of LED packet
		_digitalLedframe[led].FieldsIn.GLOBAL = 0x00; // Switch off LED global
		_digitalLedframe[led].FieldsIn.BLUE = 0x00;
		_digitalLedframe[led].FieldsIn.GREEN = 0x00;
		_digitalLedframe[led].FieldsIn.RED = 0x00;
	}
	DigiLed_update(FALSE); // Update frame buffer using the value of _frameModified as set in initialiser.
}



void DigiLed_setColor(uint8_t led, uint8_t red, uint8_t green, uint8_t blue)
{
	if (DigiLed_TestPosition(led) == RANGE_OK)
	{
		_digitalLedframe[led].FieldsIn.INIT = 0x7; // Set MSB first 3 bits to identify start of LED packet
		_digitalLedframe[led].FieldsIn.GLOBAL = 0x1F; // Set led at maximum illumination
		_digitalLedframe[led].FieldsIn.BLUE = blue;
		_digitalLedframe[led].FieldsIn.GREEN = green;
		_digitalLedframe[led].FieldsIn.RED = red;
	}
	_frameModified = TRUE;
}



void DigiLed_setAllColor(uint8_t red, uint8_t green, uint8_t blue)
{
	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		DigiLed_setColor(led, red, green, blue);
	}
}


void DigiLed_setRGB(uint8_t led, uint32_t rgb)
{
	_digitalLedframe[led].FieldsIn.INIT = 0x7;
	_digitalLedframe[led].FieldsIn.GLOBAL = 0x1F;
	_digitalLedframe[led].FieldsIn.BLUE = (uint8_t)(rgb);
	_digitalLedframe[led].FieldsIn.GREEN = (uint8_t)(rgb >> 8);
	_digitalLedframe[led].FieldsIn.RED = (uint8_t)(rgb >> 16);
	_frameModified = TRUE;
}



void DigiLed_setAllRGB(uint32_t rgb)
{
	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		DigiLed_setRGB(led, rgb);
	}
	_frameModified = TRUE;
}



void DigiLed_setLedIllumination(uint8_t led, uint8_t illumination)
{
	if (DigiLed_TestPosition(led) == RANGE_OK)
	{
		_digitalLedframe[led].FieldsIn.GLOBAL = illumination;
	}
	_frameModified = TRUE;
}



void DigiLed_setAllIllumination(uint8_t illumination)
{
	for (int led = 0; led < LED_FRAME_SIZE; led++)
	{
		_digitalLedframe[led].FieldsIn.GLOBAL = illumination;
	}
	_frameModified = TRUE;
}



void DigiLed_setLedOff(uint8_t led)
{
	if (DigiLed_TestPosition(led) == RANGE_OK)
	{
		_digitalLedframe[led].FieldsIn.GLOBAL = 0x00;
	}
	_frameModified = TRUE;
}


void DigiLed_setLedOn(uint8_t led)
{
	if (DigiLed_TestPosition(led) == RANGE_OK)
	{
		_digitalLedframe[led].FieldsIn.GLOBAL = 0x1F;
	}
	_frameModified = TRUE;
}


void DigiLed_update(uint8_t forceUpdate)
{
	if(_frameModified | forceUpdate)
	{
		// add start of frame (0x00000000)
		for(int i = 0; i < LED_START_FRAME_SIZE; i++)
		{
			SpiSendFrame[i] = 0x00;
		}

		// add all LED packets of the frame
		uint32_t SpiDataPacket = 0;
		for (uint32_t led = 0; led < LED_FRAME_SIZE; led++)
		{
			SpiSendFrame[LED_START_FRAME_SIZE + SpiDataPacket + 0] = _digitalLedframe[led].FieldsOut.CMD;		// Add INIT and GLOBAL to SPI send frame
			SpiSendFrame[LED_START_FRAME_SIZE + SpiDataPacket + 1] = _digitalLedframe[led].FieldsOut.BLUE; 	// Add BLUE to SPI send frame
			SpiSendFrame[LED_START_FRAME_SIZE + SpiDataPacket + 2] = _digitalLedframe[led].FieldsOut.GREEN;	// Add GREEN to SPI send frame
			SpiSendFrame[LED_START_FRAME_SIZE + SpiDataPacket + 3] = _digitalLedframe[led].FieldsOut.RED;		// Add RED to SPI send frame

			SpiDataPacket = SpiDataPacket + 4;
		}

		// add end of frame (0xffffffff)
		for(int i = 0; i < 4; i++)
		{
			SpiSendFrame[LED_START_FRAME_SIZE + 4*LED_FRAME_SIZE + i] = 0xFF;
		}

		// send spi frame with all led values
        spi_write_blocking(_spiHandler, SpiSendFrame, sizeof(SpiSendFrame));
	}

	_frameModified = FALSE; // reset frame modified identifier.

}


uint8_t DigiLed_getFrameSize(void)
{
	return LED_FRAME_SIZE;
}


uint8_t DigiLed_TestPosition(uint8_t led)
{
	uint8_t returnValue = OUT_OF_RANGE;
	if (led < LED_FRAME_SIZE)
	{
		returnValue = RANGE_OK;
	}
	return returnValue;
}