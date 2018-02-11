// SerialConnector_3


/*
Standard stuff to add
## variables 
   SerialConnector sc;

## setup
   sc = new SerialConnector(this,port);
   sc.listPorts();

## draw
sc.drawToScreen();


## void keyPressed() {
  sc.KeyInputforConnection(key);
}


*/


class SerialConnector{
  
  boolean isConnected;
  String connectionPort;
  int baudRate = 9600;
  
  PApplet sketchRef;
  Serial portRef;
  
  SerialConnector(PApplet skRef, Serial pRef){
    sketchRef = skRef;
    portRef = pRef;
  }
  
    void listPorts(){
      if (Serial.list().length > 0){
        for(int i = 0; i < Serial.list().length;i++){
          print(Serial.list()[i]);
          println("  <-- Press " + str(i) + " to connect.");
        }     
        println("Press x to disconnect");
        println("Press r to relist ports");
        println("Press b to change baud rate - current: " + str(baudRate));
      } else {
        println("No COM Ports open"); 
      }
    }
    
    void drawToScreen(){
        fill(255);
        textAlign(LEFT);
        textSize(12);
        if (sc.isConnected) {
          text("Connected to " + sc.connectionPort,5,height-14);
        } else {
          text("Not connected. see console for connection instructions",5,height-14);
        }
        ConnectionStillValid();
        
          if (portRef==null){  fill(255,0,0);} else {  fill(0,255,255);}
              stroke(0);
              strokeWeight(0);
          rect(width-40,height-20,20,20);
        
    }

    void writeToSerial(char x){
      if(portRef!=null){
        portRef.write(x);
      }
    }
    
    void writeToSerial(String x){
      if(portRef!=null){
        portRef.write(x);
      }
    }

    void writeToSerial(int x){
      if(portRef!=null){
        portRef.write(x);
      }
    }
    
    void writeToSerial(byte x){    
      if(portRef!=null){
        portRef.write(x);
      }
    }

   boolean ConnectionStillValid(){
     if(isConnected){
     for (String aPortName : Serial.list()) {
       if (aPortName.equals(connectionPort)) return true;
     }
     isConnected = false;
     connectionPort = "";
     }
     return false;
   }

   void makeConnection(int portIndex){
       println(Serial.list().length + "   " + str(portIndex));
       
     if (Serial.list().length <= portIndex) {
         println("No such port");
         return;
     }
     if(isConnected) {
         println("Already Connected");
         return;
     }
       
      try{
        portRef = new Serial(sketchRef,Serial.list()[portIndex], baudRate);  //Set up serial 
        portRef.bufferUntil('\n'); // try this?! 
        isConnected = true;
        connectionPort = Serial.list()[portIndex];        
      }catch(Exception e){
      System.err.println("counldn't connect");
      }    

   }
  
  
  
  
   void disconnect(){
     if(isConnected) {
        portRef.stop(); 
        portRef = null;
        isConnected = false;
        connectionPort  = null;
     } else {
       println("No connection to disconnect");
       delay(300);
     }
   }
   
   void changeBaud(){
     if (baudRate == 9600) {
       baudRate = 38400;

     } else if (baudRate == 38400) {
       baudRate = 9600;
     }
     listPorts();
     delay(650);
   }
  
  
    void KeyInputforConnection(char aKey){
        if (aKey == '0') makeConnection(0);
        if (aKey == '1') makeConnection(1);
        if (aKey == '2') makeConnection(2);
        if (aKey == '3') makeConnection(3);
        if (aKey == '4') makeConnection(4);
        if (aKey == '5') makeConnection(5);
        if (aKey == '6') makeConnection(6);
        if (aKey == '7') makeConnection(7);
        if (aKey == '8') makeConnection(8);
        if (aKey == '9') makeConnection(9);
        if (aKey == 'x') disconnect();
        if (aKey == 'r') listPorts();
        if (aKey == 'b') changeBaud();
    }

  
}