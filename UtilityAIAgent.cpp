#include "UtilityAIAgent.h"
#include "UtilityAISensors.h"
#include "UtilityAIBehaviour.h"


#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>


using namespace godot;

// Method binds.

void UtilityAIAgent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_current_behaviour_node"), &UtilityAIAgent::set_current_behaviour_node);
    ClassDB::bind_method(D_METHOD("get_current_behaviour_node"), &UtilityAIAgent::get_current_behaviour_node);

    ClassDB::bind_method(D_METHOD("set_current_action_node"), &UtilityAIAgent::set_current_action_node);
    ClassDB::bind_method(D_METHOD("get_current_action_node"), &UtilityAIAgent::get_current_action_node);


    ClassDB::bind_method(D_METHOD("evaluate_options", "delta"), &UtilityAIAgent::evaluate_options);
    ClassDB::bind_method(D_METHOD("update_current_behaviour"), &UtilityAIAgent::update_current_behaviour);

    ClassDB::bind_method(D_METHOD("set_num_behaviours_to_select", "num_behaviours_to_select"), &UtilityAIAgent::set_num_behaviours_to_select);
    ClassDB::bind_method(D_METHOD("get_num_behaviours_to_select"), &UtilityAIAgent::get_num_behaviours_to_select);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "num_behaviours_to_select", PROPERTY_HINT_RANGE, "1,16"), "set_num_behaviours_to_select","get_num_behaviours_to_select");

    ClassDB::bind_method(D_METHOD("set_thinking_delay_in_seconds", "thinking_delay_in_seconds"), &UtilityAIAgent::set_thinking_delay_in_seconds);
    ClassDB::bind_method(D_METHOD("get_thinking_delay_in_seconds"), &UtilityAIAgent::get_thinking_delay_in_seconds);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "thinking_delay_in_seconds", PROPERTY_HINT_NONE), "set_thinking_delay_in_seconds","get_thinking_delay_in_seconds");

    ADD_SUBGROUP("Debugging","");

    ClassDB::bind_method(D_METHOD("set_top_scoring_behaviour_name", "top_scoring_behaviour_name"), &UtilityAIAgent::set_top_scoring_behaviour_name);
    ClassDB::bind_method(D_METHOD("get_top_scoring_behaviour_name"), &UtilityAIAgent::get_top_scoring_behaviour_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "top_scoring_behaviour_name", PROPERTY_HINT_NONE), "set_top_scoring_behaviour_name","get_top_scoring_behaviour_name");

    ClassDB::bind_method(D_METHOD("set_current_behaviour_index", "current_behaviour_index"), &UtilityAIAgent::set_current_behaviour_index);
    ClassDB::bind_method(D_METHOD("get_current_behaviour_index"), &UtilityAIAgent::get_current_behaviour_index);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "current_behaviour_index", PROPERTY_HINT_NONE), "set_current_behaviour_index","get_current_behaviour_index");

    ClassDB::bind_method(D_METHOD("set_current_behaviour_name", "current_behaviour_name"), &UtilityAIAgent::set_current_behaviour_name);
    ClassDB::bind_method(D_METHOD("get_current_behaviour_name"), &UtilityAIAgent::get_current_behaviour_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "current_behaviour_name", PROPERTY_HINT_NONE), "set_current_behaviour_name","get_current_behaviour_name");


    
    // Add all signals.

    ADD_SIGNAL(MethodInfo("behaviour_changed", PropertyInfo(Variant::OBJECT, "behaviour_node")));
}


// Constructor and destructor.

UtilityAIAgent::UtilityAIAgent() {
    _current_behaviour_node = nullptr;
    _num_behaviours_to_select = 1;
    for( int i = 0; i < UTILITYAIAGENT_MAX_TOP_SCORING_BEHAVIOURS; ++i ) {
        _top_scoring_behaviours[i] = -1;
        _top_scoring_behaviours_score[i] = 0.0;
    }
    _top_scoring_behaviour_name = "";
    _current_behaviour_index = 0;

    _current_action_node = nullptr;

    _thinking_delay_in_seconds = 1.0; // One second delay in thinking of options.
    _thinking_delay_in_seconds_current_timer = 0.0; 
}


UtilityAIAgent::~UtilityAIAgent() {
    _current_behaviour_node = nullptr;
    _current_action_node = nullptr;
}

// Handling functions.

