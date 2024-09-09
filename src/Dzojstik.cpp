#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

uint8_t AutoAdress[] = {0xEC, 0x64, 0xC9, 0x85, 0x85, 0xBC};
int xOsa = 34;
int yOsa = 35;
int indikator = 2;

enum Direction{
    FOWARD, BACK, LEFT, RIGHT, STOP,
    FOWARDRIGHT, FOWARDLEFT, BACKRIGHT, BACKLEFT
};

typedef struct MESSAGE{
    Direction direction;
    int speed;
    float roling;
}Message;

Message message;
Message lastMessage;

float mapCustom(float, float, float);

void setup(){
    bool error = false;
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    
    pinMode(indikator, OUTPUT);
    pinMode(xOsa, INPUT);
    pinMode(yOsa, INPUT);

    message.direction = STOP;
    message.roling = 0;
    message.speed = 0;

    lastMessage = message;

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

void loop() {
    delay(100);

    int x = analogRead(xOsa) - 1965;
    int y = analogRead(yOsa) - 1925;
    float xN = 0;
    float yN = 0;
    if(x > 60 + abs(y)/3){
      xN = mapCustom(x, 150.0, 2130.0);
    }else if(x < -60 - abs(y)/3){
      xN = -mapCustom(-x, 150, 1965);
    }else{
      xN = 0;
    }

    if(y > 300 + abs(x)/3){
      yN = mapCustom(y, 300.0, 2170.0);
    }else if(y < -300 - abs(x)/3){
      yN = -mapCustom(-y, 300, 1925);
    }else{
      yN = 0;
    }

    if(yN == 0){
        if(xN > 0){
            message.direction = FOWARD;
            message.speed = xN;
            message.roling = 0;
        }else if(xN < 0){
            message.direction = BACK;
            message.speed = -xN;
            message.roling = 0;
        }else{
            message.direction = STOP;
            message.speed = 0;
            message.roling = 0;
        }
    }else{
        if(xN == 0){
            if(yN > 0){
                message.direction = RIGHT;
                message.speed = yN;
                message.roling = 1;
            }else{
                message.direction = LEFT;
                message.speed = -yN;
                message.roling = 1;
            }
        }else{
            if(yN > 0){
                if(xN > 0){
                    message.direction = FOWARDRIGHT;
                    message.speed = xN;
                    message.roling = yN / 255;
                }else{
                    message.direction = BACKRIGHT;
                    message.speed = -xN;
                    message.roling = yN / 255;
                }
            }else{
                if(xN > 0){
                    message.direction = FOWARDLEFT;
                    message.speed = xN;
                    message.roling = -yN / 255;
                }else{
                    message.direction = BACKLEFT;
                    message.speed = -xN;
                    message.roling = -yN / 255;
                }
            }
        }
    }

    if(message.direction == lastMessage.direction && (message.speed == lastMessage.speed) && message.roling == lastMessage.roling){
    }else{
        lastMessage = message;
        esp_now_send(AutoAdress, (uint8_t *) &message, sizeof(message));
    }
}

float mapCustom(float input, float minInput, float maxInput) {

    if(input < minInput) return 0;
    float normalizedInput = (input - minInput) / (maxInput - minInput); // Normalizacija na opseg 0-1
    float transformedInput = normalizedInput; // Primena kvadratnog korena

    float target = 100.0 + (transformedInput * (255.0 - 100.0));
    return target;
}