/**
 * @file bits_effects.c
 * @author Wesley José Santos (binary-quantum.com)
 * @brief Biblioteca para controlar efeitos utilizando deslocamento de bits
 * @version 1.0
 * @date 2020-07-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "bits_effects.h"

/**
 * Variáveis globais
 */
bits_effects_t bits_effects;        //!< Instância da estrutura de dados para controlar os efeitos
uint8_t i;                          //!< Índice auxiliar para controlar os efeitos
uint32_t time;                      //!< Tempo calculado com base na velocidade do efeito, e as constantes de delay (DEFAULT_MAX_DELAY, DEFAULT_MIN_DELAY)
uint32_t all_on;                    //!< Estado calculado que permite acionar todos os bits da cadeia de bits configurada

/**
 * Protótipos das funções privadas
 */
void bits_effects_delay();
void bits_effects_print_bits();

/**
 * Efeitos
 */

/**
 * @brief Acende os leds do primeiro para o último
 * 
 */
void bits_effects_ramp_up_on(){
    static bool effect_started = false;

    if(!effect_started){
        i = 0;
        bits_effects.bits = 0;
        effect_started = true;
        return;
    }

    if(i >= bits_effects.size){
        effect_started = false;
        bits_effects.effect_done = true;
        return;
    }

    bitSet(bits_effects.bits, i);
    i++;

    bits_effects_delay();
}

/**
 * @brief Apaga os leds do primeiro para o último
 * 
 */
void bits_effects_ramp_up_off(){
    static bool effect_started = false;

    if(!effect_started){
        i = 0;
        bits_effects.bits = all_on;
        effect_started = true;
        return;
    }

    if(i >= bits_effects.size){
        effect_started = false;
        bits_effects.effect_done = true;
        return;
    }

    bitClear(bits_effects.bits, i);
    i++;

    bits_effects_delay();
}

/**
 * @brief Acende os leds do último para o primeiro
 * 
 */
void bits_effects_ramp_down_on(){
    static bool effect_started = false;

    if(!effect_started){
        bits_effects.bits = 0;
        i = bits_effects.size ;
        effect_started = true;
        return;
    }

    i--;
    bitSet(bits_effects.bits, i);

    bits_effects_delay();

    if(i <= 0){
        effect_started = false;
        bits_effects.effect_done = true;
        return;
    }
}

/**
 * @brief Apaga os leds do último para o primeiro
 * 
 */
void bits_effects_ramp_down_off(){
    static bool effect_started = false;

    if(!effect_started){
        bits_effects.bits = all_on;
        i = bits_effects.size;
        effect_started = true;
        return;
    }


    i--;
    bitClear(bits_effects.bits, i);

    bits_effects_delay();

    if(i <= 0){
        effect_started = false;
        bits_effects.effect_done = true;
        return;
    }
}

/**
 * @brief Inverte os leds do primeiro para o último
 * 
 */
void bits_effects_flash_swap_up(){
    static bool effect_started = false;

    if(!effect_started){
        bits_effects.bits = all_on / 3;
        effect_started = true;
        i = 0;
        return;
    }

    bits_effects.bits = bits_effects.bits << 1;
    i++;

    bits_effects_delay();
    bits_effects_delay();

    if(i >= bits_effects.size){
        effect_started = false;
        bits_effects.effect_done = true;
        return;
    }
}

/**
 * @brief Inverte os leds do último para o primeiro
 * 
 */
void bits_effects_flash_swap_down(){
    static bool effect_started = false;

    if(!effect_started){
        bits_effects.bits = (all_on / 3) << 1;
        effect_started = true;
        i = 0;
        return;
    }

    bits_effects.bits = bits_effects.bits >> 1;
    i++;

    bits_effects_delay();
    bits_effects_delay();

    if(i >= bits_effects.size){
        effect_started = false;
        bits_effects.effect_done = true;
        return;
    }
}

/**
 * @brief Inverte os leds mantendo a sequencia estática
 * 
 */
void bits_effects_flash_swap(){
    static bool effect_started = false;

    if(!effect_started){
        bits_effects.bits = all_on / 3;
        effect_started = true;
        i = 0;
        return;
    }

    if(i % 2 == 0) bits_effects.bits = bits_effects.bits << 1;
    else bits_effects.bits = bits_effects.bits >> 1;
    i++;

    bits_effects_delay();
    bits_effects_delay();

    if(i >= bits_effects.size){
        effect_started = false;
        bits_effects.effect_done = true;
        return;
    }
}

