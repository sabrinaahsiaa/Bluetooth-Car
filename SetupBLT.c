#include "tm4c123gh6pm.h"
#include "UART0.h"
#include "BLT.h"
#include <stdint.h>  // for data type alias
#include <string.h>

#define MAX_STR_LENGTH 34
char CRLF_End[3] = "\r\n";

// main function for programming BT device with no UI
int main(void) {
	uint8_t String[MAX_STR_LENGTH];
	char Input[MAX_STR_LENGTH];
	uint8_t i;
	uint8_t SetCommands[][MAX_STR_LENGTH] = {"AT+NAME=Password\r\n","AT+UART=57600,0,1\r\n","AT+PSWD=0824\r\n","AT+ROLE=0\r\n"};
	uint8_t QueryCommands[][MAX_STR_LENGTH] = {"AT+NAME?\r\n","AT+UART?\r\n","AT+PSWD?\r\n","AT+ROLE?\r\n"};
  UART0_Init();
	BLT_Init();
  
	UART0_OutString(">>> Welcome to the Serial Terminal. <<<");
	UART0_NextLine();	
	UART0_OutString(">>> This is the setup program for HC-05 Bluetooth module. <<<");
	UART0_NextLine();	
	UART0_OutString(">>> You are in the 'AT' Command Mode. <<<");
	UART0_NextLine();	
	UART0_OutString(">>> Type 'AT' and followed with a command. <<<");
	UART0_NextLine();	
	UART0_OutString(">>> Example:AT+NAME=Sabrina <<<");
	UART0_NextLine();	
	UART0_NextLine();	
	
  while (1) {
		UART0_InString(Input, MAX_STR_LENGTH);	//ex: AT+NAME=SABRINA
		strcat(Input, CRLF_End); //"AT+NAME=SABRINA\r\n"
		UART0_NextLine();	// Enter
		BLT_OutString(Input); // Enters the data to the bluetooth
    while ((UART1_FR_R&UART_FR_BUSY) != 0){};
    BLT_InString(String);// "OK" From the bluetooth
    UART0_OutString(String);// OK to the terminal
		UART0_NextLine();		
		UART0_NextLine();
		
		UART0_InString(Input, MAX_STR_LENGTH);// ex: AT+NAME? what is the name of the bluetooth
		strcat(Input, CRLF_End); 
		UART0_NextLine();	
		BLT_OutString(Input);		
    while ((UART1_FR_R&UART_FR_BUSY) != 0){};
    BLT_InString(String);  // get the 1st string before OK
    UART0_OutString(String);
		UART0_NextLine();
		BLT_InString(String);  // get OK
    UART0_OutString(String);
		UART0_NextLine();
		UART0_NextLine();
	}
}
