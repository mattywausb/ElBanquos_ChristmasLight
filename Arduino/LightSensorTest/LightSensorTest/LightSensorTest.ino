
#define TRACE_ON
#define INPUT_ANALOG_PORT_THRESHOLD 0
#define INPUT_ANALOG_PORT_SENSOR 1

#define INPUT_HISTERESYS_GAP 30
#define INPUT_THRESHOLD_BASE INPUT_HISTERESYS_GAP+20

byte g_daylight=0;

void setup() {
  #ifdef TRACE_ON 
    char compile_signature[] = "--- START (Build: " __DATE__ " " __TIME__ ") ---";   
    Serial.begin(9600);
    Serial.println(compile_signature); 
  #endif

  // Init all Pins and interfaces
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  int sensor_value=analogRead(INPUT_ANALOG_PORT_SENSOR);
  int treshold_base_value=analogRead(INPUT_ANALOG_PORT_THRESHOLD)+INPUT_THRESHOLD_BASE;
  char stringBuffer[50];
  if(sensor_value>treshold_base_value+INPUT_HISTERESYS_GAP) g_daylight=true;
  if(sensor_value<treshold_base_value-INPUT_HISTERESYS_GAP) g_daylight=false;

  digitalWrite(LED_BUILTIN, g_daylight);
  sprintf(stringBuffer,"sensor=%5d  threshold=%5d  daylight=%1d\n",sensor_value,treshold_base_value,g_daylight);
  Serial.print(stringBuffer);
  delay(200);  
  
}
