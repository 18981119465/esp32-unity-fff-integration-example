#ifndef TESTABLE_H
#define TESTABLE_H

#include <stdbool.h>

/**
 * @brief Calls function for mockable component.
 * @retval true Called function returns zero.
 * @retval false Called functions does not returns zero.
 */
bool testable_calls_mockable(void);

#endif /* TESTABLE_H */
