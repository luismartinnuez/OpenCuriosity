#include <brazo.h>

const int pin_laser = 8;
const int pin_taladro = 8;

brazo brazo1;


void setup()
{           
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
	Serial2.println("Brazo a resposo"); 
	brazo1.go_home();   
	delay(7000);          
	Serial2.println("Sacando brazo a posición de comienzo");
	brazo1.go_start();   
	delay(7000);  
	Serial2.println("Temperatura"); 
	brazo1.carga_herramienta(1);
	delay(7000);
	Serial2.println("Taladro");
	brazo1.carga_herramienta(3);
	delay(7000); 
	Serial2.println("Laser");        
	brazo1.carga_herramienta(2);
	delay(7000);      
	Serial2.println("Posición para el laser");
	brazo1.modelo_inverso(130, -30, 0, -PI_2); 
	//digitalWrite(pin_laser, HIGH); 
	delay(7000);            
	//digitalWrite(pin_laser, LOW); 
	Serial2.println("Temperatura"); 
	brazo1.carga_herramienta(1);
	delay(7000);     
	Serial2.println("Posición para temperatura");
	brazo1.linea_recta(150, -50, 0, 150, -50, -20, -PI_2, 100); 
	delay(7000); 
	Serial2.println("Taladro");
	brazo1.carga_herramienta(3);
	delay(7000);         
	Serial2.println("Activa el taladro"); 
	digitalWrite(pin_taladro, HIGH);
	Serial2.println("Baja el taladro"); 	
	brazo1.linea_recta(150, -50, -20, 150, -50, -80, -PI_2, 100);  
	delay(5000);    	   
	Serial2.println("Sube el taladro"); 
	brazo1.linea_recta(150, -50, -80, 150, -50, -20, -PI_2, 100);  
	delay(2000);   
	Serial2.println("Apago el taladro"); 
	digitalWrite(pin_taladro, LOW); 	
	Serial2.println("Brazo a resposo"); 
	brazo1.go_home();  
	  

}

void loop()
{           
}
