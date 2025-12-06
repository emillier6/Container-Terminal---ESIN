#include "contenidor.hpp"

contenidor::contenidor(const string &m, nat l){
    bool error1 = false, error2 = false;

    if(m.length() > 0){
        int i = 0, n = m.length();
        while(i < n and not error1){
            // INV: los caracteres de 0 a i-1 de s pertenecen a ('A' - 'Z') o a ('0' - '9')
            if(not ((m[i] >= 'A' and m[i] <= 'Z') or (m[i] >= '0' and m[i] <= '9'))){
                error1 = true;
            }
            ++i;
        }

        if(l != 10 and l != 20 and l != 30){
            error2 = true;
        }
        
    }else{
        error1 = true;
    }

    if(error1){
        throw error(MatriculaIncorrecta, "contenidor", "Matricula incorrecta.");
    }else if(error2){
        throw error(LongitudIncorrecta, "contenidor", "Longitud incorrecta.");
    }else{
        _matricula = m;
        _longitud = l;
    }
}

contenidor::contenidor(const contenidor &u){
    _matricula = u._matricula;
    _longitud = u._longitud;
}

contenidor &contenidor::operator=(const contenidor &u){
    _matricula = u._matricula;
    _longitud = u._longitud;
    return *this;
}

contenidor::~contenidor() noexcept{

}

nat contenidor::longitud() const noexcept{
    return _longitud;
}

string contenidor::matricula() const noexcept{
    return _matricula;
}

bool contenidor::operator==(const contenidor &c) const noexcept{
    return (_matricula == c._matricula and _longitud == c._longitud);
}

bool contenidor::operator!=(const contenidor &c) const noexcept{
    return not(*this == c);
}

bool contenidor::operator<(const contenidor &c) const noexcept{
    string m1 = sort(_matricula), m2 = sort(c._matricula);
    return m1 < m2 or (m1 == m2 and _longitud < c._longitud);
}

bool contenidor::operator<=(const contenidor &c) const noexcept{
    string m1 = sort(_matricula), m2 = sort(c._matricula);
    return m1 < m2 or (m1 == m2 and _longitud <= c._longitud);
}

bool contenidor::operator>(const contenidor &c) const noexcept{
    string m1 = sort(_matricula), m2 = sort(c._matricula);
    return m1 > m2 or (m1 == m2 and _longitud > c._longitud);
}

bool contenidor::operator>=(const contenidor &c) const noexcept{
    string m1 = sort(_matricula), m2 = sort(c._matricula);
    return m1 > m2 or (m1 == m2 and _longitud >= c._longitud);
}

//Métodos privados

void contenidor::merge(string &s, int left, int mid, int right){
    int n1 = mid - left + 1;
    int n2 = right - mid;

    string L = "";
    string R = "";

    for(int i = 0; i < n1; ++i){
        L[i] = s[left+i];
    }
    for(int j = 0; j < n2; ++j){
        R[j] = s[mid +1 + j];
    }

    int i = 0, j = 0;
    int k = left;

    while(i < n1 and j < n2){
        if(L[i] <= R[j]){
            s[k] = L[i];
            ++i;
        }else{
            s[k] = R[j];
            ++j;
        }
        ++k;
    }

    while(i < n1){
        s[k] = L[i];
        ++i;
        ++k;
    }

    while(j < n2){
        s[k] = R[j];
        ++j;
        ++k;
    }
}

void contenidor::mergeSort(string &s, int left, int right){
    if(left < right){
        int mid = left + (right -left) / 2;
        mergeSort(s, left, mid);
        mergeSort(s, mid + 1, right);
        merge(s, left, mid, right);
    }
}

string contenidor::sort(const string& m){
    string r = m;
    mergeSort(r, 0, r.length()-1);
    return r;
}