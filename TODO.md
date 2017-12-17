## TODO

#### Recording

A single fixture, using 512 channels at 60 samples per second, works out at about 30.7Kb per second, 1.8Mb per minute. A 45m session at that rate is about 82Mb of raw uncompressed data.

- [ ] recording session at the protocol level
- [ ] data compression on DMX streams
- [ ] saving to SD card
- [ ] select recording from web ui to playback

Look at this library for compression: https://github.com/atomicobject/heatshrink

#### Power

APA102C use about 60mA of power per pixel at full brightness white. On strips of 144ppm that works out at about 8.64A pm, about ~10A for a 20% allowance, every 10A is about 15,- eur in power supply.

