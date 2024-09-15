
// C++ code


float** obtenerDatos() {
    const int NUM_COLUMNAS = 2;
    int numDatos = 0;
    float valor;
    unsigned long tiempoInicio = millis();
    unsigned long tiempoActual;

    float** matriz = new float*[1];

    while (true) {
        tiempoActual = millis();
        if (tiempoActual - tiempoInicio > 5000) {
            break;
        }

        if (numDatos % 1 == 0) {
            matriz = (float**)realloc(matriz, (numDatos + 1) * sizeof(float*));
            matriz[numDatos] = new float[NUM_COLUMNAS];
        }

        valor = analogRead(A0) * (5.0 / 1023.0);
        matriz[numDatos][0] = (tiempoActual - tiempoInicio) / 1000.0;
        matriz[numDatos][1] = valor;

        numDatos++;
    }

    return matriz;
}

void setup() {
    Serial.begin(9600);
    float** datos = obtenerDatos();
}

void loop(){
}
