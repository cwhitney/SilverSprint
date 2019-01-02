/*
 * Arduino wiring:
 *
 * Digital pin  Connected to
 * -----------  ------------
 * 2            Sensor 0
 * 3            Sensor 1
 * 4            Sensor 2
 * 5            Sensor 3
 *
 * 9            Racer0 Start LED anode, Stop LED cathode
 * 10           Racer1 Start LED anode, Stop LED cathode
 * 11           Racer2 Start LED anode, Stop LED cathode
 * 12           Racer3 Start LED anode, Stop LED cathode
 *
 */

#define VERSION "SS_v0.1.7"
#define FALSE_START_TICKS 4
#define MAX_RACERS 4

int statusLEDPin = 13;

long statusBlinkInterval = 250;
int lastStatusLEDValue = LOW;
long previousStatusBlinkMillis = 0;

boolean raceStarted = false;
boolean raceStarting = false;
boolean mockMode = false;
unsigned long raceStartMillis;
unsigned long currentTimeMillis;

char val = 0;

int racer0GoLedPin = 9;
int racer1GoLedPin = 10;
int racer2GoLedPin = 11;
int racer3GoLedPin = 12;

int sensorPins[4] = {2,3,4,5};
int previoussensorValues[4] = {HIGH,HIGH,HIGH,HIGH};
int values[4] = {0,0,0,0};
unsigned long racerTicks[4] = {0,0,0,0};
unsigned long racerFinishTimeMillis[4];

unsigned long lastCountDownMillis;
int lastCountDown;

char charBuff[8];
unsigned int charBuffPos = 0;
bool isReceivingRaceLength = false;
bool isReceivingTimeLength = false;

int raceLengthTicks = 20;
int raceLengthSecs = 60;
int previousFakeTickMillis = 0;

int updateInterval = 10;
unsigned long lastUpdateMillis = 0;

//float mockSpeedsKph[4] = {10.0, 20.0, 30.0, 40.0};
float mockSpeedsKph[4] = {40.0, 56.0, 48.0, 32.0};

bool bRaceTypeDistance = true;

void setup() 
{
    Serial.begin(115200);
    pinMode(statusLEDPin, INPUT);
    pinMode(racer0GoLedPin, OUTPUT);
    pinMode(racer1GoLedPin, OUTPUT);
    pinMode(racer2GoLedPin, OUTPUT);
    pinMode(racer3GoLedPin, OUTPUT);
    digitalWrite(racer0GoLedPin, LOW);
    digitalWrite(racer1GoLedPin, LOW);
    digitalWrite(racer2GoLedPin, LOW);
    digitalWrite(racer3GoLedPin, LOW);
    
    for(int i=0; i<MAX_RACERS; i++) {
        pinMode(sensorPins[i], INPUT);
        digitalWrite(sensorPins[i], HIGH);
    }
}

void blinkLED() 
{
    if (millis() - previousStatusBlinkMillis > statusBlinkInterval) {
        previousStatusBlinkMillis = millis();
        lastStatusLEDValue = !lastStatusLEDValue;
        digitalWrite(statusLEDPin, lastStatusLEDValue);
    }
}

boolean isAlphaNum(char c)
{
    if(c >= '0' && c <= '9'){
        return true;
    }
    return false;
}

void checkSerial()
{
    if(Serial.available() > 0) {
        val = Serial.read();
        if(val == '\r' || val == '\n') {
            if(isReceivingRaceLength){
                isReceivingRaceLength = false;
                charBuff[charBuffPos] = '\0'; // null char to terminate the string
                raceLengthTicks = atoi(charBuff);
                Serial.print("L:");
                Serial.println(raceLengthTicks);  // send confirmation
            }else if(isReceivingTimeLength){
                isReceivingTimeLength = false;
                charBuff[charBuffPos] = '\0'; // null char to terminate the string
                raceLengthSecs = atoi(charBuff);
            }
            return;
        }
        // ----------------------------------------------------
        if(isReceivingRaceLength || isReceivingTimeLength) {
            charBuff[charBuffPos] = val;
            charBuffPos++;
        }else {
            if(val == 'l') {
                memset(charBuff, 0, sizeof(charBuff));
                charBuffPos = 0;
                isReceivingRaceLength = true;
            }
            else if(val == 't') {
                memset(charBuff, 0, sizeof(charBuff));
                charBuffPos = 0;
                isReceivingTimeLength = true;
            }
            else if(val == 'v') {
                Serial.print("V:");
                Serial.println(VERSION);
            }
            else if(val == 'g') {   // start race
                for(int i=0; i<MAX_RACERS; i++){
                    racerTicks[i] = 0;
                    racerFinishTimeMillis[i] = 0;
                }

                raceStarting = true;
                raceStarted = false;
                lastCountDown = 4;
                lastCountDownMillis = millis();
            }
            else if(val == 'm') {   // toggle mock mode
                mockMode = !mockMode;
                if(mockMode){ Serial.println("M:ON"); }
                else{ Serial.println("M:OFF"); }
            }
            else if(val == 's') {   // stop race
                raceStarted = false;
                raceStarting = false;

                digitalWrite(racer0GoLedPin,LOW);
                digitalWrite(racer1GoLedPin,LOW);
                digitalWrite(racer2GoLedPin,LOW);
                digitalWrite(racer3GoLedPin,LOW);
            }
            else if(val == 'x'){
                bRaceTypeDistance = false;
            }
            else if(val == 'd'){
                bRaceTypeDistance = true;
            }
            else {
                Serial.print("ERROR:Command invalid ");
                if(val > 32 && val < 127) {
                    Serial.println(char(val));
                }else {
                    Serial.print("ERROR:Unprintable ASCII code ");
                    Serial.println(val);
                }
            }
        }
    }
}

