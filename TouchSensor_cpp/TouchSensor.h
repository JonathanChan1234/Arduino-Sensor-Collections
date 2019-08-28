#ifndef TouchSensor_h
#define TouchSensor_h

// library interface description
class TouchSensor
{
private:
    int pinNum; // Digital Pin for Touch sensor

    // For polling
    long currentMillis = 0;
    long previousMillis = 0;

    // Double Touch Status
    bool firstTouched = false;
    long firstTouchedTime = 0;
    bool firstLeave = false;
    long firstLeaveTime = 0;
    bool secondTouched = false;
    long secondTouchedTime = 0;

    // Long Touch Status
    bool startLongTouched = false;
    long startLongTouchedTime = 0;
    bool longTouched = false;

    // Double Tap Callback
    typedef void (*TapCallback)();
    TapCallback singleTapCallback, doubleTapCallback, longTapCallback;

    // Check Status
    bool checkDoubleTap = false;
    bool checkSingleTap = false;
    bool checkLongTap = false;

public:
    TouchSensor(int pinNum);
    void subscribeSingleTap(TapCallback callback);
    void subscribeDoubleTap(TapCallback callback);
    void subscribeLongTap(TapCallback callback);
    void unsubscribeSingleTap();
    void unsubscribeDoubleTap();
    void unsubscribeLongTap();
    void checkTapStatus();
};

#endif