/*
 * Sm@rt Lamp v.: 1.1.0.0 b
 * Автоматическое управление уличным освещением
 * Реле срабатыват при получении сигнала от датчика движения при недостаточном уровне освещения; уровень освещенности берется с фоторезистора
 * используется Low Level Trigger relay: HIGH - выключить, LOW - включить
 * Sm@rtDevice 14.11.2020
*/

#define DI_PIN_MOVE_DETECTOR_1 2             // пин, к которому подключен датчик движения 1 D2
#define DI_PIN_MOVE_DETECTOR_2 3             // пин, к которому подключен датчик движения 2 D3 (опционально)
#define AI_PIN_PHOTORESISTOR 7               // пин, к которому подключен фоторезистор A7
#define DO_PIN_RELAY 13                      // пин, выход на реле D13

#define TRASHOLD_LIGHT 100                   // минималный порог освещенности с фоторезистора для срабатывания системы 0..1024 (чем меньше значение, тем в более темных условиях срабатывает реле), def: 250
#define TIME_OUT_LIGHT_OFF 60000             // таймаут отключения освещения после пропадания сигнала с датчика движения (мс.), def: 60000

//#define DEBUG_MODE

unsigned long light_timer;
boolean light_on = false;

void setup() {

  digitalWrite(DO_PIN_RELAY, HIGH);          // по умолчанию выключен
  light_timer = millis();                    // max: 4,294,967,295

  pinMode(DI_PIN_MOVE_DETECTOR_1, INPUT);
  pinMode(DI_PIN_MOVE_DETECTOR_2, INPUT);

#ifdef DEBUG_MODE
  Serial.begin(9600);
  Serial.println("Start program");
#endif

}

void loop() {

  if( (analogRead(AI_PIN_PHOTORESISTOR) < TRASHOLD_LIGHT || light_on)
   && (digitalRead(DI_PIN_MOVE_DETECTOR_1) == HIGH || digitalRead(DI_PIN_MOVE_DETECTOR_2) == HIGH) ) {

#ifdef DEBUG_MODE
       Serial.println("Light ON");
#endif

       light_on = true;
       light_timer = millis();
       digitalWrite(DO_PIN_RELAY, LOW);
       delay(1000);
  }

  if(light_timer > millis()) {
    light_timer = millis();
  }

  if (light_on == true && (millis() - light_timer > TIME_OUT_LIGHT_OFF)) {

#ifdef DEBUG_MODE
       Serial.println("Light OFF");
#endif

    light_on = false;
    light_timer = millis();
    digitalWrite(DO_PIN_RELAY, HIGH);
    delay(9000);                             // после выключения освещения срабатывают датчики движения, ждем 9 с пока отключатся
  }

#ifdef DEBUG_MODE
  Serial.print("Move detector 1: "); Serial.println(digitalRead(DI_PIN_MOVE_DETECTOR_1));
  Serial.print("Move detector 2: "); Serial.println(digitalRead(DI_PIN_MOVE_DETECTOR_2));
  Serial.print("Photo resistor value: "); Serial.println(analogRead(AI_PIN_PHOTORESISTOR));

  if (digitalRead(DI_PIN_MOVE_DETECTOR_1) == HIGH) {
   Serial.println("Move 1 detected");
  }

  if (digitalRead(DI_PIN_MOVE_DETECTOR_2) == HIGH) {
   Serial.println("Move 2 detected");
  }
 
  delay(1000);
#endif

}
