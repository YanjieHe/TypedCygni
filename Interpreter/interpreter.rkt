#lang racket

(require json)
(require racket/match)

(define program
  (with-input-from-file "../build-Cygni-Desktop_Qt_5_14_1_MinGW_64_bit-Debug/sample_code/factorial.json"
    (lambda () (read-json))))


(struct scope
  (parent
   table))

(define (scope-find scope key)
  (define table (scope-table scope))
  (if (hash-has-key? table key)
      (hash-ref table key)
      (if (null? (scope-parent scope))
          (error "key not found" key)
          (scope-find (scope-parent scope) key))))

(define (scope-put scope key value)
  (define table (scope-table scope))
  (hash-set! table key value))


(define (eval-constant node)
  (match (hash-ref node 'type)
    ["Int32" (string->number (hash-ref node 'constant))]
    ["Boolean" (eq? "true" (hash-ref node 'constant))]))

(define (eval-binary node program scope function)
  (define left (evaluate (hash-ref node 'left) program scope))
  (define right (evaluate (hash-ref node 'right) program scope))
  (function left right))


(define (eval-invoke node program scope)
  (define function (evaluate (hash-ref node 'expression) program scope))
  (define arguments (for/list ([argument (hash-ref node 'arguments)])
                      (evaluate argument program scope)))
  (function arguments scope))


(define (eval-parameter node program scope)
  (scope-find scope (hash-ref node 'name)))


(define (eval-block node program scope)
  (last (for/list ([exp (hash-ref node 'expressions)])
          (evaluate exp program scope))))

(define (eval-if node program scope)
  (if (evaluate (hash-ref node 'condition) program scope)
      (evaluate (hash-ref node 'ifTrue) program scope)
      (evaluate (hash-ref node 'ifFalse) program scope)))


(define (eval-return node program scope)
  (evaluate (hash-ref node 'value) program scope))

(define (evaluate node program scope)
  (define node-type (hash-ref node 'nodeType))
  (match node-type
    ["Constant" (eval-constant node)]
    ["Add" (eval-binary node program scope +)]
    ["Subtract" (eval-binary node program scope -)]
    ["Multiply" (eval-binary node program scope *)]
    ["Divide" (eval-binary node program scope /)]
    ["Equal" (eval-binary node program scope =)]
    ["NotEqual" (eval-binary node program scope (lambda (x y) (not (= x y))))]
    ["Invoke" (eval-invoke node program scope)]
    ["Return" (eval-return node program scope)]
    ["Parameter" (eval-parameter node program scope)]
    ["Block" (eval-block node program scope)]
    ["Conditional" (eval-if node program scope)]
    )
  )

(define (eval-function func-info arguments program outer-scope)
  (define func-scope (scope outer-scope (make-hash)))
  (for/list ([parameter (hash-ref func-info 'parameters)]
             [argument arguments])
    (scope-put func-scope (hash-ref parameter 'name) argument))
  (scope-put func-scope (hash-ref func-info 'name)
             (lambda (args scope)
               (eval-function func-info args program scope)))
  (evaluate (hash-ref func-info 'body) program func-scope))


(define App (caddr (hash-ref program 'modules)))
(define Factorial (car (hash-ref App 'methods)))
(define Factorial-body (hash-ref Factorial 'body))
(define global-scope (scope '() (make-hash)))
(eval-function Factorial (list 10) program global-scope)