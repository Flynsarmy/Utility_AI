# GdUnit generated TestSuite
class_name bt_cooldown_ticks_test
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_fail_until_repeats() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTCooldownTicks = UtilityAIBTCooldownTicks.new()
	var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = UtilityAIBTNodes.SUCCESS
	_child.cooldown_ticks = 2
	_child.cooldown_return_value = UtilityAIBTNodes.FAILURE

	# First tick should work
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.SUCCESS)
	assert(_child.current_cooldown_ticks == 0)
	# Second tick it's on cooldown
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	assert(_child.current_cooldown_ticks == 1)
	# Third tick it should still be on cooldown
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	assert(_child.current_cooldown_ticks == 2)
	# Fourth tick it should be off cooldown again
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.SUCCESS)
	assert(_child.current_cooldown_ticks == 0)

	_root.free()

func test_dont_cooldown_on_running_child() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTCooldownTicks = UtilityAIBTCooldownTicks.new()
	var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = UtilityAIBTNodes.RUNNING
	_child.cooldown_ticks = 2
	_child.cooldown_return_value = UtilityAIBTNodes.FAILURE

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.RUNNING)
	assert(_child.current_cooldown_ticks == 2)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.RUNNING)
	assert(_child.current_cooldown_ticks == 2)

	# Make grandhcild return success and confirm we go on cooldown
	_grandchild.fixed_result = UtilityAIBTNodes.SUCCESS
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.SUCCESS)
	assert(_child.current_cooldown_ticks == 0)
	_root.free()

func test_failure_on_no_children() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTCooldownTicks = UtilityAIBTCooldownTicks.new()
	var blackboard: Dictionary = {}
	_root.add_child(_child)

	_child.cooldown_ticks = 2
	_child.cooldown_return_value = UtilityAIBTNodes.FAILURE

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	assert(_child.current_cooldown_ticks == 0)
	_root.free()
