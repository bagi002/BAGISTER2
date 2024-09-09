#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

enum Direction{
    FOWARD, BACK, LEFT, RIGHT, STOP,
    FOWARDRIGHT, FOWARDLEFT, BACKRIGHT, BACKLEFT
};

typedef struct MESSAGE{
    Direction direction;
    int speed;
    float rolling;
}Message;

Message message;

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len){
    memcpy(&message, incomingData, sizeof(message));
    String comm;
    comm = String((int)message.direction) + ";" + String(message.rolling) + ";" + message.speed +"\n";
    Serial2.print(comm);

}

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

    esp_now_register_recv_cb(onDataRecv);

}

void loop(){

}