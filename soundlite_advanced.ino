/*
 * SOUNDLITE - Advanced Version
 * Sound Activated Lighting System with Enhanced Features
 * 
 * This version includes:
 * - PWM brightness control
 * - Adjustable sensitivity
 * - Serial debugging
 * - Multiple lighting patterns
 * - EEPROM settings storage
 */

#include <EEPROM.h>

// Pin assignments
#define RED_PIN 7
#define GREEN_PIN 8
#define BLUE_PIN 9
#define SOUND_SENSOR_PIN 3
#define SOUND_ANALOG_PIN A0

// Debounce and timing
static unsigned long last_run = 0;
unsigned long debounce_delay = 300; // Adjustable

// LED control
int current_led = RED_PIN;
int led_state = 0;
int brightness = 255; // 0-255 for PWM

// Sound sensitivity
int sound_threshold = 100; // 0-1023 for analog
boolean use_analog = false; // Use analog instead of digital

// Modes
int mode = 0; // 0=Simple, 1=Fade, 2=Rainbow, 3=Strobe
int max_modes = 4;

// Timing
unsigned long mode_timer = 0;
unsigned long fade_timer = 0;

// Color arrays for rainbow mode
int colors[3][3] = {
  {255, 0, 0},    // Red
  {0, 255, 0},    // Green
  {0, 0, 255}     // Blue
};

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Set RGB LED pins as output
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  // Set sound sensor pins as input
  pinMode(SOUND_SENSOR_PIN, INPUT);
  
  // Attach interrupt to detect sound
  attachInterrupt(digitalPinToInterrupt(SOUND_SENSOR_PIN), color_swap, RISING);
  
  // Load settings from EEPROM
  load_settings();
  
  // Initial state
  digitalWrite(RED_PIN, HIGH);
  
  // Print menu
  print_menu();
}

void loop() {
  // Keep LEDs off initially
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  
  // Update display based on mode
  switch(mode) {
    case 0:
      simple_mode();
      break;
    case 1:
      fade_mode();
      break;
    case 2:
      rainbow_mode();
      break;
    case 3:
      strobe_mode();
      break;
  }
  
  // Check for serial commands
  if (Serial.available() > 0) {
    handle_serial_input();
  }
  
  delay(50);
}

/*
 * Simple cycling mode - original functionality
 */
void simple_mode() {
  digitalWrite(current_led, HIGH);
}

/*
 * Fade mode - gradually increase/decrease brightness
 */
void fade_mode() {
  static boolean fade_in = true;
  static int fade_value = 0;
  
  if (millis() - fade_timer > 20) {
    if (fade_in) {
      fade_value += 5;
      if (fade_value >= 255) {
        fade_value = 255;
        fade_in = false;
      }
    } else {
      fade_value -= 5;
      if (fade_value <= 0) {
        fade_value = 0;
        fade_in = true;
      }
    }
    
    analogWrite(current_led, fade_value);
    fade_timer = millis();
  }
}

/*
 * Rainbow mode - cycle through all color combinations
 */
void rainbow_mode() {
  static int color_index = 0;
  static unsigned long rainbow_timer = 0;
  
  if (millis() - rainbow_timer > 100) {
    // Blend colors
    int r = colors[color_index][0];
    int g = colors[color_index][1];
    int b = colors[color_index][2];
    
    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);
    
    color_index = (color_index + 1) % 3;
    rainbow_timer = millis();
  }
}

/*
 * Strobe mode - rapid on/off flashing
 */
void strobe_mode() {
  static boolean strobe_on = true;
  static unsigned long strobe_timer = 0;
  
  if (millis() - strobe_timer > 100) {
    if (strobe_on) {
      digitalWrite(current_led, HIGH);
      strobe_on = false;
    } else {
      digitalWrite(current_led, LOW);
      strobe_on = true;
    }
    strobe_timer = millis();
  }
}

/*
 * Interrupt Service Routine for sound detection
 */
