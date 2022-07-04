# CamServer
Software for Remote camera control


## **Basic (planned) features**</br>
This project is meant to remote controle multiple Camera Rigs in two axis. Every Rig is supposed to have one ESP32 controlling it.
The ESP32 can be connected to a network via a website hosted on said ESP32 as an access point and the networks credentials can be saved on EEPROM.
Once connected the software will communicate with other ESP32 to be selected from the website. Therefore multiple Rigs can be controled.</br>
</br>
[SocketConn](https://github.com/Treifel/CamServer/tree/main/SocketConn) contains all files related to ESP32. Including the .html files. </br>
The files for the server are also included in the [ServerFiles](https://github.com/Treifel/CamServer/tree/main/ServerFiles), which is redundant.</br>
[DEPRECATED](https://github.com/Treifel/CamServer/tree/main/DEPRECATED) contains old files and alternative ideas, which where discarded.</br>
