// SonicDisc
/*
latest 11Feb18
By Richard Firth https://github.com/richardFirth
This is a processing3 object that can be copypasted into a sketch to visually display the ouput of a sonicDisc.
*/
class SonicDisc {
  SonicDisc() {
  }
  // draw sonic ring object
  void drawSonicRing(int m0, int m1, int m2, int m3, int m4, int m5, int m6, int m7) {
    pushMatrix();
    translate(width/2, height/2);
    fill(0, 255, 255);
    rect(0, 0, 80, 80, 8); // 10 inches
    showSensorReading(0, m0);
    showSensorReading(1, m1);
    showSensorReading(2, m2);
    showSensorReading(3, m3);
    showSensorReading(4, m4);
    showSensorReading(5, m5);
    showSensorReading(6, m6);
    showSensorReading(7, m7);  

    popMatrix();
  }
  // draw sensor reading. this is local, and meant to make things easier
  void showSensorReading(int sNum, int measurement) {
    if (measurement==0) return;
    float inches = measurement/2.5;
    float pix = inches*8;
    float dist = pix + 36; // make up for sensors not at center

    float n = getClockPosition(sNum);
    fill(255, 255, 0);
    ellipse(dist*cos(n/57.2958), dist*sin(n/57.2958), 35, 35); 
    fill(0);
    text(str(sNum), dist*cos(n/57.2958), dist*sin(n/57.2958));
  }
   // get a clock postion to draw the ring in a circle. this is local, and meant to make things easier
  float getClockPosition(int SensorNumber) {

    switch(SensorNumber) {
    case 0: 
      return 270;
    case 1: 
      return 315;
    case 2: 
      return 360;
    case 3: 
      return 45; 
    case 4: 
      return 90;
    case 5: 
      return 135;
    case 6: 
      return 180;   
    case 7: 
      return 225;
    }
    return 0;
  }
}