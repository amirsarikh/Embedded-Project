#include <LiquidCrystal.h>
#include <Servo.h>

const int triggerPin = 6;      // Pin del trigger del sensore ad ultrasuoni
const int echoPin = 5;         // Pin dell'echo del sensore ad ultrasuoni
const int buttonPin = 7;       // Pin del bottone
const int redLedPin = 3;      // Pin del LED rosso
const int greenLedPin = 4;    // Pin del LED verde
const int servoPin = 2;        // Pin del servo motore

// Configurazione dello schermo LCD
LiquidCrystal lcd(13, 10, 9, 8, 11, 12);
Servo myServo;
unsigned long servoDelay = 4000;    // Tempo di blocco del servo (40 secondi)
unsigned long timer = 0;            // Variabile per il timer di 10 secondi
boolean servoEnabled = true;        // Flag per abilitare o disabilitare il movimento del servo

void setup() {
  lcd.begin(16, 2);                    // Imposta le dimensioni dello schermo (colonne x righe)
  lcd.print("Tempo:");           // Scrive il testo fisso nella prima riga dello schermo

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(0);                     // Posizione iniziale del servo motore (0 gradi)

  digitalWrite(greenLedPin, HIGH);      // Accendi il LED verde all'avvio
}

void loop() {
  // Verifica se il bottone è stato premuto
  if (digitalRead(buttonPin) == LOW) {
    moveServo();
  }

  // Verifica se il sensore ad ultrasuoni rileva una distanza inferiore a 10 centimetri
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  if (distance < 10) {
    moveServo();
  }

  // Timer di 10 secondi
  if (millis() - timer >= 10000) {
    moveServo();
    timer = millis();
  }

  // Verifica il blocco del servo
  if (!servoEnabled && millis() - timer >= servoDelay) {
    digitalWrite(greenLedPin, HIGH);    // Accendi il LED verde dopo i 40 secondi
    digitalWrite(redLedPin, LOW);
    servoEnabled = true;                // Riabilita il movimento del servo
  }

  // Mostra il tempo rimanente o la scritta "Pronto" sulla seconda riga dello schermo
  lcd.setCursor(0, 1);                // Posiziona il cursore alla riga 2, colonna 1
  if (servoEnabled) {
    lcd.print("Pronto        ");         // Scrivi "Pronto" sulla seconda riga dello schermo
  } else {
     unsigned long remainingTime = (servoDelay - (millis() - timer)) / 1000;    // Calcola il tempo rimanente in secondi
    lcd.print(remainingTime);           // Scrivi il tempo rimanente sullo schermo
    lcd.print("s ");                     // Aggiungi "s" per indicare secondi
    
  }
}

void moveServo() {
  if (servoEnabled) {
    myServo.write(90);                    // Muovi il servo a 0 gradi
    delay(100);                           // Attendi 1 secondo
    myServo.write(0);                     // Riporta il servo alla posizione iniziale (90 gradi)
    digitalWrite(redLedPin, HIGH);        // Accendi il LED rosso
    digitalWrite(greenLedPin, LOW);
    servoEnabled = false;                 // Disabilita il movimento del servo per 40 secondi
    timer = millis();                     // Avvia il timer
    lcd.clear();                          // Pulisci lo schermo LCD
    lcd.setCursor(0, 1);                  // Posiziona il cursore alla riga 2, colonna 1
    lcd.print(servoDelay / 1000);         // Scrivi il tempo rimanente iniziale sullo schermo
    lcd.print("s ");                       // Aggiungi "s" per indicare secondi
  }
}


