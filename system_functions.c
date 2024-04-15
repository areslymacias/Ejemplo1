 // FileName:        system_functions.c
 // Dependencies:    system.h
 // Processor:       MSP432
 // Board:           MSP43P401R
 // Program Version: CCS V8.3 TI
 // Company:         Texas Instruments
 // Authors:         Dayanna Macias, Eric Olivas
 // Updated:         03/2024

#include "system.h"

/* EXPLICACION DEL PROGRAMA.
 * El programa a realizar debe hacer los siguientes pasos:
 * 1. Esperar a que un usuario inicie el programa presionando una tecla del teclado
 * por terminal y dicha acción se debe reflejar en la terminal Imprimiendo
 * ”INICIANDO PROGRAMA” y encendiendo el Led AZUL.
 * 2. Se debe cambiar el LED a AMARILLO por tres segundos cada que se presiona un switch.
 * El microcontrolador no debe hacer alguna acción mientras el LED esta en AMARILLO
 * y se debe imprimir en terminal la lectura “PAUSA”.
 * 3. Cuando termine los tres segundos, se regresa a AZUL e imprime “PROGRAMA EJECUTANDOSE”.
 * 4. El microcontrolador cuenta un ciclo de 6 iteraciones que realice las acciones de los pasos 3 y 4.
 * 5. Cuando llegue a la última iteración o pulse el switch de apagado 2veces dentro del intervalo
 * (véase punto 7)  imprime el mensaje “PROGRAMA TERMINADO” y apaga el LED.
 * 6. Se regresa al paso1.
 * 7. Existe un segundo switch que al presionarlo imprime “PRESIONE DE NUEVO PARA APAGAR”,
 * (no hace pausa, ni iteración) el LED cambia a ROJO, si lo presiona dentro de 3 segundos salta
 * al paso 5 y apaga, sino continua ejecutándose normalmente, vuelve a paso 3.
 */


/*FUNCTION*********************************************************************************
*
* Function Name    : System_InicialiceIO
* Returned Value   : None.
* Comments         : Configuracion para entradas y salidas
*
*END****************************************************************************************/

void System_InicialiceIO(void)
{
    GPIO_init_board();
}

/*FUNCTION******************************************************************************
*
* Function Name    : System_InicialiceUART
* Returned Value   : None.
* Comments         : Funcion para inicializar comunicacion con terminal
*
*END***********************************************************************************/
void System_InicialiceUART (void)
{
    UART_init();
}

/*FUNCTION******************************************************************************
*
* Function Name    : funcion_inicial
* Returned Value   : None.
* Comments         : Esta funcion nos permite configurar el estado incial de la tarjeta
*
*END***********************************************************************************/

void funcion_inicial (void)
{
    GPIO_setOutput(BSP_LED1_PORT, BSP_LED1,   0); //LED 1 apagado
    GPIO_setOutput(BSP_LED2_PORT,  BSP_LED2,  0); //LED 2 rojo apagado
    GPIO_setOutput(BSP_LED3_PORT,  BSP_LED3,  0); //LED 2 verde apagado
    GPIO_setOutput(BSP_LED4_PORT,  BSP_LED4,  0); //LED 2 azul apagado
    UART_putsf(MAIN_UART, "LABORATORIO 2\r\n"); //Mensaje a terminal
}

/*FUNCTION******************************************************************************
*
* Function Name    : process_events
* Returned Value   : None.
* Comments         : Funcion para especificaciones de la apliacion del LAB2
*
*END***********************************************************************************/



