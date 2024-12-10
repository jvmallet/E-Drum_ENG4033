#include <FastLED.h>

#define NUM_LEDS_1 13 // Número de LEDs na primeira fita
#define NUM_LEDS_2 13 // Número de LEDs na segunda fita
#define NUM_LEDS_3 13 // Número de LEDs na terceira fita

#define LED_PIN_1 7   // Pino da primeira fita
#define LED_PIN_2 6   // Pino da segunda fita
#define LED_PIN_3 5   // Pino da terceira fita

CRGB leds1[NUM_LEDS_1]; // LEDs da primeira fita
CRGB leds2[NUM_LEDS_2]; // LEDs da segunda fita
CRGB leds3[NUM_LEDS_3]; // LEDs da terceira fita

String input = "";

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds1, NUM_LEDS_1);
  FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds2, NUM_LEDS_2);
  FastLED.addLeds<WS2812B, LED_PIN_3, GRB>(leds3, NUM_LEDS_3);
  FastLED.setBrightness(50);
  Serial.begin(9600);
  Serial.println("Arduino iniciado e aguardando comandos...");
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      processCommand(input);
      input = "";
    } else {
      input += c;
    }
  }
}

void processCommand(String command) {
  command.trim();
  Serial.print("Comando recebido: ");
  Serial.println(command);

  if (command.startsWith("led")) {
    int firstSpace = command.indexOf(' ');
    int secondSpace = command.indexOf(' ', firstSpace + 1);

    int ledNumber = command.substring(firstSpace + 1, secondSpace).toInt();
    int bpm = command.substring(secondSpace + 1).toInt();

    int delayTime = 60000 / bpm;

    // Chama a função de acordo com o LED correspondente
    if (ledNumber == 1) {
      acenderEApagar(leds1, 0, NUM_LEDS_1 / 2, CRGB::Yellow, delayTime); // Hi Hat (metade 1 da fita 1)
    } else if (ledNumber == 2) {
      acenderEApagar(leds1, NUM_LEDS_1 / 2, NUM_LEDS_1, CRGB::Blue, delayTime); // Snare (metade 2 da fita 1)
    } else if (ledNumber == 3) {
      acenderEApagar(leds2, 0, NUM_LEDS_2 / 2, CRGB::Red, delayTime); // Bass Drum (metade 1 da fita 2)
    } else if (ledNumber == 4) {
      acenderEApagar(leds2, NUM_LEDS_2 / 2, NUM_LEDS_2, CRGB::Orange, delayTime); // Crash (metade 2 da fita 2)
    } else if (ledNumber == 5) {
      acenderEApagar(leds3, 0, NUM_LEDS_3 / 2, CRGB::Purple, delayTime); // Tom (metade 1 da fita 3)
    } else if (ledNumber == 6) {
      acenderEApagar(leds3, NUM_LEDS_3 / 2, NUM_LEDS_3, CRGB::Green, delayTime); // Ride (metade 2 da fita 3)
    } else if (ledNumber == 99) {
      efeitoFinal(CRGB::Red);   // Jogador perdeu
    } else if (ledNumber == 100) {
      efeitoFinal(CRGB::Green); // Jogador ganhou
    } else {
      Serial.println("Número de LED inválido.");
    }
  } else {
    Serial.println("Comando inválido.");
  }
}

void acenderEApagar(CRGB* leds, int start, int end, CRGB color, int delayTime) {
  // Acende LEDs no intervalo especificado
  for (int i = start; i < end; i++) {
    leds[i] = color;
  }
  FastLED.show();

  delay(delayTime);

  // Apaga LEDs no intervalo especificado
  for (int i = start; i < end; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

// Função para efeito de vitória ou derrota
void efeitoFinal(CRGB color) {
  for (int i = 0; i < 5; i++) { // Pisca 5 vezes
    for (int j = 0; j < NUM_LEDS_1; j++) leds1[j] = color;
    for (int j = 0; j < NUM_LEDS_2; j++) leds2[j] = color;
    for (int j = 0; j < NUM_LEDS_3; j++) leds3[j] = color;
    FastLED.show();
    delay(500);
    for (int j = 0; j < NUM_LEDS_1; j++) leds1[j] = CRGB::Black;
    for (int j = 0; j < NUM_LEDS_2; j++) leds2[j] = CRGB::Black;
    for (int j = 0; j < NUM_LEDS_3; j++) leds3[j] = CRGB::Black;
    FastLED.show();
    delay(500);
  }
}
