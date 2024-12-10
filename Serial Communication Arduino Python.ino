int sensorPin1 = A0;
int sensorPin2 = A1;
int sensorPin3 = A2;
int sensorPin4 = A3;


unsigned long instanteAnt[4];


int limiarSensor = 80;


int midiNote1 = 60;  // Nota C4 (para o sensor 1)
int midiNote2 = 62;  // Nota D4 (para o sensor 2)
int midiNote3 = 64;  // Nota E4 (para o sensor 3)
int midiNote4 = 65;  // Nota F4 (para o sensor 4)

int midiVelocity = 127; // Máxima intensidade (volume)


void setup() {
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(sensorPin4, INPUT);

  Serial.begin(31250);  // Taxa de bauds para comunicação MIDI
}

// Função para enviar uma mensagem MIDI de "note on"
void sendNoteOn(int note, int velocity) {
  Serial.write(0x90); // "Note On" no canal 1 (0x90)
  Serial.write(note); // Nota MIDI (0-127)
  Serial.write(velocity); // Velocidade (0-127)
}

// Função para enviar uma mensagem MIDI de "note off"
void sendNoteOff(int note) {
  Serial.write(0x80); // "Note Off" no canal 1 (0x80)
  Serial.write(note); // Nota MIDI (0-127)
  Serial.write(0); // Velocidade 0 para "note off"
}

void loop() {
  int sensorValue1 = analogRead(sensorPin1);
  int sensorValue2 = analogRead(sensorPin2);
  int sensorValue3 = analogRead(sensorPin3);
  int sensorValue4 = analogRead(sensorPin4);
  
  unsigned long time = millis(); 
  
  // Sensor 1
  if (sensorValue1 > limiarSensor && time - instanteAnt[0] >= 500) {
    sendNoteOn(midiNote1, midiVelocity);  
    Serial.print("Sensor 1: ");
    Serial.println(sensorValue1);
    instanteAnt[0] = time;
    delay(200);  
    sendNoteOff(midiNote1);  
  }
  
  // Sensor 2
  if (sensorValue2 > limiarSensor && time - instanteAnt[1] >= 500) {
    sendNoteOn(midiNote2, midiVelocity);  
    Serial.print("Sensor 2: ");
    Serial.println(sensorValue2);
    instanteAnt[1] = time;
    delay(200);  
    sendNoteOff(midiNote2);  
  }

  // Sensor 3
  if (sensorValue3 > limiarSensor && time - instanteAnt[2] >= 500) {
    sendNoteOn(midiNote3, midiVelocity);  
    Serial.print("Sensor 3: ");
    Serial.println(sensorValue3);
    instanteAnt[2] = time;
    delay(200);  
    sendNoteOff(midiNote3);  
  }

  // Sensor 4
  if (sensorValue4 > limiarSensor && time - instanteAnt[3] >= 500) {
    sendNoteOn(midiNote4, midiVelocity);  
    Serial.print("Sensor 4: ");
    Serial.println(sensorValue4);
    instanteAnt[3] = time;
    delay(200);  
    sendNoteOff(midiNote4);  
  }
}
