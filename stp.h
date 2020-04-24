#ifndef stp_h
#define stp_h

#include <Arduino.h>
#include <ps2.h>

class STP {
public:

    struct AbsState {

        uint8_t w;
        uint8_t z;
        uint16_t x;
        uint16_t y;

        bool right;
        bool left;
        bool middle;
        uint8_t ext;
    };

    explicit STP(PS2 &ps2device);

    void reset();

    void init(uint8_t mode);

    void enable();

    void set_resolution(uint8_t res);

    void set_sample_rate(uint8_t rate);

    void set_byte_mode(uint8_t mode);

    void read_ext_buttons_mode(AbsState &state);

    uint32_t get_info(uint8_t query);

private:
    PS2 &dev;

    void send_encoded_byte(uint8_t b);

    uint8_t n_extbut;
    uint8_t extbut_mask;
};

#define STP_CMD_RESET       0xFF
#define STP_CMD_ENABLE      0xF4
#define STP_CMD_SET_RES     0xE8
#define STP_CMD_SET_SAMP    0xF3
#define STP_CMD_STAT_REQ    0xE9
#define STP_PLD_ACK                 0xFA
#define STP_PLD_SYNAPTICS_MAGIC     0x47
//special sample rate that indicates we want to set byte mode
#define STP_PLD_SET_SAMP_BYTE_MODE  20

#define STP_INFO_IDENTIFY       0x00
#define STP_INFO_CAPABILITIES   0x02
#define STP_INFO_RESOLUTIONS    0x08
#define STP_INFO_EXT_MODEL_ID   0x09
//special case; handled internally
#define STP_INFO_PS2_STATUS     0xFF

//typical modes
//taken from Figure 4-3. PS/2 mode byte values
#define STP_MODE_ABS            0x80
#define STP_MODE_ABS_W          0x81
#define STP_MODE_ABS_HI_RATE    0xC0
#define STP_MODE_ABS_W_HI_RATE  0xC1

#endif /* stp_h */

