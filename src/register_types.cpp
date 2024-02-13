#include "register_types.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "craft_station_type.h"
#include "item.h"
#include "item_category.h"
#include "item_definition.h"
#include "recipe.h"
#include "slot.h"
#include "inventory_database.h"
#include "node_inventories.h"
#include "inventory.h"
#include "categorized_slot.h"
#include "hotbar.h"


using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<ItemCategory>();
	ClassDB::register_class<ItemDefinition>();
	ClassDB::register_class<CraftStationType>();
	ClassDB::register_class<Item>();
	ClassDB::register_class<Slot>();
	ClassDB::register_class<Recipe>();
	ClassDB::register_class<InventoryDatabase>();
	ClassDB::register_class<NodeInventories>();
	ClassDB::register_class<Inventory>();
	ClassDB::register_class<CategorizedSlot>();
	ClassDB::register_class<Hotbar>();
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization
GDExtensionBool GDE_EXPORT inventory_system_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_gdextension_types);
	init_obj.register_terminator(uninitialize_gdextension_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}