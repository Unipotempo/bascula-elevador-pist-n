#include "HX711.h"                                                        // la libreria la puedes instalar en el IDE de arduino desde aqui: http://librarymanager/All#Avia_HX711
#include <Servo.h>                                                        // libreria para los servomotores

#define elevatorServoPin 9                                                // Pinout del servomotor del elevador
#define pusherServoPin 10                                                 // Pinout del servomotor del pistón
                             
#define LOADCELL_DOUT_PIN 7                                               // pin DT del modulo HX711
#define LOADCELL_SCK_PIN 8                                                // pin SCK del modulo HX711

float calibration_factor = -500;                                          // ajustar el valor de calibracion segun cada proyecto, el valor correcto es cuando la bascula da 0 
float weight;                                                             // variable donde se guarda la lectura de la bascula
int piso;                                                                 // variable donde se guarda el piso a donde ira el elevador
int angulo;                                                               // variable que guarda si el servo gira a la izquierda, a la derecha o no gira y a que velocidad

HX711 scale;                                                              // se declaran los objetos bascula

Servo elevatorServo;                                                      // se declaran los objetos Servo
Servo pusherServo;

void setup() {
  Serial.begin(9600);                                                     // configurar el monitor serial

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);                       // configurar la bascula
  scale.set_scale();
  scale.tare();

  elevatorServo.attach(elevatorServoPin);                                 // configurar los servomotores
  pusherServo.attach(pusherServoPin);
 
}

void loop() {
  // Leer el peso de la celda de carga
  scale.set_scale(calibration_factor);                                    // Configurar el factor de calibración

  weight = scale.get_units(), 1;                                         
  if (weight < 0){
    weight = 0.00;
  }

  Serial.print("peso: "); Serial.print(weight); Serial.println("Gramos"); 

  // asignar el piso segun el peso  
  if (weight > 10){                                                       // el peso debe sobrepasar los 10 gr para poder activar el elevador
    if (weight < 100) {                                                   // menos de 100 gr - planta baja
      piso = 0;                                                           
    } else if (weight >= 100 && weight < 200) {                           // de 100 gr a menos de 200 gr - piso 1
      piso = 1;
    } else if (weight >= 200 && weight < 300) {                           // de 200 gr a menos de 300 gr - piso 2
      piso = 2;
    } else {                                                              //de 300 gr en adelante - piso 3
      piso = 3;
    }

    if(piso == 0){
      angulo = 90;                                                        // el servo se detiene
    } else {
      angulo = 180;                                                       // el servo avanza 
    }
    
    Serial.print("piso: "); Serial.println(piso);

    // rutina del elevador 
    elevatorServo.write(180);                                              // sube el elevador o no   angulo
    Serial.print("angulo: "); Serial.println(angulo);
    Serial.println("subiendo...");
    delay(1000);                                                          // tiempoDelay = piso * tiempo,    
    elevatorServo.write(90);                                             // el elevador se detiene
    delay(1000); 

    pusherServo.write(180);                                               // empujar el objeto
    Serial.println("empujando...");
    delay(500);

    pusherServo.write(0);                                                 // retorcene el pistón 
    Serial.println("regresando...");
    delay(500);
    pusherServo.write(90);                                                 // el piston se detiene
    delay(1000); 

    elevatorServo.write(0);                                              // baja el elevador
    Serial.println("bajamdo...");
    delay(2000);
    

  }

}


