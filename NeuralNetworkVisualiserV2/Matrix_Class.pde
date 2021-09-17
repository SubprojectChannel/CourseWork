class Matrix {
  int rows, cols;
  float data[][];
  String name = "matrix";
  
  Matrix(int rows_, int cols_) {
    rows = rows_;
    cols = cols_;
    data = new float[rows][cols];
    for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = 0;
    //randomise();
  }
  
  Matrix(float[][] data_) {
    rows = data_.length;
    cols = data_[0].length;
    data = data_;
  }
  
  void setName(String name_) {
    name = name_;
  }
  void randomise() {
    for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = random(-1, 1);
  }

  void multiply(float scaler) {
    for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] *= scaler;
  }

  void multiply(Matrix a) {
    for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] *= a.data[i][j];
  }

  void add(float add) {
    for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] += add;
  }

  void add(Matrix a) {
    for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] += a.data[i][j];
  }


  void map(Function f, float[] parameters) {
    for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = Function.apply(data[i][j], parameters, i, j, f.index);
  }
  void map(Function f, float a) {
    float parameters[] = {a};
    for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = Function.apply(data[i][j], parameters, i, j, f.index);
  }
  void map(Function f) {
    float parameters[] = {};
    for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) data[i][j] = Function.apply(data[i][j], parameters, i, j, f.index);
  }

  void transpose() {
    int oldRows = rows;
    int oldCols = cols;
    rows = oldCols;
    cols = oldRows;
    float newData[][] = new float[oldCols][oldRows];
    for (int i = 0; i < oldRows; i++) {
      for (int j = 0; j < oldCols; j++) {
        newData[j][i] = data[i][j];
      }
    }
    data = newData;
  }


  void printOut(String name) {
    println("Matrix", name, "= {");
    for (int i = 0; i < data.length; i++) {
      print("{");
      for (int j = 0; j < data[0].length; j++) {
        if (j < data[0].length - 1) print(str(data[i][j]) + ", ");
        else print(str(data[i][j]));
      }
      if (i < data.length-1) println("},");
      else println("}");
    }
    println("};");
  }
}




Matrix MatrixMultiply(Matrix A, Matrix B) {
  if (A.cols != B.rows) {
    println("Size Mismatch in MatrixMultiply(Matrix a)");
    return B;
  }
  float a[][] = A.data;
  float b[][] = B.data;
  Matrix newMatrix = new Matrix(A.rows, B.cols);
  for (int i = 0; i < newMatrix.rows; i++) {
    for (int j = 0; j < newMatrix.cols; j++) {
      float sum = 0;
      for (int k = 0; k < A.cols; k++) {
        sum += a[i][k] * b[k][j];
      }
      newMatrix.data[i][j] = sum;
    }
  }
  return newMatrix;
}

Matrix MatrixSubtract(Matrix a, Matrix b) {
  Matrix result = new Matrix(a.rows, a.cols);
  for (int i = 0; i < a.rows; i++) for (int j = 0; j < a.cols; j++) result.data[i][j] = a.data[i][j] - b.data[i][j];
  return result;
}

Matrix MatrixTranspose(Matrix b) {
  Matrix newMatrix = new Matrix(b.cols, b.rows);
  for (int i = 0; i < b.rows; i++) {
    for (int j = 0; j < b.cols; j++) {
      newMatrix.data[j][i] = b.data[i][j];
    }
  }
  return newMatrix;
}

Matrix MatrixFromArray(float[] array) {
  Matrix m = new Matrix(array.length, 1);
  for (int i = 0; i < m.rows; i++) {
    m.data[i][0] = array[i];
  }
  return m;
}

float[] MatrixToArray(Matrix m) {
  float a[] = new float[m.rows*m.cols];
  for (int i = 0; i < m.rows; i++) {
    for (int j = 0; j < m.cols; j++) {
      a[i*m.cols+j] = m.data[i][j];
    }
  }
  return a;
}

Matrix map(Matrix M, Function f, float[] parameters) {
  Matrix m = M;
  for (int i = 0; i < m.rows; i++) for (int j = 0; j < m.cols; j++) m.data[i][j] = Function.apply(M.data[i][j], parameters, i, j, f.index);
  return m;
}
Matrix map(Matrix M, Function f, float a) {
  Matrix m = M;
  float parameters[] = {a};
  for (int i = 0; i < m.rows; i++) for (int j = 0; j < m.cols; j++) m.data[i][j] = Function.apply(M.data[i][j], parameters, i, j, f.index);
  return m;
}
Matrix map(Matrix M, Function f) {
  Matrix m = M;
  float parameters[] = {};
  for (int i = 0; i < m.rows; i++) for (int j = 0; j < m.cols; j++) m.data[i][j] = Function.apply(M.data[i][j], parameters, i, j, f.index);
  return m;
}


static class Function {
  int index;
  Function(int i) {
    index = i;
  }
  static float apply(float N, float[] parameters, int i, int j, int Index) {
    float result = 0;
    switch(Index) {
    case 0: //double
      result = sigmoid(N, parameters, i, j);
      break;
    case 1: //half
      result = dSigmoid(N, parameters, i, j);
      break;
    case 2:
      result = multiply(N, parameters, i, j);
      break;
    default:
      break;
    }
    return result;
  }
  
  static float sigmoid(float N, float[] parameters, int i, int j) {
    float e = 2.71828;
    float y;
    y = 1.0/(1.0+pow(e, -N));
    return y;
  }
  
  static float dSigmoid(float N, float[] parameters, int i, int j) {
    return N*(1-N);
  }
  
  static float multiply(float N, float[] parameters, int i, int j) {
    float scaler = parameters[0];
    return N*scaler;
  }
}
