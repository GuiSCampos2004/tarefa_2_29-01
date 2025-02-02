#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

const uint LED1 = 11;
const uint LED2 = 12;
const uint LED3 = 13;

uint identificador_atual = LED2;  //Variaveis que identificam qual led deve
uint identificador_passado = LED1;//estar ligado e qual deve ser desligado
//----------------------------------------------------------------------------------------------
bool repeating_timer_callback(struct repeating_timer *t) { //Rotina de repetição

    gpio_put(identificador_passado, 0);//Desliga o led que estava ligado até então


    switch(identificador_passado){//Identifica o proximo led segundo a ordem
        case LED1:
            identificador_atual = LED2;
            break;

        case LED2:
            identificador_atual = LED3;
            break;

        case LED3:
            identificador_atual = LED1;
            break;
    }

    
    gpio_put(identificador_atual, 1);//Liga o devido led
    identificador_passado = identificador_atual;//Identifica o proximo led a ser desligado

    return true;
}
//----------------------------------------------------------------------------------------------
int main()
{
    //----------------------------------------------------------------------------------------------------------
    //Inicialização de portas e variaveis
    stdio_init_all();

    gpio_init(LED1);
    gpio_set_dir(LED1, GPIO_OUT);
    gpio_put(LED1, 1);
    gpio_init(LED2);
    gpio_set_dir(LED2, GPIO_OUT);
    gpio_put(LED2, 0);
    gpio_init(LED3);
    gpio_set_dir(LED3, GPIO_OUT);
    gpio_put(LED3, 0);

    struct repeating_timer timer;
    absolute_time_t proximo_tempo = get_absolute_time(); //Variavel para controlar a "impressão" an tela
    int mensagem = 3; //Variavel para imprimir mensagens na ordem correta
    //----------------------------------------------------------------------------------------------------------

    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);//Inicio da rotina de repetição

    while (true) {
        if (time_reached(proximo_tempo)) {
            switch(mensagem){
                case 3:
                    printf("Trocando em %is...\n", mensagem);
                    mensagem = 2;
                    break;

                case 2:
                    printf("Trocando em %is...\n", mensagem);
                    mensagem = 1;
                    break;

                case 1:
                    printf("Trocando em %is...\n", mensagem);
                    mensagem = 3;
                    break;

            }
            proximo_tempo = delayed_by_us(proximo_tempo, 1000000);//Define que a proxima "impressão" será feita daqui 1s
        }
        sleep_ms(1); //Intervenção ara que o while(1) não consuma muito
    }
}
