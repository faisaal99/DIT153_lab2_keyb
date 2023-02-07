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
#define IS_DEBUG 1   // Used for conditional compilation, during development.
                     // Production code should always set this to 0.

// =============================================================================
// PROGRAM SET-UP

/**
 * @brief Run code to initialize program and do important preparations.
 */
void app_init(void) // TODO
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
 * @param row The current row to index. Is 1-indexed.
 */
void activate_row(uint row)
{
    volatile uchar *gpoid_otype_high = (volatile uchar*)(GPIOD_OTYPER + 1);

    switch (row)
    {
    case 1:
        *gpoid_otype_high = 0x10;
        break;
    case 2:
        *gpoid_otype_high = 0x20;
        break;
    case 3:
        *gpoid_otype_high = 0x40;
        break;
    case 4:
        *gpoid_otype_high = 0x80;
        break;
    default:
        // LMAO
    }
}


/**
 * @brief TODO
 */
int read_column()
{
    for (uint col = 1; col <= 4; col++)
        if ( 0/* Some way of determining if shit is active */ )
            return col;

    return 0;
}


/**
 * @brief TODO
 */
void out_7_seg(uchar keycode)
{
    // TODO
}


/**
 * @brief TODO
 * @param row The row of the key being pressed.
 * @param col The column of the key being pressed.
 */
uchar key_value(uint row, uint col)
{
    // TODO
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

