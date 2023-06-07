#include <LiquidCrystal.h>  // Libreria per gestire il display LCD
#include <Servo.h>  // Libreria per gestire il servo motore

const int triggerPin = 6;  // Pin del trigger del sensore ad ultrasuoni
const int echoPin = 5;  // Pin dell'echo del sensore ad ultrasuoni
const int buttonPin = 7;  // Pin del bottone
const int redLedPin = 4;  // Pin del LED rosso
const int greenLedPin = 3;  // Pin del LED verde
const int servoPin = 2;  // Pin del servo motore

LiquidCrystal lcd(13, 10, 9, 8, 11, 12);  // Oggetto per gestire il display LCD
Servo myServo;  // Oggetto per gestire il servo motore
unsigned long servoDelay = 4000;  // Tempo di blocco del servo (4 secondi)
unsigned long timer = 0;  // Variabile per il timer
boolean servoEnabled = true;  // Flag per abilitare o disabilitare il movimento del servo

void setup() {
  lcd.begin(16, 2);  // Inizializza il display LCD con 16 colonne e 2 righe
  lcd.print("Tempo:");  // Scrive il testo fisso nella prima riga dello schermo

  pinMode(triggerPin, OUTPUT);  // Imposta il pin del trigger come output
  pinMode(echoPin, INPUT);  // Imposta il pin dell'echo come input
  pinMode(buttonPin, INPUT_PULLUP);  // Imposta il pin del bottone come input con resistenza di pull-up
  pinMode(redLedPin, OUTPUT);  // Imposta il pin del LED rosso come output
  pinMode(greenLedPin, OUTPUT);  // Imposta il pin del LED verde come output

  myServo.attach(servoPin);  // Collega il servo motore al pin specificato
  myServo.write(0);  // Posizione iniziale del servo motore (0 gradi)

  digitalWrite(greenLedPin, HIGH);  // Accendi il LED verde all'avvio
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    moveServo();  // Muove il servo se il pulsante è premuto
  }

  digitalWrite(triggerPin, LOW);  // Imposta il trigger a basso
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);  // Imposta il trigger ad alto per 10 microsecondi
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);  // Imposta il trigger a basso

  unsigned long duration = pulseIn(echoPin, HIGH);  // Misura la durata del segnale di echo
  float distance = duration * 0.034 / 2;  // Calcola la distanza in base alla durata

  if (distance < 10) {
    moveServo();  // Muove il servo se la distanza è inferiore a 10 cm
  }

  if (millis() - timer >= 10000) {
    moveServo();  // Muove il servo ogni 10 secondi
    timer = millis();
  }

  if (!servoEnabled && millis() - timer >= servoDelay) {
    digitalWrite(greenLedPin, HIGH);  // Accendi il LED verde dopo 4 secondi
    digitalWrite(redLedPin, LOW);  // Spegni il LED rosso
    servoEnabled = true;  // Abilita il movimento del servo
  }

  lcd.setCursor(7, 0);  // Posiziona il cursore nella seconda colonna della prima riga del display LCD
  if (servoEnabled) {
    lcd.print("Pronto   ");  // Scrive "Pronto" sulla seconda riga del display se il servo è abilitato
  } else {
    unsigned long remainingTime = (servoDelay - (millis() - timer)) / 1000;  // Calcola il tempo rimanente in secondi
    unsigned long hours = remainingTime / 3600;  // Calcola le ore
    unsigned long minutes = (remainingTime % 3600) / 60;  // Calcola i minuti
    unsigned long seconds = remainingTime % 60;  // Calcola i secondi

    lcd.print(hours < 10 ? "0" : "");  // Aggiunge lo zero iniziale se le ore sono inferiori a 10
    lcd.print(hours);
    lcd.print(":");
    lcd.print(minutes < 10 ? "0" : "");  // Aggiunge lo zero iniziale se i minuti sono inferiori a 10
    lcd.print(minutes);
    lcd.print(":");
    lcd.print(seconds < 10 ? "0" : "");  // Aggiunge lo zero iniziale se i secondi sono inferiori a 10
    lcd.print(seconds);
  }
}

void moveServo() {
  if (servoEnabled) {
    myServo.write(90);  // Muove il servo a 90 gradi
    delay(100);  // Attende 0.1 secondi
    myServo.write(0);  // Riporta il servo alla posizione iniziale (0 gradi)
    digitalWrite(redLedPin, HIGH);  // Accendi il LED rosso
    digitalWrite(greenLedPin, LOW);  // Spegni il LED verde
    servoEnabled = false;  // Disabilita il movimento del servo per 4 secondi
    timer = millis();  // Avvia il timer
  }
}

