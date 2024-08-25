#include "tm4c123gh6pm.h"
#include "std_types.h"
#include "UART.h"
#include "UART5.h"

#define Max_Count 650
#define Min_Count 100
#define Max_Angle 180
#define Min_Angle 0

void delay(uint32 d)
{
    while(d--);
}

PWM_INIT_PF1(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20;
    while((SYSCTL_PRGPIO_R & 0x20) == 0); //PORTF Clock Enable

    SYSCTL_RCC_R |= (1<<20); //Use PredividedClock

    SYSCTL_RCC_R &= ~(0x000E0000); // Clear pre-divider field
    SYSCTL_RCC_R |= (0x05 << 17);  // Set the pre-divider to 64

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
    PWM1_2_CMPA_R = 500;

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


unsigned int convertArrayToInt(uint8* array) {
    unsigned int result = 0;
    int i = 0;

    while(array[i] != '\0') {
        result *= 10;
        result += (array[i] - '0');
        i++;
    }

    return result;
}


void Servo_Angle(int Angle) {
    uint16 Counts = (uint16)((float)((Angle - (int)Min_Angle) * ((int)Max_Count - (int)Min_Count) / ((int)Max_Angle - (int)Min_Angle) + (int)Min_Count));
    PWM1_2_CMPA_R = Counts - 1; // Adjust for 0-based index if necessary
}

void UART5FlushReceiver(void) {
    // Check if the receiver FIFO is empty
    while ((UART5_FR_R & UART_FR_RXFE) == 0) {
        // Read and discard data from the FIFO to clear it
        (void)UART5_DR_R;
    }
}

// Function to flush the UART5 transmitter FIFO
void UART5FlushTransmitter(void) {
    // Wait until the transmitter FIFO is empty
    while ((UART5_FR_R & UART_FR_TXFE) == 0)
    {
        delay(1000);
    }

}

int main(void)
{
    HC05_Init();
    PWM_INIT_PF1();
    uint8 str[20];
    while(1)
    {
        UART5FlushTransmitter();
        UART5FlushReceiver();

        uint32 uCounter = 0;

        // Clear the string buffer by setting each element to '\0'
        for (uCounter = 0; uCounter < 20; uCounter++) {
            str[uCounter] = '\0';
        }


        UART5_ReceiveString(str);
        UART5_SendString(str);
        unsigned int Angle = convertArrayToInt(str);
        Servo_Angle(Angle);
    }
}

