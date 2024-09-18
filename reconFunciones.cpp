      
#include <LiquidCrystal.h>

//declaracion de variables
unsigned int pos = 0;
unsigned int pos2 = 0;
unsigned int cant = 0;
unsigned int cont = 0;
unsigned int cont2 = 1;
int max;
int min;

bool funcAn = false;
bool bul = false;
bool medFrec = true;
bool llenandoArreglo1 = true;

float starTime ;
float endTime ;
int* ptrArray = nullptr;
int* ptrArray2 = nullptr;
int* ptrMax = &max;
int* ptrMin = &min;
LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);

//Declaracion de funciones

//calculo amplitud
 
float calcularAmp(int datos[], int longitud, int* ptrMax, int* ptrMin) {
	int valorMaximo = -2000; 
    int valorMinimo = 2000; 
    
    // Encontrar los valores máximo y mínimo en el arreglo
    for (int i = 1; i < longitud; i++) {
        if (datos[i] > valorMaximo) {
            *ptrMax = datos[i];
          	valorMaximo = datos[i];
        }
        if (datos[i] < valorMinimo) {
            *ptrMin = datos[i];
          	valorMinimo = datos [i];
        }
    }
  float amplitud = (*ptrMax - *ptrMin) / 200.0; // calcular Amplitud 
  return amplitud;										
}


void eliminarArreglo(int ptrArreglo[], int tamano) {
    for (int i = 0; i < tamano; i++) {
        ptrArreglo[i] = 0;
    }
}


//funcion para detectar tipo de señal


int detectarTipoOnda(int datos[], int longitud, int* ptrMax,int* ptrMin) {
  
  //booleanos para saber que funcion
  bool esSenoidal = true;
  bool esTriangular = true;
  bool esCuadrada = true;
  bool calculable = false;
  bool calculable2 = false;
  
  
  int estado = -1; // -1= indet, 0 = decreciendo, 1 = creciendo
  unsigned int contCuadrada = 0;
  unsigned int contMaximos = 0;
  
  int inicioCiclo = -1; // Índice del inicio del ciclo
  int finCiclo = -1;    // Índice del fin del ciclo
  int medio = (*ptrMax - abs(*ptrMin)) /2;
    
  // Buscar el primer cruce en el medio
  for (int c = 0; c < longitud - 1; c++) {
  	if ((datos[c] >= medio && datos[c + 1] < medio) || (datos[c] <= medio && datos[c + 1] >medio)) {
  		inicioCiclo = c;  // Primer cruce 
        break;
    }
  }
    
  // Buscar el siguiente cruce 
  bool cruceM= false;
  int finArreglo = 0;
  for (int  d = inicioCiclo + 1; d < longitud - 1; d++) {
    finArreglo = d;
  	if ((datos[d] >= medio && datos[d + 1] < medio) || (datos[d] <= medio && datos[d + 1] > medio)) {
      	if (cruceM == true){
      		finCiclo = d ; // Segundo cruce 
    		break;
      	}
    	cruceM = true ;  
    }
  }
  //si no hay un segundo cruce
  if (finCiclo == -1){
  	finCiclo = finArreglo;
  }
  
  //
  for (int i= inicioCiclo; i < finCiclo - 1;i++){
  	    int actual = datos[i];
        int anterior = datos[i-1];
    	int siguiente = datos[i+1];
    
    	if (actual > anterior && siguiente > actual){
      		estado = 1;
    	}
    	else if (actual < anterior && siguiente < actual){
    		estado = 0;
    	}
    	else if ( actual == anterior && siguiente == actual){
    		contCuadrada++;
    	}
    
    	if (estado == 1){
          if (siguiente >= *ptrMax -3 && siguiente <= *ptrMax + 3){
          	calculable2 = true;
          	} 
        }
    	
        if (estado == 0){
          if (siguiente >= *ptrMin -3 && siguiente <= *ptrMin + 3){
                calculable = true;
                }
    }       
  } 
  
  if (contCuadrada >= 40){
  	return(2);
  }
  
  if (calculable == false || calculable2 == false){
    	return(-1);
	}
  if (calculable == true && calculable2 == true){
    
    for (int e = inicioCiclo + 1; e < finCiclo - 1; e++) {
 		if (datos[e]== *ptrMax) {  //rangos
    		contMaximos++;
        	}
    	}
          if (contMaximos <=2){
          	return 0;//triangular
          }
          else if (contMaximos > 2 ){
            return 1;//senoidal
          }          
    }
  }
  

