#include "BTSequence.h"

/****************************************************************************************************
****************************************************************************************************/
result::result BTSequence::execute(Creature* creature) {
    int child = m_pendingIndex;
    m_pendingIndex = -1;

    if (child == -1) {
        child = 0;
    }

    // Continues through the children until one fails
    result::result result;
    for (int i = child; i < m_children.size(); i++) {
        result = m_children[i]->execute(creature);
        if (result == result::Failure) {
            return result;
        }
        else if (result == result::Pending) {
            m_pendingIndex = i;
            return result;
        }
    }
    return result::Success;
}