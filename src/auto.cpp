#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// Dodatni tipovi podataka
enum Direction{
    FOWARD, BACK, LEFT, RIGHT, STOP,
    FOWARDRIGHT, FOWARDLEFT, BACKRIGHT, BACKLEFT
};

typedef struct MESSAGE{
    Direction direction;
    int speed;
    float rolling;
}Message;

//Globalne promjenjive
Message message;
bool dataEnter = false;
portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;

// Taskovi i prekidneRutine
void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len){
    memcpy(&message, incomingData, sizeof(message));
    taskENTER_CRITICAL(&myMutex);
    dataEnter = true;
    taskEXIT_CRITICAL(&myMutex);
}

void readFrontSensor(void *parms){
    
}

void sendMessageOnMotors(void *Params){
    int notData = 0;
    String mess;
    while(1){
        if(dataEnter){
            taskENTER_CRITICAL(&myMutex);
            dataEnter = false;
            taskEXIT_CRITICAL(&myMutex);
            Message copy = message;

            mess = String((int)copy.direction)+";"+String(copy.rolling) + ";" + String(copy.speed) + "\n";
            Serial.println(mess);
            Serial2.print(mess);
            notData = 0;
        }else{
            notData++;
            if(notData>5){
                mess = "4;0.0;0\n";
                Serial2.print(mess);
            }
        }
        vTaskDelay(100/ portTICK_PERIOD_MS);
    }
}

void setup(){
    Serial2.begin(115200, SERIAL_8N1, 16, 17); // RX2 na GPIO16 i TX2 na GPIO17
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);

    if(esp_now_init() != ESP_OK){
        Serial.println("Greska u inicijalizaciji veze");
    }else{
        Serial.println("Komunikacija uspostavljena");
    }

    esp_now_register_recv_cb(onDataRecv);

    xTaskCreate(sendMessageOnMotors, "driverCom", 1024, NULL, 1, NULL);

}

void loop(){} // obavezno prazno
