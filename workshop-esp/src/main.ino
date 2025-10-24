/*
 * PRÁTICA 2 (Adaptada):
 * Pegamos o código da Prática 1 (Arduino) e adaptamos
 * para o ESP32, adicionando a conexão Wi-Fi.
 */

// --- MUDANÇA 1: Bibliotecas ---
// A biblioteca Servo é diferente no ESP32
#include <ESP32Servo.h>
// Bibliotecas novas para a Internet
#include <WiFi.h>
#include <HTTPClient.h>

// --- Configuração da Rede (Novo) ---
const char *ssid = "Wokwi-GUEST";
const char *password = "";
const char *postEndpoint = "https://httpbin.org/post";

// --- MUDANÇA 2: Pinos ---
// Os pinos físicos são diferentes no ESP32
const int pinoPotenciometro = 34; // Usar um pino ADC (ex: 34)
const int pinoServo = 18;         // Usar um pino GPIO (ex: 18)

// --- Objetos (Quase igual) ---
Servo meuServo;
HTTPClient http; // Objeto novo para a rede

// --- Variável de Controle (Novo) ---
// Para evitar enviar dados para a nuvem sem parar (SPAM)
int anguloAnterior = -1;

void setup()
{
  // --- MUDANÇA 3: Velocidade do Serial ---
  Serial.begin(115200); // ESP32 usa 115200 por padrão

  // --- Parte do Servo (Igual) ---
  meuServo.attach(pinoServo);
  pinMode(pinoPotenciometro, INPUT);

  // --- Bloco de Conexão Wi-Fi (Novo) ---
  Serial.print("Conectando na rede: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado!");
  Serial.println("-------------------------");
}

void loop()
{
  // --- PARTE 1: LÓGICA DO SERVO (Quase igual) ---

  int valorLido = analogRead(pinoPotenciometro);

  // --- MUDANÇA 4: Resolução do Sensor ---
  // O ESP32 lê de 0 a 4095 (12-bit)
  // O Arduino lia de 0 a 1023 (10-bit)
  int angulo = map(valorLido, 0, 4095, 0, 180);

  meuServo.write(angulo);


  // --- PARTE 2: LÓGICA DE REDE (Novo) ---

  // Só envia o dado para a nuvem se o ângulo realmente mudou
  if (angulo != anguloAnterior)
  {

    // Monta o pacote de dados (JSON)
    String jsonPayload = "{\"angulo\":" + String(angulo) + "}";

    // Envia os dados
    http.begin(postEndpoint);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(jsonPayload);

    // Confere se deu certo
    if (httpCode == 200)
    { // 200 = OK
      Serial.println(">>> Dados enviados para a nuvem com sucesso!");
      // String resposta = http.getString(); // Descomente se quiser ver a resposta
      // Serial.println(resposta);
    }
    else
    {
      Serial.printf(">>> Erro ao enviar dados. Codigo: %d\n", httpCode);
    }

    http.end(); // Fecha a conexão

    anguloAnterior = angulo; // Atualiza o último ângulo enviado
  }

  // --- Delay (Igual) ---
  delay(50); // Delay um pouco maior para dar tempo à rede
}