void UtilityAIAgent::evaluate_options(double delta) { //double delta) {
    if( !get_is_active() ) return;
    if( Engine::get_singleton()->is_editor_hint() ) return;
    int num_children = get_child_count();
    if( num_children < 1 ) return; // Cannot evaluate without children.

    // Respect the thinking delay.
    _thinking_delay_in_seconds_current_timer -= delta;
    if( _thinking_delay_in_seconds_current_timer > 0.0 ) return;
    _thinking_delay_in_seconds_current_timer = _thinking_delay_in_seconds;

    // If the current behaviour cannot be interrupted, don't evaluate.
    if( _current_behaviour_node != nullptr ) {
        UtilityAIBehaviour* behaviour_node = godot::Object::cast_to<UtilityAIBehaviour>(_current_behaviour_node);
        if( !behaviour_node->get_can_be_interrupted() ) {
            return;
        }
        
    }
    //WARN_PRINT("evaluate_options()");

    // Go through the behaviours and check which one seems
    // best to perform.
    double score = 0.0;
    int place_in_behaviour_list = 0;
    int num_possible_behaviours = 0;
    int chosen_node_index = 0;
    float highest_score = -1.0f;
    UtilityAIBehaviour* new_behaviour = nullptr;

    for( int b = 0; b < _num_behaviours_to_select; ++b ) {
        _top_scoring_behaviours[b] = -1;
        _top_scoring_behaviours_score[b] = -1.0;
    }
    
    // Evaluate the children.
    for( int i = 0; i < num_children; ++i ) {
        Node* node = get_child(i);

        // If it is a sensor, do an evaluation to update any groups.
        UtilityAISensors* sensorNode = godot::Object::cast_to<UtilityAISensors>(node);
        if( sensorNode != nullptr ) {
            if( sensorNode->get_is_active() ) {
                sensorNode->evaluate_sensor_value();
            }
            sensorNode = nullptr;
            continue;
        }

        // If it is a behaviour, handle it.
        UtilityAIBehaviour* behaviourNode = godot::Object::cast_to<UtilityAIBehaviour>(node);
        if( behaviourNode == nullptr ) continue;
        if( !behaviourNode->get_is_active()) continue;

        score = behaviourNode->evaluate();

        // get place on the list.

        // First on the list.
        if( num_possible_behaviours == 0 ) {
            _top_scoring_behaviours[0] = i;
            _top_scoring_behaviours_score[0] = score;
            num_possible_behaviours = 1;
            continue;
        }

        // Somewhere in the list.
        place_in_behaviour_list = -1;
        for( int b = 0; b < _num_behaviours_to_select; ++b ) {
            if( score > _top_scoring_behaviours_score[b]) {
                place_in_behaviour_list = b;
                break;
            }
        }
        if( place_in_behaviour_list < 0 ) continue; // not better than anything else on the list.

        
        // Last on the list.
        if( place_in_behaviour_list == _num_behaviours_to_select-1 ) {
            _top_scoring_behaviours[_num_behaviours_to_select-1] = i;
            _top_scoring_behaviours_score[_num_behaviours_to_select-1] = score;
            //WARN_PRINT("evaluate_options(): behaviour is last on the list.");
            continue;
        }

        // In the beginning or in the middle of a list.

        for( int b = _num_behaviours_to_select-1; b > place_in_behaviour_list; --b ) {
            _top_scoring_behaviours[b] = _top_scoring_behaviours[b-1];
            _top_scoring_behaviours_score[b] = _top_scoring_behaviours_score[b-1];
        }
        _top_scoring_behaviours[place_in_behaviour_list] = i;
        _top_scoring_behaviours_score[place_in_behaviour_list] = score;
        if(num_possible_behaviours < _num_behaviours_to_select ) {
            ++num_possible_behaviours;
        }
        //WARN_PRINT("evaluate_options(): behaviour added to the list.");
    }//endfor children

    if( num_possible_behaviours < 1 ) {
        if( _current_behaviour_node != nullptr ){
            (godot::Object::cast_to<UtilityAIBehaviour>(_current_behaviour_node))->end_behaviour();
            _current_behaviour_node = nullptr;
            _current_behaviour_index = -1;
        }
        WARN_PRINT("Agent could not find behaviours!");
        return; // No behaviour chosen.
    }


    _top_scoring_behaviour_name = godot::Object::cast_to<UtilityAIBehaviour>(get_child(_top_scoring_behaviours[0]))->get_name();

    if( num_possible_behaviours > 1 ) {
        // Pick a random behaviour.
        RandomNumberGenerator rnd;
        rnd.set_seed(time(0));

        int random_behaviour = rnd.randi_range(0, num_possible_behaviours-1);
        _current_behaviour_index = random_behaviour;
        if( _current_behaviour_index >= num_possible_behaviours ){
            _current_behaviour_index = num_possible_behaviours - 1;
        }
        _current_behaviour_index = _top_scoring_behaviours[_current_behaviour_index];
    } else {
        _current_behaviour_index = _top_scoring_behaviours[0];
    }
    
        
    //WARN_PRINT("evaluate_options(): random behaviour " + godot::String(Variant(_current_behaviour_index)));
    new_behaviour = godot::Object::cast_to<UtilityAIBehaviour>(get_child(_current_behaviour_index));
    ERR_FAIL_COND_MSG( new_behaviour == nullptr, "evaluate_options(): Error, new_behaviour is nullptr.");
    if( new_behaviour == _current_behaviour_node ) return; // No change.

    if( _current_behaviour_node != nullptr ) {
        (godot::Object::cast_to<UtilityAIBehaviour>(_current_behaviour_node))->end_behaviour();   
        _current_behaviour_node = nullptr;
    }

    // Start and signal the chosen behaviour node.
    
    _current_behaviour_name = new_behaviour->get_name();
    _current_behaviour_node = godot::Object::cast_to<Node>(new_behaviour);
    new_behaviour->start_behaviour();
    
    emit_signal("behaviour_changed", _current_behaviour_node);
    //WARN_PRINT("Agent " + get_name() + " chose a new behaviour " + new_behaviour->get_name());
}


