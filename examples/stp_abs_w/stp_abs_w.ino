#include <stp.h>

// Use the Arduino Serial Plotter to get nice graphs

PS2 ps2dev(6, 5);
STP stp(ps2dev);

void setup() {
    Serial.begin(57600);
    stp.init(STP_MODE_ABS_W);
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
}
