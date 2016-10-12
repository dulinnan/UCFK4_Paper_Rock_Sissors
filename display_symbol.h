/*  @file   display_symbol.h
    @author Jorden Nom
    @date   5 October 2016
    @breif  Displays a Rock, Paper and Sissors
*/

#ifndef DISPLAY_SYMBOL_H
#define DISPLAY_SYMBOL_H

#include "system.h"
#include "pio.h"
#include "pacer.h"


/* carries out bit manupulation on display matrix to display given character */
static void display_column (uint8_t row_pattern, uint8_t current_column);

/* configures display and calls display_column to display symbol */
void display_rock(void);

void display_paper(void);

void display_sissors(void);

#endif /* DISPLAY_SYMBOL_H */
