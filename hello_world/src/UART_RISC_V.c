#include <UART_RISC_V.h>

void uart_init(void)
{
	writel(divisor, &UART0->baudRateREG);       // Выставляю скорость
	writel(UART_TXEN_EN, &UART0->txCTRL);       // Разрешаю передачу
	writel(UART_rxEN_EN, &UART0->rxCTRL);       // Разрешаю прием
}

static inline void putchar(int c)
{
	while (readl(&UART0->txDATA) & 0x80000000)
	  	;

	writel(c & 0xff, &UART0->txDATA);            // Пишем данные в UART
}

static int vsnprintf(char * out, size_t n, const char* s, va_list vl)
{
    int format = 0;
    int longarg = 0;
    size_t pos = 0;

    for( ; *s; s++) {
        if (format) {
            switch(*s) {
            case 'l': {
                longarg = 1;
                break;
            }
            case 'p': {
                longarg = 1;
                if (out && pos < n) {
                    out[pos] = '0';
                }
                pos++;
                if (out && pos < n) {
                    out[pos] = 'x';
                }
                pos++;
            }
            case 'x': {
                long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
                int hexdigits = 2*(longarg ? sizeof(long) : sizeof(int))-1;
                for(int i = hexdigits; i >= 0; i--) {
                    int d = (num >> (4*i)) & 0xF;
                    if (out && pos < n) {
                        out[pos] = (d < 10 ? '0'+d : 'a'+d-10);
                    }
                    pos++;
                }
                longarg = 0;
                format = 0;
                break;
            }
            case 'd': {
                long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
                if (num < 0) {
                    num = -num;
                    if (out && pos < n) {
                        out[pos] = '-';
                    }
                    pos++;
                }
                long digits = 1;
                for (long nn = num; nn /= 10; digits++)
                    ;
                for (int i = digits-1; i >= 0; i--) {
                    if (out && pos + i < n) {
                        out[pos + i] = '0' + (num % 10);
                    }
                    num /= 10;
                }
                pos += digits;
                longarg = 0;
                format = 0;
                break;
            }
            case 's': {
                const char* s2 = va_arg(vl, const char*);
                while (*s2) {
                    if (out && pos < n) {
                        out[pos] = *s2;
                    }
                    pos++;
                    s2++;
                }
                longarg = 0;
                format = 0;
                break;
            }
            case 'c': {
                if (out && pos < n) {
                    out[pos] = (char)va_arg(vl,int);
                }
                pos++;
                longarg = 0;
                format = 0;
                break;
            }
            default:
                break;
            }
        }
        else if(*s == '%') {
          format = 1;
        }
        else {
          if (out && pos < n) {
            out[pos] = *s;
          }
          pos++;
        }
    }
    if (out && pos < n) {
        out[pos] = 0;
    }
    else if (out && n) {
        out[n-1] = 0;
    }
    return pos;
}

static int vprintf(const char* s, va_list vl)
{
	char buf[256];
	char *out = buf;

	vsnprintf(buf, 256, s, vl);
	buf[255] = 0x0;
	while (*out) putchar(*out++);

	return 0;
}

int printf(const char* s, ...)
{
	int res = 0;
	va_list vl;

	va_start(vl, s);
	res = vprintf(s, vl);
	va_end(vl);

	return res;
}
