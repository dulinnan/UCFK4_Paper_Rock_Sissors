/* Paper, Rock, Sissors

    Publish Date: October 2016
    Developed by Luke McIntyre (lmc211) and Linnan Du (ldu32);

    For comparisons these foloowing characters are used in this program:
    "0" for game initialized/ready hand ;
    "1" for game draw;
    "2" for game lose;
    "3" for game ch_winStatus;

    And also
    "A" for Rock;
    "B" for Sissors;
    "C" for Paper;


    Selecting the object: There will be a question mark on the loading screen, which indicates the players to choose thier object.
    Use the navswitch to select the object:
    Nav left: Rock;
    Nav right: Paper;
    Nav down: Sissor;

    Starting: Once both players have chosen the item, make sure the boards are within IR range,
    and players should push the nav button to compare the chosen items.

    Result: After both players have confirmed their choice, the screen will firstly show the winner who will get a smiley face emoji,
    and the loser will get a unhappy face emoji.
    And then the object the local player chose will be shown on the screen,
    followed by the other player's object.

    End Of Game: Press the R13 button to start the game again!

*/

#include "system.h"             // import system head
#include "tinygl.h"             // import tinygl to draw on the led matrix
#include "pacer.h"              // import timer/pacer
#include "../drivers/led.h"     // import the led drive
#include "navswitch.h"          // import the navswitch drive
#include "ir_uart.h"            // import the IR drive
#include "../fonts/font5x7_1.h" // import the font file
#include "button.h"             // import the button drive from lab 1 (which is not made by us)


#define PACER_RATE 500
#define MESSAGE_RATE 10

char character = '?';           // ? is choosen to be a non-valid character
char otherPlayer = '?';         // which is the default value
char ch_winStatus = '0';        // char to show if the palyer wins or not
int int_navEvent = 0;           // int to get the event happens on the navswitch
int int_timer = 0;              // int getting time
int int_toggle = 0;             // int toogle to show comparisons
bool bl_sentStaus = 0;          // boolean value to check if the msg is sent

void displayCharacter (char ch)
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

/* A is ROCK, B is SISSORS and C is PAPER */
char winner(char localPlayer, char remotePlayer){

  ch_winStatus = '0';                                       // reset the win status

  if(localPlayer != '?' || remotePlayer != '?'){            // reday to go
      if(localPlayer == 'A' && remotePlayer == 'B' ){       // rock beats sissors
          ch_winStatus = '3';
      }

      else if(localPlayer == 'C' && remotePlayer == 'A'){   // paper beats rock
          ch_winStatus = '3';
      }

      else if(localPlayer == 'B' && remotePlayer == 'C' ){  // sissor beats paper
          ch_winStatus = '3';
      }

      else if(localPlayer == remotePlayer){                 // draw game
          ch_winStatus = '1';
      }

      else {
          ch_winStatus = '2';
      }
  }

  return ch_winStatus;
}

/* select and compare both items that players choose */
void selection(void) {

    if(ch_winStatus == '0') {

        if(otherPlayer != 'A' || otherPlayer != 'B' || otherPlayer != 'C'){
            //receive the char from IR which has not been received yet
            if (ir_uart_read_ready_p())
            {
                led_set(LED1, 1);
                otherPlayer = ir_uart_getc();
                led_set(LED1, 0);
                int_navEvent ++;
            }
        }

        // getting choice of paper, rock or sissors from navswitch
        if (navswitch_push_event_p(NAVSWITCH_WEST)){   // left for rock
            character = 'A';
        }

        if (navswitch_push_event_p(NAVSWITCH_SOUTH)){  // down for sissor
            character = 'B';
        }

        if (navswitch_push_event_p(NAVSWITCH_EAST)){   // right for paper
            character = 'C';
        }

        // sending character over IR once the navswitch is triggered
        if (bl_sentStaus != 1 && ((character == 'A' || character == 'B' || character == 'C') && ir_uart_write_ready_p()
            && navswitch_push_event_p(NAVSWITCH_PUSH)))
        {
            led_set(LED1, 1);
            ir_uart_putc(character);
            led_set(LED1, 0);
            int_navEvent ++;
            bl_sentStaus = 1;
        }

        // get the winner infomation when both of the players chose their objects and trasnmitted
        if(int_navEvent == 2 && (character == 'A' || character == 'B' || character == 'C')
        && (otherPlayer == 'A' || otherPlayer == 'B' || otherPlayer == 'C')){
                ch_winStatus = winner(character, otherPlayer);
        }
    }
}

void display(void) {
    /* show the comparison result and the object the local player chose, followed by the romote player's choice' */

    if(int_toggle == 0 && int_timer%PACER_RATE == 0){
        displayCharacter(ch_winStatus);
        int_toggle = 1;
    }

    else if(int_toggle == 1 && int_timer%PACER_RATE == 0){
        displayCharacter(character);
        int_toggle = 2;
    }

    else if(int_toggle == 2 && int_timer%PACER_RATE == 0){
        displayCharacter(otherPlayer);
        int_toggle = 0;
    }
}

int main (void)
{
    //initialize system, tinygl, IR, led matrix, and button. And the pacer rate as well
    system_init ();

    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);

    button_init();
    navswitch_init ();

    ir_uart_init ();

    led_init ();
    pacer_init (PACER_RATE);


    while (1) {

        pacer_wait();
        tinygl_update();
        navswitch_update();
        int_timer ++;

        selection();

        display();

        // restart game if the button R13 is pressed
        if(button_pressed_p()){

            ch_winStatus = '0';  // reset the win char
            character = '?';     // reset the player's choice
            otherPlayer = '?';   // reset the otehr choice
            int_navEvent = 0;    // reset the navswitch event trigger
            bl_sentStaus = 0;    // reset the send status blooean value

        }
    }

return 0;

}
