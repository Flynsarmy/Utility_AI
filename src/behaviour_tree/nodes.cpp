#include "nodes.h"
#include "../agent_behaviours/considerations.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void UtilityAIBTNodes::_bind_methods() {
	BIND_ENUM_CONSTANT(RUNNING);
	BIND_ENUM_CONSTANT(FAILURE);
	BIND_ENUM_CONSTANT(SUCCESS);
	BIND_ENUM_CONSTANT(SKIP);

	ClassDB::bind_method(D_METHOD("set_reset_rule", "reset_rule"), &UtilityAIBTNodes::set_reset_rule);
	ClassDB::bind_method(D_METHOD("get_reset_rule"), &UtilityAIBTNodes::get_reset_rule);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "reset_rule", PROPERTY_HINT_ENUM, "WhenTicked:0,WhenCompleted:1,WhenTickedAfterBeingCompleted:2,Never:3"), "set_reset_rule", "get_reset_rule");

	ClassDB::bind_method(D_METHOD("set_evaluation_method", "evaluation_method"), &UtilityAIBTNodes::set_evaluation_method);
	ClassDB::bind_method(D_METHOD("get_evaluation_method"), &UtilityAIBTNodes::get_evaluation_method);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "evaluation_method", PROPERTY_HINT_ENUM, "Sum:0,Min:1,Max:2,Mean:3,Multiply:4,FirstNonZero:5"), "set_evaluation_method", "get_evaluation_method");

	ClassDB::bind_method(D_METHOD("set_considerations", "considerations"), &UtilityAIBTNodes::set_considerations);
	ClassDB::bind_method(D_METHOD("get_considerations"), &UtilityAIBTNodes::get_considerations);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "considerations", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "UtilityAIConsiderationResources")), "set_considerations", "get_considerations");

	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_score", "score"), &UtilityAIBTNodes::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &UtilityAIBTNodes::get_score);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score", PROPERTY_HINT_NONE), "set_score", "get_score");

	ClassDB::bind_method(D_METHOD("set_tick_result", "tick_result"), &UtilityAIBTNodes::set_tick_result);
	ClassDB::bind_method(D_METHOD("get_tick_result"), &UtilityAIBTNodes::get_tick_result);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tick_result", PROPERTY_HINT_ENUM, "Running:0,Success:1,Failure:-1,Skip:-2"), "set_tick_result", "get_tick_result");

	ClassDB::bind_method(D_METHOD("set_internal_status", "internal_status"), &UtilityAIBTNodes::set_internal_status);
	ClassDB::bind_method(D_METHOD("get_internal_status"), &UtilityAIBTNodes::get_internal_status);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "internal_status", PROPERTY_HINT_ENUM, "Unticked:0,Ticked:1,Completed:2"), "set_internal_status", "get_internal_status");

	// ClassDB::bind_method(D_METHOD("_tick", "blackboard", "delta"), &UtilityAIBTNodes::tick);

	// Signals.
	//ADD_SIGNAL(MethodInfo("btnode_entered", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
	//ADD_SIGNAL(MethodInfo("btnode_ticked", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
	//ADD_SIGNAL(MethodInfo("btnode_exited", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
}

// Constructor and destructor.

UtilityAIBTNodes::UtilityAIBTNodes() {
	_score = 0.0;
	_evaluation_method = UtilityAIBTNodesEvaluationMethod::Multiply;
	_invert_score = false;
	_tick_result = Status::SUCCESS;
	_internal_status = BT_INTERNAL_STATUS_UNTICKED;
	_reset_rule = UtilityAIBTNodesResetRule::WHEN_TICKED_AFTER_BEING_COMPLETED;
	_has_reset_rule_changed = false;
	//_is_first_tick = true;
	_num_child_btnodes = 0;
	_num_child_considerations = 0;
#ifdef DEBUG_ENABLED
	_last_visited_timestamp = 0;
	_last_evaluated_timestamp = 0;
#endif
}

