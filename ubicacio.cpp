#include "ubicacio.hpp"

#include <esin/error>
using namespace std;
// Constructora. Crea la ubicació <i, j, k>. Genera un error
// amb codi UbicacioIncorrecta si < i, j, k > no pertany a
// {< u, v, w > |u ≥ 0 ∨ v ≥ 0 ∨ w ≥ 0} o a {< −1, 0, 0 >, < −1, −1, −1 >}.
ubicacio::ubicacio(int i, int j, int k){
    if((i >= 0 and j >= 0 and k >= 0) or fora_terminal(i, j, k) or a_espera(i, j, k)){
        _filera = i;
        _placa = j;
        _pis = k;
    }else{

        cout << error(UbicacioIncorrecta, "UbicacioIncorrecta", "ubicacio Ubicacio incorrecta.") << endl;

        // cout << "falta error" << endl;
    }
}
// Constructora per còpia.
ubicacio::ubicacio(const ubicacio& u){
    *this = u;
}
// Assignació.
ubicacio& ubicacio::operator=(const ubicacio& u){
    _filera = u._filera;
    _placa = u._placa;
    _pis = u._pis;
    return *this;
}
// Destructora.
ubicacio::~ubicacio() noexcept{

}


//pre: la ubicació del paràmetre implícit està en terminal i fora d'àrea d'espera
//post: retorna el primer element de la ubicació
int ubicacio::filera() const noexcept{
    return _filera;
}

//pre: la ubicació del paràmetre implícit està en terminal i fora d'àrea d'espera
//post: retorna el segon element de la ubicació
int ubicacio::placa() const noexcept{
    return _placa;
}

//pre: la ubicació del paràmetre implícit està en terminal i fora d'àrea d'espera
//post: retorna el tercer element de la ubicació
int ubicacio::pis() const noexcept{
    return _pis;
}

bool ubicacio::operator==(const ubicacio &u) const noexcept{
    return (_filera == u._filera and _placa == u._placa and _pis == u._pis);
}

bool ubicacio::operator!=(const ubicacio &u) const noexcept{
    return not(*this == u);
}

bool ubicacio::operator<(const ubicacio &u) const noexcept{
    return (_filera < u._filera or (_filera == u._filera and _placa == u._placa and _pis < u._pis));
}

bool ubicacio::operator<=(const ubicacio &u) const noexcept{
    return (_filera <= u._filera or (_filera == u._filera and _placa == u._placa and _pis <= u._pis));
}

bool ubicacio::operator>(const ubicacio &u) const noexcept{
    return not(*this <= u);
}

bool ubicacio::operator>=(const ubicacio &u) const noexcept{
    return not(*this < u);
}
// Metodes privats
bool ubicacio::fora_terminal(int i, int j, int k){
    return (i == -1 and j == -1 and k == -1);
}
bool ubicacio::a_espera(int i, int j, int k){
    return (i == -1 and j == 0 and k == 0);
}
