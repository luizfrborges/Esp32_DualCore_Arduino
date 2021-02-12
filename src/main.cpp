
#include "headers/tasklibs.h"
#include "headers/DHT.h"
#include "headers/MQ135.h"


#define DHTPIN 18
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

TaskHandle_t Task1;
TaskHandle_t Task2;


int adc_value  = 0;
float co2ppm = 0.0;

float umidade;
float temperatura;

float alarme_temp = 25.00;
float alarme_co2ppm = 1000.00;



//Task1code: Leitura dos sensores
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  delay(2000);

  for(;;){
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    delay(1000);

    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    delay(1000);

    adc_value = analogRead(34);

    MQ135 gasSensor = MQ135(34);
    float ppm = gasSensor.getPPM();
    co2ppm = ppm;

    umidade = h;
    temperatura = t;
  } 
}


//Task2code: Led Status
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    digitalWrite(4, HIGH); // Blink opercao vermelho
    digitalWrite(16, HIGH); // Blink opercao verde
    digitalWrite(17, HIGH); // Blink opercao azul
    
    delay(4000);

    if((co2ppm > alarme_co2ppm)&&(temperatura > alarme_temp)){
      //digitalWrite(17, HIGH);
      digitalWrite(4, LOW); // CO2 Alarme vermelho
      //digitalWrite(16, LOW); // Temperatura Alarme verde
      delay(4000);
    }
    else if(co2ppm > alarme_co2ppm){
        //digitalWrite(17, HIGH);
        digitalWrite(4, LOW); 
        delay(4000);
      }
    else if(temperatura > alarme_temp){
      //digitalWrite(17, HIGH);
      digitalWrite(16, LOW);
      delay(4000);
    }
    else{
      digitalWrite(17, LOW);    
      delay(4000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 1 */
    //delay(500); 

}





void loop() {
    delay(2000);
    //MQ135 gasSensor = MQ135(34);
    
    //float rzero = gasSensor.getRZero();
    //Serial.println(rzero);

    //float ppm = gasSensor.getPPM();
    //Serial.print(F("Gas ppm: "));
    //Serial.println(ppm);


    //adc_value = analogRead(34);
    Serial.print(F("Gas ADC: "));
    Serial.println(adc_value);

    Serial.print(F("Gas ppm: "));
    Serial.println(co2ppm);



  Serial.print(F("Umidade: "));
  Serial.print(umidade);
  Serial.print(F("%  Temperatura: "));
  Serial.println(temperatura);

  Serial.println();
}