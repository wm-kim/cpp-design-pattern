// Some objects are simple and can be created in a single constructor call
// other object require lot of ceremony to create
// having constructor with 10+ arguments is not productive 
// instead pice-wise construction
// builder provide an API for constructing an object step-by-step


#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
using namespace std;

struct HtmlBuilder;

struct HtmlElement
{
    // friend class HtmlBuilder
    string name;
    string text;
    vector<HtmlElement> elements;
    const size_t indent_size = 2; // 들여쓰기 크기

    // make HtmlElement constructor private will force to use Builder 
    HtmlElement() {}
    HtmlElement(const string& name, const string& text)
        : name(name),
        text(text)
    {
    }

    string str(int indent = 0) const
    {
        ostringstream oss;
        string i(indent_size * indent, ' ');
        oss << i << "<" << name << ">" << endl;
        if (text.size() > 0)
            oss << string(indent_size * (indent + 1), ' ') << text << endl;

        for (const auto& e : elements)
            oss << e.str(indent + 1); // recursive function

        oss << i << "</" << name << ">" << endl;
        return oss.str();
    }

    // show client there is builder api. instead of returning value hoping for return value optimization
    static unique_ptr<HtmlBuilder> build(string root_name) // rename this to create
    {
        return make_unique<HtmlBuilder>(root_name);
    }
};

// This is builder
struct HtmlBuilder
{
    HtmlBuilder(string root_name)
    {
        root.name = root_name;
    }

    // void to start with
    HtmlBuilder& add_child(string child_name, string child_text)
    {
        HtmlElement e{ child_name, child_text };
        root.elements.emplace_back(e);
        return *this;
    }

    // pointer based
    HtmlBuilder* add_child_2(string child_name, string child_text)
    {
        HtmlElement e{ child_name, child_text };
        root.elements.emplace_back(e);
        return this;
    }

    string str() { return root.str(); }

    // last operation, 변환 연산자, HtmlElement에 대입되면 자동 변환
    operator HtmlElement() const { return root; } // in this case returning copy of it 
    // return std::move(root);
    HtmlElement root;

    // recommand more reference API than pointer API

    // HtmlElement build() { return root; } // better and more sensible api, because client can't know there is implicit conversion to HtmlElement
};

int demo()
{
    // <p>hello</p>
    auto text = "hello";
    string output;
    output += "<p>";
    output += text;
    output += "</p>";
    printf("<p>%s</p>", text);

    // <ul><li>hello</li><li>world</li></ul>
    string words[] = { "hello", "world" };
    ostringstream oss;
    oss << "<ul>";
    for (auto w : words)
        oss << "  <li>" << w << "</li>";
    oss << "</ul>";
    printf(oss.str().c_str());

    // easier
    HtmlBuilder builder{ "ul" };
    builder.add_child("li", "hello").add_child("li", "world"); // fluent interface
    cout << builder.str() << endl;

    // static builder in HtmlElement
    auto builder2 = HtmlElement::build("ul")
        ->add_child_2("li", "hello")->add_child_2("li", "world");
    cout << builder2 << endl;

    

    getchar();
    return 0;
}

