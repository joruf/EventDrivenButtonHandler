#include "EventDrivenButtonHandler.h"
#include "MultiButtonHandler.h"

// GPIO PINs
#define BUTTON_UP 26
#define BUTTON_DOWN 27

EventDrivenButtonHandler btnUp(BUTTON_UP);
EventDrivenButtonHandler btnDown(BUTTON_DOWN);
MultiButtonHandler btnUpDown(50);

void setup() {
  Serial.begin(115200);

  btnUp.setClickThreshold(50);
  btnUp.setLongClickThreshold(500);
  btnUp.setDebounceTime(50);
  btnUp.addClickHandler([](ClickType type) {
    switch(type) {
        case ClickType::SHORT_CLICK_DOWN:
            Serial.println("Short Click Down");
            break;
        case ClickType::SHORT_CLICK_UP:
            Serial.println("Short Click Up");
            break;
        case ClickType::LONG_CLICK_DOWN:
            Serial.println("Long Click Down");
            break;
        case ClickType::LONG_CLICK_UP:
            Serial.println("Long Click Up");
            break;
        case ClickType::DURING_PRESS:
            Serial.println("During Press");
            break;
    }
  });

  btnDown.setClickThreshold(50);
  btnDown.setLongClickThreshold(500);
  btnDown.setDebounceTime(50);
  btnDown.addClickHandler([](ClickType type) {
    switch(type) {
        case ClickType::SHORT_CLICK_DOWN:
            Serial.println("Short Down Click Down");
            break;
    }
  });

  btnUpDown.addButton(&btnUp);
  btnUpDown.addButton(&btnDown);
  btnUpDown.setSimultaneousClickHandler([](uint8_t idx1, uint8_t idx2) {
    Serial.println("Simultaneous pressed");
  });
}

void loop() {  
  btnUp.update();
  btnDown.update();
  btnUpDown.update();

  delay(100);
}

