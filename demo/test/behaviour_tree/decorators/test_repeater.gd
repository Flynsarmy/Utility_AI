# GdUnit generated TestSuite
class_name bt_repeater_test
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_failure_after_repeats() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTRepeater = UtilityAIBTRepeater.new()
	var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = UtilityAIBTNodes.SUCCESS
	_child.repeat_times = 2

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.RUNNING)
	assert(_child.get_times_repeated() == 1)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	assert(_child.get_times_repeated() == 2)

	_root.free()

func test_immediate_fail() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTRepeater = UtilityAIBTRepeater.new()
	var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = UtilityAIBTNodes.FAILURE
	_child.repeat_times = 2

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	assert(_child.get_times_repeated() == 0)
	_root.free()

func test_reset_after_complete() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTRepeater = UtilityAIBTRepeater.new()
	var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = UtilityAIBTNodes.SUCCESS
	_child.repeat_times = 2

	_root.tick(blackboard, 0.1)
	_root.tick(blackboard, 0.1)
	# After max_repeats is reached, reset counters
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.RUNNING)
	assert(_child.get_times_repeated() == 1)

	_root.free()

func test_dont_increment_repeats_on_running_child() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTRepeater = UtilityAIBTRepeater.new()
	var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = UtilityAIBTNodes.RUNNING
	_child.repeat_times = 2

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.RUNNING)
	assert(_child.get_times_repeated() == 0)
	_root.free()

func test_failure_on_no_children() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTRepeater = UtilityAIBTRepeater.new()
	var blackboard: Dictionary = {}
	_root.add_child(_child)

	_child.repeat_times = 2

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	assert(_child.get_times_repeated() == 0)
	_root.free()
