#include "terminal.hpp"

/*
    ------------------------------
    places_contenidor(c)
    ------------------------------
    Retorna el nombre de places que ocupa el contenidor c:
      longitud 10 -> 1, 20 -> 2, 30 -> 3.
    Cost: O(1)
*/
static nat places_contenidor(const contenidor &c) noexcept {
  return c.longitud() / 10;
}

/*
    ------------------------------
    Constructor
    ------------------------------
    Pre: true
    Post:
      - Crea un terminal buit de dimensions n x m x h amb estratègia st.
      - Inicialitza el magatzem amb totes les cel·les buides (matrícula "").
      - Inicialitza _ops_grua = 0 i l'àrea d'espera buida.
    Errors:
      - NumFileresIncorr si n == 0
      - NumPlacesIncorr si m == 0
      - AlcadaMaxIncorr si h == 0 o h > HMAX
      - EstrategiaIncorr si st no és una estratègia vàlida
    Cost:
      - Temps: O(T) per inicialitzar el magatzem
      - Espai: O(T) per _mag + espai per l'índex (_idx) i l'àrea d'espera
*/
terminal::terminal(nat n, nat m, nat h, estrategia st)
  : _n(n), _m(m), _h(h), _st(st),
    _ops_grua(0),
    _mag(nullptr),
    _idx((n == 0 or m == 0 or h == 0) ? 1 : (2 * n * m * h + 1)),
    _espera()
{
  if (_n == 0) throw error(NumFileresIncorr);
  if (_m == 0) throw error(NumPlacesIncorr);
  if (_h == 0 or _h > HMAX) throw error(AlcadaMaxIncorr);

  if (!(_st == estrategia::FIRST_FIT or _st == estrategia::LLIURE))
    throw error(EstrategiaIncorr);

  nat total = _n * _m * _h;
  _mag = new string[total];

  nat p = 0;
  while (p < total) {
    _mag[p] = "";
    ++p;
  }
}

/*
    ------------------------------
    Constructor per còpia
    ------------------------------
    Pre: true
    Post:
      - this és una còpia independent de b (còpia de _mag).
      - _idx i _espera també queden copiats (segons les seves operacions de còpia).
    Cost:
      - Temps: O(T) per copiar _mag + cost de copiar _idx + cost de copiar _espera
      - Espai: O(T) (nova memòria per _mag) + memòria per les còpies internes
*/
terminal::terminal(const terminal &b)
  : _n(b._n), _m(b._m), _h(b._h), _st(b._st),
    _ops_grua(b._ops_grua),
    _mag(nullptr),
    _idx(b._idx),
    _espera(b._espera)
{
  nat total = _n * _m * _h;
  _mag = new string[total];

  nat p = 0;
  while (p < total) {
    _mag[p] = b._mag[p];
    ++p;
  }
}

/*
    ------------------------------
    operator=
    ------------------------------
    Pre: true
    Post:
      - this passa a contenir el mateix estat que b (còpia de _mag).
      - En cas d'autoassignació, no es modifica res.
    Cost:
      - Temps: O(T) per copiar _mag + cost d'assignar _idx + cost d'assignar _espera
      - Espai addicional: O(T) temporal per nou_mag (després alliberat)
*/
terminal &terminal::operator=(const terminal &b) {
  bool mateix = (this == &b);

  if (!mateix)
  {
    nat total = b._n * b._m * b._h;
    string *nou_mag = new string[total];

    nat p = 0;
    while (p < total) {
      nou_mag[p] = b._mag[p];
      ++p;
    }

    delete[] _mag;
    _mag = nou_mag;

    _n = b._n;
    _m = b._m;
    _h = b._h;
    _st = b._st;
    _ops_grua = b._ops_grua;
    _idx = b._idx;
    _espera = b._espera;
  }

  return *this;
}

