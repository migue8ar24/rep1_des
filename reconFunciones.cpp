
#include <LiquidCrystal.h>

//declaracion de variables
int val = 0;
int max;
int cant = 0;
bool medFrec = true;
unsigned int tamArray = 0;
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
    int inicioCiclo = -1;
    int finCiclo = -1;

    for (unsigned int i = 0; i < longitud - 1; i++) {
        bool buscandoAscenso = datos[i + 1] > datos[i];  // True si estamos subiendo

        for (int j = i + 1; j < longitud; j++) {
            bool comparandoAscenso = datos[j] > datos[j - 1];

            // Si el valor está en el rango de +-5 y en la misma fase (ascenso/descenso)
            if ((datos[i] - datos[j]) <= 5  && buscandoAscenso == comparandoAscenso) {
                inicioCiclo = i;
                finCiclo = j;
                break;
            }
        }

        if (inicioCiclo != -1 && finCiclo != -1) {
            break;
        }
    }

    if (inicioCiclo != -1 && finCiclo != -1) {
      float frecuencia = 1.0 / (finCiclo - inicioCiclo) * intervaloTiempo;
      return frecuencia;
    } 
    else if {
      return -1;
    }
}


//llenar arreglo
fillArray(bul, ptrFillArray){
  if (bul == true){
    if (pos< cant){
  		ptrFillArray[pos] = val;
      	pos += 1;
    }
    else if (pos>=cant*4){
    	cant *= 2;
    	int* nuevoArray = new int[cant];  
    	for (int i = 0; i < size; i++) {
	  		newArray[i] = ptrFillArray[i];
    	}
    	delete[] ptrFillArray;  
    	ptrFillArray = newArray; 
    }
  }  
  if (bul == true){
  return (ptrArray);
  }
  
}


//VOID SETUP
void setup()
{
  pinMode(9,INPUT);
  pinMode(8,INPUT);
  lcd_1.begin(16, 2); // Set up the number of columns and rows on the LCD.
  Serial.begin(9600);
  int pos = 0;
  int start = analogRead(0);

//creacion del array principal (no se puede poner en una funcion por el ciclo)

  while(cant== 0){
  	int val = analogRead(0);
    max = val;
    if (val>max){
      	max = val;
    }
    else if (val<max){
    	cant = (start - max)*4;
  	}
  }
  int* ptrArrary= new int [cant];
}

//VOID LOOP
void loop()
{
  
  lcd_1.setCursor(0, 0);
  int val = analogRead(0);
  Serial.println(val);
  
  unsigned int pulOn =  digitalRead(9);
  unsigned int pulOff =  digitalRead(8);
  

  
  if(pulOn > 0){
    bool buu = true;
    unsigned float starTime =  millis();
  	delay(250);
  }
  
  
  ptrArray = fillArray(buu,ptrArray);
  
  if (buu == true){
    tamArray += 1;
  }

  if (pulOff > 0){
    buu = false;
    funcAn= true;
  	delay(50)
  }
  
  if (funcAn==true){
  	if(ptrArray[0] == val && medFrec == true){
      unsigned float endTime =  millis();
      medFrec = false;
    }
    unsigned float totalTime = (endTime -starTime);
    unsigned float amplitud = calcularAmp(ptrArray, tamArray);
    float frecuencia =  calcularFrecuencia(ptrArray, tamArray, totalTime)
  
  }
    
}
    
  
        