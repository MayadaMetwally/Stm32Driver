#ifndef APP_RECEIVER_H_
#define APP_RECEIVER_H_
#include "STD_TYPES.h"
#include "MUSART/USART.h"

#define NORMAL_MODE                    0
#define STOP_WATCH_MODE                (u8)(NORMAL_MODE+1)

#define NORMAL_MODE_EDIT               (u8)(STOP_WATCH_MODE+1)
#define EDIT_MODE_OK                   (u8)(NORMAL_MODE_EDIT+1)
#define NO_ACTION                      (u8)(EDIT_MODE_OK+1)

#define NORMAL_MODE_ACTION_UP          (u8)(NO_ACTION+1)
#define NORMAL_MODE_ACTION_DOWN        (u8)(NORMAL_MODE_ACTION_UP+1)
#define NORMAL_MODE_ACTION_RIGHT       (u8)(NORMAL_MODE_ACTION_DOWN+1)
#define NORMAL_MODE_ACTION_LEFT        (u8)(NORMAL_MODE_ACTION_RIGHT+1)

#define NORMAL_MODE_ACTION_INCREASE    (u8)(NORMAL_MODE_ACTION_LEFT+1)
#define NORMAL_MODE_ACTION_DECREASE    (u8)(NORMAL_MODE_ACTION_INCREASE+1)

#define STOP_WATCH_MODE_ACTION_START    (u8)(NORMAL_MODE_ACTION_DECREASE+1) 
#define STOP_WATCH_MODE_ACTION_PAUSE    (u8)(STOP_WATCH_MODE_ACTION_START+1)
#define STOP_WATCH_MODE_ACTION_CONTINUE (u8)(STOP_WATCH_MODE_ACTION_PAUSE+1)
#define STOP_WATCH_MODE_ACTION_STOP     (u8)(STOP_WATCH_MODE_ACTION_CONTINUE+1)

typedef struct {
    u8 Mode;
    u8 NormalModeOperation;
    u8 StopWatchOperation;
    u8 NormalModeEdit;
} RecType_tstr;




void Receive_RunnableFunc(void);
#endif //APP_RECEIVER_H_