/*
    ------------------------------
    Destructor
    ------------------------------
    Pre: true
    Post: allibera la memòria dinàmica associada al magatzem.
    Cost:
      - Temps: O(1) (alliberament de l'array)
      - Espai: O(1) addicional
*/
terminal::~terminal() noexcept {
  delete[] _mag;
  _mag = nullptr;
}

/*
    Consultors simples de dimensions/configuració/comptadors.
    Cost: O(1)
*/
nat terminal::num_fileres() const noexcept { return _n; }
nat terminal::num_places() const noexcept { return _m; }
nat terminal::num_pisos() const noexcept { return _h; }
terminal::estrategia terminal::quina_estrategia() const noexcept { return _st; }
nat terminal::ops_grua() const noexcept { return _ops_grua; }

/*
    ------------------------------
    on(m)
    ------------------------------
    Post:
      - Si m no existeix: retorna ubicacio(-1,-1,-1).
      - Si m existeix i és a l'espera: retorna ubicacio(-1,0,0).
      - Si m existeix i és al magatzem: retorna la ubicació guardada.
    Cost:
      - Temps esperat: O(1) via _idx
      - Pitjor cas: O(k) per col·lisions a _idx
*/
ubicacio terminal::on(const string &m) const noexcept {
  ubicacio u(-1, -1, -1);
  if (_idx.existeix(m))
  {
    info_cont info = _idx[m];
    if (info.en_espera) u = ubicacio(-1, 0, 0);
    else u = info.u;
  }
  return u;
}

/*
    ------------------------------
    longitud(m)
    ------------------------------
    Post: retorna la longitud del contenidor amb matrícula m.
    Errors:
      - MatriculaInexistent si m no existeix
    Cost:
      - Temps esperat: O(1) via _idx
      - Pitjor cas: O(k)
*/
nat terminal::longitud(const string &m) const {
  if (!_idx.existeix(m)) throw error(MatriculaInexistent);
  return _idx[m].c.longitud();
}

/*
    ------------------------------
    contenidor_ocupa(u, m)
    ------------------------------
    Post:
      - Escriu a m la matrícula emmagatzemada a la cel·la u del magatzem.
    Errors:
      - UbicacioNoMagatzem si u no és una ubicació vàlida del magatzem
    Cost:
      - Temps: O(1)
      - Espai: O(1)
*/
void terminal::contenidor_ocupa(const ubicacio &u, string &m) const {
  int i = u.filera();
  int j = u.placa();
  int k = u.pis();

  if (i < 0 or j < 0 or k < 0 or i >= (int)_n or j >= (int)_m or k >= (int)_h)
    throw error(UbicacioNoMagatzem);

  m = _mag[pos((nat)i, (nat)j, (nat)k)];
}

/*
    ------------------------------
    area_espera(l)
    ------------------------------
    Post:
      - Escriu a l totes les matrícules dels contenidors a l'àrea d'espera,
        ordenades alfabèticament creixent.
    Cost:
      - Temps: O(e) per construir l + cost de l.sort().
        El cost de sort de list és O(e log e).
      - Espai: O(1) addicional (la llista l és sortida; l.sort és in-place)
*/
void terminal::area_espera(list<string> &l) const noexcept {
  l.clear();
  for (list<contenidor>::const_iterator it = _espera.begin(); it != _espera.end(); ++it)
    l.push_back(it->matricula());
  l.sort();
}