/**
 * @brief Pisca todos os leds
 * 
 */
void bits_effects_flash(){
    static bool effect_started = false;

    if(!effect_started){
        bits_effects.bits = all_on;
        effect_started = true;
        i = 0;
        return;
    }

    if(i % 2 == 0) bits_effects.bits = all_on;
    else bits_effects.bits = 0;
    i++;

    bits_effects_delay();
    bits_effects_delay();

    if(i >= 16){
        effect_started = false;
        bits_effects.effect_done = true;
        return;
    }
}

/**
 * @brief Lista de efeitos a serem executados na função bits_effects_all
 * 
 */
void (*effects[])(void) = {
    bits_effects_ramp_up_on,
    bits_effects_ramp_up_off,
    bits_effects_ramp_down_on,
    bits_effects_ramp_down_off,
    bits_effects_ramp_up_on,
    bits_effects_ramp_up_off,
    bits_effects_ramp_down_on,
    bits_effects_ramp_down_off,
    bits_effects_flash_swap_up,
    bits_effects_flash_swap_down,
    bits_effects_flash_swap_up,
    bits_effects_flash_swap_down,
    bits_effects_flash_swap,
    bits_effects_flash_swap,
    bits_effects_flash_swap,
    bits_effects_flash_swap,
    bits_effects_flash,
    bits_effects_flash,
};

/**
 * Funções Públicas
 */

/**
 * @brief Inicializa a biblioteca bits_effects
 * 
 * @param effects_cfg Estrutura de dados com as configurações dos efeitos
 */
void bits_effects_init(bits_effects_t effects_cfg){
    bits_effects.size = effects_cfg.size;
    bits_effects.speed = effects_cfg.speed;
    bits_effects.bits = 0;
    bits_effects.effect_done = false;
    bits_effects.effects_done = false;
    bits_effects.selected_effect = 0;
    time = map(bits_effects.speed, 0, 100, DEFAULT_MAX_DELAY, DEFAULT_MIN_DELAY);
    all_on = pow(2, bits_effects.size) - 1;
    i = 0;
}

/**
 * @brief Executa todos os efeitos programados na lista de efeitos
 * 
 * @return true Assim que a lista de efeitos é concluída
 * @return false Enquanto a lista de efeitos estiver sendo processada
 */
bool bits_effects_all(){
    bits_effects.selected_effect += bits_effects.effect_done ? 1 : 0;
    bits_effects.effects_done = bits_effects.selected_effect >= EFFECTS_COUNT;
    bits_effects.selected_effect = bits_effects.effects_done ? 0 : bits_effects.selected_effect;
    bool ret = bits_effects.effect_done;
    bits_effects.effect_done = false;

    effects[bits_effects.selected_effect]();

    bits_effects.effects_done = false;
    return ret;
}

/**
 * @brief Reinicia a seleção dos efeitos
 * 
 */
void bits_effects_reset(){
    bits_effects.selected_effect = 0;
    bits_effects.bits = 0;
    bits_effects.effect_done = false;
    bits_effects.effects_done = false;
    i = 0;
}

/**
 * @brief Obtém os valores dos 32 bits processados pela biblioteca
 * 
 * @return uint32_t variável contendo os 32 bits processados
 */
uint32_t bits_effects_get_bits(){
    return bits_effects.bits;
}

/**
 * @brief Testa a biblioteca escrevendo a saída do processamento no serial monitor
 * 
 */
void bits_effects_test(){
    Serial.begin(9600);

    while (1)
    {
        bits_effects_all();
        bits_effects_print_bits();
    }
    
}

/**
 * Funções privadas
 */

/**
 * @brief Aguarda o tempo calculado com base na velocidade do efeito, e nas constantes de delay (DEFAULT_MAX_DELAY, DEFAULT_MIN_DELAY)
 * 
 */
void bits_effects_delay(){
    delay(time);
}

/**
 * @brief Imprime a saída do processamento da biblioteca no serial monitor
 * 
 */
void bits_effects_print_bits(){
    Serial.print(bits_effects.bits);
    Serial.print('\t');

    for (size_t i = 0; i < bits_effects.size; i++)
    {
        Serial.print(bitRead(bits_effects.bits, i));
    }
    Serial.println();
}