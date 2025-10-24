#include <Servo.h> // Necessário para controlar servos, instalar biblioteca Servo via Wokwi, Arduino IDE ou PlatformIO

Servo meuServo;

const int pinoPotenciometro = A0; // Onde lemos o potenciômetro
const int pinoServo = 9;          // Onde controlamos o servo

void setup()
{
  // Inicia o Serial Monitor para vermos os valores
  Serial.begin(9600);

  // 3. "Anexa" o nosso objeto servo ao pino físico
  meuServo.attach(pinoServo);

  // Define o pino do potenciômetro como ENTRADA
  pinMode(pinoPotenciometro, INPUT);
}

void loop()
{
  // 1. LER O SENSOR
  // Lê o valor do potenciômetro (vai de 0 até 1023)
  int valorLido = analogRead(pinoPotenciometro);

  // 2. PROCESSAR O VALOR (Mapear)
  // Converte a faixa de leitura (0-1023) para a faixa
  // de ângulos do servo (0 a 180 graus).
  int angulo = map(valorLido, 0, 1023, 0, 180);

  // 3. ATUAR
  // Envia o comando de ângulo para o servo.
  meuServo.write(angulo);

  // Um pequeno delay é necessário para o servo se mover
  delay(15);
}