//-------------------------------------------------------------------------------//
//------------------------ XIAO ESP32-C3 AnalogTouch 1.0 ------------------------//
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
int out=1;

// comparison variables
int MV=20;        //values for mean array
int mean[20]={};  //change 
int avg;
int fin;
int refV=200;     //voltage drop that triggers detection (mV)

// touch pin (Define any ADC pin on your board)
int TP=3;

// touch counter
int touch=0;

// pin reading time (higher value, lower CPU load, but lower precison)
// 100ms is usually OK

int d=100;

// Touch the pin for at least this time for touch to be recognised,
// i.e.: if d=1000ms (1 second) touch the pin for at least 1 second;
// if you hold the pin for several seconds, you might get several touch
// recognised until the mean value is lowered enough.
// This can be used as "hold" recognition.

void setup() {
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  //set the resolution to 12 bits (0-4096)
  analogReadResolution(12);

  //fill up the array
  for(int a=0;a<MV;a++){
    mean[a]=analogRead(TP);
    delay(10);
  }
}

void loop() {

  if(out==1){
    extended();
  }else if(out==0){
    minimal();
  }
  
}

void extended(){
  //start reading values
  for(int i=0;i<MV;i++){
    // read value
    mean[i]=analogRead(TP);
    //start printing array in monitor
    Serial.println("Array: [");

    //read final value
    fin=analogRead(TP);

    //get average of array
    for(int j=0;j<MV;j++){
      avg+=mean[j];
      Serial.print(mean[j]);
      Serial.print(", ");
    }
    Serial.println("]");
    
    avg=avg/MV;
    Serial.print("Average: ");
    Serial.println(avg);

    //compare average with last value
    if((avg-fin)>refV){
      Serial.println("----------  T   O    U    C    H  ----------");
      //increase counter
      touch++;
      Serial.print("Touch count: ");
      Serial.println(touch);
      //wait 1 second after touch detected
      delay(1000);
    }
    //restart counter
    if(i==(MV-1)){
      i=-1;
    }
    //print touch count
    Serial.print("T: ");
    Serial.println(touch);
    
    delay(d);
  }
}

void minimal(){
  //start reading values
  for(int i=0;i<MV;i++){
    // read value
    mean[i]=analogRead(TP);

    //read final value
    fin=analogRead(TP);

    //get average of array
    for(int j=0;j<MV;j++){
      avg+=mean[j];
    }
    
    avg=avg/MV;

    //compare average with last value
    if((avg-fin)>refV){
      Serial.println("----------  T   O    U    C    H  ----------");
      //increase counter
      touch++;
      Serial.print("Touch count: ");
      Serial.println(touch);
      //wait 1 second after touch detected
      delay(1000);
    }
    //restart counter
    if(i==(MV-1)){
      i=-1;
    }
    delay(d);
  }
}
