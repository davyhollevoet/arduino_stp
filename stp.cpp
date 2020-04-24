#include "stp.h"

STP::STP(PS2 &ps2device) : dev(ps2device), n_extbut(0), extbut_mask(0) {
}

void STP::reset() {
    dev.write(STP_CMD_RESET);
    dev.read();  // ack byte
    dev.read();  // 0xAA
    dev.read();  // 0x00
}

void STP::set_resolution(uint8_t res) {
    dev.write(STP_CMD_SET_RES);
    dev.read();  // ack
    dev.write(res);
    dev.read();  // ack
}


void STP::set_sample_rate(uint8_t rate) {
    dev.write(STP_CMD_SET_SAMP);
    dev.read();  // ack
    dev.write(rate);
    dev.read();  // ack
}

void STP::send_encoded_byte(uint8_t b) {
    set_resolution(b >> 6);
    set_resolution((b >> 4) & 0x3);
    set_resolution((b >> 2) & 0x3);
    set_resolution(b & 0x3);
}

void STP::set_byte_mode(uint8_t mode) {
    send_encoded_byte(mode);
    set_sample_rate(STP_PLD_SET_SAMP_BYTE_MODE);
}

void STP::enable() {
    dev.write(STP_CMD_ENABLE);
    dev.read();
}

uint32_t STP::get_info(uint8_t query) {

    if (query < 0x10) {
        send_encoded_byte(query);
    }

    dev.write(STP_CMD_STAT_REQ); //status request
    dev.read();  // ack

    uint32_t result = dev.read();
    result = (result << 8) | dev.read();
    return (result << 8) | dev.read();
}

void STP::init(uint8_t mode) {
    reset();

    uint32_t identify = get_info(STP_INFO_IDENTIFY);
    bool is_synaptics = ((identify >> 8) & 0xFF) == STP_PLD_SYNAPTICS_MAGIC;

    if (is_synaptics) {

        uint32_t cap_info = get_info(STP_INFO_CAPABILITIES);
        bool cap_ext = cap_info >> 23;
        uint8_t n_ext = (cap_info >> 22) & 0x07;

        if (n_ext >= 1) {
            uint32_t ext_model = get_info(STP_INFO_EXT_MODEL_ID);
            n_extbut = (ext_model >> 12) & 0x0F;

            uint8_t mask_width = (n_extbut + 1) >> 1;
            extbut_mask = (1 << mask_width) - 1;
        }

        uint8_t mode = 0x81; //Absolute mode with W
        if (mode) {
            set_byte_mode(mode);
        }

        enable();
    }
    // do not enable when not a synaptics device
    // TODO: enable anyway for some relative mode?
}

void STP::read_ext_buttons_mode(STP::AbsState &state) {

    // From Synaptics PS/2 TouchPad Interfacing Guide
    // PN: 511-000275-01 Rev. B
    // Page 29, Figure 3-10
    //    +------+------+-------------+------+------+------+------+
    // b1 |   1  |   0  |    W 3..2   |   0  |  W 1 |   R  |   L  |
    //    +------+------+--------------------+------+------+------+
    // b2 |          Y 11..8          |          X 11..8          4|
    //    +---------------------------+---------------------------+
    // b3 |                         Z 7..0                        |
    //    +------+------+-------------+------+------+------+------+
    // b4 |   1  |   1  | Y 12 | X 12 |   0  |  W 0 | R^Ex | L^M  |
    //    +------+------+-----------------------------------------+
    // b5 |          X 7..4           |  b7  |  b5  |  b3  |  b1  |
    //    +-------------------------------------------------------+
    // b6 |          Y 7..4           |  b8  |  b6  |  b4  |  b2  |
    //    +---------------------------+------+------+------+------+

    uint8_t b1 = dev.read();
    uint8_t b2 = dev.read();
    uint8_t b3 = dev.read();
    uint8_t b4 = dev.read();
    uint8_t b5 = dev.read();
    uint8_t b6 = dev.read();

    state.right = (b1 & 0x2) == 0x2;
    state.left = (b1 & 0x1) == 0x1;

    bool r_xor_ext = (b4 & 0x2) == 0x2;
    // if R and R^Ex disagree, then Ex is 1
    bool ext = state.right != r_xor_ext;

    bool m_xor_left = (b4 & 0x1) == 0x1;
    // if L and L^M disagree, then M is 1
    state.middle = state.left != m_xor_left;

    //by default, do not mask x, y
    uint8_t mask = 0;

    if (ext) {
        state.ext = ((b5 & extbut_mask) << 2) | (b6 & extbut_mask);
        //apply a mask if ext buttons are being pressed
        mask = extbut_mask;
    } else {
        state.ext = 0;
    }

    state.x = (((uint16_t) (b4 & 0x10)) << 8) | (((uint16_t) (b2 & 0x0f)) << 8) | (b5 & ~mask);
    state.y = (((uint16_t) (b4 & 0x20)) << 7) | (((uint16_t) (b2 & 0xf0)) << 4) | (b6 & ~mask);
    state.w = ((b1 & 0x30) >> 2) | ((b1 & 0x4) >> 1) | ((b4 & 0x4) >> 2);
    state.z = b3;
}
