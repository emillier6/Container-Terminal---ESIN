#include "terminal.hpp"

// Places que ocupa un contenidor (10->1, 20->2, 30->3)
static inline nat places_contenidor(const contenidor &c) noexcept
{
    return c.longitud() / 10;
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
    if (_n == 0)
        throw error(NumFileresIncorr, "terminal", "Numero de fileres incorrecte.");
    if (_m == 0)
        throw error(NumPlacesIncorr, "terminal", "Numero de places incorrecte.");
    if (_h == 0 || _h > HMAX)
        throw error(AlcadaMaxIncorr, "terminal", "Alcada maxima incorrecta.");

    if (!(_st == estrategia::FIRST_FIT || _st == estrategia::LLIURE))
        throw error(EstrategiaIncorr, "terminal", "Estrategia incorrecta.");

    const nat total = _n * _m * _h;
    _mag = new string[total];
    for (nat p = 0; p < total; ++p)
        _mag[p] = "";
}

terminal::terminal(const terminal &b)
    : _n(b._n), _m(b._m), _h(b._h), _st(b._st),
      _ops_grua(b._ops_grua),
      _mag(nullptr),
      _idx(b._idx),
      _espera(b._espera)
{
    const nat total = _n * _m * _h;
    _mag = new string[total];
    for (nat p = 0; p < total; ++p)
        _mag[p] = b._mag[p];
}

terminal &terminal::operator=(const terminal &b)
{
    if (this == &b)
        return *this;

    // Còpia profunda del magatzem (si new falla, l’enunciat permet no rollback en memòria)
    const nat total = b._n * b._m * b._h;
    string *new_mag = new string[total];
    for (nat p = 0; p < total; ++p)
        new_mag[p] = b._mag[p];

    delete[] _mag;
    _mag = new_mag;

    _n = b._n;
    _m = b._m;
    _h = b._h;
    _st = b._st;
    _ops_grua = b._ops_grua;
    _idx = b._idx;
    _espera = b._espera;

    return *this;
}

terminal::~terminal() noexcept
{
    delete[] _mag;
    _mag = nullptr;
}

// ============================================================
// Getters
// ============================================================

nat terminal::num_fileres() const noexcept { return _n; }
nat terminal::num_places() const noexcept { return _m; }
nat terminal::num_pisos() const noexcept { return _h; }
terminal::estrategia terminal::quina_estrategia() const noexcept { return _st; }
nat terminal::ops_grua() const noexcept { return _ops_grua; }

// ============================================================
// Consultes per matrícula
// ============================================================

ubicacio terminal::on(const string &m) const noexcept
{
    if (!_idx.existeix(m))
        return ubicacio(-1, -1, -1);
    info_cont info = _idx[m];
    if (info.en_espera)
        return ubicacio(-1, 0, 0);
    return info.u;
}

nat terminal::longitud(const string &m) const
{
    if (!_idx.existeix(m))
        throw error(MatriculaInexistent, "terminal", "Matricula inexistent.");
    return _idx[m].c.longitud();
}

// ============================================================
// Consulta per ubicació
// ============================================================

void terminal::contenidor_ocupa(const ubicacio &u, string &m) const
{
    const int i = u.filera();
    const int j = u.placa();
    const int k = u.pis();

    if (i < 0 || j < 0 || k < 0 ||
        i >= (int)_n || j >= (int)_m || k >= (int)_h)
    {
        throw error(UbicacioNoMagatzem, "terminal", "Ubicacio no pertany al magatzem.");
    }

    m = _mag[pos((nat)i, (nat)j, (nat)k)];
}

// ============================================================
// Àrea d’espera (retorna ordenada alfabèticament)
// ============================================================

void terminal::area_espera(list<string> &l) const noexcept
{
    l.clear();
    for (const contenidor &c : _espera)
        l.push_back(c.matricula());
    l.sort();
}

// ============================================================
// Helpers locals (com a lambdas dins dels mètodes)
// ============================================================
//
// NOTA: No podem definir helpers globals accedint a privats.
// Per això, estructurem insereix/retira amb blocs clarament separats
// i punts "switch (_st)" que després es poden completar per LLIURE.
// ============================================================

