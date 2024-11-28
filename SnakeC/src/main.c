#include <stm32f031x6.h>
#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "sound.h"
#include "musical_notes.h"
#include "serial.h"
#define SIZE 100

void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
void greenOn();
void yellowOn();
void redOn();
void greenOff();
void yellowOff();
void redOff();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void menuTheme(void);
void explosionTune(void);
void EatingApples(void);
uint32_t prbs(void);
uint32_t random_number(uint32_t lower,uint32_t upper);

volatile uint32_t milliseconds;

const uint16_t snake_head[]=
{
0,0,0,0,8448,40224,40224,40224,42240,8448,0,0,0,0,0,0,16640,40224,40224,40224,0,16640,0,0,0,0,0,0,40224,40224,40224,40224,8192,0,0,0,0,0,0,0,64800,40224,40224,40224,32033,1040,0,0,50245,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,42317,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,49160,57344,9293,9293,9293,9293,9293,16393,40960,9293,9293,9293,57344,0,9293,9293,9293,9293,9293,40960,0,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,
};

const uint16_t apple[]= 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9293,9293,25677,0,0,0,0,0,0,0,0,1604,9293,42061,9293,0,0,0,0,0,0,37186,22355,21843,50245,9293,9293,0,0,0,0,0,0,37186,21843,0,0,50245,33869,0,0,0,0,31520,40224,31776,31776,63512,0,0,0,0,0,0,23328,40224,40224,40224,40224,40224,63512,0,0,0,0,0,63512,40224,40224,40224,40224,40224,40224,0,0,0,0,0,23328,40224,40224,40224,40224,40224,23328,0,0,0,0,0,63512,40224,40224,40224,40224,40224,46616,0,0,0,0,0,23328,63512,40224,40224,40224,40224,46616,0,0,0,0,0,0,23328,23328,23328,40224,46616,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t dynamite[]=
{
0,0,24327,65315,24327,24327,0,0,0,0,0,0,0,0,40224,65315,24327,24327,0,0,0,0,0,0,0,0,65315,65315,22355,0,22355,0,0,0,0,0,0,0,0,0,24327,0,22355,0,0,0,0,0,0,0,0,0,22355,22355,0,0,0,0,0,0,0,0,0,40224,40224,40224,40224,40224,40224,0,0,0,0,0,40224,30918,40224,40224,40224,40224,61307,40224,0,0,0,0,65535,40224,30918,40224,40224,30918,40224,65535,0,0,0,0,40224,65535,40224,61307,61307,40224,65535,40224,0,0,0,0,40224,40224,65535,40224,40224,65535,40224,40224,0,0,0,0,65535,61307,40224,40224,40224,40224,30918,65535,0,0,0,0,40224,65535,30918,61307,61307,61307,65535,40224,0,0,0,0,40224,40224,65535,40224,40224,65535,40224,40224,0,0,0,0,0,40224,40224,40224,40224,40224,40224,0,0,0,0,0,0,0,40224,40224,40224,40224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t plus_one[]=
{
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 135, 135, 135, 0, 0, 0, 0, 0, 0, 0, 0, 135, 135, 135, 135, 0, 0, 0, 0, 0, 0, 0, 135, 135, 135, 135, 135, 0, 0, 0, 0, 0, 0, 135, 135, 135, 135, 135, 135, 0, 0, 0, 0, 0, 0, 135, 135, 0, 135, 135, 135, 0, 0, 0, 0, 0, 0, 0, 0, 0, 135, 135, 135, 0, 0, 0, 0, 0, 0, 135, 0, 0, 135, 135, 135, 0, 0, 0, 0, 0, 135, 135, 135, 0, 135, 135, 135, 0, 0, 0, 0, 0, 0, 135, 0, 0, 135, 135, 135, 0, 0, 0, 0, 0, 0, 0, 0, 0, 135, 135, 135, 0, 0, 0, 0, 0, 0, 0, 0, 0, 135, 135, 135, 0, 0, 0, 0, 0, 0, 135, 135, 135, 135, 135, 135, 135, 135, 0, 0, 0, 0, 135, 135, 135, 135, 135, 135, 135, 135, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


int main()
{
	uint16_t x = 60;
	uint16_t y = 70;
	uint16_t apple_x = random_number(0,110);
	uint16_t apple_y = random_number(0,140);
	uint16_t bomb_x = random_number(0,110);
	uint16_t bomb_y = random_number(0,140);
	uint16_t oldx = x;
	uint16_t oldy = y;
	uint16_t bomb_count = 1; //Counts the amount of bombs
	uint16_t apple_count = 0; //Counts the amount of apples collected
	uint16_t new_bomb_count = 0; //Counter used for spawning new bombs
	uint16_t checks = 0;
	int appleCount = 0;
	initClock();
	initSysTick();
	initSerial();
	setupIO();
	initSound();
	drawRectangle(0,0,127,159,RGBToWord(247,248,250)); //Border
	int moved = 0;
	int bombs[SIZE][2]= {bomb_x,bomb_y}; //2D array used for storing all the bomb locations
	int i = 0;
	int j = 0;
	int location_i = 1;
	int location_j = 0;
	while(1) //First while loop
	{
		//This while loop is used for the main menu screen
		while(moved == 0) //Second while loop
		{	
			//Message to begin the game
			//menuTheme();
			
			printTextX2("Press up", 20,16,RGBToWord(255,255,255), RGBToWord(0,0,0));
			printTextX2("button to", 12,50,RGBToWord(255,255,255), RGBToWord(0,0,0));
			printTextX2("begin", 35,80,RGBToWord(255,255,255), RGBToWord(0,0,0));
			delay(75);

			//If the up button is pressed the game removes the text and puts the apple,bomb and snake on screen and starts the game.
			if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
			{		
				fillRectangle(1,1,120,140,RGBToWord(0,0,0));
				putImage(apple_x,apple_y,12,16,apple,0,0);
				putImage(bomb_x,bomb_y,12,16, dynamite,0,0);
				putImage(x,y,12,16,snake_head,0,0);
				//Makes it so it doesn't start moving straight away
				delay(500);
				//Sets it to move upwards
				moved = 4;
			}
		}//End second while loop

		//This while loop is the main game and contains all the code for the main game
		while(moved < 5 && moved > 0)//Third while loop
		{	
			if(new_bomb_count % 3 == 1)//An LED turns on for each apple eaten
			{
				redOff();
				yellowOff();
				greenOn();
			}
			if(new_bomb_count % 3 == 2)
			{
				yellowOn();
			}
			if(new_bomb_count % 3 == 0 && appleCount > 0)
			{
				redOn();
			}
			
			//Calls prbs function to shuffle the random numbers
			prbs();

			//If the right button is pressed makes the snake move right continuously until it hits the border
			if(moved == 1)//Right pressed
			{
				if(x < 115)
				{
					x = x + 1;
					putImage(x,y,12,16,snake_head,0,0);
					delay(50);
					fillRectangle(oldx,oldy,1,16,0);
					oldx = x;
					oldy = y;		
				}//End inner if
			}//End if

			//If the left button is pressed makes the snake move left continuously until it hits the border
			if(moved == 2)//Left pressed
			{
				if(x>1)
				{
					x = x - 1;
					putImage(x,y,12,16,snake_head,0,0);
					delay(50); //Makes the snake move at a certain speed
					fillRectangle(oldx+11,oldy,1,16,0);
					oldx = x;
					oldy = y;	
				}//End inner if
			}//End if

			//If the down button is pressed makes the snake move down continuously until it hits the border
			if(moved == 3)//down pressed
			{
				if(y<143)
				{
					y = y + 1;
					putImage(x,y,12,16,snake_head,0,0);
					delay(50);
					fillRectangle(oldx,oldy,12,1,0);
					oldx = x;
					oldy = y;			
				}//End inner if
			}//End if

			//If the up button is pressed makes the snake move up continuously until it hits the border
			if(moved == 4)//up pressed
			{
				//If the up button is pressed makes the snake move up continuously until it hits the border
				if(y>1)
				{
					y = y - 1;
					putImage(x,y,12,16,snake_head,0,0);
					delay(50);
					fillRectangle(oldx,oldy+15,12,1,0);
					oldx = x;
					oldy = y;		

				}//End inner if
			}//End if
			if(y < 2 || x < 2 || x > 114 || y >142)//Plays a warning tune if any wall is touched
			{
				playNote(1000);
				delay(200);
				playNote(0);
				new_bomb_count = new_bomb_count + 1; //If the snake stays touching the wall for too long it spawns more bombs
			}
			//Sets moved to 1 if right button is pressed so it can run its corresponding if statement above
			if ((GPIOB->IDR & (1 << 4))==0) // right pressed
			{					
				moved = 1;	
			}
			
			//Sets moved to 2 if right button is pressed so it can run its corresponding if statement above
			if ((GPIOB->IDR & (1 << 5))==0) // left pressed
			{			
				moved = 2;			
			}
			
			//Sets moved to 3 if right button is pressed so it can run its corresponding if statement above
			if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
			{
				moved = 3;
			}

			//Sets moved to 4 if right button is pressed so it can run its corresponding if statement above
			if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
			{			
				moved = 4;
			}

			
			//Checks if the snake has eaten an apple by checking if the snake is inside the apple and removes it and spawns a new one
			if (isInside(apple_x,apple_y,12,16,x+3,y+3) || isInside(apple_x,apple_y,12,16,x+9,y+3) || isInside(apple_x,apple_y,12,16,x+3,y+10) || isInside(apple_x,apple_y,12,16,x+9,y+10) )
			{		
				fillRectangle(apple_x,apple_y,12,16,0); //Removes the old apple
				putImage(x,y,12,16,plus_one,0,0);//Shows plus one every time an apple is eaten
				appleCount++;
				//Calculates new apple location
				apple_x = random_number(0,110);
				apple_y = random_number(0,140);

				apple_count = apple_count + 1; //Adds one to the apple count
				new_bomb_count++; //Adds one to the counter until a new bomb

				//Checks if the new apple locations is inside the snake or bombs location before spawning it and if so gives it a new location
				checks = 0;
				while (checks == 0) //Check apple while loop
				{
					//Used to get the positions of the bombs from the 2d array
					i = 0;
					j = 0;

					//Checks if it is inside any of the bombs locations
					while (i < SIZE)
					{	
						// Checks if it is inside of any of the bombs locations
						if (isInside(bombs[i][j],bombs[i][j+1],12,16,apple_x,apple_y) || isInside(bombs[i][j],bombs[i][j+1],12,16,apple_x + 12,apple_y) || isInside(bombs[i][j],bombs[i][j+1],12,16,apple_x,apple_y + 16) || isInside(bombs[i][j],bombs[i][j+1],12,16,apple_x + 12, apple_y + 16) )
						{	
							//If it is Changes the apple's location
							apple_x = random_number(0,110);
							apple_y = random_number(0,140);

							//Sets it to 0 so it can run the while loop again to check if the new apple location is inside any bombs
							i = 0;
						}
						else //If apple's location isn't inside it goes to the next row in the 2d array to get the next bomb location
						{
							i = i + 1;	
						}//End if else

					}//End inner while

					//Checks if the apple is inside the snake's location
					if (isInside(x,y,12,16,apple_x,apple_y) || isInside(x,y,12,16,apple_x + 12,apple_y) || isInside(x,y,12,16,apple_x,apple_y + 16) || isInside(x,y,12,16,apple_x + 12, apple_y + 16) )
					{	
						//If it is inside changes the apple's location
						apple_x = random_number(0,110);
						apple_y = random_number(0,140);
						//Will go back to the start of the check apple while loop to make sure the new apple location is good
					}
					else
					{
						//If it isn't inside the snake's location sets it means it is at a new location  and sets checks to 1 and exits the check apple while loop
						checks = 1;
					}//End if else

				}//End check apple while loop

				//Puts image of apple on screen in new location
				putImage(apple_x,apple_y,12,16,apple,0,0);
				delay(100);
				EatingApples();
			}//End if
			//Used to check if a the player has collected 3 apples and puts a new bomb on screen
			if(new_bomb_count == 3)
			{	
				//Gets new bomb location
				bomb_x = random_number(0,110);
				bomb_y = random_number(0,140);

				bomb_count++;//Adds one to the bomb count

				//Checks if  the new bomb location is inside the snake,apple or any of the bomb locations before putting it on screen and if so gives it a new location and
				checks = 0;
				while(checks == 0) //Check bomb while loop
				{
					//Used to get the positions of the bombs from the 2d array
					i = 0;
					j = 0;


					//Checks if it is inside any of the bombs locations
					while (i < SIZE)
					{	
						//Checks if the new bomb location is the same as an old bomb location
						if (isInside(bombs[i][j],bombs[i][j+1],12,16,bomb_x,bomb_y) || isInside(bombs[i][j],bombs[i][j+1],12,16,bomb_x + 12,bomb_y) || isInside(bombs[i][j],bombs[i][j+1],12,16,bomb_x,bomb_y + 16) || isInside(bombs[i][j],bombs[i][j+1],12,16,bomb_x + 12, bomb_y + 16) )
						{	
							//If it is changes the new bomb's location
							bomb_x = random_number(0,110);
							bomb_y = random_number(0,140);

							//Sets it to 0 so it can run the while loop again to check if the new apple location is inside any bombs
							i = 0;
						}
						else //If the bomb's location isn't the same as that bomb location goes to the next row in the 2d array to get the next bomb location
						{
							i = i + 1;

						}//End if else

					}//End while
					//Checks if the bomb is the same as the apple's location or the snake's location
					if (isInside(apple_x,apple_y,12,16,bomb_x,bomb_y) || isInside(apple_x,apple_y,12,16,bomb_x + 12,bomb_y) || isInside(apple_x,apple_y,12,16,bomb_x,bomb_y + 16) || isInside(x,y,12,16,bomb_x + 12, bomb_y + 16) || isInside(x,y,12,16,bomb_x,bomb_y) || isInside(x,y,12,16,bomb_x + 12,bomb_y) || isInside(x,y,12,16,bomb_x,bomb_y + 16) || isInside(x,y,12,16,bomb_x + 12,bomb_y + 16) )
					{	
						//If it is changes the bomb's location
						bomb_x = random_number(0,110);
						bomb_y = random_number(0,140);
						//Will go back to the start of the check bomb while loop to make sure 
					}
					else
					{	
						//If it isn't the same as the apple's location or the snake's location it means it is at a new location and sets check to 1 and exits the check bomb while loop
						checks = 1;

					}//End if else

				}//End check bomb while loop

				//Puts new bomb on screen
				putImage(bomb_x,bomb_y,12,16, dynamite,0,0);
				
				//Adds new bombs x and y coordinates to the 2d array
				bombs[location_i][location_j] = bomb_x;
				bombs[location_i][location_j+1] = bomb_y;

				//Resets the new bomb count
				new_bomb_count = 0;

				//Moves it to the next row in the 2d array so the next bomb location is put in the next spot in the 2d array.
				location_i = location_i + 1;

			}//End if

			//Checks if the snake is inside a bomb so the game ends
			//For statements cycles through all the bombs locations in the 2d arrays
			for( i = 0; i < SIZE; i++)
			{
				for(j=0; j < 1; j++)
				{	
					//Stops checking when the x and y locations of a bomb is 0,0 as it means it has hit the end of the 2d array
					if (bombs[i][j] !=0 && bombs [i][j+1] != 0)
					{	
						//Checks if the snake is inside any of the bombs locations and if so runs the code
						if (isInside(bombs[i][j],bombs[i][j+1],12,16,x+3,y+3) || isInside(bombs[i][j],bombs[i][j+1],12,16,x+9,y+3) || isInside(bombs[i][j],bombs[i][j+1],12,16,x+3,y+10) || isInside(bombs[i][j],bombs[i][j+1],12,16,x+9,y+10) )
						{	
							//Removes the apple of screen

							fillRectangle(apple_x, apple_y,12,16,0);
							appleCount = 0;//Resets Count for LED
							greenOff();
							yellowOff();
							redOff();
							explosionTune(); //Resets LEDs
							//Removes all the bombs of screen
							for(location_i = 0; location_i < SIZE; location_i++)
							{	
								for(location_j = 0; location_j < 1; location_j++)
								{	
									//Stops it from checking once x and y is 0 as it means it has hit the end of the 2d array.
									if (bombs[location_i][location_j] !=0 && bombs [location_i][location_j+1] != 0)
									{
										//Fills each bomb location with black
										fillRectangle(bombs[location_i][location_j],bombs[location_i][location_j + 1],12,16,0);
									} //End if

								} // End inner for

							} // End for

							//Removes the snake of screen
							fillRectangle(x,y,12,16,0);

							//Outputs the game over screen
							printTextX2("Game Over",12,16,RGBToWord(255,255,255), RGBToWord(0,0,0));

							//Shows apple count
							printText("Apples:",12,50,RGBToWord(255,255,255), RGBToWord(0,0,0));
							printNumber(apple_count, 80, 50, RGBToWord(255,255,255), RGBToWord(0,0,0));

							//Shows bomb count
							printText("Bombs:",12,70,RGBToWord(255,255,255), RGBToWord(0,0,0));
							printNumber(bomb_count, 80, 70, RGBToWord(255,255,255), RGBToWord(0,0,0));

							printTextX2("Press up",20,90,RGBToWord(255,255,255), RGBToWord(0,0,0));
							printTextX2("to restart", 5, 110, RGBToWord(255,255,255), RGBToWord(0,0,0));
					
							//Puts game into standby mode in case of restart
							moved = 5;
							while(moved == 5)
							{	
								//If up button is clicked game restarts and sets all variables back to original values
								if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
								{	
									x = 60;
									y = 70;
									apple_x = random_number(0,110);
									apple_y = random_number(0,140);
									bomb_x = random_number(0,110);
									bomb_y = random_number(0,140);
									oldx = x;
									oldy = y;
									bomb_count = 1;
									apple_count = 0;
									new_bomb_count = 0;
									checks = 0;
									for(location_i=0; location_i < SIZE; location_i ++)
									{
										for(location_j=0; location_j < 2; location_j++)
										{
											bombs[location_i][location_j]= 0;
										}
									}
									bombs[0][0] = bomb_x;
									bombs[0][1] = bomb_y;
									i = 0;
									j = 0;
									location_i = 1;
									location_j = 0;
									fillRectangle(1,1,125,140,RGBToWord(0,0,0)); // Clears screen
									moved = 0; //Sends it back to the second while loop
								}//End inner if
								
							}//End while

						}//End second if

					}//End first if	

				}//End second for

			}//End first for

		}//End third while loop
	}//End first while loop
	
	return 0;

}//End main



uint32_t random_number(uint32_t lower,uint32_t upper)
{
    return (prbs()%(upper-lower))+lower;
}
uint32_t shift_register=1234;
uint32_t prbs()
{
	// This is an unverified 31 bit PRBS generator
	// It should be maximum length but this has not been verified 
	unsigned long new_bit=0;
	static int busy=0; // need to prevent re-entrancy here
	if (!busy)
	{
		busy=1;
		new_bit= ((shift_register & (1<<27))>>27) ^ ((shift_register & (1<<30))>>30);
		new_bit= ~new_bit;
		new_bit = new_bit & 1;
		shift_register=shift_register << 1;
		shift_register=shift_register | (new_bit);
		busy=0;
	}
	return shift_register & 0x7fffffff; // return 31 LSB's 
}

void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
	milliseconds++;

}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	pinMode(GPIOB,3,1);
	pinMode(GPIOA,12,1);
	pinMode(GPIOB,0,1);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}
