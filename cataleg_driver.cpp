#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <unordered_map>

#include <esin/error>
#include <esin/util>

#include "cataleg.hpp"

using std::string;
using std::unordered_map;
using util::nat;

// ------------------------------------------------------------
// Lector de la taula d'errors (gestterm.err)
// Format línia: <codi> <modul> <missatge...>
// Exemple: 31 cataleg Clau inexistent.
// ------------------------------------------------------------
struct ErrInfo {
  string modul;
  string msg;
};

static unordered_map<int, ErrInfo> load_err_table(const string& filename) {
  unordered_map<int, ErrInfo> m;
  if (filename.empty()) return m;

  std::ifstream f(filename);
  if (!f) return m;

  string line;
  while (std::getline(f, line)) {
    if (line.empty()) continue;

    std::istringstream iss(line);
    int code;
    string modul;
    if (!(iss >> code >> modul)) continue;

    string rest;
    std::getline(iss, rest);           // inclou espai inicial
    if (!rest.empty() && rest[0] == ' ')
      rest.erase(0, 1);                // trim espai inicial

    m[code] = {modul, rest};
  }
  return m;
}

static inline bool starts_with(const string& s, const string& p) {
  return s.rfind(p, 0) == 0;
}

// Treu cometes si ve "..." o '...'. I interpreta "" / '' com a buit.
static string normalize_key(string k) {
  if (k == "\"\"" || k == "''") return "";
  if (k.size() >= 2) {
    if ((k.front() == '"' && k.back() == '"') ||
        (k.front() == '\'' && k.back() == '\'')) {
      return k.substr(1, k.size() - 2);
    }
  }
  return k;
}

// Imprimeix error exactament com volen els JP: Error::<modul>:<codi>:<missatge>
static void print_error_as_jp(const error& e,
                             const unordered_map<int, ErrInfo>& table) {
  int code = e.codigo();

  auto it = table.find(code);
  if (it != table.end()) {
    std::cout << "Error::" << it->second.modul << ":" << code << ":"
              << it->second.msg << "\n";
  } else {
    // Fallback (si falta el fitxer o algun codi no està a la taula)
    std::cout << e << "\n";
  }
}

int main(int argc, char** argv) {
  // Permet passar el fitxer d'errors com a argument (per defecte: gestterm.err)
  // Ex: ./cataleg_driver.exe gestterm.err < jp_public_cataleg.in > out
  string errfile = "gestterm.err";
  if (argc >= 2) errfile = argv[1];

  const auto errtable = load_err_table(errfile);

  unordered_map<string, cataleg<int>*> cats;
  cataleg<int>* current = nullptr;
  string current_id;

  // Lookahead robust (evita tellg/seekg)
  string pending;
  bool has_pending = false;

  auto get_line = [&](string& out) -> bool {
    if (has_pending) {
      out = pending;
      has_pending = false;
      pending.clear();
      return true;
    }
    return static_cast<bool>(std::getline(std::cin, out));
  };

  auto unget_line = [&](const string& s) {
    pending = s;
    has_pending = true;
  };

  string line;
  while (get_line(line)) {
    if (line.empty()) {
      std::cout << "\n";
      continue;
    }

    // Headings / comentaris generals del .in
    if (starts_with(line, "##!")) {
      // "##!..." -> "###!..."
      std::cout << "###!";
      if (line.size() > 3) std::cout << line.substr(3);
      std::cout << "\n";
      continue;
    }
    if (starts_with(line, "##")) {
      // "##..." -> "###..."
      std::cout << "#" << line << "\n";
      continue;
    }

    // Si apareix una línia output-expected fora de lloc, la passem a expected
    if (starts_with(line, "#")) {
      std::cout << "##" << line.substr(1) << "\n";
      continue;
    }

    // És una operació: al .out surt comentada amb '#'
    std::cout << "#" << line << "\n";

    // L’expected output (si existeix) ha d’anar just després d’una operació.
    string expected;
    bool has_expected = false;

    string next;
    if (get_line(next)) {
      if (starts_with(next, "#") && !starts_with(next, "##")) {
        has_expected = true;
        expected = next.substr(1); // sense el primer '#'
      } else {
        unget_line(next); // no era expected
      }
    }

    // Parse operació
    std::istringstream iss(line);
    string op;
    iss >> op;

    try {
      if (op == "init") {
        // init a1 cataleg<int> 0
        string id, type;
        nat n;
        iss >> id >> type >> n;

        if (cats.count(id)) { delete cats[id]; cats.erase(id); }

        cats[id] = new cataleg<int>(n);
        current = cats[id];
        current_id = id;
      }
      else if (op == "assig") {
        // assig KuCha12 12
        string k; int v;
        iss >> k >> v;
        k = normalize_key(k);

        if (current) current->assig(k, v);
      }
      else if (op == "val") {
        // val KuCha12
        string k; iss >> k;
        k = normalize_key(k);

        if (current) {
          int v = (*current)[k];
          std::cout << v << "\n";
        }
      }
      else if (op == "existeix") {
        string k; iss >> k;
        k = normalize_key(k);

        if (current) {
          std::cout << (current->existeix(k) ? "true" : "false") << "\n";
        }
      }
      else if (op == "quants") {
        if (current) std::cout << current->quants() << "\n";
      }
      else if (op == "elimina") {
        string k; iss >> k;
        k = normalize_key(k);

        if (current) current->elimina(k);
      }
      else {
        // pot ser "<id> destroy"
        string id = op;
        string cmd;
        iss >> cmd;
        if (cmd == "destroy") {
          auto it = cats.find(id);
          if (it != cats.end()) {
            delete it->second;
            cats.erase(it);
          }
          if (id == current_id) {
            current = nullptr;
            current_id.clear();
          }
        }
      }
    }
    catch (error& e) {
      // IMPORTANT: l’error “real” ha d’anar sense prefix, tal com es veu als .res.
      print_error_as_jp(e, errtable);
    }

    // Imprimeix expected amb prefix "##"
    if (has_expected) {
      std::cout << "##" << expected << "\n";
    }
  }

  // Neteja
  for (auto& kv : cats) delete kv.second;
  return 0;
}
