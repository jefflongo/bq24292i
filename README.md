# BQ24292i

BQ24292i is a battery management controller capable of delivering high currents and a wide charging voltage range. This driver exposes the register access.

This driver can easily be ported to a custom platform. The only requirements are an i2c implementation (add an implementation to the platform-independent wrapper library [here](https://github.com/jefflongo/libi2c))
