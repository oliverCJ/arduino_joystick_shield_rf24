#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <math.h>

#define CE_PIN 9
#define CSN_PIN 10

#define BUTTON_A 2
#define BUTTON_B 3
#define BUTTON_C 4
#define BUTTON_D 5
#define BUTTON_START 6
#define BUTTON_SELECT 7
#define BUTTON_ANALOG 8 // 摇杆按压


#define BUTTON_DELAY 50

int x_axis_button = A0;
int y_axis_button = A1;

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

long lastButonSelectTime = millis();   // 最后按选择时间
long lastButtonStartTime = millis(); // 最后按开始时间
long lastButtonATime = millis();  // 最后按向下的时间
long lastButtonCTime = millis();   // 最后按向下的时间
long lastButtonDTime = millis();   // 最后按向左的时间
long lastButtonBTime = millis();  // 最后按向右的时间
long lastButtonAnalogTime = millis();  // 最后按向右的时间

int xAxisValueCache = 0;
int yAxisValueCache = 0;
int axisDiff = 20;

// 摇杆
struct axis
{
    int max;
    int min;
    int xCenter;
    int yCenter;
    int x; // 当前值
    int y; // 当前值
    String xString;
    String yString;
};
axis joystick = {1023, 0, 512, 500, 0, 0, "center", "center"};

void setup()
{
    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);
    pinMode(BUTTON_D, INPUT_PULLUP);
    pinMode(BUTTON_START, INPUT_PULLUP);
    pinMode(BUTTON_SELECT, INPUT_PULLUP);
    pinMode(BUTTON_ANALOG, INPUT_PULLUP);

    Serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
}

void loop() {
    checkStick();
    checkButton();
    delay(50);
}

void checkButton() {
    if (digitalRead(BUTTON_A) == LOW && (millis() - lastButtonATime) >= BUTTON_DELAY)
    {
        lastButtonATime = millis();
        Serial.println("button A pressed");
        char msg[] = "A";
        radio.write(&msg, sizeof(msg));
    }
    if (digitalRead(BUTTON_B) == LOW && (millis() - lastButtonBTime) >= BUTTON_DELAY)
    {
        lastButtonBTime = millis();
        Serial.println("button B pressed");
        char msg[] = "B";
        radio.write(&msg, sizeof(msg));
    }
    if (digitalRead(BUTTON_C) == LOW && (millis() - lastButtonCTime) >= BUTTON_DELAY)
    {
        lastButtonCTime = millis();
        Serial.println("button C pressed");
        char msg[] = "C";
        radio.write(&msg, sizeof(msg));
    }
    if (digitalRead(BUTTON_D) == LOW && (millis() - lastButtonDTime) >= BUTTON_DELAY)
    {
        lastButtonDTime = millis();
        Serial.println("button D pressed");
        char msg[] = "D";
        radio.write(&msg, sizeof(msg));
    }
    if (digitalRead(BUTTON_START) == LOW && (millis() - lastButtonStartTime) >= BUTTON_DELAY)
    {
        lastButtonStartTime = millis();
        Serial.println("button start pressed");
        char msg[] = "start";
        radio.write(&msg, sizeof(msg));
    }
    if (digitalRead(BUTTON_SELECT) == LOW && (millis() - lastButonSelectTime) >= BUTTON_DELAY)
    {
        lastButonSelectTime = millis();
        Serial.println("button select pressed");
        char msg[] = "select";
        radio.write(&msg, sizeof(msg));
    }
    if (digitalRead(BUTTON_ANALOG) == LOW && (millis() - lastButtonAnalogTime) >= BUTTON_DELAY)
    {
        lastButtonAnalogTime = millis();
        Serial.println("button analog pressed");
        char msg[] = "analog";
        radio.write(&msg, sizeof(msg));
    }
}

void checkStick() {
    joystick.x = analogRead(x_axis_button);
    joystick.y = analogRead(y_axis_button);

    String newxMsg = "x-" + String(joystick.x);
    String newyMsg = "y-" + String(joystick.y);
    char xmsg[10] = "";
    char ymsg[10] = "";
    newxMsg.toCharArray(xmsg, 10);
    newyMsg.toCharArray(ymsg, 10);

    radio.write(&xmsg, sizeof(xmsg));
    radio.write(&ymsg, sizeof(ymsg));

    delay(50);
}