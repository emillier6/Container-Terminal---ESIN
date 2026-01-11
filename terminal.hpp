#ifndef _TERMINAL_HPP
#define _TERMINAL_HPP 
#include <list>
#include <string>
#include <esin/error>
#include <esin/util>

#include "cataleg.hpp"
#include "contenidor.hpp"
#include "ubicacio.hpp"

using std::string;
using std::list;
using util::nat;

class terminal {
  /*
    -----------------------------------------------------------------------------
    Classe terminal
    -----------------------------------------------------------------------------

    Un objecte de la classe terminal representa un terminal d’emmagatzematge de
    contenidors. Cada contenidor, identificat per una matrícula única, pot trobar-se:

      - al magatzem, en una ubicació tridimensional (filera, placa, pis), o bé
      - a l’àrea d’espera, pendent de recol·locació.

    El terminal gestiona:
      - la inserció i retirada de contenidors,
      - la recol·locació automàtica segons una estratègia (FIRST_FIT o LLIURE),
      - el recompte d’operacions de grua,
      - consultes sobre ubicació, fragmentació i estat de l’àrea d’espera.

    La representació interna es documenta al fitxer terminal.rep.
*/
public:
  // Alçada màxima permesa del magatzem (nombre de pisos)
  static constexpr nat HMAX = 7;
  
  // Estratègia de col·locació i recol·locació de contenidors
  enum class estrategia { FIRST_FIT, LLIURE };

  /*
    Constructora.
    Crea un terminal amb n fileres, m places per filera, h pisos i estratègia st.
  */
  terminal(nat n, nat m, nat h, estrategia st);

  /*
    Constructora per còpia.
    El nou terminal és independent de l'original.
  */
  terminal(const terminal& b);

  /*
    Operador d'assignació.
    Després de l'assignació, el p.i. conté el mateix estat que b.
  */
  terminal& operator=(const terminal& b);

  /*
    Destructora.
    Allibera tota la memòria dinàmica associada al terminal.
  */
  ~terminal() noexcept;

  /*
    insereix_contenidor(c)
    Insereix el contenidor c al terminal. Si no es pot col·locar al magatzem segons
    l'estratègia, s'afegeix a l'àrea d'espera.
  */
  void insereix_contenidor(const contenidor &c);

  /*
    retira_contenidor(m)
    Retira del terminal el contenidor amb matrícula m. Pot implicar moviments
    temporals cap a l'àrea d'espera i recol·locacions posteriors.
  */
  void retira_contenidor(const string &m);

  /*
    on(m)
    Retorna la ubicació del contenidor amb matrícula m:
      - ubicació de magatzem si està emmagatzemat,
      - ubicacio(-1,0,0) si és a l'àrea d'espera,
      - ubicacio(-1,-1,-1) si no existeix.
    Aquest mètode no genera errors.
  */
  ubicacio on(const string &m) const noexcept;

  /*
    longitud(m)
    Retorna la longitud del contenidor amb matrícula m.
  */
  nat longitud(const string &m) const;

  /*
    contenidor_ocupa(u, m)
    Escriu a m la matrícula del contenidor que ocupa la ubicació u del magatzem.
  */
  void contenidor_ocupa(const ubicacio &u, string &m) const;  

  /*
    fragmentacio()
    Retorna la fragmentació del magatzem segons la definició del projecte.
    Aquest mètode no genera errors.
  */
  nat fragmentacio() const noexcept;

  /*
    ops_grua()
    Retorna el nombre d'operacions de grua acumulades.
    Aquest mètode no genera errors.
  */
  nat ops_grua() const noexcept;

  /*
    area_espera(l)
    Escriu a l les matrícules dels contenidors presents a l'àrea d'espera, ordenades
    alfabèticament. Aquest mètode no genera errors.
  */
  void area_espera(list<string> &l) const noexcept;

  /*
    Consultors de dimensions i configuració.
  */
  nat num_fileres() const noexcept;
  nat num_places() const noexcept;
  nat num_pisos() const noexcept;
  estrategia quina_estrategia() const noexcept; 

  static constexpr int NumFileresIncorr = 40;
  static constexpr int NumPlacesIncorr  = 41;
  static constexpr int AlcadaMaxIncorr  = 42;
  static constexpr int EstrategiaIncorr = 43;
  static constexpr int MatriculaDuplicada  = 44;
  static constexpr int MatriculaInexistent = 45;
  static constexpr int UbicacioNoMagatzem  = 46;

private:
  #include "terminal.rep"
};
#endif
