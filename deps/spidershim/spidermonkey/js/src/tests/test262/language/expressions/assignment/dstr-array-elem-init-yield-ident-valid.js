// This file was procedurally generated from the following sources:
// - src/dstr-assignment/array-elem-init-yield-ident-valid.case
// - src/dstr-assignment/default/assignment-expr.template
/*---
description: When a `yield` token appears within the Initializer of an AssignmentElement outside of a generator function body, it behaves as an IdentifierReference. (AssignmentExpression)
esid: sec-variable-statement-runtime-semantics-evaluation
es6id: 13.3.2.4
features: [destructuring-binding]
flags: [generated, noStrict]
info: |
    VariableDeclaration : BindingPattern Initializer

    1. Let rhs be the result of evaluating Initializer.
    2. Let rval be GetValue(rhs).
    3. ReturnIfAbrupt(rval).
    4. Return the result of performing BindingInitialization for
       BindingPattern passing rval and undefined as arguments.
---*/
var yield = 4;
var x;

var result;
var vals = [];

result = [ x = yield ] = vals;

assert.sameValue(x, 4);

assert.sameValue(result, vals);

reportCompare(0, 0);
