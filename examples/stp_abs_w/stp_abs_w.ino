#include <stp.h>

// Use the Arduino Serial Plotter to get nice graphs

PS2 ps2dev(6, 5);
STP stp(ps2dev);

uint8_t all_ext_buttons;

void setup() {
    Serial.begin(57600);
    stp.init(STP_MODE_ABS_W);

    //if state.ext == all_ext_buttons, then the user is pressing all the buttons
    all_ext_buttons = (1u << stp.n_extended_buttons()) - 1;

    //setup names for the Serial Plotter
    Serial.println("x\ty\tLeft\tRight\tMiddle\tExt");
}

void loop() {
    STP::AbsState state;
    //note that this only works on models with capExtended=1
    stp.read_ext_buttons_mode(state);

    Serial.print(state.x, DEC);
    Serial.print("\t");
    Serial.print(state.y, DEC);
    Serial.print("\t");
    Serial.print(state.left, DEC);
    Serial.print("\t");
    Serial.print(state.right, DEC);
    Serial.print("\t");
    Serial.print(state.middle, DEC);
    Serial.print("\t");
    Serial.print(state.ext, DEC);
    Serial.println();

    //switch to high rate mode and back when the user presses all extended buttons
    //don't do this if there are no buttons
    static bool pressed = false;
    static bool high_rate = false;

    if (all_ext_buttons && state.ext == all_ext_buttons) {
        pressed = true;
    } else if (pressed) {
        pressed = false;
        high_rate = !high_rate;
        stp.disable();
        stp.set_byte_mode(high_rate ? STP_MODE_ABS_W_HI_RATE : STP_MODE_ABS_W);
        stp.enable();
    }
}
