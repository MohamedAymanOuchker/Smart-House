#include <Servo.h>
#include <DHT.h>

// Broche de données du capteur DHT11
#define DHTPIN 11
// Type de capteur DHT (DHT11 ici)
#define DHTTYPE DHT11

// Broche pour le capteur PIR
#define PIR 6
// Broche pour la LED
#define LED 7

// Broches pour les LEDs
#define LED1 2
#define LED2 3
#define LED3 4

// Broche pour le buzzer
#define p_buz 10
// Broche pour le capteur de flamme
#define p_sensor A0
// Broche pour le bouton p_button
#define p_button 5
// Broche pour le bouton p_button2
#define p_button2 12

// Position initiale du servo-moteur
int positionInitiale = 90;
// Position tournée du servo-moteur
int positionTournee = 0;
// État du bouton p_button2
int etatBouton = LOW;
// Dernier état du bouton p_button2
int dernierEtatBouton = LOW;
// Indicateur de rotation du servo-moteur
bool estTourne = false;

DHT dht(DHTPIN, DHTTYPE);
Servo servoMotor1;
Servo servoMotor2;

// État du bouton p_button
int buttonState = 0;
// Indicateur d'appui sur le bouton p_button
bool isButtonPressed = false;
// Indicateur d'état des LEDs
bool areLedsOn = false;

void setup() {
  Serial.begin(9600);  // Initialise le moniteur série

  dht.begin();
  servoMotor1.attach(8);   // Attache le servo-moteur à la broche 8 (F)
  servoMotor2.attach(9);   // Attache le servo-moteur à la broche 9 (P)

  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);

  pinMode(p_buz, OUTPUT);
  pinMode(p_button, INPUT_PULLUP);
  pinMode(p_button2, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}

void loop() {
  // Lecture de la température avec DHT11
  float temperature = dht.readTemperature();  // Lit la température en Celsius

  if (!isnan(temperature)) {
    // Vérifie si la lecture est valide
    Serial.print("Température : ");
    Serial.print(temperature);
    Serial.println(" °C");

    // Rotation du servo-moteur 1 en fonction de la température
    if (temperature > 26) {
      servoMotor1.write(90);  // Effectue une rotation à 90 degrés          
    } else {
      servoMotor1.write(0);   // Remet le servo-moteur à la position initiale
    }
  }

  // Détection de mouvement avec capteur PIR
  int pirVal = digitalRead(PIR);

  if (pirVal == HIGH) {
    digitalWrite(LED, HIGH);
    Serial.println("Mouvement détecté"); // Affiche un message dans le moniteur série
    delay(1000);
  } else {
    digitalWrite(LED, LOW);
    Serial.println("Aucun mouvement détecté"); // Affiche un message dans le moniteur série
    delay(1000);
  }

  // Contrôle du buzzer en fonction de la température et du capteur de flamme
  int valeurFlamme = analogRead(p_sensor);

  Serial.print("Valeur de flamme : ");
  Serial.println(valeurFlamme);

  if (valeurFlamme < 500) {
    // Flamme détectée
    Serial.println("Flamme détectée");
    tone(p_buz, 5, 10000);
    servoMotor2.write(0);  // Effectue une rotation à 90 degrés
  } else {
    // Aucune flamme détectée
    Serial.println("Flamme non détectée");
    delay(1000);           // Attend 1 seconde
    servoMotor2.write(90);   // Remet le servo-moteur à la position initiale
    noTone(p_buz);
  }

  // Lecture de l'état du bouton p_button2
  etatBouton = digitalRead(p_button2);

  if (etatBouton != dernierEtatBouton) {
    // Le bouton p_button2 a été appuyé ou relâché
    if (etatBouton == HIGH) {
        servoMotor2.write(positionTournee);
        delay(5000);
    } else {
      servoMotor2.write(positionInitiale);
    }
    delay(50);
  }

  dernierEtatBouton = etatBouton;

  // Lecture de l'état du bouton p_button
  buttonState = digitalRead(p_button);

  if (buttonState == LOW) {
    // Bouton appuyé
    if (!isButtonPressed) {
      isButtonPressed = true;
      if (!areLedsOn) {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
        areLedsOn = true;
      } else {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        areLedsOn = false;
      }
      delay(100);  // Délai pour éviter les rebonds du bouton
    }
  } else {
    isButtonPressed = false;
  }
}
