#include <iostream>
#include "contenidor.hpp"
using namespace std;

int main(){

    string h = "HOLA";
    nat d = 10;

    string m = "ABZ1234";
    nat l = 10;
    contenidor c1 = contenidor(h, d);
    try{
        c1 = contenidor(m, l);
        cout << "Creando c1 con matricula: 'ABZ1234' y longitud 10" << endl;
    }catch(const error &er){
        cerr << er.codigo() << " " << er.modulo() << " " << er.mensaje() << endl;
    }

    //no se debería crear
    m = "ABC123Ç";
    l = 40;
    contenidor c2 = contenidor("HOLA", 10);
    try{
        c2 = contenidor(m, l);
        cout << "Creando c2 con matricula: 'ABC123Ç' y longitud 40" << endl;
    }catch(const error &er){
        cerr << er.codigo() << " " << er.modulo() << " " << er.mensaje() << endl;
    }

    //no se debería crear
    m = "ABC5678";
    l = 80;
    contenidor c3 = contenidor("HOLA", 10);
    try{
        c3 = contenidor(m, l);
        cout << "Creando c3 con matricula: 'ABC5678'  y longitud 80" << endl;
    }catch(const error &er){
        cerr << er.codigo() << " " << er.modulo() << " " << er.mensaje() << endl;
    }

    m = "DFG1234";
    l = 20;
    contenidor c4 = contenidor("HOLA", 10);
    try{
        c4 = contenidor(m, l);
        cout << "Creando c4 con matricula: 'DFG1234' y longitud 20" << endl;
    }catch(const error &er){
        cerr << er.codigo() << " " << er.modulo() << " " << er.mensaje() << endl;
    }

    m = "DFG5678";
    l = 20;
    contenidor c5 = contenidor("HOLA", 10);
    try{
        c5 = contenidor(m, l);
        cout << "Creando c5 con matricula: 'DFG5678' y longitud 20" << endl;
    }catch(const error &er){
        cerr << er.codigo() << " " << er.modulo() << " " << er.mensaje() << endl;
    }


    c3 = contenidor("1432AZB", 10);
    cout << "\nConstructor por copia\n" << endl;

    c2 = contenidor(c4);
    cout << "   contenidor c2 = contenidor(c4)" << endl;
    cout << "   c4= matricula: " << c4.matricula() << " longitud: " << c4.longitud() << endl;
    cout << "   c2= matricula: " << c2.matricula() << " longitud: " << c2.longitud() << endl;

    cout << "\nAsignación\n" << endl;

    c2 = c5;
    cout << "   c2 = c5" << endl;
    cout << "   c5= matricula: " << c5.matricula() << " longitud: " << c5.longitud() << endl;
    cout << "   c2= matricula: " << c2.matricula() << " longitud: " << c2.longitud() << endl;

    c2 = contenidor("DFG5678", 10);
    cout << "\n------------------------------------------" << endl;
    cout << "   c1= matricula: " << c1.matricula() << " longitud: " << c1.longitud() << endl;
    cout << "   c2= matricula: " << c2.matricula() << " longitud: " << c2.longitud() << endl;
    cout << "   c3= matricula: " << c3.matricula() << " longitud: " << c3.longitud() << endl;
    cout << "   c5= matricula: " << c5.matricula() << " longitud: " << c5.longitud() << endl;
    cout << "   c4= matricula: " << c4.matricula() << " longitud: " << c4.longitud() << endl;
    cout << "------------------------------------------" << endl;

    cout << "\nOperador ==\n" << endl;

    cout << "   c2 == c5: " << (c2 == c5) << endl;
    cout << "   c2 == c4: " << (c2 == c4) << endl;
    cout << "   c2 == c1: " << (c2 == c1) << endl;
    cout << "   c2 == c2: " << (c2 == c2) << endl;
    cout << "   c3 == c1: " << (c3 == c1) << endl;

    cout << "\nOperador !=\n" << endl;

    cout << "   c2 != c5: " << (c2 != c5) << endl;
    cout << "   c2 != c4: " << (c2 != c4) << endl;
    cout << "   c2 != c1: " << (c2 != c1) << endl;
    cout << "   c2 != c2: " << (c2 != c2) << endl;
    cout << "   c3 != c1: " << (c3 != c1) << endl;

    cout << "\nOperador <\n" << endl;

    cout << "   c2 < c1: " << (c2 < c1) << endl;
    cout << "   c2 < c5: " << (c2 < c5) << endl;
    cout << "   c4 < c5: " << (c4 < c5) << endl;
    cout << "   c5 < c5: " << (c5 < c5) << endl;
    cout << "   c3 < c1: " << (c3 < c1) << endl;
    
    cout << "\nOperador <=\n" << endl;

    cout << "   c2 <= c1: " << (c2 <= c1) << endl;
    cout << "   c2 <= c5: " << (c2 <= c5) << endl;
    cout << "   c4 <= c5: " << (c4 <= c5) << endl;
    cout << "   c5 <= c5: " << (c5 <= c5) << endl;
    cout << "   c3 <= c1: " << (c3 <= c1) << endl;
    
    cout << "\nOperador >\n" << endl;

    cout << "   c2 > c1: " << (c2 > c1) << endl;
    cout << "   c2 > c5: " << (c2 > c5) << endl;
    cout << "   c4 > c5: " << (c4 > c5) << endl;
    cout << "   c5 > c5: " << (c5 > c5) << endl;

    cout << "\nOperador >=\n" << endl;

    cout << "   c2 >= c1: " << (c2 >= c1) << endl;
    cout << "   c2 >= c5: " << (c2 >= c5) << endl;
    cout << "   c4 >= c5: " << (c4 >= c5) << endl;
    cout << "   c5 >= c5: " << (c5 >= c5) << endl;
    cout << "   c3 >= c1: " << (c3 >= c1) << endl;
    
}