#lang typed/racket

(require "ast.rkt")
(require typed/json)

; ******************** Load AST ********************

(define code-path
  "../build-Cygni-Desktop_Qt_5_14_1_MinGW_64_bit-Debug/sample_code/factorial.json")  


(define program-json
  (with-input-from-file code-path
    (lambda () (read-json))))


(: parse-program (-> (HashTable Symbol JSExpr) Program))
(define (parse-program program-json)
  (define package-name (cast (hash-ref program-json 'packageName) String))
  (define modules-json (cast (hash-ref program-json 'modules) (HashTable Symbol JSExpr)))
  (define classes-json (cast (hash-ref program-json 'classes) (HashTable Symbol JSExpr)))
  (define modules : (HashTable String Module) (make-hash))
  (define classes : (HashTable String Class) (make-hash))
  (for-each (lambda (module-name-symbol)
              (define module-symbol (cast module-name-symbol Symbol))
              (define module-name (symbol->string module-symbol))
              (define module-info
                (parse-module (cast (hash-ref modules-json module-symbol) JSExpr)))
              (hash-set! modules module-name module-info))
            (hash-keys modules-json))
  (for-each (lambda (class-name-symbol)
              (define class-symbol (cast class-name-symbol Symbol))
              (define class-name (symbol->string class-symbol))
              (define class-info
                (parse-class (cast (hash-ref classes-json class-symbol) JSExpr)))
              (hash-set! classes class-name class-info))
            (hash-keys (cast classes-json (HashTable Symbol JSExpr))))
  (Program package-name classes modules))


(: parse-class (-> JSExpr Class))
(define (parse-class class-json)
  (if (hash? class-json)
      (let ([name (cast (hash-ref class-json 'name) String)]
            [fields : (HashTable String Field) (make-hash)]
            [methods : (HashTable String Function) (make-hash)]
            [fields-json (cast (hash-ref class-json 'fields) (HashTable Symbol JSExpr))]
            [methods-json (cast (hash-ref class-json 'methods) (HashTable Symbol JSExpr))])
        (begin
          (for-each (lambda (field-symbol)
                      (let* ([field-symbol (cast field-symbol Symbol)]
                             [field-json (hash-ref fields-json field-symbol)]
                             [field (parse-field
                                     (cast field-json JSExpr))])
                        (hash-set! fields (Field-name field) field)))
                    (hash-keys fields-json))
          (for-each (lambda (method-symbol)
                      (let* ([method-symbol (cast method-symbol Symbol)]
                             [method-json (hash-ref methods-json method-symbol)]
                             [method (parse-func-def
                                      (cast method-json JSExpr))])
                        (hash-set! methods (Function-name method) method)))
                    (hash-keys methods-json))
          (Class name fields methods)))
      (error "class json format")))


(: parse-module (-> JSExpr Module))
(define (parse-module module-json)
  (if (hash? module-json)
      (let ([name (cast (hash-ref module-json 'name) String)]
            [fields : (HashTable String Field) (make-hash)]
            [methods : (HashTable String Function) (make-hash)]
            [fields-json (cast (hash-ref module-json 'fields) (HashTable Symbol JSExpr))]
            [methods-json (cast (hash-ref module-json 'methods) (HashTable Symbol JSExpr))])
        (begin
          (for-each (lambda (field-symbol)
                      (let* ([field-symbol (cast field-symbol Symbol)]
                             [field-json (hash-ref fields-json field-symbol)]
                             [field (parse-field
                                     (cast field-json JSExpr))])
                        (hash-set! fields (Field-name field) field)))
                    (hash-keys fields-json))
          (for-each (lambda (method-symbol)
                      (let* ([method-symbol (cast method-symbol Symbol)]
                             [method-json (hash-ref methods-json method-symbol)]
                             [method (parse-func-def
                                      (cast method-json JSExpr))])
                        (hash-set! methods (Function-name method) method)))
                    (hash-keys methods-json))
          (Module name fields methods)))
      (error "module json format")))


(: parse-field (-> JSExpr Field))
(define (parse-field field-json)
  (if (hash? field-json)
      (let ([location (parse-location (hash-ref field-json 'location))]
            [name (hash-ref field-json 'name)]
            [value (parse-expr (hash-ref field-json 'value))])
        (Field location (cast name String) value))
      (error "field json format")))


(: parse-func-def (-> JSExpr Function))
(define (parse-func-def input-json)
  (define func-json (if (hash? input-json)
                        input-json
                        (error "function json format")))
  (define location (parse-location (hash-ref func-json 'location)))
  (define name (cast (hash-ref func-json 'name) String))
  (define parameters-json (cast (hash-ref func-json 'parameters) (Listof JSExpr)))
  (define parameters
    (map
     (lambda (parameter-json)
       (cast (hash-ref
              (cast parameter-json (HashTable Symbol JSExpr)) 'name) String))
      parameters-json))
  (define annotations-json
    (cast (hash-ref func-json 'annotations) (HashTable Symbol JSExpr)))
  (define annotations
    (map
     (lambda (annotation-name)
       (define annotation-json (hash-ref annotations-json annotation-name))
       (parse-annotation (cast annotation-json JSExpr)))
     (hash-keys annotations-json)))
  (define body (parse-expr (hash-ref func-json 'body)))
  (Function location name annotations parameters body))


(: parse-annotation (-> JSExpr Annotation))
(define (parse-annotation input-json)
  (define ann-json (cast input-json (HashTable Symbol JSExpr)))
  (define name (cast (hash-ref ann-json 'name) String))
  (define constant-list
    (map (lambda (input-json)
           (define constant-json (hash-ref (cast input-json (HashTable Symbol JSExpr)) 'value))
           (parse-constant constant-json))
         (cast (hash-ref ann-json 'arguments) (Listof JSExpr))))
  (Annotation name constant-list))


(: parse-location (-> JSExpr Location))
(define (parse-location location-json)
  (if (hash? location-json)
      (Location
       (cast (hash-ref location-json 'startLine) Integer)
       (cast (hash-ref location-json 'startCol) Integer)
       (cast (hash-ref location-json 'endLine) Integer)
       (cast (hash-ref location-json 'endCol) Integer))
      (error "location json format")))


(: parse-constant (-> JSExpr Constant))
(define (parse-constant constant-json)
  (if (hash? constant-json)
      (let ([location (parse-location (hash-ref constant-json 'location))])
        (Constant location
                  (match (hash-ref constant-json 'type)
                    ["Int32" (string->number
                              (cast (hash-ref constant-json 'constant) String))]
                    ["Boolean" (string=? "true"
                                         (cast (hash-ref constant-json 'constant) String))]
                    ["String" (hash-ref constant-json 'constant)])))
      (error "constant json format")))


(: parse-binary-exp (-> JSExpr AST))
(define (parse-binary-exp input-json)
  (define binary-json (if (hash? input-json)
                          input-json
                          (error "binary expression json format")))
  (define location (parse-location (hash-ref binary-json 'location)))
  (define left (parse-expr (hash-ref binary-json 'left)))
  (define right (parse-expr (hash-ref binary-json 'right)))
  ((match (hash-ref binary-json 'nodeType)
     ["Add" Add]
     ["Subtract" Subtract]
     ["Multiply" Multiply]
     ["Divide" Divide]
     ["Modulo" Modulo]
     ["GreaterThan" GT]
     ["LessThan" LT]
     ["GreaterThanOrEqual" GE]
     ["LessThanOrEqual" LE]
     ["Equal" EQ]
     ["NotEqual" NE])
   location left right))


(: parse-block (-> JSExpr Block))
(define (parse-block input-json)
  (define block-json (cast input-json (HashTable Symbol JSExpr)))
  (define location (parse-location (hash-ref block-json 'location)))
  (define expressions
    (map parse-expr
         (cast (hash-ref block-json 'expressions) (Listof JSExpr))))
  (Block location expressions))

(: parse-var-def (-> JSExpr VariableDefinition))
(define (parse-var-def input-json)
  (define var-def-json (cast input-json (HashTable Symbol JSExpr)))
  (define location (parse-location (hash-ref var-def-json 'location)))
  (define name (parse-name (hash-ref var-def-json 'variable)))
  (define value (parse-expr (hash-ref var-def-json 'value)))
  (VariableDefinition
   location name value))


(: parse-name (-> JSExpr Name))
(define (parse-name input-json)
  (define name-json (cast input-json (HashTable Symbol JSExpr)))
  (define location (parse-location (hash-ref name-json 'location)))
  (define name (cast (hash-ref name-json 'name) String))
  (Name location name))


(: parse-new (-> JSExpr New))
(define (parse-new input-json)
  (define new-json (cast input-json (HashTable Symbol JSExpr)))
  (define location (parse-location (hash-ref new-json 'location)))
  (New location))


(: parse-assign (-> JSExpr Assign))
(define (parse-assign input-json)
  (define assign-json (cast input-json (HashTable Symbol JSExpr)))
  (define location (parse-location (hash-ref assign-json 'location)))
  (Assign location))


(: parse-return (-> JSExpr Return))
(define (parse-return input-json)
  (define return-json (cast input-json (HashTable Symbol JSExpr)))
  (define location (parse-location (hash-ref return-json 'location)))
  (Return location))

(: parse-invoke (-> JSExpr Invoke))
(define (parse-invoke input-json)
  (define invoke-json (cast input-json (HashTable Symbol JSExpr)))
  (define location (parse-location (hash-ref invoke-json 'location)))
  (Invoke location))


(: parse-conditional (-> JSExpr Conditional))
(define (parse-conditional input-json)
  (define conditional-json (cast input-json (HashTable Symbol JSExpr)))
  (define location (parse-location (hash-ref conditional-json 'location)))
  (define condition (parse-expr (hash-ref conditional-json 'condition)))
  (define if-true (parse-expr (hash-ref conditional-json 'ifTrue)))
  (define if-false (parse-expr (hash-ref conditional-json 'ifFalse)))
  (Conditional location condition if-true if-false))


(: parse-default (-> JSExpr Default))
(define (parse-default input-json)
  (define default-json (cast input-json (HashTable Symbol JSExpr)))
  (define location (parse-location (hash-ref default-json 'location)))
  (Default location))


(: parse-expr (-> JSExpr AST))
(define (parse-expr input-json)
  (define expr-json (if (hash? input-json)
                        input-json
                        (error "expression json format")))
  (define node-type (hash-ref expr-json 'nodeType))
  (define location (hash-ref expr-json 'location))
  (match node-type
    ["Constant" (parse-constant expr-json)]
    ["Add" (parse-binary-exp expr-json)]
    ["Subtract" (parse-binary-exp expr-json)]
    ["Multiply" (parse-binary-exp expr-json)]
    ["Divide" (parse-binary-exp expr-json)]
    ["Modulo" (parse-binary-exp expr-json)]
    ["GreaterThan" (parse-binary-exp expr-json)]
    ["LessThan" (parse-binary-exp expr-json)]
    ["GreaterThanOrEqual" (parse-binary-exp expr-json)]
    ["LessThanOrEqual" (parse-binary-exp expr-json)]
    ["Equal" (parse-binary-exp expr-json)]
    ["NotEqual" (parse-binary-exp expr-json)]
    ["Block" (parse-block expr-json)]
    ["VariableDefinition" (parse-var-def expr-json)]
    ["New" (parse-new expr-json)]
    ["Assign" (parse-assign expr-json)]
    ["Return" (parse-return expr-json)]
    ["Invoke" (parse-invoke expr-json)]
    ["Conditional" (parse-conditional expr-json)]
    ["Default" (parse-default expr-json)]
    ["Parameter" (parse-name expr-json)]
    [_ (error "not supported node type" node-type)]))


(parse-program (cast program-json (HashTable Symbol JSExpr)))
