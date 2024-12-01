#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct nodoDatos {
  string palabra;
  int repeticiones;

  nodoDatos(string p) : palabra(p), repeticiones(1) {}
};

struct nodoAVL {
  int FB;
  nodoDatos *info;
  nodoAVL *lista;
  nodoAVL *izq;
  nodoAVL *der;
  int alturaPos;

  nodoAVL(string p)
      : FB(0), info(new nodoDatos(p)), lista(nullptr), izq(nullptr),
        der(nullptr), alturaPos(1) {}
};

struct nodoPosicional {
  nodoAVL *nodoLista;
  nodoPosicional *izq;
  nodoPosicional *der;

  nodoPosicional() { nodoLista = nullptr, izq = nullptr, der = nullptr; };
};

class arbolAVL {
  nodoAVL *raiz;
  nodoAVL *czo;
  nodoPosicional *raizPos;
  int cantPalabras = 0;
  int comparaciones = 0;
  void Insert(string x, bool &aumento, nodoAVL *&A, int& tamanio);
  void rotarLL(nodoAVL *&A);
  void rotarRR(nodoAVL *&A);
  void rotarLRalter(nodoAVL *&A);
  void rotarRLalter(nodoAVL *&A);
  void enOrden(nodoAVL *A);
  void construirListaOrdenada(nodoAVL *A);
  void construirArbolPos(int altura, nodoPosicional *&pos);
  void asignarPos(nodoAVL *&lista, nodoPosicional *&pos);
  void ordenaSeleccion();
  void ordenaQS(int a, int b);
  
public:
  arbolAVL() { raiz = nullptr, raizPos = nullptr; czo = nullptr;};
  ~arbolAVL() {} // No se implementa el destructor para liberar memoria.
                 // Necesario para evitar fugas de memoria.
  void CreaArbolAVL(string x);
  void impresionOrdenada() { enOrden(raiz); }
  void construirListaOrdenada() {construirListaOrdenada(raiz);};
  void imprimirListaOrdenada();
  nodoAVL *accederPosicion(int k);
  void ordenarPorRepeticiones();
  void ordenarAlfabeticamente();
  void construirArbolPos(){construirArbolPos(ceil(log2(cantPalabras)), raizPos);asignarPos(czo, raizPos); czo = new nodoAVL(""); czo->lista = accederPosicion(1);};
};

void arbolAVL::CreaArbolAVL(string x) {
  bool aumento = false;
  Insert(x, aumento, raiz, cantPalabras);
}

void arbolAVL::Insert(string x, bool &aumento, nodoAVL *&A,int& tamanio) {
  if (A == nullptr) {
    A = new nodoAVL(x);
    tamanio++;
    aumento = true;
  } else {
    if (x < A->info->palabra) {
      Insert(x, aumento, A->izq, cantPalabras);
      if (aumento) {
        switch (A->FB) {
        case -1:
          A->FB = 0;
          aumento = false;
          break;
        case 0:
          A->FB = 1;
          aumento = true;
          break;
        case 1:
          if (A->izq->FB == 1) {
            rotarLL(A);
          } else {
            rotarLRalter(A);
          }
          aumento = false;
          break;
        }
      }
    } else if (x > A->info->palabra) {
      Insert(x, aumento, A->der, cantPalabras);
      if (aumento) {
        switch (A->FB) {
        case -1:
          if (A->der->FB == 1) {
            rotarRLalter(A);
          } else {
            rotarRR(A);
          }
          aumento = false;
          break;
        case 0:
          A->FB = -1;
          aumento = true;
          break;
        case 1:
          A->FB = 0;
          aumento = false;
          break;
        }
      }
    } else {
      A->info->repeticiones++;
    }
  }
}

void arbolAVL::rotarLL(nodoAVL *&A) {
  nodoAVL *aux = A->izq;
  A->izq = aux->der;
  aux->der = A;
  A->FB = 0;
  A = aux;
}

void arbolAVL::rotarRR(nodoAVL *&A) {
  nodoAVL *aux = A->der;
  A->der = aux->izq;
  aux->izq = A;
  A->FB = 0;
  A = aux;
}

void arbolAVL::rotarLRalter(nodoAVL *&A) {
  rotarRR(A->izq);
  rotarLL(A);
}

void arbolAVL::rotarRLalter(nodoAVL *&A) {
  rotarLL(A->der);
  rotarRR(A);
}

void arbolAVL::construirListaOrdenada(nodoAVL *A) {
  if (A != nullptr) {
    construirListaOrdenada(A->izq); // Recorrer el subárbol izq
    if (czo == nullptr) {
      czo = new nodoAVL("");
      czo->lista= A;

    } else {
      nodoAVL *temp = czo->lista;
      while (temp->lista != nullptr) {
        temp = temp->lista;
      }
      temp->lista = A;
    }

    construirListaOrdenada(A->der); // Recorrer el subárbol der
  }
}

