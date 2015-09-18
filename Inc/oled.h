#ifndef __OLED_H
#define __OLED_H

void OLED_Initial(void);

void OLED_Display_Picture(const unsigned char * buffer);

extern unsigned char const VAADIN_PICTURE[];
#endif
