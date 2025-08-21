#ifndef EVENT_DRIVEN_BUTTON_HANDLER_H
#define EVENT_DRIVEN_BUTTON_HANDLER_H

#include <Arduino.h>
#include <functional>
#include <vector>

/**
 * @enum ClickType
 * @brief Enumeration of different button event types
 *
 * Defines the various events that can be triggered by a button interaction.
 */
enum class ClickType {
  SHORT_CLICK_DOWN, // Triggered when the button is pressed briefly (on press)
  SHORT_CLICK_UP,   // Triggered when a short click is released
  LONG_CLICK_DOWN, // Triggered when the button is held long (when reaching long
                   // click threshold)
  LONG_CLICK_UP,   // Triggered when a long click is released
  DURING_PRESS     // Continuously triggered while the button is being held down
};

/**
 * @class SingleButtonHandler
 * @brief Advanced button handler class with callback functionality
 *
 * This class enables detection of various button interactions (short/long
 * clicks) and allows registration of callback functions for these events.
 */
class SingleButtonHandler {
public:
  /**
   * @brief Constructor for the button handler
   * @param pin The GPIO pin where the button is connected
   * @param activeLow Specifies if the button operates in active-low mode
   * (default: true)
   * @param pullup Enables the internal pullup resistor (default: true)
   */
  SingleButtonHandler(uint8_t pin, bool activeLow = true, bool pullup = true);

  /**
   * @brief Destructor for the button handler
   */
  ~SingleButtonHandler();

  /**
   * @brief Sets the threshold for click detection
   * @param threshold Time in milliseconds after which a press is recognized as
   * a valid click
   *
   * @note Difference from setDebounceTime:
   * - clickThreshold: Minimum press duration to be considered a valid click
   * - debounceTime: Minimum time between state changes to filter out mechanical
   * bouncing
   */
  void setClickThreshold(unsigned long threshold);

  /**
   * @brief Sets the threshold for long click detection
   * @param threshold Time in milliseconds after which a press is recognized as
   * a long click
   */
  void setLongClickThreshold(unsigned long threshold);

  /**
   * @brief Sets the debounce time for button state changes
   * @param time Time in milliseconds to ignore state changes after a detected
   * change
   *
   * @note Difference from setClickThreshold:
   * - debounceTime: Filters out mechanical bouncing of the button
   * - clickThreshold: Defines the minimum duration for a valid click
   */
  void setDebounceTime(unsigned long time);

  /**
   * @brief Adds a callback function to handle button events
   * @param handler Function to be called when a button event occurs
   */
  void addClickHandler(std::function<void(ClickType)> handler);

  /**
   * @brief Removes all registered callback functions
   */
  void removeClickHandlers();

  /**
   * @brief Gets the current pressed state of the button
   * @return True if the button is pressed, false otherwise
   */
  bool getPressedState() const;

  /**
   * @brief Processes button state changes - must be called regularly in the
   * main loop
   */
  void update();

  /**
   * @brief Reads the current state of the button
   * @return True if the button is pressed, false otherwise
   */
  bool isPressed() const;

private:
  uint8_t pin;                      // GPIO pin number for the button
  bool activeLow;                   // Whether the button is active low
  unsigned long clickThreshold;     // Minimum press duration for a valid click
  unsigned long longClickThreshold; // Minimum press duration for a long click
  unsigned long debounceTime; // Debounce time to filter out mechanical bouncing
  unsigned long pressStartTime; // Timestamp when the button was pressed
  unsigned long lastEventTime;  // Timestamp of the last processed event
  bool wasPressed;              // Previous state of the button
  bool longClickDetected;       // Whether a long click has been detected
  bool longClickReported;       // Whether a long click has been reported
  bool shortClickDownReported;  // Whether a short click down has been reported
  bool buttonHandled; // Whether the current button press has been handled

  std::vector<std::function<void(ClickType)>>
      handlers; // List of registered callback functions

  /**
   * @brief Notifies all registered handlers of a button event
   * @param clickType Type of button event to notify
   */
  void notifyHandlers(ClickType clickType);
};

#endif