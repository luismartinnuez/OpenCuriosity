

#include <Sabertooth.h>
#include <Wire.h>
#include <brazo.h>
#define BMP085_ADDRESS 0x77 
const unsigned char OSS = 0;

brazo brazo1;

Sabertooth ST(128);

// Calibration values BMP085
int ac1;
int ac2;
int ac3;
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1;
int b2;
int mb;
int mc;
int md;
long b5; 

const int laser = 9;
const int pos1 = 2;
const int pos2 = 3;
const int pos3 = 4;
const int pos4 = 5;
const int pos5 = 6;
const int pos6 = 7;

const int pin_laser = 10;
const int pin_taladro = 8;

void setup()         
{
  Serial2.begin(9600);              
  Serial3.begin(9600);               
  SabertoothTXPinSerial.begin(9600); 
  ST.autobaud(); 
  ST.command(16, 11);               
  
  Wire.begin();
  bmp085Calibration();
  
  pinMode(laser, OUTPUT);          
  pinMode(pos1, INPUT);
  pinMode(pos2, INPUT);
  pinMode(pos3, INPUT);
  pinMode(pos4, INPUT);
  pinMode(pin_taladro, OUTPUT);
  pinMode(pin_laser, OUTPUT);
  
  digitalWrite (pin_taladro, LOW);
  digitalWrite (laser, LOW);
  digitalWrite (pin_laser, LOW);
  
  delay(100); 
}

void set_target(unsigned char servo, unsigned int target){
    Serial3.write(0xAA); //start byte
    Serial3.write(0x0C); //device id
    Serial3.write(0x04); //command "Set Target"
    Serial3.write(servo); //servo number
    Serial3.write(target & 0x7F);
    Serial3.write((target >> 7) & 0x7F);
}

void set_speed(unsigned char servo, unsigned int spd){
    Serial3.write(0xAA); //start byte
    Serial3.write(0x0C); //device id
    Serial3.write(0x07); //command "Set Speed"
    Serial3.write(servo); //servo number
    Serial3.write(spd & 0x7F);
    Serial3.write((spd >> 7) & 0x7F);
}

void set_accel(unsigned char servo, unsigned int accel){
    Serial3.write(0xAA); //start byte
    Serial3.write(0x0C); //device id
    Serial3.write(0x09); //command "Set Acceleration"
    Serial3.write(servo); //servo number
    Serial3.write(accel & 0x7F);
    Serial3.write((accel >> 7) & 0x7F);
}

void go_home(){
    Serial3.write(0xAA); //start byte
    Serial3.write(0x0C); //device id
    Serial3.write(0x22); //command "Go Home"
}

void rueda_alineada(){
   set_target(8, 5840);
   set_target(9, 6048);
   set_target(10, 6048);
   set_target(11, 6048);
   Serial2.print("Line Wheels");
   Serial2.println();
   delay(5000);
}   
void giro_rueda(){
  set_target(8, 4288);
  set_target(9, 7808);
  set_target(10, 7808);
  set_target(11, 4288);
  Serial2.print("Turning Wheels");
  Serial2.println();
  delay(5000);
}

void cabeza_centro(){
  set_target(7, 5696);
  set_target(6, 6400);
  delay(2000);
}

void dis_laser(){
  digitalWrite(laser, HIGH);
  delay (15000);
  digitalWrite(laser, LOW);
}

