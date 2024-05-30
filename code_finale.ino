// Définition des broches
const int buzzerPin = 5;            // GPIO 5 pour le buzzer
const int photoresistorPin = 34;    // Broche analogique 34 pour la photorésistance
const int ledPin = 18;              // Broche 18 pour la LED
const int trigPin = 19;             // Broche Trig du capteur à ultrasons
const int echoPin = 21;             // Broche Echo du capteur à ultrasons

// Variables pour le suivi de la lumière
int previousLightState = 0;

// Variables pour le suivi de la distance
float previousDistance = 0.0;
float maxDifference = 500.0;
float holeThreshold = 500.0;
int lowerCount = 0;
int upperCount = 0;

void setup() {
  // Configuration des broches
  pinMode(buzzerPin, OUTPUT);
  pinMode(photoresistorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   // Configuration de la broche du vibrateur comme sortie

  // Initialisation de la communication série
  Serial.begin(9600);
}

void loop() {
  // Logique pour la photorésistance et la LED
  int lightLevel = analogRead(photoresistorPin);
  int currentLightState = lightLevel < 1000 ? 1 : 0;

  if (currentLightState == 1 && previousLightState == 0) {
    digitalWrite(ledPin, HIGH); // Allumer la LED
    playFrereJacques();         // Jouer la mélodie
    digitalWrite(ledPin, LOW);  // Éteindre la LED
    previousLightState = 1;
  } else if (currentLightState == 0 && previousLightState == 1) {
    digitalWrite(ledPin, HIGH); // Allumer la LED
    playFrereJacques();         // Jouer la mélodie
    previousLightState = 0;
  }

  // Logique pour le capteur à ultrasons
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float distance = pulseIn(echoPin, HIGH) * 0.034 / 2;

  if (abs(distance - previousDistance) <= maxDifference) {
    Serial.println("Stable");
  }

  if (distance < previousDistance) {
    lowerCount++;
    upperCount = 0;
  } else if (distance > previousDistance) {
    upperCount++;
    lowerCount = 0;
  }

  bool obstacleDetected = false;

  if (lowerCount >= 3) {
    Serial.println("Escalier à descendre !");
    tone(buzzerPin, 523, 500);
    obstacleDetected = true;
    lowerCount = 0;
  }

  if (upperCount >= 3) {
    Serial.println("Escalier à monter !");
    tone(buzzerPin, 349, 200);
    obstacleDetected = true;
    upperCount = 0;
  }

  if (distance - previousDistance >= holeThreshold) {
    Serial.println("Trou détecté !");
    tone(buzzerPin, 440, 800);
    obstacleDetected = true;
  }


  previousDistance = distance;

  // Attendre avant la prochaine mesure
  delay(2000);
}

void playFrereJacques() {
  // Notes de "Frère Jacques"
  int notes[] = {262, 262, 294, 330, 330, 294, 262, 294, 330, 330, 294, 262, 294, 349, 392, 392, 349, 294, 262, 262, 196, 262, 262, 196};
  // Durées de chaque note (en millisecondes)
  int noteDurations[] = {4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2};

  for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++) {
    int noteDuration = 1000 / noteDurations[i];
    tone(buzzerPin, notes[i], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}



