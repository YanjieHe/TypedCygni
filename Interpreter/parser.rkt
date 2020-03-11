#lang typed/racket

(require "ast.rkt")
(require typed/json)

; ******************** Load AST ********************

(define code-path
  "../build-Cygni-Desktop_Qt_5_14_1_MinGW_64_bit-Debug/sample_code/factorial.json")  

(: JSExpr)
(define program-json
  (with-input-from-file code-path
    (lambda () (read-json))))

(: (parse-program (-> JSExpr)))
(define (parse-program program-json)
  (define modules (hash-ref program-json 'modules))
  (define classes (hash-ref program-json 'classes)))

(define (parse-module module-json)
  (define name (hash-ref module-json 'name)))

(define (parse-field field-json)
  (define name (hash-ref field-json))
  (define value (hash-ref field-json)))

(: (parse-location (-> JSExpr Location)))
(define (parse-location location-json)
  (Location
    (hash-ref location-json 'startLine)
    (hash-ref location-json 'startCol)
    (hash-ref location-json 'endLine)
    (hash-ref location-json 'endCol)))


(: (parse-constant (-> JSExpr Constant)))
(define (parse-constant constant-json)
  (define location (hash-ref expr-json 'Location))
  (Constant location
    (match (hash-ref node 'type)
    ["Int32" (string->number (hash-ref node 'constant))]
    ["Boolean" (string=? "true" (hash-ref node 'constant))]
    ["String" (hash-ref node 'constant)])))

; TO DO
(define (parse-expr expr-json)
  (define node-type (hash-ref expr-json 'nodeType))
  (define location (hash-ref expr-json 'Location))
  (match node-type
    ["Constant" (parse-constant expr-json)]
    ["Add" ()]