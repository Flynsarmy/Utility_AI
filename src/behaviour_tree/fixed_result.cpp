#include "fixed_result.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void UtilityAIBTFixedResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_fixed_result", "fixed_result"), &UtilityAIBTFixedResult::set_fixed_result);
	ClassDB::bind_method(D_METHOD("get_fixed_result"), &UtilityAIBTFixedResult::get_fixed_result);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "fixed_result", PROPERTY_HINT_ENUM, "Running:0,Success:1,Failure:-1"), "set_fixed_result", "get_fixed_result");

	//ClassDB::bind_method(D_METHOD("_tick", "blackboard", "delta"), &UtilityAIBTFixedResult::tick);
}

// Constructor and destructor.

UtilityAIBTFixedResult::UtilityAIBTFixedResult() {
	_fixed_result = Status::SUCCESS;
}

UtilityAIBTFixedResult::~UtilityAIBTFixedResult() {
}

// Getters and Setters.

void UtilityAIBTFixedResult::set_fixed_result(Status fixed_result) {
	_fixed_result = fixed_result;
}

UtilityAIBehaviourTreeNodes::Status UtilityAIBTFixedResult::get_fixed_result() const {
	return _fixed_result;
}

// Handling methods.

UtilityAIBehaviourTreeNodes::Status UtilityAIBTFixedResult::tick(Variant blackboard, float delta) {
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}

	set_tick_result(_fixed_result);
	for (int i = 0; i < get_child_count(); ++i) {
		Node *node = get_child(i);
		if (UtilityAIBehaviourTreeNodes *btnode = godot::Object::cast_to<UtilityAIBehaviourTreeNodes>(node)) {
			if (!btnode->get_is_active()) {
				continue;
			}
			btnode->tick(blackboard, delta);
			//emit_signal("btnode_ticked", blackboard, delta);
			break;
		}
	}
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	//emit_signal("btnode_exited", blackboard, delta);
	return _fixed_result;
}
