/**
 * @file ElectronicRoulette.cpp
 * @author Wesley José Santos (binary-quantum.com)
 * @brief Classe principal do projeto
 * @version 1.0
 * @date 2020-07-27
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "ElectronicRoulette.h"

/**
 * Variáveis globals
 * 
 */

ElectronicRouletteState *stateToInterrupt;      //!< Estado da roleta eletrônica a ser manipulado pela interrupção
bool filter = false;                            //!< Filtro para o botão que aciona os efeitos

/**
 * Interrupções
 */

/**
 * @brief Função a ser chamada pela interrupção quando o botão que prepara a roleta for pressionado
 * 
 */
void buttonReadyRoulettePressed(){
    if(*stateToInterrupt == ElectronicRouletteState::ST_IDLE && !filter){
        *stateToInterrupt = ElectronicRouletteState::ST_READY;
    }
    if(*stateToInterrupt == ElectronicRouletteState::ST_DRAWN){
        *stateToInterrupt = ElectronicRouletteState::ST_IDLE;
        filter = true;
    }
}

/**
 * @brief Função a ser chamada pela interrupção quando o botão que inicia o sorteio for pressionado
 * 
 */
void buttonStartRoulettePressed(){
    if(*stateToInterrupt == ElectronicRouletteState::ST_READY){
        *stateToInterrupt = ElectronicRouletteState::ST_DRAWING;
    }
}

/**
 * Métodos públicos
 */

/**
 * @brief Constrói um novo objeto Electronic Roulette:: Electronic Roulette
 * 
 */
ElectronicRoulette::ElectronicRoulette(){
    this->state = ElectronicRouletteState::ST_IDLE;
    this->ledsStatus = 0;
    this->ledsCount = DEFAULT_LED_COUNT;
    this->maxLedsStatus = pow(2, ledsCount - 1);
    this->speed = DEFAULT_INITIAL_SPEED;
    this->time = map(this->speed, 0, 100, DELAY_MAX, DELAY_MIN);
    this->initialPin = DEFAULT_INITIAL_PIN;
    this->buttonReadyPin = DEFAULT_BT_RDY_PIN;
    this->buttonStartRoulettePin = DEFAULT_BT_START_PIN;
    this->selectedLed = 0;
    this->deceleration = DEFAULT_DECELERATION;
    this->stopDeceleration = DEFAULT_STOP;
    this->listIdx = 0;
    this->buzzerPin = DEFAULT_BUZ_PIN;
    this->buzzerTone = DEFAULT_BUZZER_TONE;
    this->buzzerToneDuration = DEFAULT_BUZZER_DURATION;

    randomizeNumbersList();
}

/**
 * @brief Inicializa a roleta eletrônica
 * 
 */
void ElectronicRoulette::begin(){
    bits_effects_t effects;
    uint8_t start = this->initialPin;
    uint8_t end = this->initialPin + this->ledsCount;

    effects.size = this->ledsCount;
    effects.speed = map(this->speed, 0, 100, 0, 80);
    bits_effects_init(effects);

    for (size_t i = start; i < end; i++)
    {
        pinMode(i, OUTPUT);
    }

    pinMode(this->buttonReadyPin, INPUT_PULLUP);
    pinMode(this->buttonStartRoulettePin, INPUT_PULLUP);

    attachInterrupt(
        digitalPinToInterrupt(this->buttonReadyPin),
        buttonReadyRoulettePressed,
        FALLING
    );

    attachInterrupt(
        digitalPinToInterrupt(this->buttonStartRoulettePin),
        buttonStartRoulettePressed,
        FALLING
    );

    stateToInterrupt = &state;
}

/**
 * @brief Executa as rotinas da roleta eletrônica
 * 
 */
void ElectronicRoulette::task(){
    switch (state)
    {
    case ElectronicRouletteState::ST_IDLE:
        effects();
        break;
    case ElectronicRouletteState::ST_READY:
        turnOff();
        break;
    case ElectronicRouletteState::ST_DRAWING:
        drawing();
        break;
    case ElectronicRouletteState::ST_DRAWN:
        flashSelectedLed();
        break;
    default:
        break;
    }
}

/**
 * @brief Define o pino do primeiro led da cadeia de leds da roleta
 * 
 * @param initialPin Valor do pino
 */
void ElectronicRoulette::setInitialLedsPins(uint8_t initialPin){
    this->initialPin = initialPin;
}

/**
 * @brief Define a quantidade de leds da roleta
 * 
 * @param ledCount A quantidade de leds
 */
void ElectronicRoulette::setLedCount(uint8_t ledCount){
    this->ledsCount = ledCount;
    this->maxLedsStatus = pow(2, ledsCount) - 1;
}

/**
 * @brief Define a velocidade da roleta
 * 
 * @param speed Percentual de velocidade de roleta (0 a 100)
 */
void ElectronicRoulette::setSpeed(uint8_t speed){
    this->speed = speed;
}

