#include <Thread.h>
#include <ThreadController.h>

Thread thread1 = Thread();
Thread thread2 = Thread();
Thread thread3 = Thread();
ThreadController cpu;
// Pisca o LED
int led = 13; // Digo que o pino 13 vai ser utilizado para output - onde vou colocar o led
int PWM_Pin = 3;
int RPM_Pin = 2;
float rps;
float value;
unsigned long t;
// Francisco Camille e Thatianne

void setup() {
  
  
  pinMode(led,OUTPUT); 
  pinMode(RPM_Pin, INPUT_PULLUP);
  pinMode(PWM_Pin, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), ctt, RISING);
  analogWrite(PWM_Pin, 100);

  
  thread1.onRun(fan);
  thread1.setInterval(1000);

  thread2.onRun(ledf);
  thread2.setInterval(1000);

  thread3.onRun(blinkLed);
    

  cpu.add(&thread1);
  cpu.add(&thread2);
  cpu.add(&thread3);
  
  DDRB |= (1 << PB5); // Set o pino 13 do arduino como saída
}


void ledf(){
 if(Serial.available() > 1) {
    value = Serial.parseFloat();
    thread3.setInterval(float((1000/(value))));
  }  
  //Led vai ficar piscando em 20 milisegundos
  //20 milisegundos = 0.02 segundos.
  // Frequência = 1/T => 50Hz
  Serial.print("LED: ");
  Serial.print(value);
  Serial.println(" Hz");
}

void fan(){
  Serial.print("Motor: ");
  Serial.print(rps/2); // Valor em1 HZ
  Serial.println(" Hz"); // Segundo nossos calculos o maximo é 51HZ
  //Serial.println(rpm*30);
  rps = 0;
}


void loop() {
  cpu.run();
}

void blinkLed(){
 
  PORTB |= (1 << PB5); // Seta 1 no pino de saída 13
  _delay_ms(0.2);
  delayMicroseconds(0);
  PORTB &= ~(1 <<PB5); // Seta 0 no pino de saída 13
  //_delay_ms(200);
  /*
  digitalWrite(led,HIGH); // HIGH -> equivalente a 1
  delayMicroseconds(200);
// delay(miliseconds/2);
  digitalWrite(led,LOW); // LOW -> equivalente a 0
  */
}

void ctt() {
  rps++;
}
