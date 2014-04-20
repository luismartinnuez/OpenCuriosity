
#ifndef brazo_h

#define brazo_h
#include <stdio.h>


#define tempo 1

#define PI_7_4	7*3.1415926535/4  	//315º
#define PI_3_2	3*3.1415926535/2  	//270º
#define PI_5_4	5*3.1415926535/4  	//225º
#define PI 		3.1415926535		//180º 
#define PI_3_4	3*3.1415926535/4	//135º 
#define PI_2_3	2*3.1415926535/3	//120º 
#define PI_7_12	7*3.1415926535/12	//105º
#define PI_2 	3.1415926535/2   	//90º
#define PI_4	3.1415926535/4		//45º
#define PI_1_6	3.1415926535/6		//30º
#define PI_1_12	3.1415926535/12		//15º

// Número de servo para el controlador 
#define eje_0 0		  
#define eje_1 1
#define eje_2 2
#define eje_3 3
#define eje_4 4

// Ángulo de desviación
#define desv_0 25
#define desv_1 0.1188
#define desv_2 0.104
#define desv_3 PI_1_12

//Distancia del brazo 1
#define D1 125.89

//Distancia del brazo 2
#define D2 95.52

//Valores de home
#define home_0	-PI_2    
#define home_1	-0.698
#define home_2	-PI_7_12

//Valores de home
#define start_0	-PI_4    
#define start_1	PI_4
#define start_2	-PI_2

//Valores de offset para las coordenadas
#define x_0		0   
#define y_0		0
#define z_0		0

// Límites de los motores
#define eje_0_min	0 
#define eje_0_max	PI_3_4   
#define eje_1_min	-PI_3_4 
#define eje_1_max	PI_3_4
#define eje_2_min	-PI_3_4 
#define eje_2_max	PI_4
#define eje_3_min	-PI_3_4  
#define eje_3_max	PI_3_4

// Valores directos de los servos para posicionar la herramienta en el servo 4
#define herramienta1	727		// Temperatura
#define herramienta2	1388	// Laser
#define herramienta3	2280	// Taladro



class brazo
{
	
	private:	

	float h, h0, h1, h2;
	float theta0;
	float theta1, theta11, theta12;
	float theta2, theta21, theta22; 
	float theta3; 
	
	void set_angle(unsigned char servo, float angle_rad);
	void set_speed(unsigned char servo, unsigned int spd);
	void set_accel(unsigned char servo, unsigned int accel);
	float map_f(float x, float in_min, float in_max, float out_min, float out_max);


 
	public:
	
	brazo();
	
	void go_home();
	void go_start();
	void init();
	
	void set_leg(	int8_t n_brazo,
					int8_t servo_0,
					int8_t servo_1,
					int8_t servo_2,
					int8_t servo_3,
					int8_t servo_4,
					int8_t motor);
					
	void modelo_inverso(float px,
						float py,
						float pz,
						float ang_herramienta);
	
	void linea_recta(	float x_ini,
						float y_ini,
						float z_ini,
						float x_fin,
						float y_fin,
						float z_fin,
						float ang_herramienta,
						int velocidad);
	
	void carga_herramienta(uint8_t herramienta);

};

//extern PataClass Pata;

#endif

