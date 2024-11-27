#include <FastLED.h>

#define NUM_LEDS 20  // Número de LEDs por segmento
#define LED_PIN1 5   // Pino para a primeira tira
#define LED_PIN2 # ESCOLHER UM PINO   // Pino para a segunda tira

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
      apagarLeds(leds1);
    } else if (!ledState1 && currentMillis - previousMillis1 >= temposDesligado1[tempoIndex1]) {
      ledState1 = true;
      previousMillis1 = currentMillis;

      // Acender LEDs da primeira tira
      acenderLeds(leds1, CRGB::Red);

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
      apagarLeds(leds2);
    } else if (!ledState2 && currentMillis - previousMillis2 >= temposDesligado2[tempoIndex2]) {
      ledState2 = true;
      previousMillis2 = currentMillis;

      // Acender LEDs da segunda tira
      acenderLeds(leds2, CRGB::Blue);

      tempoIndex2++;
      if (tempoIndex2 >= tamanho2) {
        tempoIndex2 = 0;
      }
    }
  }

  // Processar entrada serial para atualizar os tempos
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    processarEntrada(input);
  }
}

// Função para apagar LEDs
void apagarLeds(CRGB* leds) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

// Função para acender LEDs com cor específica
void acenderLeds(CRGB* leds, CRGB cor) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = cor;
  }
  FastLED.show();
}

// Processar string recebida
void processarEntrada(String input) {
  if (input.startsWith("L1:")) {
    atualizarTempos(input);
  }
}

// Função para atualizar os vetores de tempos
void atualizarTempos(String input) {
  // Dividir string em partes
  int sepL1 = input.indexOf(';');
  int sepD1 = input.indexOf(';', sepL1 + 1);
  int sepL2 = input.indexOf(';', sepD1 + 1);
  String ligado1Str = input.substring(3, sepL1);
  String desligado1Str = input.substring(sepL1 + 1, sepD1);
  String ligado2Str = input.substring(sepD1 + 1, sepL2);
  String desligado2Str = input.substring(sepL2 + 1);

  // Atualizar vetores
  tamanho1 = atualizarVetor(ligado1Str, temposLigado1);
  atualizarVetor(desligado1Str, temposDesligado1);
  tamanho2 = atualizarVetor(ligado2Str, temposLigado2);
  atualizarVetor(desligado2Str, temposDesligado2);

  Serial.println("Vetores atualizados.");
}

// Atualizar vetor a partir de uma string separada por vírgulas
int atualizarVetor(String temposStr, unsigned long* vetor) {
  int tamanho = 0;
  int start = 0;
  int end = temposStr.indexOf(',');

  while (end != -1) {
    vetor[tamanho++] = temposStr.substring(start, end).toInt();
    start = end + 1;
    end = temposStr.indexOf(',', start);
  }
  vetor[tamanho++] = temposStr.substring(start).toInt();

  return tamanho;
}
