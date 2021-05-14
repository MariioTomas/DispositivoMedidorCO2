//DECLARACIÓN DE VARIABLES GLOBALES
float rl = 20.0; //RL = 20.0 K Ohms. Resistencia del módulo.
float rs; //RS en K Ohms. Resistencia del sensor. Se calculará para calibrar el sistema.
float ppm; //Cantidad de PPM's de CO2 en el ambiente. Valor de ambiente ventilado = 413.95ppm
float roa; 
float rob; 
float adc; //Dato crudo,  Analógico (Entre 0-1024).

int adcConst = 60; //voltaje analógico predeterminado. Utilizado posteriormente en el condicional.

void setup() 
{
  
pinMode(A0,INPUT); //Entrada analógica sensor MQ135
pinMode(7, INPUT_PULLUP); //Entrada del botón para calibrar el sistema.
pinMode(2,OUTPUT); //Led verde.
pinMode(3,OUTPUT); //Led amarillo.
pinMode(4,OUTPUT); //Led rojo.
Serial.begin(9600);
rob=1074.35; //Inicialización de la variable R0.
}

void loop() 
{
//Adquisición y muestreo del valor captado por el pin de Arduino.
adc = analogRead(A0);
Serial.print(adc);
Serial.print(" v - ");

//Cálculo de la resistencia del sensor.
rs= 1024*(rl/adc)-rl;
Serial.print(rs);
Serial.print(" rs - ");

//Si el botón es presionado, calibrar generado la resistencia R0.
if(digitalRead(7)==0)
{
roa=5.59*pow(413.95, -0.36);
rob= rs/roa;
}

Serial.print(rob);
Serial.print(" R0 - ");

//Calculo de las PPM de CO2 del ambiente, aplicando RS y R0.
ppm = pow((((1024*(20/adc)-20)/rob)/5.59), 1/-0.36);
Serial.print(ppm);
Serial.println(" ppm");

//Si se encuentra un valor de voltaje analógico menor al predeterminado, autocalibrar el sistema.
if(adcConst > adc)
  {
  Serial.println("CALIBRADO");
  adcConst= adc;
  roa=5.59*pow(413.95, -0.36);
  rob= rs/roa; 
  }

//Condicional de aviso de descalibración. (Todos los leds encendidos).
if(ppm<320)
{
  Serial.println("DESCALIBRADO");

  //Enciende todos los leds.
  digitalWrite(4, HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(2,HIGH);  
}

//Condicional de aviso de habitación ventilada. (Led verde encendido).
if(ppm>320 and ppm<500)
{
  Serial.println("AIRE LIMPIO");
  
  //Enciende solo el led verde.
  digitalWrite(4,HIGH);
  digitalWrite(3,LOW);
  digitalWrite(2,LOW);  
}
//Condicional de aviso de habitación parcialmente con CO2. (Led amarillo encendido).
if(ppm>500 and ppm<1000)
{
  Serial.println("AIRE PARCIALMENTE LIMPIO");

  //Enciende solo el led amarillo/naranja.
  digitalWrite(4,LOW);
  digitalWrite(3,HIGH);
  digitalWrite(2,LOW);  
}
//Condicional de aviso de habitación completamente con CO2. (Led rojo encendido).

if(ppm>1000)
{
  Serial.println("AIRE CON ALTO CO2");
  
  //Enciende solo el led rojo.
  digitalWrite(4,LOW);
  digitalWrite(3,LOW);
  digitalWrite(2,HIGH);  
}

delay(500);
}
