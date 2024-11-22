#include <FastLED.h>

#define NUM_LEDS 40  // Número de LEDs por tira
#define LED_PIN1 5   // Pino para a primeira tira
#define LED_PIN2 6   // Pino para a segunda tira

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];

// Vetores de tempos para as tiras
unsigned long temposLigado1[10];   // Tempo ligado para a tira 1
unsigned long temposDesligado1[10]; // Tempo desligado para a tira 1
unsigned long temposLigado2[10];   // Tempo ligado para a tira 2
unsigned long temposDesligado2[10]; // Tempo desligado para a tira 2

int tamanho1 = 0;  // Quantidade de tempos configurados para a tira 1
int tamanho2 = 0;  // Quantidade de tempos configurados para a tira 2

int tempoIndex1 = 0;
int tempoIndex2 = 0;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
bool ledState1 = false;
bool ledState2 = false;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN1, GRB>(leds1, NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN2, GRB>(leds2, NUM_LEDS);
  FastLED.setBrightness(50);  // Define o brilho (0 a 255)
  Serial.begin(9600);         // Inicializa a comunicação serial
  Serial.println("Iniciando...");
}

void loop() {
  unsigned long currentMillis = millis();

  // Controle da primeira tira de LEDs
  if (tamanho1 > 0) {
    if (ledState1 && currentMillis - previousMillis1 >= temposLigado1[tempoIndex1]) {
      ledState1 = false;
      previousMillis1 = currentMillis;

      // Apagar LEDs da primeira tira
      for (int i = 0; i < NUM_LEDS; i++) {
        leds1[i] = CRGB::Black;
      }
      FastLED.show();
    } else if (!ledState1 && currentMillis - previousMillis1 >= temposDesligado1[tempoIndex1]) {
      ledState1 = true;
      previousMillis1 = currentMillis;

      // Acender LEDs da primeira tira
      for (int i = 0; i < NUM_LEDS; i++) {
        leds1[i] = CRGB::Red;
      }
      FastLED.show();

      tempoIndex1++;
      if (tempoIndex1 >= tamanho1) {
        tempoIndex1 = 0;
      }
    }
  }

  // Controle da segunda tira de LEDs
  if (tamanho2 > 0) {
    if (ledState2 && currentMillis - previousMillis2 >= temposLigado2[tempoIndex2]) {
      ledState2 = false;
      previousMillis2 = currentMillis;

      // Apagar LEDs da segunda tira
      for (int i = 0; i < NUM_LEDS; i++) {
        leds2[i] = CRGB::Black;
      }
      FastLED.show();
    } else if (!ledState2 && currentMillis - previousMillis2 >= temposDesligado2[tempoIndex2]) {
      ledState2 = true;
      previousMillis2 = currentMillis;

      // Acender LEDs da segunda tira
      for (int i = 0; i < NUM_LEDS; i++) {
        leds2[i] = CRGB::Blue;
      }
      FastLED.show();

      tempoIndex2++;
      if (tempoIndex2 >= tamanho2) {
        tempoIndex2 = 0;
      }
    }
  }

  // Processar entrada serial para atualizar os tempos
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input.startsWith("L1:")) {
      atualizarTempos(input);
    }
  }
}

// Função para processar e atualizar os tempos recebidos
void atualizarTempos(String input) {
  int sepL1 = input.indexOf(';');
  int sepD1 = input.indexOf(';', sepL1 + 1);
  int sepL2 = input.indexOf(';', sepD1 + 1);
  String ligado1Str = input.substring(3, sepL1);
  String desligado1Str = input.substring(sepL1 + 3, sepD1);
  String ligado2Str = input.substring(sepD1 + 3, sepL2);
  String desligado2Str = input.substring(sepL2 + 3);

  // Atualizar temposLigado1
  tamanho1 = 0;
  for (int start = 0, end = ligado1Str.indexOf(','); start < ligado1Str.length();
       start = end + 1, end = ligado1Str.indexOf(',', start)) {
    temposLigado1[tamanho1++] = ligado1Str.substring(start, end).toInt();
  }

  // Atualizar temposDesligado1
  for (int i = 0, start = 0, end = desligado1Str.indexOf(',');
       start < desligado1Str.length();
       start = end + 1, end = desligado1Str.indexOf(',', start), i++) {
    temposDesligado1[i] = desligado1Str.substring(start, end).toInt();
  }

  // Atualizar temposLigado2
  tamanho2 = 0;
  for (int start = 0, end = ligado2Str.indexOf(','); start < ligado2Str.length();
       start = end + 1, end = ligado2Str.indexOf(',', start)) {
    temposLigado2[tamanho2++] = ligado2Str.substring(start, end).toInt();
  }

  // Atualizar temposDesligado2
  for (int i = 0, start = 0, end = desligado2Str.indexOf(',');
       start < desligado2Str.length();
       start = end + 1, end = desligado2Str.indexOf(',', start), i++) {
    temposDesligado2[i] = desligado2Str.substring(start, end).toInt();
  }

  Serial.println("Vetores atualizados:");
  Serial.print("Ligado1: ");
  for (int i = 0; i < tamanho1; i++) Serial.print(temposLigado1[i]), Serial.print(" ");
  Serial.println();
  Serial.print("Desligado1: ");
  for (int i = 0; i < tamanho1; i++) Serial.print(temposDesligado1[i]), Serial.print(" ");
  Serial.println();
  Serial.print("Ligado2: ");
  for (int i = 0; i < tamanho2; i++) Serial.print(temposLigado2[i]), Serial.print(" ");
  Serial.println();
  Serial.print("Desligado2: ");
  for (int i = 0; i < tamanho2; i++) Serial.print(temposDesligado2[i]), Serial.print(" ");
  Serial.println();
}
