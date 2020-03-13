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
            [fields-json (hash-ref class-json 'fields)]
            [methods-json (hash-ref class-json 'methods)])
        (if (and (list? fields-json) (list? methods-json))
            (begin
              (for-each (lambda (field-json)
                          (let ([field (parse-field
                                        (cast field-json JSExpr))])
                            (hash-set! fields (Field-name field) field)))
                        fields-json)
              (for-each (lambda (method-json)
                          (let ([method (parse-func-def
                                         (cast method-json JSExpr))])
                            (hash-set! methods (Function-name method) method)))
                        methods-json)
              (Class name fields methods))
            (error "class fields or methods json format")))
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
    (map parse-constant (cast (hash-ref ann-json 'arguments) (Listof JSExpr))))
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
                    ["Int32" (cast (hash-ref constant-json 'constant) Number)]
                    ["Boolean" (string=? "true"
                                         (cast (hash-ref constant-json 'constant) String))]
                    ["String" (hash-ref constant-json 'constant)])))
      (error "constant json format")))


(: parse-binary-exp (-> JSExpr AST))
(define (parse-binary-exp input-json)
  (define binary-json (if (hash? input-json)
                          input-json
                          (error "binary expression json format")))
  (define location (parse-location binary-json))
  (define left (parse-expr (hash-ref binary-json 'left)))
  (define right (parse-expr (hash-ref binary-json 'right)))
  ((match (hash-ref binary-json 'type)
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
    [_ (error "not supported node type" node-type)]))


(parse-program (cast program-json (HashTable Symbol JSExpr)))
