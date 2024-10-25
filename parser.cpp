#include "parser.h"
#include "table.h"

double Parser::prim(bool get, Token_stream& ts) {
	if (get) ts.get();	//��ȡ��һ������

	double v = 1.0;		// ��������case�ж��壬����������ȥ

	switch (ts.current().kind) {
	case Kind::number: // ����������
		v = ts.current().number_value;
		ts.get();
		return v;
	case Kind::name:	// ���ʽ�еġ�������
		// ��ȡ��Ӧ����
		v = Table::table[ts.current().string_value];
		// �����һ��������'=',��ȡ'='����ı��ʽ��ֵ
		if (ts.get().kind == Kind::assign) {
			v = expr(true, ts);
		}
			
		return v;
	case Kind::minus:	// һԪ����
		return -prim(true, ts);
	case Kind::lp:		// '(���ʽ'
		// ��ȡ�����ڱ��ʽ��ֵ
		v = expr(true, ts);
		// �ж�'(���ʽ'��һ�������Ƿ���')'������������ʾ')'�쳣
		if (ts.current().kind != Kind::rp) return error("')' excepted");
		ts.get();		//')'��һ���ַ�
		return v;
	default:
		return error("primary excepted");
	}
}

double Parser::term(bool get, Token_stream& ts) {
	double left = prim(get, ts);

	for (;;) {
		switch (ts.current().kind) {
		case Kind::mul:
			left *= prim(true, ts);
			break;
		case Kind::div:
			if (auto d = prim(true, ts)) {
				left /= d;
				break;
			}
			return error("divide by 0");
		default:
			return left;
		}
	}
}

double Parser::expr(bool get, Token_stream& ts) {
	double left = term(get, ts);

	for (;;) {
		switch (ts.current().kind) {
		case Kind::plus:
			left += term(true, ts);
			break;
		case Kind::minus:
			left -= term(true, ts);
			break;
		default:
			return left;
		}
	}
}