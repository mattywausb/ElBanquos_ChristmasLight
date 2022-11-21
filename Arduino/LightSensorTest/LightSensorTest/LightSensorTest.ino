
#define TRACE_ON
#define INPUT_ANALOG_PORT_THRESHOLD 0
#define INPUT_ANALOG_PORT_SENSOR 1

#define INPUT_HISTERESYS_GAP 50
#define INPUT_THRESHOLD_BASE INPUT_HISTERESYS_GAP+10

int input_normalized_light_sensor_value=0;
byte g_daylight=0;

char stringBuffer[60];
  
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

  input_determine_normalized_light_sensor_value();
  
  if(input_normalized_light_sensor_value>1) g_daylight=1;
  if(input_normalized_light_sensor_value<-1) g_daylight=0;

  digitalWrite(LED_BUILTIN, g_daylight);
  sprintf(stringBuffer," daylight=%1d\n",g_daylight);
  Serial.print(stringBuffer);
  delay(200);  
  
}


void input_determine_normalized_light_sensor_value()
{
  int sensor_value=analogRead(INPUT_ANALOG_PORT_SENSOR);
  int treshold_base_value=analogRead(INPUT_ANALOG_PORT_THRESHOLD)+INPUT_THRESHOLD_BASE;


  input_normalized_light_sensor_value=(sensor_value-treshold_base_value)/INPUT_HISTERESYS_GAP;
  sprintf(stringBuffer,"s=%5d t=%5d nlv=%2d",sensor_value,treshold_base_value,input_normalized_light_sensor_value);
  Serial.print(stringBuffer);
}
