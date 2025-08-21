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
- **⏰ Configurable Timing**: Customizable thresholds for all timing parameters
- **🔌 Flexible Wiring**: Supports both active-low and active-high configurations
- **🚀 Non-Blocking**: Minimal impact on main loop performance

## Installation

### Using Arduino IDE Library Manager
1. Open Arduino IDE
2. Go to **Sketch > Include Library > Manage Libraries...**
3. Search for "EventDrivenButtonHandler"
4. Click Install

### Manual Installation
1. [Download the latest release](https://github.com/joruf/EventDrivenButtonHandler/releases)
2. In Arduino IDE: **Sketch > Include Library > Add .ZIP Library...**
3. Select the downloaded ZIP file

### PlatformIO
```ini
lib_deps =
    https://github.com/yourusername/EventDrivenButtonHandler.git
```

### Example
```c++
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
```
