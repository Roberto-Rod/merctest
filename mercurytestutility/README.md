# README #

This project is built using [Qt 5.4.0 for Windows 32-bit (MinGW 4.9.1)](http://www.qt.io/download-open-source/#). 

It has been developed and tested using Window 7 Professional, 64-bit.

It is dependent on the [NI-VISA](http://www.ni.com/download/ni-visa-5.4/4230/en/) libraries being installed to the default location.

### Creating a New Version and an Updated Installer ###
Please see [installer/README.md](installer/README.md) for instructions on creating a new version of Mercury Test Utility and an updated installer.

### VISA Drivers/Passports ###
TCP/IP Equipment does not need additional drivers.

USB Equipment may need device specific VISA Passports to be installed e.g. [NRP-NI-VISA Passport](http://www.rohde-schwarz.com/en/software/nrpz21/)

If a GPIB device is used then the [NI 488.2 drivers](http://sine.ni.com/psp/app/doc/p/id/psp-356/lang/en) must also be installed.

### VISA Configuration; Resource Names/Aliases ###
Equipment must be discovered in the NI-MAX application before it can be seen by the Mercury Test Utility. 

The Resource Name which appears in the NI-MAX utility should be used with the Mercury Test Utility e.g. TCPIP0::192.168.0.17::inst0::INSTR

The NI-MAX utility can be configured with Aliases for any piece of equipment and these Aliases may be used in place of the Resource Names in the Mercury Test Utility. No extra configuration is necessary, Resource Names/Aliases may be used interchangeably. It is recommended that Resource Names are used however as Aliases cannot be reused in the NI-MAX application which can lead to confusion of multiple pieces of equipment of the same type are used.