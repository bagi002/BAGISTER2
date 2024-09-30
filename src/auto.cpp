#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <Ultrasonic.h>
#include <NAV.h>
#include <batteryHelt.h>

//Globalne promjenjive
Message inMessage = {STOP, 0, 0.0};
Message toCarMessage = {STOP, 0, 0.0};
Message2 messStat = {0};
bool dataEnter = false;
portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE senzorU = portMUX_INITIALIZER_UNLOCKED;
UlSenzor senzor1(27, 35);
NAV controler(&senzor1);
Battery battery;

uint8_t DzojstikAdress[] = {0xC8, 0x2E, 0x18, 0xF8, 0x6A, 0xAC};

// Taskovi i prekidneRutine
void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len){
    memcpy(&inMessage, incomingData, sizeof(inMessage));
    taskENTER_CRITICAL(&myMutex);
    dataEnter = true;
    taskEXIT_CRITICAL(&myMutex);
}

void readBateryStatus(void *params){
    while(1){
        battery.readVoltage();
        messStat.numeric = battery.getVoltageSource();
        vTaskDelay(100/ portTICK_PERIOD_MS);
    }
}

void sendDataOnDzojstik(void *params){
    while(1){
        esp_now_send(DzojstikAdress, (uint8_t *) &messStat, sizeof(messStat));
        vTaskDelay(450/portTICK_PERIOD_MS);
    }
}

void readFrontSensor(void *parms){
    while(1){
        taskENTER_CRITICAL(&senzorU);
        senzor1.loadDistance();
        taskEXIT_CRITICAL(&senzorU);
        vTaskDelay(35/ portTICK_PERIOD_MS);
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
                Serial.println(mess);
            }
        }
        vTaskDelay(85/ portTICK_PERIOD_MS);
    }
}

void navControl(void *parms){
    while(1){
        controler.readInputMessage(inMessage);
        taskENTER_CRITICAL(&senzorU);
        controler.autonomousControl();
        taskEXIT_CRITICAL(&senzorU);
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

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, DzojstikAdress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if(esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Greska pri konektovanju na Dzojstik");
    }

    esp_now_register_recv_cb(onDataRecv);


    xTaskCreate(sendMessageOnMotors, "driverCom", 1024, NULL, 0, NULL);
    xTaskCreate(readFrontSensor, "frontSenz", 1024, NULL, 1, NULL);
    xTaskCreate(navControl, "nav", 1024, NULL, 2, NULL);
    xTaskCreate(readBateryStatus, "batery", 1024, NULL, 0, NULL);
    xTaskCreate(sendDataOnDzojstik, "sender", 1024, NULL, 0, NULL);
}

void loop(){} // obavezno prazno
