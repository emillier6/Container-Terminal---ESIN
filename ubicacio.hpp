#ifndef _UBICACIO_HPP
#define _UBICACIO_HPP 

#include <esin/error>
#include <esin/util>

class ubicacio {
/*
  -----------------------------------------------------------------------------
  Classe ubicacio
  -----------------------------------------------------------------------------
  
  Ofereix una abstracció d'una ubicació dins la terminal de contenidors.
  Les operacions permeten:
    - crear ubicacions vàlides,
    - consultar-ne els components,
    - comparar ubicacions.
  Totes les operacions tenen cost constant en temps i espai.
*/
public:
  /*
    Constructora.
    Crea una ubicació amb valors <i, j, k>.

    La ubicació creada pot correspondre a:
      - una ubicació de l'àrea d'emmagatzematge (i, j, k >= 0),
      - l'àrea d'espera (<-1, 0, 0>),
      - una ubicació inexistent (<-1, -1, -1>).

    Genera un error amb codi UbicacioIncorrecta si els valors no
    corresponen a cap ubicació vàlida segons l'enunciat.
  */
  ubicacio(int i, int j, int k);

  /*
    Constructora per còpia.
    Crea una nova ubicació amb els mateixos valors que la ubicació u.
  */
  ubicacio(const ubicacio& u);

  /*
    Operador d'assignació.
    Modifica el p.i. perquè tingui els mateixos valors que la ubicació u.
  */
  ubicacio& operator=(const ubicacio& u);

  /*
    Destructora.
  */
  ~ubicacio() noexcept;

  /*
    Retorna el número de filera de la ubicació.
  */
  int filera() const noexcept;

  /*
    Retorna el número de plaça de la ubicació.
  */
  int placa() const noexcept;

  /*
    Retorna el número de pis de la ubicació.
  */
  int pis() const noexcept;

  /*
    Retorna cert si la ubicació del p.i. i la ubicació u són iguals,
    és a dir, si tenen els mateixos valors de filera, plaça i pis.
  */
  bool operator==(const ubicacio &u) const noexcept;

  /*
    Retorna cert si la ubicació del p.i. i la ubicació u són diferents.
  */
  bool operator!=(const ubicacio &u) const noexcept;

  /*
    Retorna cert si la ubicació del p.i. és estrictament menor que u
    segons l'ordre definit a l'enunciat:
      - primer per filera,
      - després per plaça,
      - finalment per pis.
  */
  bool operator<(const ubicacio &u) const noexcept;

  /*
    Retorna cert si la ubicació del p.i. és menor o igual que u
    segons l'ordre lexicogràfic.
  */
  bool operator<=(const ubicacio &u) const noexcept;

  /*
    Retorna cert si la ubicació del p.i. és estrictament major que u
    segons l'ordre lexicogràfic.
  */
  bool operator>(const ubicacio &u) const noexcept;

  /*
    Retorna cert si la ubicació del p.i. és major o igual que u
    segons l'ordre lexicogràfic.
  */
  bool operator>=(const ubicacio &u) const noexcept;

  static constexpr int UbicacioIncorrecta = 10;

private:
  #include "ubicacio.rep"
};
#endif
