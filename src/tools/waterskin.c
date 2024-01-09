#include <stdio.h>
#include <stdlib.h>
#include <player.h>
#include <map_defs.h>
#include <map.h>
#include <items.h>
#include <inventory.h>
#include <tools/waterskin.h>
#include <entity.h>
#include <item_defs.h>

void fill_container(){
	transform_t pos = get_player_entity()->position;
	item_state_t state = get_current_item()->state;
	int fullness = state.fullness;
	
	if (get_current_item()->item.is_item &&
	    get_current_item()->item.id == WATERSKIN &&
	    get_block(pos.x, pos.y, pos.z).block.id == WATER &&
	    fullness < get_item_properties(WATERSKIN_ITEM).container_size){
		state.fullness += 1;
		set_inventory_item_state(state, get_current_slot());
	}
}

void drink_waterskin(){
	item_state_t state = get_current_item()->state;
	int fullness = state.fullness;
	int thirst = get_player_thirst();
	
	if (get_current_item()->item.id == WATERSKIN &&
	    fullness > 0){
		set_player_thirst(thirst+1);
	}
}

void pour_waterskin(){
	item_state_t state = get_current_item()->state;
	int fullness = state.fullness;	
	
	if (get_current_item()->item.id == WATERSKIN &&
	    fullness > 0){
		state.fullness = 0;
		set_inventory_item_state(state, get_current_slot());
	}
}

