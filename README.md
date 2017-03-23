# OneByteDeviceDriver
A linux driver exercise for  CS5250

Our “device” is part of the computer’s memory. The device we will build is global and persistent one-byte char device. “Global” means that if the device is opened multiple times, the data contained within the device is shared by all the file descriptors that opened it. “Persistent” means that if the device is closed and reopened, data isn’t lost. This device can be fun to work with, because it can be accessed and tested using conventional
commands, such as cp, cat, and shell I/O redirection. “One byte” means that this device can only deal with one byte.

--Copy from assignment description.