/*
    ------------------------------
    insereix_contenidor(c)
    ------------------------------
    Post:
      - Si la matrícula de c ja existeix: error(MatriculaDuplicada).
      - Si es troba ubicació al magatzem segons l'estratègia:
          * s'escriu la matrícula a len cel·les consecutives,
          * s'actualitza _idx i s'incrementa _ops_grua (1 operació).
          * després s'intenta recol·locar contenidors de l'àrea d'espera.
      - Si no hi ha ubicació:
          * s'afegeix c a l'àrea d'espera i s'actualitza _idx (sense incrementar _ops_grua).
    Cost:
      Part A) Comprovació duplicat (_idx.existeix):
        - Temps esperat O(1), pitjor O(k).

      Part B) Cerca d'ubicació:
        - Estratègia FIRST_FIT:
            Pitjor cas: O(T * len) ≃ O(T) (len<=3) per provar pot_colocar a totes les cel·les.
            Millor cas: O(len) si encaixa a la primera ubicació provada.
        - Estratègia LLIURE:
            Depèn de millor_ubicacio_lliure(len, ...).
            (Segons terminal.rep, pitjor cas O(n*h*m^2) amb len acotat.)

      Part C) Escriure al magatzem + actualitzar índex:
        - Temps: O(len) per escriure len cel·les + O(1) esperat per _idx.assig.

      Part D) Recol·locació des de l'espera:
        - S'executa mentre es pugui moure algun contenidor de l'espera al magatzem.
        - En cada iteració, es busca un contenidor recol·locable:
            * FIRST_FIT: per cada contenidor de l'espera, pot escanejar el magatzem (fins a T proves)
            * LLIURE: per cada contenidor de l'espera, crida millor_ubicacio_lliure(...)
        - Pitjor cas: pot ser elevat si es recol·loquen molts contenidors i cada pas escaneja molt.
          De forma conservadora:
            * FIRST_FIT: O(e * T) per iteració, i fins a e iteracions -> O(e^2 * T)
            * LLIURE: O(e * cost(millor_ubicacio_lliure)) per iteració, fins a e iteracions
*/
void terminal::insereix_contenidor(const contenidor &c) {
  string mat = c.matricula();
  nat len = places_contenidor(c);
  // --- 1) Comprovar duplicat ---
  if (_idx.existeix(mat)) throw error(MatriculaDuplicada);

  bool col_locat = false;
  ubicacio u(0, 0, 0);

  // --- 2) Buscar lloc segons estratègia ---
  if (_st == estrategia::FIRST_FIT)
  {
    // Recorregut filera -> placa -> pis
    nat i = 0;
    while (i < _n and !col_locat) {
      nat j = 0;
      while (j < _m and !col_locat) {
        nat k = 0;
        while (k < _h and !col_locat) {
          if (pot_colocar(i, j, k, len)) {
            col_locat = true;
            u = ubicacio((int)i, (int)j, (int)k);
          }
          ++k;
        }
        ++j;
      }
      ++i;
    }
  }
  else
  {
    nat score = 0;
    col_locat = millor_ubicacio_lliure(len, u, score);
  }

  // --- 3) Escriure al magatzem o posar a espera ---
  if (col_locat) {
    nat i0 = (nat)u.filera();
    nat j0 = (nat)u.placa();
    nat k0 = (nat)u.pis();

    nat x = j0;
    while (x < j0 + len) {
      _mag[pos(i0, x, k0)] = mat;
      ++x;
    }

    _idx.assig(mat, info_cont(c, false, u));
    _ops_grua++; // inserció directa al magatzem

    // --- 4) Recol·locació des de l’espera ---
    bool mogut = true;
    while (mogut) {
      mogut = false;

      list<contenidor>::iterator it_millor = _espera.end();
      ubicacio u_millor(0, 0, 0);

      if (_st == estrategia::FIRST_FIT)
      {
        // FIRST_FIT: LIFO => ens quedem l’últim que encaixa
        for (list<contenidor>::iterator it = _espera.begin(); it != _espera.end(); ++it) {
          bool ok = false;
          ubicacio utry(0, 0, 0);
          nat llen = places_contenidor(*it);

          nat ii = 0;
          while (ii < _n and !ok) {
            nat jj = 0;
            while (jj < _m and !ok) {
              nat kk = 0;
              while (kk < _h and !ok) {
                if (pot_colocar(ii, jj, kk, llen)) {
                  ok = true;
                  utry = ubicacio((int)ii, (int)jj, (int)kk);
                }
                ++kk;
              }
              ++jj;
            }
            ++ii;
          }

          if (ok) {
            it_millor = it;
            u_millor = utry;
          }
        }
      }
      else
      {
        // LLIURE: triem el contenidor de l’espera que encaixa millor (score mínim)
        bool trobat = false;
        nat millor_score = 0;

        for (list<contenidor>::iterator it = _espera.begin(); it != _espera.end(); ++it)
        {
          nat llen = places_contenidor(*it);
          ubicacio utry(0,0,0);
          nat score = 0;

          bool ok = millor_ubicacio_lliure(llen, utry, score);
          if (ok)
          {
            if (!trobat or score < millor_score)
            {
              trobat = true;
              millor_score = score;
              it_millor = it;
              u_millor = utry;
            }
          }
        }
      }

      if (it_millor != _espera.end()) {
        contenidor cc = *it_millor;
        string mm = cc.matricula();
        nat ll = places_contenidor(cc);

        nat ii = (nat)u_millor.filera();
        nat jj = (nat)u_millor.placa();
        nat kk = (nat)u_millor.pis();

        nat x2 = jj;
        while (x2 < jj + ll) {
          _mag[pos(ii, x2, kk)] = mm;
          ++x2;
        }

        _idx.assig(mm, info_cont(cc, false, u_millor));
        _espera.erase(it_millor);
        _ops_grua++; // espera -> magatzem
        mogut = true;
      }
    }

  } else {
    // A l’espera no compta com operació de grua
    _espera.push_back(c);
    _idx.assig(mat, info_cont(c, true, ubicacio(-1, 0, 0)));
  }
}

