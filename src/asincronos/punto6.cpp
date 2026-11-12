#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid="CESJT";
const char* password="itisjt";

AsyncWebServer server(80);
int led=10;
int est=0;
int t_anterior=0;
const char pagina_template[]PROGMEM=R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title>Pagina Web</title>
        <style>
            body{background-color: #ecf0f1;}
            .title{font-family: Arial Black;size:34px;text-align: center;}
            .panel-temp{width:550px;padding:30px;border-radius:20px;border:1px solid black;background-color: PANEL__COLOUR;}
            span{display: block;color:TXT_COLOUR}
        </style>
    </head>
    <body>
        <h1 class="title">SISTEMA DE REFRIGERACIÓN</h1>
        <div class="panel-temp">
            <span>MODO ACTUAL: XXXX</span>
            <span>TEMPERATURA OBJETIVO: YYYY</span>
            <span>ESTADO DEL VENTILADOR: ZZZZ</span>
        </div>
        <a href="/detenido">Detenido</a>
        <a href="/ventilacion">Ventilacion</a>
        <a href="/intensivo">Intensivo</a>
    </body>
</html>
)rawliteral";

void setup(){
    Serial.begin(115200);
    pinMode(led, OUTPUT);
    WiFi.begin(ssid, password);
    while(WiFi.status()!=WL_CONNECTED) delay(500);
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET,[](AsyncWebServerRequest *request){
        String pagina=pagina_template;
        String x, y, z, panel, texto;
        if(est==0){
            x="DETENIDO";
            y="--°C";
            z="APAGADO";
            panel="grey";
            texto="black";
        } 
        if(est==1){
            x="VENTILACIÓN";
            y="25°C";
            z="VELOCIDAD MEDIA";
            panel="lightblue";
            texto="black";   
        } 
        if(est==2){
            x="ENFRIAMIENTO INTENSIVO";
            y="18°C";
            z="VELOCIDAD ALTA";
            panel="darkblue";
            texto="white";
        } 

        pagina.replace("XXXX", x);
        pagina.replace("YYYY", y);
        pagina.replace("ZZZZ", z);

        pagina.replace("TXT_COLOUR", texto);
        pagina.replace("PANEL__COLOUR", panel);

        request->send(200, "text/html",pagina);
    });

    server.on("/detenido", HTTP_GET,[] (AsyncWebServerRequest *request){
        est=0;
        request->redirect("/");
    });

    server.on("/ventilacion", HTTP_GET,[] (AsyncWebServerRequest *request){
        est=1;
        request->redirect("/");
    });

    server.on("/intensivo", HTTP_GET,[] (AsyncWebServerRequest *request){
        est=2;
        request->redirect("/");
    });
    
    server.begin();
}

void loop(){
    //no necesita de server.handleClient ya que ahora el servidor toma peticiones
    //de manera asincronica, pudiendo realizar tareas en segundo plano
    if(est==0) digitalWrite(led, LOW);
    if(est==1){
        digitalWrite(led, HIGH);
        if(millis()-t_anterior>=600){
            t_anterior=millis();
            int led_estado=!led_estado
            digitalWrite(led, led_estado);
        }
    }
    if(est==2) digitalWrite(led, HIGH);
}