////////////////////////////////////////////////////////////
////////Firmware de vuelo para Quadricoptero- Completo////////////////
///////Ilyass Rezquellah/////////////////
////////incluye Giroscopio y control via puerto/////////////
////////////////////////////////////////////////////////////
#include <Servo.h> //Incluye la libreria servo para control de los motores Brushless
Servo mot;         //Declara estos objetos como Servos
Servo mot2;
Servo mot3;
Servo mot4;

const int pinServo = 9;  //Establece los pines para los servos
const int pinServo2 = 10;
const int pinServo3 = 11;
const int pinServo4 = 12;


int angulo;              //Variable que controla la velocidad va de 0 a 180
int pulsoMin = 1000;     //Pulso minimo y maximo dependiendo del ESC
int pulsoMax = 2000;
int estado = 'a';        //inicia los motores en 0

//Pines de lectura analogica
const int xPin = 0;
const int yPin = 1;
const int zPin = 2;


//los valores minimo y maximo vienen del
//acelerometro cuando esta quieto
//para correcto funcionamiento deberas cambiar esto
int minVal = 333;
int maxVal = 405;
int a;
int angulo2;     //Valores para el control individual de cada motor
int angulo3;
int angulo4;
int angulo5;


//para guardar los valores calculados
double x;
double y;
double z;
double xmax; //Variables para guardar los valores de calibracion 
double xmin;
double ymax;
double ymin;
double zmax;
double zmin;


void setup() {
  mot.attach(pinServo, pulsoMin, pulsoMax);  //Fija las propiedades a cada motor
  mot2.attach(pinServo2, pulsoMin, pulsoMax);
  mot3.attach(pinServo3, pulsoMin, pulsoMax);
  mot4.attach(pinServo4, pulsoMin, pulsoMax);
  Serial.begin(9600);
}

void loop() {
 


  //Lee los velores analogicos de el acelerometro
  int xRead = analogRead(xPin);
  int yRead = analogRead(yPin);
  int zRead = analogRead(zPin);

  //Convertir los valores leidos a grados -90 to 90 - Nescesario para atan2
  int xAng = map(xRead, minVal, maxVal, -90, 90);
  int yAng = map(yRead, minVal, maxVal, -90, 90);
  int zAng = map(zRead, minVal, maxVal, -90, 90);

  //Calcula los valores de 360 grados entonces: atan2(-yAng, -zAng)
  //atan2 muestra los valores de  -π to π (radianes)
  //convierte radianes a grados
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);



   
   if (a==0){    //Para guardar los valores de calibracion y que no sean modificados
     
     xmax = x; 
     xmin = x;
     ymax = y;
     ymin = y;
     zmax = z;
     zmin = z;
     a = 2;
     
   }
  

 
   
   if (x==xmax && y==ymax && z==zmax){  //Detecta que el drone esta volando estable
  mot.write(angulo);
  mot2.write(angulo);
  mot3.write(angulo);
  mot4.write(angulo);
   }
     
   if (y<ymin &&  z>zmax ){            //Detecta y corrige una inclinacion hacia mot4
     angulo2=angulo;
     angulo2+4;
     mot4.write(angulo2);
   }
    if (x<xmax && y<ymin && z<zmin){  //Detecta y corrige una inclinacion hacia mot3
     angulo3=angulo;
      angulo3+4;
     mot3.write(angulo3);
   }
    if (x>xmin && y>ymax && z>zmax){ //Detecta y corrige una inclinacion hacia mot2
      angulo4=angulo;
     angulo4+4;
     mot2.write(angulo4);
   }
     if ( y>ymax && z<zmin){         //Detecta y corrige una inclinacion hacia mot2
       angulo5=angulo;
    angulo5+4;
     mot.write(angulo5);
   }
  delay(100);
     
  if(Serial.available()>0){
    estado = Serial.read();
  //Lee los valores que le mandamos por bluetooth
  
  }
  
  if (estado=='b'){            //Detiene los motores
    
  angulo=0;
  mot.write(angulo);    
  mot2.write(angulo);  
  mot3.write(angulo);  
  mot4.write(angulo);
  
  }
  if (estado=='a'){           //Aumenta la velocidad de los motores
    ++angulo;
    mot.write(angulo);    
    mot2.write(angulo);  
    mot3.write(angulo);  
    mot4.write(angulo);
  }
  if (estado=='c'){          //Disminuye la velocidad de los motores
    --angulo;
    mot.write(angulo);    
    mot2.write(angulo);  
    mot3.write(angulo);  
    mot4.write(angulo);
    
  }
  if (estado=='d'){          //giro sobre tu propio eje a la izquierda
    angulo4=angulo;
    angulo3=angulo;
    angulo4+6;
    angulo3+6;  
    mot2.write(angulo4);  
    mot3.write(angulo3);  
    delay(1000);
    
  }
   if (estado=='e'){          //giro sobre tu propio eje a la derecha
    angulo2=angulo;
    angulo5=angulo;
    angulo2+6;
    angulo5+6;  
    mot.write(angulo5);  
    mot4.write(angulo2);  
    delay(1000);
    
  }
    if (estado=='f'){          //reversa
    angulo3=angulo;
    angulo2=angulo;
    angulo3+6;
    angulo2+6;  
    mot3.write(angulo3);  
    mot4.write(angulo2);  
    delay(1000);
    
  }
     if (estado=='h'){          //hacia el frente
    angulo5=angulo;
    angulo4=angulo;
    angulo5+6;
    angulo4+6;  
    mot.write(angulo5);  
    mot2.write(angulo4);  
    delay(1000);
    
  }
     if (estado=='i'){          //ladeo izquierdo
    angulo5=angulo;
    angulo3=angulo;
    angulo5+6;
    angulo3+6;  
    mot.write(angulo5);  
    mot3.write(angulo3);  
    delay(1000);
    
  }
     if (estado=='j'){          //ladeo derecho
    angulo4=angulo;
    angulo2=angulo;
    angulo5+6;
    angulo3+6;  
    mot2.write(angulo4);  
    mot4.write(angulo2);  
    delay(1000);
    
  }
  
  estado='z';              //Para no trabar el ESC
  
 
}
