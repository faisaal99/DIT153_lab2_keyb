#ifndef _KEYB_H_
#define _KEYB_H_


/* GPIOD */

#define GPIOD            0x40020C00
#define GPIOD_MODER_LOW  (GPIOD)
#define GPIOD_MODER_HIGH (GPIOD +  2)
#define GPIOD_OTYPER     (GPIOD +  4)
#define GPIOD_OSPEEDR    (GPIOD +  8)
#define GPIOD_PUPDR      (GPIOD + 12)
#define GPIOD_IDR        (GPIOD + 16)
#define GPIOD_ODR        (GPIOD + 20)
#define GPIOD_BSRR       (GPIOD + 24)


/* GPIOE */

#define GPIOE            0x40021000
#define GPIOE_MODER_LOW  (GPIOE)
#define GPIOE_MODER_HIGH (GPIOE +  2)
#define GPIOE_OTYPER     (GPIOE +  4)
#define GPIOE_OSPEEDR    (GPIOE +  8)
#define GPIOE_PUPDR      (GPIOE + 12)
#define GPIOE_IDR        (GPIOE + 16)
#define GPIOE_ODR        (GPIOE + 20)
#define GPIOE_BSRR       (GPIOE + 24)


#endif // _KEYB_H_