void loop()
{
  
  float temperature = bmp085GetTemperature(bmp085ReadUT());
  float pressure = bmp085GetPressure(bmp085ReadUP());
  float atm = pressure / 101325; 
  float altitude = calcAltitude(pressure); 

  Serial2.print("Temperature: ");
  Serial2.print(temperature, 2); 
  Serial2.println("deg C");

  Serial2.print("Pressure: ");
  Serial2.print(pressure, 0); 
  Serial2.println(" Pa");

  Serial2.print("Standard Atmosphere: ");
  Serial2.println(atm, 4); 

  Serial2.print("Altitude: ");
  Serial2.print(altitude, 2); 
  Serial2.println(" M");

  Serial2.println();
  
   set_speed(5,15); 
   set_speed(6,15); 
   set_speed(7,15); 
   set_speed(8,10);
   set_speed(9,10);
   set_speed(10,10);
   set_speed(11,10);
   
   set_accel(8,0);
   set_accel(9,0);
   set_accel(10,0);
   set_accel(11,0);
   
   cabeza_centro();
   rueda_alineada();
   //giro_rueda();
   //rueda_alineada();

   
   ST.command(1, 50); 
   ST.command(5, 50); 
   delay (5000);
   
   //TO MOVE HEAD LEFT-RIGHT
   set_target(6, 3300);
   delay(3000);
   set_target(6, 8900);
   delay(3000);
   set_target(6, 6400);
   delay(8000);
  
   //ST.command(0, 50); //motor 1 backward speed 50
   //ST.command(4, 50); //motor 2 backward speed 50
   //delay(10000);
   
   //Curiosity STOP
   ST.command(0, 0); 
   ST.command(4, 0); 
   delay(5000);
    
   giro_rueda();
 
   //Curiosity TURN LEFT
   ST.command(0, 50); 
   ST.command(5, 50); 
   delay(10000);
   
   //Curiosity STOP
   ST.command(0, 0); 
   ST.command(4, 0); 
   delay(5000);
   
   //Curiosity TURN RIGHT
   ST.command(1, 50); 
   ST.command(4, 50); 
   delay(10000);
   
   //Curiosity STOP
   ST.command(0, 0); 
   ST.command(4, 0);
   delay(5000);
   
   rueda_alineada();
     
   //TO MOVE HEAD UP
   set_target(7, 6800);
   delay(5000);
   
   //TO MOVE HEAD DOWN
   set_target(7, 4400);
   delay(3000);
   
   dis_laser();
    
   //TO MOVE HEAD CENTER
   set_target(7, 5696);
   delay(5000); 
    
   
   //OPEN CLOSED SCUTTLE
   set_target(5, 2496);
   delay(4000);
   set_target(5, 8192);
   delay(4000);
   set_target(5, 2496);
   delay(4000);
   
   //ARM/////////////////////////////////////
   
   pinMode(pin_laser, OUTPUT);
	digitalWrite(pin_laser, LOW); 
	pinMode(pin_taladro, OUTPUT);
	digitalWrite(pin_taladro, LOW);
	   
	//Serial.begin(115200);
	brazo1.init();
	//delay(5000);
	Serial2.begin(9600); 
	/*Serial2.println("1");
	brazo1.modelo_inverso(125.89, 0, 95.52, 0); 
	delay(5000); 
	Serial2.println("2");
	brazo1.modelo_inverso(110, -110, -100, 0);
	delay(5000);  */  
	Serial2.println("IDLE ARM"); 
	brazo1.go_home();   
	delay(7000);          
	Serial2.println("ARM START POSITION");
	brazo1.go_start();   
	delay(7000);  
	Serial2.println("TEMPERATURE SENSOR"); 
	brazo1.carga_herramienta(1);
	delay(7000);
	Serial2.println("DRILL");
	brazo1.carga_herramienta(3);
	delay(7000); 
	Serial2.println("LASER POINTER");        
	brazo1.carga_herramienta(2);
	delay(7000);      
	Serial2.println("LASER POINTER POSITION");
	brazo1.modelo_inverso(130, -30, 0, -PI_2); 
	digitalWrite(pin_laser, HIGH); 
	delay(7000);            
	digitalWrite(pin_laser, LOW); 
	Serial2.println("TEMPERATURE SENSOR"); 
	brazo1.carga_herramienta(1);
	delay(7000);     
	Serial2.println("TEMPERATURE SENSOR POSITION");
	brazo1.linea_recta(150, -50, 0, 150, -50, -20, -PI_2, 100); 
	delay(7000); 
	Serial2.println("DRILL POSITION");
	brazo1.carga_herramienta(3);
	delay(7000);         
	Serial2.println("DRILL START"); 
	digitalWrite(pin_taladro, HIGH);
	Serial2.println("DOWN DRILL"); 	
	brazo1.linea_recta(150, -50, -20, 150, -50, -80, -PI_2, 100);  
	delay(5000);    	   
	Serial2.println("UP DRILL"); 
	brazo1.linea_recta(150, -50, -80, 150, -50, -20, -PI_2, 100);  
	delay(2000);   
	Serial2.println("STOP DRILL"); 
	digitalWrite(pin_taladro, LOW); 	
	Serial2.println("ARM TO REST"); 
	brazo1.go_home();
   
}

// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

// Calculate temperature in deg C
float bmp085GetTemperature(unsigned int ut){
  long x1, x2;

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  float temp = ((b5 + 8)>>4);
  temp = temp /10;

  return temp;
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up){
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;

  b6 = b5 - 4000;
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p = p + ((x1 + x2 + 3791)>>4);

  return p;
}

// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
  unsigned char data;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
    ;

  return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();

  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT(){
  unsigned int ut;

  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();

  // Wait at least 4.5ms
  delay(5);

  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP(){

  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;

  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();

  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));

  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  msb = bmp085Read(0xF6);
  lsb = bmp085Read(0xF7);
  xlsb = bmp085Read(0xF8);

  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);

  return up;
}

void writeRegister(int deviceAddress, byte address, byte val) {
  Wire.beginTransmission(deviceAddress); // start transmission to device 
  Wire.write(address);                   // send register address
  Wire.write(val);                       // send value to write
  Wire.endTransmission();                // end transmission
}

int readRegister(int deviceAddress, byte address){

  int v;
  Wire.beginTransmission(deviceAddress);
  Wire.write(address);                   // register to read
  Wire.endTransmission();

  Wire.requestFrom(deviceAddress, 1);    // read a byte

  while(!Wire.available()) {
    // waiting
  }

  v = Wire.read();
  return v;
}

float calcAltitude(float pressure){

  float A = pressure/101325;
  float B = 1/5.25588;
  float C = pow(A,B);
  C = 44330 * (1 - C);
  
  return C;
}

