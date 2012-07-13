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

// the current position of the wheels
int currentAngle=0;
// the desired position of the wheels
int desiredAngle=0;
// the time since the last steering update
unsigned long t=0;
unsigned long ts;

void checkSteering() {
  // update the time since the last steering update
  t=millis()-ts;
  if(t < 1) {
    return;
  }
  ts=millis();

  // if we're to the right of where we should be
  if(currentAngle > desiredAngle) {
     digitalWrite(RIGHT_PIN, LOW);
     digitalWrite(LEFT_PIN, HIGH);
     currentAngle-=t;
   } else if(currentAngle < desiredAngle) {
     digitalWrite(LEFT_PIN, LOW);
     digitalWrite(RIGHT_PIN, HIGH);
     currentAngle+=t;
   } else {
     // dead center
     digitalWrite(RIGHT_PIN, LOW);
     digitalWrite(LEFT_PIN, LOW);
   }
}

void stopAll() {
  analogWrite(RIGHT_PIN, 0);
  analogWrite(LEFT_PIN, 0);
  analogWrite(FWD_PIN, 0);
  analogWrite(REV_PIN, 0);  
}

#define MAX_TURN 210

void stop(void) {
  analogWrite(REV_PIN, 0);
  analogWrite(FWD_PIN, 0); 
}

void fwd(int x) {
  analogWrite(REV_PIN, 0);
  analogWrite(FWD_PIN, x); 
}

void rev(int x) {
  analogWrite(FWD_PIN, 0);
  analogWrite(REV_PIN, x); 
}

void halt(byte pin, int dur) {
  stopAll();
  while(true) {
    digitalWrite(pin, HIGH);
    delay(dur);
    digitalWrite(pin, LOW);
    delay(dur);
  }
}

#define SZ 10

int l, m, r;
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
#define THRESH 100

long not_found_ctr=0;
int on=1;

void loop() {
  checkSteering();
  if((millis() / 80) % 2) {
    fwd(160);
  } else {
    stop();
  }
/*
  Serial.print(currentAngle);
  Serial.print(" ");
  Serial.println(desiredAngle);

  if((millis() / 2000) % 2) {
    desiredAngle=0;
  } else {
    if((millis() / 1000) % 2) {  
      desiredAngle=210;
    } else {
      desiredAngle=-210;
    }
  }
  */

  clearLEDs();
  l=analogRead(S_LEFT);
  r=analogRead(S_RIGHT);
  m=analogRead(S_MID);
  Serial.print(currentAngle);
  Serial.print(" ");
  Serial.print(desiredAngle);
  Serial.print(" ");
  Serial.print(l);
  Serial.print(" ");
  Serial.print(m);
  Serial.print(" ");
  Serial.println(r);
  if(l < THRESH && m < THRESH && r < THRESH) {
    halt(GREEN_PIN, 200);
  } else if(not_found_ctr > 1000) {
    halt(RED_PIN, 500);
  } else {
    if(l > THRESH && m > THRESH && r > THRESH) {
      digitalWrite(RED_PIN, HIGH);
      not_found_ctr++;
    } else {
      not_found_ctr=0;
      if(l < THRESH && desiredAngle > -200) {
        digitalWrite(CLEAR_PIN, HIGH);
        desiredAngle--;
      } else if(r < THRESH && desiredAngle < 200) {
        digitalWrite(GREEN_PIN, HIGH);
        desiredAngle++;
      } else if(m < THRESH) {
        digitalWrite(YELLOW_PIN, HIGH);
        desiredAngle=0;
      }
    }
  }
}

