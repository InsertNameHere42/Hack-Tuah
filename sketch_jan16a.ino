#include <Arduino.h>

class Button {
public:
  int pin; 

  Button(int pin) : pin(pin) {
    pinMode(pin, INPUT);
  }

  void update() {
    pressed = false;
    int state = digitalRead(pin);
    Serial.println(digitalRead(pin));
    
    if (state == prevState) return;
    
    if (state == LOW)
      pressed = true;

    prevState = state;
  }

  bool isPressed() {
    return pressed;
  }
private:      
  int prevState;
  bool pressed;
};

Button buttons[] = { Button(2) };

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (Button b : buttons) {
    b.update();

    if (b.isPressed()) {
      Serial.print("Button on pin ");
      Serial.print(b.pin);
      Serial.println(" is PRESSED");
    }
  }

  delay(100);
}