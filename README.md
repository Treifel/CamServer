# CamServer
Software for Remote camera control


## **Basic (planned) features**</br>
This project is meant to remote controle multiple Camera Rigs in two axis. Every Rig is supposed to have one ESP32 controlling it.
The ESP32 can be connected to a network via a website hosted on said ESP32. To save network credentials the Software starts the ESP32 as an access point to save the network data on the EEPROM. If the ESP32 is connected to a network, the camera rig can be controlled via websocket connection to the desired ESP32/Rig.
Once connected the software will communicate with other ESP32 to be selected from the website. Therefore multiple Rigs can be controled.</br>
</br>
[SocketConn](https://github.com/Treifel/CamServer/tree/main/SocketConn) contains all files related to ESP32. Including the .html files. </br>
The files for the server are also included in the [ServerFiles](https://github.com/Treifel/CamServer/tree/main/ServerFiles), which is redundant.</br>
[DEPRECATED](https://github.com/Treifel/CamServer/tree/main/DEPRECATED) contains old files and alternative ideas, which where discarded.</br>
