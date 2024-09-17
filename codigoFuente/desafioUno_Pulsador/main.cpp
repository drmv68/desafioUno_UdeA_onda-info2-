// C++ code

const int pinAnalogico = A0;
const unsigned long intervaloCaptura = 2;
const unsigned long duracionCaptura = 2000;
const int numSamples = 500;
int samples[numSamples];

float calcularAmplitud(int pinAnalogico, unsigned long duracionCaptura, int intervaloCaptura);
float calcularFrecuencia(int pinAnalogico, unsigned long duracionCaptura, int intervaloCaptura);
String identificarSenal(int datos[], int longitud);
int contarPicos(int datos[], int longitud);
int contarCruces(int datos[], int longitud);
int contarPico(int datos[], int longitud);

void setup() {
    Serial.begin(9600);
    delay(2000);
}

void loop() {
    float amplitud = calcularAmplitud(pinAnalogico, duracionCaptura, intervaloCaptura);
    float frecuencia = calcularFrecuencia(pinAnalogico, duracionCaptura, intervaloCaptura);

    Serial.print("Amplitud: ");
    Serial.print(amplitud);
    Serial.print(" V, Frecuencia: ");
    Serial.print(frecuencia);
    Serial.println(" Hz");

    for (int i = 0; i < numSamples; i++) {
        samples[i] = analogRead(pinAnalogico);
    }

    String tipoSenal = identificarSenal(samples, numSamples);
    Serial.println("Tipo de señal: " + tipoSenal);

    delay(5000);
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

String identificarSenal(int datos[], int longitud) {
    int picos = contarPicos(datos, longitud);
    int cruces = contarCruces(datos, longitud);
    int pico = contarPico(datos, longitud);

    if (pico > 5) {
        return "Señal desconocida";
    }

    if (picos >= cruces) {
        return "Señal Triangular";
    }

    if (picos == 0) {
        return "Señal Cuadrada";
    }

    if (cruces > picos) {
        return "Señal Senoidal";
    }
}

int contarPicos(int datos[], int longitud) {
    int picos = 0;
    for (int i = 1; i < longitud - 1; i++) {
        if (datos[i] > datos[i - 1] && datos[i] > datos[i + 1]) {
            picos++;
        }
    }
    return picos;
}

int contarCruces(int datos[], int longitud) {
    int cruces = 0;
    for (int i = 1; i < longitud; i++) {
        if ((datos[i - 1] < 480 && datos[i] >= 480) || (datos[i - 1] >= 480 && datos[i] < 480)) {
            cruces++;
        }
    }
    return cruces;
}

int contarPico(int datos[], int longitud) {
    int picos = 0;
    int aux = 0;
    for (int i = 1; i < longitud - 1; i++) {
        if (datos[i] > datos[i - 1] && datos[i] > datos[i + 1]) {
            if (datos[i] > aux) {
                picos++;
                aux = datos[i];
            }
        }
    }
    return picos;
}
