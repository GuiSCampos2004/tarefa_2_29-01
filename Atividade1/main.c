#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

const uint LED1 = 11;
const uint LED2 = 12;
const uint LED3 = 13;

uint identificador_atual = LED2;
uint identificador_passado = LED1;

bool repeating_timer_callback(struct repeating_timer *t) {

    gpio_put(identificador_passado, 0);


    switch(identificador_passado){
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

    
    gpio_put(identificador_atual, 1);
    identificador_passado = identificador_atual;

    return true;
}


int main()
{
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
    int mensagem = 3;

    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    absolute_time_t proximo_tempo = get_absolute_time();

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
            proximo_tempo = delayed_by_us(proximo_tempo, 1000000);
        }
    sleep_ms(1);
    }
}
