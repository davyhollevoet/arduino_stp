# Synaptics TouchPad Arduino library

Basis library that builds upon [ps2](https://playground.arduino.cc/ComponentLib/Ps2mouse/), to get an old PS/2 Synaptics TouchPad working.

How these things work, is explained nicely in [Synaptics PS/2 TouchPad Interfacing Guide](http://blog.amigas.ru/wp-content/uploads/2014/03/touchpad_RevB.pdf)

Todo:
- support other modes
- implement a relative mode based on absolute data? this would allow a fallback for non-Synaptic devices.

Here's an example graph from the "Arduino Serial Plotter", produced with the `stp_abs_w` example sketch.
![stp_abs_w output](examples/stp_abs_w/stp_abs_w.png?raw=true)
