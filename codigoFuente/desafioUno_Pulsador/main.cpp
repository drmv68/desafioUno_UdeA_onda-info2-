
// C++ code


const int pinAnalogico = A0;
const unsigned long intervaloCaptura = 5;
const unsigned long duracionCaptura = 5000;

float amplitud;
float frecuencia;

void setup() {
    Serial.begin(9600);
}

void loop() {
    capturarAmplitudFrecuencia();

    Serial.print("Amplitud: ");
    Serial.print(amplitud);
    Serial.print(" V, Frecuencia: ");
    Serial.print(frecuencia);
    Serial.println(" Hz");

    delay(1000);
}

void capturarAmplitudFrecuencia() {
    unsigned long tiempoInicio = millis();
    int valorMax = 0;
    int valorMin = 1023;
    int ciclosCompletos = 0;
    int valorAnterior = analogRead(pinAnalogico);
    int umbral = 512;
    int histeresis = 10;

    while (millis() - tiempoInicio < duracionCaptura) {
        int valorActual = analogRead(pinAnalogico);
        if (valorActual > valorMax) valorMax = valorActual;
        if (valorActual < valorMin) valorMin = valorActual;

        if ((valorActual > umbral + histeresis && valorAnterior <= umbral + histeresis) ||
            (valorActual < umbral - histeresis && valorAnterior >= umbral - histeresis)) {
            ciclosCompletos++;
        }

        valorAnterior = valorActual;
        delay(intervaloCaptura);
    }

    amplitud = (valorMax - valorMin) * (5.0 / 1023.0);
    float tiempoTotal = duracionCaptura / 1000.0;
    if (tiempoTotal > 0) {
        frecuencia = (float)ciclosCompletos * 1000.0 / (2.0 * intervaloCaptura * tiempoTotal);
    } else {
        frecuencia = 0.0;
    }
}
