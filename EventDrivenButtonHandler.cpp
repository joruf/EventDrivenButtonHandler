#include "EventDrivenButtonHandler.h"

/**
 * @brief Constructor for the button handler
 * @param pin The GPIO pin where the button is connected
 * @param activeLow Specifies if the button operates in active-low mode
 * @param pullup Enables the internal pullup resistor
 */
EventDrivenButtonHandler::EventDrivenButtonHandler(uint8_t pin, bool activeLow,
                                                   bool pullup)
    : pin(pin), activeLow(activeLow), clickThreshold(50),
      longClickThreshold(1000), debounceTime(50), pressStartTime(0),
      lastEventTime(0), wasPressed(false), longClickDetected(false),
      longClickReported(false), shortClickDownReported(false),
      buttonHandled(false) {
  if (pullup) {
    pinMode(pin, activeLow ? INPUT_PULLUP : INPUT);
  } else {
    pinMode(pin, INPUT);
  }
  wasPressed = isPressed(); // Capture initial state
}

/**
 * @brief Destructor for the button handler
 */
EventDrivenButtonHandler::~EventDrivenButtonHandler() { handlers.clear(); }

/**
 * @brief Gets the current pressed state of the button
 * @return True if the button is pressed, false otherwise
 */
bool EventDrivenButtonHandler::getPressedState() const { return isPressed(); }

/**
 * @brief Sets the threshold for click detection
 * @param threshold Time in milliseconds after which a press is recognized as a
 * valid click
 */
void EventDrivenButtonHandler::setClickThreshold(unsigned long threshold) {
  clickThreshold = threshold;
}

/**
 * @brief Sets the threshold for long click detection
 * @param threshold Time in milliseconds after which a press is recognized as a
 * long click
 */
void EventDrivenButtonHandler::setLongClickThreshold(unsigned long threshold) {
  longClickThreshold = threshold;
}

/**
 * @brief Sets the debounce time for button state changes
 * @param time Time in milliseconds to ignore state changes after a detected
 * change
 */
void EventDrivenButtonHandler::setDebounceTime(unsigned long time) {
  debounceTime = time;
}

/**
 * @brief Adds a callback function to handle button events
 * @param handler Function to be called when a button event occurs
 */
void EventDrivenButtonHandler::addClickHandler(
    std::function<void(ClickType)> handler) {
  handlers.push_back(handler);
}

/**
 * @brief Removes all registered callback functions
 */
void EventDrivenButtonHandler::removeClickHandlers() { handlers.clear(); }

/**
 * @brief Reads the current state of the button
 * @return True if the button is pressed, false otherwise
 */
bool EventDrivenButtonHandler::isPressed() const {
  bool currentState = digitalRead(pin);
  return activeLow ? !currentState : currentState;
}

/**
 * @brief Notifies all registered handlers of a button event
 * @param clickType Type of button event to notify
 */
void EventDrivenButtonHandler::notifyHandlers(ClickType clickType) {
  for (auto &handler : handlers) {
    handler(clickType);
  }
}

/**
 * @brief Processes button state changes - must be called regularly in the main
 * loop
 */
void EventDrivenButtonHandler::update() {
  unsigned long currentTime = millis();
  bool pressed = isPressed();

  // Detect state change
  if (pressed != wasPressed) {
    if (currentTime - lastEventTime > debounceTime) {
      lastEventTime = currentTime;

      if (pressed && !wasPressed) {
        // Button pressed (falling edge)
        pressStartTime = currentTime;
        longClickDetected = false;
        longClickReported = false;
        shortClickDownReported = false;
        buttonHandled = false;

        if (!buttonHandled) {
          buttonHandled = true;
          shortClickDownReported = true;
          notifyHandlers(ClickType::SHORT_CLICK_DOWN);
        }
      } else if (!pressed && wasPressed) {
        // Button released (rising edge)
        unsigned long pressDuration = currentTime - pressStartTime;

        if (pressDuration >= clickThreshold) {
          if (pressDuration < longClickThreshold) {
            notifyHandlers(ClickType::SHORT_CLICK_UP);
          } else {
            notifyHandlers(ClickType::LONG_CLICK_UP);
          }
        }
        buttonHandled = false;
        shortClickDownReported = false;
        longClickDetected = false;
        longClickReported = false;
      }

      wasPressed = pressed;
    }
  } else if (pressed && wasPressed) {
    // Button is still pressed
    unsigned long pressDuration = currentTime - pressStartTime;

    // Call duringPress continuously while button is held
    notifyHandlers(ClickType::DURING_PRESS);

    // Long click detection
    if (!longClickReported && pressDuration >= longClickThreshold) {
      longClickDetected = true;
      longClickReported = true;
      notifyHandlers(ClickType::LONG_CLICK_DOWN);
    }
  }
}