//VOID SETUP
void setup()
{
  
  //Inicializar entrada botones 
  pinMode(9,INPUT); 
  pinMode(8,INPUT);
  
  //Inicializar LCD y comunicacion 
  lcd_1.begin(16, 2); 
  Serial.begin(9600);
  
 //esperar a que se estabilice el generador de ondas
  
  for (unsigned int counter = 0; counter < 20; counter++) {
  int a = analogRead(0);
  }
  int start = analogRead(0);
  
//creacion de los arrays principales (no se puede poner en una funcion por el ciclo)
//lee la cantidad de datos hasta el punto max para hacer el tamaño del array
  bool control = true;
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
    	cant = (max - start)*3; 
      	if (max == start){ //si la funcion es cuadrada el inicio es el maximo, por lo que cant = 0 entonces se da un valor trivial de 350 
      		cant = 350;
    	}
    }
  
  }
// un arreglo mas grande y un arreglo auxiliar para optimizar memoria dinamica y reespaldar mientas se reestablecen datos
  ptrArray= new int [cant*2];
  ptrArray2= new int[cant/16];
}

//VOID LOOP
void loop()
{
  int val = analogRead(0);  
  
  //lee el estado de los pulsadores
  unsigned short int pulOn =  digitalRead(9);
  unsigned short int pulOff =  digitalRead(8);
  
  // si se persiona el primer boton
  if(pulOn > 0){
    bul = true;
  	delay(50); //
  }
  

if (llenandoArreglo1 == true && bul == true) {
  //ingresar valores al array
	ptrArray[pos] = val;
  // medir tiempo inicial de un ciclo 
  	if (pos == 0){
  		starTime = millis();
  	}
  
	//Imprime por el monitor serial lo guardado y aumenta la posicion donde se guarda
  	Serial.println(ptrArray[pos]);
    pos++;
  	
  	
  	//busqueda del tiempo final para completar un ciclo
    cont2++;
    if (cont2 > 15){
      if (val >= ptrArray[0]-3 && val <= ptrArray[0]+3){ //estabilizador en frecuencias altas usando cont 2 
          cont ++;
       	  cont2 = 0;  // si pasa 2 veces por el origen se completa un ciclo
          if (cont == 2 && val >= ptrArray[0]-3 && val <= ptrArray[0]+3){
              endTime = millis();  
          }
    	}

  }  
  //si se llena el array el array 1
    if (pos == cant*2) {          
    	eliminarArreglo(ptrArray2, cant/16); // Eliminar el contenido del arreglo2
        pos2 = 0;
        llenandoArreglo1 = false;
      	cont = 0;
      	cont2 = 0;
    }
}
  

else if (llenandoArreglo1 == false && bul == true){

	ptrArray2[pos2] = val;      
    Serial.println(ptrArray2[pos2]);
    pos2++;
        if (pos2 == cant/16) {
          eliminarArreglo(ptrArray, cant*2);// Eliminar el contenido del arreglo1
          pos = 0;
          llenandoArreglo1 = true;
          cont = 0;
      	  cont2 = 0;
              }
          }  

  if (pulOff > 0){
    bul = false;
    funcAn= true;
  	delay(50);
  }
  
  //si se presiona el boton 2 comienza el analisis de los arreglos
  if (funcAn==true){
      
    
    if (llenandoArreglo1 == true){
      	float totalTime = (endTime - starTime)/1000.0;           //calcula el timepo total en hacer un ciclo y lo convierte a s
    	float amplitud= calcularAmp(ptrArray,pos,ptrMax,ptrMin); // Funcion calulo amplitud
    	float frecuencia =  1/totalTime;                         // Funcion calculo frecuencia 
      	int tipOnda = detectarTipoOnda(ptrArray, pos,ptrMax,ptrMin); // Funcion analisis onda
  		
      	// Imprimir el analisis 
      	lcd_1.setCursor(0, 0);
        lcd_1.print("A: ");
      	lcd_1.setCursor(4, 0);
        lcd_1.print(amplitud);
        lcd_1.setCursor(8, 0);
        lcd_1.print(" F:");
        lcd_1.setCursor(12, 0);
        lcd_1.print(frecuencia);
      	lcd_1.setCursor(0,1);	
      	if (tipOnda == -1){
          lcd_1.print("NO REC");
        }
      	else if (tipOnda == 0){
          lcd_1.print("TRIANGULAR");
        }
      	else if (tipOnda == 1){
          lcd_1.print("SENO");
        }
      	else if (tipOnda == 2){
          lcd_1.print("CUAD");
        }
      	
      	//detiene el analisis de funciones y libera el espacio de los arreglos
        funcAn = false;
        delete[] ptrArray;
      	delete[] ptrArray2;
     	       
    	}
      }
    }
    
        