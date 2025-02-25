#ifndef UtilityAIBTTaskNodes_H_INCLUDED
#define UtilityAIBTTaskNodes_H_INCLUDED

#include "nodes.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

namespace godot {

class UtilityAIBTTaskNodes : public UtilityAIBehaviourTreeNodes {
	GDCLASS(UtilityAIBTTaskNodes, UtilityAIBehaviourTreeNodes)

private:
protected:
	static void _bind_methods();

	virtual Status on_tick(Variant blackboard, double delta) { return Status::FAILURE; };

	GDVIRTUAL2R(Status, on_tick, Variant, double);

public:
	UtilityAIBTTaskNodes();
	~UtilityAIBTTaskNodes();
};

} //namespace godot

#endif
