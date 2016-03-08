#include <stdbool.h>
#include <stdio.h>

#include "test_types.h"

bool test_function(bool (*fn)(), char* s){
	if( (*fn)() ){
		printf("OK : %s()\n", s);
		return true;
	}
	else{
		printf("Erreur : %s()", s);
		return false;
	}
}

int main(){
	test_function(&test_new_coord, "new_coord");
	test_function(&test_new_coord_empty, "test_new_coord_empty");
	test_function(&test_new_field, "test_new_field");
	test_function(&test_new_snake, "test_new_snake");
	test_function(&test_are_equal, "test_are_equal");
	test_function(&test_opposite, "test_opposite");
	test_function(&test_set_square_at, "test_set_square_at");
	//test_function(&, "");
	//test_function(&, "");
	//test_function(&, "");
	//test_function(&, "");
	//test_function(&, "");

	return 0;
}