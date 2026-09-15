#ifndef ECALIBRATIONSTATE_H
#define ECALIBRATIONSTATE_H

enum class ECalibrationState {
    Idle,
    StartCountdown,
    TouchTopLeft,
    TouchBottomRight,
    ExitCountdown
};

#endif //ECALIBRATIONSTATE_H
