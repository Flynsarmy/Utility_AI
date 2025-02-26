#ifndef UtilityAISTRoot_H_INCLUDED
#define UtilityAISTRoot_H_INCLUDED

#include "node.h"
#include "nodes.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace godot {

class UtilityAISTRoot : public UtilityAIStateTreeNodes {
	GDCLASS(UtilityAISTRoot, UtilityAIStateTreeNodes)

private:
#ifdef DEBUG_ENABLED
	uint64_t _total_tick_usec;
	uint64_t _total_transition_usec;
#endif
	TypedArray<UtilityAISTNode> _active_states;
	std::vector<UtilityAISTNode *> _active_states_vector;
	TypedArray<UtilityAISensors> _child_sensors;
	unsigned int _num_child_sensors;
	bool _is_first_tick;

	Variant _ai_context;

protected:
	static void _bind_methods();

public:
	UtilityAISTRoot();
	~UtilityAISTRoot();

// Getters and setters for attributes.
#ifdef DEBUG_ENABLED
	uint64_t get_total_tick_usec() const;
	void set_total_tick_usec(uint64_t total_tick_usec);
#endif

	TypedArray<UtilityAISTNode> get_active_states() const;

	//UtilityAIStateTreeNodes* get_active_state() const;
	//_active_states

	// Handling functions.
	//virtual TypedArray<UtilityAIStateTreeNodes> _tick( Variant blackboard, float delta) override;
	virtual void transition_to(NodePath path_to_node, Variant blackboard, float delta) override;
	bool try_transition(UtilityAIStateTreeNodes *transition_target_node, Variant blackboard, float delta);

	void tick(Variant blackboard, float delta);

	TypedArray<UtilityAISensors> get_child_sensors() { return _child_sensors; };

	// Godot virtuals.
	void _notification(int p_what);
};

} //namespace godot

#endif
