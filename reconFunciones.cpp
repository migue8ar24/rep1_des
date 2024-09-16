#include <LiquidCrystal.h>

//declaracion de variables
unsigned int tamArray = 0;
unsigned int pos = 0;
unsigned int pos2 = 0;
int cant = 0;
int max;
bool funcAn = false;
bool bul = false;
bool medFrec = true;
bool control = true;
bool llenandoArreglo1 = true;

float starTime ;
float endTime ;
int* ptrArray = nullptr;
int* ptrArray2 = nullptr;
LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);

//Declaracion de funciones

//calculo datos

float calcularAmp(int datos[], int longitud) {
    int valorMaximo = datos[0];
    int valorMinimo = datos[0];

    // Encontrar los valores máximo y mínimo en el arreglo
    for (int i = 1; i < longitud; i++) {
        if (datos[i] > valorMaximo) {
            valorMaximo = datos[i];
        }
        if (datos[i] < valorMinimo) {
            valorMinimo = datos[i];
        }
    }

    float amplitud = (valorMaximo - valorMinimo) / 2.0;

    return amplitud;
}

// calcular Frecuencia

float calcularFrecuencia(int datos[], int longitud, float intervaloTiempo) {

    float tiempoMuestreo = intervaloTiempo / longitud; // Calcular el tiempo de muestreo por dato

    int indiceInicio = 0;
    int indiceFin = -1;
    if (indiceInicio >= longitud) {
        return 0.0;
    }

    for (int i = 1; i < longitud; i++) {
        if (datos[i] >= datos[indiceInicio] - 3 && datos[i] <= datos[indiceInicio] + 3) {
            indiceFin = i; // Se ha encontrado el final del primer ciclo.
            break;
        }
    }

    if (indiceFin == -1) {
        return 0.0;
    }

    int duracionCiclo = indiceFin - indiceInicio; //Cantidad de muestras que tomó completar un ciclo.

    float duracionCicloSegundos = duracionCiclo * tiempoMuestreo; //Tiempo que duró un cilo completo (s)

    if (duracionCicloSegundos > 0) {
        return 1.0 / duracionCicloSegundos;  // Frecuencia en Hz
    } else {
        return 0.0;
    }
}



void eliminarArreglo(int arreglo[], int tamano) {
    for (int i = 0; i < tamano; i++) {
        arreglo[i] = 0;
    }
}

//VOID SETUP
void setup()
{
    pinMode(9,INPUT);
    pinMode(8,INPUT);
    lcd_1.begin(16, 2); // Set up the number of columns and rows on the LCD.
    Serial.begin(9600);

    //esperar a que se estabilice el generador de ondas

    for (int counter = 0; counter < 20; counter++) {
        int a = analogRead(0);
    }
    int start = analogRead(0);

    //creacion de los arrays principales (no se puede poner en una funcion por el ciclo)

    while(cant== 0){
        int val = analogRead(0);
        if (control == true){
            max = val-1;
            control = false;
        }
        if (val>max){
            max = val;
        }
        else if (val<max){
            cant = (max - start)*4;
        }
    }
    ptrArray= new int [cant/2];
    ptrArray2= new int[cant/2];
}

//VOID LOOP
void loop()
{
    int val = analogRead(0);

    unsigned int pulOn =  digitalRead(9);
    unsigned int pulOff =  digitalRead(8);



    if(pulOn > 0){
        bul = true;
        //starTime =  millis();
        delay(50);
    }

    if (llenandoArreglo1 == true && bul == true) {
        starTime =  millis();
        ptrArray[pos] = val;
        Serial.println(ptrArray[pos]);
        pos++;

        if (pos == cant/2) {
            eliminarArreglo(ptrArray2, cant/2); // Eliminar el contenido del arreglo2
            pos2 = 0;
            llenandoArreglo1 = false;
            endTime =  millis();
        }
    }
    else if (llenandoArreglo1 == false && bul == true){
        starTime =  millis();
        ptrArray2[pos2] = val;
        Serial.println(ptrArray2[pos2]);
        pos2++;

        if (pos2 == cant/2) {
            eliminarArreglo(ptrArray, cant/2);// Eliminar el contenido del arreglo1
            pos = 0;
            llenandoArreglo1 = true;
            endTime =  millis();
        }
    }

    if (pulOff > 0){
        bul = false;
        funcAn= true;
        delay(50);
    }

    if (funcAn==true){
        if(ptrArray[0] == val && medFrec == true){
            //endTime =  millis();
            medFrec = false;

            if (llenandoArreglo1 == true){
                float totalTime = (endTime -starTime)/1000.0;
                float amplitud = calcularAmp(ptrArray,pos);
                float TiempoTotal = 1.0;
                float frecuencia =  calcularFrecuencia(ptrArray, pos, TiempoTotal);
                lcd_1.setCursor(0, 0);
                lcd_1.print(amplitud);
                lcd_1.setCursor(0, 1);
                lcd_1.print(frecuencia);
                funcAn = false;
                medFrec = true ;

            }
            else{

                float totalTime = (endTime -starTime)/1000.0;
                float amplitud = calcularAmp(ptrArray2,pos2);
                float TiempoTotal = 1.0;
                float frecuencia =  calcularFrecuencia(ptrArray2, pos, TiempoTotal);
                lcd_1.setCursor(0, 0);
                lcd_1.print(amplitud);
                lcd_1.setCursor(0, 1);
                lcd_1.print(frecuencia);
                funcAn = false;
                medFrec = true ;

            }
        }
    }


}
