#ifndef UtilityAIBTDecoratorNodes_H_INCLUDED
#define UtilityAIBTDecoratorNodes_H_INCLUDED

#include "nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class UtilityAIBTDecoratorNodes : public UtilityAIBTNodes {
	GDCLASS(UtilityAIBTDecoratorNodes, UtilityAIBTNodes)

private:
protected:
	static void _bind_methods();

public:
	UtilityAIBTDecoratorNodes();
	~UtilityAIBTDecoratorNodes();
};

} //namespace godot

#endif
