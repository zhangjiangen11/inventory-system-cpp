#ifndef INVENTORY_CLASS_H
#define INVENTORY_CLASS_H

#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include "inventory_item.h"
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class Inventory : public Node {

	GDCLASS(Inventory, Node);
	Array slots;
	// bool has_slots_capacity;
	// int slots_capacity = 16;
	bool create_slot_if_needed = false;
	bool remove_slot_if_empty = false;
    
public:
	Inventory();
	~Inventory();
	TypedArray<Dictionary> get_slots() const;
	void set_slots(const TypedArray<Dictionary> &p_slots);
	bool is_create_slot_if_needed() const;
	void set_create_slot_if_needed(bool p_create_slot_if_needed);
	bool is_remove_slot_if_empty() const;
	void set_remove_slot_if_empty(bool p_remove_slot_if_empty);
	void set_slot(const int &p_slot_index, const Ref<InventoryItem> &p_item, const int &p_amount);
	int add(const Ref<InventoryItem> &p_item, const int &p_amount = 1);
	int add_at(const int &p_slot_index, const Ref<InventoryItem> &p_item, const int &p_amount = 1);
	int remove(const Ref<InventoryItem> &p_item, const int &p_amount = 1);
	int remove_at(const int &p_slot_index, const Ref<InventoryItem> &p_item, const int &p_amount = 1);
	bool contains(const Ref<InventoryItem> &p_item, const int &p_amount = 1) const;
	int get_amount_of(const Ref<InventoryItem> &p_item, const int &p_amount = 1) const;
	void clear();
	int get_amount() const;
	bool is_empty() const;
	bool is_full() const;
	// Slots
	bool is_empty_slot(const int &slot_index) const;

protected:
	static void _bind_methods();

private:
	// SLOTS
	int add_to_slot(const int &p_slot_index, const Ref<InventoryItem> &p_item, const int &p_amount);
	int remove_from_slot(const int &p_slot_index, const Ref<InventoryItem> &p_item, const int &p_amount);
	// SIGNALS
	void call_events(const int &p_old_amount);

};

#endif // INVENTORY_CLASS_H
