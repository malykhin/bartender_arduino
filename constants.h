// pins
#define   STEP               3
#define   DIR                6
#define   NOT_LIMIT          A3
#define   SERVO              9
#define   NOT_GLASS_SENSOR   A2

// adresses
#define   SETTINGS_ADDR      0

// settings
#define   BAUD_RATE          57600
#define   BUFFER_SIZE        512
#define   SERVO_DELAY        500

// commands
constexpr char GET_SETTINGS[] = "GET_SETTINGS";
constexpr char SET_SETTINGS[] = "SET_SETTINGS";
constexpr char RESET[] = "RESET";
constexpr char MOVE_TO[] = "MOVE_TO";
constexpr char PUSH_DOZER[] = "PUSH_DOZER";
constexpr char IDLE_DOZER[] = "IDLE_DOZER";
constexpr char PREPARE_DOZER[] = "PREPARE_DOZER";
constexpr char GET_STATUS[] = "GET_STATUS";

// responses

constexpr char RESPONSE_OK[] = "{\"status\": \"OK\"}";
constexpr char RESPONSE_READY[] = "{\"status\": \"READY\"}";
constexpr char RESPONSE_NOT_HOMED[] = "{\"status\": \"NOT_HOMED\"}";
constexpr char RESPONSE_GLASS_NOT_INSERTED[] = "{\"status\": \"GLASS_NOT_INSERTED\"}";
