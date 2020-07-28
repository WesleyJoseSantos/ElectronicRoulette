/**
 * @file main.cpp
 * @author Wesley José Santos (binary-quantum.com)
 * @brief Arquivo principal do código
 * @version 1.0
 * @date 2020-07-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <Arduino.h>
#include "ElectronicRoulette.h"

ElectronicRoulette roleta;        //!< Instância global da roleta

uint8_t numerosDaSorte[24] = {3, 2, 4, 2, 3, 4, 1, 5, 6, 3, 7, 2, 1, 5, 2, 5, 3, 6, 5, 4, 1, 8, 3, 1};    //!< Lista de números a serem sorteados

/**
 * @brief Setup principal do Arduino
 * 
 */
void setup() {
  Serial.begin(9600);                           //Inicializa a porta serial

  //Configurações da roleta
  roleta.setLedCount(8);                        //Configura a quantidade de leds da roleta
  roleta.setSpeed(75);                          //Configura a velocidade inicial da roleta
  roleta.setDeceleration(3);                    //Configura a intensidade da desaceleração da roleta
  roleta.setDuration(250);                      //Configura o intervalo de tempo entre o movimento de um led para o outro, que irá disparar o stop da roleta
  
  //Configurações do sorteio
  roleta.setNumbersList(numerosDaSorte);        //Configura os numerosDaSorte como sendo a lista de números a serem sortedos pela roleta

  //Configurações do buzzer
  roleta.setBuzzerDuration(20);                 //Configura a duração do som do buzzer. Aumentar a duração do buzzer reduz a velocidade da roleta.
  roleta.setBuzzerTone(500);                    //Configura a frequência do som do buzzer

  roleta.begin();                               //Inicializa a roleta
}

/**
 * @brief Loop principal do Arduino
 * 
 */
void loop() {
  // put your main code here, to run repeatedly:
  roleta.task();
  roleta.printLedsStatus();
}