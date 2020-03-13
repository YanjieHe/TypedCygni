#lang typed/racket

(provide (all-defined-out))

(require racket/match)

(struct None ())
(struct (a) Some ([v : a]))
 
(define-type (Opt a) (U None (Some a)))

(struct Location (
  [start-line : Integer]
  [start-col : Integer]
  [end-line : Integer]
  [end-col : Integer]))

(define-type AST (U
                  Constant
                  Add
                  Subtract
                  Multiply
                  Divide
                  Modulo
                  GT
                  LT
                  GE
                  LE
                  EQ
                  NE
                  Block
                  VariableDefinition
                  Name
                  New
                  Assign
                  Return
                  Invoke
                  Conditional
                  Default))

(struct Call (
  [location : Location]
  [function : String]
  [args : (Listof AST)]))

(struct Add (
  [location : Location]
  [left : AST]
  [right : AST]))

(struct Subtract (
  [location : Location]
  [left : AST]
  [right : AST]))

(struct Multiply (
  [location : Location]
  [left : AST]
  [right : AST]))

(struct Divide (
  [location : Location]
  [left : AST]
  [right : AST]))

(struct Modulo (
  [location : Location]
  [left : AST]
  [right : AST]))

(struct GT (
  [location : Location]
  [left : AST]
  [right : AST]))

(struct LT (
  [location : Location]
  [left : AST]
  [right : AST]))

(struct GE (
  [location : Location]
  [left : AST]
  [right : AST]))

(struct LE (
  [location : Location]
  [left : AST]
  [right : AST]))

(struct EQ (
  [location : Location]
  [left : AST]
  [right : AST]))

(struct NE (
  [location : Location]
  [left : AST]
  [right : AST]))

(struct Constant (
    [location : Location]
    [val : Any]))

(struct Block
  ([location : Location]
   [expressions : (Listof AST)]))

(struct VariableDefinition
  ([location : Location]
   [name : Name]
   [value : AST]))

(struct Name
  ([location : Location]
   [name : String]))

(struct New
  ([location : Location]))

(struct Assign
  ([location : Location]))


(struct Return
  ([location : Location]))


(struct Invoke
  ([location : Location]))


(struct Conditional
  ([location : Location]
   [condition : AST]
   [if-true : AST]
   [if-false : AST]))

(struct Default
  ([location : Location]))

(struct Annotation
  ([name : String]
   [arguments : (Listof Constant)]))

(struct Function (
    [location : Location]
    [name : String]
    [annotations : (Listof Annotation)]
    [parameters : (Listof String)]
    [body : AST]))


; ******************** Scope ********************
(struct Scope (
  [parent : (Opt Scope)]
  [table : (HashTable String Any)]))

(: Scope-find (-> Scope String Any))
(define (Scope-find scope key)
  (define table (Scope-table scope))
  (if (hash-has-key? table key)
      (hash-ref table key)
      (match (Scope-parent scope)
        [(Some parent) (Scope-find parent key)]
        [(None) (error "key not found" key)])))

(: Scope-put (-> Scope String Any Void))
(define (Scope-put scope key value)
  (define table (Scope-table scope))
  (hash-set! table key value))

(struct Field (
    [location : Location]
    [name : String]
    [value : AST]))

(struct Module (
    [name : String]
    [fields : (HashTable String Field)]
    [methods : (HashTable String Function)]))

(struct Class (
    [name : String]
    [fields : (HashTable String Field)]
    [methods : (HashTable String Function)]))

(struct Program (
    [package-name : String]
    [classes : (HashTable String Class)]
    [modules : (HashTable String Module)]))
