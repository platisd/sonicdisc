
class LocationData{
    xy Location;
    xy Size;  

    LocationData(xy alocation, xy asize){
      Location = new xy(alocation.x,alocation.y);
      Size = new xy(asize.x,asize.y);      
    }
    
  boolean mouseInside()
  {  
    if ((mouseX > Location.x-Size.x/2) && (mouseX <= Location.x+Size.x/2)){
      if ((mouseY > Location.y-Size.y/2) && (mouseY <= Location.y+Size.y/2)) return true;
    }
    return false;
  }
  
  boolean clicked()
  {
    return mousePressed&&mouseInside();
  }
  
  
  
  boolean isInside(LocationData thingToBeInside) {  
    return isInside(thingToBeInside.Location);
  }
  
  boolean isInside(xy pointToBeInside) {  
    if ((pointToBeInside.x > Location.x-Size.x/2) && (pointToBeInside.x <= Location.x+Size.x/2)){
      if ((pointToBeInside.y > Location.y-Size.y/2) && (pointToBeInside.y <= Location.y+Size.y/2)) return true;
    }
    return false;
  }  
  
  
  boolean isOverlap(LocationData thingToBeOverlap) {  
    
    if (thingToBeOverlap.isInside(new xy(Location.x+Size.x/2,Location.y+Size.y/2))) return true;
    if (thingToBeOverlap.isInside(new xy(Location.x-Size.x/2,Location.y+Size.y/2))) return true;
    if (thingToBeOverlap.isInside(new xy(Location.x+Size.x/2,Location.y-Size.y/2))) return true;
    if (thingToBeOverlap.isInside(new xy(Location.x-Size.x/2,Location.y-Size.y/2))) return true;
    return false;
  }
}


class FlagData{
  
  boolean enabled;
  boolean visible;
  boolean deleteFlag;
  int id;
  String label;
  
  FlagData(){
    label = "";
    enabled = true;
    visible = true;
    deleteFlag = false;
  }  
  
  FlagData(String alabel){
    label = alabel;
    enabled = true;
    visible = true;
    deleteFlag = false;
  }  
  
  
}