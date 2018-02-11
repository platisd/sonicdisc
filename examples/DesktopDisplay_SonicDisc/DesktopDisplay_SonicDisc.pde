// DesktopDisplay_SonicDisk
// latest 11Feb18
// By Richard Firth https://github.com/richardFirth

// processing 3 sketch to output visual display of a sonic disc https://github.com/platisd/sonicdisc


import processing.serial.*;
Serial myPort;
SerialConnector sc; // Serial connection object
SonicDisc SD;// SonicDisc object

int sonic0 = 50;int sonic1 = 50;int sonic2 = 50;int sonic3 = 50;int sonic4 = 50;int sonic5 = 50;int sonic6 = 50;int sonic7 = 50; // declare int values to which data will be posted

void setup() {
  size(600, 600);
  rectMode(CENTER);
  SD = new SonicDisc();
  sc = new SerialConnector(this, myPort);
  sc.listPorts();
}

void draw() {
  background(25);
  SD.drawSonicRing(sonic0, sonic1, sonic2, sonic3, sonic4, sonic5, sonic6, sonic7);
  sc.drawToScreen();
}
void keyPressed() {
  sc.KeyInputforConnection(key);
}