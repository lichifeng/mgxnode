- write a test: test.js
- write a new version of OO style usage

## usage
```js
parse = require(...);
// use object factory??
var rec = parse("xxx.mgx", {tryfix: true, rawencoding: 'cp936'});
var rec_data = rec.data();
var map_data = rec.map();

// https://stackoverflow.com/a/62708947
var map_file = rec.map("path/to/map.png"); // return what? a path or a file object?
var map_file_hd = rec.map("path/to/map.png", {witdh: 900, height: 450, hd: true});

var [header_file, body_file] = rec.extract({header: 'header.dat', body: 'body.dat'});
```