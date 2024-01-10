#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <items.h>
#include <stdbool.h>
#include <mjson.h>
#include <map.h>

item_map_t item_map;

void load_item_properties(char * path){
	int item_def_size = 1024;
	char * item_def_copy = malloc(item_def_size);
	FILE * def_file;
	int current_char = 0;
  
	if ((def_file = fopen(path, "r")) == NULL){
		fprintf(stderr, "Unable to load items defs\n");
		exit(-1);
	}

	// make a copy of the data, so that it can be tokenized
	do {
		char def_char = fgetc(def_file);

		if (current_char == item_def_size){
			item_def_size += 256;
			item_def_copy = realloc(item_def_copy, item_def_size);
		}
    
		if (feof(def_file)){
			break;
		} else {
			item_def_copy[current_char] = def_char;
			current_char++;
		}
	} while (true);

	const struct json_attr_t json_subobjects[] = {
		{"name", t_string, STRUCTOBJECT(item_data_t, name), .len = ITEM_NAME_LENGTH},
		{"absorbs_liquid", t_boolean, STRUCTOBJECT(item_data_t, absorbs_liquid)},
		{"container_size", t_integer, STRUCTOBJECT(item_data_t, container_size)},
		{"block_id", t_integer, STRUCTOBJECT(item_data_t, block_id)},
		{"id", t_integer, STRUCTOBJECT(item_data_t, id)},
		{NULL},
	};

	const struct json_attr_t json_subobject_list[] = {
		{"class", t_check,.dflt.check = "ITEMS"},
		{"items", t_array, STRUCTARRAY(item_map.data,
					       json_subobjects,
					       &item_map.size)},
		{NULL},
	};

	memset(&item_map, '\0', sizeof(item_map));
	json_read_object(item_def_copy, json_subobject_list, NULL);

	printf("Successfully loaded item defs\n");
  
	free(item_def_copy);
	fclose(def_file);

}

item_data_t get_item_properties(int item_id){
	return item_map.data[item_id];
}

int get_item_id(int block_id){
	if (get_block_properties(block_id).is_item){
		for (int id = 0 ; id < ITEMS_AMOUNT ; id++){
			if (get_item_properties(id).block_id == block_id){
				return id;
			}
		}
		return -1;
	}
	return -1;
}
