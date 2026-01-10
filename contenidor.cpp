#include "contenidor.hpp"

/*
  Pre:
    cert.

  Post:
    Crea un contenidor amb matrícula m i longitud l.
    - Genera error MatriculaIncorrecta si:
        * m és la cadena buida, o
        * algun caràcter de m no és lletra majúscula ('A'..'Z') ni dígit ('0'..'9').
    - Genera error LongitudIncorrecta si l ∉ {10, 20, 30}.
    Si no hi ha errors, el p.i. queda amb _matricula = m i _longitud = l.

  Cost:
    Temps: O(|m|) per validar la matrícula (recorregut complet en el pitjor cas).
    Espai: O(1) addicional (sense comptar la memòria interna gestionada per std::string).
*/
contenidor::contenidor(const string &m, nat l)
{
  //Validació de la matrícula
  if (m.empty())
  {
    throw error(MatriculaIncorrecta);
  }

  for (char c : m)
  {
    if (!((c >= 'A' and c <= 'Z') or (c >= '0' and c <= '9')))
    {
      throw error(MatriculaIncorrecta);
    }
  }

  //Validació de la longitud
  if (l != 10 and l != 20 and l != 30)
  {
    throw error(LongitudIncorrecta);
  }
  //Assignació dels atributs
  _matricula = m;
  _longitud = l;
}

/*
  Pre:
    cert.

  Post:
    El p.i. es crea com una còpia exacta de u (mateixa matrícula i longitud).

  Cost:
    Temps: O(|u._matricula|) per copiar la cadena.
    Espai: O(|u._matricula|) (espai per a la còpia de l'string).
*/
contenidor::contenidor(const contenidor &u)
{
  _matricula = u._matricula;
  _longitud = u._longitud;
}

/*
  Pre:
    cert.

  Post:
    El p.i. passa a tenir els mateixos valors que u.
    Retorna una referència al p.i.

  Cost:
    Temps: O(|u._matricula|) (còpia d'string).
    Espai: O(|u._matricula|) (pot implicar reallocació de la memòria interna de l'string).
*/
contenidor &contenidor::operator=(const contenidor &u)
{
  _matricula = u._matricula;
  _longitud = u._longitud;
  return *this;
}

/*
  Pre:
    cert.

  Post:
    Destrueix el p.i. i allibera els recursos interns (gestionats per std::string).

  Cost:
    Temps: O(1).
    Espai: O(1).
*/
contenidor::~contenidor() noexcept
{
}

/*
  Pre:
    cert.

  Post:
    Retorna la longitud del contenidor.

  Cost:
    Temps: O(1).
    Espai: O(1).
*/
nat contenidor::longitud() const noexcept
{
  return _longitud;
}

/*
  Pre:
    cert.

  Post:
    Retorna la matrícula del contenidor.
    Nota: com que es retorna per valor, es crea una còpia de l'string.

  Cost:
    Temps: O(|_matricula|).
    Espai: O(|_matricula|) (còpia del retorn).
*/
string contenidor::matricula() const noexcept
{
  return _matricula;
}

/*
  Pre:
    cert.

  Post:
    Retorna cert si la matrícula i la longitud del p.i. coincideixen amb les de c.

  Cost:
    Temps: O(min(|m1|, |m2|)) per comparar strings (pitjor cas O(|m|)).
    Espai: O(1).
*/
bool contenidor::operator==(const contenidor &c) const noexcept
{
  return (_matricula == c._matricula and _longitud == c._longitud);
}

/*
  Pre:
    cert.

  Post:
    Retorna cert si el p.i. i c són diferents.

  Cost:
    Temps: O(min(|m1|, |m2|)) (delegat a operator==).
    Espai: O(1).
*/
bool contenidor::operator!=(const contenidor &c) const noexcept
{
  return not(*this == c);
}

/*
  Pre:
    cert.

  Post:
    Defineix un ordre total segons l'enunciat:
      - primer es compara la matrícula en ordre alfabètic,
      - si les matrícules coincideixen, es compara la longitud.

    Retorna cert si el p.i. és estrictament menor que c segons aquest ordre.

  Cost:
    Temps: O(min(|m1|, |m2|)) per comparar matrícules + O(1) per longitud.
    Espai: O(1).
*/
bool contenidor::operator<(const contenidor &c) const noexcept
{
  bool resultat = false;
  if (_matricula != c._matricula) {
    resultat = (_matricula < c._matricula);
  } else {
    resultat = (_longitud < c._longitud);
  }
  return resultat;
}

/*
  Pre:
    cert.

  Post:
    Retorna cert si el p.i. és menor o igual que c segons l'ordre definit.

  Cost:
    Temps: O(min(|m1|, |m2|)) (fa servir operator< i operator==).
    Espai: O(1).
*/
bool contenidor::operator<=(const contenidor &c) const noexcept
{
  return (*this < c) or (*this == c);
}

/*
  Pre:
    cert.

  Post:
    Retorna cert si el p.i. és estrictament major que c segons l'ordre definit.

  Cost:
    Temps: O(min(|m1|, |m2|)) (delegat a operator<).
    Espai: O(1).
*/
bool contenidor::operator>(const contenidor &c) const noexcept
{
  return c < *this;
}

/*
  Pre:
    cert.

  Post:
    Retorna cert si el p.i. és major o igual que c segons l'ordre definit.

  Cost:
    Temps: O(min(|m1|, |m2|)) (fa servir operator> i operator==).
    Espai: O(1).
*/
bool contenidor::operator>=(const contenidor &c) const noexcept
{
  return (*this > c) or (*this == c);
}
