#include <stdbool.h>
#include <stdio.h>

#include "test_types.h"

bool test_function(bool (*fn)(), char* s){
	if( (*fn)() ){
		printf("OK : %s()\n", s);
		return true;
	}
	else{
		printf("Erreur : %s()\n", s);
		return false;
	}
}

int main(){
	printf("========== TEST TYPES ========================================\n");
	printf("========== Constructors ======================================\n");
	test_function(&test_new_coord, "test_new_coord");
	test_function(&test_new_coord_empty, "test_new_coord_empty");
	test_function(&test_new_field, "test_new_field");
	test_function(&test_new_snake, "test_new_snake");
	printf("========== Objects managment =================================\n");
	test_function(&test_are_equal, "test_are_equal");
	test_function(&test_opposite, "test_opposite");
	test_function(&test_turn_left, "test_turn_left");
	test_function(&test_turn_right, "test_turn_right");
	test_function(&test_get_square_at, "test_get_square_at");
	test_function(&test_set_square_at, "test_set_square_at");
	test_function(&test_get_tail, "test_get_tail");
	test_function(&test_get_head_coord, "test_get_head_coord");
	test_function(&test_get_tail_coord, "test_get_tail_coord");
	test_function(&test_coord_after_dir, "test_coord_after_dir");

	printf("========== TEST GAME ========================================\n");


	//test_function(&, "");
	//test_function(&, "");
	//test_function(&, "");
	//test_function(&, "");
	//test_function(&, "");
	//test_function(&, "");

	return 0;
}