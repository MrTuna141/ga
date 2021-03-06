#include <Wire.h>
#include <RtcDS3231.h> //Includes the library for the RTC(Real Time Clock) 
/*
RTC_MODULE 
*/
RtcDS3231<TwoWire> rtcModule(Wire);
void setup() {
  Serial.begin(115200);
  Wire.begin(D1, D2); //Reads the information from pin D1 and D2(The clock)
  
    //OBS!!! ONLY TO SET RESET THE TIME ON THE RTC
    RtcDateTime compiled = RtcDateTime(__DATE__,__TIME__);
    rtcModule.SetDateTime(compiled);
}

void loop() {
  
  RtcDateTime now = rtcModule.GetDateTime(); //Takes the current time from the RTC to the variable now
    byte hours = now.Hour();  //Gives the time in hours
    byte minutes = now.Minute();  //Gives the time in minutes
    byte seconds = now.Second();  //Gives the time in seconds
    char timeString[16];  //The ammount of characters timeString can store
    sprintf(timeString,"Time: %02u:%02u:%02u", hours, minutes, seconds);  //Writes the hours, minutes and seconds to the variable timeSting
    //Serial.println(timeString); //Prints timeString

    //if statement for timer
    if(hours == 3 && minutes == 50 && seconds == 0){
        makeCoffee();
        
    }

  Serial.println(timeString); //Used as trubleshoot
    
}