UtilityAIBTNodes::~UtilityAIBTNodes() {
}

// Getters and Setters.

void UtilityAIBTNodes::set_evaluation_method(int evaluation_method) {
	_evaluation_method = evaluation_method;
}

int UtilityAIBTNodes::get_evaluation_method() const {
	return _evaluation_method;
}

void UtilityAIBTNodes::set_score(float score) {
	_score = score;
}

float UtilityAIBTNodes::get_score() const {
	return _score;
}

void UtilityAIBTNodes::set_tick_result(Status tick_result) {
	_tick_result = tick_result;
}

UtilityAIBTNodes::Status UtilityAIBTNodes::get_tick_result() const {
	return _tick_result;
}

void UtilityAIBTNodes::set_internal_status(int internal_status) {
#ifdef DEBUG_ENABLED
	_last_visited_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif
	switch (_reset_rule) {
		case UtilityAIBTNodesResetRule::WHEN_COMPLETED: {
			if (internal_status == BT_INTERNAL_STATUS_COMPLETED) {
				reset_bt_node();
				//_is_first_tick = true;
			}
		} break;
		case UtilityAIBTNodesResetRule::WHEN_TICKED_AFTER_BEING_COMPLETED: {
			if (_internal_status == BT_INTERNAL_STATUS_COMPLETED &&
					internal_status == BT_INTERNAL_STATUS_TICKED) {
				reset_bt_node();
				//_is_first_tick = true;
			}
		} break;
		case UtilityAIBTNodesResetRule::WHEN_TICKED: {
			if (internal_status == BT_INTERNAL_STATUS_TICKED && (_internal_status != BT_INTERNAL_STATUS_UNTICKED || _has_reset_rule_changed)) {
				reset_bt_node();
				//_is_first_tick = true;
			}
		} break;
		default: {
		} break;
	}

	_internal_status = internal_status;
	_has_reset_rule_changed = false;
}

int UtilityAIBTNodes::get_internal_status() const {
	return _internal_status;
}

void UtilityAIBTNodes::set_reset_rule(int reset_rule) {
	_has_reset_rule_changed = (reset_rule != _reset_rule);
	_reset_rule = reset_rule;
}

int UtilityAIBTNodes::get_reset_rule() const {
	return _reset_rule;
}

void UtilityAIBTNodes::set_considerations(TypedArray<UtilityAIConsiderationResources> considerations) {
	_considerations = considerations;
}

TypedArray<UtilityAIConsiderationResources> UtilityAIBTNodes::get_considerations() const {
	return _considerations;
}

// Handling methods.

void UtilityAIBTNodes::reset() {
	_internal_status = BT_INTERNAL_STATUS_UNTICKED;
	for (int i = 0; i < _num_child_btnodes; ++i) {
		_child_btnodes[i]->reset();
	}
	/**
	for( int i = 0; i < get_child_count(); ++i ) {
		if( UtilityAIBTNodes* btnode = godot::Object::cast_to<UtilityAIBTNodes>(get_child(i)) ) {
			btnode->reset();
		}
	}
	/**/
}

/**
void UtilityAIBTNodes::reset_for_looping() {
	_internal_status = BT_INTERNAL_STATUS_UNTICKED;
	for( int i = 0; i < get_child_count(); ++i ) {
		if( UtilityAIBTNodes* btnode = godot::Object::cast_to<UtilityAIBTNodes>(get_child(i)) ) {
			btnode->reset_for_looping();
		}
	}
}
/**/

