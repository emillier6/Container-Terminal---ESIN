#ifndef _UBICACIO_HPP
#define _UBICACIO_HPP 

#include <esin/error>
#include <esin/util>

class ubicacio {
public:
  // Constructora. Crea la ubicació <i, j, k>. Genera un error
  // amb codi UbicacioIncorrecta si < i, j, k > no pertany a
  // {< u, v, w > |u ≥ 0 ∨ v ≥ 0 ∨ w ≥ 0} o a {< −1, 0, 0 >, < −1, −1, −1 >}.
  ubicacio(int i, int j, int k);

  // Constructora por copia. Crea una ubicacio <_filera, _placa, _pis> con los valores _filera, _placa y _pis
  // de la ubicacio u respectivamente
  ubicacio(const ubicacio& u);

  // Asignación. Modifica la ubicacio del p.i para que tenga los mismo valores _filera, _placa 
  // y _pis la ubicacio u
  ubicacio& operator=(const ubicacio& u);

  // Destructora.
  ~ubicacio() noexcept;

  // Pre: true
  // Post: devuelve la filera de la ubicacio del p.i
  int filera() const noexcept;

  // Pre: true
  // Post: devuelve la placa de la ubicacio del p.i
  int placa() const noexcept;

  // Pre: true
  // Post: devuelve el pis de la ubicacio del p.i
  int pis() const noexcept;

  // Pre: la ubicacio del p.i y la ubicacio u están el la terminal
  // Post: devuelve true si <_filera, _placa, _pis> de la ubicacio del p.i y u son iguales
  bool operator==(const ubicacio &u) const noexcept;

  // Pre: la ubicacio del p.i y la ubicacio u están el la terminal
  // Post: devuelve true si <_filera, _placa, _pis> de la ubicacio del p.i y u son diferentes
  bool operator!=(const ubicacio &u) const noexcept;

  // Pre: la ubicacio del p.i y la ubicacio u están el la terminal
  // Post: devuelve true si _filera del p.i es menor que _filera de u.
  // Si las _filera son iguales, devuelve true si _placa del p.i es menor que _placa de u.
  // Si las _filera y placa son iguales, devuelve true si _pis del p.i es menor que _placa de u.
  bool operator<(const ubicacio &u) const noexcept;

  // Pre: la ubicacio del p.i y la ubicacio u están el la terminal
  // Post: devuelve true si _filera del p.i es menor o igual que _filera de u.
  bool operator<=(const ubicacio &u) const noexcept;

  // Pre: la ubicacio del p.i y la ubicacio u están el la terminal
  // Post: devuelve true si _filera del p.i es mayor que _filera de u.
  // Si las _filera son iguales, devuelve true si _placa del p.i es mayor que _placa de u.
  // Si las _filera y placa son iguales, devuelve true si _pis del p.i es mayor que _placa de u.
  bool operator>(const ubicacio &u) const noexcept;

  // Pre: la ubicacio del p.i y la ubicacio u están el la terminal.
  // Post: devuelve true si _filera del p.i es mayor o igual que _filera de u.
  bool operator>=(const ubicacio &u) const noexcept;

  static constexpr int UbicacioIncorrecta = 10;

private:
  #include "ubicacio.rep"
};
#endif
