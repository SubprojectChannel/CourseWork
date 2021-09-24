/*
String arrayToString(float[] array, String text) {
  text += "{";
  for (int i = 0; i < array.length; i++) {
    if (i < array.length-1) text += (str(array[i]) + ",");
    else text += (str(array[i]));
  }
  text += "}";
  return text;
}

String arrayToString(float[][] array, String text) {
  text += "{\n";
  for (int i = 0; i < array.length; i++) {
    text = arrayToString(array[i], text);
    if (i < array.length-1) text += ",\n";
    else {
    }
  }
  text += "}";
  return text;
}
*/
String initialisePointers(Matrix a[], String pointerName) {
  String text = "";
  text += "char* " + pointerName + "[] = new char*[" + a.length + "];\n";
  return text;
}

String assignPointers(Matrix a[], String pointerName) {
  String text = "";
  for (int i = 0; i < a.length; i++) {
    text += pointerName + "[" + i + "] = " + a[i].name + ";\n";
  }
  return text;
}

String definePointers(Matrix a[], String pointerName) {
  String text = "";
  text += "char* " + pointerName + "[] = {";
  for (int i = 0; i < a.length; i++) {
    text += a[i].name;
    if (i < a.length - 1) text += ", ";
  }
  text+= "};\n";
  return text;
}