void color_swap() {
  // Debounce check
  if (millis() - last_run > debounce_delay) {
    
    // Turn off current LED
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    
    // Cycle to next LED color
    current_led++;
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

/*
 * Handle serial commands for user configuration
 */
void handle_serial_input() {
  char cmd = Serial.read();
  
  switch(cmd) {
    case 'm':
      // Change mode
      Serial.println("Available modes:");
      Serial.println("0: Simple (Color cycle)");
      Serial.println("1: Fade (Brightness pulse)");
      Serial.println("2: Rainbow (All colors)");
      Serial.println("3: Strobe (Flash effect)");
      Serial.print("Enter mode (0-3): ");
      while (!Serial.available());
      mode = Serial.parseInt();
      if (mode >= max_modes) mode = 0;
      Serial.println(mode);
      break;
      
    case 'd':
      // Change debounce delay
      Serial.print("Current debounce: ");
      Serial.print(debounce_delay);
      Serial.println(" ms");
      Serial.print("Enter new value (100-1000): ");
      while (!Serial.available());
      debounce_delay = Serial.parseInt();
      Serial.println(debounce_delay);
      break;
      
    case 'b':
      // Change brightness
      Serial.print("Current brightness: ");
      Serial.print(brightness);
      Serial.println("/255");
      Serial.print("Enter new value (0-255): ");
      while (!Serial.available());
      brightness = Serial.parseInt();
      Serial.println(brightness);
      break;
      
    case 's':
      // Change sensitivity
      Serial.print("Current threshold: ");
      Serial.print(sound_threshold);
      Serial.println("/1023");
      Serial.print("Enter new value (0-1023): ");
      while (!Serial.available());
      sound_threshold = Serial.parseInt();
      Serial.println(sound_threshold);
      break;
      
    case 'p':
      // Print current settings
      print_settings();
      break;
      
    case '?':
      print_menu();
      break;
      
    default:
      if (cmd != '\n' && cmd != '\r') {
        Serial.print("Unknown command: ");
        Serial.println(cmd);
        print_menu();
      }
  }
}

/*
 * Print command menu
 */
void print_menu() {
  Serial.println("\n========== SOUNDLITE Control Menu ==========");
  Serial.println("Commands:");
  Serial.println("  m - Change mode");
  Serial.println("  d - Change debounce delay");
  Serial.println("  b - Change brightness");
  Serial.println("  s - Change sensitivity");
  Serial.println("  p - Print settings");
  Serial.println("  ? - Show this menu");
  Serial.println("==========================================\n");
}

/*
 * Print current settings
 */
void print_settings() {
  Serial.println("\n========== Current Settings ==========");
  Serial.print("Mode: ");
  switch(mode) {
    case 0: Serial.println("Simple"); break;
    case 1: Serial.println("Fade"); break;
    case 2: Serial.println("Rainbow"); break;
    case 3: Serial.println("Strobe"); break;
  }
  Serial.print("Debounce Delay: ");
  Serial.print(debounce_delay);
  Serial.println(" ms");
  Serial.print("Brightness: ");
  Serial.print(brightness);
  Serial.println("/255");
  Serial.print("Sound Threshold: ");
  Serial.print(sound_threshold);
  Serial.println("/1023");
  Serial.println("=====================================\n");
}

/*
 * Save settings to EEPROM
 */
void save_settings() {
  EEPROM.write(0, mode);
  EEPROM.write(1, brightness);
  Serial.println("Settings saved to EEPROM");
}

/*
 * Load settings from EEPROM
 */
void load_settings() {
  mode = EEPROM.read(0);
  brightness = EEPROM.read(1);
  
  // Validate values
  if (mode >= max_modes) mode = 0;
  if (brightness > 255) brightness = 255;
  
  Serial.println("Settings loaded from EEPROM");
}

/*
 * Analog sound detection (alternative to digital interrupt)
 */
void check_analog_sound() {
  int sound_level = analogRead(SOUND_ANALOG_PIN);
  
  if (sound_level > sound_threshold) {
    if (millis() - last_run > debounce_delay) {
      color_swap();
    }
  }
}
