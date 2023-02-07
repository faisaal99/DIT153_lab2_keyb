/*
 * 	startup.c
 *
 */
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

// =============================================================================
// INCLUDE & PRE-PROCESSOR

#include "typedef.h" // Import some typedefs that ease coding.
#include "gpio.h"    // Addresses for different ports and their respective
                     // registers.
#define IS_DEBUG 1   // Used for conditional compilation during development.
                     // Production code should always set this to 0.

// =============================================================================
// PROGRAM SET-UP

/**
 * @brief Run code to initialize program and do important preparations.
 */
void app_init(void)
{
    // Start clocks for port D and port E.
    *(volatile ulong*)0x40023830 = 0x18;
    // Set port D to medium speed.
    *(volatile uint*)0x40020C08 = 0x55555555;

    // Rig port D to work with a keypad.
    *(volatile ushort*) GPIOD_MODER_HIGH = 0x5500;
    *(volatile ushort*) GPIOD_MODER_LOW  = 0x5555;
    *(volatile ushort*) GPIOD_OTYPER    &= 0x00FF;
    *(volatile uint*)   GPIOD_PUPDR     &= 0x0000FFFF;
    *(volatile uint*)   GPIOD_PUPDR     |= 0x00AA0000;
}


// =============================================================================
// DATA

/**
 * @brief A matrix with the key-values for the keypad.
 */
const uchar KEYCODE[][4] =
{ {  1, 2,  3, 10 }
, {  4, 5,  6, 11 }
, {  7, 8,  9, 12 }
, { 14, 0, 15, 13 }
};


// =============================================================================
// FUNCTIONS

void activate_row(uint);      // Activates a specific row.
int read_column();            // Reads each column and returns the index of the
                              // key being pressed, if that.
uchar key_value(uint, uint);  // Return the value of the key being pressed.


/**
 * @brief Read signals from the keyboard and determine which key is pressed,
 *        if one is pressed. If no key is pressed, return 0xFF.
 */
uchar keyb(void)
{
    for (uint row = 1; row <= 4; row++)
    {
        activate_row(row);
        uint column = read_column();
        if (column)
            return key_value(row, column);
    }

    return 0xFF;
}


/**
 * @brief Activates the specific row on the keyboard.
 *
 * @param row The current row to index. Is 1-indexed. 1 <= row <= 4.
 */
void activate_row(uint row)
{
    volatile uchar *gpoid_odr_high = (volatile uchar*)(GPIOD_ODR + 1);

    switch (row)
    {
    case 1:
        *gpoid_odr_high = 0x10; break;

    case 2:
        *gpoid_odr_high = 0x20; break;

    case 3:
        *gpoid_odr_high = 0x40; break;

    case 4:
        *gpoid_odr_high = 0x80; break;

    default: break;
        // NAUGHTY MOVE
    }
}


/**
 * @brief Read each column from the keypad and return the first column that is
 *        enabled.
 */
int read_column()
{
    uchar c = *(volatile uchar*)(GPIOD_IDR + 1);

    if ( c & 0b1000 ) return 1;
    if ( c & 0b0100 ) return 2;
    if ( c & 0b0010 ) return 3;
    if ( c & 0b0001 ) return 4;

    return 0;
}


/**
 * @brief Output the value from the keypad onto the 7-segment-display.
 */
void out_7_seg(uchar keycode)
{
    volatile uchar *gpiod_odr_low = (volatile uchar*)GPIOD_ODR;

    if ( 1 <= keycode && keycode <= 15 )
        *gpiod_odr_low = keycode;
    else
        *gpiod_odr_low = 0;
}


/**
 * @brief Get the keyvalue corresponding to the row and column.
 * 
 * @param row The row of the key being pressed. 1-indexed.
 * @param col The column of the key being pressed. 1-indexed.
 */
uchar key_value(uint row, uint col)
{
    row--;
    col--;

    return KEYCODE[row][col];
}


// =============================================================================
// MAIN

void main(void)
{
    app_init();

    while (true)
    {
        out_7_seg( keyb() );
#ifdef IS_DEBUG
        while (true) {}
#endif
    }
}
