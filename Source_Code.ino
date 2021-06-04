/*
SET POINT = 10cm dari sensor

E dan DE Trapesium Segitiga Trapesium
 -5 0 5

 JANGAN LUPA Set nya diatur sesuai keinginan
 ada di void setup line 39
*/
int ON,OZ,OP;
bool N, Z, P; //ERROR
bool NE, ZE, PE; // DE
float E, DE, ER;
float uE[3], uDE[3];
float V, uV;
float PWM, out;

int trigPin = 12;
int echoPin = 13;
int enB = 11;
int in4 = 9;
int in3 = 10;
int Set; // Set point 

long durasi;
float jarak;  

void setup(){
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  E = 0;
  DE = 0;
  ER = 0;
  Set = 8;
  
  N=0;
  Z=0;
  P=0;
  NE=0;
  ZE=0;
  PE=0;
   
  ON = -100;
  OZ = 0;
  OP = 100;

  }

void clr()
{
  N=0;
  Z=0;
  P=0;
  NE=0;
  ZE=0;
  PE=0; 
  uE[1]=0;
  uE[2]=0;
  uE[3]=0;
  uDE[1]=0;
  uDE[2]=0;
  uDE[3]=0;
  V=0;
  uV=0;
  PWM=0;
   E=0;
//  DE=0;
}

void fuzzifikasi() { 
//N uE[1]//cek bagian Ne
 if(E<0){
  N=1;
  if(E<0 && E>-5) uE[1]= -E/5; 
  else uE[1]=1;
  Serial.print("uE[1]= ");Serial.println(uE[1]);
 }

//Z uE[2]//cek bagian Ze
 if(E>-5 && E<5){
 Z=1;
 if(E<0 && E>-5) uE[2]= (E+5)/5;
 else uE[2]=(5-E)/5;
  Serial.print("uE[2]= ");Serial.println(uE[2]);
 }

//P//cek bagian Pe
 if(E>0){
 P=1;
 if(E<5 && E>0) uE[3]= E/5;
 else uE[3]=1;
 Serial.print("uE[3]= ");Serial.println(uE[3]);
 }
 
//============================================================



//N///cek bagian NDE
 if(DE<0){
  NE=1;
  if(DE<=-5) uDE[1] = 1;
  else uDE[1]= -DE/5;
  Serial.print("uDE[1]= ");Serial.println(uDE[1]);
   }
   
//Z1//cek bagian ZDE
 if(DE>-5 && DE<5){
 ZE=1;
 if(DE<0 && DE>-5) uDE[2]=(DE+5)/5;
 else uDE[2]=(5-DE)/5;
 Serial.print("uDE[2]= ");Serial.println(uDE[2]);
}
//P//cek bagian PDE
 if(DE>0){
 PE=1;
  if(DE>0 && DE<5) uDE[3 ]= DE/5;
  else uDE[3]=1;
  Serial.print("uDE[3]= ");Serial.println(uDE[3]);
 }


}

//==========================================================

void inference(){

  if(N==1 && NE==1){
    V = V + (min(uDE[1],uE[1])*ON);
    uV= uV + min(uDE[1],uE[1]);
  }

  if(N==1 && ZE==1){
    V = V + (min(uDE[2],uE[1])*ON);
    uV= uV + min(uDE[2],uE[1]);
  }

  if(N==1 && PE==1){
    V = V + (min(uDE[3],uE[1])*ON);
    uV= uV + min(uDE[3],uE[1]);
  }

  if(Z==1 && NE==1){
    V = V + (min(uDE[1],uE[2])*ON);
    uV= uV + min(uDE[1],uE[2]);
  }

  if(Z==1 && ZE==1){
    V =  V+(min(uDE[2] ,uE[2])*OZ);
    uV=  uV + min(uDE[2],uE[2]);
  }

  if(Z==1 && PE==1){
    V = V + (min(uDE[3],uE[2])*OP);
    uV= uV + min(uDE[3],uE[2]);
  }

  if(P==1 && NE==1){
    V = V + (min(uDE[1],uE[3])*OP);
    uV = uV + min(uDE[1],uE[3]);
  }

  if(P==1 && ZE==1){
    V = V + (min(uDE[2],uE[3])*OP);
    uV = uV + min(uDE[2],uE[3]);
  }

  if(P==1 && PE==1){
    V = V + (min(uDE[3],uE[3])*OP);
    uV = uV + min(uDE[3],uE[3]);
  }
 }

void defuzzifikasi(){
  out = (V/uV);
  {
    if(out<=0)
    PWM=0;
    if (out>0)
    PWM = out + 80;
    if (PWM>=250)
    PWM = 250;
   }
}

void loop(){
Serial.print("SET POINT = "); Serial.println(Set);
Serial.print("Jarak = ");
Serial.println(jarak);
Serial.println();



digitalWrite(trigPin, LOW); 
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
durasi= pulseIn(echoPin, HIGH);  
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW);
jarak = durasi*0.034/2;
E = jarak - Set ;
DE = E - ER ;

fuzzifikasi();
inference();
defuzzifikasi();

ER = E;

Serial.println();
Serial.println("INPUT");
Serial.print("Error: ");
Serial.println(E);
Serial.print("Delta Error: ");
Serial.println(DE);
Serial.println();
Serial.println("OUTPUT");
Serial.print("V = "); Serial.println(V);
Serial.print("uV= "); Serial.println(uV);
Serial.print("PWM: ");
Serial.println(PWM);
Serial.println("=================================");

analogWrite(enB, PWM);

  delay(500);
  clr();
}
