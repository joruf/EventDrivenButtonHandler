#ifndef MULTI_BUTTON_HANDLER_H
#define MULTI_BUTTON_HANDLER_H

#include "EventDrivenButtonHandler.h"
#include <functional>
#include <vector>

class MultiButtonHandler {
public:
  /// @brief Type definition for the simultaneous click callback function
  /// @param buttonIndex1 The index of the first button in the simultaneous
  /// press
  /// @param buttonIndex2 The index of the second button in the simultaneous
  /// press
  typedef std::function<void(uint8_t buttonIndex1, uint8_t buttonIndex2)>
      SimultaneousClickHandler;

  /// @brief Construct a new MultiButtonHandler object
  /// @param simultaneousThreshold Time threshold in milliseconds for
  /// considering
  ///        button presses as simultaneous (default: 50ms)
  MultiButtonHandler(unsigned long simultaneousThreshold = 50);

  /// @brief Add a button to be monitored for simultaneous presses
  /// @param button Pointer to an EventDrivenButtonHandler instance
  void addButton(EventDrivenButtonHandler *button);

  /// @brief Set the callback function for simultaneous press events
  /// @param handler Callback function to be invoked when simultaneous presses
  /// are detected
  void setSimultaneousClickHandler(SimultaneousClickHandler handler);

  /// @brief Update the state of all buttons - must be called regularly in the
  /// main loop
  void update();

private:
  /// @brief Internal structure to track the state of each button
  struct ButtonState {
    EventDrivenButtonHandler *handler; ///< Pointer to the button handler
    bool wasPressed;                   ///< Previous pressed state
    unsigned long pressStartTime; ///< Timestamp when the button was pressed
  };

  std::vector<ButtonState>
      buttons; ///< List of tracked buttons and their states
  SimultaneousClickHandler
      simultaneousHandler; ///< Callback for simultaneous press events
  unsigned long
      simultaneousThreshold; ///< Time window for simultaneous detection
  bool simultaneousReported; ///< Flag to prevent multiple triggers

  /// @brief Internal method to check for and handle simultaneous button presses
  void checkSimultaneousPress();
};

#endif