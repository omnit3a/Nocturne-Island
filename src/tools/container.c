#include <stdio.h>
#include <stdlib.h>
#include <player.h>
#include <map_defs.h>
#include <map.h>
#include <items.h>
#include <inventory.h>
#include <tools/container.h>
#include <entity.h>
#include <item_defs.h>

void fill_container(){
	item_state_t state = get_current_item()->state;
	int fullness = state.fullness;

	if (!get_current_item()->item.is_item){
		return;
	}

	int item_id = get_item_id(get_current_item()->item.id);
	if (item_id < 0){
		return;
	}
	
	if (get_item_properties(item_id).container_size < 0){
		return;
	}
	
	if (fullness < get_item_properties(item_id).container_size){
		state.fullness += 1;
		set_inventory_item_state(state, get_current_slot());
	}
}

void drink_container(){
	item_state_t state = get_current_item()->state;
	int fullness = state.fullness;

	if (!get_current_item()->item.is_item){
		return;
	}

	int item_id = get_item_id(get_current_item()->item.id);
	if (item_id < 0){
		return;
	}
	
	if (get_item_properties(item_id).container_size < 0){
		return;
	}	

	if (fullness <= 0){
		return;
	}
	
	/* todo */
}

void pour_container(){
	item_state_t state = get_current_item()->state;
	int fullness = state.fullness;	

	if (!get_current_item()->item.is_item){
		return;
	}

	int item_id = get_item_id(get_current_item()->item.id);
	if (item_id < 0){
		return;
	}
	
	if (get_item_properties(item_id).container_size < 0){
		return;
	}

	if (fullness <= 0){
		return;
	}
	
	state.fullness = 0;
	set_inventory_item_state(state, get_current_slot());
}

