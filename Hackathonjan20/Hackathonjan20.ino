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
    
    if (state == LOW){
      pressed = true;
      pressTime = millis();
    }
    else
      released = true;
    prevState = state;
  }
  bool isPressed() {
    return pressed;
  }
  bool isReleased() {
    return released;
    pressTime = -1;
  }

  int getHeldTime() {
    if (pressTime == -1) return -1;
    return millis() - pressTime;
  }
  void setHeldTime(int time){
    pressTime = 0;
  }

private:      
  int prevState;
  bool pressed;
  bool released;
  int pressTime;
  int timePressed;
};


Button buttons[] = { Button(2), Button(3), Button(4), Button(5), Button(6), Button(7) };
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
    numDesired = 0;
    Serial.println("In Mode 1");
    if(controlButton.getHeldTime()<=2000)
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
    Serial.println("in mode 0");
    if(controlButton.getHeldTime()<=2000){
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