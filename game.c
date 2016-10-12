/* Paper, Rock, Sissors

    A is Rock
    B is Sissors
    C is Paper

    Display shows
    - 0 for game not started/finished
    - 1 for draw
    - 2 for lose
    - 3 for win
    then shows you own selection,
    and then shows the other players selection

    Use nav switch to choose paper, rock or sissor
    Press select to restart or reset game
*/



#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "button.h"
#include "../fonts/font5x7_1.h"
#include "../drivers/led.h"


#define PACER_RATE 500
#define MESSAGE_RATE 10

char loop;

void display_character (char ch)
{
	tinygl_clear();
	if (ch == 'A'){
		//draw rock
		tinygl_draw_line (tinygl_point(2,1), tinygl_point(2,5), 1);
		tinygl_draw_line (tinygl_point(0,3), tinygl_point(4,3), 1);
		tinygl_draw_line (tinygl_point(0,1), tinygl_point(4,5), 1);
		tinygl_draw_line (tinygl_point(4,1), tinygl_point(0,5), 1);
	}
	else if (ch == 'B'){
		//draw scissors
		tinygl_draw_box (tinygl_point(0,5), tinygl_point(1,6), 1);
		tinygl_draw_box (tinygl_point(3,5), tinygl_point(4,6), 1);
		tinygl_draw_line (tinygl_point(0,0), tinygl_point(3,5), 1);
		tinygl_draw_line (tinygl_point(4,0), tinygl_point(1,5), 1);
	}
    else if (ch == 'C'){
		//draw paper
		tinygl_draw_box  (tinygl_point(1,0), tinygl_point(4,5), 1);
		tinygl_draw_line (tinygl_point(0,1), tinygl_point(0,6), 1);
		tinygl_draw_line (tinygl_point(1,6), tinygl_point(3,6), 1);
	}
	else if (ch == '0'){
		//draw ready hand
		tinygl_draw_box  (tinygl_point(0,0), tinygl_point(3,1), 1);
		tinygl_draw_line (tinygl_point(3,2), tinygl_point(3,3), 1);
		tinygl_draw_line (tinygl_point(2,6), tinygl_point(4,6), 1);
		tinygl_draw_point (tinygl_point(3,5), 1);
	}
	else if (ch == '2'){
		//draw sad face
		tinygl_draw_point (tinygl_point(1,1), 1);
		tinygl_draw_point (tinygl_point(3,1), 1);
		tinygl_draw_point (tinygl_point(0,5), 1);
		tinygl_draw_point (tinygl_point(4,5), 1);
		tinygl_draw_box (tinygl_point(1,4), tinygl_point(3,5), 1);
	}
	else if (ch == '1'){
		//draw straight face
		tinygl_draw_point (tinygl_point(1,1), 1);
		tinygl_draw_point (tinygl_point(3,1), 1);
		tinygl_draw_box (tinygl_point(1,4), tinygl_point(3,5), 1);
	}
	else if (ch == '3'){
		//draw happy face
		tinygl_draw_point (tinygl_point(1,1), 1);
		tinygl_draw_point (tinygl_point(3,1), 1);
		tinygl_draw_point (tinygl_point(0,4), 1);
		tinygl_draw_point (tinygl_point(4,4), 1);
		tinygl_draw_box (tinygl_point(1,4), tinygl_point(3,5), 1);
	}
	else
	{
		char buffer[2];

        buffer[0] = ch;
        buffer[1] = '\0';

    	tinygl_text (buffer);
	}
}

/* A is ROCK, B is SISSOR and C is PAPER */
char winner(char local_player, char remote_player){

  char win = '0';

  if(local_player != '?' || remote_player != '?'){
      if(local_player == 'A' && remote_player == 'B' ){
          win = '3';
          loop = '0';
      } else if(local_player == 'B' && remote_player == 'C' ){
          win = '3';
          loop = '0';
      } else if(local_player == 'C' && remote_player == 'A'){
          win = '3';
          loop = '0';
      } else if(local_player == remote_player){
          win = '1';
          loop = '0';
      } else {
          win = '2';
          loop = '0';
      }
  }
  return win;
}

int main (void)
{
    char character = '?';   // ? is choosen to be a non-valid character
    char other_player = '?';
    char win = '0';

    int event = 0;
    int tick = 0;
    int toggle = 0;
    bool sent = 0;

    system_init ();
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    navswitch_init ();
    ir_uart_init ();
    led_init ();
    button_init();

    pacer_init (PACER_RATE);

    while (1)
    {
      pacer_wait ();
      tinygl_update ();
      navswitch_update ();
      tick++;

        if(win == '0') {
          loop = '1';

            /* Receive a character over IR if available and havent received it already.  */
            if(other_player != 'A' || other_player != 'B' || other_player != 'C'){
                if (ir_uart_read_ready_p ())
                {
                    led_set(LED1, 1);
                    other_player = ir_uart_getc ();
                    led_set(LED1, 0);
                    event++;
                }
            }
            /* select paper, rock or sissors */
            if (navswitch_push_event_p (NAVSWITCH_WEST)){
                character = 'A';
            }
            if (navswitch_push_event_p (NAVSWITCH_SOUTH)){
                character = 'B';
            }
            if (navswitch_push_event_p (NAVSWITCH_EAST)){
                character = 'C';
            }
            /* Transmit the character over IR on a NAVSWITCH_PUSH event.  */
            if (sent != 1 && ((character == 'A' || character == 'B' || character == 'C') && ir_uart_write_ready_p()
                && navswitch_push_event_p (NAVSWITCH_PUSH)))
            {
                led_set(LED1, 1);
                ir_uart_putc(character);
                led_set(LED1, 0);
                event++;
                sent = 1;
            }

            /* only works out winner when both boards have transmitted there selection
                to each other */
            if(event == 2 && (other_player == 'A' || other_player == 'B' || other_player == 'C')
                && (character == 'A' || character == 'B' || character == 'C')){
                  win = winner(character, other_player);
            }
        }

        /* displays game status, then current selection and then other players selection */
        if(toggle == 0 && tick%500 == 0){
            display_character (win);
            toggle = 1;
        } else if(toggle == 1 && tick%500 == 0){
            display_character(character);
            toggle = 2;
        } else if(toggle == 2 && tick%500 == 0){
            display_character(other_player);
            toggle = 0;
        }

        /* restart game using select button */
        if(button_pressed_p()){
          win = '0';
          character = '?';
          other_player = '?';
          loop = '1';
          sent = 0;
          event = 0;
        }
    }

    return 0;
}
