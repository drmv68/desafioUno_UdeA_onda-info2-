// C++ code

const int pinAnalogico = A0;
const unsigned long intervaloCaptura = 2;
const unsigned long duracionCaptura = 2000;
const int numSamples = 500;
int samples[numSamples];

const int botonIniciar = 23;
const int botonImprimir = 27;

float calcularAmplitud(int pinAnalogico, unsigned long duracionCaptura, int intervaloCaptura);
float calcularFrecuencia(int pinAnalogico, unsigned long duracionCaptura, int intervaloCaptura);
String identificarSenal(int datos[], int longitud);
int contarPicos(int datos[], int longitud);
int contarCruces(int datos[], int longitud);
int contarPico(int datos[], int longitud);

void setup() {
    Serial.begin(9600);
    pinMode(botonIniciar, INPUT_PULLUP);
    pinMode(botonImprimir, INPUT_PULLUP);
}

void loop() {
    static bool capturando = false;       // Controla el estado de la captura
    static bool datosCapturados = false;  // Indica si ya se capturaron los datos
    static bool botonIniciarPresionado = false;  // Control para evitar múltiples lecturas del botón
    static bool botonImprimirPresionado = false; // Control para evitar múltiples impresiones

    float amplitud = 0, frecuencia = 0;

    // Verificar si se ha presionado el botón de inicio (para comenzar la captura de datos)
    if (digitalRead(botonIniciar) == LOW && !botonIniciarPresionado) {
        botonIniciarPresionado = true;
        capturando = true;
        datosCapturados = false;  // Reiniciar el estado de los datos
    }

    // Si el botón de inicio ya no está presionado, restablecer el control
    if (digitalRead(botonIniciar) == HIGH) {
        botonIniciarPresionado = false;
    }

    // Capturar los datos una sola vez
    if (capturando && !datosCapturados) {
        amplitud = calcularAmplitud(pinAnalogico, duracionCaptura, intervaloCaptura);
        frecuencia = calcularFrecuencia(pinAnalogico, duracionCaptura, intervaloCaptura);

        for (int i = 0; i < numSamples; i++) {
            samples[i] = analogRead(pinAnalogico);  // Captura los datos
        }

        datosCapturados = true;  // Indica que los datos ya fueron capturados
        capturando = false;  // Termina la captura
    }

    // Verificar si se ha presionado el botón de imprimir (para mostrar los datos capturados)
    if (digitalRead(botonImprimir) == LOW && datosCapturados && !botonImprimirPresionado) {
        botonImprimirPresionado = true;

        Serial.print("Amplitud: ");
        Serial.print(amplitud);
        Serial.print(" V, Frecuencia: ");
        Serial.print(frecuencia);
        Serial.println(" Hz");

        String tipoSenal = identificarSenal(samples, numSamples);
        Serial.println("Tipo de señal: " + tipoSenal);

        delay(500);  // Evitar múltiples impresiones continuas
    }

    // Si el botón de imprimir ya no está presionado, restablecer el control
    if (digitalRead(botonImprimir) == HIGH) {
        botonImprimirPresionado = false;
    }

    delay(100);  // Control de bucle para evitar lecturas continuas rápidas
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
