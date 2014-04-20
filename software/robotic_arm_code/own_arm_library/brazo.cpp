
#include <Arduino.h> 

#include "brazo.h"

brazo::brazo()
{
}




void brazo::set_angle(unsigned char servo, float angle_rad)
{
	
	unsigned int pulso;
	
	pulso = 1000 + (angle_rad * (1000 / 3.1415));
	Serial.print("Pulso 1: ");
	Serial.println(pulso, DEC);
	pulso = pulso * 4;     
	
	
    Serial3.write(0xAA); //start byte
    Serial3.write(0x0C); //device id
    Serial3.write(0x04); //command "Set Target"
    Serial3.write(servo); //servo number
    Serial3.write(pulso & 0x7F);
    Serial3.write((pulso >> 7) & 0x7F);  
}

void brazo::set_speed(unsigned char servo, unsigned int spd)
{
    Serial3.write(0xAA); //start byte
    Serial3.write(0x0C); //device id
    Serial3.write(0x07); //command "Set Speed"
    Serial3.write(servo); //servo number
    Serial3.write(spd & 0x7F);
    Serial3.write((spd >> 7) & 0x7F);   
}

void brazo::set_accel(unsigned char servo, unsigned int accel)
{
    Serial3.write(0xAA); //start byte
    Serial3.write(0x0C); //device id
    Serial3.write(0x09); //command "Set Acceleration"
    Serial3.write(servo); //servo number
    Serial3.write(accel & 0x7F);
    Serial3.write((accel >> 7) & 0x7F);    
}

void brazo::init()
{

	Serial3.begin(9600); //Puerto serie 3 para los servos
	
	set_speed(eje_0, 10);
	set_speed(eje_1, 10);
	set_speed(eje_2, 10);
	set_speed(eje_3, 10); 
	set_speed(eje_4, 10);

	set_accel(eje_0, 0);
	set_accel(eje_1, 0);
	set_accel(eje_2, 0);
	set_accel(eje_3, 0); 
	set_accel(eje_4, 0);  	
	
/*	set_angle(eje_0, home_0);
	set_angle(eje_1, home_1);
	set_angle(eje_2, home_2);
	set_angle(eje_3, home_3);*/ 
}

void brazo::go_home()
{
	
/*	Serial.println("Go home");   
	Serial.print("Theta 0: ");
	Serial.print(home_0 * 360 /6.283); 
	Serial.print(";  Theta 1: ");
	Serial.print(home_1 * 360 /6.283);
	Serial.print(";  Theta 2: ");
	Serial.println(home_2 * 360 /6.283); */
	            
	theta3 = -home_1 + home_2;
	theta0 = map_f(home_0, -PI_2, PI_2, -PI_4, PI); 
	theta1 = map_f(home_1, PI_2, -PI_2, -PI_2, PI_3_2);  
	theta2 = map_f(home_2, -PI_3_4, PI_4, -PI_2, PI_3_2);   
	theta3 = map_f(theta3, -PI, PI, PI_3_2, PI_2);

/*	Serial.print("Theta 0: ");
	Serial.print(theta0 * 360 /6.283); 
	Serial.print(";  Theta 1: ");
	Serial.print(theta1 * 360 /6.283);
	Serial.print(";  Theta 2: ");
	Serial.println(theta2 * 360 /6.283);  */

	set_angle(eje_3, theta3); 
	delay(3000);    
	set_angle(eje_2, theta2);
	delay(3000);    
	set_angle(eje_1, theta1);
	delay(3000);
	set_angle(eje_0, theta0);
	
}

void brazo::go_start()
{
	

	
	theta0 = map_f(start_0, -PI_2, PI_2, 0, PI); 
	theta1 = map_f(start_1, PI_2, -PI_2, -PI_2, PI_3_2);  
	theta2 = map_f(start_2, -PI_3_4, PI_4, -PI_2, PI_3_2);
	theta3 = /*-PI_2 +*/ theta1;

                             
	set_angle(eje_0, theta0); 
	delay(5000);             
	set_angle(eje_1, theta1);
	delay(3000);    
	set_angle(eje_2, theta2);
	delay(3000);        
	set_angle(eje_3, theta3);
	
}

