#ifndef _OLED_H
#define _OLED_H

void OLEDInit(void);
void OLEDClear(void);
void OLEDPutChar(unsigned char page,
					unsigned char col,
					unsigned char c,
					unsigned char display_type);
void OLEDPrint(unsigned char page,
				unsigned char col,
				unsigned char *str,
				unsigned char alignment,
				unsigned char display_type);

#endif