#include <stdio.h>
#include <stdlib.h>
#include <player.h>
#include <map_defs.h>
#include <map.h>
#include <items.h>
#include <inventory.h>
#include <tools/waterskin.h>
#include <entity.h>

void fill_waterskin(){
	transform_t pos = get_player_entity()->position;
	item_state_t state = get_current_item()->state;
	int fullness = state.fullness;
	
	if (get_current_item()->item.is_item &&
	    get_current_item()->item.id == WATERSKIN &&
	    get_block(pos.x, pos.y, pos.z).block.id == WATER &&
	    fullness < get_item_properties(WATERSKIN).container_size){
		state.fullness += 1;
		set_inventory_item_state(state, get_current_slot());
		printf("+1\n");
	} else {
		printf(":(\n");
	}
}

void drink_waterskin();
void pour_waterskin();

