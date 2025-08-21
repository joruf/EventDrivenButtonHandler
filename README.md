# EventDrivenButtonHandler - Arduino Library

[![Arduino Library](https://www.ardu-badge.com/badge/EventDrivenButtonHandler.svg?)](https://www.ardu-badge.com/EventDrivenButtonHandler)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A non-blocking Arduino library for event-driven button handling with advanced callback functionality. Perfect for creating responsive interfaces without blocking your main loop.

## Features

- **🏗️ Event-Driven Architecture**: Uses callback functions to handle button events asynchronously
- **🖱️ Multiple Click Types**: Comprehensive event support:
  - `SHORT_CLICK_DOWN`: Immediate feedback on button press
  - `SHORT_CLICK_UP`: Triggered when a short click is released
  - `LONG_CLICK_DOWN`: Fired when long press threshold is reached
  - `LONG_CLICK_UP`: Triggered when a long click is released
  - `DURING_PRESS`: Continuous events while button is held
- **🔧 Built-in Debouncing**: Software debouncing for reliable mechanical switch handling
- **🤝 Multi-Button Support**: Detect simultaneous button presses for advanced interactions
- **⏰ Configurable Timing**: Customizable thresholds for all timing parameters
- **🔌 Flexible Wiring**: Supports both active-low and active-high configurations
- **🚀 Non-Blocking**: Minimal impact on main loop performance

## Installation

### Manual File Placement
1. Download the library files
2. Extract the ZIP file
3. Copy the EventDrivenButtonHandler folder to your Arduino libraries directory:
* Windows: Documents\Arduino\libraries\
* Mac: Documents/Arduino/libraries/
* Linux: ~/Arduino/libraries/
    
### PlatformIO
```ini
lib_deps =
    https://github.com/yourusername/EventDrivenButtonHandler.git
```

### Example
```c++
#include "EventDrivenButtonHandler.h"
#include "MultiButtonHandler.h"

// GPIO PINs
#define BUTTON_UP 27
#define BUTTON_DOWN 28

EventDrivenButtonHandler btnUp(BUTTON_UP);
EventDrivenButtonHandler btnDown(BUTTON_DOWN);
MultiButtonHandler btnUpDown(50);

void setup() {
  Serial.begin(115200);

  btnUp.setClickThreshold(50);
  btnUp.setLongClickThreshold(500);
  btnUp.setDebounceTime(50);
  btnUp.addClickHandler([](ClickType type) { // or btnUp.addClickHandler([this](ClickType type)... when inside a class
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
```
