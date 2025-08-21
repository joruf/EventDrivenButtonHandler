#include "MultiButtonHandler.h"
#include "SingleButtonHandler.h"

// GPIO PINs
#define BUTTON_UP 26
#define BUTTON_DOWN 27

SingleButtonHandler btnUp(BUTTON_UP);
SingleButtonHandler btnDown(BUTTON_DOWN);
MultiButtonHandler btnUpDown(50);

void setup() {
  Serial.begin(115200);

  btnUp.setClickThreshold(50);
  btnUp.setLongClickThreshold(500);
  btnUp.setDebounceTime(50);
  btnUp.addClickHandler([](ClickType type) {
    switch (type) {
    case ClickType::SHORT_CLICK_DOWN:
      Serial.println("Short Click Down");
      break;
    }
  });

  btnDown.setClickThreshold(50);
  btnDown.setLongClickThreshold(500);
  btnDown.setDebounceTime(50);
  btnDown.addClickHandler([](ClickType type) {
    switch (type) {
    case ClickType::SHORT_CLICK_DOWN:
      Serial.println("Short Down Click Down");
      break;
    }
  });

  btnUpDown.addButton(&btnUp);
  btnUpDown.addButton(&btnDown);
  btnUpDown.addClickHandler([](ClickType ct) {
    switch (type) {
    case ClickType::SHORT_CLICK_DOWN:
      Serial.println("Simultaneous pressed SHORT_CLICK_DOWN");
      break;
    case ClickType::LONG_CLICK_DOWN:
      Serial.println("Simultaneous pressed LONG_CLICK_DOWN");
      break;
    case ClickType::SHORT_CLICK_UP:
      Serial.println("Simultaneous pressed SHORT_CLICK_UP");
      break;
    case ClickType::LONG_CLICK_UP:
      Serial.println("Simultaneous pressed LONG_CLICK_UP");
      break;
    }
  });
}

void loop() {
  btnUp.update();
  btnDown.update();
  btnUpDown.update();

  delay(100);
}
