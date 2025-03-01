#ifndef UtilityAIBTRepeater_H_INCLUDED
#define UtilityAIBTRepeater_H_INCLUDED

#include "decorator_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class UtilityAIBTRepeater : public UtilityAIBTDecoratorNodes {
	GDCLASS(UtilityAIBTRepeater, UtilityAIBTDecoratorNodes)

private:
	int _repeat_times;
	int _times_repeated;
	int _tick_result;

protected:
	static void _bind_methods();

public:
	UtilityAIBTRepeater();
	~UtilityAIBTRepeater();

	// Getters and setters for attributes.

	int get_repeat_times() const;
	void set_repeat_times(int repeat_times);

	int get_times_repeated() const;
	void set_times_repeated(int repeat_times);
	//int get_tick_result() const;
	//void set_tick_result( int tick_result );

	// Handling functions.
	virtual void reset_bt_node() override;
	virtual Status tick(Variant blackboard, float delta) override;
};

} //namespace godot

#endif