void loop() 
{
    blinkLED();
    checkSerial();

    if (raceStarting) {
        // Report false starts
        for(int i=0; i<MAX_RACERS; i++) {
            values[i] = digitalRead(sensorPins[i]);
            if(racerTicks[i] < FALSE_START_TICKS) {
                if(values[i] == HIGH && previoussensorValues[i] == LOW){
                    racerTicks[i]++;
                    if(racerTicks[i] == FALSE_START_TICKS) {
                        Serial.print("FS:");
                        Serial.println(i, DEC);
                        digitalWrite(racer0GoLedPin+i,LOW);
                    }
                }
            }
            previoussensorValues[i] = values[i];
        }
        if((millis() - lastCountDownMillis) > 1000){
            lastCountDown -= 1;
            lastCountDownMillis = millis();
            Serial.print("CD:");
            Serial.println(lastCountDown, DEC);
        }
        if(lastCountDown == 0) {
            raceStart();
        }
    }
    if (raceStarted) {
        currentTimeMillis = millis() - raceStartMillis;

        updateRacerTicks();
        
        if(bRaceTypeDistance == true){
            checkDistanceBased();
        }else{
            checkTimeBased();
        }

        printRacerUpdate();
    }    
}

void updateRacerTicks()
{
    for(int i=0; i<MAX_RACERS; i++) {
        previoussensorValues[i] = values[i];
        values[i] = digitalRead(sensorPins[i]);

        if(mockMode){
            // 1 km/hr = 0.2778 mm/ms
            // 114.3mm == fake roller diameter            
            float mmSec = mockSpeedsKph[i] * 277.77778;
            float curTimeSecs = currentTimeMillis / 1000.0;
            racerTicks[i] = floor( currentTimeMillis * mockSpeedsKph[i] * 0.2778 / (114.3 * PI));
        }

        if(values[i] == HIGH && previoussensorValues[i] == LOW){
            racerTicks[i]++;
        }
    }
}

void printRacerUpdate() 
{
    if(currentTimeMillis - lastUpdateMillis > updateInterval) {
        lastUpdateMillis = currentTimeMillis;

        Serial.print("R:");

        for(int i=0; i<MAX_RACERS; i++){
            Serial.print(racerTicks[i], DEC);
            Serial.print(",");
        }
        Serial.println(currentTimeMillis, DEC);
    }
}

void raceStart() 
{
    raceStartMillis = millis();
    lastUpdateMillis = raceStartMillis;

    raceStarting = false;
    raceStarted = true;

    // reset all racers
    for(int i=0; i<MAX_RACERS; i++) {
        racerTicks[i] = 0;
        racerFinishTimeMillis[i] = 0;
    }

    digitalWrite(racer0GoLedPin,HIGH);
    digitalWrite(racer1GoLedPin,HIGH);
    digitalWrite(racer2GoLedPin,HIGH);
    digitalWrite(racer3GoLedPin,HIGH);
}

void checkDistanceBased()
{
    bool bFinished = true;
    for(int i=0; i<MAX_RACERS; i++) {
        // check to see if they have finished
        if(racerFinishTimeMillis[i] == 0 && racerTicks[i] >= raceLengthTicks) {
            racerFinishTimeMillis[i] = currentTimeMillis;
            
            Serial.print(i);
            Serial.print("F:");
            Serial.println(racerFinishTimeMillis[i], DEC);
            digitalWrite(racer0GoLedPin+i,LOW);
        }
        
        if(racerFinishTimeMillis[i] == 0){
            bFinished = false;
        }
    }

    if(bFinished == true){
        raceStarting = false;
        raceStarted = false;
    }
}

void checkTimeBased()
{
    if(currentTimeMillis > raceLengthSecs * 1000){
        raceStarted = false;
        // print all racers
        for(int i=0; i<MAX_RACERS; i++) {
            Serial.print(i);
            Serial.print("F:");
            Serial.println(raceLengthSecs * 1000, DEC);
        }
        raceStarting = false;
        raceStarted = false;
    }
}
