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

#include "ElectronicRoulette.h"

ElectronicRoulette roleta;        //!< Instância global da roleta.

uint8_t numerosDaSorte[24] = {3, 2, 4, 2, 3, 4, 1, 5, 6, 3, 7, 2, 1, 5, 2, 5, 3, 6, 5, 4, 1, 8, 3, 1};    //!< Lista de números a serem sorteados.

/**
 * @brief Setup principal do Arduino
 * 
 */
void setup() {
  Serial.begin(9600);                           //Inicializa a porta serial.

  //Configurações da roleta
  roleta.setLedCount(8);                        //Quantidade de leds da roleta.
  roleta.setSpeed(75);                          //Velocidade inicial da roleta. Quanto maior, mais rápido a roleta.
  roleta.setDeceleration(3);                    //Intensidade da desaceleração da roleta. Quanto maior, mais rapido a roleta se torna lenta.
  roleta.setDuration(250);                      //Intervalo máximo entre um led e o outro. Quanto maior o intervalo, mais tempo a roleta permanece girando.
  
  //Configurações do sorteio
  roleta.setNumbersList(numerosDaSorte);        //Configura os numerosDaSorte como sendo a lista de números a serem sortedos pela roleta.

  //Configurações do buzzer
  roleta.setBuzzerDuration(20);                 //Configura a duração do som do buzzer. Aumentar a duração do buzzer reduz a velocidade da roleta.
  roleta.setBuzzerTone(500);                    //Configura a frequência do som do buzzer.

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