/**
 * @brief Define a lista de números a serem sorteados
 * 
 * @param numbersList Lista de números de 24 posições
 */
void ElectronicRoulette::setNumbersList(uint8_t numbersList[24]){
    for (size_t i = 0; i < 24; i++)
    {
        this->numbersList[i] = numbersList[i];
    }
}

/**
 * @brief Defini o tom do buzzer
 * 
 * @param tone O tom a ser definido
 */
void ElectronicRoulette::setBuzzerTone(uint16_t tone){
    this->buzzerTone = tone;
}

/**
 * @brief Define a duração do tom do buzzer
 * 
 * @param duration A duração a ser definida
 */
void ElectronicRoulette::setBuzzerDuration(uint8_t duration){
    this->buzzerToneDuration = duration;
}

/**
 * @brief Define a intensidade de desaceleração da roleta
 * 
 * @param deceleration Intensidade da desaceleração da roleta
 */
void ElectronicRoulette::setDeceleration(uint8_t deceleration){
    this->deceleration = deceleration;
}

/**
 * @brief Define por quanto tempo a roleta permanece em movimento
 * 
 * @param duration Duração do movimento da roleta
 */
void ElectronicRoulette::setDuration(uint8_t duration){
    this->stopDeceleration = duration;
}

/**
 * Métodos privados
 */

/**
 * @brief Executa os efeitos da roleta
 * 
 */
void ElectronicRoulette::effects(){
    if(bits_effects_all()) filter = false;
    this->ledsStatus = bits_effects_get_bits();
    updateLeds();
}

/**
 * @brief Atualiza as saídas que acionam os leds
 * 
 */
void ElectronicRoulette::updateLeds(){
    if(this->ledsStatus != 0) tone(this->buzzerPin, this->buzzerTone, this->buzzerToneDuration);
    for (size_t i = 0; i < ledsCount; i++)
    {
        uint8_t pin = this->initialPin + i;
        bool status = bitRead(ledsStatus, i);
        digitalWrite(pin, status);
    }
}

/**
 * @brief Desliga a roleta
 * 
 */
void ElectronicRoulette::turnOff(){
    ledsStatus = 0;
    updateLeds();
    bits_effects_reset();
}

/**
 * @brief Atribui números aleatórios para serem sorteados na roleta
 * 
 */
void ElectronicRoulette::randomizeNumbersList(){
    for (size_t i = 0; i < 24; i++)
    {
        this->numbersList[i] = random(1, this->ledsCount);
    }
}

/**
 * @brief Realiza o sorteio
 * 
 */
void ElectronicRoulette::drawing(){
    static uint16_t totalDeceleration = 0;
    uint16_t totalTime = this->time + totalDeceleration;

    this->ledsStatus = 0;
    bitSet(this->ledsStatus, this->selectedLed);
    updateLeds();    
    delay(totalTime);

    if(totalTime >= this->stopDeceleration && this->selectedLed == (this->numbersList[listIdx] - 1)){
        this->state = ElectronicRouletteState::ST_DRAWN;
        this->listIdx++;
        totalDeceleration = 0;

        if(this->listIdx >= DEFAULT_LIST_SIZE) this->listIdx = 0;
        return;
    }

    this->selectedLed++;
    totalDeceleration += this->deceleration;
    if (this->selectedLed >= this->ledsCount)
    {
        this->selectedLed = 0;
    }
}

/**
 * @brief Pisca o led sorteado
 * 
 */
void ElectronicRoulette::flashSelectedLed(){
    if(this->ledsStatus == 0){
        bitSet(this->ledsStatus, selectedLed);
        updateLeds();
    }else{
        this->ledsStatus = 0;
        updateLeds();
    }
    delay(150);
}

/**
 * @brief Realiza o teste da roleta
 * @note Necessário remover o método task do loop, para que o teste funcione corretamente
 * 
 */
void ElectronicRoulette::test(){
    Serial.begin(9600);
    Serial.println("Teste da roleta iniciado.");

    Serial.println("Roleta aguardando...\n");
    delay(1000);

    while (millis() < 15000)
    {
        task();
        printLedsStatus();
    }

    state = ElectronicRouletteState::ST_READY;
    Serial.println("Roleta preparada.");
    delay(1000);

    while(millis() < 20000){
        task();
        printLedsStatus();
    }    

    state = ElectronicRouletteState::ST_DRAWING;
    Serial.println("Sorteio iniciado.");
    delay(1000);

    while(1){
        task();
        printLedsStatus();
    }  
}

/**
 * @brief Imprime o estado dos leds
 * 
 */
void ElectronicRoulette::printLedsStatus(){
    Serial.print(ledsStatus);
    Serial.print('\t');

    for (size_t i = 0; i < ledsCount; i++)
    {
        Serial.print(bitRead(ledsStatus, i));
    }
    Serial.println();
}