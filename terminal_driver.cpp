#include <iostream>
#include <string>
#include <list>

#include <esin/error>
#include <esin/gen_driver>
#include <esin/util>

#include "contenidor.hpp"
#include "ubicacio.hpp"
#include "terminal.hpp"

using util::nat;
using namespace std;

// imprimir ubicacio i imprimir contenidor
ostream &operator<<(ostream &os, const ubicacio &u)
{
    os << "<" << u.filera() << ", " << u.placa() << ", " << u.pis() << ">";
    return os;
}

ostream &operator<<(ostream &os, const contenidor &c)
{
    os << "<" << c.matricula() << ", " << c.longitud() << ">\n";
    return os;
}

// imprimeix un booleà (format del driver del professor)
ostream &operator<=(ostream &os, const bool b)
{
    os << (b ? "true" : "false");
    return os;
}

// imprimeix una llista de T
template <typename T>
ostream &operator<<(ostream &os, const list<T> &L)
{
    if (L.empty())
    {
        os << "[]";
    }
    else
    {
        auto it = L.begin();
        T prev = *it;
        os << "[";
        ++it;
        while (it != L.end())
        {
            os << prev << ", ";
            prev = *it;
            ++it;
        }
        os << prev << "]";
    }
    return os;
}

/*-------------------------< user_init >--------------------------*/
// Crea objectes a partir de: init <id> <tipus> <args...>
void *user_init(gen_driver &dr)
{
    string tid = dr.args(2);

    if (tid == "contenidor")
    {
        if (dr.nargs() == 4)
        {
            if (util::toint(dr.args(4)) < 0)
            {
                throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
                            gen_driver::WrongTypeArgsMsg);
            }
            nat longi = static_cast<nat>(util::toint(dr.args(4)));
            return static_cast<void *>(new contenidor(dr.args(3), longi));
        }
        throw error(gen_driver::WrongNumArgs, gen_driver::nom_mod,
                    gen_driver::WrongNumArgsMsg);
    }

    if (tid == "ubicacio")
    {
        if (dr.nargs() == 5)
        {
            return static_cast<void *>(new ubicacio(util::toint(dr.args(3)),
                                                    util::toint(dr.args(4)),
                                                    util::toint(dr.args(5))));
        }
        throw error(gen_driver::WrongNumArgs, gen_driver::nom_mod,
                    gen_driver::WrongNumArgsMsg);
    }

    if (tid == "terminal")
    {
        // init t terminal n m h        -> FIRST_FIT per defecte
        // init t terminal n m h STRAT  -> FIRST_FIT o LLIURE
        if (dr.nargs() == 5 || dr.nargs() == 6)
        {
            if ((util::toint(dr.args(3)) < 0) || (util::toint(dr.args(4)) < 0) ||
                (util::toint(dr.args(5)) < 0))
            {
                throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
                            gen_driver::WrongTypeArgsMsg);
            }

            nat n = static_cast<nat>(util::toint(dr.args(3)));
            nat m = static_cast<nat>(util::toint(dr.args(4)));
            nat h = static_cast<nat>(util::toint(dr.args(5)));

            terminal::estrategia estrat = terminal::estrategia::FIRST_FIT;

            if (dr.nargs() == 6)
            {
                if (dr.args(6) == "FIRST_FIT")
                    estrat = terminal::estrategia::FIRST_FIT;
                else if (dr.args(6) == "LLIURE")
                    estrat = terminal::estrategia::LLIURE;
                else
                {
                    // si el professor passa un string no vàlid, que el vostre codi ho gestioni
                    // (o bé aquí podríem forçar WrongTypeArgs)
                    estrat = terminal::estrategia::LLIURE; // no importa: el constructor ha de validar
                }
            }

            return static_cast<void *>(new terminal(n, m, h, estrat));
        }

        throw error(gen_driver::WrongNumArgs, gen_driver::nom_mod,
                    gen_driver::WrongNumArgsMsg);
    }

    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
                gen_driver::WrongTypeArgsMsg);
}

/*-------------------------< TypeTraits >--------------------------*/
template <>
const char *TypeTraits<contenidor>::name = "contenidor";
template <>
const char *TypeTraits<ubicacio>::name = "ubicacio";
template <>
const char *TypeTraits<terminal>::name = "terminal";

/*-------------------------< CONTENIDOR >--------------------------*/
void tracta_longitud(gen_driver &dr)
{
    contenidor *c = dr.object<contenidor>();
    dr.get_ostream() << c->longitud() << endl;
}

void tracta_matricula(gen_driver &dr)
{
    contenidor *c = dr.object<contenidor>();
    dr.get_ostream() << c->matricula() << endl;
}

/*-------------------------< UBICACIO >---------------------------*/
void tracta_filera(gen_driver &dr)
{
    ubicacio *u = dr.object<ubicacio>();
    dr.get_ostream() << u->filera() << endl;
}
void tracta_placa(gen_driver &dr)
{
    ubicacio *u = dr.object<ubicacio>();
    dr.get_ostream() << u->placa() << endl;
}
void tracta_pis(gen_driver &dr)
{
    ubicacio *u = dr.object<ubicacio>();
    dr.get_ostream() << u->pis() << endl;
}

