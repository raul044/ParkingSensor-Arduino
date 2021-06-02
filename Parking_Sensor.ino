#include <TFT.h>
#include <SPI.h>
#include <Servo.h>
#include <QueueArray.h>
#define TRIGGER_PIN 5
#define ECHO_PIN 6
#define MAX_DIST 200
#define SERVO_PIN 3
#define POLL_RANGE 20
#define BUZZ_PIN 4

#define cs 10
#define dc 9
#define rst 8

TFT TFTscreen = TFT(cs, dc, rst);
Servo myservo;

struct obstacle {
  float dist;
  int ang;
};
int nr_draw = 0, q_size = 1;
QueueArray<obstacle> draw_buff;

const float rad_const= 57.2958;

int pos, dir;
int poll_idx, count, sum, avg;
long duration;
int distance;

void setup() {
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
  int w = 70;
  int h = 70;
  int r = 10;
  TFTscreen.fillRoundRect(TFTscreen.width()/2-w/2, TFTscreen.height()/2+h/4, w, h, r, 0x43A2C7);
  TFTscreen.fillCircle(TFTscreen.width()/2-w/2+r, TFTscreen.height()/2+h/4+r, r, 0x2039FF);
  TFTscreen.fillCircle(TFTscreen.width()/2+w/2-r, TFTscreen.height()/2+h/4+r, r, 0x2039FF);

  dir = 1;
  poll_idx = 0;
  count = 0;
  sum = 0;
  avg = 0;
 
    myservo.attach(SERVO_PIN);
  
    Serial.begin(9600);

    pinMode(BUZZ_PIN, OUTPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
  
    delay(50);
}


int getDistance() {
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration*0.034/2;
    distance = (distance > MAX_DIST) ? 0 : distance;

    Serial.print("Distance: ");
    Serial.println(distance);

    return distance;
}

void loadObstacle(float d, int ang, int color) {
  int dist = (int) d;
  int h = 70;
  int r = 7;
  TFTscreen.fillCircle(TFTscreen.width()/2 + dist * cos((float)ang/rad_const),
                       TFTscreen.height()/2+h/4-r-1 - dist * sin((float)ang/rad_const),
                       r, color);
}

void deleteObstacle(float d, int ang) {
  int dist = (int) d;
  int h = 70;
  int r = 7;
  TFTscreen.fillCircle(TFTscreen.width()/2 + dist * cos((float)ang/rad_const),
                       TFTscreen.height()/2+h/4-r-1 - dist * sin((float)ang/rad_const),
                       r, 0);
}

void loop() {
  for (int i = 0; i < 8; ++i) {
      if (pos >= 180 || pos < 0) {
        dir *= -1;
      }

      pos += dir;
  }
  myservo.write(pos);

  poll_idx++;
    if (poll_idx >= 100)
      poll_idx = 0;


  int dist = getDistance();


    int color = 0;
    
    if (dist < 33 && dist != 0) {
        if (poll_idx % 3 == 0)
          tone(BUZZ_PIN, 3000, 50);
        color = 0x2039FF;
    } else if (dist < 66 && dist != 0) {
        if (poll_idx % 8 == 0)
          tone(BUZZ_PIN, 3000, 100);
        color = 0x8B008B;
    } else if (dist < 100 && dist != 0) {
        if (poll_idx % 20 == 0)
          tone(BUZZ_PIN, 3000, 200);
        color = 0xd51dcc;
    } 
   
  
  float mapped = (float)dist / 100 * 70;
  if (dist > 0 && dist <= 100) {
    loadObstacle(mapped, pos, color);
    
    obstacle o;
    o.dist = mapped;
    o.ang = pos;
    draw_buff.enqueue(o);
    nr_draw++;
    
    delay(50);

    if (nr_draw == q_size) {
      obstacle out = draw_buff.dequeue();
      deleteObstacle(out.dist, out.ang);
      nr_draw--;
    }
  } else {
    if (nr_draw > 0) {
      obstacle out = draw_buff.dequeue();
      deleteObstacle(out.dist, out.ang);
      nr_draw--;
    }
    delay(50);
  }
  
}
