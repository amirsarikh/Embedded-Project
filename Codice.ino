#include <LiquidCrystal.h>
#include <Servo.h>

const int triggerPin = 6;      // Pin del trigger del sensore ad ultrasuoni
const int echoPin = 5;         // Pin dell'echo del sensore ad ultrasuoni
const int buttonPin = 7;       // Pin del bottone
const int redLedPin = 4;       // Pin del LED rosso
const int greenLedPin = 3;     // Pin del LED verde
const int servoPin = 2;        // Pin del servo motore

// Configurazione dello schermo LCD
LiquidCrystal lcd(13, 10, 9, 8, 11, 12);
Servo myServo;
unsigned long servoDelay = 4000;    // Tempo di blocco del servo (4 secondi)
unsigned long timer = 0;            // Variabile per il timer
boolean servoEnabled = true;        // Flag per abilitare o disabilitare il movimento del servo

void setup() {
  
  lcd.begin(16, 2);                  // Imposta le dimensioni dello schermo (colonne x righe)
  lcd.print("Tempo:");               // Scrive il testo fisso nella prima riga dello schermo

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(0);                  // Posizione iniziale del servo motore (0 gradi)

  digitalWrite(greenLedPin, HIGH);   // Accendi il LED verde all'avvio
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    moveServo();
  }

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

  if (millis() - timer >= 10000) {
    moveServo();
    timer = millis();
  }

  if (!servoEnabled && millis() - timer >= servoDelay) {
    digitalWrite(greenLedPin, HIGH);    // Accendi il LED verde dopo i 4 secondi
    digitalWrite(redLedPin, LOW);
    servoEnabled = true;                // Riabilita il movimento del servo
  }

  lcd.setCursor(7, 0);                  // Posiziona il cursore nella seconda colonna della prima riga
  if (servoEnabled) {
    
    lcd.print("Pronto  ");                 // Scrivi "Pronto" sulla seconda riga dello schermo
  } else {
    unsigned long remainingTime = (servoDelay - (millis() - timer)) / 1000;
    unsigned long hours = remainingTime / 3600;
    unsigned long minutes = (remainingTime % 3600) / 60;
    unsigned long seconds = remainingTime % 60;

    lcd.print(hours < 10 ? "0" : "");   // Aggiungi lo zero iniziale se le ore sono inferiori a 10
    lcd.print(hours);
    lcd.print(":");
    lcd.print(minutes < 10 ? "0" : ""); // Aggiungi lo zero iniziale se i minuti sono inferiori a 10
    lcd.print(minutes);
    lcd.print(":");
    lcd.print(seconds < 10 ? "0" : ""); // Aggiungi lo zero iniziale se i secondi sono inferiori a 10
    lcd.print(seconds);
   	
  }
}

void moveServo() {
  if (servoEnabled) {
    myServo.write(90);                  // Muovi il servo a 0 gradi
    delay(100);                         // Attendi 0.1 secondo
    myServo.write(0);                   // Riporta il servo alla posizione iniziale (90 gradi)
    digitalWrite(redLedPin, HIGH);      // Accendi il LED rosso
    digitalWrite(greenLedPin, LOW);
    servoEnabled = false;               // Disabilita il movimento del servo per 4 secondi
    timer = millis();                   // Avvia il timer
   /* lcd.clear();                        // Pulisci lo schermo LCD
    lcd.setCursor(0, 1);                // Posiziona il cursore nella prima colonna della seconda riga
    lcd.print(servoDelay / 1000);       // Scrivi il tempo rimanente iniziale sullo schermo
    lcd.print("s ");                    // Aggiungi "s" per indicare secondi*/
  }
}
