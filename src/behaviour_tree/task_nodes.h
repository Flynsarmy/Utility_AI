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

	virtual UtilityAI::Status _tick(godot::Variant p_variant, double p_delta) { return UtilityAI::Status::FAILURE; };

	GDVIRTUAL2R(UtilityAI::Status, _tick, godot::Variant, double);

public:
	UtilityAIBTTaskNodes();
	~UtilityAIBTTaskNodes();
};

} //namespace godot

#endif
