/*
 * myTimer.c
 *
 *  Created on: 3/03/2020
 *      Author: Carlos Gil
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "myTimer.h"

// TODO: Write a function that can intialize a Timer with particular values for pre-scale and count.
// Aliases for the Timers and the preScaler arguments are defined in myTimer.h
void initTimer(unsigned int timer, unsigned int preScaler, unsigned int count)
{

    // For the specified timer and pre-scale value, put the timer in 32-bit periodic mode.
    if (timer == TIMER32_0_BASE)
    Timer32_initModule (TIMER0, TIMER0_PRESCALER, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    else
    Timer32_initModule (TIMER1, TIMER1_PRESCALER, TIMER32_32BIT, TIMER32_PERIODIC_MODE);


    // For the specified timer, pass the count value.
    if (timer == TIMER32_0_BASE)
    Timer32_setCount (TIMER0, count);
    else
    Timer32_setCount (TIMER1, count);
    // For the specified timer, configure the timer to repeat once it elapses.
    if (timer == TIMER32_0_BASE)
    Timer32_startTimer(TIMER0, REPEAT);
    else
    Timer32_startTimer(TIMER1, REPEAT);


}

// TODO: Write a function that indicates whether Timer0 has expired.
// You have been given a such a function in the lecture slides.
bool timer0Expired(void)
{
    static unsigned int previousSnap = MAX_VALUE;
    unsigned int currentSnap;
    bool returnValue;

    currentSnap = Timer32_getValue(TIMER0);
    returnValue = (currentSnap > previousSnap);
    previousSnap = currentSnap;
    return returnValue;

}

// TODO: Write a function that indicates whether Timer1 has expired.
// This is essentially a copy of the previous function, using a different timer.
// Since we only have two timers, later in the course we will create software timers that use a common time base.
bool timer1Expired(void)
{
    static unsigned int previousSnap = MAX_VALUE;
    unsigned int currentSnap;
    bool returnValue;

    currentSnap = Timer32_getValue(TIMER1);
    returnValue = (currentSnap > previousSnap);
    previousSnap = currentSnap;
    return returnValue;
}
