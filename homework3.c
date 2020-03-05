#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework3.h"
#include "myGPIO.h"
#include "myTimer.h"

typedef enum {Stable_P, Tran_PtoR, Stable_R, Tran_RtoP} debounce_state_t;

int main(void)
{
    // Count variables to control the LEDs.
    unsigned int count0 = 0;
    unsigned int count1 = 0;

    // TODO: Declare the variables that main uses to interact with your state machine.
    #define UNPRESSED   0xFF
    #define PRESSED     0x00
    unsigned char buttonhistory = UNPRESSED;


    // Stops the Watchdog timer.
    initBoard();
    // Initialize the GPIO.
    // YOU MUST WRITE THIS FUNCTION IN myGPIO.c
    initGPIO();
    // Initialize Timer0 to provide a one second count interval for updating LED2.
    // YOU MUST WRITE THIS FUNCTION IN myTimer.c
    initTimer(TIMER32_0_BASE, TIMER0_PRESCALER, TIMER0_COUNT);
    // Initialize Timer1 to provide a one millisecond count interval for updating the button history.
    // YOU MUST WRITE THIS FUNCTION IN myTimer.c
    initTimer(TIMER32_1_BASE, TIMER1_PRESCALER, TIMER1_COUNT);

    while(1)
    {
        // Update the color of LED2 using count0 as the index.
        // YOU MUST WRITE THIS FUNCTION BELOW.
        changeLaunchpadLED2(count0);
        // Update the color of the Boosterpack LED using count1 as the index.
        // YOU MUST WRITE THIS FUNCTION BELOW.
        changeBoosterpackLED(count1);

        // TODO: If Timer0 has expired, increment count0.
        // YOU MUST WRITE timer0expired IN myTimer.c
        if (timer0Expired())
        {
            if (count0 != 7)
               count0++;
            else
                count0 = 0;
        }


        // TODO: If Timer1 has expired, update the button history from the pushbutton value.
        // YOU MUST WRITE timer1expired IN myTimer.c
        if (timer1Expired())
        {
            if (checkStatus_BoosterpackS1() == 1)
            {
                buttonhistory = buttonhistory << 1;
                buttonhistory |= 1;
            }
            else if (checkStatus_BoosterpackS1() == 0)
            {
               buttonhistory = buttonhistory << 1;
               buttonhistory &= ~1;
            }


        }




        // TODO: Call the button state machine function to check for a completed, debounced button press.
        // YOU MUST WRITE THIS FUNCTION BELOW.
        fsmBoosterpackButtonS1(buttonhistory);




        // TODO: If a completed, debounced button press has occurred, increment count1.
         if (fsmBoosterpackButtonS1(buttonhistory))
         {
             if (count1 != 7)
                     count1++;
             else
                     count1 = 0;
         }


    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: Map the value of a count variable to a color for LED2.
// Since count is an unsigned integer, you can mask the value in some way.
void changeLaunchpadLED2(unsigned int count)
{
    if (count == 1)
    {
        turnOn_LaunchpadLED2Red();
    }
    else if (count == 2)
    {
        turnOn_LaunchpadLED2Green();
        turnOff_LaunchpadLED2Red();
    }
    else if (count == 3)
    {
        turnOn_LaunchpadLED2Red();
    }
    else if (count == 4)
    {
        turnOn_LaunchpadLED2Blue();
        turnOff_LaunchpadLED2Red();
        turnOff_LaunchpadLED2Green();
    }
    else if (count == 5)
    {
        turnOn_LaunchpadLED2Red();
    }
    else if (count == 6)
    {
        turnOff_LaunchpadLED2Red();
        turnOn_LaunchpadLED2Green();
    }
    else if (count == 7)
    {
        turnOn_LaunchpadLED2Red();
        turnOn_LaunchpadLED2Blue();
    }
    else if (count == 0)
    {
        turnOff_LaunchpadLED2Red();
        turnOff_LaunchpadLED2Blue();
        turnOff_LaunchpadLED2Green();
    }
}

// TODO: Maybe the value of a count variable to a color for the Boosterpack LED
// This is essentially a copy of the previous function, using a different LED
void changeBoosterpackLED(unsigned int count)
{
    if (count == 1)
       {
           turnOn_BoosterpackLEDRed();
       }
       else if (count == 2)
       {
           turnOn_BoosterpackLEDGreen();
           turnOff_BoosterpackLEDRed();
       }
       else if (count == 3)
       {
           turnOn_BoosterpackLEDRed();
       }
       else if (count == 4)
       {
           turnOn_BoosterpackLEDBlue();
           turnOff_BoosterpackLEDRed();
           turnOff_BoosterpackLEDGreen();
       }
       else if (count == 5)
       {
           turnOn_BoosterpackLEDRed();
       }
       else if (count == 6)
       {
           turnOff_BoosterpackLEDRed();
           turnOn_BoosterpackLEDGreen();
       }
       else if (count == 7)
       {
           turnOn_BoosterpackLEDRed();
           turnOn_BoosterpackLEDBlue();
       }
       else if (count == 0)
       {
           turnOff_BoosterpackLEDRed();
           turnOff_BoosterpackLEDBlue();
           turnOff_BoosterpackLEDGreen();
       }
}

// TODO: Create a button state machine.
// The button state machine should return true or false to indicate a completed, debounced button press.
bool fsmBoosterpackButtonS1(unsigned char buttonhistory)
{

    bool pressed = false;

    static debounce_state_t currentState = Stable_R;

    switch (currentState) {
        case Stable_R:
            if (buttonhistory == PRESSED)
            {
                currentState = Tran_RtoP;
            }
            break;

        case Tran_RtoP:
            if (buttonhistory == PRESSED)
            {
                currentState = Stable_P;
                pressed = true;
            }
            break;

        case Stable_P:
            if (buttonhistory != PRESSED)
            {
                currentState = Tran_PtoR;
            }
            break;

        case Tran_PtoR:
            if (buttonhistory != PRESSED)
            {
                currentState = Stable_R;
            }
            break;
    }
            return pressed;
 }
