// D:\GITRoot\RotaryControlsXPlane\RotaryJoystick\Report.h


char ReportDescriptor[23] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                    // USAGE (Joystick)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x02,                    //   USAGE_PAGE (Simulation Controls)
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x28,                    //   USAGE_MAXIMUM (Button 40)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x28,                    //   REPORT_COUNT (40)
    0xc0                           // END_COLLECTION
};

