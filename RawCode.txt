
class Button {
public:
  int pin; 
  Button(int pin) : pin(pin) {
    pinMode(pin, INPUT_PULLUP);
    prevState = HIGH;
    pressTime = -1;
  }

  void update() {
    pressed = false;
    released = false;
    int state = digitalRead(pin);
    //Serial.println(digitalRead(pin));
    
    if (state == prevState) return;
    
    
    if (state == LOW){
      pressed = true;
      heldAction = true;
      Serial.println("Button Pressed");
      pressTime = millis();
    }
    else
      released = true;
    prevState = state;
  }
  bool isPressedOnce() {
    if (pressed)
      Serial.println(pressed);
    return pressed;
  }
  bool isReleased() {
    return released;
    pressTime = -1;
  }
  bool isHeld(){
    if(prevState == LOW)
      return true;
    return false;
  }

  int getHeldTime() {
    if (pressTime == -1) return -1;
    if (heldAction) //make sure this can only trigger once per hold
      return millis() - pressTime;
  }
  void resetHeldTime(){
    pressTime = -1;
  }
  void useHeldAction(){
    heldAction = false;
  }


private:      
  int prevState;
  bool pressed;
  bool released;
  int pressTime;
  int timePressed;
  bool heldAction;
};


Button buttons[] = { Button(2), Button(3), Button(4), Button(5), Button(6), Button(7) };
Button controlButton = { Button(12) };

int mode = 1; //0 means editing, 1 means reading
int numDesired;

int getNumPressed(){
  int retNum = 0;
  for(int i = 0; i < sizeof(buttons)/sizeof(buttons[0]); i++){
    if(buttons[i].isHeld())
      retNum++;
  }
  return retNum;
}

void setup() {
  pinMode(11, OUTPUT); //Green Light
  pinMode(10, OUTPUT); //Red Light
  Serial.begin(9600);
}

int startTime;
void loop() {
  
  controlButton.update();
  if(mode==1){
    // Serial.print("Num Desired: ");
    // Serial.println(numDesired);
    // Serial.print("Num Pressed: ");
    // Serial.println(getNumPressed());
    //Serial.println(controlButton.getHeldTime());
    //Serial.println("In Mode 1");
    if(controlButton.getHeldTime()>=2000){
      controlButton.useHeldAction();
      mode = 0;
      startTime = millis();
      numDesired = 0;
      controlButton.resetHeldTime();
      digitalWrite(11, LOW);
      digitalWrite(10, LOW);
    }
    if(getNumPressed() == numDesired){
      digitalWrite(11, HIGH);
      digitalWrite(10, LOW);
    }
    else{
      digitalWrite(11, LOW);
      digitalWrite(10, HIGH);
    }
    
    for (Button &b : buttons) {
      b.update();

      if (b.isPressedOnce()) {
        Serial.print("Button on pin ");
        Serial.print(b.pin);
        Serial.println(" is PRESSED");
      }
    }
  }
  else{
    //Serial.println("in mode 0");
    if(controlButton.getHeldTime()>=2000){
      mode = 1;
      numDesired--; //so that holding the button doesn't add one
      controlButton.resetHeldTime();
      controlButton.useHeldAction();
      digitalWrite(11, LOW);
      digitalWrite(10, LOW);
    }
    if(millis()<=startTime+200) //green light blinks once when entering writing mode and when adding one to desiredAmount
      digitalWrite(11, HIGH);
    else
      digitalWrite(11, LOW);

    if(millis()%1000<750){ //Red light is constantly blinking to indicate it's in writing mode
      digitalWrite(10, HIGH);
    }
    else
      digitalWrite(10, LOW);
    if(controlButton.isPressedOnce()){
      if(numDesired+1<=sizeof(buttons)/sizeof(buttons[0])){
        Serial.println("num desired up");
        numDesired++;
      }
      else{
        Serial.println("Num desired reset");
        numDesired = 1;
      }
      Serial.println(numDesired);
      startTime = millis();
    }
  }
  delay(100);
}
