#ifndef UtilityAIBTDecoratorNodes_H_INCLUDED
#define UtilityAIBTDecoratorNodes_H_INCLUDED

#include "task_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class UtilityAIBTDecoratorNodes : public UtilityAIBTTaskNodes {
	GDCLASS(UtilityAIBTDecoratorNodes, UtilityAIBTTaskNodes)

private:
protected:
	static void _bind_methods();

public:
	UtilityAIBTDecoratorNodes();
	~UtilityAIBTDecoratorNodes();
};

} //namespace godot

#endif
