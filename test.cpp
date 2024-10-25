#include <iostream>
#include <string>
#include <map>
#include <cctype>

int no_of_errors = 0;  // Initialize error count
std::istream* input = &std::cin; // Pointer for input stream
std::map<std::string, double> table; // Variable storage

enum class Kind : char {
    name, number, end,
    plus = '+', minus = '-', mul = '*', div = '/',
    print = ';', assign = '=', lp = '(', rp = ')'
};

struct Token {
    Kind kind;
    std::string string_value;
    double number_value;
};

class Token_stream {
public:
    Token_stream(std::istream& s) : ip{ &s }, owns{ false } {}
    Token_stream(std::istream* p) : ip{ p }, owns{ true } {}
    ~Token_stream() { close(); }

    Token get();

    const Token& current() { return ct; }

    void set_input(std::istream& s) { close(); ip = &s; owns = false; }
    void set_input(std::istream* p) { close(); ip = p; owns = true; }

private:
    void close() { if (owns) delete ip; }

    std::istream* ip;
    bool owns;
    Token ct{ Kind::end };
};

double expr(bool);
double term(bool);
double prim(bool);
double error(const std::string&);
void calculate();

int main() {
    table["pi"] = 3.1415926535897932385;
    table["e"] = 2.7182818284590452354;

    calculate();

    return no_of_errors;
}

void calculate() {
    while (true) {
        ts.get();
        if (ts.current().kind == Kind::end) break;
        if (ts.current().kind == Kind::print) continue;
        std::cout << expr(false) << "\n";
    }
}

double error(const std::string& s) {
    no_of_errors++;
    std::cerr << "error::" << s << '\n';
    return 1;
}

Token Token_stream::get() {
    char ch = 0;
    *ip >> std::skipws >> ch; // Skip whitespace

    switch (ch) {
    case 0:
        return ct = { Kind::end };
    case ';':
    case '*':
    case '/':
    case '+':
    case '-':
    case '(':
    case ')':
    case '=':
        return ct = { static_cast<Kind>(ch) };
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        ip->putback(ch);
        *ip >> ct.number_value;
        ct.kind = Kind::number;
        return ct;
    default:
        if (isalnum(ch)) {
            ip->putback(ch);
            *ip >> ct.string_value;
            ct.kind = Kind::name;
            return ct;
        }
    }
    error("bad token");
    return ct = { Kind::end };
}

double expr(bool get) {
    double left = term(get);

    while (true) {
        switch (ts.current().kind) {
        case Kind::plus:
            left += term(true);
            break;
        case Kind::minus:
            left -= term(true);
            break;
        default:
            return left;
        }
    }
}

double term(bool get) {
    double left = prim(get);

    while (true) {
        switch (ts.current().kind) {
        case Kind::mul:
            left *= prim(true);
            break;
        case Kind::div: {
            double d = prim(true);
            if (d == 0) return error("divide by 0");
            left /= d;
            break;
        }
        default:
            return left;
        }
    }
}

double prim(bool get) {
    if (get) ts.get();

    switch (ts.current().kind) {
    case Kind::number:
        double v = ts.current().number_value;
        ts.get();
        return v;
    case Kind::name: {
        double& v = table[ts.current().string_value];
        if (ts.get().kind == Kind::assign) v = expr(true);
        return v;
    }
    case Kind::minus:
        return -prim(true);
    case Kind::lp:
        auto e = expr(true);
        if (ts.current().kind != Kind::rp) return error("')' expected");
        ts.get();
        return e;
    default:
        return error("primary expected");
    }
}
