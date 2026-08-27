#include <LiquidCrystal_I2C.h>

// Athena - Task 2: Keeping Watch Over Odysseus
// Name: Chiranthan Radhakrishna
// ID: 2026A7PS0163H

LiquidCrystal_I2C lcd(0x20, 16, 2);

// ---------- Pin definitions ----------

const int TRIG_PIN = 9;
const int ECHO_PIN = 10;

const int LIGHT_PIN = A0;

const int BUTTON_PIN = 2;

const int LED_PIN = 6;
const int BUZZER_PIN = 7;

unsigned long lastBlinkTime = 0;
bool ledState = LOW;
unsigned long dangerStartTime = 0;

// ---------- States ----------

enum State {
  OPEN_SEA,
  ANCHOR_DROPPED,
  STORM,
  CHARYBDIS,
  WRECKED
};

State currentState = OPEN_SEA;
bool lastButtonState = HIGH;

void displayState() {

  lcd.clear();
  lcd.setCursor(0, 0);

  switch (currentState) {

    case OPEN_SEA:
      lcd.print("OPEN SEA");
      break;

    case ANCHOR_DROPPED:
      lcd.print("ANCHOR DROPPED");
      break;

    case STORM:
      lcd.print("STORM");
      break;

    case CHARYBDIS:
      lcd.print("CHARYBDIS");
      break;

    case WRECKED:
      lcd.print("WRECKED");
      break;
  }
}

long getDistance() {

  // Send a short ultrasonic pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the echo time
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Convert time to distance in cm
  long distance = duration * 0.034 / 2;

  return distance;
}

// ---------- Setup ----------

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("OPEN SEA");
}


// ---------- Main loop ----------

void loop() {

  // ------------------------------------------------
  // WRECKED STATE
  // WRECKED is permanent until simulation is restarted
  // ------------------------------------------------
  if (currentState == WRECKED) {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
    return;
  }


  // ------------------------------------------------
  // READ SENSORS
  // ------------------------------------------------

  int lightValue = analogRead(LIGHT_PIN);
  long distance = getDistance();


  // ------------------------------------------------
  // BUTTON / ANCHOR
  // ------------------------------------------------

  bool buttonState = digitalRead(BUTTON_PIN);

  // Detect a new button press
  if (buttonState == LOW && lastButtonState == HIGH) {

    // Drop anchor
    if (currentState == OPEN_SEA ||
        currentState == STORM ||
        currentState == CHARYBDIS) {

      currentState = ANCHOR_DROPPED;

      // Reset danger timer
      dangerStartTime = 0;

      displayState();
    }

    // Raise anchor
    else if (currentState == ANCHOR_DROPPED) {

      currentState = OPEN_SEA;

      displayState();
    }
  }

  lastButtonState = buttonState;


  // ------------------------------------------------
  // ANCHOR DROPPED
  // ------------------------------------------------

  // While anchored, dangers cannot affect the ship.
  if (currentState == ANCHOR_DROPPED) {

    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);

    return;
  }


  // ------------------------------------------------
  // OPEN SEA → STORM
  // ------------------------------------------------

  if (currentState == OPEN_SEA && lightValue < 512) {

    currentState = STORM;

    // Start the 5-second danger timer
    dangerStartTime = millis();

    displayState();
  }


  // ------------------------------------------------
  // OPEN SEA → CHARYBDIS
  // ------------------------------------------------

  // This is checked after STORM.
  // Therefore, if both dangers appear during the
  // same loop, STORM gets priority because it is
  // entered first.
  if (currentState == OPEN_SEA && distance < 100) {

    currentState = CHARYBDIS;

    // Start the 5-second danger timer
    dangerStartTime = millis();

    displayState();
  }


  // ------------------------------------------------
  // STORM
  // ------------------------------------------------

  if (currentState == STORM) {

    // Blink LED every 500 ms
    if (millis() - lastBlinkTime >= 200) {

      lastBlinkTime = millis();

      ledState = !ledState;

      digitalWrite(LED_PIN, ledState);
    }

    // Storm ended before 5 seconds
    if (lightValue >= 512) {

      currentState = OPEN_SEA;

      // Reset danger timer
      dangerStartTime = 0;

      displayState();
    }

    // Storm lasted 5 seconds
    else if (millis() - dangerStartTime >= 5000) {

      currentState = WRECKED;

      displayState();
    }
  }


  // ------------------------------------------------
  // CHARYBDIS
  // ------------------------------------------------

  if (currentState == CHARYBDIS) {

    // Sound buzzer while near Charybdis
    tone(BUZZER_PIN, 1000);

    // Ship escaped Charybdis before 5 seconds
    if (distance >= 100) {

      currentState = OPEN_SEA;

      // Reset danger timer
      dangerStartTime = 0;

      noTone(BUZZER_PIN);

      displayState();
    }

    // Charybdis lasted 5 seconds
    else if (millis() - dangerStartTime >= 5000) {

      currentState = WRECKED;

      noTone(BUZZER_PIN);

      displayState();
    }
  }


  // ------------------------------------------------
  // NORMAL STATE
  // ------------------------------------------------

  if (currentState != STORM) {

    digitalWrite(LED_PIN, LOW);
    ledState = LOW;
  }

  if (currentState != CHARYBDIS) {

    noTone(BUZZER_PIN);
  }


  // ------------------------------------------------
  // SERIAL MONITOR
  // Useful for testing sensors
  // ------------------------------------------------

  Serial.print("Light: ");
  Serial.print(lightValue);

  Serial.print("   Distance: ");
  Serial.println(distance);
}
