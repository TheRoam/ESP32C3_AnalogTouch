//-------------------------------------------------------------------------------//
//-------------------------- TRW Blink AnalogTouch 1.0 --------------------------//
//                                                                               //
// Uses software touch sensitivity from AnalogTouch script to blink a LED.       //
//                                                                               //
// Usage & principles:                                                           //
// Connect a led to any output PIN, then ground it via a resistor.               //
// Connect a jumper wire to ADC pin. Reading will vary constantly.               //
// However, voltage will drop when the wire is touched.                          //
// Let signal stabilize and then touch the wire.                                 //
//                                                                               //
// We read an array of values (delayed 10ms) and calculate the mean value.       //
// If last value is 200mV lower than mean, touch is recognised. LED is light.    //
// Rarely signal can drop 200mV by itself and false touch could be recognised.   //
//                                                                               //
// Original code on github:                                                      //
// https://github.com/TheRoam/TRW_AnalogTouch                                    //
//                                                                               //
// © The Roaming Workschop 2022                                                  //
// https://theroamingworkshop.cloud                                              //
//                                                                               //
//-------------------------------------------------------------------------------//


// OUTPUT verbosity
// 1: extended --> Prints ADC array, mean value, touch count and touch message with touch count
// 0: minimal  --> Prints touch message with touch count
#define OUT 0

// comparison variables
#define VALUES 20       //values for mean array
#define TOUCHPIN 3      //touch pin
#define LEDPIN 20       //led pin
#define VALINTERVAL 100 //evaluation interval
#define REFV 80         //reference voltage drop in mV

// Calibration:
// 1. Set a high REFV, for example 1000
// 2. Run the extended script with OUT 1
// 3. Open Serial Monitor and have a look at the average values for a while (e.g.: 230)
// 4. Now touch the jumper wire and see what the values are now (e.g.: 170)
// 5. Make the difference between the average at 3. and the average at 4 (230-170=60).
// 6. Set your REFV to this value (60) and play around it.
//    -- For more sensitivity, set it a bit lower (50)
//    -- If there is detection with no touch, increase it (80)
//    -- Because of CPU load, REFV value will vary if OUT 1 or OUT 0


int out=OUT;
int mean[VALUES]={};  //change 
int avg;
int fin;
int TP=TOUCHPIN;
int LP=LEDPIN;

// touch counter
int touch=0;
int touchb=0;

// Touch the pin for at least this time for touch to be recognised,
// i.e.: if d=1000ms (1 second) touch the pin for at least 1 second;
// if you hold the pin for several seconds, you might get several touch
// recognised until the mean value is lowered enough.
// This can be used as "hold" recognition.

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  //enable LED pin
  pinMode(LP,OUTPUT);
  setTouch(TP);
}

void loop() {
  setTouch(TOUCHPIN);
  
  //run check
  if(out==1){
    extended(TP);
  }else if(out==0){
    minimal(TP);
  }
}

void setTouch(int tp){
  //fill up the array
  for(int a=0;a<VALUES;a++){
    mean[a]=analogRead(tp);
    
    delay(10);
  }
}

int extended(int tp){
  for(int i=0;i<VALUES;i++){
    //get average of array
    //start printing array
    Serial.println("Array: [");
    for(int j=0;j<VALUES;j++){
      avg+=mean[j];
      //print value
      Serial.print(mean[j]);
      Serial.print(", "); 
    }
    //close array
    Serial.println("]");
    
    avg=avg/VALUES;
    Serial.print("Average: ");
    Serial.println(avg);

    //read final value
    fin=analogRead(tp);

    Serial.print("Last: ");
    Serial.println(fin);
    
    //compare average with last value
    if((avg-fin)>REFV){
      Serial.println("----------  T   O    U    C    H  ----------");
      //increase counter
      touch++;
      //light led
      ledON(LP);
      //wait 1 second after touch detected
      delay(1000);
    }
    //restart counter
    avg=0;
    //restart led
    ledOFF(LP);
    //print touch count
    Serial.print("T: ");
    Serial.println(touch);
    delay(VALINTERVAL);
  }
  return touch;
}

int minimal(int tp){
  for(int i=0;i<VALUES;i++){
    //get average of array
    for(int j=0;j<VALUES;j++){
      avg+=mean[j];
    }
    
    avg=avg/VALUES;

    //read current value
    fin=analogRead(tp);
    
    //compare average with current value
    if((avg-fin)>REFV){
      Serial.println("----------  T   O    U    C    H  ----------");
      //increase counter
      touch++;
      //print touch count
      Serial.print("T: ");
      Serial.println(touch);
      //light led
      ledON(LP);
      //wait 1 second after touch detected
      delay(1000);
    }
    //restart counter
    avg=0;
    //restart led
    ledOFF(LP);
    delay(VALINTERVAL);
  }
  return touch;
}

void ledON(int l){
  //light if touch detected
  if(touch>touchb){
    //turn LED on
    digitalWrite(l,HIGH);
    //redefine touch counters
    touchb=touch;
  }
}

void ledOFF(int l){
  digitalWrite(l,LOW);
}
