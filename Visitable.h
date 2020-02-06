#ifndef SI_VISITABLE_H__
#define SI_VISITABLE_H__

namespace SI {
namespace Interpreter { 

#define ACCEPT_VISITOR 

template <typename R>
class BaseVisitable {
public:
    using ReturnType = R;
    template <typename T>
    static ReturnType acceptImp() {
        v.visit(*this);
    }
    // R accept(Visitor<> *v) {

    // }
};

}
}

#endif // SI_VISITABLE_H__