#include <NTPClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

extern bool unit_status;
extern bool ac_mode;
extern bool fan_status;
extern bool  temp_scale;
extern int actualTemp;
extern int setTemp;

struct MyObject {
       int   unit_status;
       int   ac_mode;
       int  fan_status;
       int  temp_scale;
       int  setTemp;
};
int eeAddress = 0;
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = -21600;//Central time
String stime;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


void savetoeeprom() {
  Serial.println("Writtening thermostate data to EEPROM!");
  //Data to store.
  MyObject customVar = {
          unit_status,
          ac_mode,
          fan_status,
          temp_scale,
          setTemp,
  };
  EEPROM.put(eeAddress, customVar);
  EEPROM.commit();
}

void readfromeeprom() {
  MyObject customVar; //Variable to store custom object read from EEPROM.
  EEPROM.get( eeAddress, customVar );

  unit_status = customVar.unit_status;
  ac_mode = customVar.ac_mode;
  fan_status = customVar.fan_status;
  temp_scale = customVar.temp_scale;
  setTemp = customVar.setTemp;


  Serial.println("Reading thermostate data from EEPROM!");
  Serial.print( "unit_status (off:0,on:1)=>" );
  Serial.println( customVar.unit_status );
  Serial.print( "ac_mode(Heat:0,Ac:1)=>" );
  Serial.println( customVar.ac_mode );
  Serial.print( "temp_scale(°C:0,°F:1)=>" );
  Serial.println( customVar.temp_scale ); 
  Serial.print( "setTemp(set to)=>" );
  Serial.println( customVar.setTemp );  


}


String getTimeStampString() {
  char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  char monthOfTheYear[12][12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "July", "Aug","Sept","Oct","Nov","Dec"};
   time_t rawtime = timeClient.getEpochTime();
   struct tm * ti;
   ti = localtime (&rawtime);

   uint16_t year = ti->tm_year + 1900;
   String yearStr = String(year);

   uint8_t month = ti->tm_mon ;
   String monthStr = month < 10 ? "0" + String(month) : String(month);

   uint8_t day = ti->tm_mday;
   String dayStr = day < 10 ? "0" + String(day) : String(day);

   uint8_t hours = ti->tm_hour;
   String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

   uint8_t minutes = ti->tm_min;
   String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

   uint8_t seconds = ti->tm_sec;
   String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);
  //Serial.print(daysOfTheWeek[timeClient.getDay()]);
   //return yearStr + "-" + monthStr + "-" + dayStr + " " +hoursStr + ":" + minuteStr + ":" + secondStr;
   stime=hoursStr +":" +minuteStr+ ":"+secondStr;
   String longdate = String(daysOfTheWeek[timeClient.getDay()]) + "," + String(monthOfTheYear[monthStr.toInt()] ) + " " + dayStr + "," +yearStr;
   Serial.println(longdate);
   return longdate;
}



int booltoint(bool value){
  if(value==false){
    return 0;
  }else{
    return 1;
  }
}
char* processcommand(String comm, int value){
  char* commandbuffer;
  commandbuffer = (char *)malloc(sizeof(comm)); 
  sprintf(commandbuffer, comm.c_str(), booltoint(value));
  return commandbuffer;
}