/*
    ------------------------------
    retira_contenidor(m)
    ------------------------------
    Post:
      - Si m no existeix: error(MatriculaInexistent).
      - Si m és a l'àrea d'espera: s'elimina de llista i de l'índex (0 operacions de grua).
      - Si m és al magatzem:
          1) Determina els contenidors que cal moure (els que estan damunt o interferint).
          2) Mou a l'àrea d'espera només els contenidors necessaris (incrementant _ops_grua).
          3) Allibera les cel·les ocupades per m, elimina m de l'índex i incrementa _ops_grua.
          4) Recol·loca contenidors des de l'espera segons l'estratègia (incrementant _ops_grua).

    Cost (cas magatzem):
      - Construcció de cal_netejar: O(m)
      - Determinació de contenidors necessaris:
          Escaneja pisos per sobre i pot repetir fins estabilitzar.
          Pitjor cas: O(h * m * h * m) no és una bona lectura; millor descriure-ho com:
          * En el pitjor cas pot revisar moltes cel·les del volum de la filera: O(m*h) per passada,
            i pot fer diverses passades fins no afegir nous contenidors.
          Una cota conservadora: O((m*h)^2) en el pitjor cas.

      - Mou a espera “necessaris”:
          Per cada contenidor candidat comprova accessibilitat mirant cel·les per sobre:
            cost O(len * (h - k)) <= O(h) per contenidor (len<=3).
          Amb fins a n_nec contenidors: O(n_nec * h) per selecció.
          A més, aquest procés pot repetir-se fins buidar necessaris: O(n_nec^2 * h) conservador.

      - Alliberar target: O(len) (len<=3)
      - Recol·locació des de l'espera: similar al cas de insereix_contenidor, en funció de e.
*/
void terminal::retira_contenidor(const string &m) {
  if (!_idx.existeix(m))
    throw error(MatriculaInexistent);

  info_cont target = _idx[m];

  // 0) Si és a l’àrea d’espera: eliminar immediatament (0 grua)
  bool es_a_espera = target.en_espera;
  if (es_a_espera)
  {
    bool trobat_it = false;
    list<contenidor>::iterator it = _espera.begin();
    while (it != _espera.end() and !trobat_it)
    {
      if (it->matricula() == m) trobat_it = true;
      else ++it;
    }
    if (trobat_it) _espera.erase(it);
    _idx.elimina(m);
  }

  // Target és al magatzem
  if (!es_a_espera)
  {
    nat fila = (nat)target.u.filera();
    nat placa = (nat)target.u.placa();
    nat pis = (nat)target.u.pis();
    nat len_target = places_contenidor(target.c);

    // 1) Determinar quins contenidors són necessaris moure
    bool *cal_netejar = new bool[_m];
    for (nat j = 0; j < _m; ++j)
      cal_netejar[j] = false;

    for (nat j = placa; j < placa + len_target; ++j)
      cal_netejar[j] = true;

    nat cap_nec = _m * _h; // màxim contenidors diferents a una fila
    string *necessaris = new string[cap_nec];
    nat n_nec = 0;

    bool afegit = true;
    while (afegit)
    {
      afegit = false;

      for (nat k = pis + 1; k < _h; ++k)
      {
        for (nat j = 0; j < _m; ++j)
        {
          if (cal_netejar[j])
          {
            const string &mm = _mag[pos(fila, j, k)];
            if (mm != "")
            {
              if (_idx.existeix(mm))
              {
                info_cont ci = _idx[mm];
                if (!ci.en_espera)
                {
                  bool ja_havia = false;
                  nat t = 0;
                  while (t < n_nec && !ja_havia) {
                    if (necessaris[t] == mm) ja_havia = true;
                    ++t;
                  }

                  if (!ja_havia)
                  {
                    if (n_nec < cap_nec) {
                      necessaris[n_nec] = mm;
                      ++n_nec;
                    }
                    afegit = true;

                    nat basej = (nat)ci.u.placa();
                    nat ll = places_contenidor(ci.c);

                    for (nat x = basej; x < basej + ll; ++x)
                      cal_netejar[x] = true;
                  }
                }
              }
            }
          }
        }
      }
    }

    // 2) Moure a espera NOMÉS els necessaris en ordre accessible + ubicació mínima
    bool segueix = true;
    while (n_nec > 0 and segueix)
    {
      bool trobat = false;
      string millor_mat = "";
      nat millor_j = 0;
      nat millor_k = 0;

      for (nat t = 0; t < n_nec; ++t)
      {
        const string &mm = necessaris[t];
        info_cont ci = _idx[mm];

        nat bj = (nat)ci.u.placa();
        nat bk = (nat)ci.u.pis();
        nat ll = places_contenidor(ci.c);

        bool accessible = true;
        for (nat x = bj; x < bj + ll and accessible; ++x)
        {
          for (nat z = bk + 1; z < _h and accessible; ++z)
          {
            if (!es_buit(fila, x, z)) accessible = false;
          }
        }

        if (accessible)
        {
          if (!trobat)
          {
            trobat = true;
            millor_mat = mm;
            millor_j = bj;
            millor_k = bk;
          }
          else
          {
            if (bj < millor_j or (bj == millor_j and bk < millor_k))
            {
              millor_mat = mm;
              millor_j = bj;
              millor_k = bk;
            }
          }
        }
      }

      if (!trobat) segueix = false;

      if (trobat)
      {
        info_cont ci = _idx[millor_mat];
        nat bj = (nat)ci.u.placa();
        nat bk = (nat)ci.u.pis();
        nat ll = places_contenidor(ci.c);

        for (nat x = bj; x < bj + ll; ++x)
          _mag[pos(fila, x, bk)] = "";

        _espera.push_back(ci.c);
        _idx.assig(millor_mat, info_cont(ci.c, true, ubicacio(-1, 0, 0)));
        _ops_grua++; // magatzem -> espera

        // eliminar millor_mat de l'array necessaris
        bool esborra = false;
        nat pos_del = 0;
        nat t = 0;

        while (t < n_nec and !esborra)
        {
          if (necessaris[t] == millor_mat)
          {
            esborra = true;
            pos_del = t;
          }
          ++t;
        }

        if (esborra)
        {
          for (nat u = pos_del + 1; u < n_nec; ++u)
            necessaris[u - 1] = necessaris[u];
          --n_nec;
        }
      }
    }

    delete[] necessaris;
    delete[] cal_netejar;

    // 3) Retirar el target del magatzem
    for (nat x = placa; x < placa + len_target; ++x)
      _mag[pos(fila, x, pis)] = "";

    _idx.elimina(m);
    _ops_grua++; // retirada directa del magatzem

    // 4) Recol·locar des de l’espera (depèn d’estratègia)
    bool mogut = true;
    while (mogut)
    {
      mogut = false;

      list<contenidor>::iterator millor_it = _espera.end();
      ubicacio millor_u(0, 0, 0);

      if (_st == estrategia::FIRST_FIT)
      {
        // FIRST_FIT: LIFO (últim que encaixa)
        for (list<contenidor>::iterator it = _espera.begin(); it != _espera.end(); ++it)
        {
          nat llen = places_contenidor(*it);

          bool ok = false;
          ubicacio utry(0, 0, 0);

          nat ii = 0;
          while (ii < _n and !ok)
          {
            nat jj = 0;
            while (jj < _m and !ok)
            {
              nat kk = 0;
              while (kk < _h and !ok)
              {
                if (pot_colocar(ii, jj, kk, llen))
                {
                  ok = true;
                  utry = ubicacio((int)ii, (int)jj, (int)kk);
                }
                ++kk;
              }
              ++jj;
            }
            ++ii;
          }

          if (ok)
          {
            millor_it = it;
            millor_u = utry;
          }
        }
      }
      else
      {
        // LLIURE: millor encaix (score mínim)
        bool trobat2 = false;
        nat millor_score2 = 0;

        for (list<contenidor>::iterator it = _espera.begin(); it != _espera.end(); ++it)
        {
          nat llen = places_contenidor(*it);
          ubicacio utry(0,0,0);
          nat score = 0;

          bool ok = millor_ubicacio_lliure(llen, utry, score);
          if (ok)
          {
            if (!trobat2 or score < millor_score2)
            {
              trobat2 = true;
              millor_score2 = score;
              millor_it = it;
              millor_u = utry;
            }
          }
        }
      }

      if (millor_it != _espera.end())
      {
        contenidor cc = *millor_it;
        string mm = cc.matricula();
        nat ll = places_contenidor(cc);

        nat ii = (nat)millor_u.filera();
        nat jj = (nat)millor_u.placa();
        nat kk = (nat)millor_u.pis();

        for (nat x = jj; x < jj + ll; ++x)
          _mag[pos(ii, x, kk)] = mm;

        _idx.assig(mm, info_cont(cc, false, millor_u));
        _espera.erase(millor_it);
        _ops_grua++; // espera -> magatzem
        mogut = true;
      }
    }
  }
}

