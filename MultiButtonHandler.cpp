#include "MultiButtonHandler.h"

/**
 * @brief Construct a new MultiButtonHandler object
 * @param simultaneousThreshold Time threshold in milliseconds for considering
 *        button presses as simultaneous (default: 50ms)
 */
MultiButtonHandler::MultiButtonHandler(unsigned long simultaneousThreshold)
    : simultaneousThreshold(simultaneousThreshold),
      simultaneousReported(false) {}

/**
 * @brief Add a button to be monitored for simultaneous presses
 * @param button Pointer to an EventDrivenButtonHandler instance
 */
void MultiButtonHandler::addButton(EventDrivenButtonHandler *button) {
  ButtonState state;
  state.handler = button;
  state.wasPressed = false;
  state.pressStartTime = 0;
  buttons.push_back(state);
}

/**
 * @brief Set the callback function for simultaneous press events
 * @param handler Callback function to be invoked when simultaneous presses are
 * detected
 */
void MultiButtonHandler::setSimultaneousClickHandler(
    SimultaneousClickHandler handler) {
  simultaneousHandler = handler;
}

/**
 * @brief Update the state of all buttons - must be called regularly in the main
 * loop
 *
 * This method updates all registered buttons and checks for simultaneous
 * presses. It should be called frequently in the main loop to ensure responsive
 * button handling.
 */
void MultiButtonHandler::update() {
  for (auto &btnState : buttons) {
    btnState.handler->update();
  }
  checkSimultaneousPress();
}

/**
 * @brief Check for simultaneous button presses
 *
 * This internal method detects when multiple buttons are pressed within the
 * configured time threshold. It prevents multiple triggers for the same event
 * and resets the detection state when all buttons are released.
 */
void MultiButtonHandler::checkSimultaneousPress() {
  unsigned long currentTime = millis();
  std::vector<uint8_t> currentlyPressed;

  // Capture all currently pressed buttons
  for (uint8_t i = 0; i < buttons.size(); i++) {
    bool isPressed = buttons[i].handler->getPressedState();

    if (isPressed && !buttons[i].wasPressed) {
      // Button was just pressed - record the press time
      buttons[i].pressStartTime = currentTime;
    }

    buttons[i].wasPressed = isPressed;

    if (isPressed) {
      currentlyPressed.push_back(i);
    }
  }

  // Reset reported status when no buttons are pressed
  if (currentlyPressed.empty()) {
    simultaneousReported = false;
    return;
  }

  // Check for simultaneous pressing of at least two buttons
  if (currentlyPressed.size() >= 2 && !simultaneousReported) {
    unsigned long oldestPress = currentTime;
    unsigned long newestPress = 0;

    // Find the oldest and newest press times
    for (auto idx : currentlyPressed) {
      if (buttons[idx].pressStartTime < oldestPress) {
        oldestPress = buttons[idx].pressStartTime;
      }
      if (buttons[idx].pressStartTime > newestPress) {
        newestPress = buttons[idx].pressStartTime;
      }
    }

    // Check if the time difference is within the threshold
    if (newestPress - oldestPress <= simultaneousThreshold) {
      simultaneousReported = true;
      if (simultaneousHandler) {
        simultaneousHandler(currentlyPressed[0], currentlyPressed[1]);
      }
    }
  }
}