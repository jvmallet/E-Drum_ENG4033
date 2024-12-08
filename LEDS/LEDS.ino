#include <FastLED.h>

#define NUM_LEDS_1 13   // Número de LEDs na primeira fita
#define NUM_LEDS_2 13   // Número de LEDs na segunda fita
#define NUM_LEDS_3 13   // Número de LEDs na terceira fita
#define LED_PIN_1 7     // Pino para a primeira fita
#define LED_PIN_2 6     // Pino para a segunda fita
#define LED_PIN_3 4     // Pino para a terceira fita

CRGB leds1[NUM_LEDS_1]; // Array para a primeira fita
CRGB leds2[NUM_LEDS_2]; // Array para a segunda fita
CRGB leds3[NUM_LEDS_3]; // Array para a terceira fita

void setup() {
  // Inicializa as fitas
  FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds1, NUM_LEDS_1);
  FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds2, NUM_LEDS_2);
  FastLED.addLeds<WS2812B, LED_PIN_3, GRB>(leds3, NUM_LEDS_3);
  FastLED.setBrightness(50);  // Ajusta o brilho (0-255)

  Serial.begin(31250);         // Inicializa a comunicação serial com a taxa MIDI
  Serial.println("Sistema iniciado.");
}

void loop() {
  // Verifica se há dados disponíveis na porta serial
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Lê o comando
    input.trim();

    if (input.startsWith("led")) {
      int ledNumber = input.substring(4).toInt(); // Extrai o número do LED
      Serial.print("Comando recebido: ");
      Serial.println(input);

      if (ledNumber == 1) {
        acenderEApagar(leds1, NUM_LEDS_1, CRGB::Red);  // Acende e apaga a fita 1
      } else if (ledNumber == 2) {
        acenderEApagar(leds2, NUM_LEDS_2, CRGB::Blue); // Acende e apaga a fita 2
      } else if (ledNumber == 3) {
        acenderEApagar(leds3, NUM_LEDS_3, CRGB::Green); // Acende e apaga a fita 3
      } else {
        Serial.println("Número de LED inválido.");
      }
    } else {
      Serial.println("Comando inválido.");
    }
  }
}

// Função para acender e apagar LEDs após um intervalo de tempo
void acenderEApagar(CRGB* leds, int numLeds, CRGB color) {
  // Acende os LEDs
  for (int i = 0; i < numLeds; i++) {
    leds[i] = color;
  }
  FastLED.show();

  delay(1500); // Aguarda 1.5 segundos (tempo que o LED fica aceso)

  // Apaga os LEDs
  for (int i = 0; i < numLeds; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}
