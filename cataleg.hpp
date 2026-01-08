#ifndef _CATALEG_HPP
#define _CATALEG_HPP 
#include <string>
#include <esin/error>
#include <esin/util>

using std::string;
using util::nat;

template <typename Valor>
class cataleg {
  /*
    Classe template cataleg<Valor>

    Descripció:
    Diccionari (clau -> valor) on la clau és un string i el valor és de tipus Valor.
    Les operacions principals permeten inserir/actualitzar (assig), eliminar (elimina),
    consultar existència (existeix) i obtenir el valor associat (operator[]).

    Notes d'implementació:
      - La representació i la justificació de disseny es documenten a cataleg.rep.
      - La implementació i el contracte formal (Pre/Post i costos per mètode) es documenten a cataleg.t.
      - Com que és una classe template, la implementació està inclosa al fitxer cataleg.t.

    Gestió d'errors:
      - ClauStringBuit (30): s'utilitza quan una operació no admet la clau buida "".
      - ClauInexistent (31): s'utilitza quan es consulta/elimina una clau que no és al catàleg.
  */
public:
  /*
    Constructora.
    Crea un catàleg buit dimensionat a partir d'una estimació del nombre d'elements.
    El paràmetre numelems serveix per ajustar la mida inicial de l'estructura.
  */
  explicit cataleg(nat numelems);

  /*
    Constructora per còpia (còpia profunda).
    El nou catàleg és independent de l'original (no comparteix memòria interna).
  */
  cataleg(const cataleg& c);

  /*
    Operador d'assignació (còpia profunda).
    Després de l'assignació, el p.i. conté les mateixes associacions que c.
  */
  cataleg& operator=(const cataleg& c);

  /*
    Destructora.
    Allibera tota la memòria dinàmica associada al catàleg.
  */
  ~cataleg() noexcept;

  /*
    assig(k, v)
    Insereix l'associació (k -> v) al catàleg. Si k ja existia, actualitza el seu valor a v.

    Errors:
      - ClauStringBuit si k == "".
  */
  void assig(const string &k, const Valor &v);

  /*
    elimina(k)
    Elimina del catàleg l'associació corresponent a la clau k.

    Errors:
      - ClauInexistent si k no existeix al catàleg.
  */
  void elimina(const string &k);

  /*
    existeix(k)
    Retorna cert si la clau k existeix al catàleg, fals altrament.
    Aquest mètode no genera errors.
  */
  bool existeix(const string &k) const noexcept;

  /*
    operator[](k) (const)
    Retorna el valor associat a la clau k.

    Nota:
      - Retorna una còpia del valor (segons l'especificació).
    Errors:
      - ClauInexistent si k no existeix al catàleg.
  */
  Valor operator[](const string &k) const;

  /*
    quants()
    Retorna el nombre actual d'associacions (clau, valor) guardades al catàleg.
  */
  nat quants() const noexcept;

  static constexpr int ClauStringBuit = 30;
  static constexpr int ClauInexistent = 31;

private:
  #include "cataleg.rep"
};
#include "cataleg.t"
#endif
