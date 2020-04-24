#include <stp.h>

PS2 ps2dev(6, 5);
STP stp(ps2dev);

void print_status(uint32_t b) {

    Serial.print("Remote mode ");
    Serial.println((b >> 22) & 0x7, DEC);

    Serial.print("Enable ");
    Serial.println((b >> 21) & 0x1, DEC);

    Serial.print("Scaling ");
    Serial.println((b >> 20) & 0x1, DEC);

    Serial.print("Left ");
    Serial.println((b >> 18) & 0x1, DEC);

    Serial.print("Middle ");
    Serial.println((b >> 17) & 0x1, DEC);

    Serial.print("Right ");
    Serial.println((b >> 16) & 0x1, DEC);

    Serial.print("Resolution ");
    Serial.println((b >> 8) & 0x3, DEC);

    Serial.print("Sample rate ");
    Serial.println(b & 0xFF, DEC);
}

boolean print_identify(uint32_t b) {
    if (((b >> 8) & 0xff) == STP_PLD_SYNAPTICS_MAGIC) {
        Serial.print("Version ");
        Serial.print(b & 0x0F, DEC);
        Serial.print(".");
        Serial.print(b >> 16, DEC);
        Serial.print(", model ");
        Serial.println((b & 0xF0) >> 4, DEC);
        return true;
    }

    Serial.println("Not a Synaptics touchpad");
    return false;
}

void print_caps(uint32_t b) {

    Serial.print("capExtended ");
    Serial.println(b >> 23, DEC);

    Serial.print("nExtendedQueries ");
    Serial.println((b >> 20) & 0x7, DEC);

    Serial.print("capMiddleButton ");
    Serial.println((b >> 18) & 0x1, DEC);

    Serial.print("capPassThrough ");
    Serial.println((b >> 7) & 0x1, DEC);

    Serial.print("capLowPower ");
    Serial.println((b >> 6) & 0x1, DEC);

    Serial.print("capMultiFingerReport ");
    Serial.println((b >> 5) & 0x1, DEC);

    Serial.print("capSleep ");
    Serial.println((b >> 4) & 0x1, DEC);

    Serial.print("capFourButtons ");
    Serial.println((b >> 3) & 0x1, DEC);

    Serial.print("capBallistics ");
    Serial.println((b >> 2) & 0x1, DEC);

    Serial.print("capMultiFinger ");
    Serial.println((b >> 1) & 0x1, DEC);

    Serial.print("capPalmDetect ");
    Serial.println(b & 0x1, DEC);
}

void print_resolution(uint32_t b) {

    Serial.print("infoXupmm ");
    Serial.println((b >> 16) & 0xFF, DEC);

    Serial.print("infoYupmm ");
    Serial.println(b & 0xFF, DEC);
}

void print_ext_model_id(uint32_t b) {

    Serial.print("Light Control ");
    Serial.println((b >> 22) & 0x1, DEC);

    Serial.print("Peak Detect ");
    Serial.println((b >> 21) & 0x7, DEC);

    Serial.print("Glass Pass ");
    Serial.println((b >> 20) & 0x1, DEC);

    Serial.print("Vertical Wheel ");
    Serial.println((b >> 19) & 0x1, DEC);

    Serial.print("Extended W Mode ");
    Serial.println((b >> 18) & 0x1, DEC);

    Serial.print("Horizontal Scroll ");
    Serial.println((b >> 17) & 0x1, DEC);

    Serial.print("Vertical Scroll ");
    Serial.println((b >> 16) & 0x1, DEC);

    Serial.print("nExtendedButtons ");
    Serial.println((b >> 12) & 0x0F, DEC);

    Serial.print("InfoSensor ");
    Serial.println((b >> 10) & 0x3, DEC);

    Serial.print("Product ID ");
    Serial.println(b & 0xFF, DEC);
}

void setup() {
    Serial.begin(9600);
    stp.reset();

    Serial.println("* Identify TouchPad:");
    uint32_t identify = stp.get_info(STP_INFO_IDENTIFY);
    if (print_identify(identify)) {

        Serial.println("* Read Capabilities:");
        uint32_t caps = stp.get_info(STP_INFO_CAPABILITIES);
        print_caps(caps);

        Serial.println("* Read Resolutions:");
        uint32_t res = stp.get_info(STP_INFO_RESOLUTIONS);
        print_resolution(res);

        Serial.println("* Extended Model ID:");
        uint32_t ext_model_id = stp.get_info(STP_INFO_EXT_MODEL_ID);
        print_ext_model_id(ext_model_id);
    }

    Serial.println("* Regular PS/2 status:");
    print_status(stp.get_info(STP_INFO_PS2_STATUS));
}

void loop() {
}
