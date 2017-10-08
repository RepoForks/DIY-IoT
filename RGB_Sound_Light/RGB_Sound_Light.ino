
#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3
#define SOUND A0

const int sampleWindow = 30; // sampling ms
unsigned int sample;

int redNow;
int blueNow;
int greenNow;

unsigned int total=0;
unsigned int avg=0;
unsigned int count=1;

void setup() {
  Serial.begin(9600);

  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);

  redNow = random(255);
  blueNow = random(255);
  greenNow = random(255);

  analogWrite(REDPIN, redNow);
  analogWrite(BLUEPIN, blueNow);
  analogWrite(GREENPIN, greenNow);
}

void loop() {
  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(SOUND);
    if (sample > signalMax)
        signalMax = sample;  // save just the max levels
      else if (sample < signalMin)
        signalMin = sample;  // save just the min levels
  }

  Serial.println(sample);

  if(count < 1000) {
    total += sample;
    avg = total/count;
    count++;
  } else {
    total = 0;
    count = 1;
  }

  if((sample > signalMax - 2) || (sample < signalMin + 2)) {
    redNow = random(255);
    blueNow = random(255);
    greenNow = random(255);

    analogWrite(REDPIN, redNow);
    analogWrite(BLUEPIN, blueNow);
    analogWrite(GREENPIN, greenNow);
    delay(10);
  }
}