/*--------------------< OPERADORES RELACIONALES >-------------------*/
void tracta_menor(gen_driver &dr)
{
    if (dr.object_type() == "contenidor")
    {
        contenidor *c1 = dr.object<contenidor>();
        contenidor *c2 = dr.object<contenidor>(1);
        (dr.get_ostream() <= (*c1 < *c2)) << endl;
    }
    else if (dr.object_type() == "ubicacio")
    {
        ubicacio *u1 = dr.object<ubicacio>();
        ubicacio *u2 = dr.object<ubicacio>(1);
        (dr.get_ostream() <= (*u1 < *u2)) << endl;
    }
    else
    {
        throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
                    gen_driver::WrongTypeArgsMsg);
    }
}

void tracta_major(gen_driver &dr)
{
    if (dr.object_type() == "contenidor")
    {
        contenidor *c1 = dr.object<contenidor>();
        contenidor *c2 = dr.object<contenidor>(1);
        (dr.get_ostream() <= (*c1 > *c2)) << endl;
    }
    else if (dr.object_type() == "ubicacio")
    {
        ubicacio *u1 = dr.object<ubicacio>();
        ubicacio *u2 = dr.object<ubicacio>(1);
        (dr.get_ostream() <= (*u1 > *u2)) << endl;
    }
    else
    {
        throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
                    gen_driver::WrongTypeArgsMsg);
    }
}

void tracta_igual(gen_driver &dr)
{
    if (dr.object_type() == "contenidor")
    {
        contenidor *c1 = dr.object<contenidor>();
        contenidor *c2 = dr.object<contenidor>(1);
        (dr.get_ostream() <= (*c1 == *c2)) << endl;
    }
    else if (dr.object_type() == "ubicacio")
    {
        ubicacio *u1 = dr.object<ubicacio>();
        ubicacio *u2 = dr.object<ubicacio>(1);
        (dr.get_ostream() <= (*u1 == *u2)) << endl;
    }
    else
    {
        throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
                    gen_driver::WrongTypeArgsMsg);
    }
}

void tracta_no_igual(gen_driver &dr)
{
    if (dr.object_type() == "contenidor")
    {
        contenidor *c1 = dr.object<contenidor>();
        contenidor *c2 = dr.object<contenidor>(1);
        (dr.get_ostream() <= (*c1 != *c2)) << endl;
    }
    else if (dr.object_type() == "ubicacio")
    {
        ubicacio *u1 = dr.object<ubicacio>();
        ubicacio *u2 = dr.object<ubicacio>(1);
        (dr.get_ostream() <= (*u1 != *u2)) << endl;
    }
    else
    {
        throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
                    gen_driver::WrongTypeArgsMsg);
    }
}

void tracta_menor_igual(gen_driver &dr)
{
    if (dr.object_type() == "contenidor")
    {
        contenidor *c1 = dr.object<contenidor>();
        contenidor *c2 = dr.object<contenidor>(1);
        (dr.get_ostream() <= (*c1 <= *c2)) << endl;
    }
    else if (dr.object_type() == "ubicacio")
    {
        ubicacio *u1 = dr.object<ubicacio>();
        ubicacio *u2 = dr.object<ubicacio>(1);
        (dr.get_ostream() <= (*u1 <= *u2)) << endl;
    }
    else
    {
        throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
                    gen_driver::WrongTypeArgsMsg);
    }
}

void tracta_major_igual(gen_driver &dr)
{
    if (dr.object_type() == "contenidor")
    {
        contenidor *c1 = dr.object<contenidor>();
        contenidor *c2 = dr.object<contenidor>(1);
        (dr.get_ostream() <= (*c1 >= *c2)) << endl;
    }
    else if (dr.object_type() == "ubicacio")
    {
        ubicacio *u1 = dr.object<ubicacio>();
        ubicacio *u2 = dr.object<ubicacio>(1);
        (dr.get_ostream() <= (*u1 >= *u2)) << endl;
    }
    else
    {
        throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
                    gen_driver::WrongTypeArgsMsg);
    }
}

/*------------------------------< TERMINAL >-----------------------*/
void tracta_on(gen_driver &dr)
{
    terminal *t = dr.object<terminal>();
    string s = dr.args(1);
    dr.get_ostream() << t->on(s) << endl;
}

void tracta_longitud_terminal(gen_driver &dr)
{
    terminal *t = dr.object<terminal>();
    string s = dr.args(1);
    dr.get_ostream() << t->longitud(s) << endl;
}

void tracta_contenidor_ocupa(gen_driver &dr)
{
    terminal *t = dr.object<terminal>();
    ubicacio *u = dr.object<ubicacio>(dr.args(1));
    string m = "";
    t->contenidor_ocupa(*u, m);
    dr.get_ostream() << m << endl;
}

void tracta_fragmentacio(gen_driver &dr)
{
    terminal *t = dr.object<terminal>();
    dr.get_ostream() << t->fragmentacio() << endl;
}

