/**
 * @file pgmspace.h
 * @brief Compatibility header for legacy Arduino AVR libraries (such as LedControl).
 *
 * ESP32 / ESP-IDF Arduino core provides <pgmspace.h> instead of <avr/pgmspace.h>.
 * This header forwards includes of <avr/pgmspace.h> to <pgmspace.h>.
 */

#pragma once
#include <pgmspace.h>

