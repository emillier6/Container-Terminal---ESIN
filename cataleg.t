// ------------------------------
// Constructor
// ------------------------------
template <typename Valor>
cataleg<Valor>::cataleg(nat numelems) {
    _M = 2 * numelems + 1;     // mida recomanada
    _taula = new node_hash*[_M];

    for (nat i = 0; i < _M; ++i)
        _taula[i] = nullptr;

    _quants = 0;
}

// ------------------------------
// Constructor per còpia
// ------------------------------
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

// ------------------------------
// Assignació
// ------------------------------
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

// ------------------------------
// Destructor
// ------------------------------
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

// ------------------------------
// Inserció / Actualització
// ------------------------------
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

// ------------------------------
// Eliminació
// ------------------------------
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

// ------------------------------
// Existeix?
// ------------------------------
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

// ------------------------------
// Accés per clau
// ------------------------------
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

// ------------------------------
// Comptador
// ------------------------------
template <typename Valor>
nat cataleg<Valor>::quants() const noexcept {
    return _quants;
}
