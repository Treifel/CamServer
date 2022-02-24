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
                gap: 5%;
                padding: 2px;
            }
            
            .columnStyle{
                display: flex;
                flex-direction: column;
                justify-content: space-between;
                gap: 5px;
            }

            .input{
                display: flex;
                flex-direction: column;
                gap: 10px;
                width: 50%;
                
            }

            .checkbox{
                display: flex;
                justify-content: left;
            }

        </style>
    </head>

    <!--####################################################  HTML  ####################################################-->
    <body>
        <h1>
            ESP32 Cam Controller - Device Setup
        </h1>
        <div class="rowStyle">
            <div class="columnStyle">
            List of available Wifi connections
            <select id="listBox" Size="10">
                <!-- <option> WiFi 1 </option>  
                <option> WiFi 2 </option>  
                <option> WiFi 3 </option>  
                <option> WiFi 4 </option>   -->
            </select>  
                
              <button>Refresh</button>
            </div>
            <div class="columnStyle">
                Please enter or select Wifi and add the corresponding password.
                <div class="input">
                    <lable for="ssid"> SSID: </lable> 
                    <input type="text" id="ssid">
                    <label for="password"> Password: </label>
                    <input type="password" id="password">
                    <div class="checkBox">
                        <input type="checkbox" id="checkbox">
                        <label for="checkbox">Show password</label>
                    </div>
                </div>

                <button>Submit</button>
            </div>
        </div>
        <!--//####################################################  JavaScript  ####################################################-->
        <script>

            //websocket connection
            var url = "ws://192.168.4.1:1337/"
            var output
            var context

            function init(){
                
                wsConnect(url)
            }

            function wsConnect(url){
                console.log("Trying to connect to " + url)
                websocket = new WebSocket(url)

                //assign callbacks
                websocket.onopen = function(evt) { onOpen(evt) }
                websocket.onclose = function(evt) { onClose(evt) }
                websocket.onmessage = function(evt) { onMessage(evt) }
                websocket.onError = function(evt) { onError(evt) }

            }

            function onOpen(evt){
                console.log("Connected")

                wsSend("0:connected")
            }

            function onClose(evt){
                console.log("Disconnected")
                setTimeout(function() { wsConnect(url) }, 2000)
            }

            function onMessage(evt){
                //console.log("Recieved: " + evt.data)
                payload = evt.data.split(":")
                cmd = payload[0]
                content = payload[1]
                switch(cmd){
                    //ESP sends greeting message
                    case "0":                   
                        listbox = document.getElementById('listBox')
                        availableNetworks = content.split(",")
                        console.log(availableNetworks)
                        addContent = true
                        for (var j = 0; j < availableNetworks.length; j++){
                            network = availableNetworks[j]
                            console.log("Checking network " + network)
                            for (var i = 0; i < listbox.options.length; i++){
                                if (listbox.options[i] == network) addContent = false;
                            }
                            if (addContent) listbox.options.add(network)
                        }
                        console.log("ESP connected succesfully")

                        break
                    //ESP sends wifis in range
                    case "1":                   
                        console.log("Wifi detected by esp: " + evt.data)
                    default:break;
                }
            }

            function onError(evt){
                console.log("ERROR: " + evt.data)
            }

            function wsSend(message){
                console.log("Sending: " + message)
                websocket.send(message)
            }

            window.addEventListener("load", init, false)



            //Response Functions
            function updateWifiBox(){}

            //onsite scripts
            const lb = document.getElementById("listBox")
            const ssidInput = document.getElementById("ssid")

            lb.onclick = (event) =>{
                ssid.value = lb.value;
            }

            const passBox = document.getElementById("checkbox")
            const passwordInput = document.getElementById("password")

            passBox.onclick = (event) =>{
                if (passwordInput.type === "password"){
                    passwordInput.type = "text";
                }else{
                    passwordInput.type = "password"
                }
            }
        </script>
    </body>    
</html>
)=====";
