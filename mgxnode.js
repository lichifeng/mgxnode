var mgxnode = require('bindings')('mgxnode');

var rec = JSON.parse(mgxnode.parse("DE_test.aoe2record"));
console.log(rec); // 'world'