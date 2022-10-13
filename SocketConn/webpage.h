//HTML + CSS + JavaScript codes for webpage
//=========================================
const char webpageCode[] =
R"=====(
<!DOCTYPE html>
<html lang="en">
    <head>
        <title>
            ESP32 Device Setup
        </title>
        <!--####################################################  CSS  ####################################################-->
        <style>
            body{
                font-family: Cambria, Cochin, Georgia, Times, 'Times New Roman', serif
            }
            .rowStyle{
                display: flex;
                flex-direction: row;
                justify-content: left;
                gap: 1em;
                padding: 2px;
                margin-bottom: 5ex;
                white-space: nowrap;
            }
            
            .columnStyle{
                display: flex;
                flex-direction: column;
                justify-content: space-between;
                gap: 5px;
            }

            .input{
                display: flex;
                flex-direction: row;
                gap: 10px;
                width: 50%;
            }

            .checkbox{
                display: flex;
                justify-content: left;
            }
            
            .loader {
                position: absolute;
                margin: 3%;
                z-index: 1;
                border: 8px solid #f3f3f3; /* Light grey */
                border-top: 8px solid #3498db; /* Blue */
                border-radius: 50%;
                width: 5em;
                height: 5em;
                animation: spin 2s linear infinite;
                display: none;
            }

            .PageButton{
                height: 1em;
                border: 2px solid black;
                border-radius: 5px; 
                text-align: center; 
                padding: 0.8em;
            }

            .PageButton:hover{
                background-color: beige;
            }

            .PageButton:active{
                border-color: lightgrey;
            }
            .grid-container{
                display: grid;
                grid-template-columns: 4em 4em 4em;
                grid-template-rows: 4em 4em;
                gap: 10px;
            }

            .keyButton{
                display: table-cell;
                vertical-align: middle;
                border: 1px solid rgb(54, 54, 54);
                border-radius: 5px;
                background-color: grey;
                text-align: center;


                -moz-user-select: none; /* Old versions of Firefox */
                -ms-user-select: none; /* Internet Explorer/Edge */
                user-select: none;
                -webkit-touch-callout: none; /* iOS Safari */
                -webkit-user-select: none;   /* Safari */
                
                
                
            }

            .keyButton:hover{
                background-color: lightgray;
                cursor: pointer;
            }

            .keyButton:active{
                background-color: #3498db;
            }


            #item1 {
                grid-column: 1;
                grid-row: 2;
            }
            #item2 {
                grid-column: 2;
                grid-row: 1;
            }
            #item3 {
                grid-column: 3;
                grid-row: 2;
            }
            #item4 {
                grid-column: 2;
                grid-row: 2;
            }


            #toast {
                visibility: hidden;
                border: 2px solid rgb(78, 78, 78);
                border-radius: 3px;
                padding: 10px;
                min-width: 150px;
                background-color: grey;
                color: black;
                text-align: center;
                position: fixed;
                z-index: 1;
                left: 50%;
                top: 5%;
            }

            #toast.show{
                visibility: visible;
            }

            @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
            }
            

            input[type=number]::-webkit-inner-spin-button {
                width: 20px;
                color: #3498db;
                -webkit-appearance: none;
            }


            /* unvisited link */
            a:link {
            color: rgb(53, 53, 53);
            text-decoration: none;
            font-weight: bold;
            }

            /* visited link */
            a:visited {
            color: rgb(53, 53, 53);
            }

            /* mouse over link */
            a:hover {
            color: black;
            }

            /* selected link */
            a:active {
            color: grey;
            }

        </style>
    </head>

    <body> 
        <!--####################################################  HTML  ####################################################-->
        
        <div id="setupPage" class="columnStyle" style="display: none;"> <!-- ---------------------------- setupPage ---------------------------- -->
            <h1>
                ESP32 Cam Controller - Device Setup
            </h1>
            <div class="rowStyle">
                
                <div class="columnStyle">
                    List of available Wifi connections
                    <select id="listBox" Size="10">
                        <!-- <option>spotty</option>  
                        <option> WiFi 2 </option>  
                        <option> WiFi 3 </option>  
                        <option> WiFi 4 </option>   -->
                    </select>  
                    <div id="loader" class="loader"></div>
                    <button id="refreshButton">Refresh</button>
                </div>

                <div class="columnStyle">
                    Please enter or select Wifi and add the corresponding password.
                    <div class="columnStyle">
                        <lable for="ssid"> SSID: </lable> 
                        <input type="text" id="ssid">
                        <label for="password"> Password: </label>
                        <input type="password" id="password">
                        <div class="checkBox">
                            <input type="checkbox" id="checkbox">
                            <label for="checkbox">Show password</label>
                        </div>
                    </div>

                    <div>
                        <button id="submitButtonWiFi">Submit</button>
                    </div>
                </div>
                <a href="#" onclick="return show('indexPage','setupPage');" class="PageButton">Camera control</a>
            </div>

            <div class="rowStyle">
                <div class="columnStyle">
                    <lable for="staticIP"> Static IP: </lable> 
                    <input type="text" id="staticIP">
                    <label for="mDNSname"> mDNS name: </label>
                    <input type="text" id="mDNSname">
                </div>
                    <button id="submitButtonMeta">Submit</button>
                
            </div>

            <div class="rowStyle">
                <div class="rowStyle">
                    <span>Clear stored Wifi data:</span>
                    <button id="resetEEPROM"> Clear </button>
                </div>
    
                <div class="rowStyle">
                    <span>Restart ESP:</span>
                    <button id="restartESP">Restart</button>
                </div>
            </div>
           

        </div> 




        <div id="indexPage" style="display: visible;"> <!-- ---------------------------- indexPage ---------------------------- -->

            <h1>
                ESP32 Cam Controller - Home
            </h1>
            <div class="rowStyle">

                <div class="columnStyle">
                    Camera Positions
                    <select id="favPosBox" size="10" style="width: 20em;"></select>
                    <button id="rotateFavPosButton">Rotate to Pos</button>
                </div>

                <div class="columnStyle">
                    <span id="chosenESP">Choose ESP from list</span>

                    <div class="grid-container">
                        <div id="item1" class="keyButton" onmousedown="keyPressed(37)" onmouseup="keyReleased()">&larr;</div>    
                        <div id="item2" class="keyButton" onmousedown="keyPressed(38)" onmouseup="keyReleased()">&uarr;</div>    
                        <div id="item3" class="keyButton" onmousedown="keyPressed(39)" onmouseup="keyReleased()">&rarr;</div>    
                        <div id="item4" class="keyButton" onmousedown="keyPressed(40)" onmouseup="keyReleased()">&darr;</div>    
                    </div>
                    

                </div>

                <div class="columnStyle">

                    <div class="rowStyle">
                        <span> PosX:</span> 
                        <input type="text" id="PosX" max="6">
                    </div>
                    
                    <div class="rowStyle">
                        <span> PosY:</span> 
                        <input type="text" id="PosY" step="0.01">
                    </div> 
                    <button id="rotateTo">Rotate to</button>
                    <button id="setZero">Set Zero</button>
                    
                </div>

                <a href="#" onclick="return show('setupPage','indexPage');" class="PageButton">Settings</a>
            </div>

            <div class="rowStyle"> 
                <span id="keyCode"> Key pressed: </span>
            </div>

            
        </div>
        

        <div id="toast">Toast</div>


        <!--//####################################################  JavaScript  ####################################################-->
        <script>

            
            // -------- Debug Log --------
            //DEBUG LOG put TRUE
            debug = true
            // -------- Debug Log --------
            function debuglog(message){
                if (debug) console.log(message)
            }

            //show toast
            function showToast(text, time){
                toast.className = "show"
                toast.textContent = text

                setTimeout(function(){  
                    toast.className = toast.className.replace("show", "");
                }, time)
            }


            //websocket connection
            var hostIP = self.location.host //get IP address of ESP Host
            debuglog(self.location.host)
            var url = "ws://" + hostIP + ":1337/" // retrieve ESP host name 
            debuglog(window.location.origin)
            var output
            var context
            wsConnected = false     //Connection flag

            function wsStart(){
                wsConnect(url)
            }

            function wsConnect(url){
                debuglog("Trying to connect to " + url)
                websocket = new WebSocket(url)

                //assign callbacks
                websocket.onopen = function(evt) { onOpen(evt) }
                websocket.onclose = function(evt) { onClose(evt) }
                websocket.onmessage = function(evt) { onMessage(evt) }
                websocket.onError = function(evt) { onError(evt) }

            }

            function onOpen(evt){
                showLoader(true)
                debuglog("Connected")
                wsConnected = true
                showToast("ESP connection established!", 2000)
                wsSend("0:connected")
            }

            function onClose(evt){
                debuglog("Disconnected " + evt)
                showLoader(false)
                wsConnected = false
                //setTimeout(function() { wsConnect(url) }, 2000) //retry connecting
            }

            function onMessage(evt){
                /*      0: recieve available wifi networks
                        1: store ssid and pass
                        2: clear EEPROM
                        3: reboot ESP
                        4: rotate cam
                        5: get stepper position
                        6: send Wifi mDNS and static ip
                        7: get fav pos coords
                */
                debuglog("Recieved: " + event.data)
                payload = evt.data.split(":")
                cmd = payload[0]
                var type = typeof cmd;
                debuglog("CMD; type: " + type + ", cmd: " + cmd)
                content = payload[1]
                switch(cmd){
                    case "0":             
                        updateWifiBox(content)
                        wsSend("5:StepperPos")
                        break;

                    case "1":
                        showToast("Data succesfully stored", 2000)
                        break;

                    case "2":
                        showToast("Data succesfully erased", 2000)
                        break;

                    case "3":
                        showToast("Rebooting ESP", 2000)
                        break;

                    case "4":
                        var pos = content.split(",")
                        updatePos(parseFloat(pos[0]), parseFloat(pos[1]));
                        break;

                    case "5":
                        var pos = content.split(",");
                        updatePos(parseFloat(pos[0]), parseFloat(pos[1]));
                        break;

                    case "6":
                        showToast("Data succesfully stored", 2000)
                        debuglog("store succesful")
                        break;

                    case "7":
                        var pos = content.split(",");
                        updatePos(parseFloat(pos[0]), parseFloat(pos[1]));
                        requestRotate(-1);
                        break;

                    default:
                        debuglog("ERROR interpreting message")
                        break;
                }
                showLoader(false)
            }

            function onError(evt){
                showToast("Unknown websocket error: " + evt.data, 5000)
                debuglog("ERROR: " + evt.data)
            }

            function wsSend(message){
                debuglog("Sending: " + message)
                websocket.send(message)
            }

            window.addEventListener("load", wsStart, false)


            //Response Functions
            function updateWifiBox(content){
                listbox = document.getElementById('listBox') 
                availableNetworks = content.split(",")
                listboxLength = listbox.length
                addContent = true
                debuglog(availableNetworks + " total of " + availableNetworks.length)

                for (var j = 0; j < availableNetworks.length; j++){
                    network = availableNetworks[j]
                    //debuglog("Checking network " + network)
                    //debuglog("Listbox length: " + listboxLength)

                    for (var i = 0; i < listboxLength; i++){
                        if (listbox.options[i].text == network){ //check if network is already contained in LB
                           addContent = false
                           //debuglog(network + " is already in the listbox ")
                        }
                    }
                    if (addContent){
                        //debuglog("Adding " + network)
                        listbox.options[listboxLength] = new Option(network)
                        listboxLength++;
                    }
                }
            }
            
            function sendWifiCredentials(){
                message = "1:"
                ssid = document.getElementById("ssid").value
                pass = document.getElementById("password").value
                debuglog("SSID: " + ssid + ", Pass: " + pass)
                message += ssid + "," + pass
                debuglog("Message: " + message)
                wsSend(message)
            }

            function sendWifiMeta(){
                message = "6:"
                mDNS = document.getElementById("mDNSname").value
                ip = document.getElementById("staticIP").value
                //Check ip for validity
                const mask = /((^|\.)((25[0-5])|(2[0-4]\d)|(1\d\d)|([1-9]?\d))){4}$/ //regex mask
                var match = mask.test(ip)
                debuglog(typeof(match));
                if (match == true){
                    message += ip + "," + mDNS;
                    wsSend(message)
                } else {
                    showToast("Invalid IP Address!", 3000)
                }

                /*
                debuglog("staticIP: " + ip + ", mDNS: " + mDNS)
                message += ip + "," + mDNS
                wsSend(message)*/
            }

            function showLoader(show){
                loader = document.getElementById('loader')
                if (show){
                    loader.setAttribute("style", "display: block")
                    debuglog("showing loader")
                }else{
                    loader.setAttribute("style", "display: none")
                    debuglog("hiding loader")
                }
            }

            function updatePos(posx, posy){
                debuglog("updating pos")
                document.getElementById("PosX").value = posx;
                document.getElementById("PosY").value = posy;
            }
            
            function requestRotate(keyCode){
                var x = parseFloat(document.getElementById("PosX").value)
                var y = parseFloat(document.getElementById("PosY").value)

                switch (keyCode){
                    case 37: //left
                        debuglog("left")
                        x = x-1;
                        break;
                    case 38: //up
                        debuglog("up")
                        y = y + 1;
                        break;
                    case 39: //right
                        x = x + 1;
                        debuglog("right")
                        break;
                    case 40: //down
                        y = y - 1;
                        debuglog("down")
                        break;
                        
                }
                if(wsConnected){
                    wsSend("4:" + x + "," + y + ",12")
                }
            }

        //#############  onsite scripts  #############

        //TODO - KEYCODE DEPRECATED replace with event.key!
            //Showing index- or setupPage
            function show(shown, hidden) {
                document.getElementById(shown).style.display='block';
                document.getElementById(hidden).style.display='none';
                return false;
            } 
            
            //catching keyboard input event
            document.addEventListener('keydown', function(event) {
                keyPressed(event.keyCode)
            });

            document.addEventListener('keyup', function(event) {
                keyReleased()
            });

            //catching keyButton events
            function keyPressed(keyCode){
                var keyspan = document.getElementById("keyCode")
                if(keyCode == 37) {
                    keyspan.textContent="Key pressed: Left";
                }
                else if(keyCode == 38) {
                    keyspan.textContent="Key pressed: Up";
                }
                else if(keyCode == 39) {
                    keyspan.textContent="Key pressed: Right";
                }
                else if(keyCode == 40) {
                    keyspan.textContent="Key pressed: Down";
                }
                if (keyCode >= 37 && keyCode <= 40) requestRotate(keyCode);
                
            }

            function keyReleased(){
                keyspan = document.getElementById("keyCode");
                keyspan.textContent="Key pressed:";
            }

            //listbox events
            const lb = document.getElementById("listBox");
            const favPosBox = document.getElementById("favPosBox");

            lb.onclick = (event) =>{
                ssid.value = lb.value;
            }

            //show password checkbox
            const passBox = document.getElementById("checkbox")
            const passwordInput = document.getElementById("password")

            passBox.onclick = (event) =>{ //show password checkbox
                if (passwordInput.type === "password"){
                    passwordInput.type = "text";
                }else{
                    passwordInput.type = "password"
                }
            }

        // ------ button scripts ------
            const restartButton = document.getElementById("restartESP")

            restartButton.onclick = (event) =>{
                showToast("Restarting ESP. Please wait!", 4000)
                wsSend("3:Restart")
            }
            
            const refreshButton = document.getElementById("refreshButton")
            refreshButton.onclick = (event) =>{
                if (wsConnected){
                    wsSend("0") //refresh networks request
                    showLoader(true)
                } else{
                    wsStart()
                }
            }   

            const submitButtonMeta = document.getElementById("submitButtonMeta")
            submitButtonMeta.onclick = (event) =>{
                //if (wsConnected){
                    sendWifiMeta()
                //} 
            }

            const submitButtonWifi = document.getElementById("submitButtonWiFi")
            submitButtonWifi.onclick = (event) =>{
                if (wsConnected){
                    sendWifiCredentials()
                } 
                showLoader(true)
            }  

            const resetEEPROMButton = document.getElementById("resetEEPROM")
            resetEEPROMButton.onclick = (event) =>{
                if (wsConnected) {
                    wsSend("2:Reset EEPROM")
                }
            }

            const rotateToButton = document.getElementById("rotateTo")
            rotateToButton.onclick = (event) => {
                requestRotate(-1)
            }

            const favPosButton = document.getElementById("rotateFavPosButton")
            favPosButton.onclick = (event) => {
                //request coordinates for fav Pos
                var posName = favPosBox.value
                if (posName != null)
                wsSend("6:" + posName)
            }


        </script>
    </body>    
</html>
)=====";
