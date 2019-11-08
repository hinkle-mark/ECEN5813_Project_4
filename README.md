# ECEN5813_Project_4
Repository for Project 4 of Principles of Embedded Software

Team Members: Mike Fruge and Mark Hinkle

Repo Contents: This repository contains all code that was developed for PES Project 4, including state machine implementations, I2C drivers, 
an expanded logger, and files that have been present in other projects up to this point. This repository also contains Oscilloscope captures 
of I2C read and write transactions as detailed in the pdf, and uCunit tests to test our functionality.

Compilation and Execution Instructions: Download this repository, import the project into MCUXpresso IDE, and run/debug the project using 
the 'Debug As' and then selecting 'PEMicro Probes'.
    To change the logger level, there is a #define statement that indicated what level the logger is set to. To change this, simply 
#define one of the other logger levels

Observations and Issues: Wooooooooweeeeeee! This project was so full of issues. First of all, it took me a few days to figure out why my first sensor
was not working. This was due to the sensor sending back NACKs after every communication sequence. To fix this, I tried using another sensor which worked well
with the I2C code we had created. But, that sensor has problems too. We found that the sensor is able to take the first temperature measurement accurately,
but after that the value of the temperature bounced all over the place. I investigated this to see if it was a software issue, as did the TA''s and
Professor Montgomery, and he finally told me that it seriously looked like a hardware issue since the other communication that didn't come from
the Temp register was functional. Due to this, it was very hard to try and implement the ALERT functionality of our board because the ALERT pin was
jumping around with the temperature.

Notes: Professor gave permission for our partnership to submit this project late for full credit, due to these issues and the hours wasted tring to debug 
broken hardware.