void tracta_ops_grua(gen_driver &dr)
{
    terminal *t = dr.object<terminal>();
    dr.get_ostream() << t->ops_grua() << endl;
}

void tracta_area_espera(gen_driver &dr)
{
    terminal *t = dr.object<terminal>();
    list<string> L;
    t->area_espera(L);
    dr.get_ostream() << L << endl;
}

void tracta_num_fileres(gen_driver &dr)
{
    terminal *t = dr.object<terminal>();
    dr.get_ostream() << t->num_fileres() << endl;
}

void tracta_num_places(gen_driver &dr)
{
    terminal *t = dr.object<terminal>();
    dr.get_ostream() << t->num_places() << endl;
}

void tracta_num_pisos(gen_driver &dr)
{
    terminal *t = dr.object<terminal>();
    dr.get_ostream() << t->num_pisos() << endl;
}

void tracta_insereix_contenidor(gen_driver &dr)
{
    contenidor *c = dr.object<contenidor>(dr.args(1));
    dr.object<terminal>()->insereix_contenidor(*c);
}

void tracta_retira_contenidor(gen_driver &dr)
{
    string s = dr.args(1);
    dr.object<terminal>()->retira_contenidor(s);
}

void tracta_mapa(gen_driver &dr)
{
    terminal *t = dr.object<terminal>();
    list<string> l;
    t->area_espera(l);

    dr.get_ostream() << "espera: " << l << endl;
    dr.get_ostream() << "fragmentacio: " << t->fragmentacio() << endl;
    dr.get_ostream() << "ops_grua: " << t->ops_grua() << endl;

    for (nat i = 0; i < t->num_fileres(); ++i)
    {
        dr.get_ostream() << endl
                         << "filera " << i << endl;
        dr.get_ostream() << "---------" << endl;

        for (int k = (int)t->num_pisos() - 1; k >= 0; --k)
        {
            dr.get_ostream() << "pis " << k << ' ';
            for (nat j = 0; j < t->num_places(); ++j)
            {
                ubicacio u((int)i, (int)j, (int)k);
                string m = "";
                t->contenidor_ocupa(u, m);

                string out(3, ' ');
                if (m == "" && k == 0)
                    out = string(3, '_');
                else
                {
                    for (nat x = 0; x < m.length() && x < 3; ++x)
                        out[x] = m[x];
                }
                dr.get_ostream() << " " << out;
            }
            dr.get_ostream() << endl;
        }

        dr.get_ostream() << "       ";
        for (nat j = 0; j < t->num_places(); ++j)
        {
            string s = util::tostring((int)j);
            if (s.length() > 3)
                s = s.substr(0, 3);
            else
                while (s.length() < 3)
                    s.push_back(' ');
            dr.get_ostream() << ' ' << s;
        }
        dr.get_ostream() << endl;
    }
}

/*------------------------------< MAIN >-----------------------*/
int main()
{
    // IMPORTANT: així no cal fer load_err_table manual:
    // gen_driver carrega la taula d'errors des del fitxer.
    gen_driver d("gestterm.err", false);

    // Contenidor
    d.add_call("longitud", tracta_longitud, "contenidor");
    d.add_call("matricula", tracta_matricula, "contenidor");

    // Ubicacio
    d.add_call("filera", tracta_filera, "ubicacio");
    d.add_call("placa", tracta_placa, "ubicacio");
    d.add_call("pis", tracta_pis, "ubicacio");

    // Operadors
    d.add_call("<", tracta_menor, "any", "any");
    d.add_call(">", tracta_major, "any", "any");
    d.add_call("==", tracta_igual, "any", "any");
    d.add_call("!=", tracta_no_igual, "any", "any");
    d.add_call("<=", tracta_menor_igual, "any", "any");
    d.add_call(">=", tracta_major_igual, "any", "any");

    // Terminal
    d.add_call("on", tracta_on, "terminal", "string");
    d.add_call("long_terminal", tracta_longitud_terminal, "terminal", "string");
    d.add_call("contenidor_ocupa", tracta_contenidor_ocupa, "terminal", "ubicacio");
    d.add_call("fragmentacio", tracta_fragmentacio, "terminal");
    d.add_call("ops_grua", tracta_ops_grua, "terminal");
    d.add_call("area_espera", tracta_area_espera, "terminal");
    d.add_call("num_fileres", tracta_num_fileres, "terminal");
    d.add_call("num_places", tracta_num_places, "terminal");
    d.add_call("num_pisos", tracta_num_pisos, "terminal");

    d.add_call("insereix_c", tracta_insereix_contenidor, "terminal", "contenidor");
    d.add_call("insereix_contenidor", tracta_insereix_contenidor, "terminal", "contenidor");
    d.add_call("retira_c", tracta_retira_contenidor, "terminal", "string");
    d.add_call("retira_contenidor", tracta_retira_contenidor, "terminal", "string");
    d.add_call("mapa", tracta_mapa, "terminal");

    // Instal·lació tipus
    d.install_type<contenidor>();
    d.install_type<ubicacio>();
    d.install_type<terminal>();

    d.go();
}
