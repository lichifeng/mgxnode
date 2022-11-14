'use strict';

const analyzer = require('./mgxnode');
const { resolve } = require('path');

const absolute_path = resolve('/Workspace/mgxnode/libs/MgxParser/test/test_records/AOC10_4v4_5_5e3b2a7e.mgx');
const rec = new analyzer({
    input: absolute_path,
    map: true,
    hd: true,
    mapWidth: 600,
    mapHeight: 300,
    // header: 'testheader.dat',
    // body: 'testbody.dat',
});

const rec_info = rec.result();
const rec_map_base64 = rec.map_base64();
//rec.savemap('testmap.png');

console.log(rec_info);
console.log(rec_map_base64);