# Using UART with DMA

* Generate a square wave at frequency 10x the baud rate.
* Baud rate generator has pretty good granularity of settable
  frequency.
* Need baud rate of 120000 to generate 60kHz square wave this way.
* Set UART BRG with divider=666 dividerFract=11 (11/16 = 0.6875)
  * ESP32-C3-devkitM-1 has 160MHz CPU clock with `APB_CLK`=80MHz.
  * This is the maximum input frequency to the UART module.
  * Yields baud rate of 80.0e6/(666 + 11/16) = 119996.250117 or ~4Hz error.

	1. START bit is 0
	2. 1
	3. 0
	4. 1
	5. 0
	6. 1
	7. 0
	8. 1
	9. 0
	10. STOP bit is 1

* This requires 12000 bytes/second (counting the START and STOP bits,
  10 bits are transmitted per byte) which is easy when using
  interrupts to feed DMA.
* The bits are sent in MSB-first order, so we have to send bytes of 0x55.