void arbolAVL::imprimirListaOrdenada(){
  for(int i = 1; i<= cantPalabras; i++){
    cout << accederPosicion(i)->info->palabra << " : " << accederPosicion(i)->info->repeticiones << endl;
  }
}

void arbolAVL::construirArbolPos(int altura, nodoPosicional *&pos) {
  if (altura == -1) {
  } else if (pos == NULL) {
    pos = new nodoPosicional();
    construirArbolPos(altura - 1, pos->izq);
    construirArbolPos(altura - 1, pos->der);
  }
}

void arbolAVL::asignarPos(nodoAVL *&lista, nodoPosicional *&pos) {
  if (lista == nullptr) {
  } 
  else {
    if (pos->izq == nullptr && pos->der == nullptr) {
      // Es una hoja, asigna el nodo de la lista y avanza en la lista
      pos->nodoLista = lista->lista;
      lista = lista->lista;

    }
    else{
      asignarPos(lista, pos->izq);
      asignarPos(lista, pos->der);
    }
  }
}

nodoAVL* arbolAVL::accederPosicion(int k) { 
    if(k>cantPalabras){
      cout<<"Posicion invalida" << endl;
      return nullptr;
    }
    nodoPosicional* nodo = raizPos;
    k--; // Ajustar el índice k para la conversión binaria
    for (int i = ceil(log2(cantPalabras)) -1 ; i >= 0; i--) {
        if (k & (1 << i)) {
            if (nodo->der != nullptr) {
                nodo = nodo->der;
            } else {
                break;
            }
        } 
        else {
            if (nodo->izq != nullptr) {
                nodo = nodo->izq;
            } else {
                break;
            }
        }
    }
    return nodo->nodoLista;
}

void arbolAVL::ordenaSeleccion() {
  nodoAVL* mayor;
  for(int i=1; i<=cantPalabras; i++){
    mayor = accederPosicion(i);
    for(int j=i+1; j<=cantPalabras; j++){
        comparaciones++;
        if(accederPosicion(j)->info->repeticiones > mayor->info->repeticiones){
          
          swap(accederPosicion(j)->info,mayor->info);
        }
    }
    swap(accederPosicion(i)->info,mayor->info);
  }
}
void arbolAVL::ordenarPorRepeticiones(){
  comparaciones = 0;
  ordenaSeleccion();
  cout << "Comparaciones: " << comparaciones << endl;
}
// Quick-sort para ordenar alfabéticamente
void arbolAVL::ordenaQS(int primero, int ultimo) {
    if (primero < ultimo) {
        int i = primero, j = ultimo;
        nodoAVL* pivot = accederPosicion(ultimo);
        while (i < j) {
            while (i < j && accederPosicion(i)->info->palabra <= pivot->info->palabra) {
                i++; comparaciones++;
            }
            while (i < j && accederPosicion(j)->info->palabra >= pivot->info->palabra) {
                j--; comparaciones++;
            }
            if (i < j) {
                swap(accederPosicion(i)->info, accederPosicion(j)->info);
            }
        }
        swap(accederPosicion(i)->info,accederPosicion(ultimo)->info);
        ordenaQS(primero, i - 1);
        ordenaQS(i + 1, ultimo);
    }
}

void arbolAVL::ordenarAlfabeticamente() {
    comparaciones = 0;
    ordenaQS(1, cantPalabras);
    cout << "Comparaciones: " << comparaciones << endl;
}

void arbolAVL::enOrden(nodoAVL *A) {
  if (A != nullptr) {
    enOrden(A->izq);
    cout << A->info->palabra << ": " << A->info->repeticiones << endl;
    enOrden(A->der);
  }
}

string estandarizarPalabra(string &palabra) {
  string estandar;
  for (char c : palabra) {
    if (isalpha(c)) {
      estandar += tolower(c);
    }
  }
  return estandar;
}

int main() {
  string entrada, palabra;
  arbolAVL T;
  ifstream archivo("text.txt");

  if (!archivo.is_open()) {
    cout << "Error en abrir el archivo" << endl;
    return 1;
  }

  while (getline(archivo, entrada)) {
    stringstream ss(entrada);
    while (ss >> palabra) {
      palabra = estandarizarPalabra(palabra);
      if (!palabra.empty()) {
        T.CreaArbolAVL(palabra);
      }
    }
  }

  archivo.close();
  T.construirListaOrdenada();
  T.construirArbolPos();
  cout << "Lista ordenada alfabeticamente:" << endl;
  T.imprimirListaOrdenada();
  
  cout << "Lista ordenada por repeticiones:" << endl;
  T.ordenarPorRepeticiones();
  T.imprimirListaOrdenada();
  
  cout << "Lista ordenada alfabeticamente:" << endl;
  T.ordenarAlfabeticamente();
  T.imprimirListaOrdenada();


  return 0;
}

