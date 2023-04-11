#include "inventory_item.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/global_constants.hpp>

using namespace godot;

void InventoryItem::_bind_methods() { 
    ClassDB::bind_method(D_METHOD("set_max_stack", "max_stack"), &InventoryItem::set_max_stack);
    ClassDB::bind_method(D_METHOD("get_max_stack"), &InventoryItem::get_max_stack);
    ClassDB::bind_method(D_METHOD("set_name", "name"), &InventoryItem::set_name);
    ClassDB::bind_method(D_METHOD("get_name"), &InventoryItem::get_name);
    ClassDB::bind_method(D_METHOD("set_icon", "icon"), &InventoryItem::set_icon);
    ClassDB::bind_method(D_METHOD("get_icon"), &InventoryItem::get_icon);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_stack"), "set_max_stack", "get_max_stack");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_icon", "get_icon");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "objects"), "set_objects", "get_objects");
}

InventoryItem::InventoryItem() {
}

InventoryItem::~InventoryItem() {
}

// Properties
void InventoryItem::set_max_stack(const int &new_max_stack) {
	max_stack = new_max_stack;
}

int InventoryItem::get_max_stack() const {
	return max_stack;
}

void InventoryItem::set_name(const String &new_name) {
    name = new_name;
}

String InventoryItem::get_name() const {
    return name;
}

void InventoryItem::set_icon(const Ref<Texture2D> &p_icon) {
    icon = p_icon;
    notify_property_list_changed();
}

Ref<Texture2D> InventoryItem::get_icon() const {
    return icon;
}
