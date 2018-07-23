
#define BUTTON_0 8
#define BUTTON_1 10
#define BUTTON_2 12

// delay between polls
#define DELAY 1000

// blink length
#define BLINK_T 50000

const int BUTTONS[] = {BUTTON_0, BUTTON_1, BUTTON_2};
const int BUTTON_COUNT = 3;

bool BUTTON_STATES[BUTTON_COUNT];

long blink_cooldown = 0;

void setup() {
  // Set INPUT mode for all buttons
  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    pinMode(BUTTONS[i], INPUT);
    BUTTON_STATES[i] = false;
  }

  // Set the LED pin to output
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Set up serial port
  Serial.begin(9600);
}

void start_blink()
{
  blink_cooldown = BLINK_T;
}

// Skip forward in time
void wait()
{
  delayMicroseconds(DELAY);
  
  // LED blink handling
  blink_cooldown = blink_cooldown <= 0 ? 0 : blink_cooldown - DELAY;
  digitalWrite(LED_BUILTIN, blink_cooldown != 0);
}


// Check the state of every button and send corresponding serial messages
void check_buttons()
{
  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    bool prev_state = BUTTON_STATES[i];
    bool cur_state = digitalRead(BUTTONS[i]);

    if (prev_state != cur_state)
    {
      // if button state changes to HIGH (aka keyDown)
      // send the button number via serial and set up the blink
      if (cur_state == HIGH)
      {
        Serial.write('0'+i);
        start_blink();
      }
      BUTTON_STATES[i] = cur_state;
    }
  }
}

void loop() {
  check_buttons();
  wait();
}


