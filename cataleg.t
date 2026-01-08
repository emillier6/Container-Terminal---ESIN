/*
    -----------------------------------------------------------------------------
    Implementació de cataleg<Valor> (taula hash amb chaining)
    -----------------------------------------------------------------------------

    Nota sobre costos:
      - n = nombre d’elements (_quants)
      - M = nombre de buckets (_M)
      - α = factor de càrrega = n / M

    Amb una funció hash “bona” i α controlat, el temps esperat de les operacions és O(1).
    En el pitjor cas (totes les claus al mateix bucket) és O(n).
*/

/*
    ------------------------------
    Constructor
    ------------------------------
    Pre: true
    Post: crea un catàleg buit amb _M = 2*numelems+1 (mínim 1) i totes les
          posicions inicialitzades a nullptr; _quants=0.
    Cost:
      - Temps: Θ(M)
      - Espai: Θ(M)
*/
template <typename Valor>
cataleg<Valor>::cataleg(nat numelems) {
    _M = 2 * numelems + 1;     // mida recomanada
    _taula = new node_hash*[_M];

    for (nat i = 0; i < _M; ++i)
        _taula[i] = nullptr;

    _quants = 0;
}

/*
    ------------------------------
    Constructor per còpia (deep copy)
    ------------------------------
    Pre: true
    Post: this és una còpia independent de c.
    Cost:
      - Temps: Θ(M + n)
      - Espai: Θ(M + n)
*/
template <typename Valor>
cataleg<Valor>::cataleg(const cataleg &c) {
    _M = c._M;
    _quants = c._quants;

    _taula = new node_hash*[_M];

    // copiar cada llista encadenada
    for (nat i = 0; i < _M; ++i) {
        node_hash *p = c._taula[i];
        node_hash *last = nullptr;

        if (p == nullptr) {
            _taula[i] = nullptr;
        } else {
            // primer node
            _taula[i] = new node_hash(p->_k, p->_v);
            last = _taula[i];
            p = p->_seg;

            // resta nodes
            while (p != nullptr) {
                last->_seg = new node_hash(p->_k, p->_v);
                last = last->_seg;
                p = p->_seg;
            }
        }
    }
}

/*
    ------------------------------
    Assignació
    ------------------------------
    Pre: true
    Post: this conté una còpia independent de c i retorna *this.
    Cost:
      - Temps: Θ(M + n) per destruir + Θ(M + n) per copiar
      - Espai addicional: O(1) (sense comptar la nova memòria reservada)
*/
template <typename Valor>
cataleg<Valor>& cataleg<Valor>::operator=(const cataleg &c) {
    if (this != &c) {
        // eliminar l’estructura actual
        for (nat i = 0; i < _M; ++i) {
            node_hash *p = _taula[i];
            while (p != nullptr) {
                node_hash *aux = p;
                p = p->_seg;
                delete aux;
            }
        }
        delete[] _taula;

        // copiar com al constructor per còpia
        _M = c._M;
        _quants = c._quants;

        _taula = new node_hash*[_M];

        for (nat i = 0; i < _M; ++i) {
            node_hash *p = c._taula[i];
            node_hash *last = nullptr;

            if (p == nullptr) {
                _taula[i] = nullptr;
            } else {
                _taula[i] = new node_hash(p->_k, p->_v);
                last = _taula[i];
                p = p->_seg;

                while (p != nullptr) {
                    last->_seg = new node_hash(p->_k, p->_v);
                    last = last->_seg;
                    p = p->_seg;
                }
            }
        }
    }
    return *this;
}

/*
    ------------------------------
    Destructor
    ------------------------------
    Pre: true
    Post: memòria alliberada.
    Cost:
      - Temps: Θ(M + n)
      - Espai: O(1) addicional
*/
template <typename Valor>
cataleg<Valor>::~cataleg() noexcept {
    for (nat i = 0; i < _M; ++i) {
        node_hash *p = _taula[i];
        while (p != nullptr) {
            node_hash *aux = p;
            p = p->_seg;
            delete aux;
        }
    }
    delete[] _taula;
}

/*
    ------------------------------
    assig: inserir/actualitzar
    ------------------------------
    Pre: true
    Post:
      - si k=="" -> error(ClauStringBuit)
      - si k existia -> actualitza valor
      - si k no existia -> insereix node nou i incrementa _quants
    Cost esperat:
      - Temps: O(1) mitjà; O(n) pitjor
      - Espai: O(1) per inserció (1 node)
*/
template <typename Valor>
void cataleg<Valor>::assig(const string &k, const Valor &v) {
    if (k == "") throw error(ClauStringBuit);

    nat pos = hash(k);

    node_hash *p = _taula[pos];

    // Buscar si ja existeix
    while (p != nullptr) {
        if (p->_k == k) {
            p->_v = v;   // actualització
            return;
        }
        p = p->_seg;
    }

    // Inserir al principi de la llista
    node_hash *nou = new node_hash(k, v, _taula[pos]);
    _taula[pos] = nou;
    ++_quants;
}

/*
    ------------------------------
    elimina
    ------------------------------
    Pre: true
    Post:
      - si k no existeix -> error(ClauInexistent)
      - si existeix -> elimina el node i decrementa _quants
    Cost esperat:
      - Temps: O(1) mitjà; O(n) pitjor
      - Espai: O(1)
*/
template <typename Valor>
void cataleg<Valor>::elimina(const string &k) {
    nat pos = hash(k);
    node_hash *p = _taula[pos];
    node_hash *ant = nullptr;

    while (p != nullptr) {
        if (p->_k == k) {
            // eliminar node
            if (ant == nullptr) {
                _taula[pos] = p->_seg;
            } else {
                ant->_seg = p->_seg;
            }
            delete p;
            --_quants;
            return;
        }
        ant = p;
        p = p->_seg;
    }

    throw error(ClauInexistent);
}

/*
    ------------------------------
    existeix?
    ------------------------------
    Pre: true
    Post: retorna true si k és al catàleg, false en cas contrari.
    Cost esperat:
      - Temps: O(1) mitjà; O(n) pitjor
      - Espai: O(1)
*/
template <typename Valor>
bool cataleg<Valor>::existeix(const string &k) const noexcept {
    nat pos = hash(k);
    node_hash *p = _taula[pos];

    while (p != nullptr) {
        if (p->_k == k) return true;
        p = p->_seg;
    }
    return false;
}

/*
    ------------------------------
    Acces per clau
    ------------------------------
    Pre: true
    Post:
      - si k existeix -> retorna una còpia del Valor associat
      - si no existeix -> error(ClauInexistent)
    Cost esperat:
      - Temps: O(1) mitjà; O(n) pitjor
      - Espai: O(1) (retorn per còpia)
*/
template <typename Valor>
Valor cataleg<Valor>::operator[](const string &k) const {
    nat pos = hash(k);
    node_hash *p = _taula[pos];

    while (p != nullptr) {
        if (p->_k == k) return p->_v;
        p = p->_seg;
    }

    throw error(ClauInexistent);
}

/*
    ------------------------------
    Contador
    ------------------------------
    Pre: true
    Post: retorna _quants.
    Cost: Θ(1)
*/
template <typename Valor>
nat cataleg<Valor>::quants() const noexcept {
    return _quants;
}
