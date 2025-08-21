#include "MultiButtonHandler.h"
#include <Arduino.h>

/**
 * @brief Construct a new MultiButtonHandler object
 * @param simultaneousThreshold Time threshold in milliseconds for considering
 *        button presses as simultaneous (default: 50ms)
 * @param longThreshold Time threshold in milliseconds for considering
 *        a press as a long press (default: 1000ms)
 */
MultiButtonHandler::MultiButtonHandler(unsigned long simultaneousThreshold,
                                       unsigned long longThreshold)
    : simultaneousThreshold(simultaneousThreshold),
      longThreshold(longThreshold), comboActive(false),
      longPressReported(false) {}

/**
 * @brief Add a button to be monitored for simultaneous presses
 * @param button Pointer to an EventDrivenButtonHandler instance
 */
void MultiButtonHandler::addButton(EventDrivenButtonHandler *button) {
  ButtonState s;
  s.btn = button;
  s.pressed = button->isPressed();
  s.pressStart = s.pressed ? millis() : 0;
  s.releaseTime = 0;
  s.longReported = false;
  buttons.push_back(s);
}

/**
 * @brief Set the callback function for simultaneous press events
 * @param handler Callback function to be invoked when simultaneous presses are
 * detected
 */
void MultiButtonHandler::addClickHandler(ComboHandlerFn handler) {
  comboHandlers.push_back(handler);
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
  unsigned long now = millis();

  // Update state for each button
  for (auto &s : buttons) {
    bool currentlyPressed = s.btn->isPressed();

    // Rising edge: press started
    if (currentlyPressed && !s.pressed) {
      s.pressed = true;
      s.pressStart = now;
      s.longReported = false;
    }
    // Falling edge: released
    else if (!currentlyPressed && s.pressed) {
      s.pressed = false;
      s.releaseTime = now;
      s.longReported = false;
    }
  }

  // Check for combo events
  bool allPressed = allButtonsPressedWithinThreshold();
  bool allReleased = allButtonsReleasedWithinThreshold();

  if (allPressed && !comboActive) {
    // New combo detected - report SHORT_CLICK_DOWN
    comboActive = true;
    longPressReported = false;
    dispatchComboEvent(ClickType::SHORT_CLICK_DOWN);
  } else if (allPressed && comboActive && !longPressReported) {
    // Check for long press
    unsigned long pressDuration = now - buttons[0].pressStart;
    if (pressDuration >= longThreshold) {
      longPressReported = true;
      dispatchComboEvent(ClickType::LONG_CLICK_DOWN);
    }
  } else if (allReleased && comboActive) {
    // Combo ended - report appropriate UP event
    unsigned long pressDuration =
        buttons[0].releaseTime - buttons[0].pressStart;
    if (pressDuration >= longThreshold) {
      dispatchComboEvent(ClickType::LONG_CLICK_UP);
    } else {
      dispatchComboEvent(ClickType::SHORT_CLICK_UP);
    }
    comboActive = false;
    longPressReported = false;
  }
}

/**
 * @brief Check if all buttons were pressed within the time threshold
 * @return True if all buttons are pressed and within the time threshold
 */
bool MultiButtonHandler::allButtonsPressedWithinThreshold() {
  if (buttons.empty())
    return false;

  // Find the latest press time
  unsigned long latestPress = 0;
  for (const auto &s : buttons) {
    if (!s.pressed)
      return false;
    if (s.pressStart > latestPress)
      latestPress = s.pressStart;
  }

  // Check if all buttons were pressed within the threshold
  for (const auto &s : buttons) {
    if (latestPress - s.pressStart > simultaneousThreshold)
      return false;
  }

  return true;
}

/**
 * @brief Check if all buttons were released within the time threshold
 * @return True if all buttons are released and within the time threshold
 */
bool MultiButtonHandler::allButtonsReleasedWithinThreshold() {
  if (buttons.empty())
    return false;

  // Find the latest release time
  unsigned long latestRelease = 0;
  for (const auto &s : buttons) {
    if (s.pressed)
      return false;
    if (s.releaseTime > latestRelease)
      latestRelease = s.releaseTime;
  }

  // Check if all buttons were released within the threshold
  for (const auto &s : buttons) {
    if (latestRelease - s.releaseTime > simultaneousThreshold)
      return false;
  }

  return true;
}

/**
 * @brief Dispatch combo event to all registered handlers
 * @param clickType Type of click event to dispatch
 */
void MultiButtonHandler::dispatchComboEvent(ClickType clickType) {
  for (auto &handler : comboHandlers) {
    handler(clickType);
  }
}