float UtilityAIBTNodes::evaluate() {
//if( !get_is_active() ) return 0.0f;
//if( Engine::get_singleton()->is_editor_hint() ) return 0.0f;
#ifdef DEBUG_ENABLED
	_last_evaluated_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif

	_score = 0.0f;
	bool has_vetoed = false;
	// Evaluate the consideration resources (if any).
	int num_resources = _considerations.size();
	for (int i = 0; i < num_resources; ++i) {
		UtilityAIConsiderationResources *consideration_resource = godot::Object::cast_to<UtilityAIConsiderationResources>(_considerations[i]);
		if (consideration_resource == nullptr) {
			continue;
		}
		if (!consideration_resource->get_is_active()) {
			continue;
		}
		float score = consideration_resource->evaluate(has_vetoed, this);
		if (has_vetoed) {
			_score = 0.0f;
			return 0.0f; // A consideration vetoed.
		}
		_score += score;
	}

	// Evaluate the child considerations.
	if (_num_child_considerations < 1) {
		return _score;
	}
	//int num_children = get_child_count();
	//if( num_children < 1 ) {
	//    return _score;
	//}
	float child_score = 0.0f;
	for (unsigned int i = 0; i < _num_child_considerations; ++i) {
		//Node* node = get_child(i);
		//if( node == nullptr ) continue;
		//UtilityAIConsiderations* considerationNode = godot::Object::cast_to<UtilityAIConsiderations>(node);
		//if( considerationNode == nullptr ) continue;
		UtilityAIConsiderations *considerationNode = _child_considerations[i];
		if (!considerationNode->get_is_active())
			continue;
		child_score = considerationNode->evaluate();
		if (considerationNode->get_has_vetoed()) {
			_score = 0.0f;
			return 0.0f; // Veto zeroes out the score for the entire group.
		}

		switch (_evaluation_method) {
			case UtilityAIBTNodesEvaluationMethod::Min: {
				if (i == 0)
					_score = child_score;
				if (child_score < _score)
					_score = child_score;
			} break;
			case UtilityAIBTNodesEvaluationMethod::Max: {
				if (i == 0)
					_score = child_score;
				if (child_score > _score)
					_score = child_score;
			} break;
			case UtilityAIBTNodesEvaluationMethod::Multiply: {
				if (i == 0)
					_score = child_score;
				else
					_score *= child_score;
				// If after multiplication we are at 0.0, then none of the
				// other considerations will ever change the result, so bail.
				if (_score == 0.0f) {
					if (_invert_score) {
						_score = 1.0f;
						return 1.0f;
					}
					_score = 0.0f;
					return 0.0f;
				}
			} break;
			case UtilityAIBTNodesEvaluationMethod::FirstNonZero: {
				if (child_score > 0.0f) {
					if (_invert_score) {
						_score = 1.0f - child_score;
					} else {
						_score = child_score;
					}
					return _score;
				}
			} break;
			default:
				_score += child_score;
		} //end switch evaluation method

	} //endfor children

	if (_evaluation_method == UtilityAIBTNodesEvaluationMethod::Mean) {
		_score = _score / ((float)_num_child_considerations);
	}

	if (_invert_score) {
		_score = 1.0f - _score;
	}

	return _score;
}

UtilityAIBTNodes::Status UtilityAIBTNodes::tick(Variant blackboard, float delta) {
	return Status::RUNNING;
}

// Godot virtuals.

void UtilityAIBTNodes::_notification(int p_what) {
	if (p_what == NOTIFICATION_CHILD_ORDER_CHANGED) {
		// Update the child pointers if the child nodes are changed
		// somehow. This caching of the pointers is done to avoid
		// the expensive godot::Object::cast_to calls later.
		update_child_vectors();
	}
}

void UtilityAIBTNodes::update_child_vectors() {
	_child_btnodes.clear();
	_child_considerations.clear();
	for (int i = 0; i < get_child_count(); ++i) {
		if (UtilityAIBTNodes *btnode = godot::Object::cast_to<UtilityAIBTNodes>(get_child(i))) {
			_child_btnodes.push_back(btnode);
		} else if (UtilityAIConsiderations *consideration = godot::Object::cast_to<UtilityAIConsiderations>(get_child(i))) {
			_child_considerations.push_back(consideration);
		}
	}
	_num_child_btnodes = (unsigned int)_child_btnodes.size();
	_num_child_considerations = (unsigned int)_child_considerations.size();
}
