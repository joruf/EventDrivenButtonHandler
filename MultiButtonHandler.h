#ifndef MULTI_BUTTON_HANDLER_H
#define MULTI_BUTTON_HANDLER_H

#include "SingleButtonHandler.h"
#include <functional>
#include <vector>

/**
 * MultiButtonHandler
 *
 * Detects events occurring across multiple SingleButtonHandler instances.
 * It supports registering handlers for combinations of buttons and will
 * forward a ClickType (SHORT_CLICK_DOWN, SHORT_CLICK_UP, LONG_CLICK_DOWN,
 * LONG_CLICK_UP) to callbacks just like
 * SingleButtonHandler::addClickHandler does for single buttons.
 *
 * The API intentionally mirrors SingleButtonHandler's callback signature
 * by providing callbacks that receive a ClickType. For multi-button combos
 * the callback is registered for all buttons and is invoked with the ClickType
 * when all buttons are pressed or released within the simultaneous threshold.
 */
class MultiButtonHandler {
public:
  // @brief Handler signature: receives the ClickType of the event
  typedef std::function<void(ClickType)> ComboHandlerFn;

  /**
   * @brief Construct a new MultiButtonHandler object
   * @param simultaneousThreshold Time threshold in milliseconds for considering
   *        button presses as simultaneous (default: 50ms)
   * @param longThreshold Time threshold in milliseconds for considering
   *        a press as a long press (default: 1000ms)
   */
  MultiButtonHandler(unsigned long simultaneousThreshold = 50,
                     unsigned long longThreshold = 1000);

  /**
   * @brief Add a button to be monitored for simultaneous presses
   * @param button Pointer to an SingleButtonHandler instance
   */
  void addButton(SingleButtonHandler *button);

  /**
   * @brief Set the callback function for simultaneous press events
   * @param handler Callback function to be invoked when simultaneous presses
   * are detected
   */
  void addClickHandler(ComboHandlerFn handler);

  /**
   * @brief Update the state of all buttons - must be called regularly in the
   * main loop
   *
   * This method updates all registered buttons and checks for simultaneous
   * presses. It should be called frequently in the main loop to ensure
   * responsive button handling.
   */
  void update();

private:
  // @brief Internal structure to track the state of each button
  struct ButtonState {
    SingleButtonHandler *btn = nullptr; // Pointer to the button handler
    bool pressed = false;                    // Current pressed state
    unsigned long pressStart = 0;  // Timestamp when the button was pressed
    unsigned long releaseTime = 0; // Timestamp when the button was released
    bool longReported = false;     // Flag to track if long press was reported
  };

  std::vector<ButtonState>
      buttons; // List of tracked buttons and their states
  std::vector<ComboHandlerFn>
      comboHandlers; // List of callback functions for combo events

  unsigned long
      simultaneousThreshold;   // Time window for simultaneous detection
  unsigned long longThreshold; // Time threshold for long press detection

  bool comboActive;       // Flag to prevent multiple triggers
  bool longPressReported; // Flag to track if long press was reported

  /**
   * @brief Check if all buttons were pressed within the time threshold
   * @return True if all buttons are pressed and within the time threshold
   */
  bool allButtonsPressedWithinThreshold();

  /**
   * @brief Check if all buttons were released within the time threshold
   * @return True if all buttons are released and within the time threshold
   */
  bool allButtonsReleasedWithinThreshold();

  /**
   * @brief Dispatch combo event to all registered handlers
   * @param clickType Type of click event to dispatch
   */
  void dispatchComboEvent(ClickType clickType);
};

#endif