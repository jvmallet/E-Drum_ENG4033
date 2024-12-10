#include <FastLED.h>

#define NUM_LEDS 13
#define LED_PIN_1 7
#define LED_PIN_2 6
#define LED_PIN_3 4

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];

// Configuração dos sensores
int sensorPins[] = {A0, A1, A2, A3};
int limiarSensor = 80;
unsigned long tempoInicio;
unsigned long tempoLimite = 2000; // 2 segundos para acertar

// Estados do jogo
int ledAtual = -1; // -1 significa nenhum LED aceso
bool aguardandoResposta = false;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds1, NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds2, NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN_3, GRB>(leds3, NUM_LEDS);
  FastLED.setBrightness(50);

  for (int i = 0; i < 4; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  Serial.begin(9600);
  Serial.println("Arduino iniciado e aguardando comandos...");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }

  if (aguardandoResposta) {
    verificaResposta();
  }
}

void processCommand(String command) {
  command.trim();

  if (command.startsWith("led")) {
    int led = command.substring(4).toInt();
    acenderLED(led);
    tempoInicio = millis();
    aguardandoResposta = true;
    ledAtual = led;
    Serial.println("LED aceso. Aguardando resposta...");
  }
}

void acenderLED(int led) {
  limparLEDs();

  if (led == 1) {
    acenderIntervalo(leds1, 0, NUM_LEDS / 2, CRGB::Yellow); // Hi Hat
  } else if (led == 2) {
    acenderIntervalo(leds1, NUM_LEDS / 2, NUM_LEDS, CRGB::Blue); // Snare
  } else if (led == 3) {
    acenderIntervalo(leds2, 0, NUM_LEDS / 2, CRGB::Red); // Bass Drum
  } else if (led == 4) {
    acenderIntervalo(leds2, NUM_LEDS / 2, NUM_LEDS, CRGB::Orange); // Crash
  } else if (led == 5) {
    acenderIntervalo(leds3, 0, NUM_LEDS / 2, CRGB::Purple); // Tom
  } else if (led == 6) {
    acenderIntervalo(leds3, NUM_LEDS / 2, NUM_LEDS, CRGB::Green); // Ride
  }

  FastLED.show();
}

void verificaResposta() {
  int sensorValue = analogRead(sensorPins[ledAtual - 1]);
  unsigned long agora = millis();

  if (sensorValue > limiarSensor) {
    feedbackAcerto();
    aguardandoResposta = false;
    Serial.println("ACERTO");
  } else if (agora - tempoInicio > tempoLimite) {
    feedbackErro();
    aguardandoResposta = false;
    Serial.println("ERRO");
  }
}

void feedbackAcerto() {
  if (ledAtual <= 2) {
    acenderIntervalo(leds1, 0, NUM_LEDS / 2, CRGB::Green);
  } else if (ledAtual <= 4) {
    acenderIntervalo(leds2, NUM_LEDS / 2, NUM_LEDS, CRGB::Green);
  } else {
    acenderIntervalo(leds3, NUM_LEDS / 2, NUM_LEDS, CRGB::Green);
  }
  FastLED.show();
  delay(500);
  limparLEDs();
}

void feedbackErro() {
  if (ledAtual <= 2) {
    acenderIntervalo(leds1, 0, NUM_LEDS / 2, CRGB::Red);
  } else if (ledAtual <= 4) {
    acenderIntervalo(leds2, NUM_LEDS / 2, NUM_LEDS, CRGB::Red);
  } else {
    acenderIntervalo(leds3, NUM_LEDS / 2, NUM_LEDS, CRGB::Red);
  }
  FastLED.show();
  delay(500);
  limparLEDs();
}

void limparLEDs() {
  fill_solid(leds1, NUM_LEDS, CRGB::Black);
  fill_solid(leds2, NUM_LEDS, CRGB::Black);
  fill_solid(leds3, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void acenderIntervalo(CRGB* leds, int start, int end, CRGB color) {
  for (int i = start; i < end; i++) {
    leds[i] = color;
  }
}
