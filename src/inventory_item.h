#ifndef INVENTORY_ITEM_CLASS_H
#define INVENTORY_ITEM_CLASS_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

class InventoryItem : public Resource {

    GDCLASS(InventoryItem, Resource);

private:
    int max_stack = 0;
    String name;
    Ref<Texture2D> icon;

protected:
    static void _bind_methods();

public:
	InventoryItem();
	~InventoryItem();
    void set_max_stack(const int &new_max_stack);
	int get_max_stack() const;
    void set_name(const String &new_name);
    String get_name() const;
    void set_icon(const Ref<Texture2D> &new_icon);
	Ref<Texture2D> get_icon() const;

};

#endif