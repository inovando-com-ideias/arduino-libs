// ADConverter.cpp — v0.3
// Autor : Flavio Guimarães — Inovando com Ideias
// GitHub: github.com/inovando-com-ideias
// Licença: MIT

#include "ADConverter.h"

// -----------------------------------------------------------------------
// Construtor
// v_ref = -99.0f sinaliza "usar padrão da plataforma"
// -----------------------------------------------------------------------
ADConverter::ADConverter(uint8_t pino, float v_ref)
    : _pino(pino), _adcMax(4095), _vRef(3.3f)
{
    _configurar(v_ref);
}

// -----------------------------------------------------------------------
// Configuração principal
// -----------------------------------------------------------------------
void ADConverter::_configurar(float v_ref) {

    // Define v_ref padrão da plataforma se não informado
    float vref_solicitado = v_ref;

    if (v_ref == -99.0f) {
        #if defined(ARDUINO_ARCH_AVR)
            vref_solicitado = 5.0f;
        #else
            vref_solicitado = 3.3f;
        #endif
    }

    // Valida e aplica configuração específica da plataforma
    _validarVref(vref_solicitado);

    #if defined(ARDUINO_ARCH_ESP32)
        _adcMax = 4095;
        analogReadResolution(12);
        _aplicarVrefESP32(vref_solicitado);

    #elif defined(ARDUINO_ARCH_ESP8266)
        _adcMax = 1023;
        // ESP8266: v_ref fixo, já validado

    #elif defined(ARDUINO_ARCH_RP2040)
        _adcMax = 4095;
        analogReadResolution(12);
        // RP2040: v_ref fixo 3.3V, já validado

    #elif defined(ARDUINO_ARCH_STM32)
        _adcMax = 4095;
        analogReadResolution(12);
        // STM32: v_ref fixo 3.3V, já validado

    #elif defined(ARDUINO_ARCH_AVR)
        _adcMax = 1023;
        _aplicarVrefAVR(vref_solicitado);

    #else
        // Plataforma desconhecida — comportamento seguro
        _adcMax = 1023;
        _vRef   = vref_solicitado;
    #endif

    _vRef = vref_solicitado;
}

// -----------------------------------------------------------------------
// Validação de v_ref por plataforma
// -----------------------------------------------------------------------
void ADConverter::_validarVref(float v_ref) {

    bool valido = false;

    #if defined(ARDUINO_ARCH_ESP32)
        valido = (v_ref == 1.0f  ||
                  v_ref == 1.34f ||
                  v_ref == 2.0f  ||
                  v_ref == 3.3f);
        if (!valido) {
            Serial.println(
                "ERRO ADConverter: v_ref invalido para ESP32.\n"
                "Valores validos: 1.0, 1.34, 2.0, 3.3"
            );
            while(true); // Trava — força o dev a corrigir
        }

    #elif defined(ARDUINO_ARCH_ESP8266)
        valido = (v_ref == 3.3f);
        if (!valido) {
            Serial.println(
                "ERRO ADConverter: ESP8266 so suporta v_ref=3.3"
            );
            while(true);
        }

    #elif defined(ARDUINO_ARCH_RP2040)
        valido = (v_ref == 3.3f);
        if (!valido) {
            Serial.println(
                "ERRO ADConverter: RP2040 so suporta v_ref=3.3"
            );
            while(true);
        }

    #elif defined(ARDUINO_ARCH_STM32)
        valido = (v_ref == 3.3f);
        if (!valido) {
            Serial.println(
                "ERRO ADConverter: STM32 so suporta v_ref=3.3"
            );
            while(true);
        }

    #elif defined(ARDUINO_ARCH_AVR)
        valido = (v_ref == 1.0f  ||
                  v_ref == 1.1f  ||
                  v_ref == 2.0f  ||
                  v_ref == 2.56f ||
                  v_ref == 5.0f  ||
                  v_ref == VREF_EXTERNO);
        if (!valido) {
            Serial.println(
                "ERRO ADConverter: v_ref invalido para AVR.\n"
                "Valores validos: 1.1 (UNO), 2.56 (Mega), 5.0, VREF_EXTERNO"
            );
            while(true);
        }
    #endif
}

// -----------------------------------------------------------------------
// Aplica v_ref no AVR via analogReference()
// -----------------------------------------------------------------------
void ADConverter::_aplicarVrefAVR(float v_ref) {

    if (v_ref == VREF_EXTERNO) {
        analogReference(EXTERNAL);

    } else if (v_ref == 5.0f) {
        analogReference(DEFAULT);

    } else if (v_ref == 1.1f || v_ref == 1.0f) {
        #if defined(__AVR_ATmega2560__) // Mega
            analogReference(INTERNAL1V1);
        #else
            analogReference(INTERNAL);  // UNO, Leonardo
        #endif

    } else if (v_ref == 2.56f || v_ref == 2.0f) {
        #if defined(__AVR_ATmega2560__) // Mega
            analogReference(INTERNAL2V56);
        #else
            Serial.println(
                "ERRO ADConverter: v_ref=2.56 so disponivel no Mega."
            );
            while(true);
        #endif
    }
}

// -----------------------------------------------------------------------
// Aplica v_ref no ESP32 via atenuação
// -----------------------------------------------------------------------
void ADConverter::_aplicarVrefESP32(float v_ref) {

    if      (v_ref == 1.0f)  analogSetPinAttenuation(_pino, ADC_0db);
    else if (v_ref == 1.34f) analogSetPinAttenuation(_pino, ADC_2_5db);
    else if (v_ref == 2.0f)  analogSetPinAttenuation(_pino, ADC_6db);
    else if (v_ref == 3.3f)  analogSetPinAttenuation(_pino, ADC_11db);
}

// -----------------------------------------------------------------------
// Leituras
// -----------------------------------------------------------------------
int ADConverter::ler() {
    return analogRead(_pino);
}

float ADConverter::lerNormalizado() {
    return (float)ler() / _adcMax;
}

float ADConverter::lerTensao() {
    return lerNormalizado() * _vRef;
}