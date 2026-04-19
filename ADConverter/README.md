# ADConverter

Classe base para leitura analógica (ADC) em C++ para Arduino IDE.  
Detecta automaticamente a plataforma e configura hardware e VREF.

## Plataformas suportadas

| Plataforma                | Chip           | Resolução        | V_Ref padrão            |
|---------------------------|----------------|------------------|-------------------------|
| Arduino UNO / Leonardo    | AVR ATmega328P | 10 bits (0–1023) | 5.0V                    |
| Arduino Mega              | AVR ATmega2560 | 10 bits (0–1023) | 5.0V                    |
| ESP8266 (NodeMCU/D1 Mini) | ESP8266        | 10 bits (0–1023) | 3.3V (divisor na placa) |
| ESP8266 (ESP-01)          | ESP8266        | 10 bits (0–1023) | 1.0V (sem divisor)      |
| ESP32 / S3 / C3           | ESP32          | 12 bits (0–4095) | 3.3V                    |
| Raspberry Pi Pico         | RP2040         | 12 bits (0–4095) | 3.3V                    | 
| STM32 Blackpill / Nucleo  | STM32          | 12 bits (0–4095) | 3.3V                    |

## Instalação

1. Baixe ou clone este repositório
2. Copie a pasta `ADConverter` para:
   - Windows: `Documentos\Arduino\libraries\`
   - Linux/Mac: `~/Arduino/libraries/`
3. Reinicie o Arduino IDE

## Uso básico

```cpp
#include <ADConverter.h>

// v_ref detectado automaticamente pela plataforma
ADConverter adc(A0);

void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.println(adc.ler());          // valor bruto
    Serial.println(adc.lerNormalizado()); // 0.0 a 1.0
    Serial.println(adc.lerTensao());    // em Volts
    delay(500);
}
```

## Configurando V_Ref

```cpp
// ESP32 — range 0~1V (alta precisão para sinais pequenos)
ADConverter adc(34, 1.0);

// AVR UNO — referência interna 1.1V
ADConverter adc(A0, 1.1);

// AVR Mega — referência interna 2.56V
ADConverter adc(A0, 2.56);

// AVR — referência externa no pino AREF
ADConverter adc(A0, VREF_EXTERNO);
```

## Valores válidos de V_Ref por plataforma

| V_Ref        | ESP8266    | ESP32 | AVR UNO | AVR Mega | RP2040 | STM32 |
|--------------|------------|-------|---------|----------|--------|-------|
| 1.0V         | ✅ ESP-01  | ✅   | ❌      | ❌      | ❌    | ❌    |
| 1.1V         | ❌         | ❌   | ✅      | ✅      | ❌    | ❌    |
| 2.0V         | ❌         | ✅   | ❌      | ❌      | ❌    | ❌    |
| 2.56V        | ❌         | ❌   | ❌      | ✅      | ❌    | ❌    |
| 3.3V         | ✅ NodeMCU | ✅   | ❌      | ❌      | ✅    | ✅    |
| 5.0V         | ❌         | ❌   | ✅      | ✅      | ❌    | ❌    |
| VREF_EXTERNO | ❌         | ❌   | ✅      | ✅      | ❌    | ❌    |

## Métodos

| Método | Retorno | Descrição |
|---|---|---|
| `ler()` | `int` | Valor bruto do ADC |
| `lerNormalizado()` | `float` | Valor entre 0.0 e 1.0 |
| `lerTensao()` | `float` | Tensão em Volts |
| `vRef()` | `float` | V_Ref configurado |
| `resolucao()` | `int` | Valor máximo do ADC |
| `pino()` | `uint8_t` | Pino configurado |

## Herança

Esta classe foi projetada para ser extendida:

```cpp
#include <ADConverter.h>

class Potenciometro : public ADConverter {
  public:
    Potenciometro(uint8_t pino, float v_ref = -99.0f)
        : ADConverter(pino, v_ref) {}

    float percentual() {
        return lerNormalizado() * 100.0f;
    }
};
```

## Exemplos

- [`examples/basic/basic.ino`](examples/basic/basic.ino) — leitura simples
- [`examples/com_tensao/com_tensao.ino`](examples/com_tensao/com_tensao.ino) — leitura com barra visual

## Licença

MIT — Flavio Guimarães — [Inovando com Ideias](https://youtube.com/@inovandocomideias)