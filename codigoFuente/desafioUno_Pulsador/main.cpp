// C++ code

const int pinAnalogico = A0;
const unsigned long intervaloCaptura = 2;
const unsigned long duracionCaptura = 2000;

void setup() {
    Serial.begin(9600);
}

void loop() {

    float amplitud = calcularAmplitud(pinAnalogico, duracionCaptura, intervaloCaptura);
    float frecuencia = calcularFrecuencia(pinAnalogico, duracionCaptura, intervaloCaptura);
    Serial.print("Amplitud: ");
    Serial.print(amplitud);
    Serial.print(" V, Frecuencia: ");
    Serial.print(frecuencia);
    Serial.println(" Hz");
}

float calcularAmplitud(int pinAnalogico, unsigned long duracionCaptura, int intervaloCaptura) {
    unsigned long tiempoInicio = millis();
    int valorMax = 0;
    int valorMin = 1023;

    while (millis() - tiempoInicio < duracionCaptura) {
        int valorActual = analogRead(pinAnalogico);
        if (valorActual > valorMax) valorMax = valorActual;
        if (valorActual < valorMin) valorMin = valorActual;
        delay(intervaloCaptura);
    }

    return (valorMax - valorMin) * (5.0 / 1023.0);
}

float calcularFrecuencia(int pinAnalogico, unsigned long duracionCaptura, int intervaloCaptura) {
    unsigned long tiempoInicio = millis();
    int ciclosCompletos = 0;
    int valorAnterior = analogRead(pinAnalogico);
    int umbral = 512;
    int histeresis = 10;
    unsigned long tiempoUltimaCaptura = millis();

    while (millis() - tiempoInicio < duracionCaptura) {
        if (millis() - tiempoUltimaCaptura >= intervaloCaptura) {
            int valorActual = analogRead(pinAnalogico);

            if ((valorActual > umbral + histeresis && valorAnterior <= umbral + histeresis) ||
                (valorActual < umbral - histeresis && valorAnterior >= umbral - histeresis)) {
                ciclosCompletos++;
            }

            valorAnterior = valorActual;
            tiempoUltimaCaptura = millis();
        }
    }

    float tiempoTotal = duracionCaptura / 1000.0;
    if (tiempoTotal > 0) {
        return (float)ciclosCompletos / (2.0 * tiempoTotal);
    } else {
        return 0.0;
    }
}
