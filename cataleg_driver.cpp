#include <iostream>
#include <string>
#include "cataleg.hpp"

using namespace std;

void mostra_menu() {
    cout << "-----------------------------\n";
    cout << "DRIVER DE LA CLASSE CATALEG\n";
    cout << "Operacions disponibles:\n";
    cout << " 1 <clau> <valor>   : assig(clau, valor)\n";
    cout << " 2 <clau>           : elimina(clau)\n";
    cout << " 3 <clau>           : existeix(clau)\n";
    cout << " 4 <clau>           : operator[](clau)\n";
    cout << " 5                  : quants()\n";
    cout << " 0                  : sortir\n";
    cout << "-----------------------------\n";
}

int main() {
    cout << "Introdueix mida aproximada del catàleg: ";
    util::nat n;
    cin >> n;

    cataleg<int> C(n);   // Catàleg amb valors enters (p.exemple)
    mostra_menu();

    int op;
    while (cin >> op) {
        try {
            if (op == 0) break;

            else if (op == 1) {
                string k;
                int v;
                cin >> k >> v;
                C.assig(k, v);
                cout << "OK: Inserit/actualitzat (" << k << ", " << v << ")\n";
            }

            else if (op == 2) {
                string k;
                cin >> k;
                C.elimina(k);
                cout << "OK: Eliminada clau " << k << "\n";
            }

            else if (op == 3) {
                string k;
                cin >> k;
                cout << (C.existeix(k) ? "Existeix" : "No existeix") << endl;
            }

            else if (op == 4) {
                string k;
                cin >> k;
                cout << "Valor: " << C[k] << endl;
            }

            else if (op == 5) {
                cout << "Nombre d'elements: " << C.quants() << endl;
            }

            else {
                cout << "Opció desconeguda\n";
            }

        } catch (error &e) {
            cout << "ERROR: " << e << endl;
        }
        
        cout << endl;
        mostra_menu();
    }

    cout << "Final del driver.\n";
    return 0;
}
