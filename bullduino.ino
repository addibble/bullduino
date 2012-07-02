/* THIS CODE DOES NOT WORK FOR THE CAR AS IT IS RIGHT NOW
This is for the previous version that had a dumb motor for direction control.
I've replaced it with a custom rack and pinion assembly and a mini stepper.

To do:
* Tune the stepper motor and figure out how to orient the wheels -
the handmade assembly might skip so I recommend finding the limits and staying away from them (the gears might skip a little)
* Redo the line-following code (and hopefully optimize it)
* Win the contest

*/

// the "enable" pin turns off the ability to drive that side of the h-bridge
#define FR_EN 4
#define FWD_PIN 5
#define REV_PIN 6
#define LR_EN 8
#define RIGHT_PIN 9
#define LEFT_PIN 10

#define S_LEFT A0
#define S_MID A1
#define S_RIGHT A2

#define RED_PIN 7
#define YELLOW_PIN 11
#define CLEAR_PIN 12
#define GREEN_PIN 13

int left_low;
int mid_low;
int right_low;
int left_high;
int mid_high;
int right_high;

void printSensors() {
  while(true) {
    int i=analogRead(S_LEFT);
    Serial.print(i);
    Serial.print(" ");
    i=analogRead(S_MID);
    Serial.print(i);
    Serial.print(" ");
    i=analogRead(S_RIGHT);
    Serial.println(i);
    delay(100);
  }
}

void calibrate() {
  left(255);
  delay(1000);
  left(0);
  long start=millis();
  while(millis()-start < 3000) {
    analogRead(S_LEFT);
    analogRead(S_MID);
    analogRead(S_RIGHT);
  }
  while(millis()-start < 5000) {
    int i=analogRead(S_LEFT);
    Serial.println(i);
    if(i > left_high) left_high=i;
    if(i < left_low) left_low=i;
    i=analogRead(S_MID);
    if(i > mid_high) mid_high=i;
    if(i < mid_low) mid_low=i;
    i=analogRead(S_RIGHT);
    if(i > right_high) right_high=i;
    if(i < right_low) right_low=i;
  }
  Serial.print("left: ");
  Serial.print(left_low);
  Serial.print("-");
  Serial.print(left_high);
  Serial.print(" mid: ");
  Serial.print(mid_low);
  Serial.print("-");
  Serial.print(mid_high);
  Serial.print(" right: ");
  Serial.print(right_low);
  Serial.print("-");
  Serial.print(right_high);
  
}

void straight() {
  analogWrite(RIGHT_PIN, 0);
  analogWrite(LEFT_PIN, 0);  
}

void stopAll() {
  analogWrite(RIGHT_PIN, 0);
  analogWrite(LEFT_PIN, 0);
  analogWrite(FWD_PIN, 0);
  analogWrite(REV_PIN, 0);  
}

void right(int x) {
  analogWrite(LEFT_PIN, 0);  
  analogWrite(RIGHT_PIN, x);
}

void left(int x) {
  analogWrite(RIGHT_PIN, 0);  
  analogWrite(LEFT_PIN, x);
}

void fwd(int x) {
  analogWrite(REV_PIN, 0);
  analogWrite(FWD_PIN, x); 
}

void rev(int x) {
  analogWrite(FWD_PIN, 0);
  analogWrite(REV_PIN, x); 
}

#define SZ 10

int l, m, r;
//long lt, mt, rt;
//byte index;

void setup() {
  Serial.begin(115200);
  analogReference(DEFAULT);
  pinMode(S_LEFT, INPUT);
  pinMode(S_MID, INPUT);
  pinMode(S_RIGHT, INPUT);
  digitalWrite(S_LEFT, LOW);
  digitalWrite(S_MID, LOW);
  digitalWrite(S_RIGHT, LOW);
  pinMode(FR_EN, OUTPUT);
  pinMode(FWD_PIN, OUTPUT);
  pinMode(REV_PIN, OUTPUT);
  pinMode(LR_EN, OUTPUT);
  pinMode(LEFT_PIN, OUTPUT);
  pinMode(RIGHT_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(CLEAR_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(FR_EN, HIGH);
  digitalWrite(LR_EN, HIGH);
  analogWrite(RIGHT_PIN, 0);
  analogWrite(LEFT_PIN, 0);
  analogWrite(FWD_PIN, 0);
  analogWrite(REV_PIN, 0);
  stopAll();
}

void clearLEDs() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(CLEAR_PIN, LOW);
}
#define LOW_THRESH 300
#define HIGH_THRESH 600
#define MID_THRESH 450
// threshold for analogRead() to say we're over a reflective surface -- low is more reflective, high is less reflective
// this is empirical right now but calibrate() is supposed to find it but is not finished yet.
#define THRESH 200

long not_found_ctr=0;
int on=1;

void loop() {
  // this will move the wheels back and forth then center, then blink the red light forever
  right(60);
  delay(1000);
  left(60);
  delay(2000);
  right(60);
  delay(2000);
  left(60);
  delay(1000);
  stopAll();
    while(true){
      digitalWrite(RED_PIN, HIGH);
      delay(500);
      digitalWrite(RED_PIN, LOW);
      delay(500);
    }

  /* this is the old version -- it's commented out 
  if(on>0) {
    fwd(200);
    on++;
    if(on>30) {
      on=-100;
    }
  } else {
    fwd(0);
    on++;
  }
  l=analogRead(S_LEFT);
  r=analogRead(S_RIGHT);
  m=analogRead(S_MID);
  Serial.print(l);
  Serial.print(" ");
  Serial.print(m);
  Serial.print(" ");
  Serial.println(r);
  if(l < THRESH && m < THRESH && r < THRESH) {
    stopAll();
    while(true){
      digitalWrite(GREEN_PIN, HIGH);
      delay(200);
      digitalWrite(GREEN_PIN, LOW);
      delay(200);
    }
  } else if(not_found_ctr > 50000) {
    stopAll();
    while(true){
      digitalWrite(RED_PIN, HIGH);
      delay(500);
      digitalWrite(RED_PIN, LOW);
      delay(500);
    }
  } {
    if(l > THRESH && m > THRESH && r > THRESH) {
      digitalWrite(YELLOW_PIN, HIGH);
      not_found_ctr++;
    } else {
      digitalWrite(YELLOW_PIN, LOW);
      clearLEDs();
      not_found_ctr=0;
      if(l < THRESH) {
        digitalWrite(GREEN_PIN, HIGH);
        left(255);
      } else if(r < THRESH) {
        digitalWrite(CLEAR_PIN, HIGH);
        right(255);
      } else if(m < THRESH) {
        clearLEDs();
        straight();
      }
    }
  }*/
}

