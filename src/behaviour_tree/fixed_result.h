#ifndef UtilityAIBTFixedResult_H_INCLUDED
#define UtilityAIBTFixedResult_H_INCLUDED

#include "decorator_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class UtilityAIBTFixedResult : public UtilityAIBTDecoratorNodes {
	GDCLASS(UtilityAIBTFixedResult, UtilityAIBTDecoratorNodes)

private:
	UtilityAI::Status _fixed_result;

protected:
	static void _bind_methods();

public:
	UtilityAIBTFixedResult();
	~UtilityAIBTFixedResult();

	// Getters and setters for attributes.

	void set_fixed_result(UtilityAI::Status fixed_result);
	UtilityAI::Status get_fixed_result() const;

	// Handling functions.

	virtual UtilityAI::Status tick(Variant user_data, float delta) override;
};

} //namespace godot

#endif
