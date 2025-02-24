#include "task_nodes.h"

using namespace godot;

UtilityAIBTTaskNodes::UtilityAIBTTaskNodes() {
}

UtilityAIBTTaskNodes::~UtilityAIBTTaskNodes() {
}

void UtilityAIBTTaskNodes::_bind_methods() {
	GDVIRTUAL_BIND(_tick, "blackboard", "delta");
}
