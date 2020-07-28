/**
 * @file ElectronicRoulette.h
 * @author Wesley José Santos (binary-quantum.com)
 * @brief Classe principal do projeto
 * @version 0.1
 * @date 2020-07-27
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef __ELECTRONICROULETTE__H__
#define __ELECTRONICROULETTE__H__

#include <Arduino.h>
#include "bits_effects.h"

#define DELAY_MIN 0                     //!< Delay máximo para ajuste da velocidade máxima da roleta
#define DELAY_MAX 250                   //!< Delay mínimo para ajuste da velocidade mínima da roleta
#define DEFAULT_LED_COUNT 8             //!< Quantidade de leds padrão da roleta
#define DEFAULT_INITIAL_SPEED 80        //!< Velocidade inicial padrão da roleta
#define DEFAULT_BT_RDY_PIN 2            //!< Pino padrão para o botão que prepara a roleta para o sorteio
#define DEFAULT_BT_START_PIN 3          //!< Pino padrão para o botão que inicia o sorteio da roleta
#define DEFAULT_BUZ_PIN 13              //!< Pino padrão para conexão do buzzer
#define DEFAULT_INITIAL_PIN 4           //!< Pino inicial padrão da cadeia de leds da roleta
#define DEFAULT_DECELERATION 3          //!< Intensidade inicial padrão da desaceleração da roleta   
#define DEFAULT_STOP 250                //!< Valor da desaceleração necessário para parar a roleta
#define DEFAULT_LIST_SIZE 24            //!< Valor padrão para o tamanho da lista dos numeros sorteados
#define DEFAULT_BUZZER_DURATION 20      //!< Valor padrão para a duração do som do buzzer
#define DEFAULT_BUZZER_TONE 500         //!< Tom padrão do buzzer

/**
 * @brief Estados da roleta eletrônica
 */
enum ElectronicRouletteState
{
    ST_IDLE,          //!< Aguardando comando, reproduzindo efeitos nos leds
    ST_READY,         //!< Aguardando comando, leds apagados
    ST_DRAWING,       //!< Realizando sorteio
    ST_DRAWN          //!< Sorteio realizado. Aguardando comando.
};

/**
 * @brief Classe principal da roleta eletrônica
 */
class ElectronicRoulette
{
private:
    ElectronicRouletteState state;                  //!< Estado da roleta eletrônica
    uint32_t ledsStatus;                            //!< Variável de 32 bits para armazenar os estados dos leds
    uint32_t maxLedsStatus;                         //!< Variável de 32 bits para armazenar o estado máximo dos leds
    uint8_t ledsCount;                              //!< Quantidade de leds da roleta eletrônica (0 - 32)
    uint8_t speed;                                  //!< Velocidade da roleta eletrônica
    uint16_t time;                                  //!< Tempo calculado com base na velocidade da roleta, e as constantes de delay
    uint8_t initialPin;                             //!< Pino inicial da cadeia de leds
    uint8_t buttonReadyPin;                         //!< Pino do botão que prepara a roleta (desligando o efeito dos leds)
    uint8_t buttonStartRoulettePin;                 //!< Pino do botão que inicia o sorteio da roleta
    uint8_t selectedLed;                            //!< Led selecionado atualmente na roleta
    uint8_t deceleration;                           //!< Intensidade da desaceleração da roleta
    uint16_t stopDeceleration;                      //!< Valor utilizado para parar a roleta.
    uint8_t listIdx;                                //!< Índice da lista numbersList
    uint8_t buzzerPin;                              //!< Pino do buzzer para efeito sonoro
    uint16_t buzzerTone;                            //!< Valor do tone do buzzer
    uint8_t buzzerToneDuration;                     //!< Duração do tone do buzzer
    uint8_t numbersList[DEFAULT_LIST_SIZE];         //!< Sequência de números que serão sorteados
    void effects();
    void updateLeds();
    void turnOff();
    void randomizeNumbersList();
    void drawing();
    void flashSelectedLed();
public:
    ElectronicRoulette();
    void begin();
    void task();
    void setInitialLedsPins(uint8_t initialPin);
    void setLedCount(uint8_t ledCount);
    void setSpeed(uint8_t speed);
    void setBuzzerTone(uint16_t tone);
    void setBuzzerDuration(uint8_t duration);
    void setDeceleration(uint8_t deceleration);
    void setDuration(uint8_t duration);
    void setNumbersList(uint8_t numbersList[24]);
    void test();
    void printLedsStatus();
};

#endif  //!__ELECTRONICROULETTE__H__
