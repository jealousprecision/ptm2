#ifndef GLOBAL_H_
#define GLOBAL_H

#define F_CPU 8000000UL

#ifndef _BV
#define _BV(bit)				(1<<(bit))
#endif

#ifndef sbi
#define sbi(reg,bit)		reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg,bit)		reg &= ~(_BV(bit))
#endif

#endif  // GLOBAL_H_
