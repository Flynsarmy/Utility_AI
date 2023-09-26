#ifndef UtilityAISensorGroup_H_INCLUDED
#define UtilityAISensorGroup_H_INCLUDED 


#include "UtilityAISensors.h"
#include <godot_cpp/classes/node.hpp>



namespace godot {

class UtilityAISensorGroup : public UtilityAISensors {
    GDCLASS(UtilityAISensorGroup, UtilityAISensors )

private:
    int _evaluation_method;
protected:
    static void _bind_methods();

public:
    UtilityAISensorGroup();
    ~UtilityAISensorGroup();
    
    
    // Getters and setters for attributes.

    void set_evaluation_method( int evaluation_method );
    int get_evaluation_method() const;

    enum UtilityAISensorGroupEvaluationMethod {
        Sum = 0,
        Min=1,
        Max=2,
        Mean=3,
        Multiply=4,
        FirstNonZero=5,
        OneMinusScore=6
    };

    /**
  
    // Godot virtuals.
    void _ready();
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);
    */
    // Handling functions.

    virtual double evaluate_sensor_value() override;
    
};

}


#endif 
