# GdUnit generated TestSuite
class_name bt_cooldown_msec_test
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_fail_until_cooldown_expires() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTCooldownMsec = UtilityAIBTCooldownMsec.new()
	var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = UtilityAIBTNodes.SUCCESS
	_child.cooldown_msec = 100
	_child.cooldown_return_value = UtilityAIBTNodes.FAILURE

	# First tick should work
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.SUCCESS)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	await get_tree().create_timer(.2).timeout
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.SUCCESS)


	_root.free()

func test_dont_cooldown_on_running_child() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTCooldownMsec = UtilityAIBTCooldownMsec.new()
	var _grandchild: UtilityAIBTFixedResult = UtilityAIBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = UtilityAIBTNodes.RUNNING
	_child.cooldown_msec = 100
	_child.cooldown_return_value = UtilityAIBTNodes.FAILURE

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.RUNNING)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.RUNNING)

	# Make grandhcild return success and confirm we go on cooldown
	_grandchild.fixed_result = UtilityAIBTNodes.SUCCESS
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.SUCCESS)
	_root.free()

func test_failure_on_no_children() -> void:
	var _root: UtilityAIBTRoot = UtilityAIBTRoot.new()
	var _child: UtilityAIBTCooldownMsec = UtilityAIBTCooldownMsec.new()
	var blackboard: Dictionary = {}
	_root.add_child(_child)

	_child.cooldown_msec = 100
	_child.cooldown_return_value = UtilityAIBTNodes.FAILURE

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == UtilityAIBTNodes.FAILURE)
	_root.free()
