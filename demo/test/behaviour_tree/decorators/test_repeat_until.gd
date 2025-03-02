# GdUnit generated TestSuite
class_name bt_repeat_until_test
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_failure_after_repeats() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTRepeatUntil = UtilityAIBTRepeatUntil.new()
	var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = UtilityAIBTNodes.SUCCESS
	_child.expected_tick_result = UtilityAIBTNodes.SUCCESS
	_child.max_repeat_times = 2

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.SUCCESS)
	assert(_child.get_current_repeat_times() == 1)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	print(_child.get_current_repeat_times())
	assert(_child.get_current_repeat_times() == 1)
	# This node never resets. always fails
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	assert(_child.get_current_repeat_times() == 1)

	_root.free()

func test_immediate_fail() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTRepeatUntil = UtilityAIBTRepeatUntil.new()
	var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = UtilityAIBTNodes.FAILURE
	_child.expected_tick_result = UtilityAIBTNodes.SUCCESS
	_child.max_repeat_times = 2

	#@TODO look into this. Seems wrong
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.RUNNING)
	print(_child.get_current_repeat_times())
	assert(_child.get_current_repeat_times() == 1)
	_root.free()

#func test_reset_after_complete() -> void:
	#var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	#var _child: UtilityAIBTRepeatUntil = UtilityAIBTRepeatUntil.new()
	#var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	#var blackboard: Dictionary = {}
	#_child.add_child(_grandchild)
	#_root.add_child(_child)
#
	#_grandchild.fixed_result = UtilityAIBTNodes.SUCCESS
	#_child.expected_tick_result = UtilityAIBTNodes.SUCCESS
	#_child.max_repeat_times = 2
#
	#_root.tick(blackboard, 0.1)
	#_root.tick(blackboard, 0.1)
	## After max_repeats is reached, reset counters
	#_root.tick(blackboard, 0.1)
	#assert(_root.tick_result == UtilityAIBTNodes.RUNNING)
	#assert(_child.get_times_repeated() == 1)
#
	#_root.free()


func test_failure_on_no_children() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTRepeatUntil = UtilityAIBTRepeatUntil.new()
	var blackboard: Dictionary = {}
	_root.add_child(_child)

	_child.max_repeat_times = 2

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	assert(_child.get_current_repeat_times() == 0)
	_root.free()
