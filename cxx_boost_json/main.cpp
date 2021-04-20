#include <boost/asio/io_service.hpp>
#include <boost/json.hpp>
#include <boost/process.hpp>

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

#include <cassert>
#include <cstdio>

namespace bp  = boost::process;
namespace bj  = boost::json;
namespace bio = boost::asio;

using namespace std::literals::string_literals;

struct cpp_entity
{
    virtual ~cpp_entity()                                       = default;
    [[nodiscard]] virtual auto to_string() const -> std::string = 0;
};

auto operator<<(std::ostream& out, cpp_entity const& entity) -> std::ostream&
{
    out << entity.to_string();
    return out;
}

struct cpp_enum : cpp_entity
{
    std::string name {};
    std::optional<std::string> scope_tag {};
    std::vector<std::string> values {};

    ~cpp_enum() override = default;

    [[nodiscard]] auto to_string() const -> std::string override
    {
        auto text = std::string {"cpp_enum: "};
        text.append("enum ");
        text.append(scope_tag.has_value() ? *scope_tag : "");
        text.append(scope_tag.has_value() ? " " : "");
        text.append(name);
        text.append(" [");
        for (auto const& v : values)
        {
            text.append(v + ", ");
        }
        text.append("] ");
        return text;
    }
};

struct cpp_function : cpp_entity
{
    std::string name {};

    ~cpp_function() override = default;

    [[nodiscard]] auto to_string() const -> std::string override
    {
        auto text = std::string {"cpp_function: "};
        text.append(name);
        return text;
    }
};

[[nodiscard]] auto json_to_std_string(bj::value const& val) -> std::string
{
    auto const& s = val.as_string();
    return {s.data(), s.size()};
}

auto unquote(std::string& s) -> std::string&
{
    s.erase(std::remove(s.begin(), s.end(), '\"'), s.end());
    return s;
}

[[nodiscard]] auto parse_ast_json(boost::string_view src) -> bj::value
{
    auto opt = bj::parse_options {size_t(-1), false, false, false};
    auto p   = bj::parser {{}, opt};
    p.write(src);
    return p.release();
}

[[nodiscard]] auto run_clang(std::vector<std::string> const& args,
                             bio::io_service& svc)
    -> std::future<std::vector<char>>
{
    auto out         = std::future<std::vector<char>> {};
    auto const clang = bp::search_path("clang++");
    auto const cmd   = bp::child(clang, args, bp::std_out > out, svc);
    svc.run();
    return out;
}

auto print_recursive(bj::object const& node, boost::string_view tag) -> void
{
    if (node.contains(tag))
    {
        std::cout << node.at(tag) << '\n';
    }

    if (node.contains("inner") && node.at("inner").is_array())
    {
        auto const& array = node.at("inner").as_array();
        for (auto const& child : array)
        {
            if (child.is_object())
            {
                print_recursive(child.as_object(), tag);
            }
        }
    }
}

struct clang_json_ast_parser
{
    auto register_entity_parser(
        std::string kind,
        std::function<std::unique_ptr<cpp_entity>(bj::object const&)> handler)
        -> void
    {
        if (handlers_.find(kind) == handlers_.end())
        {
            handlers_.emplace(std::move(kind), std::move(handler));
            return;
        }

        assert(false);
    }

    auto visit(bj::object const& obj) -> std::unique_ptr<cpp_entity>
    {
        auto const& k = obj.at("kind").as_string();
        auto kind     = std::string {k.data(), k.size()};
        unquote(kind);

        if (auto handler = handlers_.find(kind); handler != handlers_.end())
        {
            return handler->second(obj);
        }

        return {};
    }

private:
    std::unordered_map<std::string, std::function<std::unique_ptr<cpp_entity>(
                                        bj::object const&)>>
        handlers_;
};

auto walk(bj::object const& root, clang_json_ast_parser& visitor) -> void
{
    if (root.contains("kind"))
    {
        if (auto node = visitor.visit(root); node)
        {
            std::cout << *node << '\n';
        }
    }

    if (root.contains("inner") && root.at("inner").is_array())
    {
        auto const& array = root.at("inner").as_array();
        for (auto const& child : array)
        {
            if (child.is_object())
            {
                walk(child.as_object(), visitor);
            }
        }
    }
}

auto parse_enum(bj::object const& obj) -> std::unique_ptr<cpp_entity>
{
    assert(obj.contains("kind"));
    auto enumeration = std::make_unique<cpp_enum>();

    if (obj.contains("name"))
    {
        enumeration->name = json_to_std_string(obj.at("name"));
    }

    if (obj.contains("scopedEnumTag"))
    {
        enumeration->scope_tag = json_to_std_string(obj.at("scopedEnumTag"));
    }

    if (obj.contains("inner"))
    {
        auto const& inner = obj.at("inner").as_array();
        for (auto const& item : inner)
        {
            auto name = json_to_std_string(item.as_object().at("name"));
            enumeration->values.push_back(unquote(name));
        }
    }

    return enumeration;
}

auto parse_function(bj::object const& obj) -> std::unique_ptr<cpp_entity>
{
    assert(obj.contains("kind"));
    auto function = std::make_unique<cpp_function>();

    if (obj.contains("name"))
    {
        function->name = json_to_std_string(obj.at("name"));
    }

    return function;
}

auto main(int argc, char** argv) -> int
{
    auto const arguments = std::vector<std::string> {
        "-Xclang",
        "-ast-dump=json",
        "-fsyntax-only",
        "-std=c++17",
        argc == 2 ? argv[1] : "../test.hpp"s,
    };

    bio::io_service svc;
    auto output = run_clang(arguments, svc);
    auto raw    = output.get();

    auto const json_ast = parse_ast_json({raw.data(), raw.size()});
    auto visitor        = clang_json_ast_parser {};
    visitor.register_entity_parser("FunctionDecl", parse_function);
    visitor.register_entity_parser("EnumDecl", parse_enum);
    walk(json_ast.as_object(), visitor);

    // print_recursive(json_ast.as_object(), "kind");

    return EXIT_SUCCESS;
}
