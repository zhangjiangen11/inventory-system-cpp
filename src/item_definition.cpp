#include "item_definition.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/global_constants.hpp>

using namespace godot;

void ItemDefinition::_bind_methods() { 
    ClassDB::bind_method(D_METHOD("set_max_stack", "max_stack"), &ItemDefinition::set_max_stack);
    ClassDB::bind_method(D_METHOD("get_max_stack"), &ItemDefinition::get_max_stack);
    ClassDB::bind_method(D_METHOD("set_name", "name"), &ItemDefinition::set_name);
    ClassDB::bind_method(D_METHOD("get_name"), &ItemDefinition::get_name);
    ClassDB::bind_method(D_METHOD("set_icon", "icon"), &ItemDefinition::set_icon);
    ClassDB::bind_method(D_METHOD("get_icon"), &ItemDefinition::get_icon);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_stack"), "set_max_stack", "get_max_stack");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_icon", "get_icon");
    BIND_CONSTANT(NONE);
}

ItemDefinition::ItemDefinition() {
}

ItemDefinition::~ItemDefinition() {
}

// Properties
void ItemDefinition::set_max_stack(const int &new_max_stack) {
	max_stack = new_max_stack;
}

int ItemDefinition::get_max_stack() const {
	return max_stack;
}

void ItemDefinition::set_name(const String &new_name) {
    name = new_name;
}

String ItemDefinition::get_name() const {
    return name;
}

void ItemDefinition::set_icon(const Ref<Texture2D> &p_icon) {
    icon = p_icon;
    notify_property_list_changed();
}

Ref<Texture2D> ItemDefinition::get_icon() const {
    return icon;
}
