#ifndef SI_VISITOR_H__
#define SI_VISITOR_H__

namespace SI {
namespace Interpreter {

class BaseVisitor {
    public:
        virtual ~Visitor() = default;
};

template <typename T, typename R = void>
class Visitor : public BaseVisitor {
    public:
      using ReturnType = R;
      R visit(T&) = 0;
};

}
}

#endif // SI_VISITOR_H__