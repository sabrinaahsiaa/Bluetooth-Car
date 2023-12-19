#include "tm4c123gh6pm.h"
#include "PWM.h"
#include "GPIO.h"

// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define SP   0x20

int RIGHT_SPEED = START_SPEED * 1.0;
int LEFT_SPEED = START_SPEED * 1.25;

void UART_Car_Dir_Init(void);
unsigned char UART1_InChar(void);
void BLT_InString(unsigned char *bufPt);
void DisableInterrupts(void);
void EnableInterrupts(void);
void WaitForInterrupt(void);
void Delay(void);

// MAIN: This main is meant for the command configuration of the hc05.
int main(void){
	unsigned char control_symbol; // for bluetooth controlled LEDs
	UART_Car_Dir_Init(); // Initialize UART1	
	LED_Init();
  PWM_PB76_Init();
  PWM_PB76_Duty(LEFT_SPEED, RIGHT_SPEED);
	
  while(1){
		control_symbol = UART1_InChar();
	
    switch (control_symbol){
      case 'w':
        LED = Green;
				PWM0_ENABLE_R |= 0x00000003; // enable both wheels
				WHEEL_DIR = FORWARD;
        break; 
      case 's':
				LED = Blue;
				PWM0_ENABLE_R |= 0x00000003; // enable both wheels
				WHEEL_DIR = BACKWARD;
        break; 
      case'a': 
        LED = Yellow;
				PWM0_ENABLE_R |= 0x00000002; // Enable right wheel
				PWM0_ENABLE_R &= ~0x00000001; // Disable left wheel
        break; 
      case'd': 
        LED = Purple;
				PWM0_ENABLE_R |= 0x00000001; // Enable left wheel
				PWM0_ENABLE_R &= ~0x00000002; // Disable right wheel
        break; 
      case SP:
        LED = Dark;
				PWM0_ENABLE_R &= ~0x00000003; // stop both wheels
        break; 
      case 'e':
				LEFT_SPEED = LEFT_SPEED * 1.1;
				RIGHT_SPEED = RIGHT_SPEED * 1.1;
        PWM_PB76_Duty(LEFT_SPEED, RIGHT_SPEED);
        break; 
      case 'q':
				LEFT_SPEED = LEFT_SPEED * 0.9;
				RIGHT_SPEED = RIGHT_SPEED * 0.9;
				PWM_PB76_Duty(LEFT_SPEED, RIGHT_SPEED);
        break; 
      default:
        break;
    }
	}
}

//------------UART_Init------------
// Initialize the UART for 19200 baud rate (assuming 16 MHz UART clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Car_Dir_Init(void){
	// Activate Clocks
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART1; // activate UART1
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
	
  UART1_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
	
	// Data Communication Mode, Buad Rate = 57600
  UART1_IBRD_R = 17;                    // IBRD = int(16,000,000 / (16 * 57600)) = int(17.3611111)
  UART1_FBRD_R = 23;                     // FBRD = round(3611111 * 64) = 27
	
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART1_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART1_CTL_R |= 0x301;                 // enable UART for both Rx and Tx
  
  GPIO_PORTB_AFSEL_R |= 0x03;           // enable alt funct on PB1,PB0
  GPIO_PORTB_DEN_R |= 0x03;             // enable digital I/O on PB1,PB0
                                        // configure PB1,PB0 as UART1
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTB_AMSEL_R &= ~0x03;          // disable analog functionality on PB1,PB0
	
	GPIO_PORTB_AMSEL_R &= ~0x3C;	// disable analog function
	GPIO_PORTB_AFSEL_R &= ~0x3C;	// no alternate function
  GPIO_PORTB_PCTL_R &= ~0x00FFFF00;	// GPIO clear bit PCTL 
	GPIO_PORTB_DIR_R |= 0x3C; // output on pin(s)
  GPIO_PORTB_DEN_R |= 0x3C;	// enable digital I/O on pin(s)
}

//------------UART1_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char UART1_InChar(void){
  while((UART1_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART1_DR_R&0xFF));
}

// This function reads response from HC-05 Bluetooth module.
void BLT_InString(unsigned char *bufPt) {
  unsigned char length=0;
  bufPt[length] = UART1_InChar();
  
  // Two possible endings for a reply from HC-05: OK\r\n, ERROR:(0)\r\n
  while (bufPt[length]!=LF) {
    length++;
    bufPt[length] = UART1_InChar();
  };
    
  // add null terminator
  length++;
  bufPt[length] = 0;
}

// Subroutine to wait 0.25 sec
// Inputs: None
// Outputs: None
// Notes: ...
void Delay(void){
	unsigned long volatile time;
  time = 727240*500/91;  // 0.25sec
  while(time){
		time--;
  }
}
