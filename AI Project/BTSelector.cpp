#include "BTSelector.h"

/****************************************************************************************************
****************************************************************************************************/
result::result BTSelector::execute(Creature* creature) {
    int child = m_pendingIndex;
    m_pendingIndex = -1;

    if (child == -1) {
        child = 0;
    }
    
    // Continues through the children until one succeeds
    result::result result;
    for (int i = child; i < m_children.size(); i++) {
        result = m_children[i]->execute(creature);
        if (result == result::Success) {
            return result;
        }
        else if (result == result::Pending) {
            m_pendingIndex = i;
            return result;
        }
    }
    return result::Failure;
}