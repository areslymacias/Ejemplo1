 // FileName:        system_Thread.c
 // Dependencies:    system.h
 // Processor:       MSP432
 // Board:           MSP43P401R
 // Program Version: CCS V8.3 TI
 // Company:         Texas Instruments
 // Description:     Definición de las funciones del thread System_Thread().
 // Authors:         Dayanna Macias, Eric Olivas
 // Updated:         03/2024


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


#include "system.h"

void Timer32_INT1 (void); // Funcion para interrupcion
void *System_Thread(void *arg0);
int var1=0; //variable entera global
int* pvar1= &var1;

/*********************************THREAD*************************************
 * Function: Timer32_INT1
 * Preconditions: None.
 * Input:  None.
 * Output: None.
 *
 *****************************************************************************/
void Timer32_INT1(void){
     T32_ClearInterruptFlag1(); //Baja bandera de interrupcion
     if((*pvar1)==1){
         (*pvar1)=0;
     }
     else{
         (*pvar1)=1;
     }

 }

/*********************************THREAD*************************************
 * Function: System_Thread
 * Preconditions: None.
 * Input:  Apuntador vacío que puede referenciarse a cualquier tipo de dato.
 * Output: None.
 *
 *****************************************************************************/

void *System_Thread(void *arg0)
{
   bool flag = TRUE; //Declaracion de variable para bandera logica

   /* Inicia el sistema (función predeterminada). */
   SystemInit();

   /* Inicializa drivers de objetos. */
   System_InicialiceIO(); //Funcion de configuracion para entradas y salidas
   System_InicialiceUART(); //Funcion para inicializar comunicacion con terminal

   if(flag != TRUE)
   {
       printf("Error al crear archivos. Cierre del programa\r\n");
       exit(1);
   }

   funcion_inicial(); //Funcion para configurar el estado incial de la tarjeta

   T32_Configure1(TIMER32_OneShotMode, TIMER32_PrescaleDiv_1,TIMER32_32BitSize);//Timer 32 bits modo oneshot prescaler 1
   //Habilitacion de la interrupcion
   Int_registerInterrupt(INT_T32_INT1, Timer32_INT1); //Proveniente de TIMER32_MSP432.c
   Int_enableInterrupt(INT_T32_INT1); //Proveniente de INT_MSP432.c
   T32_DisableTimer1();// Deshabilita timer32
   T32_EnableInterrupt1(); // Habilita interrupcion
   T32_SetLoadValue1(3000*(__SYSTEM_CLOCK/1000)); //Carga para el Timer32

   /* Máquina de estados sincronizada. */
   while(TRUE)
   {
       process_events(pvar1); //Funcion para especificaciones de la apliacion del LAB2
       usleep(1000);
   }
}
