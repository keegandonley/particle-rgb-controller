// This #include statement was automatically added by the Particle IDE.
#include <rgb-controls.h>
using namespace RGBControls;

void handleEvent(int, int);
int setRGB();

int BUTTON_IN_1 = D0;
int BUTTON_IN_2 = D5;
int BUTTON_IN_3 = D6;
int STATUS_BUTTON = D4;
int LED_B = D1;
int LED_G = D2;
int LED_R = D3;
int STATUS = D7;

int buttonValues[4] = { 0 };
int toggleValue = 0;
int readyToToggle = 1;
int isDecrement = 0;

int eventLocks[2] = { 0 };
String names[] = {"Button1", "Button2", "Button3", "ButtonReset"};

Led led(LED_R, LED_G, LED_B);
Color currentColor(255, 0, 0);
int colors[3] = { 0 };

void setup() {
    pinMode(BUTTON_IN_1, INPUT_PULLUP);
    pinMode(BUTTON_IN_2, INPUT_PULLUP);
    pinMode(BUTTON_IN_3, INPUT_PULLUP);
    pinMode(STATUS_BUTTON, INPUT_PULLUP);
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    pinMode(STATUS, OUTPUT);
}

int setRGB()
{
    Color newColor(colors[0], colors[1], colors[2]);
    //Fade once from old color to new color over 500mS
    led.fadeOnce(currentColor, newColor, 500);
    //What's new is old now
    currentColor = newColor;
    
    return 1;
}


// Event handler for events that should be fired one (button press, etc)
// takes eventCode and the value of the input acting as the trigger
void handleEvent(int eventCode, int value) {
    if (eventCode < 3) {
        if (value) {
            if (!eventLocks[eventCode]) {
                if (isDecrement) {
                    colors[eventCode] -= 20;
                } else {
                    colors[eventCode] += 20;
                }
                setRGB();
                eventLocks[eventCode] = 1;
                Particle.publish(names[eventCode], "change");
            }
        } else {
            eventLocks[eventCode] = 0;
        }
    } else {
        if (value) {
            if (readyToToggle) {
                if (isDecrement) {
                    digitalWrite(STATUS, LOW);
                    isDecrement = 0;
                } else {
                    digitalWrite(STATUS, HIGH);
                    isDecrement = 1;
                }
                readyToToggle = 0;
            }
        } else {
            readyToToggle = 1;
        }
    }
}


void loop() {
    buttonValues[0] = !digitalRead(BUTTON_IN_1);
    buttonValues[1] = !digitalRead(BUTTON_IN_2);
    buttonValues[2] = !digitalRead(BUTTON_IN_3);
    toggleValue = !digitalRead(STATUS_BUTTON);

    handleEvent(0, buttonValues[0]);
    handleEvent(1, buttonValues[1]);
    handleEvent(2, buttonValues[2]);
    handleEvent(3, toggleValue);



}
