# GdUnit generated TestSuite
class_name just_a_test
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

## _get_active_states() should be 0 before first tick
func test_no_active_children_on_unticked_root() -> void:
	var _root: UtilityAISTRoot = UtilityAISTRoot.new()
	assert(_root.get_active_states().size() == 0)
	_root.free()

## _get_active_states() should be 1 after a tick
func test_active_child_on_ticked_root() -> void:
	var _root: UtilityAISTRoot = UtilityAISTRoot.new()
	_root.add_child(UtilityAISTNode.new())
	_root.tick({}, 0.1);
	assert(_root.get_active_states().size() == 1)
	_root.free()

## _get_child_sensors() should match number of active children
func _test_child_sensor_count() -> void:
	var _root: UtilityAISTRoot = UtilityAISTRoot.new()
	var _child1: UtilityAIAngleVector2Sensor = UtilityAIAngleVector2Sensor.new()
	var _child2: UtilityAIAngleVector2Sensor = UtilityAIAngleVector2Sensor.new()
	var _child3: UtilityAIAngleVector2Sensor = UtilityAIAngleVector2Sensor.new()

	assert(_root.get_active_states().size() == 0)
	_root.add_child(_child1)
	assert(_root.get_child_sensors().size() == 1)
	_root.add_child(_child2)
	assert(_root.get_child_sensors().size() == 2)
	# child count shouldn't change as the sensor isn't active
	_child3.is_active = false
	_root.add_child(_child3)
	assert(_root.get_child_sensors().size() == 2)
	# changes children active state aren't detected.
	_child2.is_active = false
	assert(_root.get_child_sensors().size() == 2)
	_root.free()
