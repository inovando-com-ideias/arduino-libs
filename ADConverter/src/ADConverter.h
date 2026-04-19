// ADConverter.h — v0.3
// Classe base para leitura ADC multiplataforma — Arduino IDE
// Suporte: ESP8266, ESP32 (S3/C3), RP2040, STM32, AVR (UNO/Mega/Leonardo)
//
// Autor : Flavio Guimarães — Inovando com Ideias
// GitHub: github.com/inovando-com-ideias
// Licença: MIT

#ifndef ADCONVERTER_H
#define ADCONVERTER_H

#include <Arduino.h>

// -----------------------------------------------------------------------
// Constantes de v_ref especiais
// -----------------------------------------------------------------------
#define VREF_EXTERNO  -1.0f   // Sinaliza uso do pino AREF (só AVR)

class ADConverter {
  public:

    // -------------------------------------------------------------------
    // Construtor
    // pino   : pino ADC (ex: A0, 34, 26)
    // v_ref  : tensão de referência em Volts
    //          Padrão: tensão de operação da placa (auto-detectada)
    //          Use VREF_EXTERNO para habilitar pino AREF (só AVR)
    // -------------------------------------------------------------------
    ADConverter(uint8_t pino, float v_ref = -99.0f);

    virtual ~ADConverter() {}

    // -------------------------------------------------------------------
    // Leituras
    // -------------------------------------------------------------------
    virtual int   ler();
    float         lerNormalizado();
    float         lerTensao();

    // -------------------------------------------------------------------
    // Propriedades
    // -------------------------------------------------------------------
    float    vRef()     const { return _vRef; }
    int      resolucao()const { return _adcMax; }
    uint8_t  pino()     const { return _pino; }

  protected:
    uint8_t  _pino;
    int      _adcMax;
    float    _vRef;

  private:
    void _configurar(float v_ref);
    void _validarVref(float v_ref);
    void _aplicarVrefAVR(float v_ref);
    void _aplicarVrefESP32(float v_ref);
};

#endif