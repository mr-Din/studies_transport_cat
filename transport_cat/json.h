#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>

namespace json {
	using namespace std::literals;

	class Node;

	using Dict = std::map<std::string, Node>;
	using Array = std::vector<Node>;

	// Эта ошибка выбрасывается при ошибках парсинга JSON
	class ParsingError : public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};

	class Node {
	public:
		using Value = std::variant<std::nullptr_t, int, bool, double, std::string, Array, Dict>;

		Node() = default;
		Node(std::nullptr_t value);
		Node(int value);
		Node(bool value);
		Node(double value);
		Node(std::string value);
		Node(Array array);
		Node(Dict map);

		bool IsInt() const;
		bool IsDouble() const;
		bool IsPureDouble() const;
		bool IsBool() const;
		bool IsString() const;
		bool IsNull() const;
		bool IsArray() const;
		bool IsMap() const;

		int AsInt() const;
		bool AsBool() const;
		double AsDouble() const;
		const std::string& AsString() const;
		const Array& AsArray() const;
		const Dict& AsMap() const;

		const Value& GetValue() const {
			return value_;
		}

	private:
		Value value_;
	};

	inline bool operator ==(Node left, Node right) {
		return left.GetValue() == right.GetValue();
	}

	inline bool operator !=(Node left, Node right) {
		return !(left == right);
	}

	class Document {
	public:
		explicit Document(Node root);

		const Node& GetRoot() const;

	private:
		Node root_;
	};

	inline bool operator ==(Document left, Document right) {
		return left.GetRoot() == right.GetRoot();
	}

	inline bool operator !=(Document left, Document right) {
		return !(left == right);
	}

	Document Load(std::istream& input);

	/*===========================================*/
	struct PrintContext {
		std::ostream& out;
		int indent_step = 4;
		int indent = 0;

		void PrintIndent() const {
			for (int i = 0; i < indent;++i) {
				out.put(' ');
			}
		}
		
		PrintContext Indented() const {
			return { out, indent_step, indent_step + indent };
		}
	};

	template <typename Value>
	inline void PrintValue(const Value& value, const PrintContext& ctx) {
		ctx.PrintIndent();
		ctx.out << value;
	}

	inline void PrintValue(std::nullptr_t, const PrintContext& ctx) {
		ctx.PrintIndent();
		ctx.out << "null"sv;
	}

	inline void PrintValue(const bool& value, const PrintContext& ctx) {
		ctx.PrintIndent();
		ctx.out << std::boolalpha << value;
	}

	inline void PrintValue(const std::string& value, const PrintContext& ctx) {
		ctx.PrintIndent();
		ctx.out << "\""sv;
		for (const char c : value) {
			if (c == '\r') {
				ctx.out << "\\r"sv;
			}
			else if (c == '\n') {
				ctx.out << "\\n"sv;
			}
			else if (c == '\\'|| c =='\"') {
				ctx.out << "\\"sv;
				ctx.out << c;
			}
			else {
				ctx.out << c;
			}
		}
		ctx.out << "\""sv;
	}

	void PrintNode(const Node&, const PrintContext& ctx);
	inline void PrintValue(const Array& array, const PrintContext& ctx) {
		ctx.PrintIndent();
		ctx.out << "[\n"sv;
		for (size_t i = 0; i < array.size(); ++i) {
			if (i > 0) { ctx.out << ",\n"sv; }
			PrintNode(array[i], ctx.Indented());
		}
		ctx.out << "\n]"sv;
	}
	inline void PrintValue(const Dict& dict, const PrintContext& ctx) {
		ctx.PrintIndent();
		ctx.out << "{\n"sv;
		int i = 0;
		for (const auto& [key, value] : dict) {
			if (i > 0) {
				ctx.out << ",\n"s;
			}
			++i;

			PrintNode(key, ctx.Indented());
			ctx.out << ":"sv;
			PrintNode(value, ctx);
		}
		ctx.out << "\n}"sv;
	}
	inline void PrintNode(const Node& node, const PrintContext& ctx) {
		std::visit(
			[&ctx](const auto& value) { PrintValue(value, ctx);},
			node.GetValue());
	}
	/*===========================================*/

	void Print(const Document& doc, std::ostream& output);

}  // namespace json