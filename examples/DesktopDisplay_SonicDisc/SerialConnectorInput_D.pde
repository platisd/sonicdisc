
void serialEvent (Serial myPort) {
  String theInput = myPort.readStringUntil('\n'); //Gets val
  if (processInputs(theInput)) displayInputs();
}

boolean processInputs(String tInputs) {
  if (tInputs.charAt(0) == 'S') {
    // print(tInputs);
    String theValues[] = split(tInputs, ",");
    if (theValues.length!=10) return false; // exit if the expected number of variables not present
    sonic0 = int(theValues[1]);
    sonic1 = int(theValues[2]);
    sonic2 = int(theValues[3]);
    sonic3 = int(theValues[4]);
    sonic4 = int(theValues[5]);
    sonic5 = int(theValues[6]);
    sonic6 = int(theValues[7]);
    sonic7 = int(theValues[8]);
  }
  return true;
}
void displayInputs() {
  print("sonic0 :" + str(sonic0));
  print("sonic1 :" + str(sonic1));
  print("sonic2 :" + str(sonic2));
  print("sonic3 :" + str(sonic3));
  print("sonic4 :" + str(sonic4));
  print("sonic5 :" + str(sonic5));
  print("sonic6 :" + str(sonic6));
  print("sonic7 :" + str(sonic7));
  println();
}