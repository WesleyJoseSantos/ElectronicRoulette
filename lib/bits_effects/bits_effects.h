/**
 * @file bits_effects.h
 * @author Wesley José Santos (binary-quantum.com)
 * @brief Biblioteca para controlar efeitos utilizando deslocamento de bits
 * @version 1.0
 * @date 2020-07-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef __BITSEFFECTS__H__
#define __BITSEFFECTS__H__

#include <Arduino.h>

#define DEFAULT_MAX_DELAY 150
#define DEFAULT_MIN_DELAY 30
#define EFFECTS_COUNT 18

/**
 * @brief Estrutura de dados para controlar os efeitos
 * 
 */
typedef struct
{
    uint8_t size;                   //!< Quantidade de bits até 32, a serem utilizados para os efeitos
    uint8_t speed;                  //!< Velocidade dos efeitos
    uint32_t bits;                  //!< Variável para controlar o efeito de 32 bits
    bool effect_done;               //!< Efeito concluído
    bool effects_done;              //!< Todos os efeitos concluídos
    uint8_t selected_effect;        //!< Efeito selecionado
}bits_effects_t;

void bits_effects_init(bits_effects_t effects_cfg);
bool bits_effects_all();
void bits_effects_reset();
uint32_t bits_effects_get_bits();
void bits_effects_test();

#endif  //!__BITSEFFECTS__H__
