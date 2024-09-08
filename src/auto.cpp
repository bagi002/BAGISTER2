#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

enum Direction{
    FOWARD, BACK, LEFT, RIGHT
};

typedef struct MESSAGE{
    Direction direction;
    int speed;
    long roling;
}Message;

void setup(){
    Serial2.begin(115200, SERIAL_8N1, 16, 17); // RX2 na GPIO16 i TX2 na GPIO17
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    Serial.println(WiFi.macAddress());

    if(esp_now_init() != ESP_OK){
        Serial.println("Greska u inicijalizaciji veze");
    }else{
        Serial.println("Komunikacija uspostavljena");
    }

}

void loop(){

}