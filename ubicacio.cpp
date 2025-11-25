#include "ubicacio.hpp"

#include <esin/error>
using namespace std;

ubicacio::ubicacio(int i, int j, int k){
    if((i >= 0 and j >= 0 and k >= 0) or fora_terminal(i, j, k) or a_espera(i, j, k)){
        _filera = i;
        _placa = j;
        _pis = k;
    }else{
        throw error(UbicacioIncorrecta, "ubicacio", "Ubicacio incorrecta.");
        
    }
}

ubicacio::ubicacio(const ubicacio& u){
    *this = u;
}

ubicacio& ubicacio::operator=(const ubicacio& u){
    _filera = u._filera;
    _placa = u._placa;
    _pis = u._pis;
    return *this;
}

ubicacio::~ubicacio() noexcept{

}

int ubicacio::filera() const noexcept{
    return _filera;
}

int ubicacio::placa() const noexcept{
    return _placa;
}

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
    return (_filera < u._filera or (_filera == u._filera and _placa < u._placa) or (_filera == u._filera and _placa == u._placa and _pis < u._pis));
}

bool ubicacio::operator<=(const ubicacio &u) const noexcept{
    return (_filera <= u._filera or (_filera == u._filera and _placa <= u._placa) or (_filera == u._filera and _placa == u._placa and _pis <= u._pis));
}

bool ubicacio::operator>(const ubicacio &u) const noexcept{
    return (_filera > u._filera or (_filera == u._filera and _placa > u._placa) or (_filera == u._filera and _placa == u._placa and _pis > u._pis));
}

bool ubicacio::operator>=(const ubicacio &u) const noexcept{
    return (_filera >= u._filera or (_filera == u._filera and _placa >= u._placa) or (_filera == u._filera and _placa == u._placa and _pis >= u._pis));
}

// Metodes privats

bool ubicacio::fora_terminal(int i, int j, int k){
    return (i == -1 and j == -1 and k == -1);
}
bool ubicacio::a_espera(int i, int j, int k){
    return (i == -1 and j == 0 and k == 0);
}
