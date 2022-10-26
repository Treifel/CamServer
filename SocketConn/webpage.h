//HTML + CSS + JavaScript codes for webpage
//=========================================
const char webpageCode[] =
R"=====(
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>
            ESP32 Device Setup
        </title>
        <!--####################################################  CSS  ####################################################-->
        <style>
            body{
                font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
            }

            header{
                width: 960px;
	            max-width: 100%;
	            padding: 20px;
	            margin: 0 auto;
            }

            input, label{
                display: inline-block;
                vertical-align: middle;
            }
            label{
                min-width: 10em;
            }
            input{
                min-width: 5em;
            }

            input[type=button], button{
                border: 1px solid rgb(48, 48, 48);
                background-color: rgba(233, 250, 254, 0.284);
                border-radius: 5px;
                padding: .5em;
            }

            input[type=button]:hover, button:hover{
                border: 1px solid black;
                background-color: beige;
                cursor: pointer;
            }

            input[type=button]:active, button:active{
                border: 1px solid rgb(61, 61, 61);
                background-color: antiquewhite;
            }

            .rowStyle{
                display: flex;
                flex-wrap: wrap;
                flex-direction: row;
                justify-content: left;
                align-items: stretch;
                gap: 1em;
                padding: .3em;
                max-width: 100%;
                
                white-space: nowrap;
            }
            
            .columnStyle{
                display: flex;
                flex-direction: column;
                align-items: stretch;
                justify-content: start;
                gap: .2em;
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
                
                display: flex;
                height: 1em;
                border: 2px solid black;
                border-radius: 5px; 
                align-items: center;
                justify-content: center;
                padding: 0.8em;
            }

            .PageButton:hover{
                background-color: beige;
            }

            .PageButton:active{
                border-color: lightgrey;
            }
            .grid-container-arrowkeys{
                display: grid;
                grid-template-columns: 4em 4em 4em;
                grid-template-rows: 4em 4em;
                gap: 1em;
            }

            .keyButton{
                font-family: sans-serif;
                font-size: 2em;
                display: flex;
                justify-content: center;
                align-items: center;
                border: 2px solid rgb(54, 54, 54);
                border-radius: 5px;
                background-color: beige;
                margin-top: 0em;


                -moz-user-select: none; /* Old versions of Firefox */
                -ms-user-select: none; /* Internet Explorer/Edge */
                user-select: none;
                -webkit-touch-callout: none; /* iOS Safari */
                -webkit-user-select: none;   /* Safari */
                
                
                
            }

            .keyButton:hover{
                cursor: pointer;
            }

            .keyButton:active{
                background-color: #3498db;
            }


            #arrowLeft {
                grid-column: 1;
                grid-row: 2;
            }
            #arrowUp {
                grid-column: 2;
                grid-row: 1;
            }
            #arrowRight {
                grid-column: 3;
                grid-row: 2;
            }
            #arrowDown {
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
            <div class="rowStyle" style="display: flex; justify-content: space-between; align-items: center;">
                <h1>
                    ESP32 Cam Controller - Device Setup
                </h1>
                <a href="#" onclick="return show('indexPage','setupPage');" 
                    class="PageButton">Camera control</a>
            </div>  
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
                    Enter wifi credentials
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

                <div class="columnStyle">
                    Wifi meta Settings
                    <lable for="staticIP"> Static IP: </lable> 
                    <div class="rowStyle" style="display: inline-block;">
                        <!--<span style="vertical-align: middle;">192.168.0.</span>-->
                        <input type="text" id="staticIP">
                    </div> 
                    
                    <label for="mDNSname"> mDNS name: </label>
                    <input type="text" id="mDNSname">
                    <button id="submitButtonMeta" style="max-height: 2em; position: relative; height: 50%;" >Submit</button>
                </div>
            </div>




            <div class="columnStyle" style="border: 2px solid darkgrey; border-radius: 4px; max-width: 30em;
            position: absolute; padding: 1em; margin-top: 1em;">
                    Reset storage values:
                    <div>
                        <label style="margin: 10px 0;">Clear stored Wifi data:</label>
                        <input type="button" value="Clear" id="clearWifiButton" style="margin: 10px 0;">
                    </div>
                    <div>
                        <label style="margin: 10px 0;">Clear static IP:</label>
                        <input type="button" value="Clear" id="clearStaticIPButton" style="margin: 10px 0;">
                    </div>
                    <div>
                        <label style="margin: 10px 0;">Clear mDNS name:</label>
                        <input type="button" value="Clear" id="clearmDNSButton" style="margin: 10px 0;">
                    </div>
                    <div>
                        <label style="margin: 10px 0;">ESP32 factory reset:</label>
                        <input type="button" value="Reset"  id="factoryResetButton" style="margin: 10px 0;">
                    </div>
                    <div>
                        <label style="margin: 10px 0;">Restart ESP:</label>
                        <input type="button" value="Restart"  id="restartESPButton" style="margin: 10px 0;">
                    </div>
                    
            </div>
            
        </div> 




        <div id="indexPage" style="display: visible;"> <!-- ---------------------------- indexPage ---------------------------- -->
            <div class="rowStyle" style="display: flex; justify-content: space-between; align-items: center;">
                <h1>
                    ESP32 Cam Controller - Home
                </h1>
                <a href="#" onclick="return show('setupPage','indexPage');" class="PageButton">Settings</a>
            </div>
            <div class="rowStyle">

                <div class="columnStyle">
                    Camera Positions
                    <select id="favPosBox" size="10" style="width: 20em;"></select>
                    <button id="removeFavPosButton">Remove selected position</button>
                    <button id="rotateFavPosButton">Rotate to Pos</button>
                </div>

                <div class="columnStyle">
                    <span id="camControl">Camera control</span>

                    <div class="grid-container-arrowkeys" style="margin-bottom: 1em;">
                        <div id="arrowLeft" class="keyButton" onmousedown="keyPressed(37)" onmouseup="keyReleased()"><span>&larr;</span></div>    
                        <div id="arrowUp" class="keyButton" onmousedown="keyPressed(38)" onmouseup="keyReleased()"><span>&uarr;</span></div>    
                        <div id="arrowRight" class="keyButton" onmousedown="keyPressed(39)" onmouseup="keyReleased()"><span>&rarr;</span></div>    
                        <div id="arrowDown" class="keyButton" onmousedown="keyPressed(40)" onmouseup="keyReleased()"><span>&darr;</span></div>    
                    </div>
                    <div>
                        <span> Step:</span> 
                        <input type="number" id="stepInput" value="1">
                    </div>

                </div>

                <div class="columnStyle">
                    <span id="camPos">Camera angle</span>
                    <div class="rowStyle">
                        <span style="min-width: 5em;">Horizontal:</span> 
                        <input type="text" id="hor">
                    </div>
                    
                    <div class="rowStyle">
                        <span style="min-width: 5em;">Vertical:</span> 
                        <input type="text" id="ver">
                    </div> 

                    <div class="rowStyle">
                        <span style="min-width: 5em;">Speed:</span> 
                        <input type="text" id="speed" value="25">
                    </div> 

                    <button id="rotateTo">Rotate to</button>
                    <button id="addFavPosButton">Add current position</button>
                    <button id="setZeroButton">Set Zero</button>
                    
                </div>
            </div>
            
        </div>
        

        <div id="toast">Toast</div>


        <!--//####################################################  JavaScript  ####################################################-->
        <script>
            //Position x = horizontal axis
            //Position y = vertical axis
            
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
                var available = false
                try{
                    websocket = new WebSocket(url);
                    available = true;
                }catch (error){
                    debuglog("Websocket failed: " + error);
                }

                //assign callbacks
                if (available){
                    websocket.onopen = function(evt) { onOpen(evt) }
                    websocket.onclose = function(evt) { onClose(evt) }
                    websocket.onmessage = function(evt) { onMessage(evt) }
                    websocket.onError = function(evt) { onError(evt) }
                }
            }

            function onOpen(evt){
                showLoader(true)
                debuglog("Connected")
                wsConnected = true
                showToast("ESP connection established!", 2000)
                wsSend("4:0,"); //get stepper Postion
                wsSend("0:connected")
            }

            function onClose(evt){
                debuglog("Disconnected " + evt)
                showLoader(false)
                wsConnected = false
                //setTimeout(function() { wsConnect(url) }, 2000) //retry connecting
            }

            function onMessage(evt){
                /*      
                    Protcol format: cmd:[val],[val],....
                    Protocol cmdset:
                    0: recieve available wifi networks and StepperPos
                    1: DATA Request 
                    2: reboot ESP
                    3: rotate cam
                    4: get/set stepper position
                */
                debuglog("Recieved: " + evt.data)
                payload = evt.data.split(":")
                cmd = payload[0]
                var type = typeof cmd;
                content = payload[1]
                switch(cmd){
                    case "0":             
                        updateWifiBox(content)
                        break;

                    case "1":
                        var data = payload.split(",");
                        var dataID = data[0];
                        var instructions = data[1];
                        if ((dataID == 2) && (instructions == 2)){
                            updateFavBoxMap(data);
                        }
                        showToast("Data succesfully stored", 2000)
                        break;

                    case "2":
                        showToast("Rebooting ESP", 2000)
                        break;

                    case "3":
                        var pos = content.split(",")
                        updatePos(parseFloat(pos[0]), parseFloat(pos[1]));
                        break;

                    case "4":
                        var pos = content.split(",");
                        updatePos(parseFloat(pos[0]), parseFloat(pos[1]));
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
                if (wsConnected){
                    debuglog("Sending: " + message)
                    websocket.send(message)
                }else{
                    debuglog("No Connection")
                }
            }

            //Response Functions
            function updateWifiBox(content){
                var availableNetworks = content.split(",");
                var listboxLength = networkListBox.length
                var addContent = true
                debuglog(availableNetworks + " total of " + availableNetworks.length)

                for (var j = 0; j < availableNetworks.length; j++){
                    network = availableNetworks[j]
                    //debuglog("Checking network " + network)
                    //debuglog("Listbox length: " + listboxLength)

                    for (var i = 0; i < listboxLength; i++){
                        if (networkListBox.options[i].text == network){ //check if network is already contained in LB
                           addContent = false
                           //debuglog(network + " is already in the listbox ")
                        }
                    }
                    if (addContent){
                        //debuglog("Adding " + network)
                        networkListBox.options[listboxLength] = new Option(network)
                        listboxLength++;
                    }
                }
            }
            
            function sendWifiCredentials(){
                message = "1:0,1,"
                ssid = document.getElementById("ssid").value
                pass = document.getElementById("password").value
                debuglog("SSID: " + ssid + ", Pass: " + pass)
                message += ssid + "," + pass
                debuglog("Message: " + message)
                wsSend(message)
            }

            function sendWifiMeta(){
                message = "1:1,1,"
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

            function updatePos(hor, ver){
                debuglog("updating pos")
                document.getElementById("hor").value = hor;
                document.getElementById("ver").value = ver;
            }
            
            function requestRotate(keyCode){
                var hor = Math.round(parseFloat(document.getElementById("hor").value, 2));
                var ver = Math.round(parseFloat(document.getElementById("ver").value, 2));
                var step = Math.round(parseFloat(document.getElementById("stepInput").value, 2));
                var speed = parseInt(document.getElementById("speed").value);

                switch (keyCode){ //key was pressed remove or add amount to Position
                    case 37: //left -> Vertical axis 
                        ver = ver + step;
                        break; 
                    case 38: //up -> Horizontal axis 
                        hor = hor + step;
                        break;
                    case 39: //right -> Vertical axis 
                        ver = ver - step;
                        break;
                    case 40: //down -> Horizontal axis 
                        hor = hor - step;
                        break;
                        
                }

                if(wsConnected){
                    wsSend("3:" + hor + "," + ver + "," + speed)
                }
            }

            
            //#############  onsite scripts  #############

            //Showing index- or setupPage
            function show(shown, hidden) {
                document.getElementById(shown).style.display='block';
                document.getElementById(hidden).style.display='none';
                return false;
            } 
            
            //Eventlistener
            window.addEventListener("load", wsStart, false)

            //Keyboard
            document.addEventListener('keydown', function(event) {
                keyPressed(event.keyCode)
            });

            document.addEventListener('keyup', function(event) {
                keyReleased()
            });

            //catching keyButton events
            function keyPressed(keyCode){
                

                if(keyCode == 37) {
                    document.getElementById("arrowLeft").style = "background-color: #3498db"

                }
                if(keyCode == 38) {
                    document.getElementById("arrowUp").style = "background-color: #3498db"
                }
                if(keyCode == 39) {
                    document.getElementById("arrowRight").style = "background-color: #3498db"
                }
                if(keyCode == 40) {
                    document.getElementById("arrowDown").style = "background-color: #3498db"
                }
                if (keyCode >= 37 && keyCode <= 40) requestRotate(keyCode);
                
            }

            function keyReleased(){
                document.getElementById("arrowLeft").style = "background-color: beige"
                document.getElementById("arrowUp").style = "background-color: beige"
                document.getElementById("arrowRight").style = "background-color: beige"
                document.getElementById("arrowDown").style = "background-color: beige"
            }

            //Network listbox events
            const networkListBox = document.getElementById("listBox");

            networkListBox.onclick = (event) =>{
                ssid.value = networkListBox.value;
            }


            //favPos listbox events
            const favPosListBox = document.getElementById("favPosBox");
            const keyValuePairs = new Array();


            //favBox functions
            const favBoxMap = new Map();
            function updateFavBox(){ 
                //clear Listbox
                favPosListBox.innerHTML = "";
                keyValuePairs.forEach((element, index) =>{
                    favPosListBox.options[index] = new Option(element);
                });
                debuglog(favBoxMap.size() + " items updated");


               
            }

            //update
            function updateFavBoxMap(data){
                //format 2,2,[name=x|y],[name=x|y],...
                //format 0,1,2         ,3         ,...
                var pairs = new Array();

                for (i = 2; i < pairs.length(); i++){ //get every data entry
                    pairs[i-2] = data[i]; 
                }

                var name, position;
                pairs.forEach((element, index) => {
                    name = element.split('|')[0];
                    position = element.split('|')[1];
                    favBoxMap.set(name, position);
                    debuglog(name + "-> " + favBoxMap.get(name));
                });
                updateFavBox();
            }



            //show password checkbox
            const passBox = document.getElementById("checkbox");
            const passwordInput = document.getElementById("password");

            passBox.onclick = (event) =>{ //show password checkbox
                if (passwordInput.type === "password"){
                    passwordInput.type = "text";
                }else{
                    passwordInput.type = "password"
                }
            }

        // ------ button scripts ------

            /*
            EEPROM button format
            1: EEPROM request Format: 1:dataID,instruction,Key,Value
            
                dataID.         0: wifi credentials
                                1: wifiMeta
                                2: favPos 
                                3: factory Reset           
                instruction:    0: delete
                                1: store
                                2: read
            */
            const clearWifiButton = document.getElementById("clearWifiButton")
            clearWifiButton.onclick = (event) =>{
                var message = "1:0,0"
                if (wsConnected) wsSend(message)
                else debuglog("no connection")
            }
            
            const clearStaticIPButton = document.getElementById("clearStaticIPButton")
            clearStaticIPButton.onclick = (event) =>{
                var message = "1:1,0,staticIP"
                if (wsConnected) wsSend(message)
                else debuglog("no connection")
            }

            const clearmDNSButton = document.getElementById("clearmDNSButton")
            clearmDNSButton.onclick = (event) =>{
                var message = "1:1,0,mDNS"
                if (wsConnected) wsSend(message)
                else debuglog("no connection")
            }

            const factoryResetButton = document.getElementById("factoryResetButton")
            factoryResetButton.onclick = (event) =>{
                var message = "1:3"
                if (wsConnected) wsSend(message)
                else debuglog("no connection")
            }

            const restartESPButton = document.getElementById("restartESPButton")
            restartESPButton.onclick = (event) =>{
                showToast("Restarting ESP. Please wait!", 4000)
                wsSend("2:Restart")
            }
            


            const refreshButton = document.getElementById("refreshButton")
            refreshButton.onclick = (event) =>{
                if (wsConnected){
                    wsSend("0:refresh_Network_Pos") //refresh networks request
                    showLoader(true)
                } else{
                    wsStart()
                }
            }   

            const submitButtonMeta = document.getElementById("submitButtonMeta")
            submitButtonMeta.onclick = (event) =>{
                console.log("META")
                if (wsConnected){
                    sendWifiMeta()
                } 
            }

            const submitButtonWifi = document.getElementById("submitButtonWiFi")
            submitButtonWifi.onclick = (event) =>{
                if (wsConnected){
                    sendWifiCredentials()
                } 
            }  

            const rotateToButton = document.getElementById("rotateTo")
            rotateToButton.onclick = (event) => {
                requestRotate(-1)
            }

            const setZeroButton = document.getElementById("setZeroButton")
            setZeroButton.onclick = (event) => {
                if (wsConnected){
                    var message = "4:1,";
                    wsSend(message);
                }
            }

            const favPosButton = document.getElementById("rotateFavPosButton")
            favPosButton.onclick = (event) => {
                //request coordinates for fav Pos
                var name = favPosListBox.value;
                var positions = favBoxMap.get(name);
                var x = parseFloat(positions.split('|')[0]);
                var y = parseFloat(positions.split('|')[1]);
                posx.value = x;
                posy.value = y;
            }

            const addFavPosButton = document.getElementById("addFavPosButton")
            addFavPosButton.onclick = (event) => {

            }
            
            const removeFavPosButton = document.getElementById("removeFavPosButton")
            removeFavPosButton.onclick = (event) => {
            }
            
        </script>
    </body>    
</html>
)=====";
