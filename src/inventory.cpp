#include "inventory.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/object.hpp>

using namespace godot;

Inventory::Inventory() {
}

void Inventory::set_slot(const int &p_slot_index, const Ref<Item> &p_item, const int &p_amount) {
    Dictionary slot = slots[p_slot_index];
    slot["item"] = p_item;
    slot["amount"] = godot::Math::min(p_amount , p_item->get_max_stack());
    emit_signal("updated_slot", p_slot_index);
}

int Inventory::add_to_slot(const int &p_slot_index, const Ref<Item> &p_item, const int &p_amount) {
    Dictionary slot = slots[p_slot_index];
    Ref<Item> item = Ref<Item>(slot["item"]);
    if(p_amount <= 0 || (item != p_item && item != NULL)) {
        return p_amount;
    }
    int _amount_to_add = godot::Math::min(p_amount, p_item->get_max_stack() - int(slot["amount"]));
    slot["amount"] = int(slot["amount"]) + _amount_to_add;
    if(_amount_to_add > 0 && item == NULL)
    {
        slot["item"] = p_item;
    }
    emit_signal("updated_slot", p_slot_index);
    return p_amount - _amount_to_add;
}

int Inventory::remove_from_slot(const int &p_slot_index, const Ref<Item> &p_item, const int &p_amount) {
    Dictionary slot = slots[p_slot_index];
    Ref<Item> item = Ref<Item>(slot["item"]);
    if(p_amount <= 0 || (item != p_item && item != NULL))
    {
        return p_amount;
    }
    int amount_to_remove = godot::Math::min(p_amount, int(slot["amount"]));
    slot["amount"] = int(slot["amount"]) - amount_to_remove;
    if(int(slot["amount"]) <= 0) {
        slot["item"] = nullptr;
    }
    emit_signal("updated_slot", p_slot_index);
    return p_amount - amount_to_remove;
}

bool Inventory::is_empty_slot(const int &p_slot_index) const{
    if(slots.size() <= p_slot_index)
        return true;
    Dictionary slot = slots[p_slot_index];
    if(slot.has("amount") && int(slot["amount"]) > 0) {
        return false;
    }
    return true;
}

Inventory::~Inventory() {
}

TypedArray<Dictionary> Inventory::get_slots() const {
	return slots;
}

void Inventory::set_slots(const TypedArray<Dictionary> &p_slots) {
	slots = p_slots;
}

bool Inventory::is_create_slot_if_needed() const {
    return create_slot_if_needed;
}

void Inventory::set_create_slot_if_needed(bool p_create_slot_if_needed) {
    create_slot_if_needed = p_create_slot_if_needed;
}

bool Inventory::is_remove_slot_if_empty() const {
    return remove_slot_if_empty;
}

void Inventory::set_remove_slot_if_empty(bool p_remove_slot_if_empty) {
    remove_slot_if_empty = p_remove_slot_if_empty;
}

int Inventory::add(const Ref<Item> &p_item, const int &p_amount) {
	int amount_in_interact = p_amount;
    int old_amount = get_amount();
	for(int i = 0; i < slots.size(); i++) {
        amount_in_interact = add_to_slot(i, p_item, amount_in_interact);
    }
	if(create_slot_if_needed && amount_in_interact > 0) {
        Dictionary slot;
		slots.append(slot);
        emit_signal("slot_added", slots.size() - 1);
		amount_in_interact = add_to_slot(slots.size() - 1, p_item, amount_in_interact);
    }
    call_events(old_amount);
    return amount_in_interact;
}

int Inventory::add_at(const int &p_slot_index, const Ref<Item> &p_item, const int &p_amount) {
	int amount_in_interact = p_amount;
    int old_amount = get_amount();
	if(p_slot_index < slots.size()) {
        amount_in_interact = add_to_slot(p_slot_index, p_item, amount_in_interact);
        call_events(old_amount);
    }
    return amount_in_interact;
}

int Inventory::remove(const Ref<Item> &p_item, const int &p_amount) {
	int amount_in_interact = p_amount;
    int old_amount = get_amount();
    for(int i = slots.size() - 1; i >= 0; i--) {
		Dictionary slot = slots[i];
		amount_in_interact = remove_from_slot(i, p_item, amount_in_interact);
        if(remove_slot_if_empty && int(slot["amount"]) == 0) {
            slots.remove_at(i);
            emit_signal("slot_removed", i);
        }
    }
	call_events(old_amount);
	return amount_in_interact;
}

int Inventory::remove_at(const int &p_slot_index, const Ref<Item> &p_item, const int &p_amount) {
	int amount_in_interact = p_amount;
    int old_amount = get_amount();
    if(p_slot_index < slots.size()) {
        Dictionary slot = slots[p_slot_index];
		amount_in_interact = remove_from_slot(p_slot_index, p_item, amount_in_interact);
        if(remove_slot_if_empty && int(slot["amount"]) == 0) {
            slots.remove_at(p_slot_index);
            emit_signal("slot_removed", p_slot_index);
        }
	    call_events(old_amount);
    }
	return amount_in_interact;
}

