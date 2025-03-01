#include "cooldown_ticks.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void UtilityAIBTCooldownTicks::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_cooldown_ticks", "cooldown_ticks"), &UtilityAIBTCooldownTicks::set_cooldown_ticks);
	ClassDB::bind_method(D_METHOD("get_cooldown_ticks"), &UtilityAIBTCooldownTicks::get_cooldown_ticks);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cooldown_ticks", PROPERTY_HINT_RANGE, "0,1000,or_greater"), "set_cooldown_ticks", "get_cooldown_ticks");

	ClassDB::bind_method(D_METHOD("set_cooldown_return_value", "cooldown_return_value"), &UtilityAIBTCooldownTicks::set_cooldown_return_value);
	ClassDB::bind_method(D_METHOD("get_cooldown_return_value"), &UtilityAIBTCooldownTicks::get_cooldown_return_value);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cooldown_return_value", PROPERTY_HINT_ENUM, "Running:0,Success:1,Failure:-1"), "set_cooldown_return_value", "get_cooldown_return_value");

	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_current_cooldown_ticks", "current_cooldown_ticks"), &UtilityAIBTCooldownTicks::set_current_cooldown_ticks);
	ClassDB::bind_method(D_METHOD("get_current_cooldown_ticks"), &UtilityAIBTCooldownTicks::get_current_cooldown_ticks);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_cooldown_ticks", PROPERTY_HINT_RANGE, "0,1000,or_greater"), "set_current_cooldown_ticks", "get_current_cooldown_ticks");

	//ClassDB::bind_method(D_METHOD("_tick", "blackboard", "delta"), &UtilityAIBTCooldownTicks::tick);
}

// Constructor and destructor.

UtilityAIBTCooldownTicks::UtilityAIBTCooldownTicks() {
	_cooldown_ticks = 0;
	_current_cooldown_ticks = 0;
	_cooldown_return_value = Status::FAILURE;
}

UtilityAIBTCooldownTicks::~UtilityAIBTCooldownTicks() {
}

// Getters and Setters.

void UtilityAIBTCooldownTicks::set_cooldown_ticks(int cooldown_ticks) {
	_cooldown_ticks = cooldown_ticks;
}

int UtilityAIBTCooldownTicks::get_cooldown_ticks() const {
	return _cooldown_ticks;
}

void UtilityAIBTCooldownTicks::set_current_cooldown_ticks(int current_cooldown_ticks) {
	_current_cooldown_ticks = get_cooldown_ticks() - current_cooldown_ticks;
}

int UtilityAIBTCooldownTicks::get_current_cooldown_ticks() const {
	return get_cooldown_ticks() - _current_cooldown_ticks;
}

void UtilityAIBTCooldownTicks::set_cooldown_return_value(Status cooldown_return_value) {
	_cooldown_return_value = cooldown_return_value;
}

UtilityAIBTNodes::Status UtilityAIBTCooldownTicks::get_cooldown_return_value() const {
	return _cooldown_return_value;
}

// Handling methods.

UtilityAIBTNodes::Status UtilityAIBTCooldownTicks::tick(Variant blackboard, float delta) {
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}

	if (_current_cooldown_ticks > 0) {
		_current_cooldown_ticks--;
		return _cooldown_return_value;
	}
	//emit_signal("btnode_ticked", blackboard, delta);
	for (unsigned int i = 0; i < _num_child_btnodes; ++i) {
		UtilityAIBTNodes *btnode = _child_btnodes[i];
		if (!btnode->get_is_active()) {
			continue;
		}
		Status result = btnode->tick(blackboard, delta);

		if (result != Status::RUNNING) {
			_current_cooldown_ticks = _cooldown_ticks;

			set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
			set_tick_result(result);
			//emit_signal("btnode_exited", blackboard, delta);
			return result;
		} else {
			set_internal_status(BT_INTERNAL_STATUS_TICKED);
			set_tick_result(result);
			return result;
		}
		break;
	}

	_current_cooldown_ticks = _cooldown_ticks;
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	set_tick_result(Status::FAILURE);
	//emit_signal("btnode_exited", blackboard, delta);
	return Status::FAILURE;
}
