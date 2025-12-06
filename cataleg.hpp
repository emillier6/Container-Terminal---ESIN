#ifndef _CATALEG_HPP
#define _CATALEG_HPP 
#include <string>
#include <esin/error>
#include <esin/util>

using std::string;
using util::nat;

template <typename Valor>
class cataleg {
public:
  // Constructora. Crea un catàleg amb capacitat per a `numelems` claus.
  // Pre: true
  // Post: crea una estructura buida amb capacitat per `numelems` elements
  explicit cataleg(nat numelems);

  // Constructora per còpia.
  // Pre: true
  // Post: el catàleg creat és una còpia exacta de `c` (deep copy)
  cataleg(const cataleg& c);

  // Assignació.
  // Pre: true
  // Post: el catàleg actual conté una còpia exacta de `c` i retorna referència a si mateix
  cataleg& operator=(const cataleg& c);

  // Destructora.
  // Pre: true
  // Post: allibera tots els recursos associats al catàleg
  ~cataleg() noexcept;

  // Insereix o actualitza el valor associat a la clau `k`.
  // Pre: true (la clau pot ser buida — en aquest cas es llença error `ClauStringBuit`)
  // Post: si `k` és nova, l'element s'afegeix; si ja existia, el seu valor s'actualitza
  void assig(const string &k, const Valor &v);

  // Elimina la clau `k` del catàleg.
  // Pre: true (si `k` és buida es llença `ClauStringBuit`)
  // Post: si `k` existia, queda eliminada; si no existia, es llença `ClauInexistent`
  void elimina(const string &k);

  // Comprova si existeix la clau `k`.
  // Pre: true
  // Post: retorna `true` si existeix una associació per `k`, `false` en cas contrari
  bool existeix(const string &k) const noexcept;

  // Accés al valor associat a `k` (versió const).
  // Pre: true (si `k` és buida es llença `ClauStringBuit`)
  // Post: retorna el Valor associat a `k` si existeix; si no existeix es llença `ClauInexistent`
  Valor operator[](const string &k) const;

  // Nombre d'elements emmagatzemats al catàleg.
  // Pre: true
  // Post: retorna el nombre d'associacions (clau, valor) actuals
  nat quants() const noexcept;

  static constexpr int ClauStringBuit = 30;
  static constexpr int ClauInexistent = 31;

private:
  #include "cataleg.rep"
};
#include "cataleg.t"
#endif
