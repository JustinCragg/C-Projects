#pragma once
#include "BTComposite.h"

// BTSequenece (Behaviour Tree Sequence) is a class which acts as an AND
// Each child is executed until one fails
class BTSequence : public BTComposite {
public:
    // The overided function which executes each of its children until one fails
    result::result execute(Creature* creature);

protected:

};