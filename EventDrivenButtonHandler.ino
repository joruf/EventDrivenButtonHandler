
#include "EventDrivenButtonHandler.h"

#define BUTTON_ACTION 27 // GPIO PIN
EventDrivenButtonHandler myButton(BUTTON_ACTION);


void setup() {
  Serial.begin(115200);

  myButton.setClickThreshold(50);
  myButton.setLongClickThreshold(500);
  myButton.setDebounceTime(50);

  myButton.addClickHandler([](ClickType type) {
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

}


void loop() {  
  myButton.update();
  delay(100);
}

