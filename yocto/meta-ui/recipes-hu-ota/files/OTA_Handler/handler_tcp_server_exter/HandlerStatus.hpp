enum class HandlerStatus : int32_t {
    IDLE = 0,
    INIT = 1,
    WAIT = 2,
    PROCESSING = 3,
    VERIFY = 4,
    READY = 5,
    ACTIVATE = 6,
    ERROR = 7
};