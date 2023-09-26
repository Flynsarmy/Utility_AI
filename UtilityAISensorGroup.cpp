#include "UtilityAISensorGroup.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>


using namespace godot;

// Method binds.

void UtilityAISensorGroup::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_evaluation_method", "evaluation_method"), &UtilityAISensorGroup::set_evaluation_method);
    ClassDB::bind_method(D_METHOD("get_evaluation_method"), &UtilityAISensorGroup::get_evaluation_method);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "evaluation_method", PROPERTY_HINT_ENUM, "Sum:0,Min:1,Max:2,Mean:3,Multiply:4,FirstNonZero:5,OneMinusScore:6"), "set_evaluation_method","get_evaluation_method");

    /**
    ClassDB::bind_method(D_METHOD("change_to_state", "target_state_name"), &UtilityAISensorGroup::_change_to_state);
    
    ClassDB::bind_method(D_METHOD("update_current_state", "delta"), &UtilityAISensorGroup::_update_current_state);
    /**/
}


// Constructor and destructor.

UtilityAISensorGroup::UtilityAISensorGroup() {
    _evaluation_method = UtilityAISensorGroupEvaluationMethod::Mean;
}


UtilityAISensorGroup::~UtilityAISensorGroup() {
}

// Handling functions.

double UtilityAISensorGroup::evaluate_sensor_value() {
    if( !get_is_active() ) return 0.0;
    if( Engine::get_singleton()->is_editor_hint() ) return 0.0;

    double sensor_value = 0.0;

    // Evaluate the children.
    int num_children = get_child_count();
    if( num_children < 1 ) return 0.0;
    double child_sensor_value = 0.0;
    double one_over_num_children = 1.0 / (double)num_children;
    for( int i = 0; i < num_children; ++i ) {
        UtilityAISensors* sensorNode = godot::Object::cast_to<UtilityAISensors>(get_child(i));
        if( sensorNode == nullptr ) continue;
        if( !sensorNode->get_is_active() ) continue;
        child_sensor_value = sensorNode->evaluate_sensor_value();
        
        switch( _evaluation_method ) {
            case UtilityAISensorGroupEvaluationMethod::Min: 
            {
                if( i == 0 ) sensor_value = child_sensor_value;
                if( child_sensor_value < sensor_value ) sensor_value = child_sensor_value;
            }
            break;
            case UtilityAISensorGroupEvaluationMethod::Max: 
            {
                if( i == 0 ) sensor_value = child_sensor_value;
                if( child_sensor_value > sensor_value ) sensor_value = child_sensor_value;
            }
            break;
            case UtilityAISensorGroupEvaluationMethod::Mean: 
            {
                sensor_value += (child_sensor_value * one_over_num_children);
            }
            break;
            case UtilityAISensorGroupEvaluationMethod::Multiply: 
            {
                if( i == 0 ) sensor_value = child_sensor_value;
                else sensor_value *= child_sensor_value;
            }
            break;
            case UtilityAISensorGroupEvaluationMethod::FirstNonZero: 
            {
                if( child_sensor_value > 0.0 ) {
                    sensor_value = child_sensor_value;
                    return sensor_value;
                }
            }
            break;
            default: sensor_value += child_sensor_value;
        }//end switch evaluation method
        
    }//endfor children

    if( _evaluation_method == UtilityAISensorGroupEvaluationMethod::OneMinusScore ) {
        sensor_value = 1.0 - sensor_value;
    }

    set_sensor_value(sensor_value);
    return sensor_value;
}

// Getters and Setters.

void UtilityAISensorGroup::set_evaluation_method( int evaluation_method ) {
    _evaluation_method = evaluation_method;
}

int UtilityAISensorGroup::get_evaluation_method() const {
    return _evaluation_method;
}
/**/

// Godot virtuals.
/**
void UtilityAISensorGroup::_notification(int p_what) {
	switch (p_what) {
        case NOTIFICATION_ENTER_TREE: {
            // Entered the tree. 
        } break;
		case NOTIFICATION_EXIT_TREE: {
			
		} break;
	}
}

void UtilityAISensorGroup::_ready() {
    if( !_is_active ) return;
    if( Engine::get_singleton()->is_editor_hint() ) return;
    
    // Get the first state as the first child node and enter it.
    ERR_FAIL_COND_MSG( get_child_count() < 1, "UtilityAISensorGroup error, no child nodes (states) have been added to the UtilityAISensorGroup '" + get_name() + "'.");
    _current_state = Object::cast_to<UtilityAISensorGroupState>(get_child(0));
    ERR_FAIL_COND_MSG( _current_state == nullptr, "UtilityAISensorGroup error, the first child is not a UtilityAISensorGroupState node.");
    _current_state->_enter_state();
}

void UtilityAISensorGroup::_process(double delta ) {
    //if( _update_method != UtilityAISensorGroup_UPDATE_METHOD_PROCESS ) return;
    if( !_is_active ) return;
    if( Engine::get_singleton()->is_editor_hint() ) return;
    if( _current_state == nullptr ) return;

    _current_state->_state_process(delta);
    //_current_state->_state_execute_actions(delta);
}

void UtilityAISensorGroup::_physics_process(double delta ) {
    //if( _update_method != UtilityAISensorGroup_UPDATE_METHOD_PHYSICS_PROCESS ) return;
    if( !_is_active ) return;
    if( Engine::get_singleton()->is_editor_hint() ) return;
    if( _current_state == nullptr ) return;
    _current_state->_state_physics_process(delta);
    //_current_state->_state_execute_actions(delta);
}

/**/
