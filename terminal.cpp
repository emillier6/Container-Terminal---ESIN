#include "terminal.hpp"

// ------------------------------------------------------------
// Places ocupades per un contenidor (10->1,20->2,30->3)
// ------------------------------------------------------------
static nat places_contenidor(const contenidor &c) noexcept {
  return c.longitud() / 10;
}

// ============================================================
// Helpers privats (sense lambdes)
// ============================================================

// ------------------------------------------------------------
// Funció auxiliar clàssica: comprova si x és dins la llista L
// ------------------------------------------------------------
static bool esta_a_llista(const list<string> &L, const string &x) noexcept
{
  bool trobat = false;
  list<string>::const_iterator it = L.begin();
  while (it != L.end() && !trobat)
  {
    if (*it == x) trobat = true;
    ++it;
  }
  return trobat;
}

bool terminal::es_buit(nat i, nat j, nat k) const noexcept {
  return _mag[pos(i, j, k)] == "";
}

bool terminal::te_suport(nat i, nat j, nat k, nat len) const noexcept {
  bool ok = true;
  if (k != 0)
  {
    nat x = j;
    while (x < j + len && ok)
    {
      if (es_buit(i, x, k - 1)) ok = false;
      ++x;
    }
  }
  return ok;
}

bool terminal::pot_colocar(nat i, nat j, nat k, nat len) const noexcept {
  bool ok = (j + len <= _m);
  if (ok)
  {
    nat x = j;
    while (x < j + len && ok)
    {
      if (!es_buit(i, x, k)) ok = false;
      ++x;
    }
  }
  if (ok) ok = te_suport(i, j, k, len);
  return ok;
}

// --- Helpers per LLIURE ---
// Una plaça és usable per un 10 si està buida i (k==0) o té suport
bool terminal::lloc_usable10(nat i, nat j, nat k) const noexcept
{
  bool ok = es_buit(i, j, k);
  if (ok)
  {
    if (k != 0) ok = !es_buit(i, j, k - 1);
  }
  return ok;
}

// BEST_FIT + penalitza deixar forats de mida 1
bool terminal::millor_ubicacio_lliure(nat len, ubicacio &u, nat &puntuacio) const noexcept
{
  bool trobat = false;

  nat millor_i = 0, millor_j = 0, millor_k = 0;
  nat millor_puntuacio = 0;

  for (nat i = 0; i < _n; ++i)
  {
    for (nat k = 0; k < _h; ++k)
    {
      nat j = 0;
      while (j < _m)
      {
        bool usable_inici = lloc_usable10(i, j, k);

        if (!usable_inici)
        {
          ++j;
        }
        else
        {
          // Segment usable [ini..fi]
          nat ini = j;
          while (j < _m && lloc_usable10(i, j, k)) ++j;
          nat fi = j - 1;

          nat mida_forat = fi - ini + 1;

          if (mida_forat >= len)
          {
            // Provar totes les posicions dins el forat
            for (nat p = ini; p + len - 1 <= fi; ++p)
            {
              bool ok_colocar = pot_colocar(i, p, k, len);
              if (ok_colocar)
              {
                nat sobrant_esq = p - ini;
                nat sobrant_dreta = fi - (p + len - 1);
                nat sobrant = sobrant_esq + sobrant_dreta;

                // Penalització forta si deixem un forat d'1
                nat penal = 0;
                if (sobrant_esq == 1) penal += 100;
                if (sobrant_dreta == 1) penal += 100;

                nat score = penal * 1000 + sobrant;

                bool millor = false;
                if (!trobat) millor = true;
                else if (score < millor_puntuacio) millor = true;
                else if (score == millor_puntuacio)
                {
                  // desempat simple i estable
                  if (i < millor_i) millor = true;
                  else if (i == millor_i && p < millor_j) millor = true;
                  else if (i == millor_i && p == millor_j && k < millor_k) millor = true;
                }

                if (millor)
                {
                  trobat = true;
                  millor_puntuacio = score;
                  millor_i = i; millor_j = p; millor_k = k;
                }
              }
            }
          }

        }
      }
    }
  }

  bool resultat = trobat;
  if (resultat)
  {
    u = ubicacio((int)millor_i, (int)millor_j, (int)millor_k);
    puntuacio = millor_puntuacio;
  }
  return resultat;
}

// ============================================================
// Constructors / assignació / destructora
// ============================================================

terminal::terminal(nat n, nat m, nat h, estrategia st)
  : _n(n), _m(m), _h(h), _st(st),
    _ops_grua(0),
    _mag(nullptr),
    _idx((n == 0 || m == 0 || h == 0) ? 1 : (2 * n * m * h + 1)),
    _espera()
{
  if (_n == 0) throw error(NumFileresIncorr);
  if (_m == 0) throw error(NumPlacesIncorr);
  if (_h == 0 || _h > HMAX) throw error(AlcadaMaxIncorr);

  if (!(_st == estrategia::FIRST_FIT || _st == estrategia::LLIURE))
    throw error(EstrategiaIncorr);

  nat total = _n * _m * _h;
  _mag = new string[total];

  nat p = 0;
  while (p < total) {
    _mag[p] = "";
    ++p;
  }
}

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

