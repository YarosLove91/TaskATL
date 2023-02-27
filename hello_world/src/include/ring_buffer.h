#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include <stdint.h>

#define SIZE		128

typedef uint32_t data_t;
typedef unsigned char state_t;

// Переделанный кольцевой буфер из ООП в процедурный
struct RingBuffer {
	data_t data[SIZE];						// Массив для хранения данных. Размер кратен 2^x 
	unsigned r_count;						// указатель на чтение					
	unsigned w_count;						// Указатель на запись

	union {									// Состояние буфера. Сделано в стиле FPGA FIFO
		struct {
			uint8_t full        : 1;          // Полный. Писать некуда
			uint8_t empty       : 1;          // Пустой. Читать неоткуда
			uint8_t overflow    : 1;          // Попытка записи в полный буфер
			uint8_t underflow   : 1;          // Попытка чтения пустого буфера
			uint8_t half_full   : 1;          // Буфер полон на половину. Можно врубить DMA
			uint8_t RESERVED    : 3;
		} status;
		uint8_t reg_status;					  // Что бы можно было вычитать 1 переменной
	} fifo_status;
};

// Инициализация кольцевого буфера
void ring_buffer_init(struct RingBuffer *b);
// Буфер полон ?
int ring_buffer_is_full(const struct RingBuffer *b);
// Количество элементов в буфере
unsigned ring_buffer_count(const struct RingBuffer *b);
// Запись данных в буфер
int ring_buffer_write(struct RingBuffer *b, data_t value);
// Чтение данных из буфера
int ring_buffer_read(struct RingBuffer *b, data_t *val);
// Очистка буфера (указатели сами на себя заводим)
void ring_buffer_clear(struct RingBuffer *b);
// Вычитываем пакет данных
int ring_buffer_read_many(struct RingBuffer *b, data_t *val, unsigned int count);
// Читаем состояние буфера
const uint8_t get_status (const struct RingBuffer *b);

#endif
