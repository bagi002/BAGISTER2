#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <Ultrasonic.h>
#include <NAV.h>

//Globalne promjenjive
Message inMessage = {STOP, 0, 0.0};
Message toCarMessage = {STOP, 0, 0.0};
bool dataEnter = false;
portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;
UlSenzor senzor1(27, 35);
NAV controler(&senzor1);

// Taskovi i prekidneRutine
void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len){
    memcpy(&inMessage, incomingData, sizeof(inMessage));
    taskENTER_CRITICAL(&myMutex);
    dataEnter = true;
    taskEXIT_CRITICAL(&myMutex);
}

void readFrontSensor(void *parms){
    while(1){
        senzor1.loadDistance();
        vTaskDelay(25/ portTICK_PERIOD_MS);
    }
}

void sendMessageOnMotors(void *Params){
    int notData = 0;
    String mess;
    while(1){
        if(dataEnter){
            taskENTER_CRITICAL(&myMutex);
            dataEnter = false;
            taskEXIT_CRITICAL(&myMutex);
            Message copy = toCarMessage;

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
        vTaskDelay(70/ portTICK_PERIOD_MS);
    }
}

void navControl(void *parms){
    while(1){
        controler.readInputMessage(inMessage);
        controler.autonomousControl();
        toCarMessage = controler.getOutput();
        vTaskDelay(48/ portTICK_PERIOD_MS);
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
    xTaskCreate(readFrontSensor, "frontSenz", 1024, NULL, 1, NULL);
    xTaskCreate(navControl, "nav", 1024, NULL, 1, NULL);
}

void loop(){} // obavezno prazno
