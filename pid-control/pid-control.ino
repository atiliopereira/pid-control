
int ref = 20;                             // distancia de referencia
long longitud = 63;                       // longitud del tubo

double U = 0;                               // Esfuerzo de control
double kp = 3.21;                           // Constante Proporcional
double ki = 0.64;                          // Constante Integrativa ki = 0.64
double kd = 0.4;                             // Constante Derivativa kd = 0.4
double tm = 0.025;                          // tiempo de muestreo

long distanciaAnterior = 0;
volatile unsigned long tiempoAnterior;
double errorAnterior = 0;

int trigPin = 12;                         // Pin disparador. Se puede usar otro pin digital
int echoPin = 11;                         // Pin eco. Se puede usar otro pin digital
long distancia = 20;                           // Distancia del sensor
long duracion;                            // tiempo medido del sensor

long dT;                                  // Diferencia de medidas de tiempo
int error = 0;                            // Error lineal
double errorI = 0;                        // Error integrativo
double errorD = 0;                        // Error derivativo

bool bandera = 1;
int contador = 0;


void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);                 // Establece pin como salida
  pinMode(echoPin, INPUT);                  // Establece pin como entrada
  digitalWrite(trigPin, LOW);               // Pone el pin a un estado logico 
  tiempoAnterior=0;
}


void loop() {
  //Secuencia de inicio
  while(bandera == 1){
    U = U + 3;
    if (U >= 175) {
      while(contador < 300){
        analogWrite(9,U);
        delay(5);
        digitalWrite(trigPin, HIGH); 
        delayMicroseconds(10); 
        digitalWrite(trigPin, LOW);
        duracion = pulseIn(echoPin, HIGH );
        distancia = duracion/58;

        //Limitacion de rango de lectura de distancia
        if (distancia > 63){
          distancia = 63;
          }
         
        if (distancia < 4){
          distancia = 4;
          }
          
        distancia = longitud - distancia;       //Cambio de referencia al fondo del tubo
        
        if(distancia > ref){
          bandera = 0;
          distanciaAnterior = distancia;
          break; 
          }
        contador += 1; 
        }
      contador = 0; 
      }
    Serial.println(U);
    Serial.println(distancia); 
    }
  //Fin de secuencia de inicio
  
  unsigned long tiempoActual;
  tiempoActual = millis();
  
  dT = (double)(tiempoActual - tiempoAnterior);  
  
  if(dT >= tm*1000){
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);
    duracion = pulseIn(echoPin, HIGH );
    distancia = duracion/58;
    distancia = longitud - distancia;             //Cambio de referencia al fondo del tubo

    //Control de diferencia de lectura irregular
    if (abs(distancia - distanciaAnterior) > 10){ 
      distancia = distanciaAnterior;
      }
    if (distancia > 63){
      distancia = 63;
      }
       
    if (distancia < 4){
      distancia = 4;
      }
      
    error = ref - distancia;
    errorD = error - errorAnterior;
    errorI += (ki*error)*tm;
    
    U = kp*error + (kd*errorD)/tm + errorI;
   
    tiempoAnterior = tiempoActual; 
    errorAnterior = error;
    distanciaAnterior = distancia;
    
    //Limitacion de esfuerzo de control
    if (U > 190){
      U = 190;
      }
          
    if (U < 183){
      U = 183;
      } 
       
  Serial.println(distancia);  
  analogWrite(9,floor(U));
  delay(2);
  }  
}

