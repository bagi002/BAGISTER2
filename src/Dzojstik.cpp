#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

uint8_t AutoAdress[] = {0xEC, 0x64, 0xC9, 0x85, 0x85, 0xBC};
int Xosa = 34;
int Yosa = 35;
int indikator = 2;

enum Direction{
    FOWARD, BACK, LEFT, RIGHT
};

typedef struct MESSAGE{
    Direction direction;
    int speed;
    long roling;
}Message;


void setup(){
    bool error = false;
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    pinMode(indikator, OUTPUT);

    if(esp_now_init() != ESP_OK){
        error = true;
        Serial.println("Greska pri incijalizaciji uredjaja");
    }else{
        Serial.println("Inicijalizacija obavljena");
    }

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, AutoAdress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if(esp_now_add_peer(&peerInfo) != ESP_OK){
        error = true;
        Serial.println("Greska pri konektovanju na vozilo");
    }
    if(!error){
        digitalWrite(indikator, HIGH);
    }
}

void loop(){
    
}