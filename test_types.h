#ifndef H_TEST_TYPES
#define H_TEST_TYPES

#include "types.h"

#define DO_NOT_DISPLAY

// PROTOTYPES ==========================================================
// Constructors ========================================================
bool test_new_coord();
bool test_new_coord_empty();
bool test_new_field();
bool test_new_snake();

// Objects managment ===================================================
bool test_are_equal();
bool test_opposite();
bool test_turn_left();
bool test_turn_right();
bool test_get_square_at();
bool test_set_square_at();
bool test_get_tail();
bool test_get_head_coord();
bool test_get_tail_coord();
bool test_coord_after_dir();

#endif