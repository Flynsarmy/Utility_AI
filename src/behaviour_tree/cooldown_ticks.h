#ifndef UtilityAIBTCooldownTicks_H_INCLUDED
#define UtilityAIBTCooldownTicks_H_INCLUDED

#include "decorator_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class UtilityAIBTCooldownTicks : public UtilityAIBTDecoratorNodes {
	GDCLASS(UtilityAIBTCooldownTicks, UtilityAIBTDecoratorNodes)

private:
	int _cooldown_ticks;
	int _current_cooldown_ticks;

	UtilityAI::Status _cooldown_return_value;

protected:
	static void _bind_methods();

public:
	UtilityAIBTCooldownTicks();
	~UtilityAIBTCooldownTicks();

	// Getters and setters for attributes.

	void set_cooldown_ticks(int cooldown_ticks);
	int get_cooldown_ticks() const;

	void set_current_cooldown_ticks(int cooldown_ticks);
	int get_current_cooldown_ticks() const;

	void set_cooldown_return_value(UtilityAI::Status cooldown_return_value);
	UtilityAI::Status get_cooldown_return_value() const;

	// Handling functions.

	virtual UtilityAI::Status tick(Variant user_data, float delta) override;
};

} //namespace godot

#endif
