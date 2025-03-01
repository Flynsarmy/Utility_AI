# GdUnit generated TestSuite
class_name bt_limiter_test
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_fail_after_repeats() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTLimiter = UtilityAIBTLimiter.new()
	var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = UtilityAIBTNodes.SUCCESS
	_child.max_repeat_times = 2

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.SUCCESS)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.SUCCESS)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)

	_root.free()

func test_reset_after_complete() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTLimiter = UtilityAIBTLimiter.new()
	var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = UtilityAIBTNodes.SUCCESS
	_child.max_repeat_times = 1

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.SUCCESS)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.SUCCESS)

	_root.free()

func test_failure_on_no_children() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTLimiter = UtilityAIBTLimiter.new()
	var blackboard: Dictionary = {}
	_root.add_child(_child)

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	_root.free()
