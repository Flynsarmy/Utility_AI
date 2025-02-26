# GdUnit generated TestSuite
class_name stnode_test
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

## Confirm on_* methods are called when they should be
func test_on_method_state_calls() -> void:
	var _root: UtilityAISTRoot = UtilityAISTRoot.new()
	var _child = load("res://addons/utility_ai/tests/state_tree/node_example_1.gd").new()
	var blackboard: Dictionary = {}

	_root.add_child(_child)
	_root.tick(blackboard, 0.1)
	_root.tick(blackboard, 0.1)
	verify(_child, 1).on_enter_condition(blackboard, 0.1)
	verify(_child, 1).on_enter_state(blackboard, 0.1)
	verify(_child, 2).on_tick(blackboard, 0.1)
	verify(_child, 0).on_exit_state(blackboard, 0.1)
	_root.free()

func test_on_enter_condition_fail_checks() -> void:
	var _root: UtilityAISTRoot = UtilityAISTRoot.new()
	var _child = load("res://addons/utility_ai/tests/state_tree/node_example_2.gd").new()
	var blackboard: Dictionary = {}

	_root.add_child(_child)
	_root.tick(blackboard, 0.1)
	_root.tick(blackboard, 0.1)
	assert(_root.get_active_states().size() == 0)
	verify(_child, 1).on_enter_condition(blackboard, 0.1)
	verify(_child, 0).on_enter_state(blackboard, 0.1)
	verify(_child, 0).on_tick(blackboard, 0.1)
	verify(_child, 0).on_exit_state(blackboard, 0.1)
	_root.free()
