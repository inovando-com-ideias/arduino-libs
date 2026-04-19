// basic.ino — Exemplo básico de uso da classe ADConverter
// Autor : Flavio Guimarães — Inovando com Ideias
// GitHub: github.com/inovando-com-ideias

#include <ADConverter.h>

// ESP32  → pino 34, v_ref automático (3.3V)
// AVR    → pino A0, v_ref automático (5.0V)
// Pico   → pino A0, v_ref automático (3.3V)
// STM32  → pino A0, v_ref automático (3.3V)
ADConverter adc(A0);

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("=== ADConverter v0.3 ===");
    Serial.print("Pino      : "); Serial.println(adc.pino());
    Serial.print("Resolucao : "); Serial.println(adc.resolucao());
    Serial.print("V_Ref     : "); Serial.println(adc.vRef());
    Serial.println("========================");
}

void loop() {
    int   bruto     = adc.ler();
    float normaliz  = adc.lerNormalizado();
    float tensao    = adc.lerTensao();

    Serial.print("Bruto: ");
    Serial.print(bruto);
    Serial.print(" | Norm: ");
    Serial.print(normaliz, 3);
    Serial.print(" | Tensao: ");
    Serial.print(tensao, 2);
    Serial.println("V");

    delay(500);
}