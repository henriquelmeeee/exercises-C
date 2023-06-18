template <typename T = int>
class Vector {
  private:
    T var[255];
    signed int last_index;
  public:
    Vector() : last_index(-1) {}

    bool append(T val) { 
      ++last_index; 
      if(last_index < 256) { 
        var[last_index] = val;
        return true;
      } else {
        --last_index;
        return false;
      }
    }
    void remove_last() { 
      var[last_index] = T(); // coloca o valor original de T 
      (--last_index) ? (last_index > -1) : last_index; 
     }

    T& operator[](size_t i) { // Sobrecarregando operador "[]"
      if( (i > last_index) || (i < 0) )
        throw std::out_of_range("Índice inválido");
      return var[i];
    }
}
