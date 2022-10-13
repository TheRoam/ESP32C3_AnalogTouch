//-------------------------------------------------------------------------------//
//------------------------ XIAO ESP32-C3 AnalogTouch 1.1 ------------------------//
//                                                                               //
// Based on AnalogRead example script for Esp32 library by Espressif             //
// Usage & principles:                                                           //
// Connect a jumper wire to ADC pin. Reading will vary constantly.               //
// However, voltage drops about 200mV when the wire is touched.                  //
// Let signal stabilize and then touch the wire.                                 //
//                                                                               //
// We read an array of values (delayed 10ms) and calculate the mean value.       //
// If last value is 200mV lower than mean, touch is recognised.                  //
// Rarely signal can drop 200mV by itself and false touch could be recognised.   //
//                                                                               //
// Original code on github:                                                      //
// https://github.com/TheRoam/XIAO_ESP32-C3_AnalogTouch                          //
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
#define VALINTERVAL 100 //evaluation interval
#define REFV 80         //reference voltage drop in mV
#define WARMUP 0        //delay time after reboot before evaluation starts

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
//    -- Touch could triggers itself for a few seconds during start, as analog readings are higher.


int out=OUT;
int wmp=WARMUP;
int mean[VALUES]={};  //change 
int avg;
int fin;
int TP=TOUCHPIN;

// touch counter
int touch=0;

// Touch the pin for at least this time for touch to be recognised,
// i.e.: if d=1000ms (1 second) touch the pin for at least 1 second;
// if you hold the pin for several seconds, you might get several touch
// recognised until the mean value is lowered enough.
// This can be used as "hold" recognition.

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
}

void loop() {
  //wait for board to normalise
  if(wmp==1){
    Serial.println("Warming up....");
    delay(WARMUP*1000);
    Serial.println("Start readings!");
    wmp=0;
  }
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
    // send value to array
    mean[i]=analogRead(tp);
    //start printing array in monitor
    Serial.println("Array: [");

    //read final value
    fin=analogRead(tp);

    Serial.print("Last: ");
    Serial.println(fin);
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
    
    //compare average with last value
    if((avg-fin)>REFV){
      Serial.println("----------  T   O    U    C    H  ----------");
      //increase counter
      touch++;
      //wait 1 second after touch detected
      delay(1000);
    }
    //restart counter
    avg=0;
    //print touch count
    Serial.print("T: ");
    Serial.println(touch);
    delay(VALINTERVAL);
  }
  return touch;
}

int minimal(int tp){
  //start reading values
  for(int i=0;i<VALUES;i++){
    // read value
    mean[i]=analogRead(tp);

    //read final value
    fin=analogRead(tp);
    //get average of array
    for(int j=0;j<VALUES;j++){
      avg+=mean[j];
    }
    
    avg=avg/VALUES;
    
    //compare average with last value
    if((avg-fin)>REFV){
      Serial.println("----------  T   O    U    C    H  ----------");
      //increase counter
      touch++;
      //print touch count
      Serial.print("T: ");
      Serial.println(touch);
      //wait 1 second after touch detected
      delay(1000);
    }
    //restart counter
    avg=0;
    delay(VALINTERVAL);
  }
  return touch;
}
