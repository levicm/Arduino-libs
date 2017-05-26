#include "ZabbixPassiveWifi.h"

ZabbixPassiveWifi::ZabbixPassiveWifi():
  ZabbixPassiveWifi(ZABBIX_DEFAULT_PORT) {
}
ZabbixPassiveWifi::ZabbixPassiveWifi(int port):
  _server(port) {
}

void ZabbixPassiveWifi::begin(void) {
  DEBUG_PRINTLN(F("Zabbix.begin"));
  _server.begin();
}

char* ZabbixPassiveWifi::getResponse(void) {
  return _response;
}

char* ZabbixPassiveWifi::getKey(void) {
  return _buffer;
}

void ZabbixPassiveWifi::onKeyReceive(onKeyReceiveCallback keyReceived) {
  _callback = keyReceived;
}

void ZabbixPassiveWifi::listen() {
  WiFiClient client = _server.available();

  // Se nao chegou nada, pode sair daqui
  if (!client) {
    return;
  }
  // Se chegou, espera o cliente enviar algo
  DEBUG_PRINTLN("new client");
  while(!client.available()){
    delay(1);
    // TODO: incluir tratamento de timeout
  }

  // Faz a leitura de todo o stream até a quebra de linha
  int count = client.readBytesUntil('\n', _buffer, MAX_BUFFER_SIZE);
  DEBUG_PRINTLN(count);
  DEBUG_PRINTLN(_buffer);
  #if defined(DEBUG)
  for (int i = 0; i < count; ++i) {
    Serial.print((byte) _buffer[i]);
    Serial.print("(");
    Serial.print(_buffer[i]);
    Serial.print(")");
  }
  Serial.println();
  #endif

  boolean withHeader = (strncmp(_buffer, ZABBIX_HEADER, strlen(ZABBIX_HEADER)) == 0);
  DEBUG_PRINTLN2("Com header: ", withHeader);
  int status = decodeKey(count, withHeader);
  if (status != ZABBIX_STATUS_NO_KEY) {
    // Limpa a resposta
    memset(_response, 0, MAX_RESPONSE_SIZE);
    if (_callback != NULL) {
      DEBUG_PRINTLN("Invocando callback");
      (*_callback)();
    }
    client.flush();
    DEBUG_PRINT("Respondendo com: ");
    if (withHeader) {
      memset(_datalen, 0, ZABBIX_DATALEN_LENGTH);
      _datalen[0] = strlen(_response);
      DEBUG_PRINT(ZABBIX_HEADER);
      DEBUG_PRINT((char*) _datalen);
      client.print(ZABBIX_HEADER);
      client.print((char*) _datalen);
    }
    DEBUG_PRINTLN(_response);
    client.print(_response);
  } else {
    DEBUG_PRINTLN("Chave nao encontrada");
  }

  client.flush();
  client.stop();
  delay(1);
}

int ZabbixPassiveWifi::decodeKey(int size, boolean withHeader) {
  byte keySize;
  // Se a requisicao tem header (alguma versão do protocolo)
  if (withHeader) {
    // Pega o tamanho da chave descartando a quebra de linha
    keySize = ((byte) _buffer[ZABBIX_HEADER_LENGTH]) - 1;
    DEBUG_PRINTLN2("Tamanho da chave: ", keySize); 
    // Valida o tamanho da chave
    if (keySize < 0 || keySize > MAX_BUFFER_SIZE) {
      DEBUG_PRINTLN("invalid key size");
      return ZABBIX_STATUS_NO_KEY;
    }
    // Desloca a chave para o inicio do buffer
    for (int i = 0;i < keySize && i < MAX_BUFFER_SIZE; ++i) {
      _buffer[i] = _buffer[i + ZABBIX_HEADER_LENGTH + ZABBIX_DATALEN_LENGTH];
    }
  } else if (size > 0 && size < MAX_BUFFER_SIZE) {
    // Se não tem header, assume que tudo é a chave (alguma versão do protocolo)
    keySize = size;
  }
  // Limpa o resto do buffer
  for (int i = keySize ;i < MAX_BUFFER_SIZE; ++i) {
    _buffer[i] = (char) 0;
  }
  DEBUG_PRINTLN2("chave: ", _buffer); 
  return ZABBIX_STATUS_OK;
}

void ZabbixPassiveWifi::respond(String value) {
  char str[value.length()];
  value.toCharArray(str, value.length());
  sprintf(_response, str);
}
void ZabbixPassiveWifi::respond(char* value) {
  sprintf(_response, value);
}
void ZabbixPassiveWifi::respond(int value) {
  sprintf(_response, "%d", value);
}
void ZabbixPassiveWifi::respond(float value, byte decimals) {
  dtostrf(value, 10, decimals, _response);
}