void process_events(int* pvar1)
{
    static int var2=0,var3=0,var4=0;
    static bool flag1=TRUE,flag2=TRUE;
    if(flag2==TRUE){
        UART_putsf(MAIN_UART, "PRESIONE UNA TECLA PARA COMENZAR\r\n"); //Mensaje a terminal
        flag2=FALSE;
    }
    if((EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)){
        var4= 1;
        EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG; // Baja bandera TXIFG
    }
    if(var4==1){
        if(flag1==TRUE){
            GPIO_setOutput(BSP_LED2_PORT,  BSP_LED2,  0); //LED 2 rojo apagado
            GPIO_setOutput(BSP_LED3_PORT,  BSP_LED3,  0); //LED 2 verde apagado
            GPIO_setOutput(BSP_LED4_PORT,  BSP_LED4,  1); //LED 2 azul encendido
            UART_putsf(MAIN_UART, "INICIANDO PROGRAMA\r\n"); //Mensaje a terminal
            flag1=FALSE; //Negacion de bandera 1
        }
        if(GPIO_getInputPinValue(BSP_BUTTON1_PORT, BSP_BUTTON1) != BOARD_BUTTON_NORMAL_STATE) // Entra cuando se presiona el boton 1.
        {
            T32_DisableTimer1(); //Deshabilitacion del Timer32
            GPIO_setOutput(BSP_LED2_PORT,  BSP_LED2,  0); //LED 2 rojo apagado
            GPIO_setOutput(BSP_LED3_PORT,  BSP_LED3,  1); //LED 2 verde encendido
            GPIO_setOutput(BSP_LED4_PORT,  BSP_LED4,  0); //LED 2 azul apagado
            UART_putsf(MAIN_UART, "PAUSA\r\n"); //Mensaje a terminal
            var2++;
            var3=0;
            sleep(3); //Duerme 3 iteraciones
            if(var2==6){ //Entra cuando pasen 6 iteraciones
                GPIO_setOutput(BSP_LED2_PORT,  BSP_LED2,  0); //LED 2 rojo apagado
                GPIO_setOutput(BSP_LED3_PORT,  BSP_LED3,  0); //LED 2 verde apagado
                GPIO_setOutput(BSP_LED4_PORT,  BSP_LED4,  0); //LED 2 azul apagado
                UART_putsf(MAIN_UART, "PROGRAMA TERMINADO\r\n"); //Mensaje a terminal
                flag1=TRUE; // Negacion bandera 1
                flag2=TRUE; // Negacion bandera 2
                var2=0;
                var4=0;
            }
            else{
                GPIO_setOutput(BSP_LED2_PORT,  BSP_LED2,  0); //LED 2 rojo apagado
                GPIO_setOutput(BSP_LED3_PORT,  BSP_LED3,  0); //LED 2 verde apagado
                GPIO_setOutput(BSP_LED4_PORT,  BSP_LED4,  1); //LED 2 azul encendido
                UART_putsf(MAIN_UART, "PROGRAMA EJECUTANDOSE\r\n"); //Mensaje a terminal
            }
            while(GPIO_getInputPinValue(BSP_BUTTON1_PORT, BSP_BUTTON1) != BOARD_BUTTON_NORMAL_STATE);// Funcion antirrebote
        }
        else if(GPIO_getInputPinValue(BSP_BUTTON2_PORT, BSP_BUTTON2) != BOARD_BUTTON_NORMAL_STATE) // Entra cuando se presiona el boton 2.
        {
            T32_EnableTimer1(); // Habilita Timer32
            T32_EnableInterrupt1(); // Habilita interrupcion
            T32_SetLoadValue1(3000*(__SYSTEM_CLOCK/1000)); //Valor para carga del timer
            var3++;
            if(var3==1){
                GPIO_setOutput(BSP_LED2_PORT,  BSP_LED2,  1); //LED 2 rojo encendido
                GPIO_setOutput(BSP_LED3_PORT,  BSP_LED3,  0); //LED 2 verde apagado
                GPIO_setOutput(BSP_LED4_PORT,  BSP_LED4,  0); //LED 2 azul apagado
                UART_putsf(MAIN_UART, "PRESIONE DE NUEVO PARA APAGAR\r\n"); //Mensaje a terminal
            }
            if(var3==2 && (*pvar1)==0){
                T32_DisableTimer1(); // Deshabilitacion del Timer32
                GPIO_setOutput(BSP_LED2_PORT,  BSP_LED2,  0); //LED 2 rojo apagado
                GPIO_setOutput(BSP_LED3_PORT,  BSP_LED3,  0); //LED 2 verde apagado
                GPIO_setOutput(BSP_LED4_PORT,  BSP_LED4,  0); //LED 2 azul apagado
                UART_putsf(MAIN_UART, "PROGRAMA TERMINADO\r\n"); //Mensaje a terminal
                flag1=TRUE; // Negacion bandera 1
                flag2=TRUE; // Negacion bandera 2
                *pvar1=0;
                var2=0;
                var3=0;
                var4=0;
            }
            while(GPIO_getInputPinValue(BSP_BUTTON2_PORT, BSP_BUTTON2) != BOARD_BUTTON_NORMAL_STATE); // Funcion antirrebote
            }
        if((*pvar1)==1){
                    T32_DisableTimer1();
                    GPIO_setOutput(BSP_LED2_PORT,  BSP_LED2,  0); //LED 2 rojo apagado
                    GPIO_setOutput(BSP_LED3_PORT,  BSP_LED3,  0); //LED 2 verde apagado
                    GPIO_setOutput(BSP_LED4_PORT,  BSP_LED4,  1); //LED 2 azul encendido
                    UART_putsf(MAIN_UART, "PROGRAMA EJECUTANDOSE\r\n"); //Mensaje a terminal
                    (*pvar1)=0;
                    var3=0;
                }
    }
}
