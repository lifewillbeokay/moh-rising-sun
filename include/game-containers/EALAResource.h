// Scoped model of the reference-counted EALA::Resource base; see docs/GameAlgorithms.md.
// The destructor is inline because derived destructors inline it; pragma interface keeps its weak copy out of
// units that only use the class.
#ifndef MOH_EALA_RESOURCE_H
#define MOH_EALA_RESOURCE_H
#pragma interface
namespace EALA {
class Resource {
  public:
    int field_00;
    Resource() : field_00(0) {}
    virtual ~Resource() {} // Inlined in derived destructors; the weak copy is at 0x8027c918.
    void Release() {
        if (--field_00 == 0)
            delete this;
    }
    template <class T> class Pointer {
        T *value;

      public:
        Pointer() : value(0) {}
        Pointer(T *p) : value(p) {
            if (value)
                ++reinterpret_cast<Resource *>(value)->field_00;
        }
        Pointer(const Pointer &other) : value(other.value) {
            if (value)
                ++reinterpret_cast<Resource *>(value)->field_00;
        }
        template <class U> Pointer(const Pointer<U> &other) : value(other) {
            if (value)
                ++reinterpret_cast<Resource *>(value)->field_00;
        }
        ~Pointer() {
            if (value)
                reinterpret_cast<Resource *>(value)->Release();
        }
        Pointer &operator=(const Pointer &other) {
            if (value != other.value) {
                T *previous = value;
                value = other.value;
                if (value)
                    ++reinterpret_cast<Resource *>(value)->field_00;
                if (previous)
                    reinterpret_cast<Resource *>(previous)->Release();
            }
            return *this;
        }
        bool IsValid() const { return value != 0; } // Descriptive name; the compiled test yields a bool.
        T *operator->() const { return value; }
        operator T *() const { return value; }
    };
};
} // namespace EALA
#endif
