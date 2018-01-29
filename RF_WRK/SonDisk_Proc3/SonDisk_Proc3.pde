import processing.serial.*;
Serial myPort;
SerialConnector sc;

int sonic0 = 50;
int sonic1 = 50;
int sonic2 = 50;
int sonic3 = 50;
int sonic4 = 50;
int sonic5 = 50;
int sonic6 = 50;
int sonic7 = 50;

/*
TextDisplayBox tb1;
TextDisplayBox tb2;
TextDisplayBox tb3;
TextDisplayBox tb4;
TextDisplayBox tb5;
TextDisplayBox tb6;
TextDisplayBox tb7;
TextDisplayBox tb8;
*/
SonicRing SR;

void setup() {

   size(600, 600);
//Last Modified 26Dec2017
   rectMode(CENTER);
   SR = new SonicRing();

   sc = new SerialConnector(this,myPort);
   sc.listPorts();
}

void draw() {

   background(25);
   /*
   tb1.drawBox(sonic0);
   tb2.drawBox(sonic1);
   tb3.drawBox(sonic2);
   tb4.drawBox(sonic3);
   tb5.drawBox(sonic4);
   tb6.drawBox(sonic5);
   tb7.drawBox(sonic6);
   tb8.drawBox(sonic7);
   */
   SR.drawSonicRing(sonic0,sonic1,sonic2,sonic3,sonic4,sonic5,sonic6,sonic7);
   sc.drawToScreen();
}
void keyPressed() {
sc.KeyInputforConnection(key);
}