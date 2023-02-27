#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdint.h>


// Примитивный ГПСЧ с периодо 2^32
static int LFSR(uint8_t reset){
	static uint32_t S = 1;     // Стартовое значение
	if (reset)
		S = 1;         // Если выставлена 1, тогда начинаем счет за ново
	S = ((( (S>>31)^(S>>30)^(S>>29)^(S>>27)^(S>>25)^S ) & 1 ) << 31 ) | (S>>1);
	return S ;
}


#endif