/*
    ------------------------------
    fragmentacio()
    ------------------------------
    Descripció:
      Recorre el magatzem i compta els segments "usables" màxims de longitud 1,
      on una cel·la és usable si és buida i té suport (k==0 o cel·la inferior ocupada).
    Cost:
      - Temps: O(T) (cada cel·la s'examina un nombre constant de vegades)
      - Espai: O(1)
*/
nat terminal::fragmentacio() const noexcept {
  nat frag = 0;

  for (nat i = 0; i < _n; ++i)
  {
    for (nat k = 0; k < _h; ++k)
    {
      nat j = 0;

      while (j < _m)
      {
        bool buida = (_mag[pos(i, j, k)] == "");
        bool suport = (k == 0) ? true : (_mag[pos(i, j, k - 1)] != "");
        bool usable = buida and suport;

        if (!usable)
        {
          ++j;
        }
        else
        {
          nat inici = j;

          bool segueix = true;
          while (j < _m and segueix)
          {
            bool buida2 = (_mag[pos(i, j, k)] == "");
            bool suport2 = (k == 0) ? true : (_mag[pos(i, j, k - 1)] != "");
            if (buida2 and suport2) ++j;
            else segueix = false;
          }

          nat llarg = j - inici;
          if (llarg == 1) ++frag;
        }
      }
    }
  }

  return frag;
}
