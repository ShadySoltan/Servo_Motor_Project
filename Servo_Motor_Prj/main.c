#include "tm4c123gh6pm.h"
#include "std_types.h"

#define Max_Count 650
#define Min_Count 100
#define Max_Angle 180
#define Min_Angle 0

void delay(uint16 d)
{
    while(d--);
}

PWM_INIT_PF1(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20;
    while((SYSCTL_PRGPIO_R & 0x20) == 0); //PORTF Clock Enable

    SYSCTL_RCC_R |= (1<<20); //Use PredividedClock

    SYSCTL_RCGCPWM_R |= (1<<1); //Module1 Clock Enable

    delay(500);

    //Configure PF1 as PWM Pin
    GPIO_PORTF_AFSEL_R |= (1<<1);

    //Choose the PWM mode
    GPIO_PORTF_PCTL_R = 0x00000050;

    //Output direction
    GPIO_PORTF_DIR_R |= (1<<1);

    //Enable Pin
    GPIO_PORTF_DEN_R |= (1<<1);



    //Configure PWM Module1 Generators 2
    //Disable the PWM channels 5
    PWM1_2_CTL_R &= ~(1<<0);
    //Select UpCount mode
    PWM1_2_CTL_R |= (1<<1);
    //Configure the load value to be 1600 meaning that the frequency will be 10KHz
    PWM1_2_LOAD_R = 4999;
    //Set the DutyCycle
    PWM1_2_CMPA_R = 250;

    //When the PWM timer reach top it drives the PF1 to high
    PWM1_2_GENB_R &= ~(1<<3) | ~(1<<2);
    PWM1_2_GENB_R |= (1<<0) | (1<<1);

    //When the counter reaches the compartorA it will drive the pins to low
    PWM1_2_GENB_R &= ~(1<<4);
    PWM1_2_GENB_R |= (1<<5);

    //Enable the generator
    PWM1_2_CTL_R |= (1<<0);

    //Enable channel 5 PWM module 1
    PWM1_ENABLE_R |= (1<<5);
}


void Servo_Angle(uint8 Angle)
{
    uint16 Counts = (uint16)(((Angle-Min_Angle)*((float)((Max_Count-Min_Count)/(Max_Angle-Min_Angle))))+Min_Count)-1;
    PWM1_2_CMPA_R = Counts;
}

int main(void)
{
    PWM_INIT_PF1();
    while(1)
    {
        Servo_Angle(90);
    }
}

