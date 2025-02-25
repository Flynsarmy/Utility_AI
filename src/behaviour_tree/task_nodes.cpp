#include "task_nodes.h"

using namespace godot;

UtilityAIBTTaskNodes::UtilityAIBTTaskNodes() {
}

UtilityAIBTTaskNodes::~UtilityAIBTTaskNodes() {
}

void UtilityAIBTTaskNodes::_bind_methods() {
	GDVIRTUAL_BIND(on_tick, "blackboard", "delta");
}
