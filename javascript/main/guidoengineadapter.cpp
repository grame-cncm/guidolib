#include "guidoengineadapter.h"
#include "GUIDOEngine.h"
#include "GUIDOParse.h"

int GuidoEngineAdapter::nbinstantiation = 0;

GuidoEngineAdapter::GuidoEngineAdapter()
{
    nbinstantiation++;
    cout << "Constructor n°"<< nbinstantiation << endl;
}

GuidoEngineAdapter::~GuidoEngineAdapter()
{
    cout << "Destructor n°"<< nbinstantiation << endl;
}

void GuidoEngineAdapter::GuidoInit()
{
    cout << "Intialisation" << endl;
    GuidoInitWithIndependentSVG();
}

GuidoParser * GuidoEngineAdapter::GuidoOpenParser() {
    return ::GuidoOpenParser();
}

void GuidoEngineAdapter::CloseParser(GuidoParser * parser) {
    ::GuidoCloseParser(parser);
}

ARHandler GuidoEngineAdapter::GuidoString2AR(GuidoParser * p, const string guidoCode) {
    cout << "GuidoString2AR" << endl;
    return ::GuidoString2AR (p, guidoCode.c_str());
}

string GuidoEngineAdapter::getSvg(GuidoParser * p, const string guidoCode) {
    ARHandler ar = ::GuidoString2AR (p, guidoCode.c_str());
    GRHandler gr = ::GuidoGetAR2GR (ar, 0);
    string result = GuidoGetSVGExportWithFontSpec(gr, 1);
    ::GuidoFreeGR(gr);

    return result;
}

vector<string> GuidoEngineAdapter::returnVector() {
    this->testvector.push_back("bonjour");
    this->testvector.push_back("le monde");
    return this->testvector;
}

/*
#include <emscripten.h>
#include <bind.h>
#include <iostream>
#include <string>
using namespace emscripten;


  class A {
      public:
          A () {
              a=5;
          }
          void add() {
              a = a+1;
          }
          int get() {
              return a;
          }
      private:
          int a;
  };

  class B {
      public:
          B () {
              b=10;
          }
          void add() {
              b = b+1;
          }
          int get() {
              return b;
          }

          A* getA() {
              return new A;
          }

          void addA(A* a) {
              b+=a->get();
          }
          std::string getString() {
              return str;
          }
          void setString (std::string str) {
              this->str = str;
          }
      private:
          int b;
          std::string str;
  };


EMSCRIPTEN_BINDINGS(test) {
    class_<A>("A");

    class_<B>("B")
          .constructor<>()
          .function("add",&B::add)
          .function("get",&B::get)
          .function("getA",&B::getA, allow_raw_pointers())
          .function("addA",&B::addA, allow_raw_pointers())
          .function("getString",&B::getString)
          .function("setString",&B::setString)
          ;
}
*/
/*
emcc -c  --bind -std=c++11 main/guidoengineadapter.cpp -o obj/main/guidoengineadapter.o
emcc -O2 --bind obj/main/guidoengineadapter.o -o libengine.js -s ALLOW_MEMORY_GROWTH=1 -s ASM_JS=1
*/
