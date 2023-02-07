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


// =============================================================================
// PROGRAM SET-UP

/**
 * @brief Run code to initialize program and do important preparations.
 */
void app_init(void) // TODO:
{
    // Start clocks for port D and port E.
    *(ulong*)0x40023830 = 0x18;
    // Set the address on the left to the value on the right;
    *(ulong*)GPIOD = 0x00005555;
}


// =============================================================================
// FUNCTIONS

void activate_row(int);     // Activates a specific row.
int read_column();          // Reads each column and returns the index of the
                            // key being pressed, if that.
uchar key_value(int, int);  // Return the value of the key being pressed.


/**
 * @brief Read signals from the keyboard and determine which key is pressed,
 *        if one is pressed. If no key is pressed, return 0xFF.
 */
uchar keyb(void)
{
    for (int row = 1; row <= 4; row++)
    {
        activate_row(row);
        int column = read_column();
        if (column)
            return key_value(row, column);
    }

    return 0xFF;
}


/**
 * @brief TODO
 * @param row The current row to index. Is 1-indexed.
 */
void activate_row(int row)
{

}


/**
 * @brief TODO
 */
int read_column()
{

}


/**
 * @brief TODO
 */
void out_7_seg(uchar keycode)
{

}


/**
 * @brief TODO
 * @param row The row of the key being pressed.
 * @param col The column of the key being pressed.
 */
uchar key_value(int row, int col)
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

    }
}

