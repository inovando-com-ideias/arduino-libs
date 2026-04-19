// com_tensao.ino — Exemplo com barra visual de tensão
// Autor : Flavio Guimarães — Inovando com Ideias
// GitHub: github.com/inovando-com-ideias

#include <ADConverter.h>

ADConverter adc(A0);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("=== ADConverter — Monitor de Tensao ===");
    Serial.print("V_Ref configurado: ");
    Serial.print(adc.vRef());
    Serial.println("V");
    Serial.println("=======================================");
}

void loop() {
    float tensao = adc.lerTensao();

    // Barra visual proporcional à tensão
    int   barras = (int)((tensao / adc.vRef()) * 20);
    
    Serial.print("[");
    for (int i = 0; i < 20; i++) {
        Serial.print(i < barras ? "#" : ".");
    }
    Serial.print("] ");
    Serial.print(tensao, 2);
    Serial.println("V");

    delay(500);
}