// ============================================================
// Inserció
// ============================================================

void terminal::insereix_contenidor(const contenidor &c)
{
    const string mat = c.matricula();

    // Error verificable abans de modificar res
    if (_idx.existeix(mat))
        throw error(MatriculaDuplicada, "terminal", "Matricula duplicada.");

    const nat L = places_contenidor(c);

    // Accés al magatzem
    auto buit = [&](nat i, nat j, nat k) noexcept -> bool
    {
        return _mag[pos(i, j, k)] == "";
    };

    auto te_suport = [&](nat i, nat j, nat k, nat len) noexcept -> bool
    {
        if (k == 0)
            return true;
        for (nat x = j; x < j + len; ++x)
            if (buit(i, x, k - 1))
                return false;
        return true;
    };

    auto pot_colocar = [&](nat i, nat j, nat k, nat len) noexcept -> bool
    {
        if (j + len > _m)
            return false;
        for (nat x = j; x < j + len; ++x)
            if (!buit(i, x, k))
                return false;
        return te_suport(i, j, k, len);
    };

    auto escriu = [&](const string &m, nat i, nat j, nat k, nat len)
    {
        for (nat x = j; x < j + len; ++x)
            _mag[pos(i, x, k)] = m;
    };

    // ----------------------------------------------------------
    // Col·locació segons estratègia
    // (LLIURE està preparat però encara delega a FIRST_FIT)
    // FIRST_FIT: recorregut i (filera) -> j (placa) -> k (pis)
    // ----------------------------------------------------------
    auto coloca_segun_estrategia = [&](const contenidor &cc, ubicacio &out_u) -> bool
    {
        switch (_st)
        {
        case estrategia::FIRST_FIT:
        {
            for (nat i = 0; i < _n; ++i)
            {
                for (nat j = 0; j < _m; ++j)
                {
                    for (nat k = 0; k < _h; ++k)
                    {
                        if (pot_colocar(i, j, k, places_contenidor(cc)))
                        {
                            escriu(cc.matricula(), i, j, k, places_contenidor(cc));
                            out_u = ubicacio((int)i, (int)j, (int)k);
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        case estrategia::LLIURE:
        {
            // TODO: implementar quan tinguem l’enunciat de LLIURE.
            // De moment, per no trencar execució, fem el mateix que FIRST_FIT.
            for (nat i = 0; i < _n; ++i)
            {
                for (nat j = 0; j < _m; ++j)
                {
                    for (nat k = 0; k < _h; ++k)
                    {
                        if (pot_colocar(i, j, k, places_contenidor(cc)))
                        {
                            escriu(cc.matricula(), i, j, k, places_contenidor(cc));
                            out_u = ubicacio((int)i, (int)j, (int)k);
                            return true;
                        }
                    }
                }
            }
            return false;
        }
        }
        return false;
    };

    // ----------------------------------------------------------
    // Recol·locació des de l'àrea d'espera segons estratègia
    // FIRST_FIT: LIFO -> els últims inserits s’intenten primer.
    // Es repeteix fins a fer una passada completa sense moure res.
    // ----------------------------------------------------------
    auto recoloca_des_espera = [&]()
    {
        switch (_st)
        {
        case estrategia::FIRST_FIT:
        {
            while (true)
            {
                // Busquem el primer "movible" en un recorregut LIFO:
                // recorrem la llista sencera i ens quedem amb l'últim que pot entrar.
                auto millor_it = _espera.end();
                ubicacio millor_u(0, 0, 0);

                for (auto it = _espera.begin(); it != _espera.end(); ++it)
                {
                    ubicacio utry(0, 0, 0);
                    // Intent "en sec": hem de comprovar si cap sense escriure.
                    // Fem una cerca FIRST_FIT per aquest contenidor i, si existeix, guardem que és candidat.
                    bool ok = false;
                    for (nat i = 0; i < _n && !ok; ++i)
                    {
                        for (nat j = 0; j < _m && !ok; ++j)
                        {
                            for (nat k = 0; k < _h && !ok; ++k)
                            {
                                if (pot_colocar(i, j, k, places_contenidor(*it)))
                                {
                                    ok = true;
                                    utry = ubicacio((int)i, (int)j, (int)k);
                                }
                            }
                        }
                    }
                    if (ok)
                    {
                        // com que volem LIFO, ens quedem amb l'últim que apareix al recorregut (it més endavant)
                        millor_it = it;
                        millor_u = utry;
                    }
                }

                if (millor_it == _espera.end())
                    break; // passada completa sense moure cap

                // Ara col·loquem realment el contenidor seleccionat (últim que podia entrar)
                const contenidor cc = *millor_it;
                const string mm = cc.matricula();
                const nat len = places_contenidor(cc);

                // Com que ja hem trobat un u vàlid, escrivim exactament allà
                escriu(mm, (nat)millor_u.filera(), (nat)millor_u.placa(), (nat)millor_u.pis(), len);

                _idx.assig(mm, info_cont(cc, false, millor_u));
                _espera.erase(millor_it);
                _ops_grua++; // espera -> magatzem
            }
            break;
        }

        case estrategia::LLIURE:
        {
            // TODO: quan tinguem LLIURE, canviar l’ordre/criteri de selecció a l’espera.
            // De moment, fem el mateix que FIRST_FIT (LIFO).
            // (codi idèntic; per simplicitat, reutilitzem el cas FIRST_FIT)
            while (true)
            {
                auto millor_it = _espera.end();
                ubicacio millor_u(0, 0, 0);

                for (auto it = _espera.begin(); it != _espera.end(); ++it)
                {
                    ubicacio utry(0, 0, 0);
                    bool ok = false;
                    for (nat i = 0; i < _n && !ok; ++i)
                    {
                        for (nat j = 0; j < _m && !ok; ++j)
                        {
                            for (nat k = 0; k < _h && !ok; ++k)
                            {
                                if (pot_colocar(i, j, k, places_contenidor(*it)))
                                {
                                    ok = true;
                                    utry = ubicacio((int)i, (int)j, (int)k);
                                }
                            }
                        }
                    }
                    if (ok)
                    {
                        millor_it = it;
                        millor_u = utry;
                    }
                }

                if (millor_it == _espera.end())
                    break;

                const contenidor cc = *millor_it;
                const string mm = cc.matricula();
                const nat len = places_contenidor(cc);

                escriu(mm, (nat)millor_u.filera(), (nat)millor_u.placa(), (nat)millor_u.pis(), len);

                _idx.assig(mm, info_cont(cc, false, millor_u));
                _espera.erase(millor_it);
                _ops_grua++;
            }
            break;
        }
        }
    };

    // Intentem inserir
    ubicacio u(0, 0, 0);
    bool posat = coloca_segun_estrategia(c, u);

    if (posat)
    {
        _idx.assig(mat, info_cont(c, false, u));
        _ops_grua++; // inserció directa a magatzem
        recoloca_des_espera();
    }
    else
    {
        // No hi ha forat: va a espera (no reorganitza magatzem)
        _espera.push_back(c);
        _idx.assig(mat, info_cont(c, true, ubicacio(-1, 0, 0)));
    }
}

// ============================================================
// Retirada
// ============================================================

void terminal::retira_contenidor(const string &m)
{
    // Error verificable abans de modificar res
    if (!_idx.existeix(m))
        throw error(MatriculaInexistent, "terminal", "Matricula inexistent.");

    info_cont target = _idx[m];

    // Si és a espera: eliminar immediatament (0 grua)
    if (target.en_espera)
    {
        for (auto it = _espera.begin(); it != _espera.end(); ++it)
        {
            if (it->matricula() == m)
            {
                _espera.erase(it);
                break;
            }
        }
        _idx.elimina(m);
        return;
    }

    // Helpers locals
    auto buit = [&](nat i, nat j, nat k) noexcept -> bool
    {
        return _mag[pos(i, j, k)] == "";
    };

    auto escriu = [&](const string &mm, nat i, nat j, nat k, nat len)
    {
        for (nat x = j; x < j + len; ++x)
            _mag[pos(i, x, k)] = mm;
    };

    auto buida_segment = [&](nat i, nat j, nat k, nat len)
    {
        for (nat x = j; x < j + len; ++x)
            _mag[pos(i, x, k)] = "";
    };

    auto te_suport = [&](nat i, nat j, nat k, nat len) noexcept -> bool
    {
        if (k == 0)
            return true;
        for (nat x = j; x < j + len; ++x)
            if (buit(i, x, k - 1))
                return false;
        return true;
    };

    auto pot_colocar = [&](nat i, nat j, nat k, nat len) noexcept -> bool
    {
        if (j + len > _m)
            return false;
        for (nat x = j; x < j + len; ++x)
            if (!buit(i, x, k))
                return false;
        return te_suport(i, j, k, len);
    };

    // Col·locació segons estratègia (mateix patró que a inserció)
    auto coloca_segun_estrategia = [&](const contenidor &cc, ubicacio &out_u) -> bool
    {
        switch (_st)
        {
        case estrategia::FIRST_FIT:
        {
            for (nat i = 0; i < _n; ++i)
            {
                for (nat j = 0; j < _m; ++j)
                {
                    for (nat k = 0; k < _h; ++k)
                    {
                        if (pot_colocar(i, j, k, places_contenidor(cc)))
                        {
                            escriu(cc.matricula(), i, j, k, places_contenidor(cc));
                            out_u = ubicacio((int)i, (int)j, (int)k);
                            return true;
                        }
                    }
                }
            }
            return false;
        }
        case estrategia::LLIURE:
        {
            // TODO: implementar quan tinguem LLIURE. De moment, igual que FIRST_FIT.
            for (nat i = 0; i < _n; ++i)
            {
                for (nat j = 0; j < _m; ++j)
                {
                    for (nat k = 0; k < _h; ++k)
                    {
                        if (pot_colocar(i, j, k, places_contenidor(cc)))
                        {
                            escriu(cc.matricula(), i, j, k, places_contenidor(cc));
                            out_u = ubicacio((int)i, (int)j, (int)k);
                            return true;
                        }
                    }
                }
            }
            return false;
        }
        }
        return false;
    };

    // Recol·locació (FIRST_FIT LIFO)
    auto recoloca_des_espera = [&]()
    {
        switch (_st)
        {
        case estrategia::FIRST_FIT:
        case estrategia::LLIURE:
        {
            // (LLIURE TODO: de moment igual)
            while (true)
            {
                auto millor_it = _espera.end();
                ubicacio millor_u(0, 0, 0);

                for (auto it = _espera.begin(); it != _espera.end(); ++it)
                {
                    ubicacio utry(0, 0, 0);
                    bool ok = false;
                    for (nat i = 0; i < _n && !ok; ++i)
                    {
                        for (nat j = 0; j < _m && !ok; ++j)
                        {
                            for (nat k = 0; k < _h && !ok; ++k)
                            {
                                if (pot_colocar(i, j, k, places_contenidor(*it)))
                                {
                                    ok = true;
                                    utry = ubicacio((int)i, (int)j, (int)k);
                                }
                            }
                        }
                    }
                    if (ok)
                    {
                        millor_it = it;
                        millor_u = utry;
                    }
                }

                if (millor_it == _espera.end())
                    break;

                const contenidor cc = *millor_it;
                const string mm = cc.matricula();
                const nat len = places_contenidor(cc);

                escriu(mm, (nat)millor_u.filera(), (nat)millor_u.placa(), (nat)millor_u.pis(), len);

                _idx.assig(mm, info_cont(cc, false, millor_u));
                _espera.erase(millor_it);
                _ops_grua++; // espera -> magatzem
            }
            break;
        }
        }
    };

    // ----------------------------------------------------------
    // PAS 1: Calculem l'índex “i” del target i el seu footprint.
    // ----------------------------------------------------------
    const nat i0 = (nat)target.u.filera();
    const nat j0 = (nat)target.u.placa();
    const nat k0 = (nat)target.u.pis();
    const nat Lt = places_contenidor(target.c);

    // ----------------------------------------------------------
    // PAS 2: Definim el conjunt de columnes "S" que hem de netejar,
    // amb clausura transitiva:
    // - comença amb les columnes del target
    // - si un contenidor està per sobre del target i toca una columna de S,
    //   llavors totes les seves columnes s'afegeixen a S (això captura el cas H sobre F).
    // ----------------------------------------------------------
    bool *S = new bool[_m];
    for (nat j = 0; j < _m; ++j)
        S[j] = false;
    for (nat j = j0; j < j0 + Lt; ++j)
        S[j] = true;

    // Llista de matrícules presents a la filera i0 (sense STL extra: list)
    list<string> mats;
    auto ja_hi_es = [&](const string &mm) -> bool
    {
        for (const string &x : mats)
            if (x == mm)
                return true;
        return false;
    };

    for (nat j = 0; j < _m; ++j)
    {
        for (nat k = 0; k < _h; ++k)
        {
            const string mm = _mag[pos(i0, j, k)];
            if (mm != "" && !ja_hi_es(mm))
                mats.push_back(mm);
        }
    }

    auto cont_toca_S = [&](const info_cont &ci) -> bool
    {
        const nat L = places_contenidor(ci.c);
        const nat bj = (nat)ci.u.placa();
        for (nat x = bj; x < bj + L; ++x)
            if (S[x])
                return true;
        return false;
    };

    auto afegeix_cols = [&](const info_cont &ci) -> bool
    {
        bool changed = false;
        const nat L = places_contenidor(ci.c);
        const nat bj = (nat)ci.u.placa();
        for (nat x = bj; x < bj + L; ++x)
        {
            if (!S[x])
            {
                S[x] = true;
                changed = true;
            }
        }
        return changed;
    };

    bool changed = true;
    while (changed)
    {
        changed = false;
        for (const string &mm : mats)
        {
            if (!_idx.existeix(mm))
                continue;
            info_cont ci = _idx[mm];
            if (ci.en_espera)
                continue;
            if ((nat)ci.u.filera() != i0)
                continue;
            if ((nat)ci.u.pis() <= k0)
                continue; // només "per sobre" del target
            if (cont_toca_S(ci))
            {
                if (afegeix_cols(ci))
                    changed = true;
            }
        }
    }

    // ----------------------------------------------------------
    // Helpers retirada: accessible i selecció segons estratègia
    // ----------------------------------------------------------
    auto accessible = [&](const info_cont &ci) noexcept -> bool
    {
        if (ci.en_espera)
            return false;
        if (ci.u.filera() < 0 || ci.u.placa() < 0 || ci.u.pis() < 0)
            return false;
        const nat bi = (nat)ci.u.filera();
        const nat bj = (nat)ci.u.placa();
        const nat bk = (nat)ci.u.pis();
        const nat L = places_contenidor(ci.c);

        for (nat x = bj; x < bj + L; ++x)
            for (nat z = bk + 1; z < _h; ++z)
                if (!buit(bi, x, z))
                    return false;
        return true;
    };

    auto toca_S = [&](const info_cont &ci) noexcept -> bool
    {
        const nat L = places_contenidor(ci.c);
        const nat bj = (nat)ci.u.placa();
        for (nat x = bj; x < bj + L; ++x)
            if (S[x])
                return true;
        return false;
    };

    // Tria contenidor a moure segons estratègia:
    // FIRST_FIT: accessible, toca S, i ub. mínima
    auto tria_a_moure = [&](info_cont &out_ci, string &out_mat) -> bool
    {
        bool found = false;
        info_cont best = target; // placeholder
        string bestm = "";

        switch (_st)
        {
        case estrategia::FIRST_FIT:
        {
            for (const string &mm : mats)
            {
                if (!_idx.existeix(mm))
                    continue;
                info_cont ci = _idx[mm];
                if (ci.en_espera)
                    continue;
                if ((nat)ci.u.filera() != i0)
                    continue;
                if ((nat)ci.u.pis() <= k0)
                    continue;
                if (!toca_S(ci))
                    continue;
                if (!accessible(ci))
                    continue;

                if (!found || ci.u < best.u)
                {
                    found = true;
                    best = ci;
                    bestm = mm;
                }
            }
            break;
        }

        case estrategia::LLIURE:
        {
            // TODO: definir criteri LLIURE.
            // De moment, fem el mateix que FIRST_FIT.
            for (const string &mm : mats)
            {
                if (!_idx.existeix(mm))
                    continue;
                info_cont ci = _idx[mm];
                if (ci.en_espera)
                    continue;
                if ((nat)ci.u.filera() != i0)
                    continue;
                if ((nat)ci.u.pis() <= k0)
                    continue;
                if (!toca_S(ci))
                    continue;
                if (!accessible(ci))
                    continue;

                if (!found || ci.u < best.u)
                {
                    found = true;
                    best = ci;
                    bestm = mm;
                }
            }
            break;
        }
        }

        if (found)
        {
            out_ci = best;
            out_mat = bestm;
        }
        return found;
    };

    // Hi ha algun contenidor “sobre” el target (en les seves columnes)?
    auto target_tapat = [&]() noexcept -> bool
    {
        for (nat x = j0; x < j0 + Lt; ++x)
            for (nat z = k0 + 1; z < _h; ++z)
                if (!buit(i0, x, z))
                    return true;
        return false;
    };

    // ----------------------------------------------------------
    // PAS 3: Mou contenidors necessaris a espera fins destapar el target
    // ----------------------------------------------------------
    while (target_tapat())
    {
        info_cont ci = target;
        string mm = "";
        bool ok = tria_a_moure(ci, mm);
        if (!ok)
            break; // estat inconsistent; en pràctica no hauria de passar

        const nat bj = (nat)ci.u.placa();
        const nat bk = (nat)ci.u.pis();
        const nat L = places_contenidor(ci.c);

        buida_segment(i0, bj, bk, L);
        _espera.push_back(ci.c);
        _idx.assig(mm, info_cont(ci.c, true, ubicacio(-1, 0, 0)));
        _ops_grua++; // magatzem -> espera
    }

    // ----------------------------------------------------------
    // PAS 4: Retira el target
    // ----------------------------------------------------------
    buida_segment(i0, j0, k0, Lt);
    _idx.elimina(m);
    _ops_grua++; // retirada directa del magatzem

    // ----------------------------------------------------------
    // PAS 5: Recol·loca de l’espera al magatzem segons estratègia
    // ----------------------------------------------------------
    recoloca_des_espera();

    delete[] S;
}

// ============================================================
// Fragmentació
// ============================================================

nat terminal::fragmentacio() const noexcept
{
    nat frag = 0;

    auto buit = [&](nat i, nat j, nat k) noexcept -> bool
    {
        return _mag[pos(i, j, k)] == "";
    };

    // Una plaça (i,j,k) és "usable per 10" si:
    //  - està buida
    //  - i té suport (k==0 o la plaça de sota no és buida)
    auto usable10 = [&](nat i, nat j, nat k) noexcept -> bool
    {
        if (!buit(i, j, k))
            return false;
        if (k == 0)
            return true;
        return !buit(i, j, k - 1);
    };

    for (nat i = 0; i < _n; ++i)
    {
        for (nat k = 0; k < _h; ++k)
        {
            nat j = 0;
            while (j < _m)
            {
                if (!usable10(i, j, k))
                {
                    ++j;
                    continue;
                }

                // Hem trobat l'inici d'un forat contigu (en places)
                nat start = j;
                while (j < _m && usable10(i, j, k))
                    ++j;
                nat len = j - start;

                // Fragmentació: només compten els forats de mida 1
                if (len == 1)
                    frag += 1;
            }
        }
    }

    return frag;
}