float brazo::map_f(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



//Modelo inverso para superficies sin inclianción

void brazo::modelo_inverso(float px, float py, float pz, float ang_herramienta)
{

	
	h = sqrt(((px - x_0) * (px - x_0)) + ((py - y_0) * (py - y_0)) + ((pz - z_0) * (pz - z_0)));
	h1 = ((h * h) - (D2 * D2) + (D1 * D1)) / (2 * h);
	h2 = h - h1;
	h0 = sqrt(((px - x_0) * (px - x_0)) + ((py - y_0) * (py - y_0)));
	
	theta0 = acos(-(py - y_0) / (h0)) - PI_2;   
	
	if (h1 < D1)
	{
		theta21 = asin(h1 / D1); 
	}
	else
	{
		theta21 = 3.1415 / 2;
	}
	
	if (h2 < D2)
	{
		theta22 = asin(h2 / D2);  
	}
	else
	{
		theta22 = 3.1415 / 2;
	}
	
	theta2 = -3.1415 + (theta21 + theta22);

	theta12 = (3.1415 / 2) - theta21;
	theta11 = asin((pz - z_0) / h);
	theta1 = -theta11 + theta12; 
	
	theta3 = -theta1 + theta2 - ang_herramienta;	
	
	/*Serial.print("h: ");
	Serial.print(h);      
	Serial.print(";  h1: ");
	Serial.print(h1);     
	Serial.print(";  h2: ");
	Serial.println(h2);
	Serial.print("Theta 0: ");
	Serial.println(theta0 * 360 /6.283); 
	Serial.print("Theta 1: ");
	Serial.print(theta1 * 360 /6.283);
	Serial.print(";  Theta 11: ");
	Serial.print(theta11 * 360 /6.283);
	Serial.print(";  Theta 12: ");
	Serial.println(theta12 * 360 /6.283);
	Serial.print("Theta 2: ");
	Serial.print(theta2 * 360 /6.283); 
	Serial.print(";  Theta 21: ");
	Serial.print(theta21 * 360 /6.283);
	Serial.print(";  Theta 22: ");
	Serial.println(theta22 * 360 /6.283); 
	Serial.print("Theta 3: ");
	Serial.println(theta3 * 360 /6.283);  */
	
	// Se ajustan los ejes (el 3 y el 4 no necesitan ajuste)
	theta0 = theta0 - asin(desv_0 / h0);
	theta1 = theta1 - desv_1;
	theta2 = theta2 + desv_2;  
	theta3 = theta3 - desv_3; 
	
	// Limitación de los ángulos generados
	theta0 = constrain(theta0, eje_0_min, eje_0_max);  
	theta1 = constrain(theta1, eje_1_min, eje_1_max);
	theta2 = constrain(theta2, eje_2_min, eje_2_max); 
	theta3 = constrain(theta3, eje_3_min, eje_3_max); 
	
	// Se preparan los ángulos para los servos
	// Uso de map adaptada para usar floats          
	theta0 = map_f(theta0, -PI_2, PI_2, 0, PI); 
	theta1 = map_f(theta1, PI_2, -PI_2, -PI_2, PI_3_2);  
	theta2 = map_f(theta2, -PI_3_4, PI_4, -PI_2, PI_3_2); 
	theta3 = map_f(theta3, -PI, PI, PI_3_2, -PI_2);

	
	/*Serial.print("Theta 0: ");
	Serial.print(theta0 * 360 /6.283); 
	Serial.print(";  Theta 1: ");
	Serial.print(theta1 * 360 /6.283);
	Serial.print(";  Theta 2: ");
	Serial.print(theta2 * 360 /6.283); 
	Serial.print(";  Theta 3: ");
	Serial.println(theta3 * 360 /6.283);*/ 
	
	// Mandar los datos al gestor de los servos
	set_angle(eje_0, theta0); 
	set_angle(eje_1, theta1);
	set_angle(eje_2, theta2); 
	set_angle(eje_3, theta3);
  
}


//Genera una línea recta y obtiene el modelo inverso para todos los puntos de la trayectoria
void brazo::linea_recta(	float x_ini,
							float y_ini,
							float z_ini,
							float x_fin,
							float y_fin,
							float z_fin,
							float ang_herramienta,
							int velocidad)
{

	float total_distance, l;
	float alfa, beta, gamma;
	float x, y, z;
	unsigned long total_steps, steps;
	unsigned long previous;
	
	total_distance = sqrt((x_fin - x_ini) * (x_fin - x_ini) + (y_fin - y_ini) * (y_fin - y_ini) + (z_fin - z_ini) * (z_fin - z_ini));
	l = 1;
	total_steps = total_distance / l;

	//NO usar funciones POW, consumen m�s ROM!!!

	alfa = atan2( (x_fin - x_ini), sqrt((y_fin - y_ini) * (y_fin - y_ini) + (z_fin - z_ini) * (z_fin - z_ini)));
	beta = atan2( (y_fin - y_ini), sqrt((x_fin - x_ini) * (x_fin - x_ini) + (z_fin - z_ini) * (z_fin - z_ini)));
	gamma = atan2( (z_fin - z_ini), sqrt((x_fin - x_ini) * (x_fin - x_ini) + (y_fin - y_ini) * (y_fin - y_ini)));
	
	/* USB.println("Funcion de linea recta");  
	USB.print("numero de pasos: "); 
	USB.println(total_steps,DEC);*/

	for(steps = 0; steps < total_steps; steps++)
	{
    	x=(sin(alfa) * l * steps) + x_ini;   
		y=(sin(beta) * l * steps) + y_ini;
		z=(sin(gamma) * l * steps) + z_ini;
		modelo_inverso( x, y, z, ang_herramienta);
		delay(256 - velocidad);
	}

	modelo_inverso(x_fin, y_fin, z_fin, ang_herramienta);
  
}


void brazo::carga_herramienta(uint8_t herramienta)
{  	
	Serial3.write(0xAA); //start byte
	Serial3.write(0x0C); //device id
	Serial3.write(0x04); //command "Set Target"
	Serial3.write(eje_4); //servo number
	switch(herramienta)
	{
		case 1: 
			Serial3.write((herramienta1 * 4) & 0x7F);
			Serial3.write(((herramienta1 * 4) >> 7) & 0x7F);
			break;    
		case 2: 
			Serial3.write((herramienta2 * 4) & 0x7F);
			Serial3.write(((herramienta2 * 4) >> 7) & 0x7F);
			break;
		case 3: 
			Serial3.write((herramienta3 * 4) & 0x7F);
			Serial3.write(((herramienta3 * 4) >> 7) & 0x7F);
			break;
	}   			
}

//PataClass Pata;
