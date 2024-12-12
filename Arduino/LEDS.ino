#include <FastLED.h>


#define NUM_LEDS_1 13  // Número de LEDs na primeira fita
#define NUM_LEDS_2 13  // Número de LEDs na segunda fita
#define LED_PIN_1 3    // Pino para a primeira fita
#define LED_PIN_2 5    // Pino para a segunda fita

CRGB leds1[NUM_LEDS_1];  // Array para a primeira fita
CRGB leds2[NUM_LEDS_2];  // Array para a segunda fita

String input = "";  // Variável para armazenar o comando recebido

int sensorPin1 = A0;
int sensorPin2 = A1;

unsigned long instanteAnt[2];
unsigned long off_time[2];
int limiarSensor = 80;


void setup() {
  FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds1, NUM_LEDS_1);
  FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds2, NUM_LEDS_2);
  FastLED.setBrightness(50);  // Ajusta o brilho dos LEDs

  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);

  Serial.begin(31250);  // Inicializa a comunicação serial
  Serial.println("Arduino iniciado e aguardando comandos...");
}

void loop() {
  int sensorValue1 = analogRead(sensorPin1);
  int sensorValue2 = analogRead(sensorPin2);

  unsigned long time = millis();

  // Sensor 1
  if (sensorValue1 > limiarSensor) {
    if (time - instanteAnt[0] < 300) {
      acenderLedsComTemporizador(leds1, NUM_LEDS_1, CRGB::Green, 200, 0);
    } else {
      acenderLedsComTemporizador(leds1, NUM_LEDS_1, CRGB::Red, 200, 0);
    }
    sensorValue1 = mapear(sensorValue1);
    Serial.print("nota 22 ");
    Serial.println(sensorValue1);
  }

  // Sensor 2
  if (sensorValue2 > limiarSensor) {
    if (time - instanteAnt[1] < 300) {
      acenderLedsComTemporizador(leds2, NUM_LEDS_2, CRGB::Green, 200, 1);

    } else {
      acenderLedsComTemporizador(leds2, NUM_LEDS_2, CRGB::Red, 200, 1);
    }
    sensorValue2 = mapear(sensorValue2);
    Serial.print("nota 38 ");
    Serial.println(sensorValue2);
  }

  // Processar comando recebido via serial
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {  // Processa o comando ao receber uma nova linha
      processCommand(input);
      Serial.println(input);
      input = "";  // Limpa o buffer de entrada
    } else {
      input += c;
    }
  }

  if (time - off_time[0] >= 200) {
    for (int i = 0; i < NUM_LEDS_1; i++) {
      leds1[i] = CRGB::Black;
    }
    FastLED.show();
  }
  if (time - off_time[1] >= 200) {
    for (int i = 0; i < NUM_LEDS_2; i++) {
      leds2[i] = CRGB::Black;
    }
    FastLED.show();
  }
}

int mapear(int valorAnterior) {
  int valorMapeado = map(valorAnterior, 0, 500, 0, 127);
  valorMapeado = constrain(valorMapeado, 0, 127);
  return valorMapeado;
}

void processCommand(String command) {
  command.trim();
  Serial.print("Comando recebido: ");
  Serial.println(command);

  if (command.startsWith("led")) {
    int ledNumber = command.substring(4).toInt();
    if (ledNumber == 1) {
      acenderLedsComTemporizador(leds1, NUM_LEDS_1, CRGB::Red, 1000, 0);
      instanteAnt[0] = millis();
      Serial.println("LED 1 acendeu");
    } else if (ledNumber == 2) {
      acenderLedsComTemporizador(leds2, NUM_LEDS_2, CRGB::Blue, 1000, 1);
      instanteAnt[1] = millis();
      Serial.println("LED 2 acendeu");
    } else {
      Serial.println("Número de LED inválido.");
    }
  } else {
    Serial.println("Comando inválido.");
  }
}

void acenderLedsComTemporizador(CRGB* leds, int numLeds, CRGB color, unsigned long tempo, int index) {
  unsigned long time = millis();

  for (int i = 0; i < numLeds; i++) {
    leds[i] = color;
  }
  FastLED.show();

  // instanteAnt[index] = time;
  off_time[index] = time;
}
