/*
 * SOUNDLITE - Sound Activated Lighting System
 * 
 * Project: Computer Organization and Architecture (21CSS201T)
 * SRM Institute of Science and Technology
 * 
 * Authors:
 * - Aryaan Mahapatra (RA2311003010378)
 * - Subhasmita Sahu (RA2311003010387)
 * - Smrutirekha Parhi (RA2311003010396)
 * 
 * Guide: Dr. Sudestna Nahak
 * Date: November 2024
 * 
 * Description:
 * This system detects sound signals (claps, music beats, ambient noise)
 * and responds by activating RGB LEDs that change colors dynamically.
 */

// Pin assignments
#define RED_PIN 7
#define GREEN_PIN 8
#define BLUE_PIN 9
#define SOUND_SENSOR_PIN 3
#define LED_INDICATOR 9

// Debounce and timing
static unsigned long last_run = 0;
const unsigned long DEBOUNCE_DELAY = 300; // ms

// LED control
int current_led = RED_PIN;
int led_state = 0;

// Sound sensitivity threshold
const int SOUND_THRESHOLD = 100;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Set RGB LED pins as output
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  // Set sound sensor pin as input
  pinMode(SOUND_SENSOR_PIN, INPUT);
  
  // Attach interrupt to detect sound
  attachInterrupt(digitalPinToInterrupt(SOUND_SENSOR_PIN), color_swap, RISING);
  
  // Initial state - turn on red LED
  digitalWrite(current_led, HIGH);
  Serial.println("SOUNDLITE Initialized");
  Serial.println("System ready to detect sound...");
}

void loop() {
  // Keep LEDs off initially
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  
  // Turn on the current LED
  digitalWrite(current_led, HIGH);
  
  // Optional: Add delay for visual feedback
  delay(100);
}

/*
 * Interrupt Service Routine (ISR) for sound detection
 * Triggered on RISING edge of sound sensor output
 * Cycles through RGB colors: RED -> GREEN -> BLUE -> RED...
 */
void color_swap() {
  // Debounce to prevent multiple triggers from single sound
  if (millis() - last_run > DEBOUNCE_DELAY) {
    // Turn off current LED
    digitalWrite(current_led, LOW);
    
    // Cycle to next LED color
    current_led++;
    
    // Reset to red if we've gone past blue
    if (current_led == 10) {
      current_led = RED_PIN;
    }
    
    // Turn on new LED
    digitalWrite(current_led, HIGH);
    
    // Update last run time
    last_run = millis();
    
    // Debug output
    Serial.print("Sound Detected! LED: ");
    switch(current_led) {
      case RED_PIN:
        Serial.println("RED");
        break;
      case GREEN_PIN:
        Serial.println("GREEN");
        break;
      case BLUE_PIN:
        Serial.println("BLUE");
        break;
    }
  }
}
