
#include <ESP8266WiFi.h>

const char* ssid = "TP-LINK_3FE4";//Nome da sua rede Wifi

const char* password = "25235173";//Senha da rede

//IP do ESP (para voce acessar pelo browser)
IPAddress ip(192, 168, 1, 102); //Gateway Padrão. Esse será o valor que vai ser digitado no browser

//IP do roteador da sua rede wifi
IPAddress gateway(192, 168, 1, 1); //Endereço IPv4

//Mascara de rede da sua rede wifi
IPAddress subnet(255, 255, 255, 0); //Máscara de Sub


WiFiServer server(80);//Criando o servidor web na porta 80

//Pino do NodeMCU que estara conectado ao rele
const int pin = 2; //Equivalente ao D4 no NodeMCU

//Funcao que sera executada apenas ao ligar o ESP8266
void setup() {
  Serial.begin(115200);
//Preparando o pino, que esta lidago ao rele
  pinMode(pin, OUTPUT);
//Isso definirá se a saída começará em HIGH ou LOW quando o sistema for iniciado
  digitalWrite(pin, HIGH);
//Conectando a rede Wifi
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
//Verificando se esta conectado,
//caso contrario, espera um pouco e verifica de novo.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
//Iniciando o servidor Web
  server.begin();
}

//Funcao que sera executada indefinidamente enquanto o NodeMCU estiver ligado.
void loop() {
//Verificando se o servidor estapronto.
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
//Verificando se o servidor recebeu alguma requisicao
  while (!client.available()) {
    delay(1);
  }
//Obtendo a requisicao vinda do browser
  String req = client.readStringUntil('\r');

//Iniciando o buffer que ira conter a pagina HTML que sera enviada para o browser.
  String buf = "";
  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  buf += "<head><meta charset='UTF-8'><meta http-equiv='cache-control' content='max-age=0' /><meta http-equiv='cache-control' content='no-cache' /><meta http-equiv='expires' content='0' /> <meta http-equiv='expires' content='Tue, 01 Jan 1980 1:00:00 GMT' /><meta http-equiv='pragma' content='no-cache' /><title>PORTEIRO WIFI</title>";
  buf += "<style>body{font-family:Open Sans; color:#555555;} .container .col { float: left; margin: 0 0 1.2em; padding-right: 1.2em; padding-left: 1.2em; } .container { width: 100%; max-width: 1080px;} @media screen and (min-width: 300px) { .container{width: 100%; max-width: 400px; margin: 0 auto;}  }";
  buf += "* {-moz-box-sizing: border-box;} a{text-decoration:none;}  .btn {font-size:   18px;   white-space:nowrap;   width:100%;   padding:.8em   1.5em;   font-family:   Open   Sans,Helvetica,Arial,sans-serif; line-height:18px; display: inline-block;zoom: 1; color: #fff; text-align: center; position:relative;  -webkit-transition: border .25s linear, color .25s linear, background-color .25s linear; transition: border .25s linear, color .25s linear, background-color .25s linear;}";
  if(digitalRead(pin) == 1){
    buf += ".btn.btn-sea{background-color:  #32cd32; box-shadow: 0 3px 0 #228b22; border-radius: 100px}";
  } else {
    buf += ".btn.btn-sea{background-color:  #dd1111; box-shadow: 0 3px 0 #8b0000; border-radius: 100px}";
  }
  buf += "</style></head><body><div class='container'><p align='center'><font size='8'>Controle de Portão</font></p><a href='OPCL' class='btn btn-sea'>Abrir/Fechar</a><p align='center'><font size='4'>Luiz Gustavo Chinelato Setten</font></p><p align='center'><font size='4'>Lucas Pires Barboza</font></p><p align='center'><font size='4'>Pedro Henrique Borges Prado</font></p></div></body></html>";


if (req.indexOf("OPCL") != -1){
    digitalWrite(pin, !digitalRead(pin));
    delay(500);
   }
//Analisando a requisicao recebida para decidir se liga ou desliga a lampada
  
  //Enviando para o browser a 'pagina' criada.
  client.print(buf);
}
