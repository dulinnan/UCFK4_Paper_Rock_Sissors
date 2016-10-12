/*  @file   display_symbol.c
    @author Jorden Nom
    @date   5 October 2016
    @breif  Displays a Rock, Paper or Sissors
*/

#include "system.h"
#include "pio.h"
#include "pacer.h"

#define DISPLAY_TIME 600

/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

/* bitmap for rock */
static const uint8_t rock[] =
{
    0x07, 0x0F, 0x1F, 0x0F, 0x07
};

/* bitmap for paper */
static const uint8_t paper[] =
{
    0x0, 0x3E, 0x3E, 0x3E, 0x0
};

/* bitmap for sissors */
static const uint8_t sissors[] =
{
    0x42, 0x36, 0x08, 0x36, 0x42
};

/* carries out bit manupulation on display matrix to display given character */
static void display_column (uint8_t row_pattern, uint8_t current_column)
{
    static uint8_t current_row;
    static uint8_t col_prev = 0;

    /* Turn off previous column */
    pio_output_high(cols[col_prev]);

    /* Activate each row according to the bitmap */
    /* Do not turn on next column until al rows are updated with new bitmap */
    for (current_row = 0; current_row < LEDMAT_ROWS_NUM; current_row++){
      if ((row_pattern >> current_row) & 1){
        pio_output_low (rows[current_row]);
      } else {
        pio_output_high(rows[current_row]);
      }
    }
    /* Turn on column with new bitmap now loaded */
    pio_output_low(cols[current_column]);
    col_prev = current_column;
}

/* configures display and calls display_column to display symbol */
void display_rock(void)
{
    static uint8_t current_column = 0;
    static int row;
    static int col;
    static int ticks = 0;

    system_init ();
    pacer_init (500);

    // Initialise LED matrix pins
    for (row = 0; row < 7; row++){
      pio_config_set(rows[row], PIO_OUTPUT_HIGH);
      pio_output_high(rows[row]);
    }
    for (col = 0; col < 5; col++){
      pio_config_set(cols[col], PIO_OUTPUT_HIGH);
      pio_output_high(cols[col]);
    }

    while (ticks < DISPLAY_TIME)
    {
        pacer_wait ();
        ticks++;
        display_column (rock[current_column], current_column);

        current_column++;

        if (current_column > (LEDMAT_COLS_NUM - 1))
        {
            current_column = 0;
        }
    }
}

void display_paper(void)
{
    static uint8_t current_column = 0;
    static int row;
    static int col;
    static int ticks = 0;

    system_init ();
    pacer_init (500);

    // Initialise LED matrix pins
    for (row = 0; row < 7; row++){
      pio_config_set(rows[row], PIO_OUTPUT_HIGH);
      pio_output_high(rows[row]);
    }
    for (col = 0; col < 5; col++){
      pio_config_set(cols[col], PIO_OUTPUT_HIGH);
      pio_output_high(cols[col]);
    }

    while (ticks < DISPLAY_TIME)
    {
        pacer_wait ();
        ticks++;
        display_column (paper[current_column], current_column);

        current_column++;

        if (current_column > (LEDMAT_COLS_NUM - 1))
        {
            current_column = 0;
        }
    }
}

void display_sissors(void)
{
    static uint8_t current_column = 0;
    static int row;
    static int col;
    static int ticks = 0;

    system_init ();
    pacer_init (500);

    // Initialise LED matrix pins
    for (row = 0; row < 7; row++){
      pio_config_set(rows[row], PIO_OUTPUT_HIGH);
      pio_output_high(rows[row]);
    }
    for (col = 0; col < 5; col++){
      pio_config_set(cols[col], PIO_OUTPUT_HIGH);
      pio_output_high(cols[col]);
    }

    while (ticks < DISPLAY_TIME)
    {
        pacer_wait ();
        ticks++;
        display_column (sissors[current_column], current_column);

        current_column++;

        if (current_column > (LEDMAT_COLS_NUM - 1))
        {
            current_column = 0;
        }
    }
}