void menuTheme(void)
{
	playNote(E4);
  delay(500);
  playNote(E4);
  delay(500);
  playNote(E4);
  delay(500);
  playNote(E4);
  delay(500);
  playNote(D4);
  delay(500);
  playNote(C4);
  delay(500);
  playNote(D4);
  delay(500);
  playNote(E4);
  delay(500);

  playNote(G4);
  delay(500);
  playNote(G4);
  delay(500);
  playNote(G4);
  delay(500);
  playNote(G4);
  delay(500);
  playNote(F4);
  delay(500);
  playNote(E4);
  delay(500);
  playNote(F4);
  delay(500);
  playNote(G4);
  delay(500);

  playNote(E4);
  delay(500);
  playNote(E4);
  delay(500);
  playNote(E4);
  delay(500);
  playNote(E4);
  delay(500);
  playNote(D4);
  delay(500);
  playNote(C4);
  delay(500);
  playNote(D4);
  delay(500);
  playNote(E4);
  delay(500);

  playNote(G4);
  delay(500);
  playNote(G4);
  delay(500);
  playNote(G4);
  delay(500);
  playNote(G4);
  delay(500);
  playNote(F4);
  delay(500);
  playNote(E4);
  delay(500);
  playNote(F4);
  delay(500);
  playNote(G4);
  delay(500);

  playNote(E4);
  delay(500);
  playNote(E4);
  delay(500);
  playNote(E4);
  delay(500);
  playNote(E4);
  delay(500);
  playNote(D4);
  delay(500);
  playNote(C4);
  delay(500);
  playNote(D4);
  delay(500);
  playNote(E4);
  delay(500);

  playNote(G4);
  delay(500);
  playNote(G4);
  delay(500);
  playNote(G4);
  delay(500);
  playNote(G4);
  delay(500);
  playNote(F4);
  delay(500);
  playNote(E4);
  delay(500);
  playNote(F4);
  delay(500);
  playNote(G4);
  delay(500);

  playNote(C5);
  delay(500);
  playNote(C5);
  delay(500);
  playNote(B4);
  delay(500);
  playNote(A4);
  delay(500);
  playNote(B4);
  delay(500);
  playNote(C5);
  delay(500);
  playNote(B4);
  delay(500);
  playNote(A4);
  delay(500);

  playNote(G4);
  delay(500);
  playNote(A4);
  delay(500);
  playNote(B4);
  delay(500);
  playNote(C5);
  delay(500);
  playNote(B4);
}
void EatingApples()
{
	playNote(C5);
	delay(50);
	playNote(E5);
	delay(50);
	playNote(G5);
	delay(50);
	playNote(B5);
	delay(50);
	playNote(0);
}
void greenOn()
{
	GPIOB->ODR  |= (1<<3);
}
void redOn()
{
	GPIOB->ODR  |= (1<<0);
}
void yellowOn()
{
	GPIOA->ODR  |= (1<<12);
}
void greenOff()
{
	GPIOB->ODR  &= ~(1<<3);
}
void yellowOff()
{
	GPIOA->ODR  &= ~(1<<12);
}
void redOff()
{
	GPIOB->ODR  &= ~(1<<0);
}
void explosionTune()
{
	playNote(B3);
	delay(50);
	playNote(G3);
	delay(50);
	playNote(E3);
	delay(50);
	playNote(A3);
	delay(50);
	playNote(B3);
	delay(50);
	playNote(G3);
	delay(50);
	playNote(E3);
	delay(50);
	playNote(A3);
	delay(50);
	playNote(0);
}