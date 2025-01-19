class Button {
public:
  int pin; 

  Button(int pin) : pin(pin) {
    pinMode(pin, INPUT_PULLUP);
  }

  void update() {
    pressed = false;
    released = false;
    int state = digitalRead(pin);
    //Serial.println(digitalRead(pin));
    
    if (state == prevState) return;
    
    if (state == LOW)
      pressed = true;
    else
      released = true;
    prevState = state;
  }
  bool isHeld(int time){
    if(digitalRead(pin)==LOW){
      int startTime = millis();
      while(millis()<startTime+time){
        if(digitalRead(pin)==HIGH)
          return false;
      }
      return true;
    }
    else
      return false;
  }

  bool isPressed() {
    return pressed;
  }
  bool isReleased() {
    return released;
  }
private:      
  int prevState;
  bool pressed;
  bool released;
};


Button buttons[] = { Button(2) };
Button controlButton = { Button(12) };

int mode = 1; //0 means editing, 1 means reading
int numPressed;
int numDesired;

void setup() {
  pinMode(11, OUTPUT); //Green Light
  pinMode(10, OUTPUT); //Red Light
  Serial.begin(9600);
}

int startTime;
void loop() {
  
  controlButton.update();
  if(mode==1){
    Serial.println("In Mode 1");
    if(controlButton.isHeld(2000))
      mode = 0;
      startTime = millis();
    if(numPressed == numDesired)
      digitalWrite(11, HIGH);
    else
      digitalWrite(10, HIGH);
    for (Button b : buttons) {
      b.update();

      if (b.isPressed()) {
        Serial.print("Button on pin ");
        Serial.print(b.pin);
        Serial.println(" is PRESSED");
        numPressed++;
      }
      if(b.isReleased()){
        numPressed--;
      }
    }
  }
  else{
    Serial.println("in mode 2");
    if(controlButton.isHeld(2000)){
      mode = 1;
    }
    if(millis()<startTime+300) //green light blinks once when entering writing mode and when adding one to desiredAmount
      digitalWrite(11, HIGH);
    else
      digitalWrite(11, LOW);
  
    if(millis()%1000<750){ //Red light is constantly blinking to indicate it's in writing mode
      digitalWrite(10, HIGH);
      Serial.print("Something's wrong");
    }
    else
      digitalWrite(10, LOW);
    if(controlButton.isPressed()){
      numDesired++;
      startTime = millis();

    }
  }

  delay(100);
}