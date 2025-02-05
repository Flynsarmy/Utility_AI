#pragma once

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/classes/resource_loader.hpp"

#include "../Godot-SFT/SFT.hpp"
#include "../src/behaviour_tree/fixed_result.h"

void test_bt_fixed_result() {
    // UtilityFunctions::print_rich("---------\n[color=orange]Summator Scene Tests[/color]\n---------");

    Ref<UtilityAIBTFixedResult> fr;
    print_verbose(fr->get_fixed_result() == BT_SUCCESS);
    // = memnew(UtilityAIBTFixedResult);

    // NAMED_TESTS(
    //     "BTFixedResult Tests",
    //     "When empty", VAR_CHECK(fr->get_fixed_result(), true),
    // );

    godot::Dictionary map;
    map["Hello"] = 0;
    map["Hey"] = 999;
    map["this_test_will_fail"] = -1;

    TESTS(
        "dictionary_test",
        map["Hello"] == godot::Variant(0),
        map["Hey"] == godot::Variant(999),
        map["this_test_will_fail"] == godot::Variant(999),
        map.has("Howdy"),
        map.size() == 3
    );
    // test_custom_object();
    // test_custom_scene();
}

// Somewhere else...
// #ifdef TESTS_ENABLED
// do_tests()
// #endif