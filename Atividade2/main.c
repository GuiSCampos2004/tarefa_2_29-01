#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

const uint BOT = 5;
const uint LED1 = 11;
const uint LED2 = 12;
const uint LED3 = 13;

bool teste_bot = true; //Variavel para impedir que o botão interfira na rotina
static volatile uint32_t ultimo_tempo = 0; //Variavel utilizada em debounce
//--------------------------------------------------------------------------------------------------------
int64_t turn_off_callback1(alarm_id_t id, void *user_data) { //Callback que desliga o primeiro led

    gpio_put(LED1, 0);

    return 0;
}
int64_t turn_off_callback2(alarm_id_t id, void *user_data) {//Callback que desliga o segundo led

    gpio_put(LED2, 0);

    return 0;
}
int64_t turn_off_callback3(alarm_id_t id, void *user_data) {//Callback que desliga o terceiro led e libera o botão

    gpio_put(LED3, 0);
    teste_bot = true;

    return 0;
}
//--------------------------------------------------------------------------------------------------------

static void gpio_irq_handler(uint gpio, uint32_t events){ //Rotinade interrupção para uso do botão
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());
    if(((tempo_atual-ultimo_tempo)>200000)&&(teste_bot)){ //Debonce e testando se o botão esta travado
        ultimo_tempo = tempo_atual;

        gpio_put(LED1, 1);
        gpio_put(LED2, 1);
        gpio_put(LED3, 1);

        add_alarm_in_ms(3000, turn_off_callback1, NULL, false); //Inicio do alarme 1
        add_alarm_in_ms(6000, turn_off_callback2, NULL, false); //Inicio do alarme 2
        add_alarm_in_ms(9000, turn_off_callback3, NULL, false); //Inicio do alarme 3

        teste_bot = false; //Travando o botão
    }
}
//--------------------------------------------------------------------------------------------------------
int main()
{
    //----------------------------------------------------------------------------------------------------
    //Inicialização de portas
    stdio_init_all();


    gpio_init(BOT);
    gpio_set_dir(BOT, GPIO_IN);
    gpio_pull_up(BOT);
    gpio_init(LED1);
    gpio_set_dir(LED1, GPIO_OUT);
    gpio_put(LED1, 0);
    gpio_init(LED2);
    gpio_set_dir(LED2, GPIO_OUT);
    gpio_put(LED2, 0);
    gpio_init(LED3);
    gpio_set_dir(LED3, GPIO_OUT);
    gpio_put(LED3, 0);
    //----------------------------------------------------------------------------------------------------

    gpio_set_irq_enabled_with_callback(BOT, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); //Inicialização da rotina de interrupção

    while (true) {
        sleep_ms(1); //
    }
}
