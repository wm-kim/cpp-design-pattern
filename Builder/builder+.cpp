#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
using namespace std;

class HtmlBuilder;

class HtmlElement
{
    friend class HtmlBuilder;

private:
    string name;
    string text;
    vector<HtmlElement> elements;
    const size_t indent_size = 2;

public:
    HtmlElement() {}
    HtmlElement(const string &name, const string &text)
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

        for (const auto &e : elements)
            oss << e.str(indent + 1);

        oss << i << "</" << name << ">" << endl;
        return oss.str();
    }

    static unique_ptr<HtmlBuilder> create(string root_name)
    {
        return make_unique<HtmlBuilder>(root_name);
    }

    // Builder methods for creating parts of the HtmlElement
    /* static HtmlBuilder create(const string &name)
        {
            return HtmlBuilder(name);
        } */
};

// dedicated builder class for building HTML elements with a fluent interface
class HtmlBuilder
{
public:
    HtmlBuilder(string root_name)
    {
        root.name = root_name;
    }

    // void to start with
    HtmlBuilder &add_child(string child_name, string child_text)
    {
        HtmlElement e{child_name, child_text};
        root.elements.emplace_back(e);
        return *this;
    }

    // pointer based
    HtmlBuilder *add_child_2(string child_name, string child_text)
    {
        HtmlElement e{child_name, child_text};
        root.elements.emplace_back(e);
        return this;
    }

    string str() { return root.str(); }

    HtmlElement build() { return root; }

    operator HtmlElement() const { return root; } // implicit conversion
    // return std::move(root); if you use factory instead of builder this might make more sense

private:
    HtmlElement root;
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
    string words[] = {"hello", "world"};
    ostringstream oss;
    oss << "<ul>";
    for (auto w : words)
        oss << "  <li>" << w << "</li>";
    oss << "</ul>";
    printf(oss.str().c_str());

    // easier
    HtmlBuilder builder{"ul"};
    builder.add_child("li", "hello").add_child("li", "world");
    cout << builder.str() << endl;

    auto builder2 = HtmlElement::create("ul")
                        ->add_child_2("li", "hello")
                        ->add_child_2("li", "world");
    cout << builder2 << endl;

    // HtmlElement e; // unavaillable

    HtmlElement::create("")->add_child("li", "world").build();

    getchar();
    return 0;
}