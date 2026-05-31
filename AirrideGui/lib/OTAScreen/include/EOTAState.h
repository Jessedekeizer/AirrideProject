#ifndef EOTA_STATE_H
#define EOTA_STATE_H

enum class EOTAState {
    SCANNING,
    SELECT,
    UPDATING,
    ERROR,
    DONE
};

#endif // EOTA_STATE_H