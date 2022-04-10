# QtSipper
A tool for data management during the sipping test of nuclear fuel rods

The purpose of QtSipper is to minimize errors coming from human factor. QtSipper requires S561 Genie2000 programming batch and relevant detectors. QtSipper clients on each PC communicate with each other via FTP or LAN, depenging on configuration. 

QtSipper: 1) operates gamma detectors, 2) automatically processes data obtaianed from samples, 3) assesses, if a sample is "suspisous" compared to other (based on standard deviation from linear regression made with the method of least squares).

WARNING: the code is specifically adjusted to requirements of our radiological laboratory. Feel free to use this script as an insipiration, but probably great modification would be nesessary for every case.