terminal::~terminal() noexcept {
  delete[] _mag;
  _mag = nullptr;
}

// ============================================================
// Consultors simples
// ============================================================

nat terminal::num_fileres() const noexcept { return _n; }
nat terminal::num_places() const noexcept { return _m; }
nat terminal::num_pisos() const noexcept { return _h; }
terminal::estrategia terminal::quina_estrategia() const noexcept { return _st; }
nat terminal::ops_grua() const noexcept { return _ops_grua; }

// ============================================================
// on / longitud
// ============================================================

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

nat terminal::longitud(const string &m) const {
  if (!_idx.existeix(m)) throw error(MatriculaInexistent);
  return _idx[m].c.longitud();
}

// ============================================================
// contenidor_ocupa
// ============================================================

void terminal::contenidor_ocupa(const ubicacio &u, string &m) const {
  int i = u.filera();
  int j = u.placa();
  int k = u.pis();

  if (i < 0 || j < 0 || k < 0 || i >= (int)_n || j >= (int)_m || k >= (int)_h)
    throw error(UbicacioNoMagatzem);

  m = _mag[pos((nat)i, (nat)j, (nat)k)];
}

// ============================================================
// area_espera (alfabètic creixent)
// ============================================================

void terminal::area_espera(list<string> &l) const noexcept {
  l.clear();
  for (list<contenidor>::const_iterator it = _espera.begin(); it != _espera.end(); ++it)
    l.push_back(it->matricula());
  l.sort();
}

// ============================================================
// INSERCIÓ (FIRST_FIT vs LLIURE)
// ============================================================

void terminal::insereix_contenidor(const contenidor &c) {
  string mat = c.matricula();
  nat len = places_contenidor(c);

  if (_idx.existeix(mat)) throw error(MatriculaDuplicada);

  bool col_locat = false;
  ubicacio u(0, 0, 0);

  // --- 1) Buscar lloc segons estratègia ---
  if (_st == estrategia::FIRST_FIT)
  {
    // Recorregut filera -> placa -> pis
    nat i = 0;
    while (i < _n && !col_locat) {
      nat j = 0;
      while (j < _m && !col_locat) {
        nat k = 0;
        while (k < _h && !col_locat) {
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

  // --- 2) Escriure al magatzem o posar a espera ---
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

    // --- 3) Recol·locació des de l’espera ---
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
          while (ii < _n && !ok) {
            nat jj = 0;
            while (jj < _m && !ok) {
              nat kk = 0;
              while (kk < _h && !ok) {
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
            if (!trobat || score < millor_score)
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

// ============================================================
// RETIRADA (moviment necessari igual; recol·locació depèn d’estratègia)
// ============================================================

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
    while (it != _espera.end() && !trobat_it)
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

  list<string> necessaris;

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
            if (!esta_a_llista(necessaris, mm))
            {
              necessaris.push_back(mm);
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
  while (!necessaris.empty() && segueix)
  {
    bool trobat = false;
    string millor_mat = "";
    nat millor_j = 0;
    nat millor_k = 0;

    for (list<string>::const_iterator it = necessaris.begin(); it != necessaris.end(); ++it)
    {
      const string &mm = *it;
      info_cont ci = _idx[mm];

      nat bj = (nat)ci.u.placa();
      nat bk = (nat)ci.u.pis();
      nat ll = places_contenidor(ci.c);

      bool accessible = true;
      for (nat x = bj; x < bj + ll && accessible; ++x)
      {
        for (nat z = bk + 1; z < _h && accessible; ++z)
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
          if (bj < millor_j || (bj == millor_j && bk < millor_k))
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

      bool esborra = false;
      list<string>::iterator it = necessaris.begin();
      while (it != necessaris.end() && !esborra)
      {
        if (*it == millor_mat) esborra = true;
        else ++it;
      }
      if (esborra) necessaris.erase(it);
    }
  }

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
        while (ii < _n && !ok)
        {
          nat jj = 0;
          while (jj < _m && !ok)
          {
            nat kk = 0;
            while (kk < _h && !ok)
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
          if (!trobat2 || score < millor_score2)
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

// ============================================================
// FRAGMENTACIÓ (correcta: segments usables màxims de longitud 1)
// ============================================================

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
        bool usable = buida && suport;

        if (!usable)
        {
          ++j;
        }
        else
        {
          nat inici = j;

          bool segueix = true;
          while (j < _m && segueix)
          {
            bool buida2 = (_mag[pos(i, j, k)] == "");
            bool suport2 = (k == 0) ? true : (_mag[pos(i, j, k - 1)] != "");
            if (buida2 && suport2) ++j;
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
