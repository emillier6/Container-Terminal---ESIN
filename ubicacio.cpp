#include "ubicacio.hpp"

#include <esin/error>

/*
  Pre:
    cert.

  Post:
    Si <i, j, k> identifica una ubicació vàlida segons l'enunciat:
      - i >= 0, j >= 0, k >= 0  (ubicació del magatzem), o
      - <-1, 0, 0>             (àrea d'espera), o
      - <-1, -1, -1>           (ubicació inexistent),
    el p.i. queda inicialitzat amb aquests valors.
    En cas contrari, es genera un error amb codi UbicacioIncorrecta.

  Cost:
    Temps: O(1)
    Espai: O(1)
*/
ubicacio::ubicacio(int i, int j, int k){
    if((i >= 0 and j >= 0 and k >= 0) or
      (i == -1 and j == 0 and k == 0) or
      (i == -1 and j == -1 and k == -1)){
        _filera = i;
        _placa = j;
        _pis = k;
    }else{
        throw error(UbicacioIncorrecta);
    }
}

/*
  Pre:
    cert.

  Post:
    El p.i. es crea com una còpia exacta de la ubicació u.

  Cost:
    Temps: O(1)
    Espai: O(1)
*/
ubicacio::ubicacio(const ubicacio& u){
    _filera = u._filera;
    _placa = u._placa;
    _pis = u._pis;
}

/*
  Pre:
    cert.

  Post:
    El p.i. passa a tenir els mateixos valors que la ubicació u.
    Retorna una referència al p.i.

  Cost:
    Temps: O(1)
    Espai: O(1)
*/
ubicacio& ubicacio::operator=(const ubicacio& u){
    if (this != &u) {
        _filera = u._filera;
        _placa = u._placa;
        _pis = u._pis;
    }
    return *this;
}

ubicacio::~ubicacio() noexcept{

}

/*
  Pre:
    cert.

  Post:
    Retorna la filera de la ubicació del p.i.

  Cost:
    Temps: O(1)
    Espai: O(1)
*/
int ubicacio::filera() const noexcept{
    return _filera;
}

/*
  Pre:
    cert.

  Post:
    Retorna la placa de la ubicació del p.i.

  Cost:
    Temps: O(1)
    Espai: O(1)
*/
int ubicacio::placa() const noexcept{
    return _placa;
}

/*
  Pre:
    cert.

  Post:
    Retorna el pis de la ubicació del p.i.

  Cost:
    Temps: O(1)
    Espai: O(1)
*/
int ubicacio::pis() const noexcept{
    return _pis;
}

/*
  Pre:
    La ubicació del p.i. i u són ubicacions vàlides.

  Post:
    Retorna cert si totes dues ubicacions són exactament iguals,
    és a dir, tenen els mateixos valors de filera, placa i pis.

  Cost:
    Temps: O(1)
    Espai: O(1)
*/
bool ubicacio::operator==(const ubicacio &u) const noexcept{
    return (_filera == u._filera and _placa == u._placa and _pis == u._pis);
}

/*
  Pre:
    La ubicació del p.i. i u són ubicacions vàlides.

  Post:
    Retorna cert si les ubicacions són diferents.

  Cost:
    Temps: O(1)
    Espai: O(1)
*/
bool ubicacio::operator!=(const ubicacio &u) const noexcept{
    return not(*this == u);
}

/*
  Pre:
    La ubicació del p.i. i u són ubicacions vàlides.

  Post:
    Retorna cert si la ubicació del p.i. és estrictament menor que u
    segons l'ordre definit a l'enunciat:
      - primer per filera,
      - després per placa,
      - finalment per pis.

  Cost:
    Temps: O(1)
    Espai: O(1)
*/
bool ubicacio::operator<(const ubicacio &u) const noexcept{
    bool resultat = false;
    if (_filera != u._filera) {
        resultat = (_filera < u._filera);
    } else if (_placa != u._placa) {
        resultat = (_placa < u._placa);
    } else {
        resultat = (_pis < u._pis);
    }
    return resultat;
}

/*
  Pre:
    La ubicació del p.i. i u són ubicacions vàlides.

  Post:
    Retorna cert si la ubicació del p.i. és menor o igual que u
    segons l'ordre de l'enunciat.

  Cost:
    Temps: O(1)
    Espai: O(1)
*/
bool ubicacio::operator<=(const ubicacio &u) const noexcept{
    return (*this < u) or (*this == u);
}

/*
  Pre:
    La ubicació del p.i. i u són ubicacions vàlides.

  Post:
    Retorna cert si la ubicació del p.i. és estrictament major que u
    segons l'ordre de l'enunciat.

  Cost:
    Temps: O(1)
    Espai: O(1)
*/
bool ubicacio::operator>(const ubicacio &u) const noexcept{
    return u < *this;
}

/*
  Pre:
    La ubicació del p.i. i u són ubicacions vàlides.

  Post:
    Retorna cert si la ubicació del p.i. és major o igual que u
    segons l'ordre de l'enunciat.

  Cost:
    Temps: O(1)
    Espai: O(1)
*/
bool ubicacio::operator>=(const ubicacio &u) const noexcept{
    return (*this > u) or (*this == u);
}
