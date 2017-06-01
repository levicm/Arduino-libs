#include <Uptime.h>

#define TEMPO_ESPERA 5000 //Intervalo entre cada leitura

Uptime uptime;

void setup() { 
  Serial.begin(115200); //Iniciando Serial
}

void loop() {
  // computa o tempo até aqui
  uptime.compute();
  // Imprime o uptime
  uptime.print();

  // Pega os valores individualmente:
  Serial.print("Uptime: ");
  Serial.print(uptime.days());
  Serial.print("d");
  Serial.print(uptime.hours());
  Serial.print("h");
  Serial.print(uptime.minutes());
  Serial.print("m");
  Serial.print(uptime.seconds());
  Serial.print("s");
  Serial.print(uptime.milliseconds());
  Serial.print(". ");
  Serial.print(uptime.totalSeconds());
  Serial.println(" total seconds");

  // Compara com o tempo diretamente em millis():
  Serial.print("Tempo ate aqui ");
  Serial.print(millis());
  Serial.println(" milisegundos");

  delay(TEMPO_ESPERA);// Espera um tempo determinado em TEMPO_ESPERA (milisegundos)
}