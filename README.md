# BQ24292i

BQ24292i is a battery management controller capable of delivering high currents and a wide charging voltage range. This driver exposes the register access.

This driver can easily be ported to a custom platform. The only requirements are an i2c implementation. Add an implementation to the [platform-independent i2c wrapper library](https://github.com/jefflongo/libi2c). Please check the wrapper library README for details about providing the implementation. `i2c_master_init` should be called before using any of the bq24292i driver functions. If there are additional requirements for porting the code to your own platform, please submit an issue so that the compatability can be further improved in the future.

To test if the i2c implementation is successful,`bq24292i_is_present()` should return true with the BQ24292i connected to the i2c bus.
