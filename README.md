# BQ24292i

BQ24292i is a battery management controller capable of delivering high currents and a wide charging voltage range. This driver exposes the register access.

This driver can easily be ported to a custom platform. Simply implement the `read` and `write` functions of the device handle with your i2c implementation. If there are additional requirements for porting the code to your own platform, please submit an issue so that compatibility can be improved. A CMake library is included for convenience.

To test if the i2c implementation is successful, `bq24292i_is_present()` should return true with the BQ24292i connected to the i2c bus.