void UtilityAIAgent::update_current_behaviour() {
    if( _current_behaviour_node == nullptr ) return;
    //WARN_PRINT("Update behaviour for agent " + get_name() + "/" + _current_behaviour_node->get_name() );
    _current_action_node = godot::Object::cast_to<Node>((godot::Object::cast_to<UtilityAIBehaviour>(_current_behaviour_node))->update_behaviour());
    if( _current_action_node == nullptr ) {
        //WARN_PRINT("Update behaviour " + _current_behaviour_node->get_name() + ": No more actions, ending behaviour.");
        (godot::Object::cast_to<UtilityAIBehaviour>(_current_behaviour_node))->end_behaviour();
        _current_behaviour_name = "";
        //WARN_PRINT("Update behaviour: Behaviour ended");
        _current_behaviour_node = nullptr;
        return;
    }
    //WARN_PRINT("Update behaviour DONE for agent " + get_name() + " will continue behaviour " + _current_behaviour_node->get_name() + " with action " + _current_action_node->get_name() );
}

void UtilityAIAgent::set_current_action_is_finished(bool is_finished) {
    if( _current_behaviour_node == nullptr ) return;
    if( _current_action_node == nullptr ) return;
    ((UtilityAIAction*)_current_action_node)->set_is_finished(is_finished);
}

// Getters and Setters.

void UtilityAIAgent::set_current_behaviour_name( godot::String current_behaviour_name ) {
    _current_behaviour_name = current_behaviour_name;
}

godot::String UtilityAIAgent::get_current_behaviour_name() const {
    return _current_behaviour_name;
}

void UtilityAIAgent::set_current_behaviour_node( Node* new_behaviour ) {
    _current_behaviour_node = new_behaviour;
   /** if( _current_behaviour_node != nullptr ) {
        (godot::Object::cast_to<UtilityAIBehaviour>(_current_behaviour_node))->end_behaviour();
    }
    _current_behaviour_node = new_behaviour;
    if( _current_behaviour_node != nullptr ) {
        (godot::Object::cast_to<UtilityAIBehaviour>(_current_behaviour_node))->start_behaviour();
    }
    */
}

Node* UtilityAIAgent::get_current_behaviour_node() const {
    return _current_behaviour_node;
}

void  UtilityAIAgent::set_current_action_node( Node* new_action_node ) {
    _current_action_node = new_action_node;
}

Node* UtilityAIAgent::get_current_action_node() const {
    return _current_action_node;
}

void UtilityAIAgent::set_current_behaviour_index( int current_behaviour_index ) {
    _current_behaviour_index = current_behaviour_index;
}

int  UtilityAIAgent::get_current_behaviour_index() const {
    return _current_behaviour_index;
}

void UtilityAIAgent::set_num_behaviours_to_select( int num_behaviours_to_select ) {
    _num_behaviours_to_select = num_behaviours_to_select;
}

int  UtilityAIAgent::get_num_behaviours_to_select() const {
    return _num_behaviours_to_select;
}

void UtilityAIAgent::set_thinking_delay_in_seconds( double thinking_delay_in_seconds ) {
    _thinking_delay_in_seconds = thinking_delay_in_seconds;
}

double UtilityAIAgent::get_thinking_delay_in_seconds() const {
    return _thinking_delay_in_seconds;
}


void UtilityAIAgent::set_top_scoring_behaviour_name( godot::String top_scoring_behaviour_name ) {
    _top_scoring_behaviour_name = top_scoring_behaviour_name;
}

godot::String UtilityAIAgent::get_top_scoring_behaviour_name() const {
    return _top_scoring_behaviour_name;
}

