#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid="CESJT";
const char* password="itisjtsmg";

WebServer server(80);
int rojo=10;
int verde=11;
int azul=12;
String color="Apagado";
const char pagina_template[]=R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title>Pagina Web</title>
        <style>
            body{background-color: black;}
            .title{text-align: center;color: white}
            .texto{color: white}
            .contenedor{display: flex;justify-content: center;}
            .btn{border-radius: 50%;width: 80px;height: 80px;border:3px solid white;text-align: center;}
            .rojo{background-color: red}
            .verde{background-color: green}
            .azul{background-color: blue}
            .blanco{background-color: white}
            .apagado{background-color: #34495e}
        </style>
    </head>
    <body>
        <h1 class="title">Selector de Color RGB</h1>
        <span class="texto">Color Actual: XXX</span>
        <div class="contenedor">
            <a href="/rojo" class="btn rojo">Rojo</a>
            <a href="/verde" class="btn verde">Verde</a>
            <a href="/azul" class="btn azul">Azul</a>
            <a href="/blanco" class="btn blanco">Blanco</a>
            <a href="/apagado" class="btn apagado">Apagado</a>
        </div>
    </body>
</html>
)rawliteral";

void setup(){
    Serial.begin(115200);
    pinMode(rojo, OUTPUT);
    pinMode(verde, OUTPUT);
    pinMode(azul, OUTPUT);

    WiFi.begin(ssid,password);
    while(WiFi.status()!=WL_CONNECTED) delay(500);
    server.on("/",[](){
        String pagina=pagina_template;
        pagina.replace("XXX", color);
        server.send(200,"text/html",pagina);
    });

    server.on("/rojo",[](){
        color="Rojo";
        server.sendHeader("Location","/");
        server.send(302);
    });

    server.on("/verde",[](){
        color="Verde";
        server.sendHeader("Location","/");
        server.send(302);
    });

    server.on("/azul",[](){
        color="Azul";
        server.sendHeader("Location","/");
        server.send(302);
    });

    server.on("/blanco",[](){
        color="Blanco";
        server.sendHeader("Location","/");
        server.send(302);
    });

    server.on("/apagado",[](){
        color="Apagado";
        server.sendHeader("Location","/");
        server.send(302);
    });

    server.begin();

}

void loop(){
    server.handleClient();
    if(color=="Rojo"){
        analogWrite(rojo, 255); //se podria hacer con digitalWrite tambien
        analogWrite(verde,0);
        analogWrite(azul,0);
    }
    if(color=="Verde"){
        analogWrite(rojo,0);
        analogWrite(verde,255);
        analogWrite(azul,0);
    }
    if(color=="Azul"){
        analogWrite(rojo,0);
        analogWrite(verde,0);
        analogWrite(azul,255);
    }
    if(color=="Blanco"){
        analogWrite(rojo,255);
        analogWrite(verde,255);
        analogWrite(azul,255);
    }
    if(color=="Apagado"){
        analogWrite(rojo,0);
        analogWrite(verde,0);
        analogWrite(azul,0);
    }
}

