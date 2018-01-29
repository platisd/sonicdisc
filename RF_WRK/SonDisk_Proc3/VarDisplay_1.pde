class TextDisplayBox
{
  
  LocationData myloc;
  String varName;
  String varValue = "13.6";
 
  
  TextDisplayBox(float Nxpos, float Nypos, float Nw, float Nh, String vName) {
    myloc = new LocationData(new xy(Nxpos,Nypos),new xy(Nw,Nh));
    varName = vName;
  }

          // paintBasic
          // paintDisabled
          // paintMouseOver
          // paintInteraction


  void drawBox(int theVal){
    varValue = str(theVal);
    paintBox();
  }
  
  void drawBox(float theVal){
    varValue = str(theVal);
    paintBox();
  }
 
 void drawBox(boolean theVal){
    varValue = str(theVal);
    paintBox();
  }
  
  
  void paintBox()
  {
    fill(255,255,0);
    stroke(0,255,255);
    strokeWeight(2);
   rect(myloc.Location.x,myloc.Location.y,myloc.Size.x,myloc.Size.y);
   fill(0);
   textAlign(LEFT,CENTER);
   textSize(16);
   text(varName +": " + varValue,myloc.Location.x-myloc.Size.x/2+5,myloc.Location.y);
  }





}