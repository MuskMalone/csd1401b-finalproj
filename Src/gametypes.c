#include "gametypes.h"

int insert_to_entity_array(entity_type type, Entity entities[], entity_struct(*init_func)(void)) {
	// loops thru the array to see if there is any gap
	for (int i = 0; i < ENTITY_CAP; ++i) {
		if (entities[i].type == entity_null) {		
			entities[i] = (Entity){
				type,
				init_func()
			};
			return i;
		}
	}
	return -1;
}