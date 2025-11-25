#include <iostream>
#include "ubicacio.hpp"
using namespace std;

int main(){
    int i = -1, j = -1, k = -1;
    cout << "Se crea U1 = (-1, -1, -1)" << endl;
    ubicacio U1(1, 1, 1);
    try{
        U1 = ubicacio(i, j, k);
    }catch(const error &er){
        cerr << er.codigo() << " " << er.modulo() << " " << er.mensaje() << endl;
    }
    
    i = -2;
    j = 1;
    k = 1;
    cout << "Se intenta crear U2 = (-2, 1, 1)" << endl;
    ubicacio U2(1, 1, 1);
    try{
        U2 = ubicacio(i, j, k);
    }catch(const error &er){
        cerr << er.codigo() << " " << er.modulo() << " " << er.mensaje() << endl;
    }
    
    i = 2;
    j = 1;
    k = 1;
    cout << "Se crea U3 = (2, 1, 1)" << endl;
    ubicacio U3(1, 1, 1);
    try{
        U3 = ubicacio(i, j, k);
    }catch(const error &er){
        cerr << er.codigo() << " " << er.modulo() << " " << er.mensaje() << endl;
    }
    
    cout << "\n   Constructor por copia\n" << endl;
    ubicacio U4 = ubicacio(U1);
    cout << "       ubicacio U4 = ubicacio(U1)" << endl;
    cout << "       Filera: " << U4.filera() << " Plaça: " << U4.placa() << " Pis: " << U4.pis() << endl;

    cout << "\n   Asignación\n" << endl;
    U4 = U3;
    cout << "       U4 = U3" << endl;
    cout << "       Filera: " << U4.filera() << " Plaça: " << U4.placa() << " Pis: " << U4.pis() << endl;

    cout << "\n   Operador ==\n" << endl;

    cout << "       U4 = (2, 1, 1)" << endl;
    cout << "       U4 == U3: " << (U4 == U3) << endl;
    cout << "       U4 == U1: " << (U4 == U1) << endl;

    cout << "\n   Operacdor !=\n" << endl;

    cout << "       U4 = (2, 1, 1)" << endl;
    cout << "       U4 != U3: " << (U4 != U3) << endl;
    cout << "       U4 != U1: " << (U4 != U1) << endl;

    cout << "\n   Operador <\n" << endl;

    cout << "       U4 = (2, 1, 1)" << endl;
    i = 2;
    j = 0;
    k = 0;

    U1 = ubicacio(i, j, k);
    cout << "       U1 = (2, 0, 0)" << endl;
    cout << "       U1 < U4: " << (U1 < U4) << endl;

    j = 1;

    U1 = ubicacio(i, j, k);
    cout << "       U1 = (2, 1, 0)" << endl;
    cout << "       U1 < U4: " << (U1 < U4) << endl;

    k = 1;
    
    U1 = ubicacio(i, j, k);
    cout << "       U1 = (2, 1, 1)" << endl;
    cout << "       U1 < U4: " << (U1 < U4) << endl;

    j = 0;

    U1 = ubicacio(i, j, k);
    cout << "       U1 = (2, 0, 1)" << endl;
    cout << "       U1 < U4: " << (U1 < U4) << endl;

    j = 3;

    U1 = ubicacio(i, j, k);
    cout << "       U1 = (2, 3, 1)" << endl;
    cout << "       U1 < U4: " << (U1 < U4) << endl;

    cout << "\n   Operador <=\n" << endl;

    cout << "       U4 = (2, 1, 1)" << endl;
    i = 1;
    j = 1;
    U1 = ubicacio(i, j, k);
    cout << "       U1 = (1, 1, 1)" << endl;
    cout << "       U1 <= U4: " << (U1 <= U4) << endl;

    i = 0;
    U1 = ubicacio(i, j, k);
    cout << "       U1 = (0, 1, 1)" << endl;
    cout << "       U1 <= U4: " << (U1 <= U4) << endl;

    i = 2; 
    j = 3;
    k = 0;
    U1 = ubicacio(i, j, k);
    cout << "       U1 = (2, 3, 0)" << endl;
    cout << "       U1 <= U4: " << (U1 <= U4) << endl;

    cout << "\n   Operador >\n" << endl;

    cout << "       U4 = (2, 1, 1)" << endl;
    i = 2;
    j = 0;
    k = 0;

    U1 = ubicacio(i, j, k);
    cout << "       U1 = (2, 0, 0)" << endl;
    cout << "       U1 > U4: " << (U1 > U4) << endl;

    j = 1;

    U1 = ubicacio(i, j, k);
    cout << "       U1 = (2, 1, 0)" << endl;
    cout << "       U1 > U4: " << (U1 > U4) << endl;

    k = 1;
    
    U1 = ubicacio(i, j, k);
    cout << "       U1 = (2, 1, 1)" << endl;
    cout << "       U1 > U4: " << (U1 > U4) << endl;

    j = 0;

    U1 = ubicacio(i, j, k);
    cout << "       U1 = (2, 0, 1)" << endl;
    cout << "       U1 > U4: " << (U1 > U4) << endl;

    j = 3;

    U1 = ubicacio(i, j, k);
    cout << "       U1 = (2, 3, 1)" << endl;
    cout << "       U1 > U4: " << (U1 > U4) << endl;

    cout << "\n   Operador >=\n" << endl;

    cout << "       U4 = (2, 1, 1)" << endl;
    i = 1;
    j = 1;
    k = 1;
    U1 = ubicacio(i, j, k);
    cout << "       U1 = (1, 1, 1)" << endl;
    cout << "       U1 >= U4: " << (U1 >= U4) << endl;

    i = 0;
    U1 = ubicacio(i, j, k);
    cout << "       U1 = (0, 1, 1)" << endl;
    cout << "       U1 >= U4: " << (U1 >= U4) << endl;

    i = 2; 
    j = 3;
    k = 0;
    U1 = ubicacio(i, j, k);
    cout << "       U1 = (2, 3, 0)" << endl;
    cout << "       U1 >= U4: " << (U1 >= U4) << endl;

    cout << "CLASE LISTA!!!!!" << endl;
}
