double U =0;
int ref = 10;
volatile unsigned long tiempoAnterior;
bool b = 1;
int error = 0;
int cont2=0;
int contad = 0;
double ki=0.05;
double kd=25;
double kp=7;        //Constante Proporcional
int trigPin = 12; // Pin disparador. Se puede usar otro pin digital
int echoPin = 11; // Pin eco. Se puede usar otro pin digital
long duracion;
long dT;
double errorAnt=0;
double errorSuma = 0;
double errorDer=0;
long distancia =20;
void setup() {
Serial.begin(9600);
pinMode(trigPin, OUTPUT); // Establece pin como salida
pinMode(echoPin, INPUT); // Establece pin como entrada
digitalWrite(trigPin, LOW); // Pone el pin a un estado logico 
tiempoAnterior=0;
}

void loop() {
  
  //Fin de la medicion 
  while(b ==1){
    U=U+3;
    if (U>=175){
      while(contad<300){
        analogWrite(9,U);
        delay(5);
        digitalWrite(trigPin, HIGH); 
        delayMicroseconds(10); 
        digitalWrite(trigPin, LOW);
        duracion = pulseIn(echoPin, HIGH );
        distancia = duracion/58;
        if(distancia < 10){
          b=0;
          break; }
        contad+=1; }
      contad=0; }


    Serial.println(U);
    Serial.println(distancia); }

  unsigned long tiempoActual;
  tiempoActual = millis();
  //error = distancia-ref;
  //Serial.println("error");
  //Serial.println(error);
  
  dT=(double)(tiempoActual - tiempoAnterior) ; //Tiempo de muestreo en milisegundos
  //Serial.println("DT");
  //Serial.println(dT);
  if(dT>=10){
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);
    duracion = pulseIn(echoPin, HIGH );
    distancia = duracion/58;
    error=distancia-ref;
    errorDer=error-errorAnt;
    errorSuma += (ki*error);
    U=kp*error +kd*errorDer + errorSuma;
   // Serial.println()
   tiempoAnterior=tiempoActual; 
   errorAnt=error;
   if (U>200){
          U=200; }
          
   if (U<185){
        U=185; } 
   
       
  Serial.println(distancia);
  //Serial.println(U);  
  analogWrite(9,U);
  delay(2);
}
    
  
}

int medir(){ 
  //Medicion de distancia
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duracion = pulseIn(echoPin, HIGH );
  long  distancia = duracion/58; 
  return distancia;
  }


