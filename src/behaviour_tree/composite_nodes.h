#ifndef UtilityAIBTCompositeNodes_H_INCLUDED
#define UtilityAIBTCompositeNodes_H_INCLUDED

#include "nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class UtilityAIBTCompositeNodes : public UtilityAIBTNodes {
	GDCLASS(UtilityAIBTCompositeNodes, UtilityAIBTNodes)

private:
protected:
	static void _bind_methods();

public:
	UtilityAIBTCompositeNodes();
	~UtilityAIBTCompositeNodes();
};

} //namespace godot

#endif
