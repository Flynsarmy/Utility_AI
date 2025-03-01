# GdUnit generated TestSuite
class_name bt_fixed_result_test
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

## Confirm on_* methods are called when they should be
func test_success_failure_running() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}

	_root.add_child(_child)

	_child.fixed_result = UtilityAIBTNodes.SUCCESS
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.SUCCESS)

	_child.fixed_result = UtilityAIBTNodes.RUNNING
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.RUNNING)

	_child.fixed_result = UtilityAIBTNodes.FAILURE
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)

	_root.free()
