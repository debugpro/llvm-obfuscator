struct foo;
void func(foo *f) {
}
class bar;
void func(bar *f) {
}
union baz;
void func(baz *f) {
}

class B {
public:
  virtual ~B();
};

B::~B() {
}

struct C {
  static int s;
  virtual ~C();
};

C::~C() {
}

struct D {
  D();
  virtual ~D();
  void func() {
  }
};

struct E {
  E();
  virtual ~E();
  virtual void func() {
  }
};

struct F {
  struct inner {
  };
  static const int i = 2;
  virtual ~F();
};

struct G {
  virtual void func();
  struct inner {
    int j;
  };
};

struct H {};
struct I : virtual H {};
struct J : I {};
J j;

struct A {
  int one;
  static const int HdrSize = 52;
  int two;
  A() {
    int x = 1;
  }
};

void f1() {
  D x;
  x.func();
  E y;
  int i = F::i;
  F::inner z;
}

int main(int argc, char **argv) {
  B b;
  G::inner c_i;
  if (argc) {
    A a;
  }
  return 0;
}

// RUN: %clang_cc1 -triple x86_64-unknown_unknown -emit-llvm -debug-info-kind=limited -fexceptions -std=c++98 %s -o - | FileCheck -check-prefix=CHECK98 %s
// RUN: %clang_cc1 -triple i686-cygwin -emit-llvm -debug-info-kind=limited -fexceptions -std=c++98 %s -o - | FileCheck -check-prefix=CHECK98 %s
// RUN: %clang_cc1 -triple armv7l-unknown-linux-gnueabihf -emit-llvm -debug-info-kind=limited -fexceptions -std=c++98 %s -o - | FileCheck -check-prefix=CHECK98 %s
// RUN: %clang_cc1 -triple x86_64-unknown_unknown -emit-llvm -debug-info-kind=limited -fexceptions -std=c++11 %s -o - | FileCheck -check-prefix=CHECK11 %s
// RUN: %clang_cc1 -triple i686-cygwin -emit-llvm -debug-info-kind=limited -fexceptions -std=c++11 %s -o - | FileCheck -check-prefix=CHECK11 %s
// RUN: %clang_cc1 -triple armv7l-unknown-linux-gnueabihf -emit-llvm -debug-info-kind=limited -fexceptions -std=c++11 %s -o - | FileCheck -check-prefix=CHECK11 %s

// CHECK98: invoke {{.+}} @_ZN1BD1Ev(%class.B* %b)
// CHECK98-NEXT: unwind label %{{.+}}, !dbg ![[EXCEPTLOC:.*]]
// CHECK11: call {{.+}} @_ZN1BD1Ev(%class.B* %b){{.*}}, !dbg ![[EXCEPTLOC:.*]]

// CHECK: store i32 0, i32* %{{.+}}, !dbg ![[RETLOC:.*]]

// CHECK: [[F:![0-9]*]] = !DICompositeType(tag: DW_TAG_structure_type, name: "F"
// CHECK-SAME:                             DIFlagFwdDecl
// CHECK-SAME:                             identifier: "_ZTS1F"
// CHECK: !DICompositeType(tag: DW_TAG_structure_type, name: "I"
// CHECK-NOT:              DIFlagFwdDecl
// CHECK-SAME:             ){{$}}

// CHECK: ![[INT:[0-9]+]] = !DIBasicType(name: "int"
// CHECK: !DICompositeType(tag: DW_TAG_structure_type, name: "foo"
// CHECK: !DICompositeType(tag: DW_TAG_class_type, name: "bar"
// CHECK: !DICompositeType(tag: DW_TAG_union_type, name: "baz"
// CHECK: !DICompositeType(tag: DW_TAG_class_type, name: "B"
// CHECK-NOT:              DIFlagFwdDecl
// CHECK-SAME:             ){{$}}
// CHECK: !DIDerivedType(tag: DW_TAG_member, name: "_vptr$B",
// CHECK-SAME:           DIFlagArtificial

// CHECK: [[C:![0-9]*]] = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "C",
// CHECK-NOT:                              DIFlagFwdDecl
// CHECK-SAME:                             elements: [[C_MEM:![0-9]*]]
// CHECK-SAME:                             vtableHolder: [[C]]
// CHECK-SAME:                             identifier: "_ZTS1C"
// CHECK: [[C_MEM]] = !{[[C_VPTR:![0-9]*]], [[C_S:![0-9]*]], [[C_DTOR:![0-9]*]]}
// CHECK: [[C_VPTR]] = !DIDerivedType(tag: DW_TAG_member, name: "_vptr$C"
// CHECK-SAME:                        DIFlagArtificial
// CHECK: [[C_S]] = !DIDerivedType(tag: DW_TAG_member, name: "s"
// CHECK-SAME:                     baseType: ![[INT]]
// CHECK-SAME:                     DIFlagStaticMember
// CHECK: [[C_DTOR]] = !DISubprogram(name: "~C"

// CHECK: [[D:![0-9]+]] = !DICompositeType(tag: DW_TAG_structure_type, name: "D"
// CHECK-NOT:              size:
// CHECK-SAME:             DIFlagFwdDecl
// CHECK-SAME:             identifier: "_ZTS1D"
// CHECK: !DICompositeType(tag: DW_TAG_structure_type, name: "E"
// CHECK-SAME:             DIFlagFwdDecl
// CHECK-SAME:             identifier: "_ZTS1E"

// CHECK: !DISubprogram(name: "func",{{.*}} scope: [[D]]
// CHECK-SAME:          isDefinition: true
// CHECK-SAME:          declaration: [[D_FUNC_DECL:![0-9]*]]
// CHECK: [[D_FUNC_DECL]] = !DISubprogram(name: "func",{{.*}} scope: [[D]]
// CHECK-SAME:                            isDefinition: false

// CHECK: !DICompositeType(tag: DW_TAG_structure_type, name: "inner",{{.*}} line: 50
// CHECK-NOT: DIFlagFwdDecl
// CHECK-SAME: elements: [[G_INNER_MEM:![0-9]*]]
// CHECK-SAME: identifier: "_ZTSN1G5innerE"

// CHECK: !DICompositeType(tag: DW_TAG_structure_type, name: "G"
// CHECK-SAME:             DIFlagFwdDecl
// CHECK-SAME:             identifier: "_ZTS1G"
// CHECK: [[G_INNER_MEM]] = !{[[G_INNER_I:![0-9]*]]}
// CHECK: [[G_INNER_I]] = !DIDerivedType(tag: DW_TAG_member, name: "j"
// CHECK-SAME:                           baseType: ![[INT]]

// CHECK: !DICompositeType(tag: DW_TAG_structure_type, name: "A"
// CHECK: !DIDerivedType(tag: DW_TAG_member, name: "HdrSize"
//
// CHECK: ![[EXCEPTLOC]] = !DILocation(line: 84,
// CHECK: ![[RETLOC]] = !DILocation(line: 83,
