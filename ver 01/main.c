/* DIEU KHIEN THIET BI QUA MANG INTERNET
 * SU DUNG MODULE TIVA C VA ESP8266
 */

#include "main.h"
#include "ESP8266.h"

void ProcessCommand(char *CommandText);
void ReadButton(void);
void ClearData(void);
void ResponseXML(void);
void SetGPIO(void);

#define BUTTON1     GPIO_PIN_4
#define GPIO1   GPIO_PIN_1
#define GPIO2   GPIO_PIN_2

int GPIO1_STATE = 0;
int GPIO2_STATE = 0;

int main(void)
{
    //Khoi tao clock he thong
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    //Khoi tao he thong
    InitUART0();    //Khoi tao UART0
    InitUART1();    //Khoi tao UART1
    InitLED();      //Khoi tao LED
    InitTimer0();   //Khoi tao ngat TIMER0
    InitESP8266();  //Khoi tao ESP8266 V1

    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //Gui "CONNECT SUCCESS" de thong bao ket noi thanh cong
    SendString("CONNECT SUCCESS");

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
    ClearData();
    char c;
    while (true)
    {
        //SendString("start...\r\n");
        while (true)
        {
            if (UARTCharsAvail(UART1_BASE))
            {
                c = UARTCharGet(UART1_BASE);
                if (c == '\0') continue;
                UARTCharPut(UART0_BASE, c);
                ReceivedData[DATASIZE] = c;
                if (DATASIZE < 512)
                    DATASIZE++;
                else DATASIZE = 0;
            }
            if (FindString(ReceivedData, "HTTP") != -1)
            {
                //SendHttp(website);
                //SendString("continue...\r\n");
                break;
            }
            //ReadButton();
        }
        //UARTCharPut(UART0_BASE, ReceivedData[21]);
        ProcessCommand(ReceivedData);
        ReadButton();
        ResponseXML();
        SetGPIO();
        ClearData();
        SysCtlDelay(SysCtlClockGet() / 10);
    }
}

void ReadButton(void)
{
    int value = GPIOPinRead(GPIO_PORTF_BASE, BUTTON1);
    while (GPIOPinRead(GPIO_PORTF_BASE, BUTTON1) == 0);
    if (value == 0)
        GPIO1_STATE = 1 - GPIO1_STATE;
}

void SetGPIO(void)
{
    if (GPIO1_STATE)
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO1, GPIO1);
    else GPIOPinWrite(GPIO_PORTF_BASE, GPIO1, 0);
    //
    if (GPIO2_STATE)
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO2, GPIO2);
    else GPIOPinWrite(GPIO_PORTF_BASE, GPIO2, 0);
}

/* CHUONG TRINH XU LY DU LIEU TU WEB */
void ProcessCommand(char *CommandText)
{

    if (FindString(CommandText, "GET / HTTP") != -1)
    {
        SendHttp(website);
        //SendString("here...\r\n");
    }
    else if (FindString(CommandText, "GPIO1") != -1)
    {
        GPIO1_STATE = 1 - GPIO1_STATE;
    }
    else if (FindString(CommandText, "GPIO2") != -1)
    {
        GPIO2_STATE = 1 - GPIO2_STATE;
    }
}

void ResponseXML(void)
{
    SendATCommand("AT+CIPSEND=0,76");
    SysCtlDelay(SysCtlClockGet() / 10);
    Uart1Write("<?xml version='1.0' encoding='utf-8'?>");   //38 ky tu
    Uart1Write("<inputs>");     //8
    if (GPIO1_STATE)
        Uart1Write("<GPIO>GPIO1_ON</GPIO>");    //21
    else
        Uart1Write("<GPIO>GPIO1_OF</GPIO>");
    Uart1Write("</inputs>");    //9
    memset(ReceivedData, 0, sizeof(ReceivedData));
    ReceiveResponse("SEND OK", ReceivedData, 1000, true);
    SendString("\r\n");             /* Kiem tra */
}

void ClearData(void)
{
    DATASIZE = 0;
    memset(ReceivedData, 0, sizeof(ReceivedData));
}