bool Inventory::contains(const Ref<Item> &p_item, const int &p_amount) const {
    int amount_in_inventory = 0;
    for(int i = 0;i < slots.size(); i++) {
        Dictionary slot = slots[i];
        if(Ref<Item>(slot["item"]) == p_item) {
            amount_in_inventory += int(slot["amount"]);
            if(amount_in_inventory > p_amount) {
                return true;
            }
        }
    }
    return false;
}

int Inventory::get_amount_of(const Ref<Item> &p_item, const int &p_amount) const {
    int amount_in_inventory = 0;
    for(int i = 0;i < slots.size(); i++) {
        Dictionary slot = slots[i];
        if(Ref<Item>(slot["item"]) == p_item) {
            amount_in_inventory += int(slot["amount"]);
        }
    }
    return amount_in_inventory;
}

int Inventory::get_amount() const {
    int amount_in_inventory = 0;
    for(int i = 0;i < slots.size(); i++) {
        Dictionary slot = slots[i];
        amount_in_inventory += int(slot["amount"]);
    }
    return amount_in_inventory;
}

void Inventory::clear() {
    slots.clear();
    emit_signal("inventory_changed");
    emit_signal("emptied");
}

void Inventory::call_events(const int &p_old_amount) {
    int actual_amount = get_amount();
    if(p_old_amount != actual_amount) {
        emit_signal("inventory_changed");
        if(is_empty()) {
            emit_signal("emptied");
        }
        if(is_full()) {
            emit_signal("filled");
        }
    }
}

bool Inventory::is_empty() const{
    return get_amount() == 0;
}

bool Inventory::is_full() const{
    for(int i = 0;i < slots.size(); i++) {
        Dictionary slot = slots[i];
        Ref<Item> item = Ref<Item>(slot["item"]);
        if(item == NULL || int(slot["amount"]) < item->get_max_stack()) {
            return false;
        }
    }
    return true;
}

void Inventory::_bind_methods() {
    ClassDB::bind_method(D_METHOD("is_create_slot_if_needed"), &Inventory::is_create_slot_if_needed);
    ClassDB::bind_method(D_METHOD("set_create_slot_if_needed", "create_slot_if_needed"), &Inventory::set_create_slot_if_needed);
    ClassDB::bind_method(D_METHOD("is_remove_slot_if_empty"), &Inventory::is_remove_slot_if_empty);
    ClassDB::bind_method(D_METHOD("set_remove_slot_if_empty", "remove_slot_if_empty"), &Inventory::set_remove_slot_if_empty);
    ClassDB::bind_method(D_METHOD("get_slots"), &Inventory::get_slots);
    ClassDB::bind_method(D_METHOD("set_slots", "slots"), &Inventory::set_slots);
    ClassDB::bind_method(D_METHOD("contains", "item", "amount"), &Inventory::contains, DEFVAL(1));
    ClassDB::bind_method(D_METHOD("get_amount_of", "item", "amount"), &Inventory::get_amount_of, DEFVAL(1));
    ClassDB::bind_method(D_METHOD("add", "item", "amount"), &Inventory::add, DEFVAL(1));
    ClassDB::bind_method(D_METHOD("set_slot", "slot_index", "item", "amount"), &Inventory::set_slot);
    ClassDB::bind_method(D_METHOD("add_at", "slot_index", "item", "amount"), &Inventory::add_at, DEFVAL(1));
    ClassDB::bind_method(D_METHOD("remove", "item", "amount"), &Inventory::remove, DEFVAL(1));
    ClassDB::bind_method(D_METHOD("remove_at", "slot_index", "item", "amount"), &Inventory::remove_at, DEFVAL(1));
    ClassDB::bind_method(D_METHOD("clear"), &Inventory::clear);
    ClassDB::bind_method(D_METHOD("get_amount"), &Inventory::get_amount);
    // Slots
    ClassDB::bind_method(D_METHOD("is_empty_slot", "slot_index"), &Inventory::is_empty_slot);

    ADD_SIGNAL(MethodInfo("inventory_changed"));
    ADD_SIGNAL(MethodInfo("slot_added", PropertyInfo(Variant::INT, "slot_index")));
    ADD_SIGNAL(MethodInfo("slot_removed", PropertyInfo(Variant::INT, "slot_index")));
    ADD_SIGNAL(MethodInfo("filled"));
    ADD_SIGNAL(MethodInfo("emptied"));
    ADD_SIGNAL(MethodInfo("updated_slot", PropertyInfo(Variant::INT, "slot_index")));

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "create_slot_if_needed"), "set_create_slot_if_needed", "is_create_slot_if_needed");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "remove_slot_if_empty"), "set_remove_slot_if_empty", "is_remove_slot_if_empty");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "slots"), "set_slots", "get_slots");
}