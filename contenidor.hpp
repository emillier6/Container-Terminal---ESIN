#ifndef _CONTENIDOR_HPP
#define _CONTENIDOR_HPP

#include <string>
#include <esin/error>
#include <esin/util>

using std::string;
using util::nat;

class contenidor
{
public:
  /*
    Constructora.
    Pre: true
    Post: Crea un contenidor amb matrícula m i longitud l (en peus).
          Genera error amb codi MatriculaIncorrecta si m és buida o bé conté algun
          caràcter que no sigui lletra majúscula ('A'..'Z') o dígit ('0'..'9').
          Genera error amb codi LongitudIncorrecta si l no pertany a {10, 20, 30}.
    Cost: O(|m|) temps per validar la matrícula; O(1) espai addicional.
  */
  contenidor(const string &m, nat l);

  /*
    Constructora per còpia.
    Pre: true
    Post: El p.i. és una còpia exacta de u (mateixa matrícula i longitud).
    Cost: O(|u.matricula()|) temps (còpia d'string); O(|u.matricula()|) espai.
  */
  contenidor(const contenidor &u);

  /*
    Operador d'assignació.
    Pre: true
    Post: El p.i. passa a tenir la mateixa matrícula i longitud que u.
    Cost: O(|u.matricula()|) temps; O(|u.matricula()|) espai (pot implicar reallocació de l'string).
  */
  contenidor &operator=(const contenidor &u);

  /*
    Destructora.
    Pre: true
    Post: Elimina el contenidor.
    Cost: O(1).
  */
  ~contenidor() noexcept;

  /*
    Consultora de longitud.
    Pre: true
    Post: Retorna la longitud del contenidor (en peus).
    Cost: O(1).
  */
  nat longitud() const noexcept;
  
  /*
    Consultora de matrícula.
    Pre: true
    Post: Retorna la matrícula del contenidor.
    Nota: es retorna per valor segons l'especificació.
    Cost: O(|matricula|) temps i espai (còpia de l'string retornat).
  */
  string matricula() const noexcept;

  /*
    Operadors de comparació.
    Definició (segons enunciat):
     - == : cert ssi matrícula i longitud coincideixen.
     - <  : cert ssi la matrícula del p.i. és menor alfabèticament que la de c,
            o si matrícules iguals i la longitud del p.i. és menor que la de c.
     - La resta d'operadors són consistents amb <.
    Pre: true
    Post: Retornen el resultat de la comparació definida.
    Cost: O(min(|m1|,|m2|)) per comparar matrícules; O(1) addicional.
  */
  bool operator==(const contenidor &c) const noexcept;
  bool operator!=(const contenidor &c) const noexcept;
  bool operator<(const contenidor &c) const noexcept;
  bool operator<=(const contenidor &c) const noexcept;
  bool operator>(const contenidor &c) const noexcept;
  bool operator>=(const contenidor &c) const noexcept;

  static constexpr int MatriculaIncorrecta = 20;
  static constexpr int LongitudIncorrecta = 21;

private:
#include "contenidor.rep"
};
#endif
