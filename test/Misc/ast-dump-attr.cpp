// RUN: %clang_cc1 -triple x86_64-pc-linux -std=c++11 -ast-dump -ast-dump-filter Test %s | FileCheck --strict-whitespace %s

int TestLocation
__attribute__((unused));
// CHECK:      VarDecl{{.*}}TestLocation
// CHECK-NEXT:   UnusedAttr 0x{{[^ ]*}} <line:[[@LINE-2]]:16>

int TestIndent
__attribute__((unused));
// CHECK:      {{^\(VarDecl.*TestIndent[^()]*$}}
// CHECK-NEXT: {{^  \(UnusedAttr[^()]*\)\)$}}

void TestAttributedStmt() {
  switch (1) {
  case 1:
    [[clang::fallthrough]];
  case 2:
    ;
  }
}
// CHECK:      FunctionDecl{{.*}}TestAttributedStmt
// CHECK:      AttributedStmt
// CHECK-NEXT:   FallThroughAttr
// CHECK-NEXT:   NullStmt

[[clang::warn_unused_result]] int TestCXX11DeclAttr();
// CHECK:      FunctionDecl{{.*}}TestCXX11DeclAttr
// CHECK-NEXT:   WarnUnusedResultAttr

int TestAlignedNull __attribute__((aligned));
// CHECK:      VarDecl{{.*}}TestAlignedNull
// CHECK-NEXT:   AlignedAttr
// CHECK-NEXT:     <<<NULL>>>

int TestAlignedExpr __attribute__((aligned(4)));
// CHECK:      VarDecl{{.*}}TestAlignedExpr
// CHECK-NEXT:   AlignedAttr
// CHECK-NEXT:     IntegerLiteral

int TestEnum __attribute__((visibility("default")));
// CHECK:      VarDecl{{.*}}TestEnum
// CHECK-NEXT:   VisibilityAttr{{.*}} Default

class __attribute__((lockable)) Mutex {
} mu1, mu2;
int TestExpr __attribute__((guarded_by(mu1)));
// CHECK:      VarDecl{{.*}}TestExpr
// CHECK-NEXT:   GuardedByAttr
// CHECK-NEXT:     DeclRefExpr{{.*}}mu1

class Mutex TestVariadicExpr __attribute__((acquired_after(mu1, mu2)));
// CHECK:      VarDecl{{.*}}TestVariadicExpr
// CHECK:        AcquiredAfterAttr
// CHECK-NEXT:     DeclRefExpr{{.*}}mu1
// CHECK-NEXT:     DeclRefExpr{{.*}}mu2

void function1(void *) {
  int TestFunction __attribute__((cleanup(function1)));
}
// CHECK:      VarDecl{{.*}}TestFunction
// CHECK-NEXT:   CleanupAttr{{.*}} Function{{.*}}function1

void TestIdentifier(void *, int)
__attribute__((pointer_with_type_tag(ident1,1,2)));
// CHECK: FunctionDecl{{.*}}TestIdentifier
// CHECK:   ArgumentWithTypeTagAttr{{.*}} ident1

void TestBool(void *, int)
__attribute__((pointer_with_type_tag(bool1,1,2)));
// CHECK: FunctionDecl{{.*}}TestBool
// CHECK:   ArgumentWithTypeTagAttr{{.*}} IsPointer

void TestUnsigned(void *, int)
__attribute__((pointer_with_type_tag(unsigned1,1,2)));
// CHECK: FunctionDecl{{.*}}TestUnsigned
// CHECK:   ArgumentWithTypeTagAttr{{.*}} 0 1

void TestInt(void) __attribute__((constructor(123)));
// CHECK:      FunctionDecl{{.*}}TestInt
// CHECK-NEXT:   ConstructorAttr{{.*}} 123

int TestString __attribute__((alias("alias1")));
// CHECK:      VarDecl{{.*}}TestString
// CHECK-NEXT:   AliasAttr{{.*}} "alias1"

extern struct s1 TestType
__attribute__((type_tag_for_datatype(ident1,int)));
// CHECK:      VarDecl{{.*}}TestType
// CHECK-NEXT:   TypeTagForDatatypeAttr{{.*}} int

void *TestVariadicUnsigned1(int) __attribute__((alloc_size(1)));
// CHECK: FunctionDecl{{.*}}TestVariadicUnsigned1
// CHECK:   AllocSizeAttr{{.*}} 0

void *TestVariadicUnsigned2(int, int) __attribute__((alloc_size(1,2)));
// CHECK: FunctionDecl{{.*}}TestVariadicUnsigned2
// CHECK:   AllocSizeAttr{{.*}} 0 1