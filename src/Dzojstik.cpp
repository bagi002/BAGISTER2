#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_CS -1
#define TFT_RST 4
#define TFT_DC 5
#define TFT_MOSI 22
#define TFT_SCLK 18

uint8_t AutoAdress[] = {0xEC, 0x64, 0xC9, 0x85, 0x85, 0xBC};
int xOsa = 34;
int yOsa = 35;
int indikator = 21;
int sirenPin = 32;


float oldNapon = 0;

Adafruit_ST7789 tft = Adafruit_ST7789(-1, 5, 4);

enum Direction{
    FOWARD, BACK, LEFT, RIGHT, STOP,
    FOWARDRIGHT, FOWARDLEFT, BACKRIGHT, BACKLEFT
};

typedef struct MESSAGE{
    Direction direction;
    int speed;
    float roling;
    bool siren;
}Message;

typedef struct MESSAGE2{
    int numeric;
}Message2;

Message message;
Message2 autoData;

float mapCustom(float, float, float);
void printBateryData();

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len){
    memcpy(&autoData, incomingData, sizeof(autoData));
}

void setup(){
    bool error = false;
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    Serial.println(WiFi.macAddress());
    autoData.numeric = 0;
    
    pinMode(indikator, OUTPUT);
    pinMode(xOsa, INPUT);
    pinMode(yOsa, INPUT);

    message.direction = STOP;
    message.roling = 0;
    message.speed = 0;

    // LCD
        tft.init(240, 240, SPI_MODE3);
        tft.setRotation(2);
        tft.fillScreen(ST77XX_BLACK);
        tft.setCursor(5,100);
        tft.setTextColor(ST77XX_WHITE);
        tft.setTextSize(4);
        tft.println("BAGISTER2");
        tft.setTextColor(ST77XX_CYAN);
        tft.setTextSize(3);
        delay(2000);
        tft.fillScreen(ST77XX_BLACK);
        tft.setCursor(15,20);
        tft.setTextColor(ST77XX_WHITE);
        tft.setTextSize(3);
        tft.print("Napon: ");
        tft.println("0 V");
        


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
       esp_now_send(AutoAdress, (uint8_t *) &message, sizeof(message));
    esp_now_register_recv_cb(onDataRecv);
}

void loop() {
    delay(87);

    int x = analogRead(xOsa) - 1965;
    int y = analogRead(yOsa) - 1925;
    x = -x;
    y = -y;


    float xN = 0;
    float yN = 0;
    if(x > 60 + abs(y)/3){
      xN = mapCustom(x, 150.0, 1965);
    }else if(x < -60 - abs(y)/3){
      xN = -mapCustom(-x, 150, 2130);
    }else{
      xN = 0;
    }

    if(y > 300 + abs(x)/3){
      yN = mapCustom(y, 300.0, 1925.0);
    }else if(y < -300 - abs(x)/3){
      yN = -mapCustom(-y, 300, 2170);
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

        esp_now_send(AutoAdress, (uint8_t *) &message, sizeof(message));
        printBateryData();
}

float mapCustom(float input, float minInput, float maxInput) {

    if(input < minInput) return 0;
    float normalizedInput = (input - minInput) / (maxInput - minInput); // Normalizacija na opseg 0-1
    float transformedInput = normalizedInput; // Primena kvadratnog korena

    float target = 100.0 + (transformedInput * (255.0 - 100.0));
    return target;
}

void printBateryData(){
        float in = (3.3 / 4095.0) * autoData.numeric + 0.12;
        float napon = in * 3.195;
        if(napon + 0.08 < oldNapon || napon - 0.08 > oldNapon){
            tft.fillScreen(ST77XX_BLACK);
            tft.setCursor(3,20);
            tft.setTextColor(ST77XX_GREEN);
            tft.setTextSize(3);
            tft.print("Napon: ");
            tft.print(napon);
            tft.println(" V");
        }
        oldNapon = napon;
}