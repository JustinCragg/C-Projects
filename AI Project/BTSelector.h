#pragma once
#include "BTComposite.h"

// BTSelector (Behaviour Tree Selector) is a class which acts as an OR
// Each child is executed until all are checked or one is succesful
class BTSelector : public BTComposite {
public:
    // The overided function which executes each of its children until all fail or one succedes
    result::result execute(Creature* creature);

protected:

};