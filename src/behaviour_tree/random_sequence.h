#ifndef UtilityAIBTRandomSequence_H_INCLUDED
#define UtilityAIBTRandomSequence_H_INCLUDED

#include "composite_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class UtilityAIBTRandomSequence : public UtilityAIBTCompositeNodes {
	GDCLASS(UtilityAIBTRandomSequence, UtilityAIBTCompositeNodes)

private:
	//bool _is_reactive;
	int _current_child_index;
	TypedArray<int> _child_node_order;

protected:
	static void _bind_methods();

public:
	UtilityAIBTRandomSequence();
	~UtilityAIBTRandomSequence();

	// Getters and setters for attributes.
	//void set_is_reactive( bool is_reactive );
	//bool get_is_reactive() const;

	// Handling functions.

	virtual void reset_bt_node() override;

	virtual UtilityAI::Status tick(Variant user_data, float delta) override;
};

} //namespace godot

#endif
